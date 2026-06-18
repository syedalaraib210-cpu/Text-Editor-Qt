# 📝 Text Editor — Qt C++ Application


A fully functional **rich text editor** built in C++ using the Qt framework,
implementing complete file management, text formatting, and editing features
with a clean GUI.

---

## 👩‍💻 Author
**Syeda Laraib** — [@Syedalaraib210](https://github.com/Syedalaraib210)
> PUCIT Lahore — Data Structures & Algorithms Assignment 2

---

## ✨ Features

### 📁 File Management
- **New** — create a new file with unsaved changes prompt
- **Open** — open .txt and .html files
- **Recent Files** — tracks and displays last 5 opened files
- **Save** — save current file
- **Save As** — save with a new name/location
- **Close** — close current file safely
- **Exit** — exit with unsaved changes check

### ✏️ Text Editing
- Type, delete, backspace, insert
- **Undo / Redo** support
- Cut, Copy, Paste
- Select All
- **Find & Replace** — finds all occurrences and replaces them
- Enter key creates new paragraph

### 🎨 Text Formatting
- **Bold, Italic, Underline** — via menu and toolbar
- **Font selection** — full font picker from system fonts
- **Font size** — sizes from 8 to 72pt
- **Text color** — full color picker dialog
- **Word Wrap** — toggle on/off

### 🖥️ UI & Navigation
- Menu bar with File, Edit, Format, View menus
- Formatting toolbar with font, size, bold, italic, underline, color
- Status bar showing live **Line & Column** numbers
- **Zoom In / Zoom Out**
- **Scroll bars** — vertical always visible, horizontal as needed
- PgUp, PgDn, Home, End keys fully functional
- Unsaved changes indicator (\*) in title bar

---

## 🛠️ Tech Stack

| Technology | Usage |
|------------|-------|
| **C++** | Core programming language |
| **Qt Framework** | GUI framework |
| **QMainWindow** | Main application window |
| **QTextEdit** | Rich text editing widget |
| **Qt Signals & Slots** | Event-driven programming |

---

## 📂 File Structure
Text-Editor-Qt/

├── main.cpp          # Application entry point

├── texteditor.h      # Main editor class (UI + logic)

└── TextEditor.pro    # Qt project configuration file

## 🚀 How to Run

### Requirements
- Qt 5.x or Qt 6.x installed
- C++ compiler (MinGW / MSVC / GCC)

### Steps
1. Clone the repository:
git clone https://github.com/Syedalaraib210/Text-Editor-Qt.git
2. Open **TextEditor.pro** in **Qt Creator**
3. Click **Build** → **Run** (Ctrl+R)

---


## 🎓 Academic Context
Built as **Assignment 2** for the Data Structures & Algorithms course at
PUCIT Lahore. Demonstrates practical use of Qt GUI programming, object-oriented
design, event-driven architecture, and C++ class implementation.
