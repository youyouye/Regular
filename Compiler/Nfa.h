#ifndef NFA_H_
#define NFA_H_
#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include "Graph.h"
using namespace std;
/*
*实际上有很多种设计思路.
*整体一个表的形式.
*/
class Nfa
{
public:
	Nfa();
	~Nfa();
	void initMatrix();
	int getStateSum();
	void setStartNum(int mStartNum){ startNum = mStartNum; };
	int getStartNum(){ return startNum; }
	void setSuccessNum(int mSuccessNum){ successNum = mSuccessNum; }
	int getSuccessNum(){ return successNum; }
	void setStateSum(int mStateNum){ stateSum = mStateNum; }
	void insertNode(shared_ptr<FANode> node);
	void insertEdge(shared_ptr<FAEdge> edge);
	void insertToNTable(int n1,int n2,int exist);
	void insertToETable(int n1,int n2,shared_ptr<FAEdge> edge);
	map<pair<int, int>, shared_ptr<FAEdge>>& getEdgeTable(){ return edgeTable; }
	map<pair<int, int>, int>& getNodeMatrix(){ return nodeMatrix; }
	vector<shared_ptr<FAEdge>>& getEdges(){ return edges; }
	vector<int>& getSuccessCol(){ return successNumCol; }
	map<pair<int, shared_ptr<FAEdge>>, int>& getVevTable(){ return vevTabel; }
	vector<shared_ptr<FANode>> nodes;
private:	
	int stateSum; //状态数
	vector<shared_ptr<FAEdge>> edges;
	map<pair<int, int>, shared_ptr<FAEdge>> edgeTable; //2-3边加2节点=
	map<pair<int, int>, int> nodeMatrix;
	//初始状态和接受状态集合.
	int startNum;
	int successNum;
	vector<int> successNumCol;
	map<pair<int, shared_ptr<FAEdge>>, int> vevTabel; //点边点表,为hopcroft算法构建.
};
class Dfa
{
public:
	Dfa();
	~Dfa();
	map<int, shared_ptr<X_Status>> status;
	map<int, shared_ptr<X_Edge>> edges;
	map<int, shared_ptr<StringLiteral>> contents;
	int startNum;
	int statusMark;
	int edgeMark;
	int contentMark;
private:

};
//子集构造法
shared_ptr<Nfa> generateDFA(shared_ptr<Nfa> nfa);
shared_ptr<vector<int>> generateClosure(int stateNum, shared_ptr<Nfa> nfa);
shared_ptr<Nfa> hopcroft(shared_ptr<Nfa> dfa);
shared_ptr<vector<vector<int>>> hopcroftSplit(vector<int>& sState, vector<vector<int>>& stateCol, shared_ptr<Nfa> dfa);
map<shared_ptr<vector<int>>, int>::iterator findInMap(map<shared_ptr<vector<int>>, int>& mapTest, shared_ptr<vector<int>>& test);
bool equalVector(vector<vector<int>>& p1, vector<vector<int>>& p2);
bool equalVector(vector<string>& p1,vector<string>& p2);
int findInVec(vector<vector<int>>& test,int target); //在vector中找一个数字看它是在第几个?
shared_ptr<vector<vector<int>>> union2Vec(vector<vector<int>>& p1, vector<vector<int>>& p2); //求并集,感觉能专门做一组函数了.
int findNFMap(map<int, vector<int>>& table,int target);

shared_ptr<Dfa> convertToLink(shared_ptr<Nfa> nfa);
	
#endif
