#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

int main() {
    // Crear ventana
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "CoralCode - Editor Visual");
    window.setFramerateLimit(60);
    
    // Variables del editor
    std::vector<std::string> lines = {""};
    size_t currentLine = 0;
    size_t currentCol = 0;
    
    // Colores
    sf::Color backgroundColor(30, 30, 30);
    sf::Color textColor(255, 255, 255);
    sf::Color cursorColor(255, 255, 0);
    
    // Cursor visual
    sf::RectangleShape cursor(sf::Vector2f(2.0f, 20.0f));
    cursor.setFillColor(cursorColor);
    
    // Variables para mostrar texto como rectángulos
    std::vector<std::vector<sf::RectangleShape>> characterRects;
    
    std::cout << "=== EDITOR VISUAL INICIADO ===" << std::endl;
    std::cout << "Cada carácter aparecerá como un rectángulo blanco en la pantalla." << std::endl;
    std::cout << "Los espacios aparecen como rectángulos grises." << std::endl;
    std::cout << "Controles:" << std::endl;
    std::cout << "- Escribe normalmente (verás rectángulos)" << std::endl;
    std::cout << "- Enter: nueva línea" << std::endl;
    std::cout << "- Backspace: borrar" << std::endl;
    std::cout << "- Flechas: navegar" << std::endl;
    std::cout << "- ESC: salir" << std::endl;
    std::cout << "- El texto real se muestra en esta consola" << std::endl;
    std::cout << "================================" << std::endl;
    
    auto updateCharacterRects = [&]() {
        characterRects.clear();
        characterRects.resize(lines.size());
        
        for (size_t i = 0; i < lines.size(); ++i) {
            characterRects[i].clear();
            for (size_t j = 0; j < lines[i].length(); ++j) {
                sf::RectangleShape rect(sf::Vector2f(10.0f, 18.0f));
                rect.setPosition(sf::Vector2f(15.0f + j * 12.0f, 15.0f + i * 25.0f));
                
                char c = lines[i][j];
                if (c == ' ') {
                    rect.setFillColor(sf::Color(80, 80, 80)); // Espacios en gris
                } else if (c >= 'A' && c <= 'Z') {
                    rect.setFillColor(sf::Color(150, 200, 255)); // Mayúsculas en azul
                } else if (c >= 'a' && c <= 'z') {
                    rect.setFillColor(sf::Color(255, 255, 255)); // Minúsculas en blanco
                } else if (c >= '0' && c <= '9') {
                    rect.setFillColor(sf::Color(255, 200, 100)); // Números en amarillo
                } else {
                    rect.setFillColor(sf::Color(200, 100, 255)); // Símbolos en magenta
                }
                
                rect.setOutlineThickness(1);
                rect.setOutlineColor(sf::Color(100, 100, 100));
                characterRects[i].push_back(rect);
            }
        }
    };
    
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
                    updateCharacterRects();
                    std::cout << "\n--- NUEVA LÍNEA ---" << std::endl;
                    for (size_t i = 0; i < lines.size(); ++i) {
                        std::cout << "Línea " << i << ": [" << lines[i] << "]" << std::endl;
                    }
                }
                else if (keyEvent->code == sf::Keyboard::Key::Backspace) {
                    // Borrar carácter
                    if (currentCol > 0) {
                        char deleted = lines[currentLine][currentCol - 1];
                        lines[currentLine].erase(currentCol - 1, 1);
                        currentCol--;
                        std::cout << "BORRADO: '" << deleted << "' - Línea ahora: [" << lines[currentLine] << "]" << std::endl;
                    } else if (currentLine > 0) {
                        // Fusionar con línea anterior
                        currentCol = lines[currentLine - 1].length();
                        lines[currentLine - 1] += lines[currentLine];
                        lines.erase(lines.begin() + currentLine);
                        currentLine--;
                        std::cout << "LÍNEAS FUSIONADAS - Línea actual: [" << lines[currentLine] << "]" << std::endl;
                    }
                    updateCharacterRects();
                }
                else if (keyEvent->code == sf::Keyboard::Key::Left && currentCol > 0) {
                    currentCol--;
                    std::cout << "CURSOR ← posición: línea=" << currentLine << " col=" << currentCol << std::endl;
                }
                else if (keyEvent->code == sf::Keyboard::Key::Right && currentCol < lines[currentLine].length()) {
                    currentCol++;
                    std::cout << "CURSOR → posición: línea=" << currentLine << " col=" << currentCol << std::endl;
                }
                else if (keyEvent->code == sf::Keyboard::Key::Up && currentLine > 0) {
                    currentLine--;
                    currentCol = std::min(currentCol, lines[currentLine].length());
                    std::cout << "CURSOR ↑ posición: línea=" << currentLine << " col=" << currentCol << std::endl;
                }
                else if (keyEvent->code == sf::Keyboard::Key::Down && currentLine < lines.size() - 1) {
                    currentLine++;
                    currentCol = std::min(currentCol, lines[currentLine].length());
                    std::cout << "CURSOR ↓ posición: línea=" << currentLine << " col=" << currentCol << std::endl;
                }
            }
            else if (auto* textEvent = event->getIf<sf::Event::TextEntered>()) {
                // Agregar carácter
                uint32_t unicode = textEvent->unicode;
                if (unicode >= 32 && unicode < 127) { // Solo ASCII imprimible
                    char c = static_cast<char>(unicode);
                    lines[currentLine].insert(currentCol, 1, c);
                    currentCol++;
                    updateCharacterRects();
                    std::cout << "ESCRITO: '" << c << "' - Línea: [" << lines[currentLine] << "]" << std::endl;
                }
            }
        }
        
        // Renderizar
        window.clear(backgroundColor);
        
        // Mostrar todos los caracteres como rectángulos
        for (const auto& lineRects : characterRects) {
            for (const auto& rect : lineRects) {
                window.draw(rect);
            }
        }
        
        // Mostrar cursor
        cursor.setPosition(sf::Vector2f(15.0f + currentCol * 12.0f, 15.0f + currentLine * 25.0f));
        window.draw(cursor);
        
        // Mostrar indicador de línea actual
        sf::RectangleShape lineIndicator(sf::Vector2f(5.0f, 20.0f));
        lineIndicator.setPosition(sf::Vector2f(5.0f, 15.0f + currentLine * 25.0f));
        lineIndicator.setFillColor(sf::Color(255, 0, 0));
        window.draw(lineIndicator);
        
        window.display();
    }
    
    // Mostrar contenido final
    std::cout << "\n=== CONTENIDO FINAL DEL EDITOR ===" << std::endl;
    for (size_t i = 0; i < lines.size(); ++i) {
        std::cout << "Línea " << i << ": [" << lines[i] << "]" << std::endl;
    }
    std::cout << "Total de líneas: " << lines.size() << std::endl;
    
    return 0;
}