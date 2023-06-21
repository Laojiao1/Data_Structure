#include <bits/stdc++.h>
using namespace std;
int Post[1001], In[1001];
struct TreeNode {
	int data;
	TreeNode *left;
	TreeNode *right;
}Node;
typedef struct TreeNode *Tree;

Tree PostAndIn(int front1, int rear1, int front2, int rear2) {
	Tree root = new struct TreeNode;
	root->data = Post[rear1];
	root->left = root->right = NULL;
	int p = front2;
	while(In[p] != Post[rear1]) {
		p++;
	}
	int cnt = p - front2;
	if(p != front2) 
		root->left = PostAndIn(front1, front1+cnt-1, front2, p-1);
	if(p != rear2)
		root->right = PostAndIn(front1+cnt, rear1-1, p+1, rear2);
	return root;
}

void Sequence(Tree root) {
	queue<Tree> t;
	if(root) {
		t.push(root);
	}
	while(!t.empty()) {
		root = t.front();
		t.pop();
		cout << root->data;
		if(root->left) {
			t.push(root->left);
		}
		if(root->right) {
			t.push(root->right);
		} 
		if(!t.empty()) {
			cout << " ";
		}
	}
}

int main() {
	int n, i;
	cin >> n;
	for(i = 0; i < n; i++) {
		cin >> Post[i];
	}
	for(i = 0; i < n; i++) {
		cin >> In[i];
	}
	Tree root = PostAndIn(0, n-1, 0, n-1);
	Sequence(root);
	return 0;
}
