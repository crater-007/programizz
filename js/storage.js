/**
 * Storage Helper - Manages local persistence for zero-compute, zero-backend deployment.
 * Everything is stored directly in browser localStorage.
 */
const AppStorage = {
  KEYS: {
    LANGUAGE: 'programiz_active_lang',
    CODE_PREFIX: 'programiz_code_',
    THEME: 'programiz_theme',
    FONT_SIZE: 'programiz_font_size'
  },

  getActiveLanguage() {
    return localStorage.getItem(this.KEYS.LANGUAGE) || 'python';
  },

  saveActiveLanguage(lang) {
    localStorage.setItem(this.KEYS.LANGUAGE, lang);
  },

  getSavedCode(lang) {
    return localStorage.getItem(this.KEYS.CODE_PREFIX + lang);
  },

  saveCode(lang, code) {
    try {
      localStorage.setItem(this.KEYS.CODE_PREFIX + lang, code);
    } catch (e) {
      console.warn('Storage quota exceeded', e);
    }
  },

  getTheme() {
    return localStorage.getItem(this.KEYS.THEME) || 'dark';
  },

  saveTheme(theme) {
    localStorage.setItem(this.KEYS.THEME, theme);
  },

  getFontSize() {
    return parseInt(localStorage.getItem(this.KEYS.FONT_SIZE) || '14', 10);
  },

  saveFontSize(size) {
    localStorage.setItem(this.KEYS.FONT_SIZE, size.toString());
  }
};
