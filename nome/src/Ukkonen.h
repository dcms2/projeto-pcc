#ifndef UKKONEN
#define UKKONEN

#include <bits/stdc++.h>
#include "Ternary_Tree.h"

using namespace std;

class Ukkonen {
private:
	/*
		This class represents a state of the ukkonen automaton. It has a vector of 
		integers representing the corresponding column and a function that maps each 
		character of the alphabet to a new state.
	*/
	class State {
	public:
		vector<int> column;
		unordered_map<char,int> sigma;
		State() {}
		State(vector<int> column) : column(column) {}
	};

	/*
		This method generates the column of the next state of the automaton, according to the column
		represented by the actual state and the current character of the alphabet.
	*/
	vector<int> nextColumn(const vector<int>& column, const char a, const string& pattern, const int error) {
		vector<int> newColumn = {0};
		int m = pattern.size();
		for (int i = 1; i <= m; ++i) {
			int mn = error+1;
			mn = min(mn, newColumn[i-1]+1);
			mn = min(mn, column[i]+1);
			mn = min(mn, column[i-1]+(pattern[i-1]!=a));
			newColumn.push_back(mn);
		}	
		return newColumn;
	}

	vector<State> automaton; //This is the final automaton.
	unordered_set<char> alphabet; //This represents all the characters of the pattern plus a character that doesn't occur.
	int error; 				 //This represents the error.

	/*
		This method constructs the automaton that will be used later to find approximate matchings.
	*/

	void buildUkkFsm(const string& pattern, const int error) {
		int id = 0;
		int m = pattern.size();
		vector<int> column;
		for (int i = 0; i <= m; ++i) {
			column.push_back(min(i,error+1));
		}
		automaton.push_back(State(column));
		queue<int> N; N.push(0);
		TernaryTree* TT = new TernaryTree();
		TT->searchOrAdd(column, id++);
		while (!N.empty()) {
			int act = N.front(); N.pop();
			for (char a : alphabet) {
				column = nextColumn(automaton[act].column, a, pattern, error);
				int nxtid = TT->searchOrAdd(column, id);
				if (nxtid == id) {
					automaton.push_back(State(column));
					N.push(id++);
				}
				automaton[act].sigma[a] = nxtid;
			}

			//after we built all the transictions from this column, we are only interested in the last element.
			int last = automaton[act].column[m];
			automaton[act].column.clear();
			automaton[act].column.push_back(last);
		}

		delete TT;
	}

public:

	Ukkonen() {}

	Ukkonen(const string& pattern, const int error) : error(error) {
		for (char c : pattern) {
			alphabet.insert(c);
		}
		alphabet.insert(0); //this char is to represent any character that doesn't occur in the pattern.
		buildUkkFsm(pattern, error);
	}

	/*
      	If onlyOccurs is false, this method returns the number of distinct ends of approximate
      	matches found in the text.
      	If onlyOccurs is true, this method returns 0 if no approximate match was found and 1 otherwise.
	*/
	int numTimes(const string& text, const bool onlyOccurs) {
		int curr = 0, n = text.size();
		int numOccurPattern = 0;

		if (automaton[curr].column[0] <= error) {
			++numOccurPattern;
			if (onlyOccurs) {
				return 1;
			}
		}

		for (int i = 0; i < n; ++i) {
			auto it = alphabet.find(text[i]);
			char c = (it == alphabet.end()) ? 0 : text[i];
			curr = automaton[curr].sigma[c];
			if (automaton[curr].column[0] <= error) {
				++numOccurPattern;
				if (onlyOccurs) {
					return 1;
				}
			}
		}

		return numOccurPattern;
	}

};

#endif