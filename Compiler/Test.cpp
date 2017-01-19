#include<cstdio>  
#include<map>  
#include<string>  
#include<cstring>  
#include<algorithm>  
using namespace std;

const int N = 105;
const int M = 55;
FILE *f1;
/*
储存读入的文法文件.将字符和数字对应.
*/
struct CharHash{
	map<char, int> mp; //字符对应位置
	char ch[N];	 //储存所有字符
	bool End[N]; //非终结符为0,终结符1
	int cnt;	//字符总数
	void init(){
		memset(End, 1, sizeof(End));
		mp.clear();
		cnt = 0;
		ch[cnt] = '#';
		mp['#'] = cnt++;
	}
	//插入到map中,如果存在跳过.
	int Insert(char c){
		if (mp.find(c) == mp.end()){
			ch[cnt] = c;
			mp[c] = cnt++;
		}
		return mp[c];
	}
	char Find(int c){
		return ch[c];
	}
	void SetnEnd(int c){
		End[c] = 0;
	}
};

CharHash ChHash;

struct Unknowname{
	//处理文件中的一行.保存结果到CharHash中.
	struct Derivation{
		int s;  //非终结符字符所在位置
		int t[10]; //展开式存储
		int tcnt;  
		int select[M]; //选择集
		void add(){
			memset(select, 0, sizeof(select));
			char tmp[10];
			fscanf(f1, "%s", tmp);
			s = ChHash.Insert(tmp[0]);
			fscanf(f1, "%s", tmp);
			tcnt = strlen(tmp);
			for (int i = 0; tmp[i] != '\0'; i++)
				t[i] = ChHash.Insert(tmp[i]);
			fscanf(f1, "%s", tmp);
			for (int i = 0; tmp[i] != '\0'; i++)
				select[ChHash.Insert(tmp[i])] = 1;
			ChHash.SetnEnd(s);
		}
		bool operator < (const Derivation &a)const{
			return s < a.s;
		}
	};
	Derivation Der[N];
	int n;
	int table[M][M];  //生成的识别表达式的LL(1)语法分析器表
	int queue[N], qcnt;
	char Schar; //起始字符
	void init(){
		memset(table, -1, sizeof(table));
	};
	/*
	读入文件,每一行用Derivation处理.保存在CharHash中.
	*/
	void read(){
		fscanf(f1, "%d", &n);
		for (int i = 0; i < n; i++)
			Der[i].add();
		sort(Der, Der + n);
		fscanf(f1, " %c", &Schar);
	}
	//检测是否是LL(1)文法.是书上的一个固定的方法。
	bool check(){
		bool in[M];
		bool ok = 1;
		memset(in, 0, sizeof(in));
		for (int i = 0; i < n && ok; i++){
			if (i && Der[i].s != Der[i - 1].s)
				memset(in, 0, sizeof(in));
			for (int j = 0; j < ChHash.cnt; j++){
				if (in[j] && Der[i].select[j]) ok = 0;
				in[j] |= Der[i].select[j];
			}
		}
		puts(ok ? "The Data is ok!" : "Wrong Data!");
		return ok;
	}
	//构建表算法
	void GetTable(){
		int cc = ChHash.cnt;
		for (int i = 0; i < n; i++){
			for (int j = 0; j < cc; j++){
				if (Der[i].select[j] == 0) continue;
				int u = Der[i].s;
				int v = j;
				table[u][v] = i;
			}
		}
	}
	//LL(1)语法分析.好像跟书上的差不多.
	bool Analysis(char s[]){
		char stack[N], top = 0;
		stack[top++] = ChHash.Insert('$');
		stack[top++] = ChHash.Insert(Schar); //起始字符
		int tail = 0, len = strlen(s); //s是要分析的
		qcnt = 0;
		for (int i = 0; s[i] != '\0'; i++)
			s[i] = ChHash.Insert(s[i]);
		while (top){
			int u = stack[top - 1]; //栈顶元素
			int v = s[tail];        //当前
			if (u == 0){
				top--;
				continue;
			}
			if (ChHash.End[u]){     //判断是否是终结符
				if (v != u){
					printf("Wrong!1\n");
					return 0;
				}
				else{
					top--;
					tail++;
				}
			}
			else{
				int id = table[u][v];
				if (id == -1){
					printf("Wrong!2\n");
					return 0;
				}
				top--;
				for (int i = Der[id].tcnt - 1; i >= 0; i--)
					stack[top++] = Der[id].t[i];
				queue[qcnt++] = id;
			}
		}
		if (top == 0 && tail == len){
			printf("Accepted!\n");
			return 1;
		}
		else{
			printf("Wrong3!\n");
			return 0;
		}
	}
	void output(){
		for (int i = 0; i < qcnt; i++){
			int id = queue[i];
			printf("%c->", ChHash.Find(Der[id].s));
			for (int j = 0; j < Der[id].tcnt; j++)
				printf("%c", ChHash.Find(Der[id].t[j]));
			printf("\n");
		}
	}
};

Unknowname Table;
/*
文件格式：
N 产生式个数
接下来x行每行有三个字符串，分别代表产生式左边，右边还有对应的select集
最后一行字母s,代表起始字符
*/
int main(){
	//freopen("out","w",stdout);  
	f1 = fopen("d://study//test.txt", "r");
	ChHash.init();
	Table.init();
	//读取文件内容,保存在ChHash中.
	Table.read();
	//检测是否符合LL(1)文法.
	if (Table.check() == 0) return 0;
	//构建表算法,生成的表存储在table[M][M]中.
	Table.GetTable();
	char tmp[100]; //储存要判断的string.
	printf("please input the string: ");
	//输入^,表示结束输入,否则继续.
	while (~scanf("%s", tmp)){
		if (tmp[0] == '^') break;
		int len = strlen(tmp);
		tmp[len] = '$';
		tmp[len + 1] = '\0';
		//LL(1)语法分析
		bool ok = Table.Analysis(tmp);
		/*
		if (ok){
			//这里的展开不要也行.
			int t;
			printf("do you want to know the derivation?\n(0 is no  1 is yes)\n");
			scanf("%d", &t);
			if (t) Table.output();
		}
		*/
		printf("please input the string: (^ is over)");
	}
	fclose(f1);
	return 0;
}

/*
i+(i*i+i)
i(i)
((i))
i+i+
*/