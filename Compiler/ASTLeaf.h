#ifndef ASTLEAFH_
#define ASTLEAF_H_
#include "Token.h"
#include "ASTree.h"
class ASTLeaf : public ASTree
{
public:
	ASTLeaf();
	ASTLeaf(Token& t);
	~ASTLeaf();
	//我不知道C++该怎样写
	//ASTree child(int i);
	string location();
	Token* getToken();
	int numChildren();
	string toString();
protected:
	Token* token;
private:

};



#endif