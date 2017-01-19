#include "ReASTree.h"
#include "Nfa.h"
#include "Graph.h"

CharLiteral::CharLiteral()
{
}
CharLiteral::CharLiteral(Token& t) :ASTLeaf(t)
{
}

CharLiteral::~CharLiteral()
{
}
shared_ptr<ThompResult> CharLiteral::thompson(shared_ptr<Nfa> mNfa)
{
	int stateNum0 = mNfa->getStateSum();
	auto node0 = make_shared<FANode>(0,stateNum0);
	int stateNum1 = stateNum0+1;
	mNfa->setStateSum(stateNum1+1);
	auto node1 = make_shared<FANode>(2,stateNum1);
	mNfa->insertNode(node0); mNfa->insertNode(node1);
	auto edge = make_shared<FAEdge>(vector < string > {token->getChar()});
	mNfa->insertEdge(edge);
	mNfa->insertToNTable(stateNum0,stateNum1,1);
	mNfa->insertToETable(stateNum0,stateNum1,edge);
	mNfa->setStartNum(stateNum0); mNfa->setSuccessNum(stateNum1);
	return make_shared<ThompResult>(stateNum0,stateNum1,stateNum1);
}
shared_ptr<ThompResult> CharLiteral::x_thompson(shared_ptr<Dfa> mNfa)
{
	int statusNum = mNfa->statusMark;
	int edgeNum = mNfa->edgeMark;
	int contentNum = mNfa->contentMark;
	auto status1 = make_shared<X_Status>(); status1->setType(0);
	status1->outEdges.push_back(edgeNum);
	mNfa->status.insert({statusNum,status1});
	mNfa->statusMark++;
	auto status2 = make_shared<X_Status>(); status2->setType(2);
	status2->inEdges.push_back(edgeNum);
	mNfa->status.insert({statusNum+1,status2});
	mNfa->statusMark++;
	auto edge = make_shared<X_Edge>(statusNum, statusNum + 1,1); edge->setMatchContent(contentNum);
	mNfa->edges.insert({edgeNum,edge});
	auto content = make_shared<StringLiteral>(token->getChar(),1);
	mNfa->contents.insert({ statusNum, content});
	mNfa->contentMark++;mNfa->edgeMark++;
	//设置开始点.
	mNfa->startNum = statusNum;
	return make_shared<ThompResult>(statusNum,statusNum+1,statusNum+1);
}
CharCollection::CharCollection()
{
}
CharCollection::CharCollection(Token& t1, Token& t2) :start(&t1), end(&t2)
{
}

CharCollection::~CharCollection()
{
}

string CharCollection::toString()
{
	string s = "";
	s.append("(");
	s.append(start->getChar()+"-"+end->getChar());
	s.append(")");
	return s;
}
shared_ptr<ThompResult> CharCollection::thompson(shared_ptr<Nfa> mNfa)
{
	//暂时没想好
	return nullptr;
}
shared_ptr<ThompResult> CharCollection::x_thompson(shared_ptr<Dfa> mNfa)
{
	return nullptr;
}

SelectStmnt::SelectStmnt()
{
}
SelectStmnt::SelectStmnt(vector<shared_ptr<ASTree>> t) :ASTList(t)
{
}
SelectStmnt::~SelectStmnt()
{
}
string SelectStmnt::toString()
{
	string s = "";
	s.append("(");
	int mark = 0;
	for (auto it = children.begin(); it != children.end(); it++)
	{
		shared_ptr<ASTree> temp = *it;
		s.append(temp->toString());
		if (mark == 0)
		{
			s.append("|");
		}
		mark = 1;
	}
	s.append(")");

	return s;
}
shared_ptr<ThompResult> SelectStmnt::thompson(shared_ptr<Nfa> mNfa)
{
	auto result1 = children.at(0)->thompson(mNfa);
	auto result2 = children.at(1)->thompson(mNfa);
	//需要构建两个节点,4条边.
	int stateNum0 = mNfa->getStateSum();
	auto node0 = make_shared<FANode>(0, stateNum0);
	int stateNum1 = stateNum0 + 1;
	mNfa->setStateSum(stateNum1+1);
	auto node1 = make_shared<FANode>(2, stateNum1);
	mNfa->insertNode(node0); mNfa->insertNode(node1);
	auto edge1 = make_shared<FAEdge>(vector < string > {""});
	auto edge2 = make_shared<FAEdge>(vector < string > {""});
	auto edge3 = make_shared<FAEdge>(vector < string > {""});
	auto edge4 = make_shared<FAEdge>(vector < string > {""});
	mNfa->insertEdge(edge1); mNfa->insertEdge(edge2); mNfa->insertEdge(edge3);
	mNfa->insertEdge(edge4);
	mNfa->insertToNTable(stateNum0,result1->start,1);
	mNfa->insertToNTable(stateNum0,result2->start,1);
	mNfa->insertToNTable(result1->end,stateNum1,1);
	mNfa->insertToNTable(result2->end,stateNum1,1);
	mNfa->insertToETable(stateNum0, result1->start,edge1);
	mNfa->insertToETable(stateNum0, result2->start, edge2);
	mNfa->insertToETable(result1->end, stateNum1, edge3);
	mNfa->insertToETable(result2->end, stateNum1, edge4);
	//修改接受节点,初始节点
	mNfa->nodes.at(result1->success)->setStateNum(1);
	mNfa->nodes.at(result2->success)->setStateNum(1);
	mNfa->nodes.at(result1->start)->setStateNum(1);
	mNfa->nodes.at(result2->start)->setStateNum(1);
	mNfa->setStartNum(stateNum0); mNfa->setSuccessNum(stateNum1);
	return make_shared<ThompResult>(stateNum0,stateNum1,stateNum1);
}
shared_ptr<ThompResult> SelectStmnt::x_thompson(shared_ptr<Dfa> mNfa)
{
	auto result1 = children.at(0)->x_thompson(mNfa);
	auto result2 = children.at(1)->x_thompson(mNfa);
	int statusNum = mNfa->statusMark;
	int edgeNum = mNfa->edgeMark;
	auto status1 = make_shared<X_Status>(); status1->setType(0);
	status1->outEdges.push_back(edgeNum); status1->outEdges.push_back(edgeNum+1);
	mNfa->status.insert({ statusNum, status1 });
	mNfa->statusMark++;
	auto status2 = make_shared<X_Status>(); status2->setType(2);
	status2->inEdges.push_back(edgeNum + 2); status2->inEdges.push_back(edgeNum+2);
	mNfa->status.insert({ statusNum + 1, status2 });
	mNfa->statusMark++;
	auto edge1 = make_shared<X_Edge>(statusNum,result1->start, 2); 
	auto edge2 = make_shared<X_Edge>(statusNum, result2->start, 2);
	auto edge3 = make_shared<X_Edge>(result1->end,statusNum+1,2);
	auto edge4 = make_shared<X_Edge>(result2->end,statusNum+1,2);
	mNfa->edges.insert({ edgeNum, edge1 }); mNfa->edges.insert({edgeNum+1,edge2});
	mNfa->edges.insert({ edgeNum + 2, edge3 }); mNfa->edges.insert({edgeNum+3,edge4});
	mNfa->edgeMark = edgeNum + 4;
	//修改已有节点.
	auto r1 = mNfa->status.at(result1->start); r1->inEdges.push_back(edgeNum); r1->setType(1);
	auto r2 = mNfa->status.at(result1->end); r2->outEdges.push_back(edgeNum + 2); r2->setType(1);
	auto r3 = mNfa->status.at(result2->start); r3->inEdges.push_back(edgeNum + 1); r3->setType(1);
	auto r4 = mNfa->status.at(result2->end); r4->outEdges.push_back(edgeNum + 3); r4->setType(1);
	//设置开始点.
	mNfa->startNum = statusNum;
	return make_shared<ThompResult>(statusNum, statusNum + 1, statusNum+1);
}

LinkStmnt::LinkStmnt()
{
}

LinkStmnt::LinkStmnt(vector<shared_ptr<ASTree>> t) :ASTList(t)
{
}

LinkStmnt::~LinkStmnt()
{
}
shared_ptr<ThompResult> LinkStmnt::thompson(shared_ptr<Nfa> mNfa)
{	
	auto result1 = children.at(0)->thompson(mNfa);
	auto result2 = children.at(1)->thompson(mNfa);
	auto edge = make_shared<FAEdge>(vector < string > {""});
	mNfa->insertEdge(edge);
	mNfa->insertToNTable(result1->end,result2->start,1);
	mNfa->insertToETable(result1->end,result2->start,edge);
	//改变状态,需要传递结束状态.
	mNfa->nodes.at(result1->success)->setStateNum(1);
	mNfa->nodes.at(result2->start)->setStateNum(1);
	mNfa->setStartNum(result1->start); mNfa->setSuccessNum(result2->success);
	return make_shared<ThompResult>(result1->start,result2->end,result2->end);
}
shared_ptr<ThompResult> LinkStmnt::x_thompson(shared_ptr<Dfa> mNfa)
{
	auto result1 = children.at(0)->x_thompson(mNfa);
	auto result2 = children.at(1)->x_thompson(mNfa);
	int edgeNum = mNfa->edgeMark;
	//真是没啥用的代码..
	auto edge = make_shared<X_Edge>(result1->end, result2->start,2);
	mNfa->edges.insert({ edgeNum, edge }); mNfa->edgeMark++;
	auto status1 = mNfa->status.at(result1->end);  status1->outEdges.push_back(edgeNum);
	status1->setType(1);
	auto status2 = mNfa->status.at(result2->start); status2->inEdges.push_back(edgeNum);
	status2->setType(1);
	return make_shared<ThompResult>(result1->start, result2->end, result2->end);
}

ClosureStmnt::ClosureStmnt()
{
}
ClosureStmnt::ClosureStmnt(vector<shared_ptr<ASTree>> t, int mType, bool mGreedy) :ASTList(t), type(mType), greedy(mGreedy)
{
}

ClosureStmnt::~ClosureStmnt()
{
}
string ClosureStmnt::toString()
{
	string s = "";
	s.append("(");
	for (auto it = children.begin(); it != children.end(); it++)
	{
		shared_ptr<ASTree> temp = *it;
		s.append(temp->toString());
	}
	s.append("*");
	s.append(")");
	return s;
}
shared_ptr<ThompResult> ClosureStmnt::thompson(shared_ptr<Nfa> mNfa)
{
	auto result = children.at(0)->thompson(mNfa);
	int stateNum0 = mNfa->getStateSum();
	auto node0 = make_shared<FANode>(1, stateNum0);
	int stateNum1 = stateNum0 + 1;
	mNfa->setStateSum(stateNum1+1);
	auto node1 = make_shared<FANode>(2, stateNum1);
	mNfa->insertNode(node0); mNfa->insertNode(node1);
	auto edge1 = make_shared<FAEdge>(vector < string > {""});
	auto edge2 = make_shared<FAEdge>(vector < string > {""});
	auto edge3 = make_shared<FAEdge>(vector < string > {""});
	auto edge4 = make_shared<FAEdge>(vector < string > {""});
	mNfa->insertEdge(edge1); mNfa->insertEdge(edge2); mNfa->insertEdge(edge3);
	mNfa->insertEdge(edge4);
	mNfa->insertToNTable(stateNum0,result->start,1);
	mNfa->insertToNTable(stateNum0,stateNum1,1);
	mNfa->insertToNTable(result->end,stateNum1,1);
	mNfa->insertToNTable(result->end,result->start,1);
	mNfa->insertToETable(stateNum0, result->start,edge1);
	mNfa->insertToETable(stateNum0, stateNum1, edge2);
	mNfa->insertToETable(result->end, stateNum1, edge3);
	mNfa->insertToETable(result->end, result->start, edge4);
	//修改接受状态
	mNfa->nodes.at(result->success)->setStateNum(1);
	mNfa->nodes.at(result->start)->setStateNum(1);
	mNfa->setStartNum(stateNum0); mNfa->setSuccessNum(stateNum1);
	return make_shared<ThompResult>(stateNum0,stateNum1,stateNum1);
}
shared_ptr<ThompResult> ClosureStmnt::x_thompson(shared_ptr<Dfa> mNfa)
{
	auto result = children.at(0)->x_thompson(mNfa);
	int statusNum = mNfa->statusMark;
	int edgeNum = mNfa->edgeMark;
	//也就是有6种.*,+,?.*?,+?,??
	if (greedy)
	{
		if (type == 1)
		{
			auto status1 = make_shared<X_Status>(); status1->setType(4);
			status1->outEdges.push_back(edgeNum); status1->inEdges.push_back(edgeNum+1);
			mNfa->status.insert({statusNum,status1});  mNfa->statusMark++;
			auto edge1 = make_shared<X_Edge>(statusNum,result->start,2);
			auto edge2 = make_shared<X_Edge>(result->end,statusNum,2);
			mNfa->edges.insert({ edgeNum, edge1 }); mNfa->edges.insert({edgeNum+1,edge2});
			mNfa->edgeMark = edgeNum + 2;
			auto r1 = mNfa->status.at(result->start); r1->inEdges.push_back(edgeNum); r1->setType(1);
			auto r2 = mNfa->status.at(result->end); r2->outEdges.push_back(edgeNum + 1); r2->setType(1);
			mNfa->startNum = statusNum;
			return make_shared<ThompResult>(statusNum, statusNum, statusNum);
		}
		else if (type == 2)
		{
			
		}
		else if (type == 3)
		{

		}
	}
	else
	{
		if (type == 1)
		{

		}
		else if (type == 2)
		{

		}
		else if (type == 3)
		{

		}
	}
	return nullptr;
}

RepeatStmnt::RepeatStmnt()
{

}
RepeatStmnt::RepeatStmnt(vector<shared_ptr<ASTree>> t, int mstart, int mend, bool mGreedy) :ASTList(t), start(mstart), end(mend), greedy(mGreedy)
{
}
RepeatStmnt::~RepeatStmnt()
{
}
shared_ptr<ThompResult> RepeatStmnt::x_thompson(shared_ptr<Dfa> mNfa)
{
	return nullptr;
}

ForwardLook::ForwardLook()
{
}
ForwardLook::ForwardLook(vector<shared_ptr<ASTree>> t) :ASTList(t)
{
}

ForwardLook::~ForwardLook()
{
}
shared_ptr<ThompResult> ForwardLook::x_thompson(shared_ptr<Dfa> mNfa)
{
	return nullptr;
}

ReverseLook::ReverseLook()
{
}
ReverseLook::ReverseLook(vector<shared_ptr<ASTree>> t) :ASTList(t)
{
	
}
ReverseLook::~ReverseLook()
{
}
shared_ptr<ThompResult> ReverseLook::x_thompson(shared_ptr<Dfa> mNfa)
{
	return nullptr;
}

NameCatch::NameCatch()
{
}
NameCatch::NameCatch(vector<shared_ptr<ASTree>> t, string mname) :ASTList(t), name(mname)
{
}
NameCatch::~NameCatch()
{
}
shared_ptr<ThompResult> NameCatch::x_thompson(shared_ptr<Dfa> mNfa)
{
	return nullptr;
}

NameCheck::NameCheck()
{
}
NameCheck::NameCheck(vector<shared_ptr<ASTree>> t) :ASTList(t)
{

}
NameCheck::NameCheck(string mname)
{
	name = mname;
}
NameCheck::~NameCheck()
{
}
shared_ptr<ThompResult> NameCheck::x_thompson(shared_ptr<Dfa> mNfa)
{
	return nullptr;
}

AnonymityCatch::AnonymityCatch()
{
}

AnonymityCatch::AnonymityCatch(vector<shared_ptr<ASTree>> t) :ASTList(t)
{

}

AnonymityCatch::~AnonymityCatch()
{
}

shared_ptr<ThompResult> AnonymityCatch::x_thompson(shared_ptr<Dfa> mNfa)
{
	return nullptr;
}
