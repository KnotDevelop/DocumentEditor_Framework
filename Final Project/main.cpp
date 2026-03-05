#include "Document.hpp"
#include "DocumentBuilder.hpp"
#include "FileComposite.hpp"
#include "ParagraphDecorator.hpp"
#include "IRenderer.hpp"
#include "FileManager.hpp"
#include "CommandManager.hpp" 
#include "StatusBar.hpp"
#include "ExportStrategy.hpp"
#include "Iterator.hpp" 
#include "WordCountVisitor.hpp"
#include "UIComponents.hpp"
#include "DocumentValidator.hpp"
#include "MacroInterpreter.hpp"

using namespace std;

int main()
{
    // ==========================================================
    // เฟสที่ 1: เตรียมระบบพื้นฐาน
    // ==========================================================
    ConsoleRenderer consoleRenderer;
    HTMLRenderer htmlRenderer;
    DocumentElementFactory elementFactory(consoleRenderer);

    StyleFactory factory;
    CharacterStyle* headerStyle = factory.get_style("Arial", 24, "Red");
    CharacterStyle* bodyStyle = factory.get_style("Times", 14, "Black");

    // ==========================================================
    // เฟสที่ 2: Undo/Redo System
    // ==========================================================
    std::cout << "\n========== [Phase 2: Undo/Redo System] ==========\n";
    Document myDoc;
    myDoc.header = "Testing Undo / Redo";
    CommandManager cmdManager;

    StatusBar statusBar;
    myDoc.addObserver(&statusBar);

    std::cout << "--- Initial State ---\n";
    myDoc.display();

    auto circle = elementFactory.makeShape("Circle", 50);
    cmdManager.executeCommand(std::make_unique<AddElementCommand>(myDoc, std::move(circle)));

    std::cout << "--- After Add (Circle created) ---\n";
    myDoc.display();

    std::cout << "--- Undo Pressed (Revert) ---\n";
    cmdManager.undo();
    myDoc.display();

    std::cout << "--- Redo Pressed (Reapply) ---\n";
    cmdManager.redo();
    myDoc.display();

    // ==========================================================
    // เฟสที่ 3: Document Generation
    // ==========================================================
    std::cout << "\n========== [Phase 3: Document Generation] ==========\n";
    Document doc1 = DocumentBuilder::create(htmlRenderer)
        .header("Original Document")
        .add_paragraph("This is easy paragraph", bodyStyle)
        .add_image("hero_avatar.png", "Main character img")
        .add_table(3, 2)
        .add_shape("Circle", 15)
        .add_shape("Rectangle", 20, 10)
        .build();

    auto myStar = elementFactory.makeShape("Star", 50);
    auto myHexagon = elementFactory.makeShape("Hexagon", 100, 20);
    doc1.addElement(std::move(myStar));
    doc1.addElement(std::move(myHexagon));

    auto clonedPtr = doc1.clone();
    Document* doc2 = static_cast<Document*>(clonedPtr.get());
    if (doc2) {
        doc2->header = "Clone Copy";
        auto newPara = make_unique<Paragraph>(consoleRenderer, "Doc 2 Custom Text", nullptr);
        newPara->setText("Modify text before assign");
        doc2->addElement(std::move(newPara));
    }

    std::cout << "\n>>> Showing Doc 1 <<<\n";
    doc1.display();

    if (doc2) {
        std::cout << "\n>>> Showing Doc 2 <<<\n";
        doc2->display();
    }

    // ==========================================================
    // เฟสที่ 4: Text Decorator
    // ==========================================================
    std::cout << "\n========== [Phase 4: Text Decorator] ==========\n";
    auto plain = make_unique<PlainText>("Hello World");
    auto boldText = make_unique<BoldDecorator>(std::move(plain));
    auto boldItalicText = make_unique<ItalicDecorator>(std::move(boldText));
    cout << "[Decorator Output]: " << boldItalicText->getText() << "\n";

    // ==========================================================
    // เฟสที่ 5: State Pattern (Workflow)
    // ==========================================================
    std::cout << "\n========== [Phase 5: State Pattern (Workflow)] ==========\n";
    Document workflowDoc;
    workflowDoc.header = "Top Secret Game Design Document";
    workflowDoc.footer = "Confidential 2024";

    std::cout << "--- 1. Current State: " << workflowDoc.state->getName() << " ---\n";
    workflowDoc.addElement(elementFactory.makeShape("Circle", 10));
    workflowDoc.requestReview();
    workflowDoc.approve();
    workflowDoc.reject(); // กลับมาเป็น Draft เพื่อให้แก้ได้
    workflowDoc.addElement(elementFactory.makeShape("Star", 30));

    std::cout << "\n>>> Final Workflow Document <<<\n";
    workflowDoc.display();

    // ตั้งเป็น Review เพื่อโชว์ใน JSON
    workflowDoc.requestReview();

    // ==========================================================
    // เฟสที่ 6: Save Individual Docs
    // ==========================================================
    std::cout << "\n========== [Phase 6: Save Individual Docs] ==========\n";
    FileManagerFacade fileManager;
    fileManager.save(doc1, "savegame_01.json");
    if (doc2) {
        fileManager.save(*doc2, "savegame_02.json");
    }

    // ==========================================================
    // เฟสที่ 7 : Strategy Pattern (Export)
    // ==========================================================
    std::cout << "\n========== [Phase 7: Strategy Pattern (Export System)] ==========\n";
    DocumentExporter exporter;
    exporter.setStrategy(std::make_unique<MarkdownExportStrategy>());
    exporter.exportDocument(doc1, "Exported_Document.md");

    exporter.setStrategy(std::make_unique<PDFExportStrategy>());
    exporter.exportDocument(doc1, "Exported_Document.pdf");

    // ==========================================================
    // เฟสที่ 8: Iterator Pattern
    // ==========================================================
    std::cout << "\n========== [Phase 8: Iterator Pattern (Traversing)] ==========\n";
    auto iterator = doc1.createIterator();
    int itemCount = 1;
    while (iterator->hasNext()) {
        DocumentElement* el = iterator->next();
        // Just iterating (display already shown in previous phases)
    }
    std::cout << "  Iterated through " << itemCount - 1 << " elements successfully.\n";

    // ==========================================================
    // เฟสที่ 9: Visitor Pattern
    // ==========================================================
    std::cout << "\n========== [Phase 9: Visitor Pattern (Operations)] ==========\n";
    WordCountVisitor wordCounter;
    doc1.accept(wordCounter);
    std::cout << "Total Word Count in doc1: " << wordCounter.getWordCount() << " words\n";

    XMLExportVisitor xmlExporter;
    doc1.accept(xmlExporter);
    // (แสดงผล XML เล็กน้อย)

    // ==========================================================
    // เฟสที่ 10: Chain of Responsibility
    // ==========================================================
    std::cout << "\n========== [Phase 10: Chain of Responsibility (Events)] ==========\n";
    auto eventIterator = doc1.createIterator();
    DocumentElement* pParagraph = nullptr;
    if (eventIterator->hasNext()) pParagraph = eventIterator->next();
    if (pParagraph) {
        std::cout << "--- Sending 'select' event ---\n";
        pParagraph->handleEvent("select");
    }

    // ==========================================================
    // 🌟 เฟสที่ 11 (มาใหม่!): Interpreter Pattern (Macro System)
    // ทดสอบการสั่งงานด้วยประโยคภาษาอังกฤษ
    // ==========================================================
    std::cout << "\n========== [Phase 11: Interpreter Pattern (Macro System)] ==========\n";

    // รายการคำสั่ง Macro ที่ผู้ใช้พิมพ์เข้ามา
    vector<string> macros = {
        "BOLD HEADER",    // ทำหัวกระดาษเป็นตัวหนา
        "UPPER FOOTER",   // ทำท้ายกระดาษเป็นตัวใหญ่
        "RESET HEADER",   // รีเซ็ตหัวกระดาษ
        "DELETE IMAGES"   // คำสั่งมั่วๆ (ทดสอบ Error)
    };

    for (const auto& cmdStr : macros) {
        std::cout << "User typed: \"" << cmdStr << "\"\n";

        // 1. แปลงข้อความ -> คำสั่ง (Parsing)
        auto command = MacroParser::parse(cmdStr);

        if (command) {
            // 2. ถ้าแปลรู้เรื่อง -> สั่งทำงานกับ doc1 (Interpreting)
            command->interpret(doc1);
        }
    }

    std::cout << "\n>>> Result after Macros <<<\n";
    std::cout << "Header: " << doc1.header << "\n";
    std::cout << "Footer: " << doc1.footer << "\n";

    // ==========================================================
    // เฟสที่ 12: Template Method (Validation)
    // ==========================================================
    std::cout << "\n========== [Phase 12: Template Method (Validation)] ==========\n";
    FormatValidator formatChecker;
    formatChecker.validate(doc1);

    SecurityValidator securityChecker;
    securityChecker.validate(workflowDoc);

    // ==========================================================
    // เฟสที่ 13: File System Structure
    // ==========================================================
    std::cout << "\n========== [Phase 13: File System Structure] ==========\n";
    auto root = std::make_unique<Folder>("Root");
    auto docsFolder = std::make_unique<Folder>("My Documents");
    auto gamesFolder = std::make_unique<Folder>("Games");

    docsFolder->add(std::make_unique<File>("Resume.pdf", std::move(doc1)));
    if (doc2) docsFolder->add(std::make_unique<File>("Budget.xlsx", std::move(*doc2)));
    gamesFolder->add(std::make_unique<File>("GameDesign.docx", std::move(workflowDoc)));

    root->add(std::move(docsFolder));
    root->add(std::move(gamesFolder));

    root->display();

    // ==========================================================
    // เฟสที่ 14: Save Full System
    // ==========================================================
    std::cout << "\n========== [Phase 14: Save Full Virtual File System] ==========\n";
    fileManager.saveFileSystem(*root, "full_filesystem_save.json");

    // ==========================================================
    // เฟสที่ 15: Mediator Pattern (UI)
    // ==========================================================
    std::cout << "\n========== [Phase 15: Mediator Pattern (UI Interaction)] ==========\n";
    EditorUIMediator uiMediator;
    Button btnSave("Save");
    Button btnLoad("Load");
    Checkbox chkEditMode;
    uiMediator.registerComponents(&btnSave, &btnLoad, &chkEditMode);

    chkEditMode.toggle();
    btnSave.click();

    return 0;
}