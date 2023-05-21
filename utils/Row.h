//
// Created by mtame on 5/10/2023.
//

#ifndef GRAPHICSPROJECT_ROW_H
#define GRAPHICSPROJECT_ROW_H


#include "Column.h"

class Row : public Column {

public:
    Row(
            const vector<View *> &children,
            int margin,
            COLORREF backgroundColor = RGB(51, 181, 137),
            COLORREF strokeColor = RGB(0, 0, 0),
            int strokeThickness = 4
    ) : Column(children, margin, backgroundColor, strokeThickness) {}

    void drawChildren(HDC hdc) {
        int heights = height;
        int widths = width / children.size();
        int xCounter = x + children[0]->getMargin();

        for (const auto &i: children) {
            i->setDimensions(
                    xCounter,
                    y + i->getMargin(),
                    widths - (2 * i->getMargin()),
                    heights - (2 * i->getMargin())
            );
            i->draw(hdc);
            xCounter += widths;
        }
    }

    void draw(HDC &hdc) override {
        drawBoundaries(hdc, x, y, width, height);
//        fillBackground(hdc, x, y, width, height);
        drawChildren(hdc);
    }
};


#endif //GRAPHICSPROJECT_ROW_H
