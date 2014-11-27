#include <bits/stdc++.h>

using namespace std;

/*
	The 2 structs (Node and the ternary tree of nodes) below are constructed to make the proccess of checking if a state of the
	automaton already exists in the set of states when the automaton is being constructed.
	With the ternary tree, the cost of searching for a state turns out to be linear in the size of the state.
*/

struct Node {
	int id;
	Node* left;
	Node* mid;
	Node* right;
	Node(int id) {
		this->id = id;
		this->left = NULL;
		this->right = NULL;
		this->mid = NULL;
	}
};

struct TernaryTree {
	Node* head; 

	TernaryTree() {
		head = new Node(0);
	}

	/*
		This method adds the column to the tree if it does not exist
		and returns the id of the column added.
	*/
	int searchOrAdd(const vector<int>& newColumn, int id) {
		Node* act = head;
		for (int i = 1; i < newColumn.size(); ++i) {
			int dif = newColumn[i]-newColumn[i-1];
			if (dif == -1) {
				if (act->left != NULL) {
					act = act->left;
				} else {
					act->left = new Node(id);
					act = act->left;
				}
			} else if (dif == 0) {
				if (act->mid != NULL) {
					act = act->mid;
				} else {
					act->mid = new Node(id);
					act = act->mid;
				}
			} else {
				if (act->right != NULL) {
					act = act->right;
				} else{
					act->right = new Node(id);
					act = act->right;
				}
			}
		}
		return act->id;
	}
};

/*
	This struct represents a state of the ukkonen automaton. It has a vector of 
	integers representing the corresponding column and a function that maps each 
	character of the alphabet to a new state.
*/

struct state {
	vector<int> column;
	unordered_map<char,int> sigma;
	state() {}
	state(vector<int> column) : column(column) {}
};

/*
	This method generates the column of the next state of the automaton, according to the column
	represented by the actual state and the current character of the alphabet.
*/

vector<int> nextColumn(const vector<int>& column, const char a, const string& pattern, const int r) {
	vector<int> newColumn = {0};
	int m = pattern.size();
	for (int i = 1; i <= m; ++i) {
		int mn = r+1;
		mn = min(mn, newColumn[i-1]+1);
		mn = min(mn, column[i]+1);
		mn = min(mn, column[i-1]+(pattern[i-1]!=a));
		newColumn.push_back(mn);
	}	
	return newColumn;
}

/*
	This method constructs the automaton that will be used later to find approximate matchings.
*/

vector<state> buildUkkFsm(const string& pattern, const string& alphabet, const int r) {
	int id = 0;
	int m = pattern.size();
	vector<state> automaton;
	vector<int> column;
	for (int i = 0; i <= m; ++i) {
		column.push_back(min(i,r+1));
	}
	automaton.push_back(state(column));
	queue<int> N; N.push(0);
	TernaryTree TT = TernaryTree();
	TT.searchOrAdd(column, id++);
	while (!N.empty()) {
		int act = N.front(); N.pop();
		for (char a : alphabet) {
			column = nextColumn(automaton[act].column, a, pattern, r);
			int nxtid = TT.searchOrAdd(column, id);
			if (nxtid == id) {
				automaton.push_back(state(column));
				N.push(id++);
			}
			automaton[act].sigma[a] = nxtid;
		}
	}
	return automaton;
}

/*
	This is the main method of the ukkonen approximated matching algorithm.  It proccesses the text
	looking for matches. If an approximated matched ocurred at some suffix of the preffix [0, j), than
	j will be in the returned vector. 
*/

vector<int> ukkonen(const string& text, const int m, const int r, vector<state>& Q) {
	vector<int> occurences;
	int act = 0;
	int n = text.size();
	if (Q[act].column[m] <= r) {
		occurences.push_back(0);
	}
	for (int j = 0; j < n; ++j) {
		act = Q[act].sigma[text[j]];
		if (Q[act].column[m] <= r) {
			occurences.push_back(j+1);
		}
	}
	return occurences;
}


int main(){
	int TC; scanf("%d", &TC);
	while(TC--){
		printf("pattern: "); string pat; cin >> pat;
		printf("alphabet: "); string alphabet; cin >> alphabet;
		printf("max error: "); int r; cin >> r;
		vector<state> Q = buildUkkFsm(pat, alphabet, r);
		printf("num texts: "); int q; cin >> q;
		while (q--) {
			printf("text: "); string text; cin >> text;
			vector<int> ans = ukkonen(text, (int)pat.size(), r, Q);
			for (int num : ans) cout << num << " ";
			cout << endl;
		}
	}
	/*
	TernaryTree test_tree = TernaryTree();
	int m; scanf("%d", &m);
	int n; scanf("%d", &n);
	int id = 0;
	while(n--){
		vector<int> vec;
		for(int i = 0; i < m; ++i){
			int x; scanf("%d", &x);
			vec.push_back(x);
		}
		printf("%d\n", test_tree.searchOrAdd(vec, id++));
	}*/
	return 0;
}