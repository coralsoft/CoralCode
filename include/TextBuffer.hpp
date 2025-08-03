#pragma once

#include <vector>
#include <string>
#include <cstddef>

namespace CoralCode {
    
    /**
     * @brief Gestiona el contenido de texto del editor
     * 
     * Responsable de:
     * - Almacenar líneas de texto
     * - Operaciones de inserción/eliminación
     * - Navegación del cursor
     * - Validación de posiciones
     */
    class TextBuffer {
    public:
        TextBuffer();
        explicit TextBuffer(const std::vector<std::string>& initialLines);
        
        // Gestión de contenido
        void insertChar(size_t line, size_t col, char ch);
        void insertText(size_t line, size_t col, const std::string& text);
        void deleteChar(size_t line, size_t col);
        void deleteLine(size_t line);
        void insertLine(size_t line, const std::string& content = "");
        
        // Operaciones de línea
        void splitLine(size_t line, size_t col);
        void mergeLine(size_t line);
        
        // Acceso al contenido
        const std::string& getLine(size_t line) const;
        std::string& getLine(size_t line);
        size_t getLineCount() const;
        size_t getLineLength(size_t line) const;
        
        // Validación
        bool isValidPosition(size_t line, size_t col) const;
        std::pair<size_t, size_t> clampPosition(size_t line, size_t col) const;
        
        // Operaciones en bloque
        std::vector<std::string> getLines(size_t startLine, size_t endLine) const;
        void replaceLines(size_t startLine, const std::vector<std::string>& newLines);
        
        // Conversión
        std::string toString() const;
        void fromString(const std::string& content);
        
        // Estadísticas
        size_t getTotalCharacters() const;
        bool isEmpty() const;
        
    private:
        std::vector<std::string> lines_;
        
        void ensureLineExists(size_t line);
        void validateLineIndex(size_t line) const;
    };
    
} // namespace CoralCode