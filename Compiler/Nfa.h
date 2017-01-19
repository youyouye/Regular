#ifndef NFA_H_
#define NFA_H_
#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include "Graph.h"
using namespace std;
/*
*ʵ�����кܶ������˼·.
*����һ�������ʽ.
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
	int stateSum; //״̬��
	vector<shared_ptr<FAEdge>> edges;
	map<pair<int, int>, shared_ptr<FAEdge>> edgeTable; //2-3�߼�2�ڵ�=
	map<pair<int, int>, int> nodeMatrix;
	//��ʼ״̬�ͽ���״̬����.
	int startNum;
	int successNum;
	vector<int> successNumCol;
	map<pair<int, shared_ptr<FAEdge>>, int> vevTabel; //��ߵ��,Ϊhopcroft�㷨����.
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
//�Ӽ����취
shared_ptr<Nfa> generateDFA(shared_ptr<Nfa> nfa);
shared_ptr<vector<int>> generateClosure(int stateNum, shared_ptr<Nfa> nfa);
shared_ptr<Nfa> hopcroft(shared_ptr<Nfa> dfa);
shared_ptr<vector<vector<int>>> hopcroftSplit(vector<int>& sState, vector<vector<int>>& stateCol, shared_ptr<Nfa> dfa);
map<shared_ptr<vector<int>>, int>::iterator findInMap(map<shared_ptr<vector<int>>, int>& mapTest, shared_ptr<vector<int>>& test);
bool equalVector(vector<vector<int>>& p1, vector<vector<int>>& p2);
bool equalVector(vector<string>& p1,vector<string>& p2);
int findInVec(vector<vector<int>>& test,int target); //��vector����һ�����ֿ������ڵڼ���?
shared_ptr<vector<vector<int>>> union2Vec(vector<vector<int>>& p1, vector<vector<int>>& p2); //�󲢼�,�о���ר����һ�麯����.
int findNFMap(map<int, vector<int>>& table,int target);

shared_ptr<Dfa> convertToLink(shared_ptr<Nfa> nfa);
	
#endif
