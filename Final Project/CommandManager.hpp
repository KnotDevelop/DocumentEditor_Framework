#pragma once
#include "Document.hpp"
#include <vector>

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class AddElementCommand : public ICommand {
private:
    Document& doc;
    std::unique_ptr<DocumentElement> elementToAdd;
    std::unique_ptr<DocumentMemento> backupBefore; // เก็บสถานะก่อนทำ
    std::unique_ptr<DocumentMemento> backupAfter;  // เก็บสถานะหลังทำ (เอาไว้ทำ Redo)

public:
    AddElementCommand(Document& d, std::unique_ptr<DocumentElement> el)
        : doc(d), elementToAdd(std::move(el)) {
    }

    void execute() override {
        if (!backupAfter) {
            // รันครั้งแรก (แอดข้อมูลจริงๆ และสร้าง Backup หัวท้าย)
            backupBefore = doc.saveState();
            doc.addElement(std::move(elementToAdd));
            backupAfter = doc.saveState();
        }
        else {
            // กรณีเป็นการกด Redo (แค่โหลดสถานะหลังทำกลับมาทับ)
            doc.restoreState(*backupAfter);
        }
        std::cout << "[Command] Execute: Element added.\n";
    }

    void undo() override {
        if (backupBefore) {
            // โหลดสถานะก่อนหน้ากลับมาทับ
            doc.restoreState(*backupBefore);
            std::cout << "[Command] Undo: Element removed.\n";
        }
    }
};


class CommandManager {
private:
    std::vector<std::unique_ptr<ICommand>> undoStack;
    std::vector<std::unique_ptr<ICommand>> redoStack;

public:
    // สั่งรันคำสั่งใหม่
    void executeCommand(std::unique_ptr<ICommand> cmd) {
        cmd->execute();
        undoStack.push_back(std::move(cmd));
        redoStack.clear(); // ถ้าทำคำสั่งใหม่ ต้องลบประวัติ Redo ทิ้ง
    }

    void undo() {
        if (!undoStack.empty()) {
            auto cmd = std::move(undoStack.back());
            undoStack.pop_back();

            cmd->undo(); // สั่งให้คำสั่งทำการย้อนเวลาตัวเอง
            redoStack.push_back(std::move(cmd)); // ย้ายไปเก็บใน Redo เผื่อเปลี่ยนใจ
        }
        else {
            std::cout << "[Manager] Nothing to Undo.\n";
        }
    }

    void redo() {
        if (!redoStack.empty()) {
            auto cmd = std::move(redoStack.back());
            redoStack.pop_back();

            cmd->execute(); // สั่งให้คำสั่งทำงานซ้ำ
            undoStack.push_back(std::move(cmd)); // ย้ายกลับไปเก็บใน Undo
        }
        else {
            std::cout << "[Manager] Nothing to Redo.\n";
        }
    }
};