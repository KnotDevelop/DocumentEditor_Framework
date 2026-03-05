#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "CharacterStyle.hpp"
#include "IRenderer.hpp"
#include "DocumentElementFactory.hpp"
#include "LegacyShapeDrawer.hpp"

using namespace std;

class Paragraph;
class Image;
class Table;
class ShapeAdapter;

class IDocumentVisitor {
public:
    virtual ~IDocumentVisitor() = default;
    virtual void visit(Paragraph& element) = 0;
    virtual void visit(Image& element) = 0;
    virtual void visit(Table& element) = 0;
    virtual void visit(ShapeAdapter& element) = 0;
};

class IEventHandler {
protected:
    IEventHandler* nextHandler = nullptr;

public:
    virtual ~IEventHandler() = default;

    void setNext(IEventHandler* handler) {
        nextHandler = handler;
    }

    // ฟังก์ชันจัดการ Event (ถ้าทำไม่ได้ ให้ส่งต่อ)
    virtual void handleEvent(const string& eventType) {
        if (nextHandler) {
            nextHandler->handleEvent(eventType);
        }
        else {
            cout << " [Chain] Event dropped: '" << eventType << "' (No one handled it)\n";
        }
    }
};

class DocumentElement: public IEventHandler 
{
protected:
    IRenderer& renderer;
public:
    DocumentElement(IRenderer& render_impl) : renderer(render_impl) {}
    virtual ~DocumentElement() = default;
    virtual void display() const = 0;
    virtual unique_ptr<DocumentElement> cloneElement() const = 0;
    virtual string toJson() const = 0;
    virtual void accept(IDocumentVisitor& visitor) = 0;
};

class Paragraph : public DocumentElement 
{
private:
    string text;
    CharacterStyle* style;
public:
    Paragraph(IRenderer& render_impl, string t, CharacterStyle* s = nullptr)
        : DocumentElement(render_impl), text(move(t)), style(s) {
    }

    Paragraph(const Paragraph& other)
        : DocumentElement(other.renderer), text(other.text), style(other.style) {
    }

    void display() const override {
        renderer.renderParagraph(text, style);
    }
    void setText(string t) { text = std::move(t); }
    unique_ptr<DocumentElement> cloneElement() const override {
        return make_unique<Paragraph>(*this);
    }
    string toJson() const override {
        string json = "{ \"type\": \"Paragraph\", \"text\": \"" + text + "\"";

        if (style) {
            json += ", \"font\": \"" + style->font_name + "\", "
                "\"size\": " + to_string(style->font_size) + ", "
                "\"color\": \"" + style->color + "\"";
        }
        json += " }";
        return json;
    }
    string getText() const { return text; }
    void accept(IDocumentVisitor& visitor) override { visitor.visit(*this); }
    void handleEvent(const string& eventType) override {
        if (eventType == "select") {
            cout << "[Paragraph Handler] Text Highlighted: '" << text.substr(0, 10) << "...'\n";
        }
        else {
            IEventHandler::handleEvent(eventType);
        }
    }
};

class Image : public DocumentElement {
private:
    string url;
    string caption;
public:
    Image(IRenderer& render_impl, string u, string c = "")
        : DocumentElement(render_impl), url(move(u)), caption(move(c)) {
    }

    Image(const Image& other)
        : DocumentElement(other.renderer), url(other.url), caption(other.caption) {
    }

    void display() const override {
        renderer.renderImage(url, caption);
    }
    unique_ptr<DocumentElement> cloneElement() const override {
        return make_unique<Image>(*this);
    }
    string toJson() const override {
        return "{ \"type\": \"Image\", \"url\": \"" + url + "\", \"caption\": \"" + caption + "\" }";
    }
    string getUrl() const { return url; }
    string getCaption() const { return caption; }
    void accept(IDocumentVisitor& visitor) override { visitor.visit(*this); }
    void handleEvent(const string& eventType) override {
        if (eventType == "double_click") {
            cout << "[Image Handler] Opening Image Editor for: " << caption << "\n";
        }
        else {
            IEventHandler::handleEvent(eventType);
        }
    }
};

class Table : public DocumentElement {
private:
    int rows, cols;
public:
    Table(IRenderer& render_impl, int r, int c)
        : DocumentElement(render_impl), rows(r), cols(c) {
    }

    Table(const Table& other)
        : DocumentElement(other.renderer), rows(other.rows), cols(other.cols) {
    }

    void display() const override {
        renderer.renderTable(rows, cols);
    }
    unique_ptr<DocumentElement> cloneElement() const override {
        return make_unique<Table>(*this);
    }
    string toJson() const override {
        return "{ \"type\": \"Table\", \"rows\": " + to_string(rows) + ", \"cols\": " + to_string(cols) + " }";
    }
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    void accept(IDocumentVisitor& visitor) override { visitor.visit(*this); }
};

class ShapeAdapter : public DocumentElement {
private:
    string shapeType;
    int param1, param2;
    LegacyShapeDrawer legacyDrawer;

public:
    ShapeAdapter(IRenderer& render_impl, string type, int p1, int p2 = 0)
        : DocumentElement(render_impl), shapeType(move(type)), param1(p1), param2(p2) {
    }

    ShapeAdapter(const ShapeAdapter& other)
        : DocumentElement(other.renderer), shapeType(other.shapeType),
        param1(other.param1), param2(other.param2), legacyDrawer(other.legacyDrawer) {
    }

    void display() const override {
        if (shapeType == "Circle") {
            legacyDrawer.drawOldCircle(param1);
        }
        else if (shapeType == "Rectangle") {
            legacyDrawer.drawOldRectangle(param1, param2);
        }
        else {
            cout << "[Adapter] Unknown shape type!\n";
        }
    }

    unique_ptr<DocumentElement> cloneElement() const override {
        return make_unique<ShapeAdapter>(*this);
    }

    string toJson() const override {
        return "{ \"type\": \"ShapeAdapter\", \"shape\": \"" + shapeType +
            "\", \"p1\": " + to_string(param1) + ", \"p2\": " + to_string(param2) + " }";
    }
    string getShapeType() const { return shapeType; }
    int getParam1() const { return param1; }
    int getParam2() const { return param2; }
    void accept(IDocumentVisitor& visitor) override { visitor.visit(*this); }
};

enum class ElementType
{
    Paragraph,
    Image,
    Table,
    Shape
};

class DocumentElementFactory
{
    IRenderer& renderer;

public:
    explicit DocumentElementFactory(IRenderer& render_impl) : renderer(render_impl) {}

    unique_ptr<DocumentElement> makeElement(ElementType element)
    {
        switch (element)
        {
        case ElementType::Paragraph:
            return make_unique<Paragraph>(renderer, "Default Text", nullptr);

        case ElementType::Image:
            return make_unique<Image>(renderer, "default.png", "Default Caption");

        case ElementType::Table:
            return make_unique<Table>(renderer, 1, 1);

        case ElementType::Shape:
            return make_unique<ShapeAdapter>(renderer, "Circle", 10);

        default:
            return nullptr;
        }
    }
    unique_ptr<DocumentElement> makeShape(string shapeType, int p1, int p2 = 0) {
        return make_unique<ShapeAdapter>(renderer, shapeType, p1, p2);
    }
};
