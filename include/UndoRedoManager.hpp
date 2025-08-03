#pragma once

#include "TextBuffer.hpp"
#include <deque>
#include <string>
#include <memory>
#include <chrono>

namespace CoralCode {
    
    /**
     * @brief Tipo de operación para el historial
     */
    enum class OperationType {
        Insert,
        Delete,
        Replace,
        Compound
    };
    
    /**
     * @brief Estado del editor para undo/redo
     */
    struct EditorState {
        std::vector<std::string> lines;
        CursorPosition cursor;
        std::chrono::steady_clock::time_point timestamp;
        OperationType operation;
        std::string description;
        
        EditorState() = default;
        EditorState(const std::vector<std::string>& l, const CursorPosition& c, 
                   OperationType op, const std::string& desc)
            : lines(l), cursor(c), timestamp(std::chrono::steady_clock::now()),
              operation(op), description(desc) {}
    };
    
    /**
     * @brief Gestiona el historial de undo/redo
     * 
     * Responsable de:
     * - Almacenar estados del editor
     * - Operaciones de undo/redo
     * - Agrupación inteligente de operaciones
     * - Límites de memoria del historial
     */
    class UndoRedoManager {
    public:
        UndoRedoManager(size_t maxHistorySize = 100);
        ~UndoRedoManager() = default;
        
        // Gestión del historial
        void saveState(const TextBuffer& buffer, const CursorPosition& cursor,
                      OperationType operation, const std::string& description);
        bool undo(TextBuffer& buffer, CursorPosition& cursor);
        bool redo(TextBuffer& buffer, CursorPosition& cursor);
        
        // Estado del historial
        bool canUndo() const;
        bool canRedo() const;
        size_t getUndoCount() const;
        size_t getRedoCount() const;
        
        // Configuración
        void setMaxHistorySize(size_t size);
        size_t getMaxHistorySize() const;
        void clear();
        
        // Agrupación de operaciones
        void beginCompoundOperation(const std::string& description);
        void endCompoundOperation();
        bool isInCompoundOperation() const;
        
        // Información del historial
        std::string getUndoDescription() const;
        std::string getRedoDescription() const;
        std::vector<std::string> getHistoryDescriptions() const;
        
        // Optimización de memoria
        void compactHistory();
        size_t getMemoryUsage() const;
        
    private:
        std::deque<EditorState> undoHistory_;
        std::deque<EditorState> redoHistory_;
        size_t maxHistorySize_;
        
        // Agrupación de operaciones
        bool inCompoundOperation_;
        std::string compoundDescription_;
        std::unique_ptr<EditorState> compoundStartState_;
        
        // Configuración de agrupación
        static constexpr auto MAX_GROUP_TIME = std::chrono::milliseconds(1000);
        static constexpr size_t MAX_GROUP_SIZE = 50;
        
        // Métodos internos
        void addToHistory(const EditorState& state);
        void clearRedoHistory();
        bool shouldGroupWithPrevious(const EditorState& newState) const;
        void trimHistoryIfNeeded();
        
        // Utilidades
        size_t calculateStateSize(const EditorState& state) const;
        bool statesAreEqual(const EditorState& a, const EditorState& b) const;
    };
    
} // namespace CoralCode