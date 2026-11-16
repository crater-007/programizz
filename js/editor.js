/**
 * Code Editor Manager - Exact Programiz replication with 13 languages
 */
const CodeEditor = {
  instance: null,
  currentLanguage: 'python',

  LANGUAGES: {
    python: {
      name: 'Python',
      title: 'Python Online Compiler',
      filename: 'main.py',
      cmMode: 'python',
      defaultCode: `# Online Python compiler (interpreter)
# Write and run Python online using this editor.

print("Also try Visualize to understand code.")
`
    },
    r: {
      name: 'R',
      title: 'R Online Compiler',
      filename: 'main.r',
      cmMode: 'r',
      defaultCode: `# Online R compiler (interpreter)
# Write and run R code online using this editor.

print("Hello, World!")
`
    },
    sql: {
      name: 'SQL',
      title: 'SQL Online Editor',
      filename: 'main.sql',
      cmMode: 'sql',
      defaultCode: `-- Online SQL Editor
CREATE TABLE Users (
    id INT PRIMARY KEY,
    name VARCHAR(50)
);

INSERT INTO Users VALUES (1, 'Alice'), (2, 'Bob');

SELECT * FROM Users;
`
    },
    html: {
      name: 'HTML',
      title: 'HTML Online Editor',
      filename: 'index.html',
      cmMode: 'htmlmixed',
      defaultCode: `<!DOCTYPE html>
<html>
<head>
    <title>Programiz HTML</title>
</head>
<body>
    <h1>Welcome to Programiz</h1>
    <p>Write HTML, CSS, and JS here.</p>
</body>
</html>
`
    },
    java: {
      name: 'Java',
      title: 'Java Online Compiler',
      filename: 'Main.java',
      cmMode: 'text/x-java',
      defaultCode: `// Online Java Compiler
class Main {
    public static void main(String[] args) {
        System.out.println("Hello, World!");
    }
}
`
    },
    kotlin: {
      name: 'Kotlin',
      title: 'Kotlin Online Compiler',
      filename: 'main.kt',
      cmMode: 'text/x-kotlin',
      defaultCode: `// Online Kotlin Compiler
fun main() {
    println("Hello, World!")
}
`
    },
    c: {
      name: 'C',
      title: 'C Online Compiler',
      filename: 'main.c',
      cmMode: 'text/x-csrc',
      defaultCode: `// Online C compiler
#include <stdio.h>

int main() {
    printf("Hello, World!\\n");
    return 0;
}
`
    },
    cpp: {
      name: 'C++',
      title: 'C++ Online Compiler',
      filename: 'main.cpp',
      cmMode: 'text/x-c++src',
      defaultCode: `// Online C++ compiler
#include <iostream>
using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    return 0;
}
`
    },
    csharp: {
      name: 'C#',
      title: 'C# Online Compiler',
      filename: 'main.cs',
      cmMode: 'text/x-csharp',
      defaultCode: `// Online C# compiler
using System;

class Program {
    static void Main() {
        Console.WriteLine("Hello, World!");
    }
}
`
    },
    javascript: {
      name: 'JavaScript',
      title: 'JavaScript Online Compiler',
      filename: 'main.js',
      cmMode: 'javascript',
      defaultCode: `// Online JavaScript Compiler
console.log("Hello, World!");
`
    },
    typescript: {
      name: 'TypeScript',
      title: 'TypeScript Online Compiler',
      filename: 'main.ts',
      cmMode: 'javascript',
      defaultCode: `// Online TypeScript Compiler
const message: string = "Hello, World!";
console.log(message);
`
    },
    go: {
      name: 'Go',
      title: 'Go Online Compiler',
      filename: 'main.go',
      cmMode: 'text/x-go',
      defaultCode: `// Online Go Compiler
package main

import "fmt"

func main() {
    fmt.Println("Hello, World!")
}
`
    },
    rust: {
      name: 'Rust',
      title: 'Rust Online Compiler',
      filename: 'main.rs',
      cmMode: 'text/x-rustsrc',
      defaultCode: `// Online Rust Compiler
fn main() {
    println!("Hello, World!");
}
`
    }
  },

  init(textareaElement) {
    this.instance = CodeMirror.fromTextArea(textareaElement, {
      lineNumbers: true,
      mode: 'python',
      styleActiveLine: true,
      matchBrackets: true,
      autoCloseBrackets: true,
      indentUnit: 4,
      tabSize: 4,
      indentWithTabs: false,
      lineWrapping: true,
      extraKeys: {
        "Tab": function(cm) {
          if (cm.somethingSelected()) {
            cm.indentSelection("add");
          } else {
            cm.replaceSelection("    ", "end");
          }
        }
      }
    });

    // Auto-save code on change
    this.instance.on('change', () => {
      AppStorage.saveCode(this.currentLanguage, this.instance.getValue());
    });

    // Load initial language (default Python)
    const savedLang = AppStorage.getActiveLanguage();
    this.setLanguage(savedLang, false);
  },

  setLanguage(langKey, forceReset = false) {
    if (!this.LANGUAGES[langKey]) langKey = 'python';
    this.currentLanguage = langKey;
    AppStorage.saveActiveLanguage(langKey);

    const langConfig = this.LANGUAGES[langKey];

    // Update CodeMirror Mode
    this.instance.setOption('mode', langConfig.cmMode);

    // Load saved or default code
    const saved = AppStorage.getSavedCode(langKey);
    if (!forceReset && saved !== null && saved !== undefined) {
      this.instance.setValue(saved);
    } else {
      this.instance.setValue(langConfig.defaultCode);
    }

    // Update UI elements to match screenshot
    const tabName = document.getElementById('tab-filename');
    const topTitle = document.getElementById('top-compiler-title');

    if (tabName) tabName.textContent = langConfig.filename;
    if (topTitle) topTitle.textContent = langConfig.title;

    // Update active icon on sidebar
    document.querySelectorAll('.lang-nav-item').forEach(btn => {
      if (btn.getAttribute('data-lang') === langKey) {
        btn.classList.add('active');
      } else {
        btn.classList.remove('active');
      }
    });

    this.instance.clearHistory();
    this.instance.focus();
  },

  getCode() {
    return this.instance ? this.instance.getValue() : '';
  },

  setCode(code) {
    if (this.instance) {
      this.instance.setValue(code);
      this.instance.focus();
    }
  },

  resetCurrentCode() {
    const langConfig = this.LANGUAGES[this.currentLanguage];
    if (langConfig) {
      this.instance.setValue(langConfig.defaultCode);
    }
  }
};
