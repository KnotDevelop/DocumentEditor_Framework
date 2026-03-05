#pragma once
#include "Document.hpp"
#include "DocumentElementFactory.hpp"

using namespace std;

class DocumentBuilder {
private:
    Document doc;
    IRenderer& renderer; // Bridge Pattern 

public:
    explicit DocumentBuilder(IRenderer& render_impl) : renderer(render_impl)
    {
        doc.high = 29.7f;
        doc.width = 21.0f;
        doc.border = 3.0f;
        doc.header = "";
        doc.footer = "";
    }

    static DocumentBuilder create(IRenderer& render_impl) {
        return DocumentBuilder(render_impl);
    }

    DocumentBuilder& size(float w, float h) {
        doc.width = w;
        doc.high = h;
        return *this;
    }

    DocumentBuilder& border(float b) {
        doc.border = b;
        return *this;
    }

    DocumentBuilder& header(const string& h) {
        doc.header = h;
        return *this;
    }

    DocumentBuilder& footer(const string& f) {
        doc.footer = f;
        return *this;
    }

    DocumentBuilder& add_element(std::unique_ptr<DocumentElement> el) {
        doc.addElement(std::move(el));
        return *this;
    }

    DocumentBuilder& add_paragraph(const string& text, CharacterStyle* style = nullptr) {
        doc.addElement(make_unique<Paragraph>(renderer, text, style));
        return *this;
    }

    DocumentBuilder& add_image(const string& url, const string& caption = "") {
        doc.addElement(make_unique<Image>(renderer, url, caption));
        return *this;
    }

    DocumentBuilder& add_table(int rows, int cols) {
        doc.addElement(make_unique<Table>(renderer, rows, cols));
        return *this;
    }

    DocumentBuilder& remove_element(int index) {
        doc.removeElement(index);
        return *this;
    }

    DocumentBuilder& clear_elements() {
        doc.elements.clear();
        return *this;
    }

    DocumentBuilder& add_shape(const string& type, int p1, int p2 = 0) {
        doc.addElement(make_unique<ShapeAdapter>(renderer, type, p1, p2));
        return *this;
    }

    Document build() {
        return std::move(doc);
    }
};