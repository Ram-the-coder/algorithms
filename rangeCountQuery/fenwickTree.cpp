/*
	The problem is to find the number of elements in the array in the index range [l, r] that are greater than k.
	We'll first store the elements in the array and the queries.
	Sort them in descending order of the values. (Check the sortNodes function on how exactly we are sorting)
	The concept is we'll add elements in descending order.
	BIT[i] gives the number of elements added to the BIT till index i.
	Since we're processing addition of elements and queries in descending order,
	when we come across a query, all we have to find is queryBIT(r) - queryBIT(l-1) as all the elements added
	upto that point are greater than k, the above equation will give the number of elements present between
	l and r that are greater than k.
	
	Time Complexity: O((n+q).log(n+q))
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

class Node {
public:
	lld queryIndex;
	lld arrayIndex;
	lld low, high;
	lld val;

	// Array element
	Node(lld elVal, lld pos) : queryIndex(-1), arrayIndex(pos), low(0), high(0), val(elVal) {}

	// Query
	Node(lld l, lld r, lld k, lld i) : queryIndex(i), arrayIndex(-1), low(l), high(r), val(k) {} 
};

bool sortNodes(const Node *a, const Node *b) {
	// If the values are same, prioritize queries to array elements
	if(a->val == b->val) 
		return a->queryIndex > b->queryIndex;

	// Otherwise sort by value in descending order
	return a->val > b->val;
}

void update(vll &BIT, lld idx) {
	while(idx < BIT.size()) {
		++BIT[idx];
		idx += idx & (-idx);
	}
}

// query(i) - returns the number of elements present in the BIT till ith element
lld query(const vll &BIT, lld idx) {
	lld count = 0;
	while(idx > 0) {
		count += BIT[idx];
		idx -= idx & (-idx);
	}
	return count;
}


int main() {
	fastio;

	lld n, q;

	cin >> n;

	vector<Node*> arr; // Store all elements to be added and the queries here

	vll BIT(n + 1, 0);

	lld val;
	Node *temp;
	for(lld i=0; i<n; ++i) {
		cin >> val;
		temp = new Node(val, i+1);
		arr.pb(temp);
	}

	// Get the queries
	cin >> q;
	lld l, r, k;

	for(lld i=0; i<q; ++i) {
		cin >> l >> r >> k;
		temp = new Node(l+1, r+1, k, i);
		arr.pb(temp);
	}

	sort(arr.begin(), arr.end(), sortNodes);

	vector<lld> ans(q);

	lld queryCount = 0;

	for(Node *n : arr) {
		if(n->queryIndex == -1) {
			update(BIT, n->arrayIndex);
		} else {
			ans[n->queryIndex] = query(BIT, n->high) - query(BIT, n->low-1);
			if(++queryCount >= q) break; // If all queries have been answered, break
		}
	}

	for(lld a : ans) {
		cout << a << '\n';
	}

	return 0;
}