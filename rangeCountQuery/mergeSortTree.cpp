/* MergeSortTree is a segment tree where each node instead of storing some range computation,
   stores the sorted array in the range.
   Now we can select any range in the array and perform some computation on that, 
   that would run efficiently if the array was sorted.

   Here in this example we answer queries to find the number of elements in array in the range [l, r] that is greater k
   We can easily solve this in O(Q.N) time and O(1) space
   By using merge sort tree, we are solving this in O(NlogN + Q.logN.logN) time and O(NlogN) space

   The reason for using merge sort tree here is because we can find the number of elements greater than k in an array
   in O(logN) time if its was sorted and in O(N) time if it wasn't.
*/
#include<bits/stdc++.h>
#define lld long long
#define vll vector<lld>
#define vvll vector<vector<lld>>
#define pb push_back
#define mp make_pair
#define ipv(v) for(lld i=0; i<v.size(); ++i) cin >> v[i]
#define dispv(v) for(lld i=0; i<v.size();++i) cout << v[i] << " "; cout << '\n'
#define dispa(a, n) for(lld i=0; i<n; ++i) cout << a[i] << " "; cout << '\n'
#define fastio ios_base::sync_with_stdio(false); cin.tie(NULL)
using namespace std;

void buildMST(vll &a, vvll &segTree, lld low, lld high, lld pos) {
	if(low == high) {
		segTree[pos].pb(a[low]);
		return;
	}

	lld mid = (high + low) / 2;
	buildMST(a, segTree, low, mid, 2*pos + 1);
	buildMST(a, segTree, mid+1, high, 2*pos + 2);

	segTree[pos].resize(segTree[2*pos+1].size() + segTree[2*pos+2].size());
	merge(segTree[2*pos + 1].begin(), 
			segTree[2*pos + 1].end(), 
			segTree[2*pos + 2].begin(), 
			segTree[2*pos + 2].end(), 
			segTree[pos].begin());
}

vvll constructMST(vll &a) {
	lld n = a.size();
	lld stLen = pow(2, ceil(log2(n)) + 1) - 1;
	vvll segTree(stLen, vll());
	buildMST(a, segTree, 0, a.size()-1, 0);
	return segTree;
}

// Should return number of elements greater than k in the range [qlow, qhigh] in the array a
lld query(const vvll &segTree, lld qlow, lld qhigh, lld low, lld high, lld k, lld pos) {
	if(qlow > high || qhigh < low) return 0; // No overlap

	// Total overlap
	if(qlow <= low && qhigh >= high) {
		return segTree[pos].size() - (upper_bound(segTree[pos].begin(), segTree[pos].end(), k) - segTree[pos].begin()); 	
	} 

	// Partial overlap
	lld mid = (high + low) / 2;
	return query(segTree, qlow, qhigh, low, mid, k, 2*pos+1) + query(segTree, qlow, qhigh, mid+1, high, k, 2*pos+2);
}


void displayMST(const vvll &segTree) {
	for(lld i=0; i<segTree.size(); ++i) {
		printf("MST[%d]: ", i);
		for(lld j=0; j<segTree[i].size(); ++j)
			printf("%d ", segTree[i][j]);
		printf("\n");
	}
}

int main() {
	fastio;

	lld n, q;

	// Get the array
	cin >> n;
	vll a(n);
	ipv(a);

	// Construct the segment tree / merge sort tree
	vvll segTree = constructMST(a);

	// Get the queries
	cin >> q;
	lld l, r, k;

	for(lld i=0; i<q; ++i) {
		cin >> l >> r >> k;
		cout << query(segTree, l, r, 0, a.size()-1, k, 0) << '\n';
	}

	return 0;
}