#pragma once
#include <vector>
#include <algorithm>

class Document;

class IObserver {
public:
    virtual ~IObserver() = default;

    // ฟังก์ชันนี้จะถูกเรียกอัตโนมัติเมื่อเอกสารมีการเปลี่ยนแปลง
    virtual void update(const Document& doc) = 0;
};

class IObservable {
private:
    std::vector<IObserver*> observers;

public:
    IObservable() = default;
    virtual ~IObservable() = default;

    IObservable(const IObservable&) {}
    IObservable& operator=(const IObservable&) { return *this; }
    IObservable(IObservable&&) noexcept {}
    IObservable& operator=(IObservable&&) noexcept { return *this; }

    void addObserver(IObserver* observer) {
        if (std::find(observers.begin(), observers.end(), observer) == observers.end()) {
            observers.push_back(observer);
        }
    }

    void removeObserver(IObserver* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

protected:
    void notifyObservers(const Document& doc) {
        for (IObserver* obs : observers) {
            obs->update(doc);
        }
    }
};