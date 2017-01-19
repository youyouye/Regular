#ifndef LEXER_H_
#define LEXER_H_
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <memory>
using namespace std;
class Token;
class Lexer
{
public:
	Lexer();
	Lexer(string file);
	~Lexer();
	Token* read();
	Token* peek(int i);
	void readLine();
	int numOfToken(){ return tokens.size(); }
	int getMark(){ return mark; }
private:
	vector<Token*> tokens;
	int mark;	//读到了第几个字符
	string filename;
};
#endif