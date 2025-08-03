#include "UI/TextArea.h"
#include <algorithm>
#include <SFML/Graphics.hpp>

namespace CoralCode {

TextArea::TextArea() 
    : cursorLine_(0), cursorCol_(0), selecting_(false), firstVisibleLine_(0), scrollOffset_(0.0f) {
    
    // Configurar componentes visuales
    background_.setFillColor(sf::Color(45, 45, 45));  // Gris más claro
    cursor_.setFillColor(sf::Color(255, 255, 255));
    cursor_.setSize(sf::Vector2f(2.0f, 18.0f));
    
    selectionRect_.setFillColor(sf::Color(100, 100, 200, 100));
    
    // Configurar parser y highlighter
    parser_.setLanguage(Language::CPP);
    highlighter_.setTheme("dark");
    
    // Configurar callback para cambios en el buffer
    buffer_.setChangeCallback([this](size_t line, size_t col, const std::string& text) {
        // Marcar línea como sucia para re-renderizado
        highlighter_.markLineDirty(line);
    });
}

void TextArea::setPosition(float x, float y) {
    position_ = sf::Vector2f(x, y);
    background_.setPosition(position_);
    updateCursorPosition();
}

void TextArea::setSize(float width, float height) {
    size_ = sf::Vector2f(width, height);
    background_.setSize(size_);
    updateCursorPosition();
}

void TextArea::setFont(const sf::Font& font) {
    highlighter_.setFont(font);
    updateTextSprites();
}

void TextArea::setFontSize(unsigned int size) {
    highlighter_.setFontSize(size);
    updateTextSprites();
}

void TextArea::handleKeyPress(sf::Keyboard::Key key, bool ctrl, bool shift, bool alt) {
    switch (key) {
        case sf::Keyboard::Key::Left:
            moveCursor(-1, 0);
            break;
        case sf::Keyboard::Key::Right:
            moveCursor(1, 0);
            break;
        case sf::Keyboard::Key::Up:
            moveCursor(0, -1);
            break;
        case sf::Keyboard::Key::Down:
            moveCursor(0, 1);
            break;
        case sf::Keyboard::Key::Home:
            setCursorPosition(cursorLine_, 0);
            break;
        case sf::Keyboard::Key::End:
            setCursorPosition(cursorLine_, buffer_.getLineLength(cursorLine_));
            break;
        case sf::Keyboard::Key::PageUp:
            // Scroll hacia arriba
            break;
        case sf::Keyboard::Key::PageDown:
            // Scroll hacia abajo
            break;
        case sf::Keyboard::Key::Enter:
            try {
                // Método simple como simple_editor
                buffer_.splitLine(cursorLine_, cursorCol_);
                cursorLine_++;
                cursorCol_ = 0;
            } catch (...) {
                // Si hay error, no hacer nada
            }
            break;
        case sf::Keyboard::Key::Backspace:
            try {
                if (hasSelection()) {
                    deleteSelection();
                } else if (cursorCol_ > 0) {
                    // Borrar carácter simple
                    buffer_.remove(cursorLine_, cursorCol_ - 1, 1);
                    cursorCol_--;
                } else if (cursorLine_ > 0) {
                    // Fusionar con línea anterior
                    cursorCol_ = buffer_.getLineLength(cursorLine_ - 1);
                    buffer_.mergeLines(cursorLine_ - 1);
                    cursorLine_--;
                }
            } catch (...) {
                // Si hay error, no hacer nada
            }
            break;
        case sf::Keyboard::Key::Delete:
            if (hasSelection()) {
                deleteSelection();
            } else {
                buffer_.remove(cursorLine_, cursorCol_, 1);
            }
            break;
        case sf::Keyboard::Key::Tab:
            insertText("    "); // 4 espacios
            break;
        case sf::Keyboard::Key::C:
            if (ctrl) {
                copySelection();
            }
            break;
        case sf::Keyboard::Key::V:
            if (ctrl) {
                pasteText();
            }
            break;
        case sf::Keyboard::Key::X:
            if (ctrl) {
                copySelection();
                deleteSelection();
            }
            break;
        case sf::Keyboard::Key::Z:
            if (ctrl) {
                undo();
            }
            break;
        case sf::Keyboard::Key::Y:
            if (ctrl) {
                redo();
            }
            break;
        default:
            break;
    }
    
    if (shift) {
        if (!selecting_) {
            startSelection();
        }
    } else {
        if (selecting_) {
            endSelection();
        }
    }
}

void TextArea::handleTextInput(uint32_t unicode) {
    // Método simple como simple_editor que funciona
    if (unicode >= 32 && unicode < 127) {
        char c = static_cast<char>(unicode);
        
        if (hasSelection()) {
            deleteSelection();
        }
        
        // Insertar directamente en el string de la línea actual usando el buffer
        try {
            buffer_.insert(cursorLine_, cursorCol_, std::string(1, c));
            cursorCol_++;
        } catch (...) {
            // Si hay error, no hacer nada
        }
    }
}

void TextArea::handleMouseClick(float x, float y) {
    auto coords = screenToTextCoords(x, y);
    setCursorPosition(coords.first, coords.second);
    clearSelection();
}

void TextArea::handleMouseScroll(float delta) {
    // Scroll vertical
    scrollOffset_ += delta * 20.0f;
    if (scrollOffset_ < 0) scrollOffset_ = 0;
    
    updateScroll();
}

void TextArea::moveCursor(int deltaX, int deltaY) {
    // Método simple como simple_editor
    if (deltaY < 0 && cursorLine_ > 0) {
        cursorLine_--;
        cursorCol_ = std::min(cursorCol_, buffer_.getLineLength(cursorLine_));
    } else if (deltaY > 0 && cursorLine_ < buffer_.getLineCount() - 1) {
        cursorLine_++;
        cursorCol_ = std::min(cursorCol_, buffer_.getLineLength(cursorLine_));
    }
    
    if (deltaX < 0 && cursorCol_ > 0) {
        cursorCol_--;
    } else if (deltaX > 0 && cursorCol_ < buffer_.getLineLength(cursorLine_)) {
        cursorCol_++;
    }
}

void TextArea::setCursorPosition(size_t line, size_t col) {
    if (isValidPosition(line, col)) {
        cursorLine_ = line;
        cursorCol_ = col;
        updateCursorPosition();
        scrollToCursor();
    }
}

std::pair<size_t, size_t> TextArea::getCursorPosition() const {
    return {cursorLine_, cursorCol_};
}

void TextArea::startSelection() {
    if (!selecting_) {
        selecting_ = true;
        selectionStartLine_ = cursorLine_;
        selectionStartCol_ = cursorCol_;
    }
}

void TextArea::endSelection() {
    selecting_ = false;
    selectionEndLine_ = cursorLine_;
    selectionEndCol_ = cursorCol_;
}

void TextArea::clearSelection() {
    selecting_ = false;
}

bool TextArea::hasSelection() const {
    if (!selecting_ && selectionStartLine_ == selectionEndLine_ && 
        selectionStartCol_ == selectionEndCol_) {
        return false;
    }
    return true;
}

std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>> TextArea::getSelection() const {
    if (selecting_) {
        return {{selectionStartLine_, selectionStartCol_}, {cursorLine_, cursorCol_}};
    } else {
        return {{selectionStartLine_, selectionStartCol_}, {selectionEndLine_, selectionEndCol_}};
    }
}

void TextArea::insertText(const std::string& text) {
    if (text == "\n") {
        // Dividir línea
        buffer_.splitLine(cursorLine_, cursorCol_);
        setCursorPosition(cursorLine_ + 1, 0);
    } else {
        buffer_.insert(cursorLine_, cursorCol_, text);
        moveCursor(text.length(), 0);
    }
}

void TextArea::deleteSelection() {
    auto selection = getSelection();
    auto start = selection.first;
    auto end = selection.second;
    
    if (start.first == end.first) {
        // Selección en una sola línea
        buffer_.remove(start.first, start.second, end.second - start.second);
    } else {
        // Selección multi-línea
        // Eliminar líneas intermedias
        for (size_t i = start.first + 1; i < end.first; ++i) {
            buffer_.remove(i, 0, buffer_.getLineLength(i));
        }
        
        // Eliminar parte final de la primera línea
        buffer_.remove(start.first, start.second, buffer_.getLineLength(start.first) - start.second);
        
        // Eliminar parte inicial de la última línea
        buffer_.remove(end.first, 0, end.second);
        
        // Fusionar líneas
        if (end.first > start.first) {
            buffer_.mergeLines(start.first);
        }
    }
    
    setCursorPosition(start.first, start.second);
    clearSelection();
}

void TextArea::copySelection() {
    // Implementación básica - en una implementación real usarías el portapapeles del sistema
    if (hasSelection()) {
        auto selection = getSelection();
        // TODO: Implementar copia al portapapeles
    }
}

void TextArea::pasteText() {
    // Implementación básica - en una implementación real obtendrías del portapapeles del sistema
    // Por ahora, no hace nada
}

void TextArea::undo() {
    if (!undoHistory_.empty()) {
        auto entry = undoHistory_.back();
        undoHistory_.pop_back();
        
        if (entry.isInsertion) {
            buffer_.remove(entry.line, entry.col, entry.text.length());
        } else {
            buffer_.insert(entry.line, entry.col, entry.text);
        }
        
        setCursorPosition(entry.line, entry.col);
    }
}

void TextArea::redo() {
    if (!redoHistory_.empty()) {
        auto entry = redoHistory_.back();
        redoHistory_.pop_back();
        
        if (entry.isInsertion) {
            buffer_.insert(entry.line, entry.col, entry.text);
        } else {
            buffer_.remove(entry.line, entry.col, entry.text.length());
        }
        
        setCursorPosition(entry.line, entry.col);
    }
}

void TextArea::render(sf::RenderWindow& window) {
    // Renderizar fondo
    window.draw(background_);
    
    // Indicador de línea actual (como en visual_editor)
    sf::RectangleShape lineIndicator(sf::Vector2f(5.0f, 20.0f));
    float lineY = position_.y + 15.0f + (cursorLine_ - firstVisibleLine_) * 25.0f;
    lineIndicator.setPosition(sf::Vector2f(position_.x + 5.0f, lineY));
    lineIndicator.setFillColor(sf::Color(255, 0, 0)); // Rojo como visual_editor
    window.draw(lineIndicator);
    
    // Renderizar selección
    if (hasSelection()) {
        updateSelection();
        window.draw(selectionRect_);
    }
    
    // Renderizar texto
    for (const auto& textSprite : textSprites_) {
        window.draw(textSprite);
    }
    
    // Renderizar cursor
    window.draw(cursor_);
}

void TextArea::update() {
    // Solo actualizar si es necesario para mejorar rendimiento
    static int updateCounter = 0;
    updateCounter++;
    
    // Actualizar cursor cada frame (es rápido)
    updateCursorPosition();
    
    // Actualizar texto cada 5 frames para mejor responsividad
    if (updateCounter % 5 == 0) {
        updateTextSprites();
    }
}

TextBuffer& TextArea::getBuffer() {
    return buffer_;
}

const TextBuffer& TextArea::getBuffer() const {
    return buffer_;
}

SyntaxHighlighter& TextArea::getHighlighter() {
    return highlighter_;
}

const SyntaxHighlighter& TextArea::getHighlighter() const {
    return highlighter_;
}

void TextArea::setLanguage(Language lang) {
    parser_.setLanguage(lang);
}

Language TextArea::getLanguage() const {
    return parser_.getLanguage();
}

void TextArea::scrollToLine(size_t line) {
    firstVisibleLine_ = line;
    updateScroll();
}

void TextArea::scrollToCursor() {
    if (cursorLine_ < firstVisibleLine_) {
        firstVisibleLine_ = cursorLine_;
    } else if (cursorLine_ >= firstVisibleLine_ + static_cast<size_t>(size_.y / highlighter_.getLineHeight())) {
        firstVisibleLine_ = cursorLine_ - static_cast<size_t>(size_.y / highlighter_.getLineHeight()) + 1;
    }
    updateScroll();
}

size_t TextArea::getFirstVisibleLine() const {
    return firstVisibleLine_;
}

size_t TextArea::getLastVisibleLine() const {
    return firstVisibleLine_ + static_cast<size_t>(size_.y / highlighter_.getLineHeight());
}

void TextArea::updateCursorPosition() {
    // Usar métricas fijas como en visual_editor
    float charWidth = 12.0f;  // Mismo que visual_editor
    float lineHeight = 25.0f; // Mismo que visual_editor
    
    float x = position_.x + 15.0f + cursorCol_ * charWidth;
    float y = position_.y + 15.0f + (cursorLine_ - firstVisibleLine_) * lineHeight;
    
    cursor_.setPosition(sf::Vector2f(x, y));
    cursor_.setSize(sf::Vector2f(2.0f, 20.0f)); // Asegurar tamaño visible
}

void TextArea::updateSelection() {
    if (!hasSelection()) return;
    
    auto selection = getSelection();
    auto start = selection.first;
    auto end = selection.second;
    
    auto startPos = textToScreenCoords(start.first, start.second);
    auto endPos = textToScreenCoords(end.first, end.second);
    
    selectionRect_.setPosition(startPos);
    selectionRect_.setSize(sf::Vector2f(endPos.x - startPos.x, highlighter_.getLineHeight()));
}

void TextArea::updateTextSprites() {
    textSprites_.clear();
    
    // Usar las mismas métricas que el cursor para que coincidan
    float lineHeight = 25.0f;  // Mismo que cursor
    float charWidth = 12.0f;   // Mismo que cursor
    
    size_t lastVisibleLine = std::min(firstVisibleLine_ + 15, buffer_.getLineCount()); // Limitar líneas visibles para mejor rendimiento
    float yOffset = position_.y + 15.0f; // Mismo offset que cursor
    
    for (size_t line = firstVisibleLine_; line < lastVisibleLine; ++line) {
        const Line& bufferLine = buffer_.getLine(line);
        
        if (!bufferLine.content.empty()) {
            // Usar las mismas fuentes que funcionan en simple_editor
            static sf::Font systemFont;
            static bool fontLoaded = false;
            static bool fontAttempted = false;
            
            if (!fontAttempted) {
                // Intentar cargar fuentes del sistema macOS (igual que simple_editor)
                std::vector<std::string> fontPaths = {
                    "/System/Library/Fonts/Monaco.ttf",
                    "/System/Library/Fonts/Menlo.ttc", 
                    "/System/Library/Fonts/Courier New.ttf",
                    "/System/Library/Fonts/Helvetica.ttc"
                };
                
                for (const auto& path : fontPaths) {
                    if (systemFont.openFromFile(path)) {
                        fontLoaded = true;
                        break;
                    }
                }
                fontAttempted = true;
            }
            
            if (fontLoaded) {
                try {
                    // Método súper simple: solo texto blanco, sin highlighting por ahora
                    sf::Text textSprite(systemFont, bufferLine.content, 16);
                    textSprite.setPosition(sf::Vector2f(position_.x + 15.0f, yOffset));
                    textSprite.setFillColor(sf::Color(240, 240, 240)); // Solo blanco
                    textSprites_.push_back(textSprite);
                } catch (...) {
                    // Fallback si hay error
                }
            }
        }
        
        yOffset += lineHeight;
    }
}

void TextArea::updateScroll() {
    // Ajustar firstVisibleLine_ basado en scrollOffset_
    firstVisibleLine_ = static_cast<size_t>(scrollOffset_ / highlighter_.getLineHeight());
}

std::pair<size_t, size_t> TextArea::screenToTextCoords(float x, float y) const {
    // Usar las mismas métricas que el renderizado
    float lineHeight = 25.0f;
    float charWidth = 12.0f;
    
    size_t line = firstVisibleLine_ + static_cast<size_t>((y - position_.y - 15.0f) / lineHeight);
    size_t col = static_cast<size_t>((x - position_.x - 15.0f) / charWidth);
    
    clampPosition(line, col);
    return {line, col};
}

sf::Vector2f TextArea::textToScreenCoords(size_t line, size_t col) const {
    // Usar las mismas métricas que el renderizado
    float lineHeight = 25.0f;
    float charWidth = 12.0f;
    
    float x = position_.x + 15.0f + col * charWidth;
    float y = position_.y + 15.0f + (line - firstVisibleLine_) * lineHeight;
    
    return sf::Vector2f(x, y);
}

bool TextArea::isValidPosition(size_t line, size_t col) const {
    return line < buffer_.getLineCount() && col <= buffer_.getLineLength(line);
}

void TextArea::clampPosition(size_t& line, size_t& col) const {
    if (line >= buffer_.getLineCount()) {
        line = buffer_.getLineCount() - 1;
    }
    if (col > buffer_.getLineLength(line)) {
        col = buffer_.getLineLength(line);
    }
}

void TextArea::addToHistory(const HistoryEntry& entry) {
    undoHistory_.push_back(entry);
    redoHistory_.clear(); // Limpiar redo al hacer una nueva acción
}

} // namespace CoralCode 