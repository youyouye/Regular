#ifndef GRAPH_H_
#define GRAPH_H_
#include <vector>
#include <memory>
#include <string>
using namespace std;
/*
采用邻接表法好了.
*/

class FANode{
public:
	FANode();
	FANode(int mType,int mStateNum);
	~FANode();
	int getType(){ return type; }
	void setType(int mType){ type = mType; }
	int getStateNum(){ return stateNum; }
	void setStateNum(int mStateNum){ stateNum = mStateNum; }
private:
	int type;	//0:start;1:普通状态;2:accept;-1:error;
	int stateNum;  //编号,从0开始
};
class FAEdge
{
public:                                           
	FAEdge();
	FAEdge(vector<string> mContents);
	~FAEdge();
	vector<string> getContent(){ return contents; }
	void setContent(vector<string> mContents){ contents = mContents; }
	void setFromTo(int from, int to){ fromState = from; toState = to; }
	void setFromState(int from){ fromState = from; }
	int getFromState(){ return fromState; }
	void setToState(int to){ toState = to; }
	int getToState(){ return toState; }
private:
	int fromState;
	int toState;
	vector<string> contents;
};
class X_Status
{
public:
	X_Status();
	~X_Status();
	vector<int> inEdges;
	vector<int> outEdges;
	int getType () const{ return type; }
	void setType(const int mtype){ type = mtype; }
private:
	int type; //0:start;1:normal;2.success.4.0&&2
};
class X_Edge
{
public:
	X_Edge();
	X_Edge(int mstart,int mend);
	X_Edge(int mstart, int mend,int mtype);
	~X_Edge();
	int start;
	int end;
	int getMatchContent() const{ return matchContent; }
	void setMatchContent(const int mContent){ matchContent = mContent; }
private:
	int matchContent;
	int type; //1.内容边 2.Nfa中的空边,没有内容. 3.功能边,暂待.
};
//专门构建的字符类.先分三类试一试,单个字符,字符片段.字符合集.
class StringLiteral
{
public:
	StringLiteral();
	StringLiteral(string mword,int mtype);
	StringLiteral(vector<string> msegment,int mtype);
	~StringLiteral();
	int getType() const { return type; }
	void setType(const int mtype){ type = mtype; }
private:
	string word;
	vector<string> segment;
	int type; //1:单个.2:片段 3.字符集
};


#endif