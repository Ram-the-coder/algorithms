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

/**** Start Sparse Table Related functions *****/

vector<vector<lld>> buildSparseTable(const vector<lld> &arr) {
	lld n = arr.size();

	// sparse[i][j] = The minimum of the 2^j elements starting from index i
	vector<vector<lld>> sparse(n, vector<lld>(floor(log2(n)) + 1));
	
	// Initialize the first column
	for(lld i=0; i<n; ++i)
		sparse[i][0] = arr[i]; // As per definition

	// Fill up the rest of the columns
	for(lld j=1; pow(2, j) <= n; ++j) {
		for(lld i=0; i + pow(2, j) - 1 < n; ++i) {
			sparse[i][j] = min(sparse[i][j-1], sparse[i + pow(2, j-1)][j-1]); 
		}
	}

	return sparse;
}

// Range Minimum Query
// Returns the minimum value between low and high inclusive
lld rmq(const vector<vector<lld>> &sparse, lld low, lld high) {
	if(low > high) {
		lld temp = low;
		low = high;
		high = temp;
	}

	lld len = high - low + 1;
	lld k = log2(len);
	return min(sparse[low][k], sparse[low + len - pow(2, k)][k]);
}

/**** End Sparse Table Related functions *****/

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

	// Construct the sparse table for range minimum queries
	vector<vector<lld>> sparse = buildSparseTable(et);

	// Perform the queries
	lld q, n1, n2;
	cin >> q;

	for(lld i=0; i<q; ++i) {
		cin >> n1 >> n2;
		lld lcaIndex = rmq(sparse, firstOccurence[etNodeToIndex[nodes[n1]]], firstOccurence[etNodeToIndex[nodes[n2]]]);
		Node *lcaNode = etIndexToNode[lcaIndex];
		cout << "Found node with value " << lcaNode->val << " as LCA\n";
	}

	return 0;
}