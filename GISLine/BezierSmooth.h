#pragma once
#include<iostream>
#include<time.h>
#include<vector>
#include<queue>
#include<unordered_map>
#include<string>

using namespace std;
struct Point {
	double x;
	double y;
	double norm()
	{
		return sqrt(x * x + y * y);
	}
	Point(double x_, double y_) {
		x = x_;
		y = y_;
	}
	Point() {
		x = 0.0;
		y = 0.0;
	}
	Point operator*=(float a) {
		x *= a;
		y *= a;
		return *this;
	}
	Point operator* (double a) {
		double xx = x * a;
		double yy = y * a;
		return Point(xx, yy);
	}
	Point operator+(const Point a) {
		double xs = a.x + x;
		double ys = a.y + y;
		return Point(xs, ys);
	}
	Point operator-(const Point a) {
		double xs = -a.x + x;
		double ys = -a.y + y;
		return Point(xs, ys);
	}
	Point operator=(const Point& a) {
		x = a.x;
		y = a.y;
		return *this;
	}
	Point operator+=(const Point& a) {
		x += a.x;
		y += a.y;
		return *this;
	}
};

class BezierSmooth
{
public:
	BezierSmooth()
	{
	}
	~BezierSmooth()
	{
	}

	virtual Point at(vector<Point>ct, double t);

	virtual double mypow(double r, int k);

	virtual Point dev(vector<Point>& ct, double t);

	virtual Point dev2(vector<Point>& ct, double t);

	virtual vector<Point> fit_curve(vector<Point>& pts, const double& maxE);

	virtual vector<Point> fit_cubic(vector<Point>& pts, int first, int last, Point left_tangent, Point right_tangent, const double& maxErr);

	virtual vector<Point> generateBezier(vector<Point>& pts, const int& first, const int last, vector<double> parameters, Point left_tag, Point right_tag);

	virtual vector<double> chordLengthParameterize(vector<Point>& pts, const int& first, const int& last);

	virtual vector<double> reparameterize(vector<Point>& ctrls, vector<Point>& pts, vector<double> parameters, const int& first, const int& last);

	virtual double newtonRaphsonRootFind(vector<Point>& ctrls, const Point& p, const double& u);

	virtual void computeMaxError(vector<Point>& pts, const int& first, const int& last, vector<Point>& ctrls, vector<double> params, double& maxDis, int& splitPoint);
	
};

