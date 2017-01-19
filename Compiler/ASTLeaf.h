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
	//�Ҳ�֪��C++������д
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