#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;


class CharacterStyle
{
public:
    string font_name;
    int font_size;
    string color;

    CharacterStyle(string fontName, int fontSize, string color)
        : font_name(move(fontName)), font_size(fontSize), color(move(color)){
    }

    void print(char value) const {
        std::cout << "Char: " << value << " (Font: " << font_name << ", Size: " << font_size << ")" << std::endl;
    }
};

class StyleFactory
{
    unordered_map<string, CharacterStyle*> charStyle;

public:
    CharacterStyle* get_style(const string& font, int size, const string& color = "Black")
    {
        string key = font + to_string(size) + color;
        if (charStyle.find(key) == charStyle.end()) {
            charStyle[key] = new CharacterStyle(font, size, color);
        }

        return charStyle[key];
    }
};