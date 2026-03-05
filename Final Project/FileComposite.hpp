#pragma once
#include "IFileSystemComponent.hpp"
#include <string>
#include "Document.hpp"

using namespace std;

class File : public IFileSystemComponent
{
    string name;
    Document doc;

public:
    File(const string& name, Document doc) : name(move(name)), doc(move(doc)){}
    std::string getName() const override { return name; }
    void display(int depth = 0) const override {
        std::string indent(depth, ' ');
        std::cout << indent << "- [File] " << name << "\n";
    }
    std::string toJson() const override {
        string json = "{ \"type\": \"File\", \"name\": \"" + name + "\", \"document\": ";
        json += doc.toJson(); // เรียก Document ให้แปลงตัวเอง
        json += " }";
        return json;
    }
};
class Folder : public IFileSystemComponent
{
    std::string name;
    std::vector<std::unique_ptr<IFileSystemComponent>> children;

public:
    explicit Folder(std::string name) : name(std::move(name)) {}

    std::string getName() const override { return name; }

    void add(std::unique_ptr<IFileSystemComponent> component)
    {
        if (component) {
            children.push_back(std::move(component));
        }
    }

    void display(int depth = 0) const override {
        std::string indent(depth, ' ');
        std::cout << indent << "+ [Folder] " << name << "\n";

        for (const auto& child : children) {
            child->display(depth + 2);
        }
    }
    std::string toJson() const override {
        string json = "{ \"type\": \"Folder\", \"name\": \"" + name + "\", \"contents\": [\n";
        for (size_t i = 0; i < children.size(); ++i) {
            json += "    " + children[i]->toJson(); // สั่งให้ลูกแปลงตัวเอง
            if (i < children.size() - 1) json += ",";
            json += "\n";
        }
        json += "] }";
        return json;
    }
};