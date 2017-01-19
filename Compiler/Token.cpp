#include "Token.h"

Token::Token()
{
}
//Type:1是char.2是保留字.3是空字符.
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
