#pragma once
#include "Mediator.hpp"

class Button : public Component {
private:
    std::string name;
    bool isEnabled = false; // สถานะปุ่ม (กดได้/ไม่ได้)

public:
    Button(const std::string& n) : name(n) {}

    void setEnabled(bool enabled) {
        isEnabled = enabled;
        std::cout << "  [UI] Button '" << name << "' is now " << (isEnabled ? "ENABLED" : "DISABLED") << ".\n";
    }

    void click() {
        if (isEnabled) {
            std::cout << "  [User] Clicked button: " << name << "\n";
            if (mediator) mediator->notify(this, "click");
        }
        else {
            std::cout << "  [User] Clicked button: " << name << " (But it's disabled!)\n";
        }
    }
};

class Checkbox : public Component {
private:
    bool isChecked = false;

public:
    bool checkState() const { return isChecked; }

    void toggle() {
        isChecked = !isChecked;
        std::cout << "  [User] Toggled 'Edit Mode' to: " << (isChecked ? "ON" : "OFF") << "\n";
        if (mediator) mediator->notify(this, "check");
    }
};

class EditorUIMediator : public IMediator {
private:
    Button* saveButton;
    Button* loadButton;
    Checkbox* editModeBox;

public:
    // ลงทะเบียน Component
    void registerComponents(Button* save, Button* load, Checkbox* mode) {
        saveButton = save;
        loadButton = load;
        editModeBox = mode;

        // ผูก Component กลับมาหาคนกลาง
        saveButton->setMediator(this);
        loadButton->setMediator(this);
        editModeBox->setMediator(this);
    }

    void notify(Component* sender, const std::string& event) override {

        // 1. ถ้ามีการติ๊กเปลี่ยนโหมด (Edit Mode)
        if (sender == editModeBox && event == "check") {
            if (editModeBox->checkState()) {
                // ถ้าเปิด Edit Mode -> ให้ปุ่ม Save กดได้
                std::cout << "  (Mediator: Unlock Save Button)\n";
                saveButton->setEnabled(true);
            }
            else {
                // ถ้าปิด Edit Mode -> ล็อคปุ่ม Save
                std::cout << "  (Mediator: Lock Save Button)\n";
                saveButton->setEnabled(false);
            }
        }

        // 2. ถ้ากดปุ่ม Save
        if (sender == saveButton && event == "click") {
            std::cout << "  (Mediator: Saving Logic Triggered...)\n";
            std::cout << "  SYSTEM: Document Saved Successfully!\n";

            // เซฟเสร็จแล้ว ปิด Edit Mode อัตโนมัติ
            editModeBox->toggle();
        }

        // 3. ถ้ากดปุ่ม Load
        if (sender == loadButton && event == "click") {
            std::cout << "  (Mediator: Loading Logic Triggered...)\n";
            std::cout << "  SYSTEM: Document Loaded.\n";
        }
    }
};