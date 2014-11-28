#ifndef WUMANBER
#define WUMANBER

#include <bits/stdc++.h>
#include "Bitset.h"

using namespace std;

class WuManber {
private:

    unordered_map<char,int> alphabet; //This contains all characters from the pattern plus one character that doesn't occur.
    Bitset *C, msb;
    int patternSize;

    void char_mask(const string& pattern) {
        C = new Bitset[alphabet.size()];
        Bitset K = Bitset(pattern.size(),1);

        for (auto& it : alphabet) {
            C[it.second] = Bitset(K);
        }
        
        K.clear(0);
        for (char c : pattern) {
            int i = alphabet[c];
            C[i].andd(K);
            K.shift();
            K.set(0);
        }
        K.delete_array();
    }

public:

    WuManber() {}

    WuManber(const string& pattern) : patternSize(pattern.size()) {
        int cnt = 0;
        alphabet[0] = cnt++; //This is to represent any character from the text that doesn't occur in pattern.
        for (char c : pattern) {
            if (alphabet.find(c) == alphabet.end()) {
                alphabet[c] = cnt++;
            }
        }
        char_mask(pattern);
        msb = Bitset(patternSize);
        msb.set(patternSize-1);
    }

    int numTimes(const string& text, int error, const bool onlyOccurs) {
        int numOccur = 0;

        Bitset* S = new Bitset[error+1];
        
        S[0] = Bitset(patternSize,1);
        for (int q = 1; q <= error; ++q) {
            S[q] = Bitset(S[q-1]);
            S[q].shift();
        }

        Bitset S_1 = Bitset(S[0]), S_2 = Bitset(S[0]), aux = Bitset(S[0]);

        for (int j = 0; j < text.size(); ++j) {
            auto it = alphabet.find(text[j]);
            int i = (it == alphabet.end()) ? 0 : it->second;
            
            S_1.update_array(S[0]);
            S[0].shift();
            S[0].orr(C[i]);

            for (int q = 1; q <= error; ++q) {
                S_2.update_array(S[q]);
                S[q].shift(); //Sq = Sq << 1
                S[q].orr(C[i]); //Sq = Sq | C[text[j]]
                aux.update_array(S_1); aux.shift(); //aux = S' << 1
                S[q].andd(aux); //Sq = Sq & (S' << 1)
                aux.update_array(S[q-1]); aux.shift(); //aux = S[q-1] << 1
                S[q].andd(aux); //Sq = Sq & (S[q-1] << 1)
                S[q].andd(S_1); //Sq = Sq & S'
                S_1.update_array(S_2);
            }

            if (S[error] < msb) {
                //printf("%d\n", j);
                numOccur++;
                if (onlyOccurs) {
                    return 1;
                }
            }
        }


        for (int i = 0; i <= error; ++i) {
            S[i].delete_array();
        }

        delete[] S;
        S = NULL;

        return numOccur;
    }

};

#endif