#ifndef BOYERMOORE
#define BOYERMOORE

#include <bits/stdc++.h>

using namespace std;

class BoyerMoore {
private:

    string pattern;
    unordered_map<char, int> alphabet; //This contains all characters of the pattern plus one that doesn't occur.
    vector<int> S; //This is the good-suffix table of the pattern.
    vector< vector<int> > table; //This is the bad-character table of the pattern.

    /*
        This method computes the border of string str.
    */
    vector<int> buildBorder(const string& str) {
        int i = 0, j = -1, n = str.size();
        vector<int> border(n+1);
        border[0] = -1;
        while (i < n) {
            while (j >= 0 && str[i] != str[j]) {
                j = border[j];
            }
            border[++i] = ++j;
        }
        return border;
    }

    /*
        This method processes the pattern to calculate the good suffix table, which will return,
        for each position, the size of the jump of the text if a mismatch ocurrs in that position.
    */
    void goodSuffixTable() {
        vector<int> b = buildBorder(pattern);
        reverse(pattern.begin(), pattern.end());
        vector<int> bR = buildBorder(pattern);
        reverse(pattern.begin(), pattern.end());
        int m = pattern.size();
        S.resize(m+1);
        for (int i = 0; i <= m; ++i) {
            S[i] = m-b[m];
        }
        for (int l = 0; l <= m; ++l) {
            int j = m-1-bR[l];
            if (S[j] > l-bR[l]) {
                S[j] = l-bR[l];
            }
        }
    }

    /*
        This method processes the pattern to calculate the table of bad characteres.
        The table contains the last index to the left of the position i of the character a,
        for each position i of the pattern and character a of the alphabet.
    */
    void computeBadChairTable() {
        vector<int> lastocc(alphabet.size());
        int m = pattern.size();
        for (int j = 0; j < alphabet.size(); ++j) {
            lastocc[j] = -1;
            table.push_back(vector<int>(m));
        }
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < alphabet.size(); ++j) {
                int a = alphabet[j];
                table[a][i] = lastocc[a];
            }
            lastocc[alphabet[pattern[i]]] = i;
        }
    }

public:

    BoyerMoore(const string& pattern) : pattern(pattern) {
        alphabet[0] = 0; //0 represents any character that doesn't occur in pattern.
        int cnt = 1;
        for (int i = 0; i < pattern.size(); ++i) {
            if (alphabet.find(pattern[i]) == alphabet.end()) {
                alphabet[pattern[i]] = cnt++;
            }
        }
        computeBadChairTable();
        goodSuffixTable();
    }

    /*
      If onlyOccurs is false, this method returns the number of times a pattern was found in the text.
      If onlyOccurs is true, this method returns 0 if no pattern was found and 1 if some pattern was found.
    */
    int numTimes(const string& text, const bool onlyOccurs) {
        int numOccur = 0;
        int n = text.size(), m = pattern.size(), i = 0;
        while (i <= n-m) {
            int j = m-1, x = i + j;
            while (j >= 0 && text[x] == pattern[j]) {
                --j;
                --x;
            }
            if (j < 0) {
                ++numOccur;
                if (onlyOccurs) {
                    return 1;
                }
                i += S[0];
            } else {
                unordered_map<char, int>::iterator it = alphabet.find(text[i+j]);
                int pos = (it == alphabet.end()) ? 0 : it->second;
                i += max(j-table[pos][j], S[j]);
            }
        }
        return numOccur;
    }
};

#endif