#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <memory>

using namespace std;

class IImage {
public:
    virtual ~IImage() = default;
    virtual void draw() const = 0;
};

class RealImage : public IImage {
private:
    string filename;
public:
    RealImage(const string& filename) : filename(filename) {
        cout << "Loading image from " << filename << "..." << endl;
        this_thread::sleep_for(chrono::seconds(2));
    }
    void draw() const override {
        cout << "Drawing image: " << filename << endl;
    }
};

class ImageProxy : public IImage {
    string filename;
    mutable unique_ptr<RealImage> real_image;
public:
    ImageProxy(const string& filename) : filename(filename) {}

    ImageProxy(const ImageProxy& other) : filename(other.filename) {
    }

    void draw() const override {
        if (!real_image) {
            real_image = make_unique<RealImage>(filename);
        }
        real_image->draw();
    }
};