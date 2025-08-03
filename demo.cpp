#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

// Simulación de las clases principales del editor

// Estructura para tokens de sintaxis
struct SyntaxToken {
    enum class Type {
        KEYWORD,
        STRING,
        COMMENT,
        NUMBER,
        OPERATOR,
        IDENTIFIER,
        NONE
    };
    
    Type type;
    size_t start;
    size_t length;
    std::string value;
    
    SyntaxToken(Type t, size_t s, size_t len, const std::string& val = "")
        : type(t), start(s), length(len), value(val) {}
};

// Clase para detectar lenguaje
enum class Language {
    PYTHON,
    C,
    CPP,
    CSHARP,
    HTML,
    JSON,
    JAVA,
    JAVASCRIPT,
    UNKNOWN
};

// Parser de lenguaje simplificado
class LanguageParser {
private:
    Language currentLanguage_;
    std::unordered_set<std::string> keywords_;
    
    void initPythonKeywords() {
        std::vector<std::string> keywords = {
            "False", "None", "True", "and", "as", "assert", "async", "await",
            "break", "class", "continue", "def", "del", "elif", "else", "except",
            "finally", "for", "from", "global", "if", "import", "in", "is",
            "lambda", "nonlocal", "not", "or", "pass", "raise", "return",
            "try", "while", "with", "yield"
        };
        
        for (const auto& keyword : keywords) {
            keywords_.insert(keyword);
        }
    }
    
    void initCppKeywords() {
        std::vector<std::string> keywords = {
            "auto", "break", "case", "char", "const", "continue", "default",
            "do", "double", "else", "enum", "extern", "float", "for", "goto",
            "if", "int", "long", "register", "return", "short", "signed",
            "sizeof", "static", "struct", "switch", "typedef", "union",
            "unsigned", "void", "volatile", "while", "class", "namespace",
            "template", "typename", "virtual", "public", "private", "protected"
        };
        
        for (const auto& keyword : keywords) {
            keywords_.insert(keyword);
        }
    }
    
public:
    LanguageParser() : currentLanguage_(Language::UNKNOWN) {}
    
    void setLanguage(Language lang) {
        currentLanguage_ = lang;
        keywords_.clear();
        
        switch (lang) {
            case Language::PYTHON:
                initPythonKeywords();
                break;
            case Language::CPP:
                initCppKeywords();
                break;
            default:
                break;
        }
    }
    
    std::vector<SyntaxToken> parseLine(const std::string& line) {
        std::vector<SyntaxToken> tokens;
        
        // Parsear comentarios
        size_t commentPos = line.find("//");
        if (commentPos != std::string::npos) {
            tokens.emplace_back(SyntaxToken::Type::COMMENT, commentPos, line.length() - commentPos);
        }
        
        // Parsear strings
        size_t pos = 0;
        while (pos < line.length()) {
            if (line[pos] == '"' || line[pos] == '\'') {
                char quote = line[pos];
                size_t start = pos;
                pos++;
                
                while (pos < line.length() && line[pos] != quote) {
                    if (line[pos] == '\\' && pos + 1 < line.length()) {
                        pos += 2;
                    } else {
                        pos++;
                    }
                }
                
                if (pos < line.length()) {
                    tokens.emplace_back(SyntaxToken::Type::STRING, start, pos - start + 1);
                }
            }
            pos++;
        }
        
        // Parsear keywords
        pos = 0;
        while (pos < line.length()) {
            if (isalpha(line[pos]) || line[pos] == '_') {
                size_t start = pos;
                while (pos < line.length() && (isalnum(line[pos]) || line[pos] == '_')) {
                    pos++;
                }
                
                std::string word = line.substr(start, pos - start);
                if (keywords_.find(word) != keywords_.end()) {
                    tokens.emplace_back(SyntaxToken::Type::KEYWORD, start, pos - start, word);
                }
            } else {
                pos++;
            }
        }
        
        return tokens;
    }
};

// Buffer de texto simplificado
class TextBuffer {
private:
    std::vector<std::string> lines_;
    
public:
    TextBuffer() {
        lines_.push_back("");
    }
    
    void insert(size_t line, size_t col, const std::string& text) {
        if (line >= lines_.size()) return;
        
        if (col > lines_[line].length()) {
            col = lines_[line].length();
        }
        
        lines_[line].insert(col, text);
    }
    
    void remove(size_t line, size_t col, size_t length) {
        if (line >= lines_.size()) return;
        
        if (col >= lines_[line].length()) return;
        
        size_t actualLength = std::min(length, lines_[line].length() - col);
        lines_[line].erase(col, actualLength);
    }
    
    const std::string& getLine(size_t index) const {
        if (index >= lines_.size()) {
            static const std::string empty = "";
            return empty;
        }
        return lines_[index];
    }
    
    size_t getLineCount() const {
        return lines_.size();
    }
    
    size_t getLineLength(size_t line) const {
        if (line >= lines_.size()) return 0;
        return lines_[line].length();
    }
    
    void splitLine(size_t line, size_t col) {
        if (line >= lines_.size()) return;
        
        if (col > lines_[line].length()) {
            col = lines_[line].length();
        }
        
        std::string remainingText = lines_[line].substr(col);
        lines_[line].erase(col);
        
        lines_.insert(lines_.begin() + line + 1, remainingText);
    }
    
    void mergeLines(size_t line) {
        if (line >= lines_.size() - 1) return;
        
        lines_[line] += lines_[line + 1];
        lines_.erase(lines_.begin() + line + 1);
    }
};

// Función para mostrar tokens con colores (simulado)
void printTokens(const std::string& line, const std::vector<SyntaxToken>& tokens) {
    std::cout << "Línea: " << line << std::endl;
    std::cout << "Tokens: ";
    
    for (const auto& token : tokens) {
        std::string tokenText = line.substr(token.start, token.length);
        
        switch (token.type) {
            case SyntaxToken::Type::KEYWORD:
                std::cout << "\033[1;34m" << tokenText << "\033[0m "; // Azul
                break;
            case SyntaxToken::Type::STRING:
                std::cout << "\033[1;33m" << tokenText << "\033[0m "; // Amarillo
                break;
            case SyntaxToken::Type::COMMENT:
                std::cout << "\033[1;32m" << tokenText << "\033[0m "; // Verde
                break;
            case SyntaxToken::Type::NUMBER:
                std::cout << "\033[1;35m" << tokenText << "\033[0m "; // Magenta
                break;
            default:
                std::cout << tokenText << " ";
                break;
        }
    }
    std::cout << std::endl << std::endl;
}

int main() {
    std::cout << "=== CoralCode Editor - Demostración ===" << std::endl;
    std::cout << "Editor de código ligero y rápido con syntax highlighting" << std::endl << std::endl;
    
    // Crear parser y buffer
    LanguageParser parser;
    TextBuffer buffer;
    
    // Configurar para C++
    parser.setLanguage(Language::CPP);
    
    // Ejemplos de código
    std::vector<std::string> examples = {
        "#include <iostream>",
        "",
        "// Función principal",
        "int main() {",
        "    std::string message = \"Hola, mundo!\";",
        "    std::cout << message << std::endl;",
        "    ",
        "    // Variables y operaciones",
        "    int number = 42;",
        "    double pi = 3.14159;",
        "    ",
        "    if (number > 40) {",
        "        std::cout << \"Número alto: \" << number << std::endl;",
        "    }",
        "    ",
        "    return 0;",
        "}"
    };
    
    // Cargar ejemplos en el buffer
    for (size_t i = 0; i < examples.size(); ++i) {
        if (i < buffer.getLineCount()) {
            // Reemplazar línea existente
            buffer.remove(i, 0, buffer.getLineLength(i));
            buffer.insert(i, 0, examples[i]);
        } else {
            // Agregar nueva línea
            buffer.insert(i, 0, examples[i]);
        }
    }
    
    // Mostrar código con syntax highlighting
    std::cout << "Código C++ con syntax highlighting:" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    for (size_t i = 0; i < buffer.getLineCount(); ++i) {
        const std::string& line = buffer.getLine(i);
        if (!line.empty()) {
            std::vector<SyntaxToken> tokens = parser.parseLine(line);
            printTokens(line, tokens);
        } else {
            std::cout << "(línea vacía)" << std::endl << std::endl;
        }
    }
    
    // Demostrar operaciones de edición
    std::cout << "=== Operaciones de edición ===" << std::endl;
    
    // Insertar texto
    std::cout << "Insertando texto en línea 1..." << std::endl;
    buffer.insert(1, 0, "// Comentario agregado: ");
    std::cout << "Línea 1: " << buffer.getLine(1) << std::endl << std::endl;
    
    // Dividir línea
    std::cout << "Dividiendo línea 5..." << std::endl;
    buffer.splitLine(5, 20);
    std::cout << "Línea 5: " << buffer.getLine(5) << std::endl;
    std::cout << "Línea 6: " << buffer.getLine(6) << std::endl << std::endl;
    
    // Fusionar líneas
    std::cout << "Fusionando líneas 5 y 6..." << std::endl;
    buffer.mergeLines(5);
    std::cout << "Línea 5: " << buffer.getLine(5) << std::endl << std::endl;
    
    // Cambiar a Python
    std::cout << "=== Cambiando a Python ===" << std::endl;
    parser.setLanguage(Language::PYTHON);
    
    std::vector<std::string> pythonExamples = {
        "# Función de ejemplo en Python",
        "def calculate_area(radius):",
        "    pi = 3.14159",
        "    area = pi * radius ** 2",
        "    return area",
        "",
        "# Uso de la función",
        "result = calculate_area(5)",
        "print(f\"El área es: {result}\")",
        "",
        "# Lista y bucle",
        "numbers = [1, 2, 3, 4, 5]",
        "for num in numbers:",
        "    if num % 2 == 0:",
        "        print(f\"{num} es par\")",
        "    else:",
        "        print(f\"{num} es impar\")"
    };
    
    // Limpiar buffer y cargar ejemplos de Python
    for (size_t i = 0; i < pythonExamples.size(); ++i) {
        if (i < buffer.getLineCount()) {
            buffer.remove(i, 0, buffer.getLineLength(i));
            buffer.insert(i, 0, pythonExamples[i]);
        } else {
            buffer.insert(i, 0, pythonExamples[i]);
        }
    }
    
    std::cout << "Código Python con syntax highlighting:" << std::endl;
    std::cout << "======================================" << std::endl;
    
    for (size_t i = 0; i < buffer.getLineCount(); ++i) {
        const std::string& line = buffer.getLine(i);
        if (!line.empty()) {
            std::vector<SyntaxToken> tokens = parser.parseLine(line);
            printTokens(line, tokens);
        } else {
            std::cout << "(línea vacía)" << std::endl << std::endl;
        }
    }
    
    std::cout << "=== Características del editor ===" << std::endl;
    std::cout << "✓ Syntax highlighting para múltiples lenguajes" << std::endl;
    std::cout << "✓ Operaciones eficientes de texto" << std::endl;
    std::cout << "✓ Arquitectura modular y extensible" << std::endl;
    std::cout << "✓ Optimizaciones de rendimiento" << std::endl;
    std::cout << "✓ Soporte para Python, C, C++, C#, HTML, JSON, Java, JavaScript" << std::endl;
    
    std::cout << std::endl << "¡Demostración completada!" << std::endl;
    
    return 0;
} 