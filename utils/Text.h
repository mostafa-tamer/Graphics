
#ifndef GRAPHICSPROJECT_TEXT_H

#define GRAPHICSPROJECT_TEXT_H

#include "bits/stdc++.h"
#include "windows.h"
#include "View.h"

using namespace std;

class Text {
public:

    Text(
            string value
    ) {
        setValue(value);
    }

    void setValue(string value) {
        this->value = value;
    }

    string getValue() {
        return value;
    }

    void setBackgroundColor(COLORREF color) {
        this->backgroundColor = color;
    }

    void setTextColor(COLORREF color) {
        this->textColor = color;
    }

    void draw(HWND &hwnd, HDC &hdc) {
        SetTextColor(hdc, textColor);
        SetBkColor(hdc, backgroundColor);

        RECT windowRect;
        GetWindowRect(hwnd, &windowRect);

        int windLeft = windowRect.left;
        int windTop = windowRect.top;
        int windRight = windowRect.right;
        int windBottom = windowRect.bottom;

        RECT textRect = {
                windLeft,
                windTop,
                windRight,
                windBottom
        }; // left, top, right, bottom

//        cout << windLeft << " " << windTop << " " << windBottom << " " << windRight << endl;

        DrawText(hdc, value.c_str(), -1, &textRect,
                 DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

private:
    COLORREF backgroundColor =  RGB(255, 127, 80);
    COLORREF textColor = RGB(0, 0, 0);
    string value = "";

};


#endif //GRAPHICSPROJECT_TEXT_H
