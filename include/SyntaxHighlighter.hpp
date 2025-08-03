#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <utility>
#include <memory>

namespace CoralCode {
    
    /**
     * @brief Tipo de token para syntax highlighting
     */
    enum class TokenType {
        Keyword,
        String,
        Comment,
        Number,
        Operator,
        Identifier,
        Whitespace,
        Unknown
    };
    
    /**
     * @brief Información de color para renderizado
     */
    struct TokenColor {
        uint8_t r, g, b, a;
        
        TokenColor(uint8_t red = 255, uint8_t green = 255, uint8_t blue = 255, uint8_t alpha = 255)
            : r(red), g(green), b(blue), a(alpha) {}
    };
    
    /**
     * @brief Token con su tipo y posición
     */
    struct Token {
        std::string text;
        TokenType type;
        size_t start;
        size_t end;
        
        Token(const std::string& t, TokenType tp, size_t s, size_t e)
            : text(t), type(tp), start(s), end(e) {}
    };
    
    /**
     * @brief Definición de un lenguaje
     */
    struct LanguageDefinition {
        std::string name;
        std::unordered_set<std::string> keywords;
        std::vector<std::string> singleLineComments;
        std::vector<std::pair<std::string, std::string>> multiLineComments;
        std::vector<char> stringDelimiters;
        std::vector<char> operators;
        bool caseSensitive;
        
        LanguageDefinition(const std::string& n = "Plain Text")
            : name(n), caseSensitive(true) {}
    };
    
    /**
     * @brief Sistema de syntax highlighting
     * 
     * Responsable de:
     * - Análisis léxico de líneas de texto
     * - Identificación de tokens (keywords, strings, comentarios)
     * - Asignación de colores por tipo de token
     * - Soporte para múltiples lenguajes
     */
    class SyntaxHighlighter {
    public:
        SyntaxHighlighter();
        ~SyntaxHighlighter() = default;
        
        // Configuración de lenguaje
        void setLanguage(const std::string& languageName);
        void setLanguageByExtension(const std::string& fileExtension);
        std::string getCurrentLanguage() const;
        
        // Análisis de líneas
        std::vector<Token> tokenizeLine(const std::string& line) const;
        std::vector<std::pair<std::string, TokenColor>> highlightLine(const std::string& line) const;
        
        // Análisis multi-línea (para comentarios de bloque)
        struct MultiLineState {
            bool inBlockComment = false;
            std::string blockCommentEnd;
        };
        
        std::vector<std::pair<std::string, TokenColor>> highlightLineWithState(
            const std::string& line, MultiLineState& state) const;
        
        // Configuración de colores
        void setTokenColor(TokenType type, const TokenColor& color);
        TokenColor getTokenColor(TokenType type) const;
        
        // Gestión de lenguajes
        void addLanguageDefinition(const LanguageDefinition& language);
        std::vector<std::string> getAvailableLanguages() const;
        
        // Configuración de tema
        void setTheme(const std::string& themeName);
        std::vector<std::string> getAvailableThemes() const;
        
        // Utilidades
        bool isKeyword(const std::string& word) const;
        TokenType identifyToken(const std::string& token) const;
        
    private:
        std::unique_ptr<LanguageDefinition> currentLanguage_;
        std::vector<std::unique_ptr<LanguageDefinition>> languages_;
        std::unordered_map<TokenType, TokenColor> colorScheme_;
        std::string currentTheme_;
        
        // Análisis interno
        std::vector<Token> parseTokens(const std::string& line) const;
        TokenType classifyToken(const std::string& token) const;
        bool isStringDelimiter(char ch) const;
        bool isOperator(char ch) const;
        bool isNumber(const std::string& token) const;
        
        // Inicialización
        void initializeLanguages();
        void initializeColorSchemes();
        void loadDefaultTheme();
        
        // Búsqueda de lenguajes
        const LanguageDefinition* findLanguageByName(const std::string& name) const;
        const LanguageDefinition* findLanguageByExtension(const std::string& extension) const;
    };
    
} // namespace CoralCode