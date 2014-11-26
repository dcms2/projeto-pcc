#include <bits/stdc++.h>

using namespace std;

/*
	The 2 structs (Node and the ternary tree of nodes) below are constructed to make the proccess of checking if a state of the
	automaton already exists in the set of states when the automaton is being constructed.
	With the ternary tree, the cost of searching for a state turns out to be linear.
*/

struct Node{
	Node* left;
	Node* mid;
	Node* right;
	Node () {}
	Node(Node* left, Node* mid, Node* right){
		this->left = left;
		this->mid = mid;
		this->right = right;
	}
};

struct TernaryTree{
	Node* head; 

	TernaryTree() {
		head = new Node();
	}

	/*
		this method returns false if the state to be added already exists
		or returns true and add it to the tree if it does not.
	*/
	bool searchOrAdd(vector<int> new_column){
		Node* act = head;
		int i = 1;
		bool ok = 1;
		while(i < (int)new_column.size()){
			int dif = new_column[i]-new_column[i-1];
			if(dif==-1){
				if(act->left != NULL) act = act->left;
				else{
					ok = 0;
					act->left = new Node();
					act = act->left;
				}
			}else if(dif == 0){
				if(act->mid != NULL) act = act->mid;
				else {
					ok = 0;
					act->mid = new Node();
					act = act->mid;
				}
			} else {
				if(act->right != NULL) act = act->right;
				else{
					ok = 0;
					act->right = new Node();
					act = act->right;
				}
			}
			i++;
		}
		return !ok;
	}
};



int main(){
	TernaryTree test_tree = TernaryTree();
	int m; scanf("%d", &m);
	int n; scanf("%d", &n);
	while(n--){
		vector<int> vec;
		for(int i = 0; i < m; ++i){
			int x; scanf("%d", &x);
			vec.push_back(x);
		}
		printf("%d\n", test_tree.searchOrAdd(vec));
	}
	return 0;
}