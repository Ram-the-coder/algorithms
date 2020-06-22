/*
	Graham's Scan to find convex hull
	Time Complexity: O(nlogn)
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

template <class T>
void swap(vector<T> a, lld i, lld j) {
	T temp = a[i];
	a[i] = a[j];
	a[j] = temp;
}

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

lld distSq(Point a, Point b) {
	return (b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y);
}


/***** Graham's Algorithm *****/
vector<Point> convexHull(vector<Point> &points) {

	vector<Point> hull;

	if(points.size() < 3) return hull;

	// Find the bottom most point - p0
	// The point with min y
	// If more than one point exists with the same min y coordinate, 
	// choose the one with the min x coordinate
	lld bottom_most = 0;
	for(lld i=1; i<points.size(); ++i) {
		if(points[i].y < points[bottom_most].y) 
			bottom_most = i;
		else if(points[i].y == points[bottom_most].y && points[i].x < points[bottom_most].x)
			bottom_most = i;
	}

	// Bring the bottom most point (p0) to 0 index
	swap(points, 0, bottom_most);

	// Sort the rest of the points by polar angle with respect to p0
	sort(points.begin() + 1, points.end(), [&points](const Point &p1, const Point &p2) -> bool {
		int ori = orientation(points[0], p1, p2);

		if(ori == COLLINEAR) return distSq(points[0], p1) < distSq(points[0], p2);

		return ori == COUNTER_CLOCKWISE;
	});

	// If two or more points have the same polar angle with respect to p0
	// Then just keep the farthest point
	lld mod_size = 1; // Size of the array when applying the above modificiation

	for(lld i=1; i<points.size(); ++i) {
		while(i < points.size()-1 && orientation(points[0], points[i], points[i+1]) == COLLINEAR) ++i;
		points[mod_size++] = points[i];
	}

	if(mod_size < 3) return hull;

	// p0 and points[1] belong to the hull by default
	hull.pb(points[0]);
	hull.pb(points[1]);
	hull.pb(points[2]);

	for(lld i=3; i<mod_size; ++i) {
		// prev - The last but one point in the hull
		// cur - The last point in the hull
		// next - points[i]
		// If orientation(prev, cur, next) != COUNTER_CLOCKWISE then remove cur from the hull
		// Because for all points the hull, all other points remain in counter clockwise direction only
		while(orientation(hull[hull.size()-2], hull[hull.size()-1], points[i]) != COUNTER_CLOCKWISE) {
			hull.pop_back();
		}

		hull.pb(points[i]);
	}

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