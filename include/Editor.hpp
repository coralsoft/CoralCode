#pragma once

#include "TextBuffer.hpp"
#include "Viewport.hpp"
#include "SyntaxHighlighter.hpp"
#include <cstddef>
#include <string>
#include <memory>

namespace CoralCode {
    
    // Forward declarations
    class Window;
    class UndoRedoManager;
    class ClipboardManager;
    
    /**
     * @brief Posición del cursor en el editor
     */
    struct CursorPosition {
        size_t line;
        size_t column;
        
        CursorPosition(size_t l = 0, size_t c = 0) : line(l), column(c) {}
        
        bool operator==(const CursorPosition& other) const {
            return line == other.line && column == other.column;
        }
        
        bool operator!=(const CursorPosition& other) const {
            return !(*this == other);
        }
    };
    
    /**
     * @brief Selección de texto en el editor
     */
    struct TextSelection {
        CursorPosition start;
        CursorPosition end;
        bool active;
        
        TextSelection() : active(false) {}
        TextSelection(const CursorPosition& s, const CursorPosition& e) 
            : start(s), end(e), active(true) {}
        
        bool hasSelection() const { return active && start != end; }
        void clear() { active = false; }
        
        std::pair<CursorPosition, CursorPosition> getOrderedBounds() const;
    };
    
    /**
     * @brief Clase principal del editor
     * 
     * Responsable de:
     * - Coordinar todos los subsistemas
     * - Gestionar el estado del editor
     * - Procesar eventos de entrada
     * - Orquestar la renderización
     */
    class Editor {
    public:
        Editor();
        ~Editor();
        
        // Inicialización
        bool initialize();
        void shutdown();
        
        // Loop principal
        void run();
        bool isRunning() const;
        void stop();
        
        // Gestión de archivos
        bool newFile();
        bool openFile(const std::string& filepath);
        bool saveFile();
        bool saveFileAs(const std::string& filepath);
        bool hasUnsavedChanges() const;
        std::string getCurrentFilePath() const;
        
        // Edición de texto
        void insertChar(char ch);
        void insertText(const std::string& text);
        void deleteChar();
        void deleteCharForward();
        void insertNewLine();
        void deleteSelection();
        
        // Navegación
        void moveCursor(int deltaLine, int deltaCol);
        void moveCursorToPosition(size_t line, size_t col);
        void moveCursorToLineStart();
        void moveCursorToLineEnd();
        void moveCursorToDocumentStart();
        void moveCursorToDocumentEnd();
        
        // Scroll
        void scrollVertical(int lines);
        void scrollHorizontal(int columns);
        void scrollToLine(size_t line);
        void ensureCursorVisible();
        
        // Selección
        void startSelection();
        void updateSelection();
        void selectAll();
        void clearSelection();
        std::string getSelectedText() const;
        
        // Clipboard
        void copy();
        void cut();
        void paste();
        
        // Undo/Redo
        void undo();
        void redo();
        
        // Búsqueda
        bool find(const std::string& text, bool caseSensitive = false, bool wholeWord = false);
        bool findNext();
        bool findPrevious();
        void replace(const std::string& oldText, const std::string& newText);
        void replaceAll(const std::string& oldText, const std::string& newText);
        
        // Estado del editor
        CursorPosition getCursorPosition() const;
        TextSelection getSelection() const;
        size_t getTotalLines() const;
        size_t getCurrentLineLength() const;
        std::string getCurrentLine() const;
        
        // Configuración
        void setLanguage(const std::string& language);
        void setTheme(const std::string& theme);
        void setTabSize(size_t size);
        void setWordWrap(bool enabled);
        
        // Eventos (llamados por EventHandler)
        void onKeyPressed(int keyCode, bool ctrl, bool shift, bool alt);
        void onTextEntered(char ch);
        void onMouseClicked(float x, float y, bool leftButton);
        void onMouseMoved(float x, float y, bool leftButtonDown);
        void onMouseScrolled(float delta, bool horizontal);
        void onWindowResized(size_t width, size_t height);
        
    private:
        // Subsistemas
        std::unique_ptr<TextBuffer> textBuffer_;
        std::unique_ptr<Viewport> viewport_;
        std::unique_ptr<SyntaxHighlighter> syntaxHighlighter_;
        std::unique_ptr<Window> window_;
        std::unique_ptr<UndoRedoManager> undoRedoManager_;
        std::unique_ptr<ClipboardManager> clipboardManager_;
        
        // Estado del editor
        CursorPosition cursor_;
        TextSelection selection_;
        std::string currentFilePath_;
        bool hasUnsavedChanges_;
        bool isRunning_;
        
        // Configuración
        size_t tabSize_;
        bool wordWrap_;
        
        // Estado de búsqueda
        std::string lastSearchText_;
        bool lastSearchCaseSensitive_;
        bool lastSearchWholeWord_;
        CursorPosition lastSearchPosition_;
        
        // Métodos internos
        void updateTitle();
        void markAsModified();
        void saveState();
        bool confirmUnsavedChanges();
        
        // Validación
        void validateCursorPosition();
        void adjustSelectionAfterEdit();
        
        // Utilidades
        bool isWordCharacter(char ch) const;
        CursorPosition findInText(const std::string& text, const CursorPosition& startPos,
                                  bool caseSensitive, bool wholeWord) const;
    };
    
} // namespace CoralCode