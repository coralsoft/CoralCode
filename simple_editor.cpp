#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

int main() {
    // Crear ventana más pequeña y simple
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "CoralCode - Editor Simple");
    window.setFramerateLimit(60);
    
    // Variables del editor
    std::vector<std::string> lines = {""};
    size_t currentLine = 0;
    size_t currentCol = 0;
    
    // Colores más visibles
    sf::Color backgroundColor(40, 40, 40);
    sf::Color textColor(255, 255, 255);
    sf::Color cursorColor(255, 255, 0);
    
    // Configurar fuente por defecto del sistema
    sf::Font font;
    bool fontLoaded = false;
    
    // Intentar cargar fuentes del sistema macOS
    std::vector<std::string> fontPaths = {
        "/System/Library/Fonts/Monaco.ttf",
        "/System/Library/Fonts/Menlo.ttc", 
        "/System/Library/Fonts/Courier New.ttf",
        "/System/Library/Fonts/Helvetica.ttc",
        "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", // Linux backup
        "/Windows/Fonts/consola.ttf" // Windows backup
    };
    
    for (const auto& path : fontPaths) {
        if (font.openFromFile(path)) {
            fontLoaded = true;
            std::cout << "Fuente cargada: " << path << std::endl;
            break;
        }
    }
    
    if (!fontLoaded) {
        std::cout << "ADVERTENCIA: No se pudo cargar ninguna fuente. El texto puede no ser visible." << std::endl;
        std::cout << "Pero puedes seguir escribiendo - verás el output en la consola." << std::endl;
    }
    
    // Configurar texto
    std::vector<sf::Text> textObjects;
    
    // Cursor visual
    sf::RectangleShape cursor(sf::Vector2f(2.0f, 20.0f));
    cursor.setFillColor(cursorColor);
    
    std::cout << "Editor iniciado. Instrucciones:" << std::endl;
    std::cout << "- Escribe normalmente" << std::endl;
    std::cout << "- Enter para nueva línea" << std::endl;
    std::cout << "- Backspace para borrar" << std::endl;
    std::cout << "- ESC para salir" << std::endl;
    
    while (window.isOpen()) {
        // Manejar eventos
        while (auto event = window.pollEvent()) {
            if (auto* closeEvent = event->getIf<sf::Event::Closed>()) {
                window.close();
            }
            else if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
                else if (keyEvent->code == sf::Keyboard::Key::Enter) {
                    // Nueva línea
                    std::string currentText = lines[currentLine];
                    std::string remainingText = currentText.substr(currentCol);
                    lines[currentLine] = currentText.substr(0, currentCol);
                    lines.insert(lines.begin() + currentLine + 1, remainingText);
                    currentLine++;
                    currentCol = 0;
                    std::cout << "Nueva línea. Total líneas: " << lines.size() << std::endl;
                }
                else if (keyEvent->code == sf::Keyboard::Key::Backspace) {
                    // Borrar carácter
                    if (currentCol > 0) {
                        lines[currentLine].erase(currentCol - 1, 1);
                        currentCol--;
                    } else if (currentLine > 0) {
                        // Fusionar con línea anterior
                        currentCol = lines[currentLine - 1].length();
                        lines[currentLine - 1] += lines[currentLine];
                        lines.erase(lines.begin() + currentLine);
                        currentLine--;
                    }
                    std::cout << "Texto borrado. Línea actual: " << lines[currentLine] << std::endl;
                }
                else if (keyEvent->code == sf::Keyboard::Key::Left && currentCol > 0) {
                    currentCol--;
                }
                else if (keyEvent->code == sf::Keyboard::Key::Right && currentCol < lines[currentLine].length()) {
                    currentCol++;
                }
                else if (keyEvent->code == sf::Keyboard::Key::Up && currentLine > 0) {
                    currentLine--;
                    currentCol = std::min(currentCol, lines[currentLine].length());
                }
                else if (keyEvent->code == sf::Keyboard::Key::Down && currentLine < lines.size() - 1) {
                    currentLine++;
                    currentCol = std::min(currentCol, lines[currentLine].length());
                }
            }
            else if (auto* textEvent = event->getIf<sf::Event::TextEntered>()) {
                // Agregar carácter
                uint32_t unicode = textEvent->unicode;
                if (unicode >= 32 && unicode < 127) { // Solo ASCII imprimible
                    char c = static_cast<char>(unicode);
                    lines[currentLine].insert(currentCol, 1, c);
                    currentCol++;
                    std::cout << "Carácter agregado: '" << c << "'. Línea: " << lines[currentLine] << std::endl;
                }
            }
        }
        
        // Limpiar ventana
        window.clear(backgroundColor);
        
        // Preparar texto para renderizar
        textObjects.clear();
        
        for (size_t i = 0; i < lines.size() && i < 25; ++i) {
            if (!lines[i].empty()) {
                if (fontLoaded) {
                    try {
                        sf::Text text(font, lines[i], 16);
                        text.setPosition(sf::Vector2f(10.0f, 10.0f + i * 22.0f));
                        text.setFillColor(textColor);
                        textObjects.push_back(text);
                    } catch (const std::exception& e) {
                        std::cout << "Error renderizando línea " << i << ": " << e.what() << std::endl;
                        std::cout << "Contenido: " << lines[i] << std::endl;
                    }
                } else {
                    // Fallback: mostrar rectángulos como caracteres
                    for (size_t j = 0; j < lines[i].length(); ++j) {
                        sf::RectangleShape charRect(sf::Vector2f(8.0f, 16.0f));
                        charRect.setPosition(sf::Vector2f(10.0f + j * 10.0f, 10.0f + i * 22.0f));
                        charRect.setFillColor(sf::Color(100, 255, 100)); // Verde para indicar que hay texto
                        charRect.setOutlineThickness(1);
                        charRect.setOutlineColor(textColor);
                        // No podemos agregar rectangles a textObjects, necesitamos renderizarlos directamente
                    }
                    std::cout << "Línea " << i << " (sin fuente): " << lines[i] << std::endl;
                }
            }
        }
        
        // Renderizar texto
        for (const auto& text : textObjects) {
            window.draw(text);
        }
        
        // Mostrar cursor
        cursor.setPosition(sf::Vector2f(10.0f + currentCol * 10.0f, 10.0f + currentLine * 22.0f));
        window.draw(cursor);
        
        // Mostrar todo
        window.display();
    }
    
    // Mostrar contenido final
    std::cout << "\n=== Contenido final del editor ===" << std::endl;
    for (size_t i = 0; i < lines.size(); ++i) {
        std::cout << "Línea " << i << ": " << lines[i] << std::endl;
    }
    
    return 0;
}