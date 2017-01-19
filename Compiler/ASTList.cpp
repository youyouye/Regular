#include "ASTList.h"
ASTList::ASTList()
{
}
ASTList::ASTList(vector<shared_ptr<ASTree>> &list)
{
	children = list;
}
ASTList::~ASTList()
{
}
shared_ptr<ASTree> ASTList::child(int i)
{
	return children.at(i);
}
string ASTList::location()
{
	
	for (auto it = children.begin(); it != children.end(); it++)
	{
		shared_ptr<ASTree> temp = *it;
		string s = temp->location();
		if (!s.empty())
		{
			return s;
		}
	}
	
	return nullptr;
}
int ASTList::numChildren()
{
	return children.size();
}
string ASTList::toString()
{
	string s = "";
	s.append("(");
	for (auto it = children.begin(); it != children.end(); it++)
	{
		shared_ptr<ASTree> temp = *it;
		s.append(temp->toString());
	}
	s.append(")");
	
	return s;
}
