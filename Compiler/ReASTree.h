#ifndef CHARLITERAL_H_
#define CHARLITERAL_H_
#include <memory>
#include "ASTLeaf.h"
#include "ASTList.h"
class Nfa;
class Dfa;
using namespace std;
class CharLiteral:public ASTLeaf 
{
public:
	CharLiteral();
	CharLiteral(Token& t);
	~CharLiteral();
	shared_ptr<ThompResult> thompson(shared_ptr<Nfa> mNfa);
	shared_ptr<ThompResult> x_thompson(shared_ptr<Dfa> mNfa);
private:
};

class CharCollection :public ASTLeaf
{
public:
	CharCollection();
	CharCollection(Token& t1,Token& t2);
	~CharCollection();
	string toString();
	shared_ptr<ThompResult> thompson(shared_ptr<Nfa> mNfa);
	shared_ptr<ThompResult> x_thompson(shared_ptr<Dfa> mNfa);
private:
	Token* start;
	Token* end;
};

class SelectStmnt :public ASTList 
{
public:
	SelectStmnt();
	SelectStmnt(vector<shared_ptr<ASTree>> t);
	~SelectStmnt();
	string toString();
	shared_ptr<ThompResult> thompson(shared_ptr<Nfa> mNfa);
	shared_ptr<ThompResult> x_thompson(shared_ptr<Dfa> mNfa);
private:

};

class LinkStmnt : public ASTList
{
public:
	LinkStmnt();
	LinkStmnt(vector<shared_ptr<ASTree>> t);
	~LinkStmnt();
	shared_ptr<ThompResult> thompson(shared_ptr<Nfa> mNfa);
	shared_ptr<ThompResult> x_thompson(shared_ptr<Dfa> mNfa);
private:

};

class ClosureStmnt : public ASTList
{
public:
	ClosureStmnt();
	ClosureStmnt(vector<shared_ptr<ASTree>> t,int mType,bool mGreedy);
	~ClosureStmnt();
	string toString();
	shared_ptr<ThompResult> thompson(shared_ptr<Nfa> mNfa);
	shared_ptr<ThompResult> x_thompson(shared_ptr<Dfa> mNfa);
	int type; // 1:* 2:+ 3:?
	bool greedy;
private:
};

class RepeatStmnt : public ASTList{
public:
	RepeatStmnt();
	RepeatStmnt(vector<shared_ptr<ASTree>> t, int start,int end,bool mGreedy);
	~RepeatStmnt();
	shared_ptr<ThompResult> x_thompson(shared_ptr<Dfa> mNfa);
	int start;
	int end;
	bool greedy;
};

class ForwardLook : public ASTList
{ 
public:
	ForwardLook();
	ForwardLook(vector<shared_ptr<ASTree>> t);
	~ForwardLook();
	shared_ptr<ThompResult> x_thompson(shared_ptr<Dfa> mNfa);
private:
};
class ReverseLook : public ASTList  
{
public:
	ReverseLook ();
	ReverseLook(vector<shared_ptr<ASTree>> t);
	~ReverseLook ();
	shared_ptr<ThompResult> x_thompson(shared_ptr<Dfa> mNfa);
private:

};

class NameCatch : public ASTList
{
public:
	NameCatch();
	NameCatch(vector<shared_ptr<ASTree>> t,string mname);
	~NameCatch();
	shared_ptr<ThompResult> x_thompson(shared_ptr<Dfa> mNfa);
	string name;
private:
};

class NameCheck : public ASTList
{
public:
	NameCheck();
	NameCheck(vector<shared_ptr<ASTree>> t);
	NameCheck(string mname);
	~NameCheck();
	shared_ptr<ThompResult> x_thompson(shared_ptr<Dfa> mNfa);
	string name;
private:

};

class AnonymityCatch : public ASTList
{
public:
	AnonymityCatch();
	AnonymityCatch(vector<shared_ptr<ASTree>> t);
	~AnonymityCatch();
	shared_ptr<ThompResult> x_thompson(shared_ptr<Dfa> mNfa);
private:

};


#endif