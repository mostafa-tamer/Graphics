//
// Created by mtame on 5/11/2023.
//

#ifndef GRAPHICSPROJECT_ALGORITHMS_H
#define GRAPHICSPROJECT_ALGORITHMS_H

#include <iostream>
#include "windows.h"
#include <cmath>

using namespace std;


class Algorithms {

public:

    static void PointClipping(HDC hdc, int x, int y, int xleft, int ytop, int xright, int ybottom, COLORREF color) {
        if (x >= xleft && x <= xright && y >= ytop && y <= ybottom)
            mySetPixel(hdc, x, y, color);
    }

    static void LineClipping(HDC hdc, int xs, int ys, int xe, int ye, int xleft, int ytop, int xright, int ybottom) {
        double x1 = xs, y1 = ys, x2 = xe, y2 = ye;
        OutCode out1 = GetOutCode(x1, y1, xleft, ytop, xright, ybottom);
        OutCode out2 = GetOutCode(x2, y2, xleft, ytop, xright, ybottom);
        while ((out1.All || out2.All) && !(out1.All & out2.All)) {
            double xi, yi;
            if (out1.All) {
                if (out1.left)VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
                else if (out1.top)HIntersect(x1, y1, x2, y2, ytop, &xi, &yi);
                else if (out1.right)VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
                else HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi);
                x1 = xi;
                y1 = yi;
                out1 = GetOutCode(x1, y1, xleft, ytop, xright, ybottom);
            } else {
                if (out2.left)VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
                else if (out2.top)HIntersect(x1, y1, x2, y2, ytop, &xi, &yi);
                else if (out2.right)VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
                else HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi);
                x2 = xi;
                y2 = yi;
                out2 = GetOutCode(x2, y2, xleft, ytop, xright, ybottom);
            }
        }
        if (!out1.All && !out2.All) {
            MoveToEx(hdc, round(x1), round(y1), NULL);
            LineTo(hdc, round(x2), round(y2));
        }
    }

    static void PolygonClip(HDC hdc, const std::vector<POINT> &points, int xleft, int ytop, int xright, int ybottom) {
        VertexList vlist;
        for (const auto &point: points) {
            vlist.emplace_back(point.x, point.y);
        }
        vlist = ClipWithEdge(vlist, xleft, InLeft, VIntersect);
        vlist = ClipWithEdge(vlist, ytop, InTop, HIntersect);
        vlist = ClipWithEdge(vlist, xright, InRight, VIntersect);
        vlist = ClipWithEdge(vlist, ybottom, InBottom, HIntersect);
        Vertex v1 = vlist[vlist.size() - 1];
        for (auto v2: vlist) {
            MoveToEx(hdc, round(v1.x), round(v1.y), NULL);
            LineTo(hdc, round(v2.x), round(v2.y));
            v1 = v2;
        }
    }

    static void ConvexFill(HDC hdc, POINT p[], int n, COLORREF color) {
        Entry *table = new Entry[MAXENTRIES];
        InitEntries(table);
        POINT v1 = p[n - 1];
        for (int i = 0; i < n; i++) {
            POINT v2 = p[i];
            ScanEdge(v1, v2, table);
            v1 = p[i];
        }
        DrawSanLines(hdc, table, color);
        delete table;
    }

    static void GeneralPolygonFill(HDC hdc, POINT *polygon, int n, COLORREF c) {
        EdgeList *table = new EdgeList[MAX_ENTRIES];
        InitEdgeTable(polygon, n, table);
        int y = 0;
        while (y < MAX_ENTRIES && table[y].size() == 0) y++;
        if (y == MAX_ENTRIES)return;
        EdgeList ActiveList = table[y];
        while (ActiveList.size() > 0) {
            ActiveList.sort();
            for (EdgeList::iterator it = ActiveList.begin(); it != ActiveList.end(); it++) {
                int x1 = (int) ceil(it->x);
                it++;
                int x2 = (int) floor(it->x);
                for (int x = x1; x <= x2; x++)mySetPixel(hdc, x, y, c);
            }
            y++;
            EdgeList::iterator it = ActiveList.begin();
            while (it != ActiveList.end())
                if (y == it->ymax) it = ActiveList.erase(it); else it++;
            for (EdgeList::iterator it = ActiveList.begin(); it != ActiveList.end(); it++)
                it->x += it->minv;
            ActiveList.insert(ActiveList.end(), table[y].begin(), table[y].end());
        }
        delete[] table;
    }

    static void FloodFill(HDC hdc, int x, int y, COLORREF Cb, COLORREF Cf) {
        COLORREF C = GetPixel(hdc, x, y);
        if (C == Cb || C == Cf) return;
        mySetPixel(hdc, x, y, Cf);
        FloodFill(hdc, x + 1, y, Cb, Cf);
        FloodFill(hdc, x - 1, y, Cb, Cf);
        FloodFill(hdc, x, y + 1, Cb, Cf);
        FloodFill(hdc, x, y - 1, Cb, Cf);
    }

    static void NRFloodFill(HDC hdc, int x, int y, COLORREF Cf, COLORREF Cb) {
        stack<pair<int, int>> S;
        S.emplace(x, y);
        while (!S.empty()) {
            cout << "Stack Size: " << S.size() << endl;
            pair<int, int> v = S.top();
            S.pop();
            COLORREF c = GetPixel(hdc, v.first, v.second);
            if (c == Cb || c == Cf) continue;
            mySetPixel(hdc, v.first, v.second, Cf);
            S.emplace(v.first + 1, v.second);
            S.emplace(v.first - 1, v.second);
            S.emplace(v.first, v.second + 1);
            S.emplace(v.first, v.second - 1);
        }
    }

    static void DrawSpline(HDC hdc, vector<pair<int, int>> &points, double tension, COLORREF color) {
        int size = points.size();
        vector<pair<int, int>> T(size);

        for (int i = 1; i < size - 2; ++i) {
            int x = tension * (points[i + 1].first - points[i - 1].first);
            int y = tension * (points[i + 1].second - points[i - 1].second);
            T[i] = (make_pair(x, y));
        }

        T[0] = make_pair(
                tension * (points[1].first - points[0].first),
                tension * (points[1].second - points[0].second)
        );

        T[size - 1] = make_pair(
                tension * (points[size - 1].first - points[size - 2].first),
                tension * (points[size - 1].second - points[size - 2].second)
        );

        for (int i = 0; i < size - 1; ++i) {
            pair current_point = points[i];
            pair current_T = T[i];
            pair next_point = points[i + 1];
            pair next_T = T[i + 1];
            DrawParametricCurveHermite(
                    hdc,
                    current_point.first, current_point.second,
                    current_T.first, current_T.second,
                    next_point.first, next_point.second,
                    next_T.first, next_T.second,
                    color
            );
        }
    }

    static void DrawParametricCurveHermite(
            HDC hdc,
            int x1, int y1,
            int u1, int v1,
            int x2, int y2,
            int u2, int v2,
            COLORREF color
    ) {
        int helperMatrix[4][4] = {
                {2,  1,  -2, 1},
                {-3, -2, 3,  -1},
                {0,  1,  0,  0},
                {1,  0,  0,  0}
        };

        int coordinatesMatrix[4][2] = {
                {x1, y1},
                {u1, v1},
                {x2, y2},
                {u2, v2}
        };

        int resultMatrix[4][2];

        matrixMultiplication(helperMatrix, coordinatesMatrix, resultMatrix);

        double t = 0;

        int x = x1;
        int y = y1;

        while (t <= 1) {
            x = round(
                    pow(t, 3) * resultMatrix[0][0] +
                    pow(t, 2) * resultMatrix[1][0] +
                    t * resultMatrix[2][0] +
                    resultMatrix[3][0]
            );

            y = round(
                    pow(t, 3) * resultMatrix[0][1] +
                    pow(t, 2) * resultMatrix[1][1] +
                    t * resultMatrix[2][1] +
                    resultMatrix[3][1]
            );

            mySetPixel(hdc, x, y, color);
            t += 0.001;
        }
    }

    static void DrawParametricCurveBezier(
            HDC hdc,
            int x1, int y1,
            int u1, int v1,
            int x2, int y2,
            int u2, int v2,
            COLORREF color
    ) {
        int helperMatrix[4][4] = {
                {-1, 3,  -3, 1},
                {3,  -6, 3,  0},
                {-3, 3,  0,  0},
                {1,  0,  0,  0}
        };

        int coordinatesMatrix[4][2] = {
                {x1, y1},
                {u1, v1},
                {x2, y2},
                {u2, v2}
        };

        int resultMatrix[4][2];

        matrixMultiplication(helperMatrix, coordinatesMatrix, resultMatrix);

        double t = 0;

        int x = x1;
        int y = y1;

        while (t <= 1) {
            x = round(
                    pow(t, 3) * resultMatrix[0][0] +
                    pow(t, 2) * resultMatrix[1][0] +
                    t * resultMatrix[2][0] +
                    resultMatrix[3][0]
            );

            y = round(
                    pow(t, 3) * resultMatrix[0][1] +
                    pow(t, 2) * resultMatrix[1][1] +
                    t * resultMatrix[2][1] +
                    resultMatrix[3][1]
            );

            mySetPixel(hdc, x, y, color);
            t += 0.001;
        }
    }

    static void DrawParametricLine(HDC hdc, int xs, int ys, int xe, int ye, COLORREF c) {
        double t = 0;
        int x = xs;
        int y = ys;
        mySetPixel(hdc, x, y, c);

        while (t <= 1) {
            x = round(xs + (xe - xs) * t);
            y = round(ys + (ye - ys) * t);
            mySetPixel(hdc, x, y, c);
            t += 0.001;
        }
    }

    static void LineMidpoint(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color) {
        int dx = abs(xe - xs);
        int dy = abs(ye - ys);
        int x = xs;
        int y = ys;
        int sx = (xs < xe) ? 1 : -1;
        int sy = (ys < ye) ? 1 : -1;
        int err = dx - dy;

        mySetPixel(hdc, x, y, color);

        while (x != xe || y != ye) {
            int e2 = 2 * err;

            if (e2 > -dy) {
                err -= dy;
                x += sx;
            }

            if (e2 < dx) {
                err += dx;
                y += sy;
            }

            mySetPixel(hdc, x, y, color);
        }
    }

    static void PolarEllipse(HDC hdc, int x1, int y1, int a, int b, COLORREF c) {
        double dd = 1.0 / max(a, b);
        double cc = cos(dd),
                ss = sin(dd);
        double x = a, y = 0;
        for (double i = 0; i < 1.58; i += dd) {
            draw4points(hdc, x1, y1, round(x), round(y), c);
            double x3 = x * cc - ((double) a / b) * y * ss;
            y = y * cc + ((double) b / a) * x * ss;
            x = x3;
        }
    }

    static void MidPointEllipse(HDC hdc, int xc, int yc, int a, int b, COLORREF color) {
        int x = 0, y = b;
        int d = b * b - a * a * b + a * a / 4;
        int dx = 2 * b * b * x, dy = 2 * a * a * y;

        while (dx < dy) {
            draw4points(hdc, xc, yc, x, y, color);

            x++;
            dx += 2 * b * b;
            if (d < 0) {
                d += dx + b * b;
            } else {
                y--;
                dy -= 2 * a * a;
                d += dx - dy + b * b;
            }
        }

        d = b * b * (x + 0.5) * (x + 0.5) + a * a * (y - 1) * (y - 1) - a * a * b * b;
        while (y >= 0) {
            draw4points(hdc, xc, yc, x, y, color);

            y--;
            dy -= 2 * a * a;
            if (d > 0) {
                d += a * a - dy;
            } else {
                x++;
                dx += 2 * b * b;
                d += a * a + dx - dy;
            }
        }
    }

    static void DirectEllipse(HDC hdc, int xc, int yc, int a, int b, COLORREF c) {
        double x, y, theta;
        double dd = 1.0 / max(a, b);
        x = a;
        y = 0;
        theta = 0;
        draw4points(hdc, xc, yc, round(x), round(y), c);
        while (theta < M_PI / 2) {
            x = a * cos(theta);
            y = b * sin(theta);
            theta += dd;
            draw4points(hdc, xc, yc, round(x), round(y), c);
        }
    }

    static void
    ModifiedDrawParametricLine(HDC hdc, int xs, int ys, int xe, int ye, COLORREF c, vector<pair<int, int>> &v) {
        double t = 0;
        int x = xs;
        int y = ys;
        mySetPixel(hdc, x, y, c);

        while (t <= 1) {
            x = round(xs + (xe - xs) * t);
            y = round(ys + (ye - ys) * t);
            v.emplace_back(x, y);
            t += 0.001;
        }
    }

    static vector<pair<int, int>> PolygonVector(HDC hdc, vector<pair<int, int>> v, COLORREF color) {
        vector<pair<int, int>> polygonPixels;

        for (int i = 0; i < v.size() - 1; ++i) {
            ModifiedDrawParametricLine(hdc, v[i].first, v[i].second, v[i + 1].first, v[i + 1].second, color,
                                       polygonPixels);
        }
        ModifiedDrawParametricLine(hdc, v[0].first, v[0].second, v[v.size() - 1].first, v[v.size() - 1].second, color,
                                   polygonPixels);

        return polygonPixels;
    }

    static void CircleClipping(HDC hdc, int xc, int yc, int R, vector<pair<int, int>> v, COLORREF color) {
        for (int i = 0; i < v.size(); ++i) {
            if ((pow(v[i].first - xc, 2) + pow(v[i].second - yc, 2)) <= pow(R, 2)) {
                mySetPixel(hdc, v[i].first, v[i].second, color);
            }
        }
    }

    static void CircleFasterMidpoint(HDC hdc, int xc, int yc, int R, COLORREF color, int quarter = 0) {
        int x = 0, y = R;
        int d = 1 - R;
        int c1 = 3, c2 = 5 - 2 * R;
        draw8Points(hdc, xc, yc, x, y, color, quarter);
        while (x < y) {
            if (d < 0) {
                d += c1;
                c2 += 2;
            } else {
                d += c2;
                c2 += 4;
                y--;
            }
            c1 += 2;
            x++;
            draw8Points(hdc, xc, yc, x, y, color, quarter);
        }
    }

    static void ModifiedCircleMidpoint(HDC hdc, int xc, int yc, int R, COLORREF color) {
        LineMidpoint(hdc, xc, yc - R, xc, yc + R, color);
        LineMidpoint(hdc, xc - R, yc, xc + R, yc, color);
        int x = 0, y = R;
        int d = 1 - R;
        draw8Points(hdc, xc, yc, x, y, color);
        while (x < y) {
            if (d < 0)
                d += 2 * x + 2;
            else {
                d += 2 * (x - y) + 5;
                y--;
            }
            x++;
            draw8Points(hdc, xc, yc, x, y, color);
        }
    }

    static void CircleMidpoint(HDC hdc, int xc, int yc, int R, COLORREF color) {
        int x = 0, y = R;
        int d = 1 - R;
        draw8Points(hdc, xc, yc, x, y, color);
        while (x < y) {
            if (d < 0)
                d += 2 * x + 2;
            else {
                d += 2 * (x - y) + 5;
                y--;
            }
            x++;
            draw8Points(hdc, xc, yc, x, y, color);
        }
    }

    static void CircleIterativePolar(HDC hdc, int xc, int yc, int R, COLORREF c, int quarter = 0) {
        double x = 0, y = R, dtheta = 0.1 / R;
        double CC = cos(dtheta);
        double s = sin(dtheta);
        draw8Points(hdc, xc, yc, x, y, c);
        while (x < y) {
            double newx = x * CC - y * s;
            y = x * s + y * CC;
            x = newx;
            draw8Points(hdc, xc, yc, x, y, c);
        }
    }

    static void CirclePolar(HDC hdc, int xc, int yc, int R, COLORREF color) {
        int x = R, y = 0;
        double theta = 0, dtheta = 1.0 / R;
        draw8Points(hdc, xc, yc, x, y, color);
        while (x > y) {
            theta += dtheta;
            x = round(R * cos(theta));
            y = round(R * sin(theta));
            draw8Points(hdc, xc, yc, x, y, color);
        }
    }

    static void CircleDirect(HDC hdc, int xc, int yc, int R, COLORREF color) {
        int x = 0, y = R;
        int R2 = R * R;
        draw8Points(hdc, xc, yc, x, y, color);
        while (x < y) {
            x++;
            y = round(sqrt((double) (R2 - x * x)));
            draw8Points(hdc, xc, yc, x, y, color);
        }
    }

    static void SimpleDDA(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color) {
        int dx = xe - xs;
        int dy = ye - ys;
        if (abs(dy) <= abs(dx)) {
            double slope = (double) dy / dx;
            if (xs > xe) {
                swap(xs, xe);
                swap(ys, ye);
            }
            for (int x = xs; x <= xe; x++) {
                int y = round(ys + (x - xs) * slope);
                mySetPixel(hdc, x, y, color);
            }
        } else {
            double slope = (double) dx / dy;
            if (ys > ye) {
                swap(xs, xe);
                swap(ys, ye);
            }
            for (int y = ys; y <= ye; y++) {
                int x = round(xs + (y - ys) * slope);
                mySetPixel(hdc, x, y, color);
            }
        }
    }
};


#endif //GRAPHICSPROJECT_ALGORITHMS_H
