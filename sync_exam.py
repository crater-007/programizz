#!/usr/bin/env python3
"""
Sync Exam Folder to js/exam_codes.js
Zero external dependencies.
"""
import os
import json
import re

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
EXAM_DIR = os.path.join(BASE_DIR, "exam")
OUTPUT_FILE = os.path.join(BASE_DIR, "js", "exam_codes.js")

LANG_EXT_MAP = {
    ".cpp": "cpp",
    ".c": "c",
    ".py": "python",
    ".txt": "text",
    ".java": "java",
    ".js": "javascript",
    ".cs": "csharp",
    ".go": "go",
    ".rs": "rust"
}

def clean_title(filename):
    name, ext = os.path.splitext(filename)
    # Handle theory files nicely
    is_theory = "theory" in name.lower()
    is_alt = bool(re.search(r'\(\s*\d+\s*\)', name))
    
    # Clean out (1), (2), etc.
    clean = re.sub(r'\(\s*\d+\s*\)', '', name)
    clean = clean.replace("_", " ").replace("-", " ")
    
    # Capitalize words & handle common acronyms
    words = []
    for w in clean.split():
        w_lower = w.lower()
        if w_lower == 'oops':
            words.append('OOPs')
        elif w_lower == 'baseclass':
            words.append('Base Class')
        elif not w.isupper():
            words.append(w.capitalize())
        else:
            words.append(w)
    title = " ".join(words)
    
    if is_theory:
        # Avoid duplicate "Theory"
        title = re.sub(r'(?i)\btheory\b', '', title).strip()
        title = f"{title} (Theory)" if title else "Theory"
        if is_alt:
            title += " (Alt)"
    elif is_alt:
        title += " (Alt)"
    return title

def main():
    if not os.path.exists(EXAM_DIR):
        print(f"Error: {EXAM_DIR} not found.")
        return

    codes = {}
    files = sorted(os.listdir(EXAM_DIR), key=lambda x: x.lower())

    for fname in files:
        fpath = os.path.join(EXAM_DIR, fname)
        if os.path.isfile(fpath):
            _, ext = os.path.splitext(fname)
            ext_lower = ext.lower()
            lang = LANG_EXT_MAP.get(ext_lower, "text")
            
            with open(fpath, "r", encoding="utf-8", errors="replace") as f:
                content = f.read()

            safe_key = re.sub(r'[^a-zA-Z0-9_]', '_', os.path.splitext(fname)[0]).lower()
            if safe_key in codes:
                safe_key += "_" + ext_lower.replace(".", "")
                
            codes[safe_key] = {
                "name": clean_title(fname),
                "filename": fname,
                "language": lang,
                "extension": ext_lower,
                "code": content
            }

    js_content = "/**\n * Auto-generated Exam Code Repository\n * Zero-backend operation.\n */\n"
    js_content += "const ExamCodes = " + json.dumps(codes, indent=2) + ";\n"

    with open(OUTPUT_FILE, "w", encoding="utf-8") as f:
        f.write(js_content)

    print(f"Successfully synced {len(codes)} files from exam/ to {OUTPUT_FILE}")

if __name__ == "__main__":
    main()
