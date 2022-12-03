#include <iostream>
#include <string>
#include <vector>
typedef long long ll;
using namespace std;

struct Node { //node class 
	ll val; // value of the node 
	Node *l, *r; // left and right child of the node

	Node(ll x) : val(x), l(nullptr), r(nullptr) {} //constructor
	Node(Node *ll, Node *rr) {  //constructor
		l = ll, r = rr; // left and right child of the node
		val = 0; // value of the node
		if (l) val += l->val; // if left child exists, add its value to the value of the node
		if (r) val += r->val;   // if right child exists, add its value to the value of the node
	}
	Node(Node *cp) : val(cp->val), l(cp->l), r(cp->r) {} //constructor
};

int n, cnt = 1; // n is the number of elements in the array, cnt is the number of nodes in the segment tree
ll a[200001]; // array of elements 
Node* roots[200001]; // roots[i] is the root of the segment tree after i-th update

Node* build(int l = 1, int r = n) { // build the segment tree
	if (l == r) return new Node(a[l]); // if the segment is a single element, return a node with the value of the element
	int mid = (l + r) / 2; // mid of the segment
	return new Node(build(l, mid), build(mid + 1, r)); // return a node with the value of the sum of the values of its children
}

Node* update(Node* node, int val, int pos, int l = 1, int r = n) { // update the segment tree
	if (l == r) return new Node(val); // if the segment is a single element, return a node with the value of the element
	int mid = (l + r) / 2; // mid of the segment
	if (pos > mid) return new Node(node->l, update(node->r, val, pos, mid + 1, r)); // if the position is in the right child, return a node with the value of the sum of the values of its children
	else return new Node(update(node->l, val, pos, l, mid), node->r); // if the position is in the left child, return a node with the value of the sum of the values of its children
}

ll query(Node* node, int a, int b, int l = 1, int r = n) { // query the segment tree
	if (l > b || r < a) return 0; // if the segment is outside the query range, return 0
	if (l >= a && r <= b) return node->val; // if the segment is inside the query range, return the value of the node
	int mid = (l + r) / 2; // mid of the segment
	return query(node->l, a, b, l, mid) + query(node->r, a, b, mid + 1, r); // return the sum of the values of the left and right child
}

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	int q; // q is the number of queries
	cin >> n >> q; // read n and q
	for (int i = 1; i <= n; i++) cin >> a[i]; // read the array
	roots[cnt++] = build(); // build the segment tree and store it in roots[1]

	while (q--) { // for each query
		int t; // t is the type of the query
		cin >> t; // read t
		if (t == 1) { // if the query is an update
			int k, i, x; // k is the index of the segment tree, i is the index of the element to be updated, x is the new value of the element
			cin >> k >> i >> x; // read k, i and x
			roots[k] = update(roots[k], x, i); // update the segment tree and store it in roots[k]
		} else if (t == 2) { // if the query is a merge
			int k, l, r; // k is the index of the segment tree, l and r are the indices of the segment trees to be merged
			cin >> k >> l >> r; // read k, l and r
			cout << query(roots[k], l, r) << '\n'; // query the segment tree and print the result
		} else {
			int k; // k is the index of the segment tree
			cin >> k; // read k
			roots[cnt++] = new Node(roots[k]); // copy the segment tree and store it in roots[cnt]
		}
	}
	return 0;
}
