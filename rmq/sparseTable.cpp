// Range Minimum Query using sparse table data structure
#include<bits/stdc++.h>
#define lld long long
#define ipv(v) for(lld i=0; i<v.size(); ++i) cin >> v[i]
using namespace std;

vector<vector<lld>> buildSparseTable(const vector<lld> &arr) {
	lld n = arr.size();

	// Create the sparse table
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
	lld len = high - low + 1;
	lld k = log2(len);
	return min(sparse[low][k], sparse[low + len - pow(2, k)][k]);
}

int main() {

	// Get the array
	lld n;
	cin >> n;
	vector<lld> arr(n);
	ipv(arr);

	// Build Sparse Table
	vector<vector<lld>> sparse = buildSparseTable(arr);

	// Get the queries
	lld q, low, high;
	cin >> q;

	for(lld i=0; i<q; ++i) {
		cin >> low >> high;
		cout << rmq(sparse, low, high) << '\n';
	}

	return 0;
}
