#ifndef REPARSER_H_
#define REPARSER_H_
#include <memory>
#include <string>
using namespace std;
class Lexer;
class ASTree;
class ReParser
{
public:
	ReParser();
	ReParser(Lexer& l){ lexer = &l; }
	~ReParser();

	/*
	re:select
	re_base:char | char_range | '('re')'
	select:select_base select_end
	select_base=link
	select_end='|' select_base select_end
	link:link_base link_end
	link_base:re_base|closure
	link_end:link_base link_end
	closure:re_base '*'
	*/

	shared_ptr<ASTree> re();
	shared_ptr<ASTree> re_base();
	shared_ptr<ASTree> char_range();
	shared_ptr<ASTree> select();
	shared_ptr<ASTree> select_base();

	shared_ptr<ASTree> select_end();
	shared_ptr<ASTree> link();
	shared_ptr<ASTree> link_base();
	shared_ptr<ASTree> link_end();
	shared_ptr<ASTree> closure(shared_ptr<ASTree> t1);
	//�����ӹ����﷨.
	shared_ptr<ASTree> function(int select);
	//����Ԥ��
	shared_ptr<ASTree> forwardLook();
	//����Ԥ��
	shared_ptr<ASTree> reverseLook();
	//��������
	shared_ptr<ASTree> nameCatch();
	//�������
	shared_ptr<ASTree> nameCheck();
	//��������
	shared_ptr<ASTree> anonymityCatch();
	void token(char ch);
	bool isToken(char ch);
	bool isTokenN(char ch, int n);
	bool isTokenChar();
	bool isMatchC();	//����Ƿ�ƥ��()*���ָ�ʽ��.
	int detectFun();
	string nameR();
	int numberR();
private:
	Lexer* lexer;
};



#endif
