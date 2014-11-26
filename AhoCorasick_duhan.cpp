#include <bits/stdc++.h>

using namespace std;

class AhoCorasick {
private:
    /*
      This class represents a state of the automaton which will be constructed. 
      Each state has a transition function g, the id of the state which the automaton
      goes in case this state fails, and a set, which stores all the patterns that occurs at
      that state.
    */
    class State {
    public:
        unordered_map<char,int> g;
        int f;
        int qnt;
        State(): f(0), qnt(0) {}
    };

    vector<State> automaton; //This is the final automaton of the patterns.

    /*
      This method finishes constructing the automaton, adding the fail state for each state the automaton 
      already has.
    */
    void buildFailFunction() {
        queue<int> Q;
        for (auto& it : automaton[0].g) {
            Q.push(it.second);
        }
        while (!Q.empty()) {
            int curr = Q.front(); Q.pop();
            for (auto& it : automaton[curr].g) {
                char a = it.first;
                int next = it.second;
                Q.push(next);
                int b = automaton[curr].f;
                while (automaton[b].g.find(a) == automaton[b].g.end()) {
                    if (!b) break;
                    b = automaton[b].f;
                }
                auto it2 = automaton[b].g.find(a);
                automaton[next].f = (it2 == automaton[b].g.end()) ? b : it2->second;
                int fail = automaton[next].f;
                automaton[next].qnt += automaton[fail].qnt;
            }
        }
    }

    /*
      This method construct the automaton without the fail function for each state.
      It simply creates all the necessary states and connect them according to the patterns.
    */
    void buildTransitionFunction(const vector<string>& patterns) {
        int next = 0, s = patterns.size();
        automaton.push_back(State());

        for (int k = 0; k < s; ++k) {
            int curr = 0, j = 0, m = patterns[k].size();

            auto it = automaton[curr].g.find(patterns[k][j]);
            while (j < m && it != automaton[curr].g.end()) {
                curr = it->second;
                j++;
                it = automaton[curr].g.find(patterns[k][j]);
            }

            while (j < m) {
                automaton[curr].g[patterns[k][j]] = ++next;
                automaton.push_back(State());
                curr = next;
                j++;
            }

            automaton[curr].qnt++;
        }
    }

public:

    AhoCorasick(const vector<string>& patterns) {
        buildTransitionFunction(patterns);
        buildFailFunction();
    }

    /*
      If onlyOccurs is false, this method returns the number of times a pattern was found in the text.
      If onlyOccurs is true, this method returns 0 if no pattern was found and 1 if some pattern was found.
    */
    int numTimes(const string& text, const bool onlyOccurs) {
        int n = text.size();
        int curr = 0, numOccurPattern = 0;
        for (int i = 0; i < n; ++i) {
            while (automaton[curr].g.find(text[i]) == automaton[curr].g.end()) {
                if(!curr) break;
                curr = automaton[curr].f;
            }
            auto it = automaton[curr].g.find(text[i]);
            curr = (it == automaton[curr].g.end()) ? curr : it->second;
            numOccurPattern += automaton[curr].qnt;
            if (onlyOccurs && numOccurPattern) return 1;
        }
        return numOccurPattern;
    }
};

int main() {
    int n;
    while (cin >> n) {
        vector<string> patterns(n);
        for (int i = 0; i < n; ++i) cin >> patterns[i];
        AhoCorasick aho = AhoCorasick(patterns);

        cin >> n;
        while (n--) {
            string text; cin >> text;
            printf("num of matches: %d\n", aho.numTimes(text, false));
            printf("found some: %s\n", aho.numTimes(text, true) ? "Yes" : "No");
        }
    }
    return 0;
}
