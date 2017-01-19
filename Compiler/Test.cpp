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
���������ķ��ļ�.���ַ������ֶ�Ӧ.
*/
struct CharHash{
	map<char, int> mp; //�ַ���Ӧλ��
	char ch[N];	 //���������ַ�
	bool End[N]; //���ս��Ϊ0,�ս��1
	int cnt;	//�ַ�����
	void init(){
		memset(End, 1, sizeof(End));
		mp.clear();
		cnt = 0;
		ch[cnt] = '#';
		mp['#'] = cnt++;
	}
	//���뵽map��,�����������.
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
	//�����ļ��е�һ��.��������CharHash��.
	struct Derivation{
		int s;  //���ս���ַ�����λ��
		int t[10]; //չ��ʽ�洢
		int tcnt;  
		int select[M]; //ѡ��
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
	int table[M][M];  //���ɵ�ʶ����ʽ��LL(1)�﷨��������
	int queue[N], qcnt;
	char Schar; //��ʼ�ַ�
	void init(){
		memset(table, -1, sizeof(table));
	};
	/*
	�����ļ�,ÿһ����Derivation����.������CharHash��.
	*/
	void read(){
		fscanf(f1, "%d", &n);
		for (int i = 0; i < n; i++)
			Der[i].add();
		sort(Der, Der + n);
		fscanf(f1, " %c", &Schar);
	}
	//����Ƿ���LL(1)�ķ�.�����ϵ�һ���̶��ķ�����
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
	//�������㷨
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
	//LL(1)�﷨����.��������ϵĲ��.
	bool Analysis(char s[]){
		char stack[N], top = 0;
		stack[top++] = ChHash.Insert('$');
		stack[top++] = ChHash.Insert(Schar); //��ʼ�ַ�
		int tail = 0, len = strlen(s); //s��Ҫ������
		qcnt = 0;
		for (int i = 0; s[i] != '\0'; i++)
			s[i] = ChHash.Insert(s[i]);
		while (top){
			int u = stack[top - 1]; //ջ��Ԫ��
			int v = s[tail];        //��ǰ
			if (u == 0){
				top--;
				continue;
			}
			if (ChHash.End[u]){     //�ж��Ƿ����ս��
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
�ļ���ʽ��
N ����ʽ����
������x��ÿ���������ַ������ֱ�������ʽ��ߣ��ұ߻��ж�Ӧ��select��
���һ����ĸs,������ʼ�ַ�
*/
int main(){
	//freopen("out","w",stdout);  
	f1 = fopen("d://study//test.txt", "r");
	ChHash.init();
	Table.init();
	//��ȡ�ļ�����,������ChHash��.
	Table.read();
	//����Ƿ����LL(1)�ķ�.
	if (Table.check() == 0) return 0;
	//�������㷨,���ɵı�洢��table[M][M]��.
	Table.GetTable();
	char tmp[100]; //����Ҫ�жϵ�string.
	printf("please input the string: ");
	//����^,��ʾ��������,�������.
	while (~scanf("%s", tmp)){
		if (tmp[0] == '^') break;
		int len = strlen(tmp);
		tmp[len] = '$';
		tmp[len + 1] = '\0';
		//LL(1)�﷨����
		bool ok = Table.Analysis(tmp);
		/*
		if (ok){
			//�����չ����ҪҲ��.
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