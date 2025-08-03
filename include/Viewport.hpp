#pragma once

#include <cstddef>
#include <utility>

namespace CoralCode {
    
    /**
     * @brief Gestiona el viewport y scroll del editor
     * 
     * Responsable de:
     * - Posición del scroll (línea y columna)
     * - Cálculo de líneas/columnas visibles
     * - Auto-scroll basado en cursor
     * - Validación de límites de scroll
     */
    class Viewport {
    public:
        Viewport();
        Viewport(size_t windowWidth, size_t windowHeight);
        
        // Configuración del viewport
        void setWindowSize(size_t width, size_t height);
        void setCharacterMetrics(float charWidth, float lineHeight);
        void setUIMetrics(float lineNumberWidth, float statusBarHeight, float scrollBarWidth);
        
        // Gestión del scroll
        void setScrollPosition(size_t line, size_t col);
        void scrollToLine(size_t line);
        void scrollToColumn(size_t col);
        void scrollVertical(int delta);
        void scrollHorizontal(int delta);
        
        // Auto-scroll basado en cursor
        void ensureCursorVisible(size_t cursorLine, size_t cursorCol, size_t totalLines);
        void autoScrollToCursor(size_t cursorLine, size_t cursorCol, size_t totalLines);
        
        // Cálculos de visibilidad
        size_t getVisibleLines() const;
        size_t getVisibleColumns() const;
        size_t getFirstVisibleLine() const;
        size_t getLastVisibleLine(size_t totalLines) const;
        size_t getFirstVisibleColumn() const;
        size_t getLastVisibleColumn() const;
        
        // Conversión de coordenadas
        std::pair<size_t, size_t> screenToTextPosition(float x, float y) const;
        std::pair<float, float> textToScreenPosition(size_t line, size_t col) const;
        
        // Estado del scroll
        bool canScrollUp() const;
        bool canScrollDown(size_t totalLines) const;
        bool canScrollLeft() const;
        bool canScrollRight(size_t maxLineLength) const;
        
        // Scroll de página
        void pageUp();
        void pageDown(size_t totalLines);
        void scrollToTop();
        void scrollToBottom(size_t totalLines);
        
        // Información de scroll para UI
        float getVerticalScrollRatio(size_t totalLines) const;
        float getHorizontalScrollRatio(size_t maxLineLength) const;
        
        // Getters
        size_t getScrollLine() const { return scrollLine_; }
        size_t getScrollColumn() const { return scrollCol_; }
        
    private:
        // Posición del scroll
        size_t scrollLine_;
        size_t scrollCol_;
        
        // Métricas de ventana
        size_t windowWidth_;
        size_t windowHeight_;
        
        // Métricas de caracteres
        float charWidth_;
        float lineHeight_;
        
        // Métricas de UI
        float lineNumberWidth_;
        float statusBarHeight_;
        float scrollBarWidth_;
        
        // Cálculos internos
        float getTextAreaWidth() const;
        float getTextAreaHeight() const;
        void validateScrollPosition(size_t totalLines, size_t maxLineLength);
    };
    
} // namespace CoralCode