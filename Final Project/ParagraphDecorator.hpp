#pragma once
#include <iostream>
#include <string>
#include <memory>

using namespace std;

struct ITextDecorator
{
    virtual string getText() const = 0;
    virtual ~ITextDecorator() = default;
};

class PlainText : public ITextDecorator
{
    string text;
public:
    explicit PlainText(string t) : text(std::move(t)) {}

    string getText() const override {
        return text;
    }
};

class BoldDecorator : public ITextDecorator
{
    unique_ptr<ITextDecorator> wrap;

public:
    explicit BoldDecorator(unique_ptr<ITextDecorator> w) : wrap(std::move(w)) {}

    string getText() const override
    {
        return "<b>" + wrap->getText() + "</b>";
    }
};

class ItalicDecorator : public ITextDecorator
{
    unique_ptr<ITextDecorator> wrap;
public:
    explicit ItalicDecorator(unique_ptr<ITextDecorator> w) : wrap(std::move(w)) {}

    string getText() const override {
        return "<i>" + wrap->getText() + "</i>";
    }
};