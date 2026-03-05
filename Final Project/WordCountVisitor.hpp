#pragma once
#include "DocumentElementFactory.hpp"
#include <iostream>
#include <sstream>
#include <string>

// Word Count Visitor
class WordCountVisitor : public IDocumentVisitor {
private:
    int count = 0;
public:
    int getWordCount() const { return count; }

    void visit(Paragraph& element) override {
        // นับคำในย่อหน้าโดยแยกด้วยช่องว่าง (Space)
        std::stringstream ss(element.getText());
        std::string word;
        while (ss >> word) { count++; }
    }

    void visit(Image& element) override {
        // นับคำใน Caption ของรูปภาพด้วย
        std::stringstream ss(element.getCaption());
        std::string word;
        while (ss >> word) { count++; }
    }

    // ตารางกับรูปทรง ไม่รู้จะนับคำยังไง ปล่อยว่างไว้
    void visit(Table& element) override {}
    void visit(ShapeAdapter& element) override {}
};

// XML Export Visitor
class XMLExportVisitor : public IDocumentVisitor {
private:
    std::string xml = "";
public:
    std::string getXML() const { return xml; }

    void visit(Paragraph& element) override {
        xml += "  <Paragraph>\n    <Text>" + element.getText() + "</Text>\n  </Paragraph>\n";
    }
    void visit(Image& element) override {
        xml += "  <Image url=\"" + element.getUrl() + "\" caption=\"" + element.getCaption() + "\" />\n";
    }
    void visit(Table& element) override {
        xml += "  <Table rows=\"" + std::to_string(element.getRows()) +
            "\" cols=\"" + std::to_string(element.getCols()) + "\" />\n";
    }
    void visit(ShapeAdapter& element) override {
        xml += "  <Shape type=\"" + element.getShapeType() +
            "\" p1=\"" + std::to_string(element.getParam1()) +
            "\" p2=\"" + std::to_string(element.getParam2()) + "\" />\n";
    }
};