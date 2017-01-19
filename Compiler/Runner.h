#ifndef __RUNNER_H_
#define __RUNNER_H_
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <fstream>
using namespace std;

int input = 0;
int fence = 0;
int n = 2048;
ifstream* in;
vector<string> buffer;
void reRunner();
void init();
void readWord();
string nextChar();
void rollBack();
#endif