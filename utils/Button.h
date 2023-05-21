//
// Created by mtame on 5/10/2023.
//


#ifndef GRAPHICSPROJECT_BUTTON_H

#define GRAPHICSPROJECT_BUTTON_H


#include "Text.h"

using namespace std;

class Button : public View {
public:

    Button(
            Text *text,
            const function<void(Button *)> &callbackFunction = {},
            int margin = 0,
            COLORREF backgroundColor = RGB(255, 127, 80),
            COLORREF strokeColor = RGB(0, 0, 0),
            int strokeThickness = 3
    ) : child(text) {
        this->margin = margin;
        this->strokeColor = strokeColor;
        this->backGroundColor = backgroundColor;
        this->strokeThickness = strokeThickness;
        this->callBackFunction = callbackFunction;

        hwnd = new HWND();
        *hwnd = CreateWindow(
                "STATIC", "",
                WS_CHILD | SS_NOTIFY,
                x + strokeThickness,
                y + strokeThickness,
                width - 2 * strokeThickness,
                height - 2 * strokeThickness,
                nullptr, nullptr, nullptr, nullptr
        );
    }

    void setDimensions(int x, int y, int width, int height) override {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    void draw(HDC &hdc) override {
        drawBoundaries(hdc, x, y, width, height);
        fillBackground(hdc, x, y, width, height);

        child->draw(*hwnd, hdc);
    }

    void setOnClickListener(const function<void(Button *)> &callbackFunction) {
        this->callBackFunction = callbackFunction;
    }

    void activateButton(int posX, int posY) {
        if (posX >= x && posX <= x + width &&
            posY >= y && posY <= y + height) {
            isActivated = true;
            callBackFunction(this);
        }
    }

    Text *getChild() {
        return child;
    }

    void setActive(bool value) {
        isActivated = value;
    }

    bool getIsActivated() const {
        return isActivated;
    }

    HWND *getHWND() {
        return hwnd;
    }

private:
    HWND *hwnd;
    Text *child;
    std::string m_text;
    function<void(Button *)> callBackFunction;
    bool isActivated = false;
};


#endif //GRAPHICSPROJECT_BUTTON_H
