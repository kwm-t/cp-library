#include <stdio.h>
#include<iostream>
#pragma warning(disable : 4996)
int main() {
	//コマンドプロンプトのカレントディレクトリ適切にする必要がある
	FILE *in = freopen("in/in.txt", "r", stdin);
	FILE *out = freopen("out/out.txt", "w", stdout);

	//C:\Users\kiwam\Desktop\kyopro\000_project\Project1\x64\Release\in
	//C:\Users\kiwam\Desktop\kyopro\000_project\Project1\x64\Release\out
	return 0;
}