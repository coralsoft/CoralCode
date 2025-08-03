#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <functional>

namespace CoralCode {
    
    // Forward declarations
    class EventHandler;
    class Renderer;
    
    /**
     * @brief Configuración de ventana
     */
    struct WindowConfig {
        size_t width = 1000;
        size_t height = 700;
        std::string title = "CoralCode";
        bool resizable = true;
        bool vsync = true;
        size_t antialiasing = 4;
        
        WindowConfig() = default;
    };
    
    /**
     * @brief Gestiona la ventana principal del editor
     * 
     * Responsable de:
     * - Creación y configuración de ventana SFML
     * - Gestión del loop principal de eventos
     * - Coordinación entre EventHandler y Renderer
     * - Gestión del estado de la ventana
     */
    class Window {
    public:
        Window();
        ~Window();
        
        // Inicialización
        bool create(const WindowConfig& config = WindowConfig{});
        void close();
        bool isOpen() const;
        
        // Loop principal
        void pollEvents();
        void clear();
        void display();
        
        // Configuración de ventana
        void setTitle(const std::string& title);
        void setSize(size_t width, size_t height);
        void setPosition(int x, int y);
        void setIcon(const std::string& iconPath);
        
        // Estado de ventana
        sf::Vector2u getSize() const;
        sf::Vector2i getPosition() const;
        bool hasFocus() const;
        bool isMinimized() const;
        
        // Renderizado
        sf::RenderWindow& getRenderWindow();
        const sf::RenderWindow& getRenderWindow() const;
        
        // Cursores
        enum class CursorType {
            Arrow,
            Text,
            Hand,
            ResizeHorizontal,
            ResizeVertical,
            ResizeDiagonal1,
            ResizeDiagonal2
        };
        
        void setCursor(CursorType type);
        
        // Configuración de renderizado
        void setVSync(bool enabled);
        void setFramerateLimit(unsigned int limit);
        
        // Callbacks de eventos (configurados externamente)
        void setEventHandler(std::shared_ptr<EventHandler> handler);
        void setRenderer(std::shared_ptr<Renderer> renderer);
        
        // Información del sistema
        static std::vector<sf::VideoMode> getAvailableVideoModes();
        static sf::VideoMode getDesktopMode();
        
    private:
        std::unique_ptr<sf::RenderWindow> window_;
        std::shared_ptr<EventHandler> eventHandler_;
        std::shared_ptr<Renderer> renderer_;
        WindowConfig config_;
        
        // Cursores del sistema
        std::unordered_map<CursorType, std::unique_ptr<sf::Cursor>> cursors_;
        CursorType currentCursor_;
        
        // Estado interno
        bool isInitialized_;
        
        // Métodos internos
        void initializeCursors();
        void handleEvent(const sf::Event& event);
        sf::Uint32 getWindowStyle() const;
        sf::ContextSettings getContextSettings() const;
        
        // Validación
        bool validateConfig(const WindowConfig& config) const;
    };
    
} // namespace CoralCode