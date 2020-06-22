/* 	Jarvis's Algorithm to find Convex Hull
	Time complexity: O(m*n) where m is the number of points on the hull
	and n is the number of total points
	Worst Case Time complexity: O(n^2)
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
#define CLOCKWISE 1
#define COUNTER_CLOCKWISE -1
#define COLLINEAR 0
using namespace std;

class Point {
public:
	lld x, y;

	Point() {}
	Point(lld i_x, lld i_y) : x(i_x), y(i_y) {}

	string print() {
		return ("(" + to_string(x) + ", " + to_string(y) + ")");
	}
};

int orientation(Point p, Point q, Point r) {

	lld cross_product = (q.x - p.x)*(r.y - q.y) - (r.x - q.x)*(q.y - p.y);

	if(cross_product < 0) return CLOCKWISE;
	if(cross_product > 0) return COUNTER_CLOCKWISE;
	return COLLINEAR;
}

/***** Jarvis's Algorithm *****/
vector<Point> convexHull(vector<Point> &points) {
	if(points.size() < 3) return vector<Point>();

	// Get the left most point
	lld left_most = 0;
	for(lld i=1; i<points.size(); ++i) {
		if(points[i].x < points[left_most].x) {
			left_most = i;
		}
	}

	lld p = left_most, q;
	vector<Point> hull;

	do {

		hull.pb(points[p]);

		// Find a point q such that for every other point r, orientation(p, q, r) is COUNTER_CLOCKWISE
		q = (p+1) % points.size();
		for(lld r=0; r<points.size(); ++r) {
			// If orientation(p, q, r) is clockwise then orientation(p, r, q) will be counter clockwise
			// orientation(p, r, k) will also be counter clockwise for all k where orientation(p, q, k) was counter clockwise
			// thus r is more counter clockwise than q, so set q as r.
			if(orientation(points[p], points[q], points[r]) == CLOCKWISE) q = r;	
		}
		
		p = q; // q is a part of the convex hull

	} while(p != left_most);

	return hull;
}

int main() {
	fastio;

	lld num_points;

	cin >> num_points;

	vector<Point> points(num_points);
	lld x, y;
	for(lld i=0; i<num_points; ++i) {
		cin >> x >> y;
		points[i] = Point(x, y);
	}

	vector<Point> hull = convexHull(points);
	for(lld i=0; i<hull.size(); ++i) {
		cout << hull[i].print() << " ";
	}
	cout << '\n';
		
	return 0;
}