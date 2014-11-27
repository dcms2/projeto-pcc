#include <bits/stdc++.h>

using namespace std;

/*
    This method computes the fail function of the automaton, and finishes computing the occurences at each state simultaneously.
*/

void buildFailFunction(map< pair <int, char>, int>& g, map< int, int >&f, map<int,  set<int> >& o, const string& alphabet){
    queue<int> Q;
    int q = alphabet.size();
    for(int i = 0; i < q; ++i){
        char a = alphabet[i];
        if(g[make_pair(0, a)] != 0){
            Q.push(g[make_pair(0,a)]);
            f[g[make_pair(0,a)]] = 0;
        }
    }
    while(!Q.empty()){
        int curr = Q.front(); Q.pop();
        for(int i = 0; i < q; ++i){
            char a = alphabet[i];
            if(g.find(make_pair(curr, a)) != g.end()){
                int next = g[make_pair(curr, a)];
                Q.push(next);
                int b = f[curr];
                while(g.find(make_pair(b, a))==g.end())
                    b = f[b];
                f[next] = g[make_pair(b, a)];
                for(set<int>::iterator it = o[f[next]].begin(); it != o[f[next]].end(); ++it)
                    o[next].insert(*it);
            }
        }
    }
}

/*
    this method computes all the transitions between the states of the AhoCorasick automaton, which will be stored at g. 
    The method stores, as well, some of the occurrences of the patterns in each of the states, but not all of it; the rest
    of it is computed at the buildFailFunction.
*/

void buildTransitionFunction(const vector<string>& patterns, map< pair <int, char>, int>& g, map <int, set<int> >& o, const string& alphabet){
    int next = 0;
    int s = patterns.size();
    for(int k = 0; k < s; ++k){
        int curr = 0, j = 0, m = patterns[k].size();
        while(j < m && g.find(make_pair(curr, patterns[k][j])) != g.end()){
            curr = g[make_pair(curr, patterns[k][j])];
            j++;
        }
        while(j < m){
            g[make_pair(curr, patterns[k][j])] = ++next;
            curr = next;
            j++;
        }
        o[curr].insert(k);
    }
    int q = alphabet.size();
    for(int i = 0; i < q; ++i){
        char a = alphabet[i];
        if(g.find(make_pair(0, a))==g.end())
            g[make_pair(0,a)] = 0;
    }
}

/*
    This method computes all the tables we need in the main method, which are: the transition function of the automaton, 
    the fail fuction of the automaton and the occurrences of the patterns at each state of the automaton.
*/
void buildFsm(const vector<string>& patterns, map< pair <int, char>, int >& g, map< int, int >& f, map < int, set<int> >& o, const string& alphabet){
    buildTransitionFunction(patterns, g, o, alphabet);
    buildFailFunction(g, f, o, alphabet);
}

/*
    This is the main method of the algorithm. This method receives, as parameters, the text
    as a string, a vector containing all the patterns as strings, and the alphabet as a string.
    It returns, then, a vector of sets. The set at the i-th position of the vector will store
    all the occurrences of the i-th pattern in the text. 
*/
vector< set <int> > ahoCorasick(const string& text, const vector<string>& patterns, const string& alphabet){
    map< pair <int, char>, int > g;
    map< int, int > f;
    map <int,  set<int> > o;
    buildFsm(patterns, g, f, o, alphabet);
    int n = text.size(), s = patterns.size();
    vector< set<int> > occurrences(s);
    int curr = 0;
    for(int i = 0; i < n; ++i){
        while(g.find(make_pair(curr, text[i])) == g.end())
            curr = f[curr];
        curr = g[make_pair(curr, text[i])];
        for(set<int>::iterator it = o[curr].begin(); it != o[curr].end(); ++it)
            occurrences[*it].insert(i-(int)patterns[*it].size()+1);
    }
    return occurrences;
}


//tem que ver como é a leitura e tal pra botar no main, ne? essa que ta ai foi pra testar com a NHAY

int main(){
    int n;
    bool first = 1;
    while(scanf("%d", &n) == 1){
        string text, pat;
        cin >> pat >> text;
        vector<string> patterns; patterns.push_back(pat);
        string alphabet = "abcdefghijklmnopqrstuvxwyz";
        vector< set<int> > vec = ahoCorasick(text, patterns, alphabet);
        for(set<int>::iterator it = vec[0].begin(); it != vec[0].end(); ++it) cout << (*it) << endl;   
    }
    return 0;
}