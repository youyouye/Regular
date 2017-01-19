#include "ASTLeaf.h"

ASTLeaf::ASTLeaf()
{
}
ASTLeaf::ASTLeaf(Token& t)
{
	token = &t;
}
ASTLeaf::~ASTLeaf()
{
}

string ASTLeaf::location()
{
	return string("at line" + token->getLineRank());
}
Token* ASTLeaf::getToken()
{
	return token;
}
int ASTLeaf::numChildren()
{
	return 0;
}
string ASTLeaf::toString()
{
	return token->getText();
}
