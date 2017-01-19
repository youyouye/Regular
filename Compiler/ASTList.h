#ifndef ASTLIST_H_
#define ASTLIST_H_
#include <vector>
#include <memory>
#include "ASTree.h"
using namespace std;
class ASTList : public ASTree 
{
public:
	ASTList();
	ASTList(vector<shared_ptr<ASTree>> &list);
	~ASTList();

	shared_ptr<ASTree> child(int i);
	string location();
	int numChildren();
	string toString();
protected:
	vector<shared_ptr<ASTree>> children;
private:

};

#endif