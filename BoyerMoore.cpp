#include <bits/stdc++.h>

using namespace std;

/*
	This method processes the pattern to calculate the table
	of bad characteres. It returns a 2D table that gives the
	last index to the left of the position i of the character a.
*/

map< pair<char,int>, int > computeBadChairTable(const string& pattern, const string& alphabet){
	map< pair<char, int>, int> table;
	map<char, int> lastocc;
	int m = pattern.size();
	for(char a : alphabet) lastocc[a] = -1;
	for(int i = 0; i < m; ++i){
		for(char a : alphabet) table[make_pair(a, i)] = lastocc[a];
		lastocc[pattern[i]] = i;
	}
	return table;
}

/*
	This is the main method of the boyer moore algorithm.
	It receives as parameters the preprocessed tables that 
	represents the two heuristics taught in class. The bad
	character and the good suffix, and uses these to make
	smart jumps in the text when trying to find occurences 
	of the pattern
*/

vector<int> boyerMoore(const string& text, const string& pattern, map< pair<char,int>, int >& badCharTable){
	vector<int> occurrences;
	int n = text.size();
	int m = pattern.size();
	int i = 0;
	while(i <= n-m){
		int j = m-1;
		while(j >= 0 && text[i+j]==pattern[j]) j--;
		if(j<0){
			occurrences.push_back(i);
			i++;
		} else {
			i += j-badCharTable[make_pair(text[i+j], j)];
		}
	}
	return occurrences;
}

int main(){
	int TC; scanf("%d", &TC);
	while(TC--){
		string pat; cin >> pat;
		string alphabet; cin >> alphabet;
		map< pair<char,int>, int> badChar = computeBadChairTable(pat, alphabet);
		int q; cin >> q;
		while(q--){
			string text; cin >> text;
			vector<int> occ = boyerMoore(text, pat, badChar);
			for(int a : occ) cout << a << " ";
			cout << endl;
		}
	}
}