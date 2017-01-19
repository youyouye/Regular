#ifndef TOKEN_H_
#define TOKEN_H_
#include <string>
using namespace std;
class Token
{
public:
	Token();
	Token(int wordType);
	virtual ~Token();
	int getType(){ return type; }
	void setType(int tokenType){ type = tokenType; }
	virtual int getNumber() { return -1; };
	virtual string getChar() { return nullptr; };
	virtual string getText() { return nullptr; };
	virtual int getLineRank(){ return lineRank; };
private:
	int type;//¥ ∑®¿‡–Õ.
	int lineRank;
};

class CharToken : public Token
{
public:
	CharToken();
	CharToken(string ch);
	~CharToken();
	string getChar(){ return word; }
	string getText();
private:
	string word;
};
class NumberToken : public Token
{
public:
	NumberToken();
	~NumberToken();
	int getNumber(){ return number; }
private:
	int number;
};

class ReservedToken : public Token
{
public:
	ReservedToken();
	ReservedToken(string ch);
	~ReservedToken();
	string getChar(){ return word; }
	string getText();
private:
	string word;
};

#endif