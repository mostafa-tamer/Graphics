//
// Created by mtame on 5/12/2023.
//

#ifndef GRAPHICSPROJECT_DRAW_H
#define GRAPHICSPROJECT_DRAW_H


#include "Algorithms.h"
#include "Button.h"

class Draw {

public:
    static void fillShape(HWND &hwnd, int posX, int posY, COLORREF fColor, COLORREF bColor, Button *button) {
        if (!button->getIsActivated())
            return;

        HDC hdc = GetDC(hwnd);
        static int counter = -1;

        if (counter == 0) {
            button->setActive(false);
            cout << "Start Filling Shape!" << endl;
            Algorithms::NRFloodFill(hdc, posX, posY, fColor, bColor);
            counter = -1;
            cout << "Fill Shape Done!" << endl;
        } else {
            counter++;
        }
        ReleaseDC(hwnd, hdc);
    }

    static void drawCircle(HWND &hwnd, int posX, int posY, COLORREF color, Button *button) {
        if (!button->getIsActivated())
            return;

        HDC hdc = GetDC(hwnd);

        static int counter = -1;
        static int xs = 0, ys = 0;

        if (counter == 1) {
            button->setActive(false);
            cout << "Start Drawing Circle!" << endl;
            int R = distance(xs, ys, posX, posY);
            Algorithms::CircleIterativePolar(hdc, xs, ys, R, color, 0);
            counter = -1;
            xs = 0, ys = 0;
            cout << "Draw Circle Done!" << endl;
        } else {
            xs = posX;
            ys = posY;
            counter++;
        }
        ReleaseDC(hwnd, hdc);
    }

    static void drawLine(HWND &hwnd, int posX, int posY, COLORREF color, Button *button) {

        if (!button->getIsActivated())
            return;

        HDC hdc = GetDC(hwnd);

        static int counter = -1;
        static int xs = 0, ys = 0, xe = 0, ye = 0;

        if (counter == 1) {
            button->setActive(false);
            cout << "Start Drawing Line!" << endl;
            xe = posX;
            ye = posY;
            Algorithms::DrawParametricLine(hdc, xs, ys, xe, ye, color);
            counter = -1;
            xs = 0, ys = 0, xe = 0, ye = 0;
            cout << "Draw Line Done!" << endl;
        } else {
            xs = posX;
            ys = posY;
            counter++;
        }
        ReleaseDC(hwnd, hdc);
    }

    static void changeWidowColor(
            HWND hwnd,
            int topLeftX, int topLeftY, int topRightX, int topRightY,
            int bottomLeftX, int bottomLeftY, int bottomRightX, int bottomRightY,
            COLORREF color,
            Button *button
    ) {
        if (!button->getIsActivated())
            return;

        button->setActive(false);

        POINT vertices[] = {
                {topLeftX,     topLeftY},
                {topRightX,    topRightY},
                {bottomLeftX,  bottomLeftY},
                {bottomRightX, bottomRightY},
        };
        int size = sizeof(vertices) / sizeof(vertices[0]);

        HDC hdc = GetDC(hwnd);

        cout << "Start Filling Window!" << endl;
        Algorithms::ConvexFill(hdc, vertices, size, color);

        cout << "Fill Window Done!" << endl;

        ReleaseDC(hwnd, hdc);
    }

    static void drawRectangle(HWND &hwnd, int posX, int posY, COLORREF color, Button *button) {

        if (!button->getIsActivated())
            return;

        HDC hdc = GetDC(hwnd);

        static int counter = -1;
        static int xs = 0, ys = 0, xe = 0, ye = 0;

        if (counter == 1) {
            cout << "Start Drawing Rectangle!" << endl;
            xe = posX;
            ye = posY;
            Algorithms::DrawParametricLine(hdc, xs, ys, xe, ys, color);
            Algorithms::DrawParametricLine(hdc, xs, ys, xs, ye, color);
            Algorithms::DrawParametricLine(hdc, xe, ys, xe, ye, color);
            Algorithms::DrawParametricLine(hdc, xs, ye, xe, ye, color);

            if (ys > ye) swap(ys, ye);

            for (int i = 0; i < ye - ys; i += 1) {
                Algorithms::DrawParametricCurveHermite(
                        hdc,
                        xs, ys + i,
                        0, 0,
                        xe, ys + i,
                        0, 0,
                        color
                );
            }

            for (int i = -ye + ys; i < ye - ys; i += 1) {
                Algorithms::DrawParametricCurveHermite(
                        hdc,
                        xs, ys,
                        (xs + xe) / 2, ys + i,
                        xe, ye,
                        xe, (ye + ys) / 2 - i,
                        RGB(255, 0, 0)

                );
            }


            counter = -1;
            xs = 0, ys = 0, xe = 0, ye = 0;
            button->setActive(false);
            cout << "Draw Rectangle Done!" << endl;
        } else {
            xs = posX;
            ys = posY;
            counter++;
        }
        ReleaseDC(hwnd, hdc);
    }


    static void drawSquare(HWND &hwnd, int posX, int posY, COLORREF color, Button *button) {

        if (!button->getIsActivated())
            return;

        HDC hdc = GetDC(hwnd);

        static int counter = -1;
        static int xs = 0, ys = 0, xe = 0, ye = 0;

        if (counter == 1) {
            button->setActive(false);

            cout << "Start Drawing Square!" << endl;
            xe = posX;
            ye = posY;
            int D = distance(xs, ys, xe, ye);
            Algorithms::DrawParametricLine(hdc, xs, ys, xs + D, ys, color);
            Algorithms::DrawParametricLine(hdc, xs, ys, xs, ys + D, color);
            Algorithms::DrawParametricLine(hdc, xs + D, ys, xs + D, ys + D, color);
            Algorithms::DrawParametricLine(hdc, xs, ys + D, xs + D, ys + D, color);

            for (int i = 0; i < D; i += 1) {
                Algorithms::DrawParametricCurveBezier(
                        hdc,
                        xs + i, ys,
                        xs + i, ys + D / 2,
                        xs + i, ys + D / 2,
                        xs + i, ys + D,
                        color
                );
            }

            for (int i = D / 2; i > -D / 2; i -= 1) {
                Algorithms::DrawParametricCurveBezier(
                        hdc,
                        xs, ys,
                        xs, (ys + D / 2) + i,
                        xs + D, (ys + D / 2) + i,
                        xs + D, ys + D,
                        RGB(255, 0, 0)
                );
            }


            counter = -1;
            xs = 0, ys = 0, xe = 0, ye = 0;
            cout << "Draw Square Done!" << endl;
        } else {
            xs = posX;
            ys = posY;
            counter++;
        }
        ReleaseDC(hwnd, hdc);
    }

    static void Draw8Points(HDC hdc, int xc, int yc, int a, int b, int q, COLORREF color) {
        if (q == 1) {
            Algorithms::SimpleDDA(hdc, xc, yc, xc + a, yc - b, color);
            Algorithms::SimpleDDA(hdc, xc, yc, xc + b, yc - a, color);
        }

        if (q == 2) {
            Algorithms::SimpleDDA(hdc, xc, yc, xc - a, yc - b, color);
            Algorithms::SimpleDDA(hdc, xc, yc, xc - b, yc - a, color);
        }

        if (q == 3) {
            Algorithms::SimpleDDA(hdc, xc, yc, xc - a, yc + b, color);
            Algorithms::SimpleDDA(hdc, xc, yc, xc - b, yc + a, color);
        }

        if (q == 4) {
            Algorithms::SimpleDDA(hdc, xc, yc, xc + a, yc + b, color);
            Algorithms::SimpleDDA(hdc, xc, yc, xc + b, yc + a, color);
        }
    }

    static void handleLineQuarter(COLORREF color, HDC hdc, int xs, int ys, int R, int quarter) {
        switch (quarter) {
            case 1: {
                fillCircleWithLines(hdc, xs, ys, R, 1, color);
                break;
            }
            case 2: {
                fillCircleWithLines(hdc, xs, ys, R, 2, color);

                break;
            }
            case 3: {
                fillCircleWithLines(hdc, xs, ys, R, 3, color);

                break;
            }
            case 4: {
                fillCircleWithLines(hdc, xs, ys, R, 4, color);

                break;
            }
            default: {
                cout << "Please enter a suitable quarter [1: 4]!" << endl;
            }
        }
    }

    static void fillCircleWithLines(HDC hdc, int xc, int yc, int R, int q, COLORREF color) {
        int x = 0, y = R, d = 1 - R, c1 = 3, c2 = 5 - 2 * R;
        while (x <= y) {
            Draw8Points(hdc, xc, yc, x, y, q, color);
            if (d < 0) d += c1, c2 += 2;
            else d += c2, c2 += 4, y--;
            c1 += 2, x++;
        }
    }


    static void handleCircleQuarter(COLORREF color, HDC hdc, int xs, int ys, int R, int quarter) {
        switch (quarter) {
            case 1: {
                for (int i = R; i > 0; i -= 1) {
                    Algorithms::CircleFasterMidpoint(hdc, xs, ys, R - i, color, 1);
                }
                break;
            }
            case 2: {
                for (int i = R; i > 0; i -= 1) {
                    Algorithms::CircleFasterMidpoint(hdc, xs, ys, R - i, color, 2);
                }
                break;
            }
            case 3: {
                for (int i = R; i > 0; i -= 1) {
                    Algorithms::CircleFasterMidpoint(hdc, xs, ys, R - i, color, 3);
                }
                break;
            }
            case 4: {
                for (int i = R; i > 0; i -= 1) {
                    Algorithms::CircleFasterMidpoint(hdc, xs, ys, R - i, color, 4);
                }
                break;
            }
            default: {
                cout << "Please enter a suitable quarter [1: 4]!" << endl;
            }
        }
    }

    static void drawDecoratedCircle(HWND hwnd, int posX, int posY, COLORREF color, Button *button) {
        if (!button->getIsActivated())
            return;

        HDC hdc = GetDC(hwnd);

        static int counter = -1;
        static int xs = 0, ys = 0;

        if (counter == 1) {
            button->setActive(false);

            cout << "Start Drawing Circle!" << endl;
            int R = distance(xs, ys, posX, posY);
            Algorithms::ModifiedCircleMidpoint(hdc, xs, ys, R, color);

            int quarter;

            cout << "Enter quarter to decorate with line" << endl;
            cin >> quarter;

            handleLineQuarter(color, hdc, xs, ys, R, quarter);

            cout << "Enter quarter to decorate with circle" << endl;
            cin >> quarter;

            handleCircleQuarter(color, hdc, xs, ys, R, quarter);

            counter = -1;
            xs = 0, ys = 0;
            cout << "Draw Circle Done!" << endl;
        } else {
            xs = posX;
            ys = posY;
            counter++;
        }
        ReleaseDC(hwnd, hdc);
    }

    static void drawSplines(HWND hwnd, int x, int y, COLORREF color, Button *button) {
        if (!button->getIsActivated())
            return;

        HDC hdc = GetDC(hwnd);

        static int numberOfPoints = 0;

        static int counter = -1;
        static int counterForPoints = 0;

        if (counter == 0) {
            static vector<pair<int, int>> points;
            cout << x << " " << y << endl;
            points.emplace_back(x, y);
            if (counterForPoints == numberOfPoints - 1) {
                button->setActive(false);
                counter = -1;
                counterForPoints = 0;
                Algorithms::DrawSpline(hdc, points, 0.5, color);
                points.clear();
                cout << "Spline is drawn!" << endl;
            } else {
                counterForPoints++;
            }
        } else {
            cout << "Enter number of points: " << endl;
            cin >> numberOfPoints;
            if (numberOfPoints > 2) {
                cout << "Pick points on the screen" << endl;
                counter++;
            } else {
                cout << "Enter points more than 2" << endl;
            }
        }

        ReleaseDC(hwnd, hdc);
    }

    static void drawEllipse(HWND hwnd, int posX, int posY, COLORREF color, Button *button) {
        if (!button->getIsActivated())
            return;

        HDC hdc = GetDC(hwnd);

        static int counter = -1;
        static int xs = 0, ys = 0;
        static int R1 = 0;

        if (counter == 2) {
            button->setActive(false);
            cout << "Start Drawing Circle!" << endl;
            int R2 = distance(xs, ys, posX, posY);
            cout << "R2 is: " << R2 << endl;
            Algorithms::PolarEllipse(hdc, xs, ys, R1, R2, color);
            counter = -1;
            xs = 0, ys = 0;
            cout << "Draw Circle Done!" << endl;
        } else if (counter == 1) {
            R1 = distance(xs, ys, posX, posY);
            cout << "R1 is: " << R1 << endl;
            counter++;
        } else if (counter == 0) {
            xs = posX;
            ys = posY;
            cout << "Center is: " << xs << " " << ys << endl;
            counter++;
        } else {
            counter++;
        }
        ReleaseDC(hwnd, hdc);
    }

    static void circleClipping(HWND hwnd, COLORREF color, Button *button) {
        if (!button->getIsActivated())
            return;
        button->setActive(false);

//      500 400 250 3 300 200 400 750 750 700

        cout << "Enter circle dimensions" << endl;

        int xc, yc, R;
        cout << "Enter x center" << endl;
        cin >> xc;
        cout << "Enter y center" << endl;
        cin >> yc;
        cout << "Enter R center" << endl;
        cin >> R;

        cout << "Enter number of polygon points" << endl;
        int polygonPointsNumber;
        cin >> polygonPointsNumber;

        vector<pair<int, int>> polygonPoints;
        cout << "Enter points' coordinates" << endl;
        for (int i = 0; i < polygonPointsNumber; ++i) {
            int x, y;
            cout << "Enter x" << i + 1 << ": ";
            cin >> x;
            cout << "Enter y" << i + 1 << ": ";
            cin >> y;
            polygonPoints.emplace_back(x, y);
        }
        HDC hdc = GetDC(hwnd);
        Algorithms::CircleMidpoint(hdc, xc, yc, R, color);
        Algorithms::CircleClipping(hdc, xc, yc, R, Algorithms::PolygonVector(hdc, polygonPoints, color), color);


        ReleaseDC(hwnd, hdc);

    }

    static void rectangleClipping(HWND hwnd, int posX, int posY, COLORREF color, Button *button) {
        if (!button->getIsActivated())
            return;

        HDC hdc = GetDC(hwnd);

        static int counter = 0;
        static int xs = 0, ys = 0, xe = 0, ye = 0;
        static int selection = -1;

        static POINT point;
        static pair<POINT, POINT> lineStartEnd;
        static vector<POINT> points;

        if (counter == 2) {
            button->setActive(false);

            cout << "Start Drawing Rectangle!" << endl;
            xe = posX;
            ye = posY;

            int left = xs, top = ys, right = xe, bottom = ye;

            Algorithms::DrawParametricLine(hdc, xs, ys, xe, ys, color);
            Algorithms::DrawParametricLine(hdc, xs, ys, xs, ye, color);
            Algorithms::DrawParametricLine(hdc, xe, ys, xe, ye, color);
            Algorithms::DrawParametricLine(hdc, xs, ye, xe, ye, color);

            switch (selection) {
                case 1: {
                    Algorithms::PointClipping(
                            hdc,
                            point.x, point.y,
                            left, top, right, bottom,
                            color
                    );
                    break;
                }
                case 2: {
                    Algorithms::LineClipping(
                            hdc,
                            lineStartEnd.first.x, lineStartEnd.first.y,
                            lineStartEnd.second.x, lineStartEnd.second.y,
                            left, top, right, bottom
                    );
                    break;
                }
                case 3: {
                    Algorithms::PolygonClip(hdc, points, left, top, right, bottom);
                    break;
                }
            }

            counter = -1;
            selection = -1;
            xs = 0, ys = 0, xe = 0, ye = 0;
            cout << "Draw Rectangle Done!" << endl;
        } else if (counter == 1) {
            xs = posX;
            ys = posY;
            counter++;
        } else {
            cout << "Enter type of your drawing 1 => point, 2 => line, 3 => polygon" << endl;
            cin >> selection;
            if (selection == 1 || selection == 2 || selection == 3) {
                if (selection == 1) {
                    cout << "Enter Line Coordinates " << endl;
                    cout << "Enter start x: ";
                    cin >> point.x;
                    cout << "Enter start y: ";
                    cin >> point.y;
                    Algorithms::CircleIterativePolar(hdc, point.x, point.y, 10, color);
                } else if (selection == 2) {
                    cout << "Enter Line Coordinates " << endl;
                    cout << "Enter start x: ";
                    cin >> lineStartEnd.first.x;
                    cout << "Enter start y: ";
                    cin >> lineStartEnd.first.y;

                    cout << "Enter end x: ";
                    cin >> lineStartEnd.second.x;
                    cout << "Enter end y: ";
                    cin >> lineStartEnd.second.y;

                    Algorithms::CircleIterativePolar(hdc, lineStartEnd.first.x, lineStartEnd.first.y, 10, color);
                    Algorithms::CircleIterativePolar(hdc, lineStartEnd.second.x, lineStartEnd.second.y, 10, color);
                } else if (selection == 3) {
                    cout << "Enter number of polygon points: ";
                    int numberOfPoints;
                    cin >> numberOfPoints;

                    for (int i = 0; i < numberOfPoints; ++i) {
                        POINT point;
                        cout << "Enter x: ";
                        cin >> point.x;
                        cout << "Enter y: ";
                        cin >> point.y;

                        Algorithms::CircleIterativePolar(hdc, point.x, point.y, 10, color);

                        points.push_back(point);
                    }
                }
                counter++;
            } else {
                cout << "Please select a correct choice!" << endl;
                counter = -1;
                selection = -1;
                xs = 0, ys = 0, xe = 0, ye = 0;
            }
        }
        ReleaseDC(hwnd, hdc);
    }

    static void squareClipping(HWND hwnd, int posX, int posY, COLORREF color, Button *button) {
        if (!button->getIsActivated())
            return;

        HDC hdc = GetDC(hwnd);

        static int counter = 0;
        static int xs = 0, ys = 0, xe = 0, ye = 0;
        static int selection = -1;

        static pair<POINT, POINT> lineStartEnd;
        static vector<POINT> points;

        if (counter == 2) {
            button->setActive(false);

            cout << "Start Drawing Square!" << endl;
            xe = posX;
            ye = posY;

            int D = distance(xs, ys, xe, ye);

            int left = xs, top = ys, right = xs + D, bottom = ys + D;

            Algorithms::DrawParametricLine(hdc, xs, ys, xs + D, ys, color);
            Algorithms::DrawParametricLine(hdc, xs, ys, xs, ys + D, color);
            Algorithms::DrawParametricLine(hdc, xs + D, ys, xs + D, ys + D, color);
            Algorithms::DrawParametricLine(hdc, xs, ys + D, xs + D, ys + D, color);

            switch (selection) {
                case 1: {
                    Algorithms::LineClipping(
                            hdc,
                            lineStartEnd.first.x, lineStartEnd.first.y,
                            lineStartEnd.second.x, lineStartEnd.second.y,
                            left, top, right, bottom
                    );
                    break;
                }
                case 2: {
                    Algorithms::PolygonClip(hdc, points, left, top, right, bottom);
                    break;
                }
            }

            counter = -1;
            selection = -1;
            xs = 0, ys = 0, xe = 0, ye = 0;
            cout << "Draw Square Done!" << endl;
        } else if (counter == 1) {
            xs = posX;
            ys = posY;
            counter++;
        } else {
            cout << "Enter type of your drawing 1 => line, 2 => polygon" << endl;
            cin >> selection;
            if (selection == 1 || selection == 2) {
                if (selection == 1) {
                    cout << "Enter Line Coordinates " << endl;
                    cout << "Enter start x: ";
                    cin >> lineStartEnd.first.x;
                    cout << "Enter start y: ";
                    cin >> lineStartEnd.first.y;

                    cout << "Enter end x: ";
                    cin >> lineStartEnd.second.x;
                    cout << "Enter end y: ";
                    cin >> lineStartEnd.second.y;

                    Algorithms::CircleIterativePolar(hdc, lineStartEnd.first.x, lineStartEnd.first.y, 10, color);
                    Algorithms::CircleIterativePolar(hdc, lineStartEnd.second.x, lineStartEnd.second.y, 10, color);
                } else if (selection == 2) {
                    cout << "Enter number of polygon points: ";
                    int numberOfPoints;
                    cin >> numberOfPoints;

                    for (int i = 0; i < numberOfPoints; ++i) {
                        POINT point;
                        cout << "Enter x: ";
                        cin >> point.x;
                        cout << "Enter y: ";
                        cin >> point.y;

                        Algorithms::CircleIterativePolar(hdc, point.x, point.y, 10, color);

                        points.push_back(point);
                    }
                }
                counter++;
            } else {
                cout << "Please select a correct choice!" << endl;
                counter = -1;
                selection = -1;
                xs = 0, ys = 0, xe = 0, ye = 0;
            }
        }
        ReleaseDC(hwnd, hdc);
    }
};


#endif //GRAPHICSPROJECT_DRAW_H
