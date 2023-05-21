//
// Created by mtame on 5/10/2023.
//

#ifndef GRAPHICSPROJECT_VIEW_H
#define GRAPHICSPROJECT_VIEW_H

#include "windows.h"
#include "Utils.h"

using namespace std;


class View {
public:

    virtual void draw(HDC &pHdc) = 0;

    virtual void setDimensions(int x, int y, int width, int height) = 0;

    void fillBackground(HDC const &hdc, int x, int y, int width, int height) const {
//        return;
        RECT rect;
        rect.left = x;
        rect.top = y;
        rect.right = x + width;
        rect.bottom = y + height;

        for (int i = x; i <= x + width; ++i) {
            for (int j = y; j <= y + height; ++j) {
                SetPixel(hdc, i, j, backGroundColor);
            }
        }
    }

    void drawBoundaries(const HDC &hdc, int x, int y, int width, int height) const {
        RECT rect;
        rect.left = x;
        rect.top = y;
        rect.right = x + width;
        rect.bottom = y + height;

        for (int i = 0; i < strokeThickness; ++i) {
            drawLine(hdc, rect.left - i, rect.top - i, rect.right + i, rect.top - i, strokeColor);
            drawLine(hdc, rect.left - i, rect.bottom + i, rect.right + i, rect.bottom + i, strokeColor);
            drawLine(hdc, rect.left - i, rect.top - i, rect.left - i, rect.bottom + i, strokeColor);
            drawLine(hdc, rect.right + i, rect.top - i, rect.right + i, rect.bottom + i, strokeColor);
        }
    }

    static void drawLine(const HDC &hdc, const int x1, const int y1, const int x2, const int y2, const COLORREF color) {
        int dx = x2 - x1;
        int dy = y2 - y1;
        int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
        float xIncrement = static_cast<float>(dx) / steps;
        float yIncrement = static_cast<float>(dy) / steps;

        auto x = static_cast<float>(x1);
        auto y = static_cast<float>(y1);

        for (int i = 0; i <= steps; ++i) {
            SetPixel(hdc, static_cast<int>(x), static_cast<int>(y), color);
            x += xIncrement;
            y += yIncrement;
        }
    }

    int getMargin() {
        return margin;
    }

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }

    void setBackgroundColor(COLORREF color) {
        this->backGroundColor = color;
    }

    COLORREF getBackGroundColor(){
        return backGroundColor;
    }

    void setStrokeColor(COLORREF color) {
        this->strokeColor = color;
    }

    void setStrokeThickness(int strokeThickness) {
        this->strokeThickness = strokeThickness;
    }

protected:
    COLORREF backGroundColor;
    COLORREF strokeColor;
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    int margin = 0;
    int strokeThickness = 4;
};


#endif //GRAPHICSPROJECT_VIEW_H
