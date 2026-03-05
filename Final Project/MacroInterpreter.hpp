#pragma once
#include "Document.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include <sstream>
#include <vector>

using namespace std;

class TextSelector {
public:
    virtual ~TextSelector() = default;
    virtual string& select(Document& doc) = 0;
};

class MacroCommand {
protected:
    unique_ptr<TextSelector> selector;
public:
    MacroCommand(unique_ptr<TextSelector> s) : selector(move(s)) {}
    virtual ~MacroCommand() = default;

    virtual void interpret(Document& doc) = 0;
};

class SelectHeader : public TextSelector {
public:
    string& select(Document& doc) override { return doc.header; }
};

class SelectFooter : public TextSelector {
public:
    string& select(Document& doc) override { return doc.footer; }
};

class BoldCommand : public MacroCommand {
public:
    using MacroCommand::MacroCommand;

    void interpret(Document& doc) override {
        string& text = selector->select(doc);
        if (text.empty()) return;

        if (text.find("<b>") == string::npos) {
            text = "<b>" + text + "</b>";
            cout << "  [Macro] Applied BOLD to text.\n";
        }
        else {
            cout << "  [Macro] Text is already bold.\n";
        }
    }
};

class UpperCaseCommand : public MacroCommand {
public:
    using MacroCommand::MacroCommand;

    void interpret(Document& doc) override {
        string& text = selector->select(doc);
        // แปลงทุกตัวอักษรเป็นตัวใหญ่
        transform(text.begin(), text.end(), text.begin(), ::toupper);
        cout << "  [Macro] Converted text to UPPERCASE.\n";
    }
};

// คำสั่งรีเซ็ตค่า
class ResetCommand : public MacroCommand {
public:
    using MacroCommand::MacroCommand;

    void interpret(Document& doc) override {
        string& text = selector->select(doc);
        text = "Untitled";
        cout << "  [Macro] RESET text to default.\n";
    }
};

class MacroParser {
public:
    static std::unique_ptr<MacroCommand> parse(const std::string& commandLine) {
        stringstream ss(commandLine);
        string effect, target;
        ss >> effect >> target;

        unique_ptr<TextSelector> selector = nullptr;
        if (target == "HEADER") selector = std::make_unique<SelectHeader>();
        else if (target == "FOOTER") selector = std::make_unique<SelectFooter>();
        else {
            std::cout << "  [Syntax Error] Unknown target: " << target << "\n";
            return nullptr;
        }

        if (effect == "BOLD") return std::make_unique<BoldCommand>(std::move(selector));
        else if (effect == "UPPER") return std::make_unique<UpperCaseCommand>(std::move(selector));
        else if (effect == "RESET") return std::make_unique<ResetCommand>(std::move(selector));
        else {
            std::cout << "  [Syntax Error] Unknown action: " << effect << "\n";
            return nullptr;
        }
    }
};