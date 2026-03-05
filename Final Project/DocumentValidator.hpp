#pragma once
#include "Document.hpp"
#include "Iterator.hpp"
#include <iostream>
#include <string>

class DocumentValidator {
public:
    virtual ~DocumentValidator() = default;

    void validate(const Document& doc) {
        std::cout << "\n[Validator] Starting Validation Process...\n";

        prepareValidation();

        bool headerOk = checkHeader(doc);
        bool bodyOk = checkBody(doc);

        summarize(headerOk && bodyOk);
    }

protected:
    // เตรียมความพร้อม
    virtual void prepareValidation() {
        std::cout << "  1. Loading checking rules...\n";
    }

    // ตรวจหัวกระดาษ
    virtual bool checkHeader(const Document& doc) = 0;

    // ตรวจเนื้อหา
    virtual bool checkBody(const Document& doc) = 0;

    void summarize(bool passed) {
        std::cout << "  4. Validation Result: " << (passed ? "PASSED" : "FAILED") << "\n";
    }
};

class FormatValidator : public DocumentValidator {
protected:
    bool checkHeader(const Document& doc) override {
        std::cout << "  2. Checking Header format...\n";
        if (doc.header.empty()) {
            std::cout << "     ->Error: Header is empty!\n";
            return false;
        }
        return true;
    }

    bool checkBody(const Document& doc) override {
        std::cout << "  3. Checking Body content (Paragraphs must not be empty)...\n";
        auto it = doc.createIterator();
        bool allGood = true;

        while (it->hasNext()) {
            // (ในโค้ดจริงอาจจะ cast เพื่อเช็ค Paragraph)
            // สมมติว่าผ่านหมด
            it->next();
        }

        if (doc.elements.empty()) {
            std::cout << "     ->Error: Document has no content!\n";
            return false;
        }
        return true;
    }
};

class SecurityValidator : public DocumentValidator {
protected:
    void prepareValidation() override {
        std::cout << "  1. Loading top-secret keyword database...\n";
    }

    bool checkHeader(const Document& doc) override {
        std::cout << "  2. Scanning Header for classification...\n";
        // ถ้าเจอคำว่า Top Secret ถือว่าผ่าน
        if (doc.header.find("Top Secret") != std::string::npos) {
            std::cout << "     ->Secure Header found.\n";
            return true;
        }
        std::cout << "     ->Header is normal (Not Top Secret).\n";
        return true; // ไม่ถือว่าผิด
    }

    bool checkBody(const Document& doc) override {
        std::cout << "  3. Scanning Body for sensitive keywords...\n";
        // ใช้ Iterator วนลูปตรวจ (สมมติว่าตรวจแล้วปลอดภัย)
        return true;
    }
};