// CBezier.cpp: 实现文件
//

#include "pch.h"
#include "GISLine.h"
#include "CBezier.h"


// CBezier

IMPLEMENT_DYNCREATE(CBezier, CView)

CBezier::CBezier()
{

}

CBezier::~CBezier()
{
}

BEGIN_MESSAGE_MAP(CBezier, CView)
END_MESSAGE_MAP()


// CBezier 绘图

void CBezier::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码
}


// CBezier 诊断

#ifdef _DEBUG
void CBezier::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CBezier::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBezier 消息处理程序
double CBezier::mypow(double r, int k) {
	if (k < 0)return 0;
	return pow(r, k);
}
Point CBezier::dev(vector<Point>& ct, double t) {
	Point ans = (ct[1] - ct[0]) * (3 * mypow(1 - t, 2)) + (ct[2] - ct[1]) * (6 * t * (1 - t)) + (ct[3] - ct[2]) * (3 * t * t);
	return ans;
}
Point CBezier::dev2(vector<Point>& ct, double t) {
	Point ans;
	ans = (ct[0] + ct[2] - ct[1] * 2) * (6 * (1 - t)) + (ct[1] + ct[3] - ct[2] * 2) * (6 * t);
	return ans;
}
Point CBezier::at(vector<Point>ct, double t) {
	Point ans = ct[0] * mypow(1 - t, 3) + (ct[1] * (1 - t) + ct[2] * t) * (1 - t) * t + ct[3] * mypow(t, 3);
	return ans;
}


void CBezier::computeMaxError(vector<Point>& pts, const int& first, const int& last, vector<Point>& ctrls, vector<double> params, double& maxDis, int& splitPoint)
{
	double md = 0.0;
	int sp = (first + last) / 2;
	int sz = last - first + 1;
	for (int i = 0; i < sz; ++i) {
		double dist = (at(ctrls, params[i]) - pts[i + first]).norm();
		if (dist * dist > md) {
			md = dist * dist;
			sp = i;
		}
	}
	maxDis = md;
	splitPoint = sp;
}
vector<Point> CBezier::generateBezier(vector<Point>& pts, const int& first, const int last, vector<double> parameters, Point left_tag, Point right_tag)
{
	vector<Point> ans(4);
	ans[0] = pts[first]; ans[3] = pts[last];

	//if (pts.size() != parameters.size())cerr << "This is wrong between the parameter size and points size in function generateBezier!\n";
	int sz = last - first + 1;


	vector<vector<vector<double>>> A(sz, vector<vector<double>>(2, vector<double>(2, 0.0)));
	for (int i = 0; i < sz; ++i) {
		double u = parameters[i];
		double u_ = 1 - u;
		Point temp = left_tag * (3 * u * u_ * u_);
		A[i][0][0] = temp.x;
		A[i][0][1] = temp.y;
		temp = right_tag * (3 * u * u * u_);
		A[i][1][0] = temp.x;
		A[i][1][1] = temp.y;
	}

	vector<vector<double>> C(2, vector<double>(2, 0));
	vector<double> X(2, 0.0);

	for (int i = 0; i < sz; ++i) {
		C[0][0] += A[i][0][0] * A[i][0][0] + A[i][0][1] * A[i][0][1];
		C[0][1] += A[i][0][0] * A[i][1][0] + A[i][0][1] * A[i][1][1];
		C[1][0] += A[i][0][0] * A[i][1][0] + A[i][0][1] * A[i][1][1];
		C[1][1] += A[i][1][0] * A[i][1][0] + A[i][1][1] * A[i][1][1];

		Point temp = pts[i + first] - at({ pts[first],pts[first],pts[last],pts[last] }, parameters[i]);

		X[0] += A[i][0][0] * temp.x + A[i][0][1] * temp.y;
		X[1] += A[i][1][0] * temp.x + A[i][1][1] * temp.y;
	}

	double det_C0_C1 = C[0][0] * C[1][1] - C[1][0] * C[0][1];
	double det_C0_X = C[0][0] * X[1] - C[1][0] * X[0];
	double det_X_C1 = X[0] * C[1][1] - X[1] * C[0][1];

	double alpha_l = det_C0_C1 == 0 ? 0.0 : det_X_C1 / det_C0_C1;
	double alpha_r = det_C0_C1 == 0 ? 0.0 : det_C0_X / det_C0_C1;

	double segLength = (pts[first] - pts[last]).norm();
	double eps = segLength * 1e-6;
	if (alpha_l < eps || alpha_r < eps) {
		ans[1] = ans[0] + left_tag * (segLength / 3.0);
		ans[2] = ans[3] + right_tag * (segLength / 3.0);
	}
	else {
		ans[1] = ans[0] + left_tag * alpha_l;
		ans[2] = ans[3] + right_tag * alpha_r;
	}
	return ans;
}

vector<double> CBezier::chordLengthParameterize(vector<Point>& pts, const int& first, const int& last) {
	vector<double> us = { 0 };
	for (int i = first + 1; i <= last; ++i) {
		us.push_back(us.back() + (pts[i] - pts[i - 1]).norm());
	}
	for (int i = 0; i < us.size(); ++i) {
		us[i] /= us.back();
	}
	return us;
}

double CBezier::newtonRaphsonRootFind(vector<Point>& ctrls, const Point& p, const double& u)
{
	Point d = at(ctrls, u) - p;
	Point dev1 = dev(ctrls, u);
	Point dev22 = dev2(ctrls, u);
	double numerator = d.x * dev1.x + d.y * dev1.y;
	double denominator = dev1.x * dev1.x + dev1.y * dev1.y + d.x * dev22.x + d.y * dev22.y;
	if (numerator == 0.0)return u;
	return u - numerator / denominator;
}

vector<double> CBezier::reparameterize(vector<Point>& ctrls, vector<Point>& pts, vector<double> parameters, const int& first, const int& last) {
	vector<double> ans;
	//if (pts.size() != parameters.size())cerr << "The size of points and parameters is not Equall in reparameter!";
	int sz = last - first + 1;
	for (int i = 0; i < sz; ++i) {
		ans.push_back(newtonRaphsonRootFind(ctrls, pts[i + first], parameters[i]));
	}
	return ans;
}





vector<Point> CBezier::fit_cubic(vector<Point>& pts, int first, int last, Point left_tangent, Point right_tangent, const double& maxErr) {
	vector<Point> ans;
	int npts = last - first + 1;
	if (npts == 2) {
		double dis = (pts[last] - pts[first]).norm() / 3;
		ans.push_back(pts[first]);
		ans.push_back(pts[first] + left_tangent * dis);
		ans.push_back(pts[last] + right_tangent * dis);
		ans.push_back(pts[last]);
		return ans;
	}
	vector<double> u = chordLengthParameterize(pts, first, last);
	vector<Point> bezeCurv = generateBezier(pts, first, last, u, left_tangent, right_tangent);
	double maxError;
	int splitPoint;
	computeMaxError(pts, first, last, bezeCurv, u, maxError, splitPoint);
	printf("max error: %f", maxError);
	if (maxError < maxErr)return bezeCurv;
	if (maxErr > 1 && maxError < maxErr * maxErr) {
		for (int j = 0; j < 20; ++j) {
			vector<double> up = reparameterize(bezeCurv, pts, u, first, last);
			bezeCurv = generateBezier(pts, first, last, up, left_tangent, right_tangent);
			computeMaxError(pts, first, last, bezeCurv, u, maxError, splitPoint);
			if (maxError < maxErr)
				return bezeCurv;
			u = up;
		}
	}
	vector<Point> beziers;
	Point t = pts[splitPoint - 1] - pts[splitPoint + 1];
	Point centerTangent = t * (1 / t.norm());
	vector<Point> ptsF(pts.begin(), pts.begin() + splitPoint);
	vector<Point> ptsB(pts.begin() + splitPoint, pts.end());

	beziers = fit_cubic(ptsF, first, splitPoint, left_tangent, centerTangent, maxErr);
	vector<Point> k = fit_cubic(ptsB, splitPoint, last, centerTangent, right_tangent, maxErr);
	beziers.insert(beziers.end(), k.begin(), k.end());
	return beziers;
}


vector<Point> CBezier::fit_curve(vector<Point>& pts, const double& maxE) {
	int sz = pts.size();
	vector<Point> ans;

	Point left_tan = pts[1] - pts[0];
	left_tan = left_tan * (1 / left_tan.norm());
	Point right_tan = pts[sz - 2] - pts[sz - 1];
	right_tan = right_tan * (1 / right_tan.norm());
	return fit_cubic(pts, 0, sz - 1, left_tan, right_tan, maxE);
}