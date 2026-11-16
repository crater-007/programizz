/**
 * Code Execution Runner - Zero-Compute Runner
 * Uses the free public Piston API for execution and in-browser execution for JS.
 */
const CodeRunner = {
  isRunning: false,

  PISTON_MAP: {
    python: { lang: 'python', version: '3.10.0', file: 'main.py' },
    javascript: { lang: 'javascript', version: '18.15.0', file: 'main.js' },
    c: { lang: 'c', version: '10.2.0', file: 'main.c' },
    cpp: { lang: 'cpp', version: '10.2.0', file: 'main.cpp' },
    java: { lang: 'java', version: '15.0.2', file: 'Main.java' },
    csharp: { lang: 'csharp', version: '6.12.0', file: 'main.cs' },
    r: { lang: 'r', version: '4.1.1', file: 'main.r' },
    sql: { lang: 'sqlite3', version: '3.36.0', file: 'main.sql' },
    kotlin: { lang: 'kotlin', version: '1.8.20', file: 'main.kt' },
    html: { lang: 'html', version: '5', file: 'index.html' },
    typescript: { lang: 'typescript', version: '4.2.3', file: 'main.ts' },
    go: { lang: 'go', version: '1.16.2', file: 'main.go' },
    rust: { lang: 'rust', version: '1.68.2', file: 'main.rs' }
  },

  init() {
    this.bindEvents();
  },

  bindEvents() {
    // Run button in editor header
    const runBtn = document.getElementById('run-btn');
    if (runBtn) {
      runBtn.addEventListener('click', () => this.runCode());
    }

    // Keyboard shortcut to run (Ctrl+Enter or Cmd+Enter)
    document.addEventListener('keydown', (e) => {
      if ((e.ctrlKey || e.metaKey) && e.key === 'Enter') {
        e.preventDefault();
        this.runCode();
      }
    });

    // STDIN Submit
    const stdinBtn = document.getElementById('stdin-submit-btn');
    const stdinInput = document.getElementById('stdin-input');
    if (stdinBtn && stdinInput) {
      const handleStdin = () => {
        const val = stdinInput.value;
        stdinInput.value = '';
        this.appendOutput(val + '\n', 'stdout');
        this.hideStdinInput();
      };
      stdinBtn.addEventListener('click', handleStdin);
      stdinInput.addEventListener('keydown', (e) => {
        if (e.key === 'Enter') {
          e.preventDefault();
          handleStdin();
        }
      });
    }
  },

  async runCode() {
    if (this.isRunning) return;

    const code = CodeEditor.getCode();
    const langKey = CodeEditor.currentLanguage;
    const config = this.PISTON_MAP[langKey] || { lang: langKey, version: '*', file: 'main.txt' };

    if (!code.trim()) {
      this.setOutput('Please enter some code before running.\n', 'stderr');
      return;
    }

    this.setRunningState(true);
    this.clearOutput();

    try {
      if (langKey === 'javascript') {
        await this.runBrowserJs(code);
      } else {
        await this.runPiston(config, code);
      }
    } catch (err) {
      this.appendOutput(`\nExecution Error: ${err.message || 'Server unreachable'}\n`, 'stderr');
    } finally {
      this.setRunningState(false);
    }
  },

  async runPiston(config, code) {
    const url = 'https://emkc.org/api/v2/piston/execute';

    const payload = {
      language: config.lang,
      version: config.version,
      files: [
        {
          name: config.file,
          content: code
        }
      ]
    };

    const response = await fetch(url, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(payload)
    });

    if (!response.ok) {
      throw new Error(`Execution service status: ${response.status}`);
    }

    const data = await response.json();

    if (data.run) {
      const stdout = data.run.stdout;
      const stderr = data.run.stderr;

      if (stdout) this.appendOutput(stdout, 'stdout');
      if (stderr) this.appendOutput(stderr, 'stderr');
      if (!stdout && !stderr) this.appendOutput('(Program exited with no output)\n', 'info');
    } else if (data.compile && data.compile.stderr) {
      this.appendOutput(data.compile.stderr, 'stderr');
    } else {
      this.appendOutput('Program executed successfully.\n', 'success');
    }
  },

  runBrowserJs(code) {
    return new Promise((resolve) => {
      const logs = [];
      const origLog = console.log;
      const origErr = console.error;

      console.log = (...args) => {
        logs.push({ text: args.map(a => typeof a === 'object' ? JSON.stringify(a) : String(a)).join(' ') + '\n', type: 'stdout' });
      };
      console.error = (...args) => {
        logs.push({ text: args.map(a => String(a)).join(' ') + '\n', type: 'stderr' });
      };

      try {
        const fn = new Function(code);
        fn();
      } catch (err) {
        logs.push({ text: `${err.name}: ${err.message}\n`, type: 'stderr' });
      } finally {
        console.log = origLog;
        console.error = origErr;
        logs.forEach(l => this.appendOutput(l.text, l.type));
        resolve();
      }
    });
  },

  setRunningState(running) {
    this.isRunning = running;
    const runBtn = document.getElementById('run-btn');
    if (runBtn) {
      const span = runBtn.querySelector('span');
      if (span) {
        span.textContent = running ? 'Running...' : 'Run';
      }
      runBtn.style.opacity = running ? '0.75' : '1';
      runBtn.style.pointerEvents = running ? 'none' : 'auto';
    }
  },

  clearOutput() {
    const outText = document.getElementById('output-text');
    if (outText) outText.innerHTML = '';
  },

  appendOutput(text, type = 'stdout') {
    const outText = document.getElementById('output-text');
    if (!outText) return;

    const span = document.createElement('span');
    span.className = type;
    span.textContent = text;
    outText.appendChild(span);

    const body = document.getElementById('output-body');
    if (body) body.scrollTop = body.scrollHeight;
  },

  setOutput(text, type = 'stdout') {
    this.clearOutput();
    this.appendOutput(text, type);
  },

  hideStdinInput() {
    const container = document.getElementById('stdin-container');
    if (container) container.classList.add('hidden');
  }
};
