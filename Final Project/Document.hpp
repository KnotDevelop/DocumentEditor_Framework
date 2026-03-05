#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "DocumentElementFactory.hpp"
#include "IClonable.hpp"
#include "Observer.hpp"
#include "Iterator.hpp"
#include "DocumentState.hpp" 

using namespace std;

class DocumentMemento;

class Document : public IClonable, public IObservable, public IIterable, public IEventHandler
{
public:
    float high, width, border;
    string header, footer;
    vector<unique_ptr<DocumentElement>> elements;

    std::unique_ptr<IDocumentState> state;

    Document()
        : high(0.0f), width(0.0f), border(0.0f), header(""), footer("") {
        state = std::make_unique<DraftState>();
    }

    Document(const Document& other)
        : high(other.high), width(other.width), border(other.border),
        header(other.header), footer(other.footer)
    {
        state = other.state->clone();
        for (const auto& el : other.elements) {
            if (el) {
                elements.push_back(el->cloneElement());
            }
        }
    }

    Document(Document&&) noexcept = default;
    Document& operator=(Document&&) noexcept = default;

    string toJson() const {
        string json = "{\n";
        json += "      \"header\": \"" + header + "\",\n";
        json += "      \"footer\": \"" + footer + "\",\n";
        json += "      \"state\": \"" + state->getName() + "\",\n";
        json += "      \"elements\": [\n";
        for (size_t i = 0; i < elements.size(); ++i) {
            if (elements[i]) {
                json += "        " + elements[i]->toJson();
                if (i < elements.size() - 1) json += ",";
                json += "\n";
            }
        }
        json += "      ]\n    }";
        return json;
    }

    friend ostream& operator << (ostream& os, const Document& obj) {
        return os
            << " paper size: " << obj.width << "x" << obj.high << "\n"
            << " border size: " << obj.border << "\n"
            << " state: " << obj.state->getName() << "\n"
            << " header text: " << obj.header << "\n"
            << " footer text: " << obj.footer << "\n";
    }

    void display() const {
        std::cout << "\n=== Document Information ===\n"
            << " Paper Size:  " << width << " x " << high << "\n"
            << " Border Size: " << border << "\n"
            << " State:       [" << state->getName() << "]\n"
            << " Header:      " << (header.empty() ? "None" : header) << "\n"
            << " Footer:      " << (footer.empty() ? "None" : footer) << "\n"
            << "--- Elements (" << elements.size() << ") ---\n";

        for (size_t i = 0; i < elements.size(); ++i) {
            if (elements[i]) {
                std::cout << " [" << i << "] ";
                elements[i]->display();
            }
        }
        std::cout << "============================\n";
    }

    // ฟังก์ชันควบคุม State และการกระทำ
    void setState(std::unique_ptr<IDocumentState> newState) {
        state = std::move(newState);
        std::cout << "[Workflow] Document state changed to: " << state->getName() << "\n";
        notifyObservers(*this);
    }

    void requestReview() { state->requestReview(*this); }
    void approve() { state->approve(*this); }
    void reject() { state->reject(*this); }

    void addElement(unique_ptr<DocumentElement> el) {
        if (!state->canEdit()) {
            std::cout << "  [Blocked] Cannot ADD element. Document is locked in '" << state->getName() << "' state!\n";
            return;
        }
        if (el) {
            el->setNext(this);
            elements.push_back(move(el));
            notifyObservers(*this);
        }
    }

    void removeElement(int index) {
        if (!state->canEdit()) {
            std::cout << "  [Blocked] Cannot REMOVE element. Document is locked in '" << state->getName() << "' state!\n";
            return;
        }
        if (index < elements.size()) {
            elements.erase(elements.begin() + index);
            notifyObservers(*this);
        }
        else {
            cout << "[Error] Index out of range\n";
        }
    }

    std::unique_ptr<IClonable> clone() const override {
        return make_unique<Document>(*this);
    }

    std::unique_ptr<DocumentMemento> saveState() const;
    void restoreState(const DocumentMemento& memento);

    std::unique_ptr<IIterator> createIterator() const override {
        return std::make_unique<DocumentIterator>(elements);
    }

    void accept(IDocumentVisitor& visitor) {
        for (auto& el : elements) {
            if (el) {
                el->accept(visitor);
            }
        }
    }

    void handleEvent(const string& eventType) override {
        if (eventType == "save_shortcut") {
            cout << "[Document Handler] Ctrl+S detected! Auto-saving document...\n";
        }
        else if (eventType == "close") {
            cout << "[Document Handler] Closing document workflow.\n";
        }
        else {
            IEventHandler::handleEvent(eventType);
        }
    }
};

#include "DocumentMemento.hpp"

// Implementation การทำงานจริง (หลบปัญหา Circular Dependency)

inline DocumentMemento::DocumentMemento(std::unique_ptr<Document> s) : state(std::move(s)) {}
inline DocumentMemento::~DocumentMemento() = default;

inline std::unique_ptr<DocumentMemento> Document::saveState() const {
    auto clonedDoc = std::unique_ptr<Document>(static_cast<Document*>(this->clone().release()));
    return std::make_unique<DocumentMemento>(std::move(clonedDoc));
}

inline void Document::restoreState(const DocumentMemento& memento) {
    if (memento.state) {
        Document tempCopy(*(memento.state));
        *this = std::move(tempCopy);
        notifyObservers(*this);
    }
}

// การทำงานของ State Pattern ที่ต้องใช้คำสั่งจาก Document
inline void DraftState::requestReview(Document& doc) { doc.setState(std::make_unique<ReviewState>()); }
inline void DraftState::approve(Document& doc) { std::cout << "  [State Error] Cannot approve a Draft.\n"; }
inline void DraftState::reject(Document& doc) { std::cout << "  [State Error] Cannot reject a Draft.\n"; }

inline void ReviewState::requestReview(Document& doc) { std::cout << "  [State Error] Already in Review.\n"; }
inline void ReviewState::approve(Document& doc) { doc.setState(std::make_unique<PublishedState>()); }
inline void ReviewState::reject(Document& doc) { doc.setState(std::make_unique<DraftState>()); }

inline void PublishedState::requestReview(Document& doc) { std::cout << "  [State Error] Cannot review a Published document.\n"; }
inline void PublishedState::approve(Document& doc) { std::cout << "  [State Error] Already Published.\n"; }
inline void PublishedState::reject(Document& doc) { doc.setState(std::make_unique<DraftState>()); }