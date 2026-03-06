เพื่อความง่ายในการอ่าน สามารถอ่านผ่านลิงค์นี้ได้
https://docs.google.com/document/d/1EjbP1ewLxFLZJ8qjlexb_G1hSWh6eTqcm2ZoHeI4vGM/edit?usp=sharing

Document Editor Framework 
(C++ Design Patterns Project)
รายละเอียดโครงการ
โปรเจกต์นี้คือ Framework สำหรับแก้ไขเอกสาร (Document Editor) ที่พัฒนาด้วยภาษา C++ โดยเน้นการประยุกต์ใช้ Design Patterns ทั้ง 3 หมวดหมู่ (Creational, Structural, Behavioral) เพื่อแก้ปัญหาทางสถาปัตยกรรมซอฟต์แวร์ต่างๆ เช่น การจัดการหน่วยความจำ, การแยกส่วนติดต่อผู้ใช้ (UI), การบันทึกสถานะ (Undo/Redo), และการขยายความสามารถในอนาคต

การออกแบบ (Design Patterns Implementation)
1. หมวด Creational Patterns (การสร้างอ็อบเจกต์)
Singleton: ใช้ในคลาส Settings เพื่อเก็บค่า Config กลางของโปรแกรม (เช่น ความละเอียดหน้าจอ) ให้มีเพียง instance เดียวตลอดการทำงาน
Builder: ใช้ DocumentBuilder เพื่อแยกขั้นตอนการสร้าง Document ที่ซับซ้อน (กำหนด Header, Footer, ขนาดกระดาษ) ออกจากตัวคลาสหลัก ทำให้โค้ดอ่านง่ายแบบ Fluent API
Factory Method: ใช้ DocumentElementFactory เพื่อซ่อน logic การสร้างวัตถุ (Paragraph, Image, Table) ทำให้ client ไม่ต้องใช้ new โดยตรง
Prototype: ใช้ IClonable ในคลาส Document และ DocumentElement ช่วยให้สามารถคัดลอก (Clone) วัตถุได้ทันทีโดยไม่ต้องสร้างใหม่จากศูนย์ (Deep Copy)
2. หมวด Structural Patterns (โครงสร้าง)
Composite: ใช้ในระบบไฟล์ (Folder, File) และโครงสร้างเอกสาร เพื่อให้สามารถจัดการ "กลุ่มของวัตถุ" (Folder) ได้เหมือนกับ "วัตถุเดี่ยว" (File) ผ่าน Interface เดียวกัน (IFileSystemComponent)
Decorator: ใช้ BoldDecorator, ItalicDecorator ห่อหุ้มข้อความ (PlainText) เพื่อเพิ่มคุณสมบัติ (เช่น ตัวหนา, ตัวเอียง) ได้แบบซ้อนทับกัน (Recursive) โดยไม่ต้องแก้คลาสเดิม
Flyweight: ใช้ StyleFactory เก็บอ็อบเจกต์ CharacterStyle (Font, Size, Color) ที่ซ้ำกันไว้ใน Pool เพื่อลดการใช้ RAM เมื่อมีข้อความจำนวนมากใช้สไตล์เดียวกัน
Bridge: แยกส่วน DocumentElement (Abstraction) ออกจาก IRenderer (Implementation) ทำให้สามารถสลับการแสดงผลระหว่าง ConsoleRenderer และ HTMLRenderer ได้โดยไม่ต้องแก้โค้ดฝั่ง Element
Facade: ใช้ FileManagerFacade รวบรวมระบบย่อยที่ซับซ้อน (Serializer, FileHandler) ให้เหลือเพียงคำสั่งง่ายๆ คือ save() และ load()
Adapter: ใช้ ShapeAdapter แปลง Interface ของ LegacyShapeDrawer (ไลบรารีเก่า) ให้สามารถทำงานร่วมกับระบบใหม่ (DocumentElement) ได้
3. หมวด Behavioral Patterns (พฤติกรรม)
Command & Memento: ใช้ AddElementCommand ห่อหุ้มคำสั่งแก้ไขเอกสาร และใช้ DocumentMemento (Snapshot) เก็บสถานะก่อน/หลังทำ เพื่อรองรับระบบ Undo/Redo ที่สมบูรณ์
Observer: ใช้ StatusBar คอยเฝ้าดู Document เมื่อมีการเปลี่ยนแปลง (เพิ่ม/ลบ Element) จะทำการอัปเดตสถานะบนหน้าจอทันที (Push Notification)
State: ใช้จัดการสถานะเอกสาร (Draft, Review, Published) เพื่อควบคุมสิทธิ์การแก้ไข (เช่น ห้ามแก้เมื่อ Published) โดยไม่ต้องเขียน if-else จำนวนมาก
Strategy: ใช้ IExportStrategy เพื่อให้ผู้ใช้เลือกอัลกอริทึมการ Export (PDF หรือ Markdown) ได้อิสระขณะรันไทม์
Iterator: ใช้ DocumentIterator เพื่อวนลูปเข้าถึง Element ภายในเอกสารโดยไม่ต้องเปิดเผยโครงสร้างข้อมูลภายใน (std::vector)
Visitor: ใช้ WordCountVisitor และ XMLExportVisitor เพื่อแยก Logic การประมวลผล (นับคำ, แปลง XML) ออกจากคลาสข้อมูล ทำให้เพิ่มฟีเจอร์ใหม่ได้โดยไม่ต้องแก้คลาสเดิม
Chain of Responsibility: ใช้จัดการ Event (เช่น click, save) โดยส่งต่อจาก Element ลูก ขึ้นไปหาแม่ (Document) จนกว่าจะมีคนจัดการ
Mediator: ใช้ EditorUIMediator เป็นคนกลางคุมการทำงานระหว่าง UI Components (Button, Checkbox) ลดการผูกมัด (Coupling) ระหว่างปุ่มต่างๆ
Template Method: ใช้ DocumentValidator กำหนดโครงสร้างลำดับการตรวจสอบเอกสาร (Header -> Body) แต่ปล่อยให้ Subclass (SecurityValidator, FormatValidator) กำหนดวิธีการตรวจเอง
Interpreter: ใช้ MacroInterpreter แปลงคำสั่งภาษาอังกฤษ (เช่น "BOLD HEADER") ให้กลายเป็นการกระทำในโค้ด (Macro System)
วิธีคอมไพล์และรัน (How to Compile & Run)
Requirements
C++ Compiler ที่รองรับ C++14 ขึ้นไป (เช่น g++, clang, MSVC)
Standard Template Library (STL)
ขั้นตอน (Visual Studio)
สร้างโปรเจกต์ใหม่แบบ Console App.
นำไฟล์ Header (.hpp) ทั้งหมด และ main.cpp ใส่ในโปรเจกต์.
กด Build และ Run (F5)


Class Diagram
A <|-- B (Inheritance / Realization)
A *-- B (Composition)
A o-- B (Aggregation)
A --> B (Association / Dependency)

    Document <|-- IClonable
    Document <|-- IObservable
    Document <|-- IIterable
    Document <|-- IEventHandler
    
    Document *-- DocumentElement
    DocumentElement <|-- Paragraph
    DocumentElement <|-- Image
    DocumentElement <|-- Table
    DocumentElement <|-- ShapeAdapter
    
    ShapeAdapter --> LegacyShapeDrawer : Adapts
    
    DocumentElement --> IRenderer : Uses (Bridge)
    IRenderer <|-- ConsoleRenderer
    IRenderer <|-- HTMLRenderer
    
    CommandManager o-- ICommand
    ICommand -->  Document : Executes on
    ICommand -->  DocumentMemento : Saves state
    
    Document -->  IDocumentState : Has State
    IDocumentState <|-- DraftState
    IDocumentState <|-- ReviewState
    
    DocumentExporter -->  IExportStrategy : Uses
    IExportStrategy <|-- PDFExportStrategy
    
    DocumentValidator <|-- FormatValidator : Template Method
    
    IMediator <|-- EditorUIMediator
    EditorUIMediator -->  Button
    EditorUIMediator -->  Checkbox
