#pragma once
#include <memory>

class Document;

class DocumentMemento {
private:
    std::unique_ptr<Document> state;
    friend class Document;
public:
    explicit DocumentMemento(std::unique_ptr<Document> s);
    ~DocumentMemento();
};