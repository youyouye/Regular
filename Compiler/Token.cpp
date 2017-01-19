#include "Token.h"

Token::Token()
{
}
//Type:1��char.2�Ǳ�����.3�ǿ��ַ�.
Token::Token(int wordType)
{
	type = wordType;
}
Token::~Token()
{
}

CharToken::CharToken()
{
}
CharToken::CharToken(string ch) :Token(1)
{
	word = ch;
}
CharToken::~CharToken()
{
}
string CharToken::getText()
{
	return word;
}

NumberToken::NumberToken()
{
}

NumberToken::~NumberToken()
{
}


ReservedToken::ReservedToken() :Token(2)
{
}
ReservedToken::ReservedToken(string ch) : Token(2), word(ch)
{
	word = ch;
}
ReservedToken::~ReservedToken()
{
}
string ReservedToken::getText()
{
	return word;
}
