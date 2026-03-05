#pragma once
#include "Document.hpp"
#include <string>
#include <fstream>
#include <iostream>

class IExportStrategy {
public:
    virtual ~IExportStrategy() = default;

    virtual void exportDoc(const Document& doc, const std::string& filename) const = 0;
};

// Export เป็น Markdown จำลอง (.md)
class MarkdownExportStrategy : public IExportStrategy {
public:
    void exportDoc(const Document& doc, const std::string& filename) const override {
        std::cout << "[Export] Exporting as Markdown to " << filename << "...\n";
        std::ofstream file(filename);
        if (file.is_open()) {
            // เขียนโครงสร้างแบบ Markdown
            file << "# " << (doc.header.empty() ? "Untitled Document" : doc.header) << "\n\n";
            file << "> Document State: **" << doc.state->getName() << "**\n\n";

            // จำลองการดึงเนื้อหา
            file << "```json\n" << doc.toJson() << "\n```\n";

            if (!doc.footer.empty()) {
                file << "\n---\n*" << doc.footer << "*\n";
            }
            file.close();
            std::cout << "Success! Markdown saved.\n";
        }
    }
};

// Export เป็น PDF จำลอง (.pdf)
class PDFExportStrategy : public IExportStrategy {
public:
    void exportDoc(const Document& doc, const std::string& filename) const override {
        std::cout << "[Export] Exporting as PDF to " << filename << "...\n";
        std::ofstream file(filename);
        if (file.is_open()) {
            // จำลองโครงสร้างไฟล์ PDF แบบ Binary
            file << "%PDF-1.4\n";
            file << "1 0 obj\n<< /Title (" << doc.header << ") /State (" << doc.state->getName() << ") >>\nendobj\n";
            file << "%%EOF\n";
            file.close();
            std::cout << "Success! PDF saved.\n";
        }
    }
};

class DocumentExporter {
private:
    std::unique_ptr<IExportStrategy> strategy;

public:
    // ฟังก์ชันถอดสลับตอน Runtime
    void setStrategy(std::unique_ptr<IExportStrategy> newStrategy) {
        strategy = std::move(newStrategy);
    }

    void exportDocument(const Document& doc, const std::string& filename) const {
        if (strategy) {
            strategy->exportDoc(doc, filename);
        }
        else {
            std::cout << "[Export Error] No export strategy has been set!\n";
        }
    }
};