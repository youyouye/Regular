#ifndef ASTREE_H_
#define ASTREE_H_
#include <string>
#include <memory>
using namespace std;
class Nfa;
class Dfa;
struct ThompResult
{
	ThompResult(int st, int en, int suc) :start(st), end(en), success(suc){}
	int start;
	int end;
	int success;
};
class ASTree
{
public:
	ASTree();
	virtual ~ASTree();
	virtual shared_ptr<ASTree> child(int i);
	virtual string location();
	virtual int numChildren();
	virtual string toString();
	virtual shared_ptr<ThompResult> thompson(shared_ptr<Nfa> mNfa);
	virtual shared_ptr<ThompResult> x_thompson(shared_ptr<Dfa> mNfa);
private:
};


#endif