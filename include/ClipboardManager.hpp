#pragma once

#include <string>
#include <vector>

namespace CoralCode {
    
    /**
     * @brief Gestiona las operaciones del clipboard del sistema
     * 
     * Responsable de:
     * - Integración con el clipboard del sistema
     * - Conversión de formatos de texto
     * - Historial interno del clipboard
     * - Soporte multi-plataforma
     */
    class ClipboardManager {
    public:
        ClipboardManager();
        ~ClipboardManager() = default;
        
        // Operaciones básicas del clipboard
        bool setText(const std::string& text);
        std::string getText() const;
        bool hasText() const;
        
        // Operaciones con múltiples líneas
        bool setLines(const std::vector<std::string>& lines);
        std::vector<std::string> getLines() const;
        
        // Historial interno
        void enableHistory(bool enabled);
        bool isHistoryEnabled() const;
        void setHistorySize(size_t size);
        size_t getHistorySize() const;
        
        // Acceso al historial
        std::vector<std::string> getHistory() const;
        std::string getHistoryItem(size_t index) const;
        void clearHistory();
        
        // Detección de formato
        enum class TextFormat {
            PlainText,
            MultiLine,
            RichText,
            Unknown
        };
        
        TextFormat detectFormat(const std::string& text) const;
        
        // Conversión de formato
        std::string normalizeLineEndings(const std::string& text) const;
        std::string convertTabsToSpaces(const std::string& text, size_t tabSize = 4) const;
        std::string convertSpacesToTabs(const std::string& text, size_t tabSize = 4) const;
        
        // Estado del sistema
        bool isSystemClipboardAvailable() const;
        std::string getLastError() const;
        
    private:
        bool historyEnabled_;
        size_t maxHistorySize_;
        std::vector<std::string> clipboardHistory_;
        mutable std::string lastError_;
        
        // Implementaciones específicas por plataforma
        bool setSystemClipboard(const std::string& text) const;
        std::string getSystemClipboard() const;
        bool hasSystemClipboard() const;
        
        // Utilidades internas
        void addToHistory(const std::string& text);
        void trimHistory();
        
        // Detección de plataforma
        #ifdef _WIN32
            bool setWindowsClipboard(const std::string& text) const;
            std::string getWindowsClipboard() const;
        #elif __APPLE__
            bool setMacClipboard(const std::string& text) const;
            std::string getMacClipboard() const;
        #elif __linux__
            bool setLinuxClipboard(const std::string& text) const;
            std::string getLinuxClipboard() const;
        #endif
    };
    
} // namespace CoralCode