#include "Graph.h"

FANode::FANode()
{

}
FANode::FANode(int mType, int mStateNum) :type(mType),stateNum(mStateNum)
{
}

FANode::~FANode()
{

}

FAEdge::FAEdge()
{
}

FAEdge::FAEdge(vector<string> mContents) :contents(mContents)
{
}


FAEdge::~FAEdge()
{
}

X_Status::X_Status()
{
}

X_Status::~X_Status()
{
}

X_Edge::X_Edge() :start(0), end(0)
{
}
X_Edge::X_Edge(int mstart, int mend)
{
	start = mstart;
	end = mend;
}
X_Edge::X_Edge(int mstart, int mend, int mtype) :start(mstart), end(mend), type(mtype)
{
}
X_Edge::~X_Edge()
{
}
StringLiteral::StringLiteral()
{
}
StringLiteral::StringLiteral(string mword, int mtype) :word(mword), type(mtype)
{	
}
StringLiteral::StringLiteral(vector<string> msegment, int mtype) : segment(msegment), type(mtype)
{
}
StringLiteral::~StringLiteral()
{
}
