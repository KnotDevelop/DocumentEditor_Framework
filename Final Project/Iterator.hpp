#pragma once
#include "DocumentElementFactory.hpp"
#include <vector>
#include <memory>

class IIterator {
public:
    virtual ~IIterator() = default;

    virtual bool hasNext() const = 0;

    virtual DocumentElement* next() = 0;
};

class DocumentIterator : public IIterator {
private:
    const std::vector<std::unique_ptr<DocumentElement>>& elements;
    size_t position;

public:
    explicit DocumentIterator(const std::vector<std::unique_ptr<DocumentElement>>& els)
        : elements(els), position(0) {
    }

    bool hasNext() const override {
        return position < elements.size();
    }

    DocumentElement* next() override {
        if (hasNext()) {
            // คืนค่า Pointer ของของชิ้นนี้ แล้วขยับตำแหน่งตัวเองไป +1
            return elements[position++].get();
        }
        return nullptr; // ถ้าหมดแล้วก็คืนค่าความว่างเปล่า
    }
};

class IIterable {
public:
    virtual ~IIterable() = default;

    virtual std::unique_ptr<IIterator> createIterator() const = 0;
};