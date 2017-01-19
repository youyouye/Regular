#include"ReParser.h"
#include "Lexer.h"
#include "Token.h"
#include "ASTree.h"
#include "ReASTree.h"
ReParser::ReParser()
{
}

ReParser::~ReParser()
{
}

shared_ptr<ASTree> ReParser::re()
{
	return select();
}
shared_ptr<ASTree> ReParser::re_base()
{
	if (isToken('('))
	{
		token('(');
		shared_ptr<ASTree> t = re();
		token(')');
		return t;
	}
	else
	{
		return char_range();
	}
}
shared_ptr<ASTree> ReParser::char_range()
{
	Token* t = lexer->peek(0);
	if (t == nullptr)
	{
		return nullptr;
	}
	else if (t->getType() == 2)
	{
		return nullptr;
	}
	else if (t->getType() == 1)
	{
		Token*  t1 = lexer->read();
		if (isToken('-'))
		{
			token('-');
			if (isTokenChar())
			{
				Token* last = lexer->read();
				return make_shared<CharCollection>(*t1, *last);
			}
			else
			{
				throw runtime_error("读取不符合规则");
			}
		}
		else
		{
			return make_shared<CharLiteral>(*t1);
		}
	}
}
shared_ptr<ASTree> ReParser::select()
{
	shared_ptr<ASTree> t1 = select_base();
	if (t1 == nullptr)
	{
		return nullptr;
	}
	shared_ptr<ASTree> t2 = select_end();
	if (t2 == nullptr)
	{
		return t1;
	}
	return make_shared<SelectStmnt>(vector<shared_ptr<ASTree>>{t1, t2});
}
shared_ptr<ASTree> ReParser::select_base()
{
	return link();
}


shared_ptr<ASTree> ReParser::select_end()
{
	if (isToken('|'))
	{
		token('|');
		shared_ptr<ASTree> t1 = select_base();
		if (t1 == nullptr)
		{
			return nullptr;
		}
		shared_ptr<ASTree> t2 = select_end();
		if (t2 == nullptr)
		{
			return t1;
		}
		return make_shared<SelectStmnt>(vector<shared_ptr<ASTree>>{t1, t2});
	}
	else
	{
		return nullptr;
	}
}
shared_ptr<ASTree> ReParser::link()
{
	shared_ptr<ASTree> t1 = link_base();
	if (t1 == nullptr)
	{
		return nullptr;
	}
	shared_ptr<ASTree> t2 = link_end();
	if (t2 == nullptr)
	{
		return t1;
	}
	return make_shared<LinkStmnt>(vector<shared_ptr<ASTree>> {t1, t2});
}

shared_ptr<ASTree> ReParser::link_base()
{
	/*
	if (isMatchC())
	{
		shared_ptr<ASTree> t3 = closure();
		return t3;
	}
	if (isTokenN('*',1)){
		shared_ptr<ASTree> t2 = closure();
		return t2;
	}else
	{
		shared_ptr<ASTree> t1 = re_base();
		return t1;
	}
	*/
	//增加功能,消除回溯.
	int select = detectFun();
	if (select == -1)
	{
		return nullptr;
	}
	else if (select != 0){
		return function(select);
	}
	else
	{
		shared_ptr<ASTree> t1 = re_base();
		shared_ptr<ASTree> t2 = closure(t1);
		return t2;
	}
}


shared_ptr<ASTree> ReParser::link_end()
{
	shared_ptr<ASTree> t1 = link_base();
	if (t1 == nullptr)
	{
		return nullptr;
	}
	shared_ptr<ASTree> t2 = link_end();
	if (t2 == nullptr)
	{
		return t1;
	}
	return make_shared<LinkStmnt>(vector <shared_ptr<ASTree>> {t1, t2});
}

shared_ptr<ASTree> ReParser::closure(shared_ptr<ASTree> t1)
{
	/*
	shared_ptr<ASTree> t1 = re_base();
	if (isToken('*'))
	{
		token('*');
		vector<shared_ptr<ASTree>> temp{t1};
		return make_shared<ClosureStmnt>(temp);
	}
	else
	{
		return t1;
	}
	*/
	if (isToken('*'))
	{
		token('*');
		if (isToken('?'))
		{
			token('?');
			return make_shared<ClosureStmnt>(vector<shared_ptr<ASTree>> {t1}, 1, false);
		}
		else
		{
			return make_shared<ClosureStmnt>(vector<shared_ptr<ASTree>> {t1}, 1, true);
		}
	}
	else if (isToken('+')){
		token('+');
		if (isToken('?'))
		{
			token('?');
			return make_shared<ClosureStmnt>(vector<shared_ptr<ASTree>> {t1}, 2, false);
		}
		else
		{
			return make_shared<ClosureStmnt>(vector<shared_ptr<ASTree>> {t1}, 2, true);
		}
	}
	else if (isToken('?'))
	{
		token('?');
		if (isToken('?'))
		{
			token('?');
			return make_shared<ClosureStmnt>(vector<shared_ptr<ASTree>> {t1}, 3, false);
		}
		else
		{
			return make_shared<ClosureStmnt>(vector<shared_ptr<ASTree>> {t1}, 3, true);
		}
	}
	else if (isToken('{'))
	{
		token('{');
		int start = numberR();
		token(',');
		int end = numberR();
		token('}');
		if (isToken('?'))
		{
			token('?');
			return make_shared<RepeatStmnt>(vector<shared_ptr<ASTree>> {t1}, start, end, false);
		}
		else
		{
			return make_shared<RepeatStmnt>(vector<shared_ptr<ASTree>> {t1}, start, end, true);
		}
	}
	else
	{
		return t1;
	}
}

shared_ptr<ASTree> ReParser::function(int select)
{
	switch (select)
	{
	case 0:
		return nullptr;
	case 1:  //正向预查
		return forwardLook();
	case 2:  //反向预查
		return reverseLook();
	case 3:  //命名捕获
		return nameCatch();
	case 4:  //命名检查
		return nameCheck();
	case 5:  //匿名捕获
		return anonymityCatch();
	case -1: //语法错误
		throw runtime_error("功能语法错误");
	default:
		break;
	}
}
//正向预查
shared_ptr<ASTree> ReParser::forwardLook()
{
	token('?');
	token('=');
	auto t = re();
	return make_shared<ForwardLook>(vector<shared_ptr<ASTree>> {t});
}
//反向预查
shared_ptr<ASTree> ReParser::reverseLook()
{
	token('?');
	token('!');
	auto t = re();
	return make_shared<ReverseLook>(vector<shared_ptr<ASTree>> {t});
}
//命名捕获
shared_ptr<ASTree> ReParser::nameCatch()
{
	token('?');token(':');token('<');token('#');
	string name = nameR();
	token('>');
	auto t = re();
	return make_shared<NameCatch>(vector<shared_ptr<ASTree>> {t},name);
}
//命名检查
shared_ptr<ASTree> ReParser::nameCheck()
{
	token('?'); token(':'); token('<'); token('$');
	string name = nameR();
	token('>');
	return make_shared<NameCheck>(name);
}
//匿名捕获
shared_ptr<ASTree> ReParser::anonymityCatch()
{
	token('?');
	auto t = re();
	return make_shared<AnonymityCatch>(vector<shared_ptr<ASTree>> {t});
}

void ReParser::token(char ch)
{
	Token* t = lexer->read();
	if (!(ch == *t->getChar().c_str()))
	{
		throw runtime_error("字符不匹配");
	}
}
//预读n个判断
bool ReParser::isTokenN(char ch, int n)
{
	Token* t = lexer->peek(n);
	if (t == nullptr || *t->getChar().c_str() != ch)
	{
		return false;
	}
	else
	{
		return true;
	}

}

bool ReParser::isToken(char ch)
{
	Token* t = lexer->peek(0);
	if ((t != nullptr) &&(*t->getChar().c_str() == ch))
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool ReParser::isTokenChar()
{
	Token* t = lexer->peek(0);
	if (t == nullptr)
	{
		return false;
	}
	if (*(t->getChar()).c_str() >= 48 || *(t->getChar()).c_str() <= 57 || *(t->getChar()).c_str() >= 97 || *(t->getChar()).c_str() <= 122 || *(t->getChar()).c_str() >= 65 || *(t->getChar()).c_str()<=90)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool ReParser::isMatchC()
{
	if (!isToken('('))
	{
		return false;
	}
	int now = lexer->getMark();
	int end = lexer->numOfToken() - now;
	for (int i = 1; i < end; i++)
	{
		Token* t = lexer->peek(i);
		if (t == nullptr)
		{
			return false;
		}
		if (*t->getChar().c_str() == ')')
		{
			Token* t1 = lexer->peek(i + 1);
			if (t1 == nullptr)
			{
				return false;
			}
			if (t1 != nullptr || *t1->getChar().c_str() != '*')
			{
				return true;
			}
		}
	}
	return false;
}
int ReParser::detectFun()
{
	if (isToken('?'))
	{
		if (isTokenN('=',1))
		{
			return 1; //正向预查
		}
		else if (isTokenN('!',1)){ 
			return 2; //反向预查
		}
		else if (isTokenN(':',1)){
			if (isTokenN('<',2) && isTokenN('#',3))
			{
				return 3; //命名捕获
			}
			else if (isTokenN('<',2) && isToken('$'),3){
				return 4; //命名检查
			}
			else
			{
				return -1; //不符合语法
			}
		}
		else
		{
			return 5;//匿名捕获
		}
	}
	return 0; //不存在功能
}
string ReParser::nameR()
{
	string name;
	Token* t = lexer->peek(0);
	while((*t->getChar().c_str() >= 65 && *t->getChar().c_str()<=90)
		|| (*t->getChar().c_str() >= 97 && *t->getChar().c_str() <= 122) || *t->getChar().c_str()==95)
	{
		name.append(t->getChar());
		lexer->read();
		t = lexer->peek(0);
	}
	return name;
}
int ReParser::numberR()
{
	int result = 0;
	Token* t = lexer->peek(0);
	while (*t->getChar().c_str() >=48 && *t->getChar().c_str() <=57)
	{
		result = result*10 + (int)*t->getChar().c_str();
		lexer->read();
		t = lexer->peek(0);
	}
	return result;
}
