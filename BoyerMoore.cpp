#include <bits/stdc++.h>

using namespace std;

/*
	This method computes the size of the border of a string.
	The border will be used to calculate the good suffix
	table.
*/

vector<int> border(const string& str){
	int i = 0, j = -1, n = str.size();
	vector<int> border; border.resize(n+1);
	border[0] = -1;
	while(i<n){
		while(j>=0 && str[i] != str[j]) j = border[j];
		i++; j++;
		border[i] = j;
	}
	return border;
}

/*
	This method processes the pattern to calculate
	the good suffix table, which will return, for each
	position, the size of the jump of the text if a 
	mismatch ocurrs in that position.
*/

vector<int> goodSuffixTable(string pattern){
	vector<int> b = border(pattern);
	reverse(pattern.begin(), pattern.end());
	vector<int> bR = border(pattern);
	int m = pattern.size();
	vector<int> S; S.resize(m+1);
	for(int i = 0; i <= m; ++i) S[i] = m-b[m];
	for(int l = 1; l <= m; ++l){
		int j = m-1-bR[l];
		if(pattern[j] != pattern[m-l-1] && S[j] > l-bR[l])
			S[j] = l-bR[l];
	}
	return S;
}

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

vector<int> boyerMoore(const string& text, const string& pattern, map< pair<char,int>, int >& badCharTable, vector<int>& S){
	vector<int> occurrences;
	int n = text.size();
	int m = pattern.size();
	int i = 0;
	while(i <= n-m){
		int j = m-1;
		while(j >= 0 && text[i+j]==pattern[j]) j--;
		if(j<0){
			occurrences.push_back(i);
			i += S[0];
		} else {
			i += max(j-badCharTable[make_pair(text[i+j], j)], S[j]);
		}
	}
	return occurrences;
}

int main(){
	//goodSuffixTable("abca");
	int TC; scanf("%d", &TC);
	while(TC--){
		string pat; cin >> pat;
		string alphabet; cin >> alphabet;
		map< pair<char,int>, int> badChar = computeBadChairTable(pat, alphabet);
		vector<int> S = goodSuffixTable(pat);
		int q; cin >> q;
		while(q--){
			string text; cin >> text;
			vector<int> occ = boyerMoore(text, pat, badChar, S);
			for(int a : occ) cout << a << " ";
			cout << endl;
		}
	}
}