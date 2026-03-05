#pragma once
#include <iostream>
#include <string>

class Component;

class IMediator {
public:
    virtual ~IMediator() = default;
    virtual void notify(Component* sender, const std::string& event) = 0;
};

class Component {
protected:
    IMediator* mediator;
public:
    explicit Component(IMediator* m = nullptr) : mediator(m) {}

    void setMediator(IMediator* m) {
        mediator = m;
    }
};