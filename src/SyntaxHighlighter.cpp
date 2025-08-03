#include "SyntaxHighlighter.h"
#include <algorithm>

namespace CoralCode {

SyntaxHighlighter::SyntaxHighlighter() 
    : fontSize_(14), lineHeight_(0.0f), charWidth_(0.0f) {
    setDarkTheme();
    calculateMetrics();
}

void SyntaxHighlighter::setTheme(const std::string& themeName) {
    if (themeName == "dark") {
        setDarkTheme();
    } else if (themeName == "light") {
        setLightTheme();
    } else if (themeName == "monokai") {
        setMonokaiTheme();
    }
}

void SyntaxHighlighter::setKeywordColor(const sf::Color& color) {
    keywordColor_ = color;
}

void SyntaxHighlighter::setStringColor(const sf::Color& color) {
    stringColor_ = color;
}

void SyntaxHighlighter::setCommentColor(const sf::Color& color) {
    commentColor_ = color;
}

void SyntaxHighlighter::setNumberColor(const sf::Color& color) {
    numberColor_ = color;
}

void SyntaxHighlighter::setOperatorColor(const sf::Color& color) {
    operatorColor_ = color;
}

void SyntaxHighlighter::setDefaultColor(const sf::Color& color) {
    defaultColor_ = color;
}

std::vector<RenderedLine> SyntaxHighlighter::renderText(const TextBuffer& buffer) {
    std::vector<RenderedLine> renderedLines;
    size_t lineCount = buffer.getLineCount();
    
    for (size_t i = 0; i < lineCount; ++i) {
        const Line& line = buffer.getLine(i);
        RenderedLine renderedLine = renderLine(line, LanguageParser());
        renderedLines.push_back(renderedLine);
    }
    
    return renderedLines;
}

RenderedLine SyntaxHighlighter::renderLine(const Line& line, const LanguageParser& parser) {
    RenderedLine renderedLine;
    
    if (line.content.empty()) {
        renderedLine.height = lineHeight_;
        return renderedLine;
    }
    
    // Parsear la línea si está sucia
    if (line.dirty) {
        std::vector<SyntaxToken> tokens = const_cast<LanguageParser&>(parser).parseLine(line.content);
        
        // Convertir tokens a fragmentos
        size_t currentPos = 0;
        for (const auto& token : tokens) {
            // Agregar texto antes del token
            if (token.start > currentPos) {
                std::string textBefore = line.content.substr(currentPos, token.start - currentPos);
                if (!textBefore.empty()) {
                    renderedLine.fragments.emplace_back(textBefore, defaultColor_);
                }
            }
            
            // Agregar el token
            std::string tokenText = line.content.substr(token.start, token.length);
            TextFragment fragment = tokenToFragment(token, line.content);
            renderedLine.fragments.push_back(fragment);
            
            currentPos = token.start + token.length;
        }
        
        // Agregar texto restante
        if (currentPos < line.content.length()) {
            std::string remainingText = line.content.substr(currentPos);
            if (!remainingText.empty()) {
                renderedLine.fragments.emplace_back(remainingText, defaultColor_);
            }
        }
    } else {
        // Si la línea no está sucia, usar el texto completo con color por defecto
        renderedLine.fragments.emplace_back(line.content, defaultColor_);
    }
    
    // Calcular altura de la línea
    renderedLine.height = lineHeight_;
    
    return renderedLine;
}

void SyntaxHighlighter::markLineDirty(size_t line) {
    if (line < lineCache_.size()) {
        lineCache_[line].dirty = true;
    }
}

void SyntaxHighlighter::clearCache() {
    lineCache_.clear();
}

void SyntaxHighlighter::setFont(const sf::Font& font) {
    font_ = font;
    calculateMetrics();
}

void SyntaxHighlighter::setFontSize(unsigned int size) {
    fontSize_ = size;
    calculateMetrics();
}

const sf::Font& SyntaxHighlighter::getFont() const {
    return font_;
}

float SyntaxHighlighter::getLineHeight() const {
    return lineHeight_;
}

float SyntaxHighlighter::getCharWidth() const {
    return charWidth_;
}

unsigned int SyntaxHighlighter::getFontSize() const {
    return fontSize_;
}

TextFragment SyntaxHighlighter::tokenToFragment(const SyntaxToken& token, const std::string& lineText) {
    std::string text = lineText.substr(token.start, token.length);
    sf::Color color = defaultColor_;
    bool bold = false;
    bool italic = false;
    
    switch (token.type) {
        case SyntaxToken::Type::KEYWORD:
            color = keywordColor_;
            bold = true;
            break;
        case SyntaxToken::Type::STRING:
            color = stringColor_;
            break;
        case SyntaxToken::Type::COMMENT:
            color = commentColor_;
            italic = true;
            break;
        case SyntaxToken::Type::NUMBER:
            color = numberColor_;
            break;
        case SyntaxToken::Type::OPERATOR:
            color = operatorColor_;
            break;
        case SyntaxToken::Type::IDENTIFIER:
            color = defaultColor_;
            break;
        default:
            color = defaultColor_;
            break;
    }
    
    return TextFragment(text, color, bold, italic);
}

void SyntaxHighlighter::calculateMetrics() {
    // Usar valores por defecto para evitar problemas con SFML 3.0
    lineHeight_ = fontSize_ * 1.2f;
    charWidth_ = fontSize_ * 0.6f;
}

void SyntaxHighlighter::setDarkTheme() {
    keywordColor_ = sf::Color(120, 180, 255);   // Azul más brillante
    stringColor_ = sf::Color(255, 200, 150);    // Naranja más brillante
    commentColor_ = sf::Color(120, 200, 120);   // Verde más brillante
    numberColor_ = sf::Color(200, 230, 200);    // Verde claro más brillante
    operatorColor_ = sf::Color(200, 200, 200);  // Gris más claro
    defaultColor_ = sf::Color(240, 240, 240);   // Blanco más brillante
}

void SyntaxHighlighter::setLightTheme() {
    keywordColor_ = sf::Color(0, 0, 255);      // Azul
    stringColor_ = sf::Color(163, 21, 21);     // Rojo oscuro
    commentColor_ = sf::Color(0, 128, 0);      // Verde oscuro
    numberColor_ = sf::Color(0, 0, 128);       // Azul oscuro
    operatorColor_ = sf::Color(0, 0, 0);       // Negro
    defaultColor_ = sf::Color(0, 0, 0);        // Negro
}

void SyntaxHighlighter::setMonokaiTheme() {
    keywordColor_ = sf::Color(249, 38, 114);   // Rosa
    stringColor_ = sf::Color(230, 219, 116);   // Amarillo
    commentColor_ = sf::Color(117, 113, 94);   // Gris
    numberColor_ = sf::Color(174, 129, 255);   // Púrpura
    operatorColor_ = sf::Color(249, 38, 114);  // Rosa
    defaultColor_ = sf::Color(248, 248, 242);  // Blanco crema
}

} // namespace CoralCode 