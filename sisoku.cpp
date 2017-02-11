#include <bits/stdc++.h>
using namespace std;
 
//四つの整数の加減乗除においてとりうる数式の形（ただし、明らかに結果が10にならないものは除いている）
const string number[60] = {
	"a+b+c+d", "a+b+c-d", "a+b-c-d", 
	"a*b*c*d", "a*b*c/d", "a*b/c/d",
	"a+b+c*d", "a+b-c*d", "a-b+c*d", "a*b-c-d", "a+b+c/d", "a+b-c/d", "a-b+c/d", 
	"(a+b)*c+d", "(a+b)*c-d", "(a+b)/c+d", "(a+b)/c-d", "(a-b)*c+d", "(a-b)*c-d", "(a-b)/c+d", "a/(b+c)+d", "a/(b-c)+d", 
	"(a+b+c)*d", "(a+b-c)*d", "(a-b-c)*d", "(a+b+c)/d", "(a+b-c)/d", 
	"a*b*c+d", "a*b*c-d", "a*b/c+d", "a*b/c-d", "a/b/c+d", 
	"a*b+c*d", "a*b+c/d", "a/b+c/d", "a*b-c*d", "a*b-c/d", 
	"(a+b)*c*d", "(a+b)*c/d", "(a+b)/c/d", "(a-b)*c*d", "(a-b)*c/d", "a*b/(c+d)", "a*b/(c-d)", 
	"(a*b+c)*d", "(a*b-c)*d", "(a/b+c)*d", "(a/b-c)*d", "(a*b+c)/d", "(a*b-c)/d", "(a/b+c)/d", "(a-b*c)*d", "(a-b/c)*d", "a/(b/c+d)", "a/(b/c-d)", "a/(b-c/d)", 
	"(a+b)*(c+d)", "(a+b)*(c-d)", "(a-b)*(c-d)", "(a+b)/(c-d)"
};
 
//分数を扱うための構造体
struct num {
	int ch, pa; //分子、分母
};
 
//分数の計算
num calc(char x, num a, num b){
	num ret;
	if(x == '+'){
		ret.pa = a.pa*b.pa;
		ret.ch = a.ch*b.pa+a.pa*b.ch;
	}
	else if(x == '-'){
		ret.pa = a.pa*b.pa;
		ret.ch = a.ch*b.pa-a.pa*b.ch;
	}
	else if(x == '*'){
		ret.pa = a.pa*b.pa;
		ret.ch = a.ch*b.ch;
	} else{
		ret.pa = a.pa*b.ch;
		ret.ch = a.ch*b.pa;
	}
	return ret;
}
 
num check(string str, num x[4]){
	int cnt = 0, f;
	num y[10];
	for(int i = 0; i < 4; i++) y[i] = x[i];
 
	//カッコを処理
	for(int i = 0; i < str.size(); i++){
		if(str[i] == '(') f = i;
		if(str[i] == ')'){
			cnt++;
			y[3+cnt] = check(str.substr(f+1,i-f-1), x);
			str.erase(f,i-f+1);
			string s;
			str.insert(f,s='d'+cnt);
			i = 0;
		}
	}
 
	//乗除を処理
	for(int i = 0; i < str.size(); i++){
		if(str[i] == '*' || str[i] == '/'){
			cnt++;
			y[3+cnt] = calc(str[i],y[str[i-1]-'a'],y[str[i+1]-'a']);
			if(y[3+cnt].pa == 0) return (num){-100000,-100000}; //分母が0になったら数値が10にならないような値を返す
			str.erase(i-1,3);
			string s;
			str.insert(i-1,s='d'+cnt);
			i = 0;
		}
	}
 
	//加減を処理
	for(int i = 0; i < str.size(); i++){
		if(str[i] == '+' || str[i] == '-'){
			cnt++;
			y[3+cnt] = calc(str[i],y[str[i-1]-'a'],y[str[i+1]-'a']);
			if(y[3+cnt].pa == 0) return (num){-100000,-100000}; //分母が0になったら数値が10にならないような値を返す
			str.erase(i-1,3);
			string s;
			str.insert(i-1,s='d'+cnt);
			i = 0;
		}
	}
 
	return y[3+cnt];
}
 
 
//条件を満たす数式を求める,第二引数はallで呼び出されたかどうか（allなら一つでも見つければよい）
vector<string> solve(num x[4], bool all){
	vector<string> ret;
	int a[4]; //この関数内ではxを整数で扱ってよい
	for(int i = 0; i < 4; i++) a[i] = x[i].ch;
	for(int i = 0; i < 60; i++){
		sort(a,a+4);
		string str = number[i];
		//a,b,c,dに数字を全通り入れる
		do{
			for(int j = 0; j < 4; j++) x[j].ch = a[j];
			num t = check(str,x);
 
			//演算結果が10になったとき
			if(t.ch == 10*t.pa){
				for(int j = 0; j < str.size(); j++)
					if(0<=str[j]-'a'&&str[j]-'a'<4)
						str[j] = '0'+a[str[j]-'a'];
 
				ret.push_back(str);
				if(all) return ret;
				break;
			}
		} while(next_permutation(a,a+4));
	}
	return ret;
}
 
//1234などの数字が入力されたら条件を満たす数式を出力
void input(){
	string str;
	while(cin >> str){
		num x[4];
		for(int i = 0; i < 4; i++){
			x[i].ch = str[i]-'0';
			x[i].pa = 1;
		}
 
		//出力
		vector<string> out = solve(x,0); 
		if(out.size() == 0){
			printf("not exist\n");
			continue;
		}
		for(int i = 0; i < out.size(); i++)
			printf("%s\n", out[i].c_str());
	}
}
 
//4つの数字が全て異なる場合について全て調べる
void all(){
	bool ok = true;
	num x[4];
	for(int i = 0; i < 4; i++) x[i].pa = 1;
	int a[4];
	for(a[0] = 1; a[0] <= 9; a[0]++){
		for(a[1] = a[0]+1; a[1] <= 9; a[1]++){
			for(a[2] = a[1]+1; a[2] <= 9; a[2]++){
				for(a[3] = a[2]+1; a[3] <= 9; a[3]++){
					for(int i = 0; i < 4; i++) x[i].ch = a[i];
					if(solve(x,1).size() == 0) ok = false;
				}
			}
		}
	}
 
	//全ての場合に答えが存在したらYes
	printf("%s\n", ok?"Yes":"No");
}
 
int main(){
	input(); //入力に対して出力
	all(); //全ての場合に答えが存在するかどうか
}
