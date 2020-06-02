/* Range Minimum Query using segment tree data structure
	Segment Tree can handle updates efficiently unlike sparse table
	Same comlexity for all types of queries
	Preprocessing - O(n)
	Query - O(logn)
	Update - O(logn)
*/
#include<bits/stdc++.h>
#define lld long long
#define ipv(v) for(lld i=0; i<v.size(); ++i) cin >> v[i]
using namespace std;


void buildTree(const vector<lld> &arr, vector<lld> &segTree, lld low, lld high, lld pos) {
	// low, high signifies the bounds of the array
	// at each node in the segment tree, we'll divide the array at that point into two halves

	// The segment tree is stored as an array
	// (Left Half after the split) Left child of node with index i = 2*i + 1
	// (Right Half after the split) Right child of node with index i = 2*i + 2
	// segTree[pos] = min(segTree[2*i + 1], segTree[2*i + 2])

	if(low == high) {
		segTree[pos] = arr[low];
		return;
	}

	lld mid = (high + low) / 2;
	buildTree(arr, segTree, low, mid, 2*pos + 1);
	buildTree(arr, segTree, mid+1, high, 2*pos + 2);
	segTree[pos] = min(segTree[2*pos + 1], segTree[2*pos + 2]);
}

vector<lld> constructSegTree(const vector<lld> &arr) {
	lld n = arr.size();
	lld stLen = pow(2, ceil(log2(n)) + 1) - 1;
	vector<lld> segTree(stLen, LLONG_MAX);
	buildTree(arr, segTree, 0, n-1, 0);
	return segTree;
}

lld rmq(const vector<lld> &segTree, const lld qlow, const lld qhigh, lld low, lld high, lld pos) {

	if(qlow > qhigh) {
		lld temp = qlow;
		qlow = qhigh;
		qhigh = temp;
	}

	if(qlow <= low && qhigh >= high) return segTree[pos]; // Total Overlap

	if(qlow > high || qhigh < low) return LLONG_MAX; // No Overlap

	// Partial Overlap
	lld mid = (low + high) / 2;

	return min(	rmq(segTree, qlow, qhigh, low, mid, 2*pos + 1),
				rmq(segTree, qlow, qhigh, mid+1, high, 2*pos + 2));
}

int main() {

	// Get the array
	lld n;
	cin >> n;
	vector<lld> arr(n);
	ipv(arr);

	// Construct segment tree
	vector<lld> segTree = constructSegTree(arr);

	// Get the queries
	lld q, low, high;
	cin >> q;

	for(lld i=0; i<q; ++i) {
		cin >> low >> high;
		cout << rmq(segTree, low, high, 0, n-1, 0) << '\n';
	}

	return 0;
}