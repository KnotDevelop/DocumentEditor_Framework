#pragma once
#include <iostream>
#include <string>
#include "CharacterStyle.hpp"
#include "LegacyShapeDrawer.hpp"

using namespace std;

struct IRenderer {
    virtual ~IRenderer() = default;
    virtual void renderParagraph(const string& text, CharacterStyle* style) const = 0;
    virtual void renderImage(const string& url, const string& caption) const = 0;
    virtual void renderTable(int rows, int cols) const = 0;
    virtual void renderShape(const string& shapeType, int p1, int p2) const = 0;
};

struct ConsoleRenderer : public IRenderer {
    LegacyShapeDrawer legacyDrawer;
    void renderParagraph(const string& text, CharacterStyle* style) const override {
        cout << "[Console Paragraph]: " << text;
        if (style) cout << " (Style: " << style->color << " text)\n";
        else cout << "\n";
    }
    void renderImage(const string& url, const string& caption) const override {
        cout << "[Console Image]: " << url << " | Caption: " << caption << "\n";
    }
    void renderTable(int rows, int cols) const override {
        cout << "[Console Table]: " << rows << " rows x " << cols << " columns\n";
    }
    void renderShape(const string& shapeType, int p1, int p2) const override {
        if (shapeType == "Circle") {
            legacyDrawer.drawOldCircle(p1);
        }
        else if (shapeType == "Rectangle") {
            legacyDrawer.drawOldRectangle(p1, p2);
        }
        else {
            cout << "[Console Shape]: Unknown shape\n";
        }
    }
};

struct HTMLRenderer : public IRenderer {
    void renderParagraph(const string& text, CharacterStyle* style) const override {
        if (style) {
            cout << "<p style=\"font-family: '" << style->font_name
                << "'; font-size: " << style->font_size
                << "px; color: " << style->color << ";\">"
                << text << "</p>\n";
        }
        else {
            cout << "<p>" << text << "</p>\n";
        }
    }
    void renderImage(const string& url, const string& caption) const override {
        cout << "<figure>\n  <img src=\"" << url << "\" alt=\"" << caption << "\">\n"
            << "  <figcaption>" << caption << "</figcaption>\n</figure>\n";
    }
    void renderTable(int rows, int cols) const override {
        cout << "<table border=\"1\">\n";
        for (int i = 0; i < rows; ++i) {
            cout << "  <tr>";
            for (int j = 0; j < cols; ++j) cout << "<td>Cell</td>";
            cout << "</tr>\n";
        }
        cout << "</table>\n";
    }
    void renderShape(const string& shapeType, int p1, int p2) const override {
        if (shapeType == "Circle") {
            cout << "<svg height=\"" << p1 * 2 << "\" width=\"" << p1 * 2 << "\">\n"
                << "  <circle cx=\"" << p1 << "\" cy=\"" << p1 << "\" r=\"" << p1 << "\" fill=\"green\" />\n"
                << "</svg>\n";
        }
        else if (shapeType == "Rectangle") {
            cout << "<svg height=\"" << p2 << "\" width=\"" << p1 << "\">\n"
                << "  <rect width=\"" << p1 << "\" height=\"" << p2 << "\" style=\"fill:blue;\" />\n"
                << "</svg>\n";
        }
    }
};