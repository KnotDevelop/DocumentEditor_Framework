#pragma once
#include "Observer.hpp"
#include "Document.hpp"
#include <iostream>

class StatusBar : public IObserver {
public:
    void update(const Document& doc) override {
        std::cout << "\n[StatusBar] Auto-Update! "
            << "Elements Count: " << doc.elements.size()
            << " | Layout Size: " << doc.width << "x" << doc.high << "\n";
    }
};