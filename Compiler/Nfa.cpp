#include "Nfa.h"
#include <stack>
#include <queue>
Nfa::Nfa() :stateSum(0)
{
}

Nfa::~Nfa()
{
}
void Nfa::initMatrix()
{
}
int Nfa::getStateSum()
{
	return stateSum;
}
void Nfa::insertNode(shared_ptr<FANode> node)
{
	nodes.push_back(node);
}
void Nfa::insertEdge(shared_ptr<FAEdge> edge)
{
	edges.push_back(edge);
}
void Nfa::insertToNTable(int n1, int n2, int exist)
{
	nodeMatrix.insert({ { n1, n2 }, exist });
}
void Nfa::insertToETable(int n1, int n2, shared_ptr<FAEdge> edge)
{
	edge->setFromTo(n1,n2);
	edgeTable.insert({ {n1,n2},edge });
}

shared_ptr<Nfa> generateDFA(shared_ptr<Nfa> nfa)
{
	vector<int> start;
	vector<shared_ptr<vector<int>>> totalState;
	queue<shared_ptr<vector<int>>> workList;
	map<shared_ptr<vector<int>>, int> markTable;	//状态合集及其编号..
	//T[q,c] <- t;
	map<pair<shared_ptr<vector<int>>, shared_ptr<FAEdge>>, shared_ptr<vector<int>>> table;
	auto startSC = generateClosure(nfa->getStartNum(),nfa);
	totalState.push_back(startSC);
	workList.push(startSC);
	while (!workList.empty())
	{
		auto q = workList.front();
		workList.pop();
		for (auto it = nfa->getEdges().begin(); it != nfa->getEdges().end(); it++)
		{
			//对于每一条边.找到能够的转移
			int toState = -1;
			if ((*it)->getContent().front() != "")
			{
				for (auto i = q->begin(); i != q->end(); i++)
				{
					if (*i == (*it)->getFromState())
					{
						toState = (*it)->getToState();
						auto temp = generateClosure(toState, nfa);
						table.insert({ {q,*it},temp});
						bool findResult = false;
						//这里需要更高级的find用法
						for (auto l = totalState.begin(); l !=totalState.end(); l++)
						{
							if ((*l)->size() == temp->size())
							{
								findResult = true;
								for (auto ll = (*l)->begin(),lt=temp->begin(); ll != (*l)->end();ll++,lt++)
								{
									if (*ll != *lt)
									{
										findResult = false;
										break;
									}
								}
								if (findResult == true)
								{
									break;
								}
							}
						}
						if (!findResult)
						{
							totalState.push_back(temp);
							workList.push(temp);
						}
						break;
					}
				}
			}
		}	
	}
	//到此为止就可以开始由Q,T到DFA了.
	auto dfa = make_shared<Nfa>();
	for (auto it = totalState.begin(); it != totalState.end(); it++)
	{
		//从中找到初始状态
		if ((*it)->size() == 1 && (nfa->nodes.at((*it)->front())->getType() == 0))
		{
			int stateNum0 = dfa->getStateSum();
			auto node0 = make_shared<FANode>(0, stateNum0);
			markTable.insert({*it,stateNum0});
			dfa->setStateSum(stateNum0+1);
			dfa->insertNode(node0);
			dfa->setStartNum(stateNum0);
		}
		else
		{
			//找到接受状态
			bool isSuccess = false;
			for (auto i = (*it)->begin(); i != (*it)->end(); i++)
			{
				if (nfa->nodes.at((*i))->getType() == 2)
				{
					isSuccess = true;
					break;
				}
			}
			int stateNum0 = dfa->getStateSum();
			if (isSuccess == true)
			{
				auto node0 = make_shared<FANode>(2, stateNum0);
				markTable.insert({ *it, stateNum0 });
				dfa->setStateSum(stateNum0 + 1);
				dfa->insertNode(node0);
				dfa->getSuccessCol().push_back(stateNum0);
			}
			else
			{
				auto node0 = make_shared<FANode>(1, stateNum0);
				markTable.insert({ *it, stateNum0 });
				dfa->setStateSum(stateNum0 + 1);
				dfa->insertNode(node0);
			}
		}
	}
	//然后构建边.进行关联容器遍历.
	for (auto it = table.begin(); it != table.end();it++)
	{
		auto fromVec = it->first.first;
		auto edge = it->first.second;
		auto toVec = it->second;
		//要构造边,edgeTable,nodeMatrix.
		//不懂为什么报错?只有一个状态谜之报错,其他都可以.
		auto from = markTable.find(fromVec)->second;
		int to = findInMap(markTable,toVec)->second;
		edge->setFromTo(from,to);
		dfa->insertEdge(edge);
		dfa->insertToNTable(from, to, 1);
		dfa->insertToETable(from, to, edge);
		dfa->getVevTable().insert({ {from,edge},to });
	}
	return dfa;
}
shared_ptr<vector<int>> generateClosure(int stateNum, shared_ptr<Nfa> nfa)
{
	stack<int> stateStack;
	auto result = make_shared<vector<int>>();
	stateStack.push(stateNum);
	result->push_back(stateNum);
	int sum = nfa->getStateSum();
	while (!stateStack.empty())
	{
		int state = stateStack.top();
		stateStack.pop();
		for (int i = 0; i < sum; i++){
			if (nfa->getNodeMatrix().find(make_pair(state,i)) != nfa->getNodeMatrix().end())
			{
				auto it = nfa->getEdgeTable().find(make_pair(state, i));
				shared_ptr<FAEdge> edge = it->second;
				if (edge->getContent().size() == 1 && edge->getContent().front() == "")
				{
					stateStack.push(i);
					result->push_back(i);
				}
			}
		}
	}
	return result;
}
map<shared_ptr<vector<int>>, int>::iterator findInMap(map<shared_ptr<vector<int>>, int>& mapTest, shared_ptr<vector<int>>& test)
{
	bool isFind = false;
	for (auto i = mapTest.begin(); i != mapTest.end(); i++)
	{
		auto temp = (*i).first;
		if ((temp->size() == test->size()))
		{
			for (auto j = temp->begin(), q = test->begin(); j != temp->end(); j++, q++)
			{
				if (*j != *q)
				{
					isFind = false;
					break;
				}
			}
			isFind = true;
			return i;
		}
	}
	return mapTest.end();
}
//DFA到最小化DFA
shared_ptr<Nfa> hopcroft(shared_ptr<Nfa> dfa)
{
	vector<vector<int>> tstates;
	vector<vector<int>> pstates;
	vector<int> startOne;
	for (int i = 0; i < dfa->getStateSum(); i++)
	{
		if (find(dfa->getSuccessCol().begin(), dfa->getSuccessCol().end(), i) == dfa->getSuccessCol().end())
		{
			startOne.push_back(i);
		}
	}
	tstates.push_back(dfa->getSuccessCol());
	tstates.push_back(startOne);
	while (!equalVector(pstates,tstates))
	{
		pstates.assign(tstates.begin(),tstates.end());
		tstates.erase(tstates.begin(),tstates.end());
		for (auto i = pstates.begin(); i != pstates.end(); i++)
		{
			auto splitResult = hopcroftSplit(*i,pstates,dfa);
			tstates = *union2Vec(tstates,*splitResult);
		}
	}
	//构建最小的dfa...
	//想到的是排序,使得nodes里的status是按顺序的,这样取的时候就很方便了.
	sort(dfa->nodes.begin(), dfa->nodes.end(), [](shared_ptr<FANode> n1,shared_ptr<FANode> n2){
		return n1->getStateNum() < n2->getStateNum();
	});
	auto mindfa = make_shared<Nfa>();
	map<int, vector<int>> nodeWithnode;
	int j = 0;
	for (auto i = tstates.begin(); i != tstates.end(); i++,j++)
	{
		int type = 1;
		nodeWithnode.insert({j,*i});
		for (auto k =(*i).begin();k != (*i).end(); k++)
		{
			if (dfa->nodes.at(*k)->getType() == 0){
				type = 0;
				break;
			}
			else if (dfa->nodes.at(*k)->getType() == 2)
			{
				type = 2;
				break;
			}
		}
		mindfa->insertNode(make_shared<FANode>(type,j));
	}
	//这里和上面的一个函数要做的事情差不多.
	for (auto i = dfa->getEdges().begin(); i != dfa->getEdges().end(); i++)
	{
		auto edge = *i;
		int f = edge->getFromState();
		int t = edge->getToState();
		int from = findNFMap(nodeWithnode,f);
		int to = findNFMap(nodeWithnode,t);
		edge->setFromState(from);
		edge->setToState(to);
		if (mindfa->getEdgeTable().find({from,to}) == mindfa->getEdgeTable().end())
		{
			mindfa->insertEdge(edge);
			mindfa->insertToETable(from, to, edge);
			mindfa->insertToNTable(from, to, 1);
		}
	}
	sort(mindfa->nodes.begin(), mindfa->nodes.end(), [](shared_ptr<FANode> n1, shared_ptr<FANode> n2){
		return n1->getStateNum() < n2->getStateNum();
	});
	return mindfa;
}
shared_ptr<vector<vector<int>>> hopcroftSplit(vector<int>& sState, vector<vector<int>>& stateCol, shared_ptr<Nfa> dfa)
{
	auto ret = make_shared<vector<vector<int>>>();
	auto edges = dfa->getEdges();
	int psize = stateCol.size()+1;
	//将edge分类.
	map<int, vector<int>> edgeGroup;
	int add1 = 0,add2 = 0;	//用于递增
	for (auto i = edges.begin(); i != edges.end(); i++,add1++)
	{
		if (add1 == 0)
		{
			vector<int> tt; tt.push_back(0);
			edgeGroup.insert({ 0, tt});
			add2++;
			continue;
		}
		int count = 0;
		for (auto j = edgeGroup.begin(); j != edgeGroup.end(); j++)
		{
			auto temp = ((*j).second).front();
			if (equalVector((*i)->getContent(),edges.at(temp)->getContent()))
			{
				(*j).second.push_back(add1);
				break;
			}
			else
			{
				count++;
			}
		}
		if (count == edgeGroup.size())
		{
			vector<int> tt; tt.push_back(add1);
			edgeGroup.insert({add2,tt});
			add2++;
		}
	}
	for (auto n = edgeGroup.begin(); n != edgeGroup.end(); n++)
	{
		map<int, int> sc;
		auto group = (*n).second;
		for (auto k = sState.begin(); k != sState.end(); k++)
		{
			int count = 0;
			for (auto j = group.begin(); j != group.end(); j++)
			{
				int whathappen = *j;
				//试图加错误处理.记得.
				auto edge = edges.at(whathappen);
				auto result = dfa->getVevTable().find(make_pair(*k, edge));   //能找到的吗,应该找的到,如果这都做不到,还叫泛型吗?
				if (result != dfa->getVevTable().end())
				{
					int temp = result->second;
					int level = findInVec(stateCol, temp);
					sc.insert({ *k, level });
					break;
				}
				else
				{
					count++;
				}
			}
			if (count == group.size())
			{
				sc.insert({ *k, psize });
			}
		}
		//然后进行分类
		map<int, vector<int>> nodeGroup;
		for (int  m = 0; m < stateCol.size(); m++)
		{
			nodeGroup.insert({m,vector<int>()});
		}
		nodeGroup.insert({ stateCol.size()+1,vector<int>()});
		for (auto l = sState.begin(); l < sState.end(); l++)
		{
			int result = (sc.find(*l))->second;
			nodeGroup.find(result)->second.push_back(*l);
		}
		bool isOneGroup = false;
		for (auto m = nodeGroup.begin(); m != nodeGroup.end();m++)
		{
			if ((*m).second.size() == sState.size())
			{
				isOneGroup = true;
			}
		}
		if (!isOneGroup)
		{
			//不,第一个不为0的组就是答案.
			bool isFirst = true;
			vector<int> oneVec; vector<int> twoVec;
			for (auto m = nodeGroup.begin(); m != nodeGroup.end(); m++)
			{
				if (!isFirst)
				{
					twoVec.insert(twoVec.end(), (*m).second.begin(), (*m).second.end());
				}
				if ((*m).second.size() != 0 && isFirst)
				{
					isFirst = false;
					oneVec.assign((*m).second.begin(),(*m).second.end());
				}
			}
			ret->push_back(oneVec); ret->push_back(twoVec);
			return ret;
		}
	}
	ret->push_back(sState);
	return ret;
}
bool equalVector(vector<vector<int>>& p1, vector<vector<int>>& p2)
{
	if (p1.size() == p2.size())
	{
		for (auto i = p1.begin(), j = p2.begin(); i != p1.end(); i++,j++)
		{
			auto temp1 = (*i);
			auto temp2 = (*j);
			if (temp1.size() == temp2.size())
			{
				for (auto i1 = temp1.begin(),j1=temp2.begin(); i1 != temp1.end(); i1++,j1++)
				{
					if (*i1!=*j1)
					{
						return false;
					}
				}
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}
int findInVec(vector<vector<int>>& test,int target)
{
	int count = 0;
	for (auto i = test.begin(); i != test.end(); i++,count++)
	{
		auto temp = *i;
		for (auto j = temp.begin(); j != temp.end(); j++)
		{
			if (*j == target)
			{
				return count;
			}
		}
	}
	return -1;
}
shared_ptr<vector<vector<int>>> union2Vec(vector<vector<int>>& p1, vector<vector<int>>& p2)
{
	auto ret = make_shared<vector<vector<int>>>(p1.begin(),p1.end());
	for (auto i = p2.begin(); i != p2.end(); i++)
	{
		auto temp1 = *i;
		int count = 0;
		for (auto j = p1.begin(); j!= p1.end(); j++)
		{
			auto temp2 = *j;
			if (temp1 == temp2)
			{
				break;
			}
			count++;
		}
		if (count == p1.size())
		{
			ret->push_back(temp1);
		}
	}
	return ret;
}
int findNFMap(map<int, vector<int>>& table, int target)
{
	for (auto i = table.begin(); i != table.end(); i++)
	{
		auto temp = *i;
		auto tempVec = temp.second;
		if (find(tempVec.begin(),tempVec.end(),target)!=tempVec.end())
		{
			return temp.first;
		}
	}
	return -1;
}
bool equalVector(vector<string>& p1, vector<string>& p2)
{
	if (p1.size() != p2.size()){
		return false;
	}
	int count = 0;
	for (auto i = p1.begin(); i != p1.end(); i++)
	{
		if (find(p2.begin(), p2.end(), *i) == p2.end()){
			return false;
		}
	}
	return true;
}
//将图的矩阵改为链接形式.
shared_ptr<Dfa> convertToLink(shared_ptr<Nfa> nfa){
	auto dfa = make_shared<Dfa>();
	for (int i = 0; i < nfa->getStateSum(); i++)
	{
		auto status = shared_ptr<X_Status>();
		dfa->status.insert({i,status});
		//要分辨出开始,接受状态.
		if (find(nfa->getSuccessCol().begin(), nfa->getSuccessCol().end(), i)!= nfa->getSuccessCol().end()){
			status->setType(2);
		}
		if (i == nfa->getStateSum()){
			status->setType(0);
			dfa->startNum = i;
		}
		status->setType(1);
	}
	for (auto it = nfa->getEdges().begin(); it != nfa->getEdges().end(); it++)
	{
		int from = (*it)->getFromState();
		int to = (*it)->getToState();
		auto edge = make_shared<X_Edge>(from,to);
		int currentMark = dfa->edgeMark;
		dfa->edges.insert({currentMark,edge});
		dfa->status.find(from)->second->outEdges.push_back(to);
		dfa->status.find(to)->second->inEdges.push_back(from);
		dfa->edgeMark++;
	}
	return dfa;
}

//--------------------------------------Dfa-----------------------------------
Dfa::Dfa() :startNum(0), statusMark(0), edgeMark(0), contentMark(0)
{
}

Dfa::~Dfa()
{
}

