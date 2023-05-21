
#include "windows.h"
#include "utils/Column.h"
#include "utils/Text.h"
#include "utils/Button.h"
#include "utils/Row.h"
#include "utils/Draw.h"
#include <string>
#include <cmath>
#include <functional>


//using namespace std;

const int margin6 = 6;
int margin14 = 14;

COLORREF mainColor = RGB(0, 0, 0);

Button *colorRedButton = new Button(
        new Text(""),
        [](Button *button) {
            mainColor = button->getBackGroundColor();
        },
        margin14, RGB(255, 0, 0));

Button *colorGreenButton = new Button(
        new Text(""),
        [](Button *button) {
            mainColor = button->getBackGroundColor();

        },
        margin14, RGB(51, 181, 137));

Button *colorBlueButton = new Button(
        new Text(""),
        [](Button *button) {
            mainColor = button->getBackGroundColor();

        },
        margin14, RGB(0, 0, 255));

Button *colorYellowButton = new Button(
        new Text(""),
        [](Button *button) {
            mainColor = button->getBackGroundColor();
        },
        margin14, RGB(255, 255, 0));

Button *colorBlackButton = new Button(
        new Text(""),
        [](Button *button) {
            mainColor = button->getBackGroundColor();
        },
        margin14, RGB(0, 0, 0));

Button *colorWhiteButton = new Button(
        new Text(""),
        [](Button *button) {
            mainColor = button->getBackGroundColor();
        },
        margin14, RGB(255, 255, 255));

Button *colorGrayButton = new Button(
        new Text(""),
        [](Button *button) {
            mainColor = button->getBackGroundColor();
        },
        margin14, RGB(192, 192, 192));

Row *row = new Row(
        {
                colorRedButton,
                colorGreenButton,
                colorBlueButton,
                colorYellowButton,
                colorBlackButton,
                colorWhiteButton,
                colorGrayButton
        }, 0
);


Button *drawLineButton = new Button(
        new Text("Draw Line"),
        [](Button *button) {

        }, margin6);

Button *fillShapeButton = new Button(
        new Text("Fill Shape"),
        [](Button *button) {

        }, margin6);

Button *changeWidowBackgroundColorButton = new Button(
        new Text("Change Window Color"),
        [](Button *button) {

        }, margin6);

Button *drawCircleButton = new Button(
        new Text("Draw Circle"),
        [](Button *button) {
        }, margin6);

Button *drawDecoratedCircleButton = new Button(
        new Text("Draw Decorated Circle"),
        [](Button *button) {
        }, margin6);

Button *drawSquareButton = new Button(
        new Text("Draw Square"),
        [](Button *button) {
        }, margin6);

Button *drawRectangleButton = new Button(
        new Text("Draw Rectangle"),
        [](Button *button) {
        }, margin6);

Button *drawSplineButton = new Button(
        new Text("Draw Spline"),
        [](Button *button) {
        }, margin6);

Button *drawEllipseButton = new Button(
        new Text("Draw Ellipse"),
        [](Button *button) {
        }, margin6);

Button *drawRectangleClippingButton = new Button(
        new Text("Draw Rectangle Clipping"),
        [](Button *button) {
        }, margin6);

Button *drawSquareClippingButton = new Button(
        new Text("Draw Square Clipping"),
        [](Button *button) {
        }, margin6);

Button *writeToFileButton = new Button(
        new Text("Write To File"),
        [](Button *button) {
        }, margin6);

Button *circleClippingButton = new Button(
        new Text("Circle Clipping"),
        [](Button *button) {
        }, margin6);

Button *readFromFileButton = new Button(
        new Text("Read From File"),
        [](Button *button) {
        }, margin6);


Button *translateLineButton = new Button(
        new Text("Translate Line"),
        [](Button *button) {
        }, margin6);

Button *scaleLineButton = new Button(
        new Text("Scale Line"),
        [](Button *button) {
        }, margin6);


Column *column = new Column(
        {
                drawLineButton,
                fillShapeButton,
                changeWidowBackgroundColorButton,
                drawCircleButton,
                drawDecoratedCircleButton,
                drawSquareButton,
                drawRectangleButton,
                drawSplineButton,
                drawEllipseButton,
                drawRectangleClippingButton,
                drawSquareClippingButton,
                circleClippingButton,
                translateLineButton,
                scaleLineButton,
                writeToFileButton,
                readFromFileButton
        }, 0
);

void applyClickListeners(int posX, int posY, const vector<View *> &rowChildren) {
    for (auto &i: rowChildren) {
        if (type_index(typeid(*i)) == type_index(typeid(Button))) {
            Button *button = dynamic_cast<Button *>(i);
            if (button) {
                button->activateButton(posX, posY);
            }
        }
    }
}

void readAndWrite(HWND &hwnd, int &width, int &height) {
    writeToFileButton->setOnClickListener([](Button *button) {
        vector<COLORREF> pixelsColor(100000);
        cout << "Start writing to file..." << endl;
        writeToFile(pixels, "output.txt");
    });

    readFromFileButton->setOnClickListener([&width, &height, &hwnd](Button *button) {

        HDC hdc = GetDC(hwnd);

        auto fileOutput = readColorsFromFile("output.txt");
        cout << "Start Coloring..." << endl;

        for (auto i: fileOutput) {
            SetPixel(hdc, i.x, i.y, RGB(i.red, i.green, i.blue));
        }

        cout << "Done!" << endl;
        ReleaseDC(hwnd, hdc);
    });
}

void drawShapes(HWND hwnd, int width, int height, int posX, int posY) {
    Draw::changeWidowColor(hwnd, width / 5 + 1, height / 5 + 1, width, height / 5 + 1, width, height, width / 5 + 1,
                           height, mainColor, changeWidowBackgroundColorButton);
    Draw::drawLine(hwnd, posX, posY, mainColor, drawLineButton);
    Draw::fillShape(hwnd, posX, posY, mainColor, RGB(0, 0, 0), fillShapeButton);
    Draw::drawCircle(hwnd, posX, posY, mainColor, drawCircleButton);
    Draw::drawDecoratedCircle(hwnd, posX, posY, mainColor, drawDecoratedCircleButton);
    Draw::drawSquare(hwnd, posX, posY, mainColor, drawSquareButton);
    Draw::drawRectangle(hwnd, posX, posY, mainColor, drawRectangleButton);
    Draw::drawSplines(hwnd, posX, posY, mainColor, drawSplineButton);
    Draw::drawEllipse(hwnd, posX, posY, mainColor, drawEllipseButton);
    Draw::rectangleClipping(hwnd, posX, posY, mainColor, drawRectangleClippingButton);
    Draw::squareClipping(hwnd, posX, posY, mainColor, drawSquareClippingButton);
    Draw::circleClipping(hwnd, mainColor, circleClippingButton);
}

void translateAndScaleLine(HWND &hwnd) {
    translateLineButton->setOnClickListener([&hwnd](Button *button) {
        HDC hdc = GetDC(hwnd);

        cout << "Enter xs and ys" << endl;
        int xs, ys, xe, ye, dx, dy;
        //250 250 500 500 200 100

        cout << "Enter xs: ";
        cin >> xs;
        cout << "Enter ys: ";
        cin >> ys;
        cout << "Enter xe: ";
        cin >> xe;
        cout << "Enter ye: ";
        cin >> ye;
        cout << "Enter dx: ";
        cin >> dx;
        cout << "Enter dy: ";
        cin >> dy;

        Matrix2 translationMatrix = translate(dx, dy);
        Vector2 startPoint(xs, ys);
        Vector2 endPoint(xe, ye);

        Vector2 translatedStartPoint = translationMatrix * startPoint;
        Vector2 translatedEndPoint = translationMatrix * endPoint;

        Algorithms::DrawParametricLine(
                hdc,
                xs, ys,
                xe, ye,
                mainColor
        );

        Algorithms::DrawParametricLine(
                hdc,
                translatedStartPoint[0], translatedStartPoint[1],
                translatedEndPoint[0], translatedEndPoint[1],
                mainColor
        );
        ReleaseDC(hwnd, hdc);

    });

    scaleLineButton->setOnClickListener([&hwnd](Button *button) {
        HDC hdc = GetDC(hwnd);

        cout << "Enter xs and ys" << endl;
        int xs, ys, xe, ye, alpha, beta;
        //300 300 600 600 2 1

        cout << "Enter xs: ";
        cin >> xs;
        cout << "Enter ys: ";
        cin >> ys;
        cout << "Enter xe: ";
        cin >> xe;
        cout << "Enter ye: ";
        cin >> ye;
        cout << "Enter alpha: ";
        cin >> alpha;
        cout << "Enter beta: ";
        cin >> beta;

        Matrix2 scalingMatrix = scale(alpha, beta);
        Vector2 startPoint(xs, ys);
        Vector2 endPoint(xe, ye);

        Vector2 scaledStartPoint = scalingMatrix * startPoint;
        Vector2 scaledEndPoint = scalingMatrix * endPoint;

        Algorithms::DrawParametricLine(
                hdc,
                xs, ys,
                xe, ye,
                mainColor
        );

        Algorithms::DrawParametricLine(
                hdc,
                scaledStartPoint[0], scaledStartPoint[1],
                scaledEndPoint[0], scaledEndPoint[1],
                mainColor
        );

        ReleaseDC(hwnd, hdc);

    });

}

LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp) {
    RECT window;
    GetWindowRect(hwnd, &window);

    int width = window.right - window.left - 15;
    int height = window.bottom - window.top - 40;

    switch (mcode) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            column->setDimensions(0, 0, width / 5, height);
            column->draw(hdc);

            row->setDimensions((width / 5) + 1, 0, width - width / 5, height / 5);
            row->draw(hdc);

            EndPaint(hwnd, &ps);
            break;
        }
        case WM_LBUTTONDOWN: {
            int posX = LOWORD(lp);
            int posY = HIWORD(lp);

            readAndWrite(hwnd, width, height);
            translateAndScaleLine(hwnd);

            applyClickListeners(posX, posY, row->getChildren());
            applyClickListeners(posX, posY, column->getChildren());

//            drawShapes(hwnd, width, height, posX, posY);

            thread myThread(drawShapes, hwnd, width, height, posX, posY);
            myThread.detach();


            break;
        }
        case WM_RBUTTONDOWN: {

        }
        case WM_SETCURSOR:
            SetCursor(LoadCursor(NULL, IDC_HAND));
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, mcode, wp, lp);
    }
    return 0;
}


int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmd, int nsh) {
    WNDCLASS wc;
    wc.cbClsExtra = wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(51, 181, 137));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = hinst;
    wc.lpfnWndProc = MyWndProc;
    wc.lpszClassName = "MyClass";
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);

    // Get the size of the screen
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Calculate the position of the window to center it on the screen
    int windowWidth = 1080;
    int windowHeight = 720;
    int windowX = (screenWidth - windowWidth) / 2;
    int windowY = (screenHeight - windowHeight) / 2;

    HWND hwnd = CreateWindow("MyClass", "My First Window", WS_OVERLAPPEDWINDOW, windowX, windowY, windowWidth,
                             windowHeight, NULL, NULL, hinst, 0);
    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}