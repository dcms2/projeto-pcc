#include <bits/stdc++.h>

using namespace std;

/*
	This struct represents a state of the automaton which will be constructed. 
	Each state has a transition function g, the id of the state which the automaton
	goes in case this state fails, and a vector, which stores, for each pattern, 0 if it
	finishes at this state or 1 otherwise.
*/

struct state{
    map<char,int> g;
    set<int> o;
    int f;
    state() {
    	this->f = 0;
    }
};

/*
	This method finishes constructing the automaton, adding the fail state for each state the automaton 
	already has, and simultaneously adding the occurences.
*/

void buildFailFunction(vector<state>& automaton){
	queue<int> Q;
	for(map<char,int>::iterator it = automaton[0].g.begin(); it != automaton[0].g.end(); ++it) 
		Q.push((*it).second);
	while(!Q.empty()){
		int curr = Q.front(); Q.pop();
		for(map<char, int>::iterator it = automaton[curr].g.begin(); it != automaton[curr].g.end(); ++it){
			char a = (*it).first;
			int next = automaton[curr].g[a];
			Q.push(next);
			int b = automaton[curr].f;
			while(automaton[b].g.find(a) == automaton[b].g.end()){
				if(!b) break;
				b = automaton[b].f;
			}
			automaton[next].f = (automaton[b].g.find(a) == automaton[b].g.end()) ? b : automaton[b].g[a];
			int fail = automaton[next].f;
			for(set<int>::iterator it2 = automaton[fail].o.begin(); it2 != automaton[fail].o.end(); ++it2)
				automaton[next].o.insert(*it2);
		}
	}
}


/*
	This method construct the automaton withou the fail function for each state. It simply create all the
	necessary states and connect them according to the patterns.
*/

void buildTransitionFunction(vector<state>& automaton, const vector<string>& patterns){
	int next = 0;
	int s = patterns.size();
	automaton.push_back(state());
	for(int k = 0; k < s; ++k){
		int curr = 0, j = 0, m = patterns[k].size();
		while(j < m && automaton[curr].g.find(patterns[k][j]) != automaton[curr].g.end()){
			curr = automaton[curr].g[patterns[k][j]];
			j++;
		}
		while(j<m){
			automaton[curr].g[patterns[k][j]] = ++next;
			automaton.push_back(state());
			curr = next;
			j++;
		}
		automaton[curr].o.insert(k);
	}
}

/*
    This method construct the automaton we need to process the text and look for patterns. 
    First, we construct the automaton without computing the fail funcion. Then, we compute 
    the fail function and finish computing the ocurrences according to the fails.
*/

vector<state> buildFsm(const vector<string>& patterns){
	vector<state> automaton; //the final automaton
	buildTransitionFunction(automaton, patterns);
	buildFailFunction(automaton);
	return automaton;
}

/*
	This is the main method of the algorithm. This method receives, as parameters, the text
    as a string, a vector containing all the patterns as strings, and the alphabet as a string.
    It returns, then, a vector of vectors. The inner vector at the i-th position of the outer vector will store
    all the occurrences of the i-th pattern in the text. 
*/

vector< vector<int> > ahoCorasick(const string& text, const vector<string>& patterns){
    vector<state> states = buildFsm(patterns);
    int n = text.size(), s = patterns.size();
    vector< vector<int> > occurrences(s);
    int curr = 0;
    for(int i = 0; i < n; ++i){
        while(states[curr].g.find(text[i]) == states[curr].g.end()){
        	if(!curr) break;
            curr = states[curr].f;
        }
        curr = (states[curr].g.find(text[i]) == states[curr].g.end()) ? curr : states[curr].g[text[i]];
        for(set<int>::iterator it = states[curr].o.begin(); it != states[curr].o.end(); ++it)
        	occurrences[*it].push_back(i-patterns[*it].size()+1);
    }
    return occurrences;
}

int main(){
    int TC; scanf("%d", &TC);
    while(TC--){
        string text, pat;
        cin >> text;
        int q; scanf("%d", &q);
        vector<string> patterns; 
        while(q--){
            cin >> pat;
            patterns.push_back(pat);
        }
        vector< vector<int> > vec = ahoCorasick(text, patterns);
        for(int i = 0; i < vec.size(); ++i){
            cout << "Padrao: " << patterns[i] << endl;
           	for(int j = 0; j < vec[i].size(); ++j) cout << vec[i][j] << " ";
           	cout << endl;
         }
        //for(set<int>::iterator it = vec[0].begin(); it != vec[0].end(); ++it) cout << (*it) << endl;   
    }
    return 0;
}
