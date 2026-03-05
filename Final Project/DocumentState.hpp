#pragma once
#include <memory>
#include <string>
#include <iostream>

class Document;

class IDocumentState {
public:
    virtual ~IDocumentState() = default;

    virtual bool canEdit() const = 0;
    virtual std::string getName() const = 0;

    virtual void requestReview(Document& doc) = 0;
    virtual void approve(Document& doc) = 0;
    virtual void reject(Document& doc) = 0;

    virtual std::unique_ptr<IDocumentState> clone() const = 0;
};

class DraftState : public IDocumentState {
public:
    bool canEdit() const override { return true; }
    std::string getName() const override { return "Draft"; }

    void requestReview(Document& doc) override;
    void approve(Document& doc) override;
    void reject(Document& doc) override;

    std::unique_ptr<IDocumentState> clone() const override { return std::make_unique<DraftState>(*this); }
};

class ReviewState : public IDocumentState {
public:
    bool canEdit() const override { return false; }
    std::string getName() const override { return "Review"; }

    void requestReview(Document& doc) override;
    void approve(Document& doc) override;
    void reject(Document& doc) override;

    std::unique_ptr<IDocumentState> clone() const override { return std::make_unique<ReviewState>(*this); }
};

class PublishedState : public IDocumentState {
public:
    bool canEdit() const override { return false; }
    std::string getName() const override { return "Published"; }

    void requestReview(Document& doc) override;
    void approve(Document& doc) override;
    void reject(Document& doc) override;

    std::unique_ptr<IDocumentState> clone() const override { return std::make_unique<PublishedState>(*this); }
};