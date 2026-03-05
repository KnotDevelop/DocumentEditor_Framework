#pragma once
#include <iostream>

using namespace std;

class LegacyShapeDrawer {
public:
    void drawOldCircle(int radius) const {
        cout << "[Legacy API] Drawing Circle with radius: " << radius << "\n";
    }

    void drawOldRectangle(int width, int height) const {
        cout << "[Legacy API] Drawing Rectangle " << width << " x " << height << "\n";
    }
};