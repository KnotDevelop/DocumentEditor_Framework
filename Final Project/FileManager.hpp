#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "Document.hpp"

using namespace std;

// ระบบแปลงข้อมูล
class DocumentSerializer {
public:
    static string toJson(const Document& doc) {
        string json = "{\n";
        json += "  \"header\": \"" + doc.header + "\",\n";
        json += "  \"footer\": \"" + doc.footer + "\",\n";
        json += "  \"elements\": [\n";

        for (size_t i = 0; i < doc.elements.size(); ++i) {
            if (doc.elements[i]) {
                json += "    " + doc.elements[i]->toJson();

                // เติม comma ต่อท้ายถ้ายังไม่ใช่ตัวสุดท้ายของ Array
                if (i < doc.elements.size() - 1) {
                    json += ",";
                }
                json += "\n";
            }
        }

        json += "  ]\n";
        json += "}";
        return json;
    }

    static Document fromJson(const string& jsonString) {
        Document doc;
        doc.header = "[Loaded from File]";
        return doc;
    }
};

// ระบบจัดการไฟล์
class FileHandler {
public:
    static bool writeToFile(const string& path, const string& content) {
        ofstream file(path);
        if (file.is_open()) {
            file << content;
            file.close();
            return true;
        }
        return false;
    }

    static string readFromFile(const string& path) {
        ifstream file(path);
        string content, line;
        if (file.is_open()) {
            while (getline(file, line)) {
                content += line + "\n";
            }
            file.close();
        }
        else {
            cout << "[Error] Cannot find or open file: " << path << "\n";
        }
        return content;
    }
};

// FileManagerFacade
class FileManagerFacade {
public:
    // ฟังก์ชันช่วย Save เอกสาร (Document)
    bool save(const Document& document, const string& path) {
        cout << "[Facade] Starting to save document to: " << path << "...\n";

        string serializedData = DocumentSerializer::toJson(document);

        if (FileHandler::writeToFile(path, serializedData)) {
            cout << "[Facade] [SUCCESS] Document saved!\n";
            return true;
        }
        else {
            cout << "[Facade] [FAILED] Could not save document!\n";
            return false;
        }
    }

    // ฟังก์ชันช่วย Load เอกสาร
    Document load(const string& path) {
        cout << "[Facade] Starting to load document from: " << path << "...\n";

        string rawData = FileHandler::readFromFile(path);
        Document doc = DocumentSerializer::fromJson(rawData);

        cout << "[Facade] [SUCCESS] Document loaded!\n";
        return doc;
    }

    // ฟังก์ชันช่วย Save โครงสร้างโฟลเดอร์แบบยกชุด
    bool saveFileSystem(const IFileSystemComponent& rootNode, const string& path) {
        cout << "[Facade] Starting to save folder structure to: " << path << "...\n";

        // สั่งให้ Root แปลงร่างตัวเองและลูกๆ ลงไปจนสุดสาย
        string serializedData = rootNode.toJson();

        if (FileHandler::writeToFile(path, serializedData)) {
            cout << "[Facade] [SUCCESS] Folder structure saved!\n";
            return true;
        }
        else {
            cout << "[Facade] [FAILED] Could not save folder structure!\n";
            return false;
        }
    }
};