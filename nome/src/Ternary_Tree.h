#ifndef TERNARYTREE
#define TERNARYTREE

#include <bits/stdc++.h>

using namespace std;

/*
  With the ternary tree, the cost of checking if a state is in the automaton turns out to be linear in the size of the state.
*/

class TernaryTree {
private:
    class Node {
    public:
        int id;
        Node *left, *mid, *right;
        Node(int id) : id(id), left(NULL), mid(NULL), right(NULL) {}
    };

    Node* head; 

public:

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

#endif