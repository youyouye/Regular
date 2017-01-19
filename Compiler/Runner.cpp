#include "Runner.h"
#include "ASTree.h"
#include "ReASTree.h"
#include "Lexer.h"
#include "ReParser.h"
#include "Nfa.h"
#include<sstream>
/*
void main()
{
	ReParser* parse = new ReParser(Lexer());
	shared_ptr<ASTree> t = parse->re();
	string result = t->toString();
	cout << result << endl;
	
	auto nfa = make_shared<Nfa>();
	t->thompson(nfa);
	cout << "生成节点数"<<nfa->getStateSum() << endl;
	//nfa->dfa
	auto dfa = generateDFA(nfa);
	cout << "dfa节点数" << dfa->getStateSum() << endl;
	//nfa最小化.
	auto mindfa = hopcroft(dfa);
	cout << "mindfa节点数" <<mindfa->nodes.size()<< endl;
	cout << "转化成链表形式" << endl;
	auto linkDfa = convertToLink(mindfa);
	reRunner(linkDfa);
	
}
*/
void reRunner(shared_ptr<Dfa> nfa){
	init();
	string temp;		
	
	while ((temp = nextChar()) != "-1")
	{
		
	}
}

void init(){
	in = new ifstream("D:\\vs2015\\MyCode\\Compiler\\Compiler\\character.txt");
	int i = 0;
	if (in)
	{
		char temp;
		while ((temp = in->get())!=EOF && i<2048){
			stringstream stream; stream << temp;
			string stemp = stream.str();
			buffer.push_back(stemp);
			i++;
		}
		if (temp == EOF){
			buffer.push_back("-1");
		}
	}
}
void readWord(){
	int type = 0;
	if (input == n)
	{
		type = 1;
	}
	else if (input == 2*n)
	{
		type = 2;
	}
	int i = 0;
	if (in)
	{
		char temp;
		while ((temp = in->get()) != EOF && i<2048){
			stringstream stream; stream << temp;
			string stemp = stream.str();
			if (type == 1)
			{
				buffer.push_back(stemp);
			}
			else if (type == 2)
			{
				buffer.at(i) = stemp;
			}
			i++;
		}
		if (temp == EOF){
			buffer.push_back("-1");
		}
	}
}
string nextChar(){
	string word = buffer.at(input);
	input = (input + 1) % 2*n;
	if (input%n == 0)
	{
		readWord();
		fence = (input + n) % 2*n;
	}
	return word;
}
void rollBack(){
	if (input == fence)
	{
		throw new runtime_error("world ruin");
	}
	input = (input - 1) % 2 * n;
}