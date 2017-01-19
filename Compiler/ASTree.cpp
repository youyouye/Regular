#include "ASTree.h"
ASTree::ASTree()
{
}

ASTree::~ASTree()
{
}
shared_ptr<ASTree> ASTree::child(int i)
{
	return nullptr;
}
string ASTree::location()
{
	return nullptr;
}
int ASTree::numChildren()
{
	return 0;
}
string ASTree::toString()
{
	return nullptr;
}
shared_ptr<ThompResult> ASTree::thompson(shared_ptr<Nfa> mNfa)
{
	return nullptr;
}
shared_ptr<ThompResult> ASTree::x_thompson(shared_ptr<Dfa> mNfa)
{
	return nullptr;
}
