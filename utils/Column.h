//
// Created by mtame on 5/10/2023.
//


#ifndef GRAPHICSPROJECT_COLUMN_H

#define GRAPHICSPROJECT_COLUMN_H

#include "windows.h"
#include "Button.h"
#include <iostream>

using namespace std;

class Column : public View {
public:
    Column(const vector<View *> &children,
           int margin,
           COLORREF backgroundColor = RGB(51, 181, 137),
           COLORREF strokeColor = RGB(0, 0, 0),
           int strokeThickness = 0
    ) {
        this->backGroundColor = backgroundColor;
        this->strokeColor = strokeColor;
        this->strokeThickness = strokeThickness;
        this->margin = margin;
        this->children = children;
    };

    virtual void drawChildren(HDC &hdc) {
        int heights = height / children.size();
        int widths = width;
        int yCounter = y + children[0]->getMargin();

        for (const auto &i: children) {
            i->setDimensions(
                    x + i->getMargin(),
                    yCounter,
                    widths - (2 * i->getMargin()),
                    heights - (2 * i->getMargin())
            );
            i->draw(hdc);
            yCounter += heights;
        }
    }

    void setDimensions(int x, int y, int width, int height) {
        this->x = x + margin;
        this->y = y + margin;
        this->width = width - 2 * margin;
        this->height = height - 2 * margin;

        rect.left = this->x;
        rect.top = this->y;
        rect.right = this->x + this->width;
        rect.bottom = this->y + this->width;
    }

    void draw(HDC &hdc) {
        drawBoundaries(hdc, x, y, width, height);
//        fillBackground(hdc, x, y, width, height);
        drawChildren(hdc);
    }

    vector<View *> getChildren() {
        return children;
    }

    vector<View *> children;
    int x;
    int y;
    int width;
    int height;
    RECT rect;
    HWND hwnd;
};

#endif //GRAPHICSPROJECT_COLUMN_H
