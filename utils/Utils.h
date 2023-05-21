//
// Created by mtame on 5/12/2023.
//

#include "bits/stdc++.h"
#include "windows.h"

using namespace std;


vector<pair<COLORREF, pair<int, int>>> pixels(10000);

void mySetPixel(HDC hdc, int x, int y, COLORREF color) {
    pixels.emplace_back(color, pair(x, y));
    SetPixel(hdc, x, y, color);
}

//////////////////////////////////////////////////////////////////
//  Non-convex filling utils                                    //
//////////////////////////////////////////////////////////////////
#define MAXENTRIES 1920

struct Entry {
    int xmin, xmax;
};

void InitEntries(Entry table[]) {
    for (int i = 0; i < MAXENTRIES; i++) {
        table[i].xmin = MAXINT;
        table[i].xmax = -MAXINT;
    }
}

void ScanEdge(POINT v1, POINT v2, Entry table[]) {
    if (v1.y == v2.y)return;
    if (v1.y > v2.y)swap(v1, v2);
    double minv = (double) (v2.x - v1.x) / (v2.y - v1.y);
    double x = v1.x;
    int y = v1.y;
    while (y < v2.y) {
        if (x < table[y].xmin)table[y].xmin = (int) ceil(x);
        if (x > table[y].xmax)table[y].xmax = (int) floor(x);
        y++;
        x += minv;
    }
}

void DrawSanLines(HDC hdc, Entry table[], COLORREF color) {
    for (int y = 0; y < MAXENTRIES; y++)
        if (table[y].xmin < table[y].xmax)
            for (int x = table[y].xmin; x <= table[y].xmax; x++)
                mySetPixel(hdc, x, y, color);
}

//////////////////////////////////////////////////////////////////
//  Non-convex filling utils                                    //
//////////////////////////////////////////////////////////////////
#define MAX_ENTRIES 1920

struct EdgeRec {
    double x;
    double minv;
    int ymax;

    bool operator<(EdgeRec r) {
        return x < r.x;
    }
};

typedef list<EdgeRec> EdgeList;

EdgeRec InitEdgeRec(POINT &v1, POINT &v2) {
    if (v1.y > v2.y) swap(v1, v2);
    EdgeRec rec;
    rec.x = v1.x;
    rec.ymax = v2.y;
    rec.minv = (double) (v2.x - v1.x) / (v2.y - v1.y);
    return rec;
}

void InitEdgeTable(POINT *polygon, int n, EdgeList table[]) {
    POINT v1 = polygon[n - 1];
    for (int i = 0; i < n; i++) {
        POINT v2 = polygon[i];
        if (v1.y == v2.y) {
            v1 = v2;
            continue;
        }
        EdgeRec rec = InitEdgeRec(v1, v2);
        table[v1.y].push_back(rec);
        v1 = polygon[i];
    }
}

//////////////////////////////////////////////////////////////////
//  circle ellipse  drawing                                     //
//////////////////////////////////////////////////////////////////
double distance(int x1, int y1, int x2, int y2) {
    double dx = x1 - x2;
    double dy = y1 - y2;
    return sqrt(dx * dx + dy * dy);
}

void draw8Points(HDC hdc, int xc, int yc, int a, int b, COLORREF color, int quarter = 0) {
    if (quarter == 1 || quarter == 0) {
        mySetPixel(hdc, xc + a, yc - b, color);
        mySetPixel(hdc, xc + b, yc - a, color);
    }
    if (quarter == 2 || quarter == 0) {
        mySetPixel(hdc, xc - a, yc - b, color);
        mySetPixel(hdc, xc - b, yc - a, color);
    }
    if (quarter == 3 || quarter == 0) {
        mySetPixel(hdc, xc - a, yc + b, color);
        mySetPixel(hdc, xc - b, yc + a, color);
    }
    if (quarter == 4 || quarter == 0) {
        mySetPixel(hdc, xc + a, yc + b, color);
        mySetPixel(hdc, xc + b, yc + a, color);
    }
}

void draw4points(HDC hdc, int xc, int yc, int x, int y, COLORREF color) {
    mySetPixel(hdc, xc + x, yc + y, color);
    mySetPixel(hdc, xc - x, yc + y, color);
    mySetPixel(hdc, xc + x, yc - y, color);
    mySetPixel(hdc, xc - x, yc - y, color);
}

//////////////////////////////////////////////////////////////////
//  hermite and bezier                                          //
//////////////////////////////////////////////////////////////////
void matrixMultiplication(int mat1[][4], int mat2[][2], int result[][2]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            int sum = 0;
            for (int k = 0; k < 4; k++) {
                sum += mat1[i][k] * mat2[k][j];
            }
            result[i][j] = sum;
        }
    }
}

//////////////////////////////////////////////////////////////////
//  polygon clipping                                            //
//////////////////////////////////////////////////////////////////
struct Vertex {
    double x, y;

    Vertex(int x1 = 0, int y1 = 0) {
        x = x1;
        y = y1;
    }
};

typedef vector<Vertex> VertexList;

typedef bool (*IsInFunc)(Vertex &v, int edge);

typedef Vertex (*IntersectFunc)(Vertex &v1, Vertex &v2, int edge);

VertexList ClipWithEdge(VertexList p, int edge, IsInFunc In, IntersectFunc Intersect) {
    VertexList OutList;
    Vertex v1 = p[p.size() - 1];
    bool v1_in = In(v1, edge);
    for (int i = 0; i < (int) p.size(); i++) {
        Vertex v2 = p[i];
        bool v2_in = In(v2, edge);
        if (!v1_in && v2_in) {
            OutList.push_back(Intersect(v1, v2, edge));
            OutList.push_back(v2);
        } else if (v1_in && v2_in) OutList.push_back(v2);
        else if (v1_in) OutList.push_back(Intersect(v1, v2, edge));
        v1 = v2;
        v1_in = v2_in;
    }
    return OutList;
}

bool InLeft(Vertex &v, int edge) {
    return v.x >= edge;
}

bool InRight(Vertex &v, int edge) {
    return v.x <= edge;
}

bool InTop(Vertex &v, int edge) {
    return v.y >= edge;
}

bool InBottom(Vertex &v, int edge) {
    return v.y <= edge;
}

Vertex VIntersect(Vertex &v1, Vertex &v2, int xedge) {
    Vertex res;
    res.x = xedge;
    res.y = v1.y + (xedge - v1.x) * (v2.y - v1.y) / (v2.x - v1.x);
    return res;
}

Vertex HIntersect(Vertex &v1, Vertex &v2, int yedge) {
    Vertex res;
    res.y = yedge;
    res.x = v1.x + (yedge - v1.y) * (v2.x - v1.x) / (v2.y - v1.y);
    return res;
}

//////////////////////////////////////////////////////////////////
//  line clipping                                               //
//////////////////////////////////////////////////////////////////
union OutCode {
    unsigned All: 4;
    struct {
        unsigned left: 1, top: 1, right: 1, bottom: 1;
    };
};

OutCode GetOutCode(double x, double y, int xleft, int ytop, int xright, int ybottom) {
    OutCode out;
    out.All = 0;
    if (x < xleft)out.left = 1; else if (x > xright)out.right = 1;
    if (y < ytop)out.top = 1; else if (y > ybottom)out.bottom = 1;
    return out;
}

void VIntersect(double xs, double ys, double xe, double ye, int x, double *xi, double *yi) {
    *xi = x;
    *yi = ys + (x - xs) * (ye - ys) / (xe - xs);
}

void HIntersect(double xs, double ys, double xe, double ye, int y, double *xi, double *yi) {
    *yi = y;
    *xi = xs + (y - ys) * (xe - xs) / (ye - ys);
}

//////////////////////////////////////////////////////////////////
//  read and write                                              //
//////////////////////////////////////////////////////////////////
void writeToFile(const std::vector<pair<COLORREF, pair<int, int>>> &colors, const std::string &filename) {

    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    if (file.is_open()) {
        for (const auto &color: colors) {
            int red = GetRValue(color.first);
            int green = GetGValue(color.first);
            int blue = GetBValue(color.first);
            file << red << "," << green << "," << blue << "," << color.second.first << "," << color.second.second
                 << "\n";
        }
        file.close();
        std::cout << "Colors successfully written to " << filename << std::endl;
    } else {
        std::cerr << "Error opening file " << filename << std::endl;
    }
    cout << "Done!" << endl;
}

struct ColorData {
    int red;
    int green;
    int blue;
    int x;
    int y;
};

std::vector<ColorData> readColorsFromFile(const std::string &filename) {
    std::vector<ColorData> colorDataList;
    std::ifstream inputFile(filename);
    std::string line;

    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string token;
        ColorData colorData;

        // Read the comma-separated values
        std::getline(ss, token, ',');
        colorData.red = std::stoi(token);
        std::getline(ss, token, ',');
        colorData.green = std::stoi(token);
        std::getline(ss, token, ',');
        colorData.blue = std::stoi(token);
        std::getline(ss, token, ',');
        colorData.x = std::stoi(token);
        std::getline(ss, token, ',');
        colorData.y = std::stoi(token);

        // Add the color data to the list
        colorDataList.push_back(colorData);
    }
    for (auto i: colorDataList) {
        pixels.emplace_back(RGB(i.red, i.green, i.blue), pair(i.x, i.y));
    }
    inputFile.close();
    return colorDataList;
}
//////////////////////////////////////////////////////////////////
//  translation and scaling                                     //
//////////////////////////////////////////////////////////////////

class Vector2 {
    double v[3];
public:
    Vector2() {
        v[0] = v[1];
        v[2] = 1;
    }

    Vector2(double x, double y) {
        v[0] = x;
        v[1] = y;
        v[2] = 1;
    }

    double &operator[](int n) { return v[n]; }
};

class Matrix2 {
    Vector2 A[3];
public:
    Vector2 &operator[](int n) { return A[n]; }

    friend Matrix2 operator*(Matrix2 &A, Matrix2 &B) {
        Matrix2 C;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) {
                C[i][j] = 0;
                for (int k = 0; k < 3; k++)C[i][j] += A[i][k] * B[k][j];
            }
        return C;
    }

    friend Vector2 operator*(Matrix2 &A, Vector2 &v) {
        Vector2 r;
        for (int i = 0; i < 3; i++) {
            r[i] = 0;
            for (int j = 0; j < 3; j++)r[i] += A[i][j] * v[j];
        }
        return r;
    }

    Matrix2 &operator*=(Matrix2 &B) {
        *this = *this * B;
        return *this;
    }
};

Matrix2 Identity() {
    Matrix2 A;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            A[i][j] = i == j;
    return A;
}

Matrix2 translate(double dx, double dy) {
    Matrix2 T = Identity();
    T[0][2] = dx;
    T[1][2] = dy;
    return T;
}

Matrix2 scale(double alpha, double beta) {
    Matrix2 Sc = Identity();
    Sc[0][0] = alpha;
    Sc[1][1] = beta;
    return Sc;
}
//////////////////////////////////////////////////////////////////

