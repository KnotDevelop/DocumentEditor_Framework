#pragma once
#include <string>

class IFileSystemComponent {
public:
    virtual ~IFileSystemComponent() = default;
    virtual std::string getName() const = 0;
    virtual void display(int depth = 0) const = 0;
    virtual std::string toJson() const = 0;
};