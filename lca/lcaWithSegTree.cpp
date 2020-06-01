// Ansewring Lowest Common Ancestor queries with the help of segment tree
#include<bits/stdc++.h>
#define lld long long
#define pb push_back
#define ipv(v) for(lld i=0; i<v.size(); ++i) cin >> v[i]
#define dispv(v) for(lld i=0; i<v.size();++i) cout << v[i] << " "; cout << '\n'
using namespace std;

class Node {
	public:
	lld val;
	vector<Node*> adjacency;

	Node(lld v) : val(v) {}
};

/**** Start LCA related functions and data *****/
unordered_map<lld, Node*> etIndexToNode;
unordered_map<Node*, lld> etNodeToIndex;
unordered_set<Node*> visited;
unordered_map<lld, lld> firstOccurence; // First occurence of a a node in the euler tour
										// i.e. firstOccurence[i] = the index of the first occurence of euler-tour-index i in the array et

// Performs the Euler Tour in a DFS fashion and pushes the nodes it visits into the arary et
void etDFS(Node *root, vector<lld> &et, lld &index) {
	if(!root) return;

	lld curIndex = index++;

	visited.insert(root);
	etIndexToNode[curIndex] = root;
	etNodeToIndex[root] = curIndex;
	et.pb(curIndex); // Visiting curIndex
	firstOccurence[curIndex] = et.size() - 1;

	for(Node* child : root->adjacency) {
		if(visited.find(child) != visited.end()) continue; // If already visited continue with the next child
		etDFS(child, et, index);
		et.pb(curIndex); // Visiting curIndex
	}
}


vector<lld> eulerTour(Node *root) {
	etIndexToNode.clear();
	etNodeToIndex.clear();
	firstOccurence.clear();
	visited.clear();

	lld index = 1;
	vector<lld> et;
	etDFS(root, et, index);
	return et;
}

/**** End LCA related functions and data *****/

/**** Start Segment Tree Data Structure Functions ****/
void buildSegTree(vector<lld> &segTree, const vector<lld> &arr, lld low, lld high, lld pos) {
	if(low == high) {
		segTree[pos] = arr[low];
		return;
	}

	lld mid = (high + low) / 2;

	buildSegTree(segTree, arr, low, mid, 2*pos + 1);
	buildSegTree(segTree, arr, mid+1, high, 2*pos + 2);

	segTree[pos] = min(segTree[2*pos + 1], segTree[2*pos + 2]);
}

vector<lld> constructSegTree(const vector<lld> &arr) {
	lld n = arr.size();
	lld stLen = pow(2, ceil(log2(n)) + 1) - 1;
	vector<lld> segTree(stLen, LLONG_MAX);
	buildSegTree(segTree, arr, 0, n-1, 0);
	return segTree;
}

lld rmq(const vector<lld> &segTree, lld qlow, lld qhigh, lld low, lld high, lld pos) {
	if(qlow > qhigh) {
		lld temp = qlow;
		qlow = qhigh;
		qhigh = temp;
	}

	if(qlow <= low && qhigh >= high) return segTree[pos]; // Complete Overlap

	if(qlow > high || qhigh < low) return LLONG_MAX; // No overlap

	// Parial overlap
	lld mid = (high + low) / 2;
	return min(	rmq(segTree, qlow, qhigh, low, mid, 2*pos + 1),
				rmq(segTree, qlow, qhigh, mid+1, high, 2*pos + 2));
}

/**** End Segment Tree Data Structure Functions ****/

int main()
{
	lld n; // no of nodes in the tree
	cin >> n;

	// Input the node values
	vector<Node*> nodes(n);
	lld val;
	for(lld i=0; i<n; ++i) {
		cin >> val;
		nodes[i] = new Node(val);
	}

	// Input the edges
	lld u, v;
	for(lld i=0; i<n-1; ++i) {
		cin >> u >> v;
		nodes[u]->adjacency.pb(nodes[v]);
		nodes[v]->adjacency.pb(nodes[u]);
	}

	// Construct the Euler Tour
	vector<lld> et = eulerTour(nodes[0]);

	// Construct the segment tree for range minimum queries
	vector<lld> segTree = constructSegTree(et);

	// Perform the queries
	lld q, n1, n2;
	cin >> q;

	for(lld i=0; i<q; ++i) {
		cin >> n1 >> n2;
		lld lcaIndex = rmq(segTree, firstOccurence[etNodeToIndex[nodes[n1]]], firstOccurence[etNodeToIndex[nodes[n2]]], 0, et.size()-1, 0);
		Node *lcaNode = etIndexToNode[lcaIndex];
		cout << "Found node with value " << lcaNode->val << " as LCA\n";
	}

	return 0;
}