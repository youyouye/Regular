#pragma comment(lib,"libglog.lib")
#include "Token.h"
#include "Lexer.h"
#include "Nfa.h"
#include "glog/logging.h"
using namespace std;

Lexer::Lexer() :mark(0)
{
}
Lexer::Lexer(string file)
{
	filename = file;
}
Lexer::~Lexer()
{
}
Token* Lexer::read()
{
	if (tokens.size() == 0)
	{
		readLine();
	}
	int temp = mark;
	if (mark == tokens.size())
	{
		return nullptr;
	}
	mark++;
	return tokens.at(temp);
}
Token* Lexer::peek(int i)
{
	if (tokens.size() == 0)
	{
		readLine();
	}
	if (mark+i>=tokens.size())
	{
		return nullptr;
	}
	return tokens.at(mark+i);
}
void Lexer::readLine()
{
	ifstream in("D:\\vs2015\\MyCode\\Compiler\\Compiler\\test.txt");
	if (in)
	{
		char temp;
		while ((temp = in.get()) != EOF)
		{
			stringstream stream; stream << temp;
			string stemp = stream.str();
			if ((temp == '(') || (temp == ')') || (temp=='|') ||(temp=='-'))
			{
				tokens.push_back(new ReservedToken(stemp));
			}
			else
			{
				tokens.push_back(new CharToken(stemp));
			}
		}
	}
	in.close();
}


/*
void main(int argc,char* argv[])
{
	google::InitGoogleLogging(argv[0]);
	google::SetLogDestination(google::INFO,"D:\\vs2015\\MyCode\\Compiler");
	//LOG(WARNING) << t1.getChar() << endl;
	auto lexer =Lexer();
	lexer.readLine();
	Token* t1 = lexer.read();
	cout << t1->getChar() << endl;
	Token* t2 = lexer.peek(1);
	cout << t2->getChar() << endl;
}
*/





