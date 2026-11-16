# Programiz-Style Online Compiler with Instant Code Selector

An ultra-lightweight online code editor and compiler designed to replicate the clean aesthetic of **Programiz Online Compiler** with an inline **Direct Paste** option and **Exam Code Selector** directly integrated into the 3-dots icon.

Built with a **100% Zero-Backend Architecture** (pure HTML, CSS, and JS) ready for immediate deployment on **GitHub Pages**.

---

## ✨ Key Features

- **Programiz Visual Aesthetic**:
  - Full-length 72px dark header with Programiz logo, leader advertisement banner, and Install / PRO buttons.
  - Left icon sidebar matching Programiz with all 13 languages: **Python, R, SQL, HTML, Java, Kotlin, C, C++, C#, JS, TS, Go, Rust**.
  - Proportional 42px tab bar with `main.py`, `Visualize` button, and `Run ▶` button.
  - Resizable dual-pane workspace: Code editor on the left, terminal console on the right.
  - Custom dark scrollbars and One Dark syntax highlighting matching Programiz.
  - Interactive terminal output with stdout/stderr formatting and interactive STDIN.

- **3-Dots Menu (Inline Dropdown - No External Modals)**:
  - Clicking the **3-Dots button ⋮** in the Output header opens an inline menu right underneath (no popups or external windows).
  - **Paste from Clipboard**: Directly pastes code from your clipboard into the editor while preserving exact indentation.
  - **"Code" Option (Exam Files)**:
    - Displays an inline list of all 29 files from the `exam/` folder (C, C++, and OS Theory notes):
      1. Demand Paging (`.cpp` & `.txt`)
      2. Dining Philosophers (`.cpp`)
      3. FCFS Scheduling (`.cpp` & `.txt`)
      4. Indexed Allocation (`.c`)
      5. Memory Allocation (`.cpp` & `.txt`)
      6. System Calls: mycp, myls, mystat (`.c`)
      7. Paging Architecture (`.cpp` & `.txt`)
      8. Peterson Solution, Race Condition, Strict Alternation (`.cpp`)
      9. Producer Consumer (`.cpp`)
      10. SJF Scheduling (`.cpp` & `.txt`)
      11. Task 1 & Task 2 (`.cpp` & `.txt`)
      12. Thread Synchronization (`.txt`)
      13. Two Level Paging (`.cpp` & `.txt`)
      14. Operating Systems Complete Theory (`.txt`)
    - Clicking any item:
      - **Immediately pastes** the code into the editor with 100% accurate indentation.
      - **Automatically copies** the code to your system clipboard.
      - Automatically switches the editor language mode (`C`, `C++`, or `Text`) and updates the active tab name.
      - Seamlessly closes the menu without disrupting your workspace.
  - **Copy Editor Code**: One-click to copy whatever is in the editor.
  - **Reset Code**: Quick reset to default starter.

- **Zero-Backend & 100% Client-Side**:
  - Pure static front-end. No server or backend needed.
  - Works offline or by double-clicking `index.html`.
  - Multi-language execution powered by the public, free Piston API (or browser-native JS).

---

## 🌐 How to Host on GitHub Pages (Step-by-Step)

1. **Initialize Git & Push to GitHub**:
   ```bash
   git init
   git add .
   git commit -m "Initial commit - Programiz Online Compiler"
   git branch -M main
   git remote add origin https://github.com/<your-username>/<your-repo-name>.git
   git push -u origin main
   ```

2. **Enable GitHub Pages**:
   - Go to your repository on **GitHub.com**.
   - Click on **Settings** > **Pages** (in the left sidebar).
   - Under **Build and deployment** > **Branch**:
     - Select branch: `main`
     - Select folder: `/ (root)`
   - Click **Save**.

3. **Done!**
   Your compiler will be live at:
   `https://<your-username>.github.io/<your-repo-name>/`

---

## 📁 Repository Structure

```text
code editor/
├── index.html        # Main static site for GitHub Pages
├── favicon.png       # Programiz P favicon
├── assets/
│   ├── ad_banner.png # Centered header advertisement banner
│   ├── favicon.png   # Apple touch icon
│   ├── programiz_logo.png # Header Programiz logo
│   └── sidebar/      # 13 language icons (Python, R, SQL, Java, C, etc.)
├── css/
│   └── style.css     # Complete styling (42px tab bar, dark scrollbars, One Dark syntax)
├── js/
│   ├── app.js        # Main UI coordinator, splitter, 3-dots dropdown
│   ├── editor.js     # CodeMirror setup with 13 languages
│   ├── exam_codes.js # Embedded exam code algorithms
│   ├── runner.js     # Code execution via free Piston API
│   └── storage.js    # LocalStorage manager
├── exam/             # C, C++, and OS Theory exam files
├── sync_exam.py      # Automated utility script to sync files from exam/ to JS
└── README.md         # Documentation
```
