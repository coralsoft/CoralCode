#pragma once

#include <SFML/Window/Event.hpp>
#include <functional>
#include <unordered_map>
#include <memory>

namespace CoralCode {
    
    // Forward declaration
    class Editor;
    
    /**
     * @brief Información extendida de teclas
     */
    struct KeyInfo {
        sf::Keyboard::Key key;
        bool ctrl;
        bool shift;
        bool alt;
        bool system; // Cmd en Mac, Windows en PC
        
        KeyInfo(sf::Keyboard::Key k, bool c = false, bool s = false, bool a = false, bool sys = false)
            : key(k), ctrl(c), shift(s), alt(a), system(sys) {}
    };
    
    /**
     * @brief Información de mouse
     */
    struct MouseInfo {
        float x, y;
        sf::Mouse::Button button;
        bool pressed;
        
        MouseInfo(float mx = 0, float my = 0, sf::Mouse::Button b = sf::Mouse::Left, bool p = false)
            : x(mx), y(my), button(b), pressed(p) {}
    };
    
    /**
     * @brief Gestiona todos los eventos de entrada
     * 
     * Responsable de:
     * - Procesamiento de eventos SFML
     * - Traducción a eventos del editor
     * - Gestión de shortcuts y combinaciones
     * - Filtrado y validación de eventos
     */
    class EventHandler {
    public:
        EventHandler();
        ~EventHandler() = default;
        
        // Configuración
        void setEditor(std::shared_ptr<Editor> editor);
        
        // Procesamiento de eventos
        bool handleEvent(const sf::Event& event);
        
        // Configuración de shortcuts
        using ShortcutCallback = std::function<void()>;
        void addShortcut(const KeyInfo& keyInfo, ShortcutCallback callback);
        void removeShortcut(const KeyInfo& keyInfo);
        void clearShortcuts();
        
        // Configuración de comportamiento
        void setKeyRepeatEnabled(bool enabled);
        void setMouseSensitivity(float sensitivity);
        void setScrollSensitivity(float sensitivity);
        
        // Estado del input
        bool isKeyPressed(sf::Keyboard::Key key) const;
        bool isMouseButtonPressed(sf::Mouse::Button button) const;
        sf::Vector2f getMousePosition() const;
        
        // Filtros de eventos
        void setTextInputFilter(std::function<bool(char)> filter);
        void setKeyFilter(std::function<bool(const KeyInfo&)> filter);
        
        // Configuración específica por plataforma
        void configureMacShortcuts();
        void configureWindowsShortcuts();
        void configureLinuxShortcuts();
        
    private:
        std::shared_ptr<Editor> editor_;
        
        // Estado del teclado y mouse
        std::unordered_map<sf::Keyboard::Key, bool> keyStates_;
        std::unordered_map<sf::Mouse::Button, bool> mouseStates_;
        sf::Vector2f mousePosition_;
        
        // Shortcuts
        struct ShortcutKey {
            sf::Keyboard::Key key;
            bool ctrl, shift, alt, system;
            
            bool operator==(const ShortcutKey& other) const {
                return key == other.key && ctrl == other.ctrl && 
                       shift == other.shift && alt == other.alt && system == other.system;
            }
        };
        
        struct ShortcutKeyHash {
            std::size_t operator()(const ShortcutKey& k) const {
                return std::hash<int>()(static_cast<int>(k.key)) ^
                       (std::hash<bool>()(k.ctrl) << 1) ^
                       (std::hash<bool>()(k.shift) << 2) ^
                       (std::hash<bool>()(k.alt) << 3) ^
                       (std::hash<bool>()(k.system) << 4);
            }
        };
        
        std::unordered_map<ShortcutKey, ShortcutCallback, ShortcutKeyHash> shortcuts_;
        
        // Configuración
        bool keyRepeatEnabled_;
        float mouseSensitivity_;
        float scrollSensitivity_;
        
        // Filtros
        std::function<bool(char)> textInputFilter_;
        std::function<bool(const KeyInfo&)> keyFilter_;
        
        // Handlers específicos
        bool handleKeyPressed(const sf::Event::KeyEvent& keyEvent);
        bool handleKeyReleased(const sf::Event::KeyEvent& keyEvent);
        bool handleTextEntered(const sf::Event::TextEvent& textEvent);
        bool handleMouseButtonPressed(const sf::Event::MouseButtonEvent& mouseEvent);
        bool handleMouseButtonReleased(const sf::Event::MouseButtonEvent& mouseEvent);
        bool handleMouseMoved(const sf::Event::MouseMoveEvent& mouseEvent);
        bool handleMouseWheelScrolled(const sf::Event::MouseWheelScrollEvent& wheelEvent);
        bool handleWindowResized(const sf::Event::SizeEvent& sizeEvent);
        bool handleWindowClosed();
        
        // Utilidades
        KeyInfo createKeyInfo(const sf::Event::KeyEvent& keyEvent) const;
        bool checkModifiers(const sf::Event::KeyEvent& keyEvent, bool ctrl, bool shift, bool alt, bool system) const;
        ShortcutKey createShortcutKey(const KeyInfo& keyInfo) const;
        
        // Filtrado
        bool isValidTextInput(char ch) const;
        bool isValidKeyInput(const KeyInfo& keyInfo) const;
        
        // Configuración por defecto
        void setupDefaultShortcuts();
        void detectPlatformAndConfigure();
    };
    
} // namespace CoralCode