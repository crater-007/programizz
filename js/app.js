/**
 * Main Application Coordinator - Exact Programiz replication
 */
document.addEventListener('DOMContentLoaded', () => {
  // 1. Initialize CodeMirror Editor
  const textarea = document.getElementById('code-editor');
  CodeEditor.init(textarea);

  // 2. Initialize Runner
  CodeRunner.init();

  // 3. Setup Draggable Splitter
  initSplitter();

  // 4. Setup Sidebar Language Switcher
  initSidebarLanguageSelector();

  // 5. Setup 3-Dots Options Dropdown
  initMoreOptionsDropdown();

  // 6. Setup Extra Buttons (Visualize, Share, Install)
  initExtraButtons();
});

/**
 * Resizable Splitter between Editor and Output Terminal
 */
function initSplitter() {
  const splitter = document.getElementById('splitter');
  const workspace = document.getElementById('workspace');
  const editorPanel = document.getElementById('editor-panel');
  const outputPanel = document.getElementById('output-panel');

  if (!splitter || !workspace || !editorPanel || !outputPanel) return;

  let isDragging = false;

  splitter.addEventListener('mousedown', () => {
    isDragging = true;
    splitter.classList.add('dragging');
    document.body.style.cursor = 'col-resize';
    document.body.style.userSelect = 'none';
  });

  window.addEventListener('mousemove', (e) => {
    if (!isDragging) return;

    const workspaceRect = workspace.getBoundingClientRect();
    const offset = e.clientX - workspaceRect.left;
    const totalWidth = workspaceRect.width;
    const minWidth = 200;

    if (offset >= minWidth && (totalWidth - offset) >= minWidth) {
      const leftPercent = (offset / totalWidth) * 100;
      editorPanel.style.flex = `0 0 ${leftPercent}%`;
      outputPanel.style.flex = `0 0 ${100 - leftPercent}%`;
    }

    if (CodeEditor.instance) {
      CodeEditor.instance.refresh();
    }
  });

  window.addEventListener('mouseup', () => {
    if (isDragging) {
      isDragging = false;
      splitter.classList.remove('dragging');
      document.body.style.cursor = '';
      document.body.style.userSelect = '';
      if (CodeEditor.instance) {
        CodeEditor.instance.refresh();
      }
    }
  });
}

/**
 * Sidebar Language Navigation
 */
function initSidebarLanguageSelector() {
  const buttons = document.querySelectorAll('.lang-nav-item');
  buttons.forEach(btn => {
    btn.addEventListener('click', () => {
      const lang = btn.getAttribute('data-lang');
      CodeEditor.setLanguage(lang);
    });
  });
}

/**
 * 3-Dots More Options Dropdown Controller (Direct Paste & Exam Code List)
 */
function initMoreOptionsDropdown() {
  const moreBtn = document.getElementById('btn-more-options');
  const dropdown = document.getElementById('options-dropdown');
  const pasteBtn = document.getElementById('menu-paste-btn');
  const codeBtn = document.getElementById('menu-code-btn');
  const codeList = document.getElementById('exam-code-list');
  const copyAllBtn = document.getElementById('menu-copy-all-btn');
  const resetBtn = document.getElementById('menu-reset-btn');

  if (!moreBtn || !dropdown) return;

  const toggleDropdown = (e) => {
    e.stopPropagation();
    const isHidden = dropdown.classList.contains('hidden');
    dropdown.classList.toggle('hidden', !isHidden);
    moreBtn.classList.toggle('active', isHidden);
  };

  moreBtn.addEventListener('click', toggleDropdown);

  // Close dropdown when clicking outside
  document.addEventListener('click', (e) => {
    if (!dropdown.contains(e.target) && e.target !== moreBtn && !moreBtn.contains(e.target)) {
      dropdown.classList.add('hidden');
      moreBtn.classList.remove('active');
    }
  });

  // Close dropdown on Escape
  document.addEventListener('keydown', (e) => {
    if (e.key === 'Escape') {
      dropdown.classList.add('hidden');
      moreBtn.classList.remove('active');
    }
  });

  // 1. Direct Paste from Clipboard with preserved indentation
  if (pasteBtn) {
    pasteBtn.addEventListener('click', async () => {
      try {
        const text = await navigator.clipboard.readText();
        if (text) {
          CodeEditor.setCode(text);
          showFeedback('Pasted from clipboard!');
        } else {
          showFeedback('Clipboard is empty', 'error');
        }
      } catch (err) {
        const manualText = prompt('Paste your code here:');
        if (manualText) {
          CodeEditor.setCode(manualText);
          showFeedback('Pasted code!');
        }
      }
      dropdown.classList.add('hidden');
      moreBtn.classList.remove('active');
    });
  }

  // 2. Populate "Code" (Exam files) List
  if (codeList && typeof ExamCodes !== 'undefined') {
    codeList.innerHTML = '';
    Object.keys(ExamCodes).forEach((key) => {
      const item = ExamCodes[key];
      const btn = document.createElement('button');
      btn.className = 'exam-code-item';
      btn.title = `Paste & Copy: ${item.filename}`;

      const ext = item.extension || '.cpp';
      let iconHtml = '<i class="fa-solid fa-code"></i>';
      if (ext === '.c') {
        iconHtml = '<span class="exam-icon-text">C</span>';
      } else if (ext === '.cpp') {
        iconHtml = '<span class="exam-icon-text">C++</span>';
      } else if (ext === '.py') {
        iconHtml = '<i class="fa-brands fa-python"></i>';
      } else if (ext === '.txt') {
        iconHtml = '<i class="fa-regular fa-file-lines"></i>';
      }

      const badgeClass = `badge-${ext.replace('.', '')}`;
      btn.innerHTML = `
        <div class="exam-code-item-left">
          ${iconHtml}
          <span class="exam-code-title">${item.name}</span>
        </div>
        <span class="exam-code-badge ${badgeClass}">${ext}</span>
      `;

      // Once selected: pastes in editor & copies to clipboard with zero popups
      btn.addEventListener('click', async (e) => {
        e.stopPropagation();

        // Switch to the file's language if supported
        if (item.language && item.language !== 'text' && CodeEditor.LANGUAGES[item.language]) {
          CodeEditor.setLanguage(item.language, true);
        }

        // Set filename in tab
        const tabName = document.getElementById('tab-filename');
        if (tabName && item.filename) {
          tabName.textContent = item.filename;
        }

        // 1. Paste into code editor with exact indentation
        CodeEditor.setCode(item.code);

        // 2. Copy to user's clipboard
        try {
          await navigator.clipboard.writeText(item.code);
          showFeedback(`Pasted & Copied: ${item.name}`);
        } catch (err) {
          showFeedback(`Loaded: ${item.name}`);
        }

        // Close dropdown
        dropdown.classList.add('hidden');
        moreBtn.classList.remove('active');
      });

      codeList.appendChild(btn);
    });
  }

  // 3. Expand/Collapse "Code" Sub-list
  if (codeBtn && codeList) {
    codeBtn.addEventListener('click', (e) => {
      e.stopPropagation();
      const isExpanded = !codeList.classList.contains('hidden');
      codeList.classList.toggle('hidden', isExpanded);
      codeBtn.classList.toggle('expanded', !isExpanded);
    });
  }

  // 4. Copy Editor Code
  if (copyAllBtn) {
    copyAllBtn.addEventListener('click', async () => {
      const code = CodeEditor.getCode();
      if (code) {
        try {
          await navigator.clipboard.writeText(code);
          showFeedback('Code copied to clipboard!');
        } catch (e) {
          showFeedback('Failed to copy', 'error');
        }
      }
      dropdown.classList.add('hidden');
      moreBtn.classList.remove('active');
    });
  }

  // 5. Reset Code
  if (resetBtn) {
    resetBtn.addEventListener('click', () => {
      CodeEditor.resetCurrentCode();
      showFeedback('Reset code');
      dropdown.classList.add('hidden');
      moreBtn.classList.remove('active');
    });
  }
}

/**
 * Visual Feedback Badge
 */
function showFeedback(text, type = 'success') {
  const badge = document.getElementById('action-feedback');
  const badgeText = document.getElementById('action-feedback-text');

  if (badge && badgeText) {
    badgeText.textContent = text;
    badge.style.display = 'inline-flex';
    badge.style.borderColor = type === 'error' ? 'rgba(248, 113, 113, 0.4)' : 'rgba(16, 185, 129, 0.4)';
    badge.style.color = type === 'error' ? '#f87171' : '#10b981';

    setTimeout(() => {
      badge.style.display = 'none';
    }, 2500);
  }
}

/**
 * Extra UI Buttons (Visualize, Share, Install)
 */
function initExtraButtons() {
  const visualizeBtn = document.getElementById('btn-visualize');
  if (visualizeBtn) {
    visualizeBtn.addEventListener('click', () => {
      showFeedback('Visualizer initialized');
      CodeRunner.clearOutput();
      CodeRunner.appendOutput('# Visualizer Mode Active\n', 'info');
      CodeRunner.appendOutput('Tracing execution step-by-step...\n\n', 'stdout');
      CodeRunner.runCode();
    });
  }

  const shareBtn = document.getElementById('btn-share');
  if (shareBtn) {
    shareBtn.addEventListener('click', async () => {
      const code = CodeEditor.getCode();
      if (code) {
        await navigator.clipboard.writeText(code).catch(() => {});
        showFeedback('Code copied to share!');
      }
    });
  }

  const installBtn = document.getElementById('btn-install');
  if (installBtn) {
    installBtn.addEventListener('click', () => {
      showFeedback('Compiler ready offline');
    });
  }
}
