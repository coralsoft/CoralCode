#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cstring>
#include <deque>

// Función para verificar si una palabra es reservada
bool isKeyword(const std::string& word) {
    static const std::set<std::string> keywords = {
        // C++
        "if", "else", "for", "while", "do", "switch", "case", "break", "continue",
        "return", "class", "struct", "public", "private", "protected", "virtual",
        "int", "float", "double", "char", "bool", "void", "const", "static",
        "auto", "template", "namespace", "using", "typedef", "sizeof", "new", "delete",
        // C
        "printf", "scanf", "malloc", "free", "sizeof", "extern", "register", "volatile",
        // Java
        "package", "import", "extends", "implements", "interface", "final", "abstract",
        // JavaScript
        "var", "let", "const", "function", "arrow", "this", "prototype", "typeof",
        // Python
        "def", "lambda", "import", "from", "as", "pass", "self", "None", "True", "False",
        // C#
        "readonly", "override", "partial", "sealed", "abstract", "interface"
    };
    return keywords.find(word) != keywords.end();
}

// Función para obtener color de una palabra
sf::Color getWordColor(const std::string& word) {
    if (isKeyword(word)) {
        return sf::Color(100, 150, 255); // Azul para keywords
    }
    return sf::Color(255, 255, 255); // Blanco para texto normal
}

// Función para procesar una línea y obtener palabras con colores
std::vector<std::pair<std::string, sf::Color>> processLine(const std::string& line) {
    std::vector<std::pair<std::string, sf::Color>> result;
    std::string currentWord;
    
    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        
        // Verificar si es un comentario
        if (c == '/' && i + 1 < line.length() && line[i + 1] == '/') {
            // Si tenemos una palabra pendiente, agregarla
            if (!currentWord.empty()) {
                result.push_back({currentWord, getWordColor(currentWord)});
                currentWord.clear();
            }
            // Agregar el resto de la línea como comentario
            std::string comment = line.substr(i);
            result.push_back({comment, sf::Color(100, 200, 100)}); // Verde para comentarios
            break;
        }
        
        // Verificar si es un string
        if (c == '"') {
            // Si tenemos una palabra pendiente, agregarla
            if (!currentWord.empty()) {
                result.push_back({currentWord, getWordColor(currentWord)});
                currentWord.clear();
            }
            
            // Buscar el final del string
            std::string stringLiteral = "\"";
            bool foundClosing = false;
            size_t j;
            for (j = i + 1; j < line.length(); ++j) {
                stringLiteral += line[j];
                if (line[j] == '"' && (j == 0 || line[j-1] != '\\')) {
                    foundClosing = true;
                    i = j;
                    break;
                }
            }
            
            // Si no encontramos comilla de cierre, solo tomar el resto de la línea
            if (!foundClosing) {
                stringLiteral = line.substr(i); // Tomar desde la comilla hasta el final
                i = line.length() - 1; // Mover el índice al final
            }
            
            result.push_back({stringLiteral, sf::Color(255, 200, 100)}); // Naranja para strings
            continue;
        }
        
        // Si es letra, número o underscore, es parte de una palabra
        if (std::isalnum(c) || c == '_') {
            currentWord += c;
        } else {
            // Si tenemos una palabra, procesarla
            if (!currentWord.empty()) {
                result.push_back({currentWord, getWordColor(currentWord)});
                currentWord.clear();
            }
            // Agregar el carácter especial
            if (c != ' ' && c != '\t') {
                result.push_back({std::string(1, c), sf::Color(255, 255, 255)});
            } else {
                result.push_back({std::string(1, c), sf::Color(255, 255, 255)});
            }
        }
    }
    
    // Agregar última palabra si existe
    if (!currentWord.empty()) {
        result.push_back({currentWord, getWordColor(currentWord)});
    }
    
    return result;
}

// Funciones de clipboard real para macOS
std::string getClipboard() {
    std::string result;
    FILE* pipe = popen("pbpaste", "r");
    if (pipe) {
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
        pclose(pipe);
    }
    return result;
}

void setClipboard(const std::string& text) {
    FILE* pipe = popen("pbcopy", "w");
    if (pipe) {
        fputs(text.c_str(), pipe);
        pclose(pipe);
        std::cout << "📋 Texto copiado al clipboard del sistema: \"" << text.substr(0, 30) << "...\"" << std::endl;
    }
}

// Función para validar y corregir el estado del editor
void validateEditorState(std::vector<std::string>& lines, size_t& cursorLine, size_t& cursorCol) {
    // Asegurar que siempre hay al menos una línea
    if (lines.empty()) {
        lines.push_back("");
    }
    
    // Validar posición del cursor
    if (cursorLine >= lines.size()) {
        cursorLine = lines.size() - 1;
    }
    if (cursorCol > lines[cursorLine].length()) {
        cursorCol = lines[cursorLine].length();
    }
    
    // NO eliminar líneas - eso puede causar pérdida de contenido
    // Solo validar que el estado sea consistente
}

// Función helper para calcular posiciones correctas de selección
void getSelectionBounds(size_t selStartLine, size_t selStartCol, size_t selEndLine, size_t selEndCol,
                       size_t& minLine, size_t& maxLine, size_t& startCol, size_t& endCol) {
    minLine = std::min(selStartLine, selEndLine);
    maxLine = std::max(selStartLine, selEndLine);
    
    if (selStartLine < selEndLine) {
        // Selección multi-línea hacia abajo
        startCol = selStartCol;
        endCol = selEndCol;
    } else if (selStartLine > selEndLine) {
        // Selección multi-línea hacia arriba
        startCol = selEndCol;
        endCol = selStartCol;
    } else {
        // Selección en la misma línea
        startCol = std::min(selStartCol, selEndCol);
        endCol = std::max(selStartCol, selEndCol);
    }
}

// Función helper para calcular líneas visibles de manera consistente
size_t calculateVisibleLines(const sf::Vector2u& windowSize) {
    float statusBarHeight = 25.0f;
    float scrollBarHeight = 15.0f;
    float textAreaHeight = std::max(0.0f, static_cast<float>(windowSize.y) - statusBarHeight - scrollBarHeight);
    return textAreaHeight > 24.0f ? static_cast<size_t>(textAreaHeight / 24.0f) : 0;
}

// Estructura para el historial de undo/redo
struct EditorState {
    std::vector<std::string> lines;
    size_t cursorLine;
    size_t cursorCol;
    std::string description;
    
    EditorState(const std::vector<std::string>& l, size_t cl, size_t cc, const std::string& desc)
        : lines(l), cursorLine(cl), cursorCol(cc), description(desc) {}
};

// Variables globales para undo/redo
std::deque<EditorState> undoHistory;
std::deque<EditorState> redoHistory;
const size_t MAX_HISTORY = 100;

// Función para guardar estado actual
void saveState(const std::vector<std::string>& lines, size_t cursorLine, size_t cursorCol, const std::string& description) {
    // Validar que el estado sea válido antes de guardarlo
    if (lines.empty()) return;
    
    // Limpiar redo history cuando se hace un nuevo cambio
    redoHistory.clear();
    
    // Asegurar que cursor está en límites válidos antes de guardar
    size_t validCursorLine = cursorLine;
    size_t validCursorCol = cursorCol;
    
    if (validCursorLine >= lines.size()) {
        validCursorLine = lines.size() - 1;
    }
    if (validCursorCol > lines[validCursorLine].length()) {
        validCursorCol = lines[validCursorLine].length();
    }
    
    // Agregar estado actual al historial de undo con cursor válido
    undoHistory.push_back(EditorState(lines, validCursorLine, validCursorCol, description));
    
    // Mantener límite de historial
    if (undoHistory.size() > MAX_HISTORY) {
        undoHistory.pop_front();
    }
}

// Función para deshacer (undo)
bool undo(std::vector<std::string>& lines, size_t& cursorLine, size_t& cursorCol) {
    if (undoHistory.empty()) {
        std::cout << "↶ No hay más cambios para deshacer" << std::endl;
        return false;
    }
    
    // Guardar estado actual en redo
    redoHistory.push_back(EditorState(lines, cursorLine, cursorCol, "Redo"));
    
    // Restaurar estado anterior
    const EditorState& previousState = undoHistory.back();
    lines = previousState.lines;
    cursorLine = previousState.cursorLine;
    cursorCol = previousState.cursorCol;
    
    // Validar límites del cursor después de restaurar
    if (cursorLine >= lines.size()) {
        cursorLine = lines.size() > 0 ? lines.size() - 1 : 0;
    }
    if (lines.size() > 0 && cursorCol > lines[cursorLine].length()) {
        cursorCol = lines[cursorLine].length();
    }
    
    undoHistory.pop_back();
    
    std::cout << "↶ Undo: " << previousState.description << std::endl;
    return true;
}

// Función para rehacer (redo)
bool redo(std::vector<std::string>& lines, size_t& cursorLine, size_t& cursorCol) {
    if (redoHistory.empty()) {
        std::cout << "↷ No hay más cambios para rehacer" << std::endl;
        return false;
    }
    
    // Guardar estado actual en undo
    undoHistory.push_back(EditorState(lines, cursorLine, cursorCol, "Undo"));
    
    // Restaurar estado siguiente
    const EditorState& nextState = redoHistory.back();
    lines = nextState.lines;
    cursorLine = nextState.cursorLine;
    cursorCol = nextState.cursorCol;
    
    // Validar límites del cursor después de restaurar
    if (cursorLine >= lines.size()) {
        cursorLine = lines.size() > 0 ? lines.size() - 1 : 0;
    }
    if (lines.size() > 0 && cursorCol > lines[cursorLine].length()) {
        cursorCol = lines[cursorLine].length();
    }
    
    redoHistory.pop_back();
    
    std::cout << "↷ Redo: " << nextState.description << std::endl;
    return true;
}

int main() {
    // Crear ventana redimensionable
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1000, 700)), "CoralCode - Editor con Syntax Highlighting", sf::Style::Default);
    window.setFramerateLimit(60);
    
    // Configurar vista inicial para mapeo 1:1 de pixels
    sf::View initialView;
    initialView.setSize(sf::Vector2f(1000.0f, 700.0f));
    initialView.setCenter(sf::Vector2f(500.0f, 350.0f));
    window.setView(initialView);
    
    // Variables del editor - método que funciona perfecto
    std::vector<std::string> lines = {""};
    size_t currentLine = 0;
    size_t currentCol = 0;
    
    // Guardar estado inicial
    saveState(lines, currentLine, currentCol, "Estado inicial");
    
    // Variables para selección
    bool isSelecting = false;
    size_t selectionStartLine = 0;
    size_t selectionStartCol = 0;
    size_t selectionEndLine = 0;
    size_t selectionEndCol = 0;
    
    // Variables para scroll y tamaño dinámico
    size_t scrollLine = 0;  // Primera línea visible
    size_t scrollCol = 0;   // Primera columna visible (scroll horizontal)
    sf::Vector2u windowSize = window.getSize();
    
    // Variables para barra de scroll
    bool isScrolling = false;  // Si el usuario está arrastrando la barra vertical
    bool isScrollingHorizontal = false;  // Si el usuario está arrastrando la barra horizontal
    float scrollBarWidth = 15.0f;  // Ancho de la barra de scroll vertical
    float scrollBarHeight = 15.0f;  // Alto de la barra de scroll horizontal
    
    // Colores
    sf::Color backgroundColor(25, 25, 25);  // Más oscuro
    sf::Color cursorColor(255, 255, 0);     // Amarillo
    sf::Color lineIndicatorColor(255, 50, 50); // Rojo
    sf::Color lineNumberColor(100, 100, 100); // Gris para números de línea
    sf::Color selectionColor(70, 130, 180, 100); // Azul translúcido para selección
    sf::Color statusBarColor(40, 40, 40);   // Gris oscuro para barra de estado
    sf::Color scrollBarColor(60, 60, 60);   // Gris para barra de scroll
    sf::Color scrollThumbColor(120, 120, 120); // Gris claro para el thumb
    
    // Cargar fuente del sistema
    sf::Font font;
    bool fontLoaded = false;
    
    std::vector<std::string> fontPaths = {
        "/System/Library/Fonts/Monaco.ttf",
        "/System/Library/Fonts/Menlo.ttc", 
        "/System/Library/Fonts/Courier New.ttf",
        "/System/Library/Fonts/Helvetica.ttc"
    };
    
    for (const auto& path : fontPaths) {
        if (font.openFromFile(path)) {
            fontLoaded = true;
            std::cout << "✅ Fuente cargada: " << path << std::endl;
            break;
        }
    }
    
    if (!fontLoaded) {
        std::cout << "❌ No se pudo cargar fuente. Texto puede no ser visible." << std::endl;
    }
    
    // Elementos visuales
    sf::RectangleShape cursor(sf::Vector2f(2.0f, 20.0f));
    cursor.setFillColor(cursorColor);
    
    // Indicador de línea actual (más delgado y a la izquierda)
    sf::RectangleShape lineIndicator(sf::Vector2f(2.0f, 20.0f));
    lineIndicator.setFillColor(lineIndicatorColor);
    
    // Barra de estado (dinámico)
    sf::RectangleShape statusBar;
    statusBar.setFillColor(statusBarColor);
    
    // Área de números de línea (dinámico)
    sf::RectangleShape lineNumberArea;
    lineNumberArea.setFillColor(sf::Color(35, 35, 35));
    
    // Barra de scroll
    sf::RectangleShape scrollBar;
    scrollBar.setFillColor(scrollBarColor);
    
    // Thumb de la barra de scroll vertical
    sf::RectangleShape scrollThumb;
    scrollThumb.setFillColor(scrollThumbColor);
    
    // Barra de scroll horizontal
    sf::RectangleShape horizontalScrollBar;
    horizontalScrollBar.setFillColor(scrollBarColor);
    
    // Thumb de la barra de scroll horizontal
    sf::RectangleShape horizontalScrollThumb;
    horizontalScrollThumb.setFillColor(scrollThumbColor);
    
    // Variables de layout
    const float lineNumberWidth = 50.0f;
    const float textStartX = lineNumberWidth + 10.0f;
    
    std::cout << "🚀 CoralCode - Editor Profesional Iniciado" << std::endl;
    std::cout << "📝 Escribe código en C++, C, Java, JavaScript, Python, C#" << std::endl;
    std::cout << "🎨 Syntax highlighting: azul=keywords, verde=comentarios, naranja=strings" << std::endl;
    std::cout << "🔢 Números de línea automáticos" << std::endl;
    std::cout << "📊 Barra de estado con información detallada" << std::endl;
    std::cout << "🖱️  Click y arrastra para seleccionar texto" << std::endl;
    std::cout << "📋 Cmd+C para copiar, Cmd+V para pegar (clipboard del sistema)" << std::endl;
    std::cout << "↶ Cmd+Z para deshacer, Cmd+Shift+Z para rehacer (límite: 100 cambios)" << std::endl;
    std::cout << "🔄 Scroll: Rueda vertical (up/down), Shift+Rueda horizontal, Trackpad horizontal" << std::endl;
    std::cout << "⚡ Ctrl/Cmd+Flechas: ↑↓ scroll 10 líneas, ←→ inicio/fin de línea" << std::endl;
    std::cout << "⌨️  ESC para salir" << std::endl;
    
    while (window.isOpen()) {
        // Verificar posición del mouse y ajustar cursor (solo cuando cambie)
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        float mouseX = static_cast<float>(mousePos.x);
        float mouseY = static_cast<float>(mousePos.y);
        float scrollBarX = static_cast<float>(windowSize.x) - scrollBarWidth;
        
        // Determinar qué tipo de cursor usar
        float horizontalScrollY = static_cast<float>(windowSize.y) - 25 - scrollBarHeight;
        
        sf::Cursor::Type newCursorType;
        if (mouseX > textStartX && mouseX < scrollBarX && 
            mouseY > 0 && mouseY < horizontalScrollY) {
            // En área de texto - cursor de texto
            newCursorType = sf::Cursor::Type::Text;
        } else if (mouseX > scrollBarX || 
                  (mouseY > horizontalScrollY && mouseY < windowSize.y - 25)) {
            // En barras de scroll - cursor de mano
            newCursorType = sf::Cursor::Type::Hand;
        } else {
            // En otras áreas - cursor normal
            newCursorType = sf::Cursor::Type::Arrow;
        }
        
        // Cambiar cursor solo si es diferente al actual
        static sf::Cursor::Type currentCursorType = sf::Cursor::Type::Arrow;
        if (newCursorType != currentCursorType) {
            auto cursor = sf::Cursor::createFromSystem(newCursorType);
            if (cursor) {
                window.setMouseCursor(cursor.value());
                currentCursorType = newCursorType;
            }
        }
        
        // Manejar eventos - LÓGICA QUE FUNCIONA PERFECTO
        while (auto event = window.pollEvent()) {
            if (auto* closeEvent = event->getIf<sf::Event::Closed>()) {
                window.close();
            }
            else if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
                else if (keyEvent->code == sf::Keyboard::Key::Enter) {
                    // Guardar estado antes de modificar
                    saveState(lines, currentLine, currentCol, "Nueva línea");
                    
                    // Nueva línea - método que funciona
                    std::string currentText = lines[currentLine];
                    std::string remainingText = currentText.substr(currentCol);
                    lines[currentLine] = currentText.substr(0, currentCol);
                    lines.insert(lines.begin() + currentLine + 1, remainingText);
                    currentLine++;
                    currentCol = 0;
                    
                    // Auto-scroll si la nueva línea no es visible
                    size_t visibleLines = calculateVisibleLines(windowSize);
                    if (visibleLines > 0 && currentLine >= scrollLine + visibleLines) {
                        scrollLine = currentLine - visibleLines + 1;
                    }
                }
                else if (keyEvent->code == sf::Keyboard::Key::Backspace) {
                    // Guardar estado antes de modificar
                    saveState(lines, currentLine, currentCol, "Borrar");
                    
                    // Si hay selección, borrar todo lo seleccionado
                    if (isSelecting) {
                        // Borrar texto seleccionado
                        size_t startLine, endLine, startCol, endCol;
                        getSelectionBounds(selectionStartLine, selectionStartCol, selectionEndLine, selectionEndCol,
                                         startLine, endLine, startCol, endCol);
                        
                        if (startLine == endLine) {
                            // Selección en una sola línea
                            lines[startLine].erase(startCol, endCol - startCol);
                        } else {
                            // Selección multi-línea
                            std::string remainingText = lines[endLine].substr(endCol);
                            lines[startLine] = lines[startLine].substr(0, startCol) + remainingText;
                            lines.erase(lines.begin() + startLine + 1, lines.begin() + endLine + 1);
                        }
                        
                        // Posicionar cursor al inicio de la selección
                        currentLine = startLine;
                        currentCol = startCol;
                        isSelecting = false;
                    } else {
                        // Borrar carácter normal
                        if (currentCol > 0) {
                            lines[currentLine].erase(currentCol - 1, 1);
                            currentCol--;
                        } else if (currentLine > 0) {
                            currentCol = lines[currentLine - 1].length();
                            lines[currentLine - 1] += lines[currentLine];
                            lines.erase(lines.begin() + currentLine);
                            currentLine--;
                        }
                    }
                }
                else if (keyEvent->code == sf::Keyboard::Key::Left) {
                    // Detectar si se presiona Ctrl (Windows/Linux) o Cmd (Mac)
                    bool isCtrlCmd = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || 
                                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl) ||
                                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem) || 
                                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RSystem);
                    
                    if (isCtrlCmd) {
                        // Ir al inicio de la línea
                        currentCol = 0;
                        scrollCol = 0; // Resetear scroll horizontal
                    } else if (currentCol > 0) {
                        // Navegación normal
                        currentCol--;
                        
                        // Auto-scroll horizontal hacia la izquierda si es necesario
                        if (currentCol < scrollCol) {
                            scrollCol = currentCol;
                        }
                    }
                }
                else if (keyEvent->code == sf::Keyboard::Key::Right) {
                    // Detectar si se presiona Ctrl (Windows/Linux) o Cmd (Mac)
                    bool isCtrlCmd = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || 
                                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl) ||
                                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem) || 
                                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RSystem);
                    
                    if (isCtrlCmd) {
                        // Ir al final de la línea
                        currentCol = lines[currentLine].length();
                        
                        // Auto-scroll horizontal hacia la derecha si es necesario
                        float textStartX = 55.0f; // Después del área de números de línea
                        size_t visibleCols = static_cast<size_t>((windowSize.x - textStartX - scrollBarWidth) / 9.6f);
                        if (currentCol >= scrollCol + visibleCols) {
                            scrollCol = currentCol >= visibleCols ? currentCol - visibleCols + 1 : 0;
                        }
                    } else if (currentCol < lines[currentLine].length()) {
                        // Navegación normal
                        currentCol++;
                        
                        // Auto-scroll horizontal hacia la derecha si es necesario
                        float textStartX = 55.0f; // Después del área de números de línea
                        size_t visibleCols = static_cast<size_t>((windowSize.x - textStartX - scrollBarWidth) / 9.6f);
                        if (currentCol >= scrollCol + visibleCols) {
                            scrollCol = currentCol - visibleCols + 1;
                        }
                    }
                }
                else if (keyEvent->code == sf::Keyboard::Key::Up) {
                    // Detectar si se presiona Ctrl (Windows/Linux) o Cmd (Mac)
                    bool isCtrlCmd = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || 
                                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl) ||
                                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem) || 
                                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RSystem);
                    
                    if (isCtrlCmd) {
                        // Scroll rápido de 10 líneas hacia arriba
                        if (scrollLine >= 10) {
                            scrollLine -= 10;
                        } else {
                            scrollLine = 0;
                        }
                        // Mover cursor al inicio del scroll visible
                        currentLine = scrollLine;
                        currentCol = std::min(currentCol, lines[currentLine].length());
                    } else if (currentLine > 0) {
                        // Navegación normal
                        currentLine--;
                        currentCol = std::min(currentCol, lines[currentLine].length());
                        
                        // Auto-scroll hacia arriba si es necesario
                        if (currentLine < scrollLine) {
                            scrollLine = currentLine;
                        }
                    }
                }
                else if (keyEvent->code == sf::Keyboard::Key::Down) {
                    // Detectar si se presiona Ctrl (Windows/Linux) o Cmd (Mac)
                    bool isCtrlCmd = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || 
                                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl) ||
                                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem) || 
                                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RSystem);
                    
                    if (isCtrlCmd) {
                        // Scroll rápido de 10 líneas hacia abajo
                        size_t visibleLines = calculateVisibleLines(windowSize);
                        if (visibleLines > 0) {
                            scrollLine += 10;
                            if (scrollLine + visibleLines > lines.size()) {
                                scrollLine = lines.size() > visibleLines ? lines.size() - visibleLines : 0;
                            }
                            // Mover cursor al inicio del scroll visible
                            currentLine = scrollLine;
                            currentCol = std::min(currentCol, lines[currentLine].length());
                        }
                    } else if (currentLine < lines.size() - 1) {
                        // Navegación normal
                        currentLine++;
                        currentCol = std::min(currentCol, lines[currentLine].length());
                        
                        // Auto-scroll hacia abajo si es necesario
                        size_t visibleLines = calculateVisibleLines(windowSize);
                        if (visibleLines > 0 && currentLine >= scrollLine + visibleLines) {
                            scrollLine = currentLine - visibleLines + 1;
                        }
                    }
                }
                else if (keyEvent->code == sf::Keyboard::Key::PageUp) {
                    // Scroll rápido hacia arriba
                    size_t visibleLines = calculateVisibleLines(windowSize);
                    if (visibleLines > 0) {
                        if (scrollLine >= visibleLines) {
                            scrollLine -= visibleLines;
                        } else {
                            scrollLine = 0;
                        }
                        currentLine = scrollLine;
                        currentCol = 0;
                    }
                }
                else if (keyEvent->code == sf::Keyboard::Key::PageDown) {
                    // Scroll rápido hacia abajo
                    size_t visibleLines = calculateVisibleLines(windowSize);
                    if (visibleLines > 0) {
                        scrollLine += visibleLines;
                        if (scrollLine >= lines.size()) {
                            scrollLine = lines.size() > visibleLines ? lines.size() - visibleLines : 0;
                        }
                        currentLine = std::min(scrollLine + visibleLines - 1, lines.size() - 1);
                        currentCol = 0;
                    }
                }
                // Scroll horizontal con Shift+Flecha
                else if (keyEvent->code == sf::Keyboard::Key::Left && 
                        (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || 
                         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift))) {
                    // Scroll horizontal hacia la izquierda
                    if (scrollCol > 0) {
                        scrollCol = scrollCol > 10 ? scrollCol - 10 : 0;
                    }
                }
                else if (keyEvent->code == sf::Keyboard::Key::Right && 
                        (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || 
                         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift))) {
                    // Scroll horizontal hacia la derecha
                    scrollCol += 10;
                }
                else if (keyEvent->code == sf::Keyboard::Key::Delete) {
                    // Guardar estado antes de modificar
                    saveState(lines, currentLine, currentCol, "Borrar");
                    
                    // Si hay selección, borrar todo lo seleccionado (igual que Backspace)
                    if (isSelecting) {
                        // Borrar texto seleccionado
                        size_t startLine, endLine, startCol, endCol;
                        getSelectionBounds(selectionStartLine, selectionStartCol, selectionEndLine, selectionEndCol,
                                         startLine, endLine, startCol, endCol);
                        
                        if (startLine == endLine) {
                            // Selección en una sola línea
                            lines[startLine].erase(startCol, endCol - startCol);
                        } else {
                            // Selección multi-línea
                            std::string remainingText = lines[endLine].substr(endCol);
                            lines[startLine] = lines[startLine].substr(0, startCol) + remainingText;
                            lines.erase(lines.begin() + startLine + 1, lines.begin() + endLine + 1);
                        }
                        
                        // Posicionar cursor al inicio de la selección
                        currentLine = startLine;
                        currentCol = startCol;
                        isSelecting = false;
                    } else {
                        // Borrar carácter siguiente
                        if (currentCol < lines[currentLine].length()) {
                            lines[currentLine].erase(currentCol, 1);
                        } else if (currentLine < lines.size() - 1) {
                            // Fusionar con línea siguiente
                            lines[currentLine] += lines[currentLine + 1];
                            lines.erase(lines.begin() + currentLine + 1);
                        }
                    }
                }
                // Cmd+C para copiar (Mac)
                else if (keyEvent->code == sf::Keyboard::Key::C && 
                        (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem) || 
                         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RSystem))) {
                    if (isSelecting) {
                        // Copiar texto seleccionado
                        std::string selectedText = "";
                        size_t startLine, endLine, startCol, endCol;
                        getSelectionBounds(selectionStartLine, selectionStartCol, selectionEndLine, selectionEndCol,
                                         startLine, endLine, startCol, endCol);
                        
                        if (startLine == endLine) {
                            selectedText = lines[startLine].substr(startCol, endCol - startCol);
                        } else {
                            selectedText = lines[startLine].substr(startCol) + "\n";
                            for (size_t i = startLine + 1; i < endLine; ++i) {
                                selectedText += lines[i] + "\n";
                            }
                            selectedText += lines[endLine].substr(0, endCol);
                        }
                        setClipboard(selectedText);
                    }
                }
                // Cmd+Z para undo (Mac) o Ctrl+Z para undo (Windows)
                else if (keyEvent->code == sf::Keyboard::Key::Z && 
                        (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem) || 
                         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RSystem) ||
                         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || 
                         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl))) {
                    undo(lines, currentLine, currentCol);
                }
                // Cmd+Shift+Z para redo (Mac) o Ctrl+Y para redo (Windows)
                else if ((keyEvent->code == sf::Keyboard::Key::Z && 
                         (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem) || 
                          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RSystem)) &&
                         (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || 
                          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift))) ||
                        (keyEvent->code == sf::Keyboard::Key::Y && 
                         (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || 
                          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl)))) {
                    redo(lines, currentLine, currentCol);
                }
                // Cmd+V para pegar (Mac)
                else if (keyEvent->code == sf::Keyboard::Key::V && 
                        (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem) || 
                         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RSystem))) {
                    std::string clipboardText = getClipboard();
                    if (!clipboardText.empty()) {
                        // Guardar estado antes de pegar
                        saveState(lines, currentLine, currentCol, "Pegar");
                        
                        // Insertar texto del clipboard
                        for (char c : clipboardText) {
                            if (c == '\n') {
                                // Nueva línea
                                std::string currentText = lines[currentLine];
                                std::string remainingText = currentText.substr(currentCol);
                                lines[currentLine] = currentText.substr(0, currentCol);
                                lines.insert(lines.begin() + currentLine + 1, remainingText);
                                currentLine++;
                                currentCol = 0;
                            } else if (c >= 32 && c < 127) {
                                lines[currentLine].insert(currentCol, 1, c);
                                currentCol++;
                            }
                        }
                    }
                }
            }
            else if (auto* textEvent = event->getIf<sf::Event::TextEntered>()) {
                // Agregar carácter - método que funciona perfecto
                uint32_t unicode = textEvent->unicode;
                if (unicode >= 32 && unicode < 127) {
                    char c = static_cast<char>(unicode);
                    
                    // Guardar estado antes de modificar
                    saveState(lines, currentLine, currentCol, "Escribir");
                    
                    // Si hay selección, reemplazarla con el nuevo carácter
                    if (isSelecting) {
                        // Borrar texto seleccionado primero
                        size_t startLine, endLine, startCol, endCol;
                        getSelectionBounds(selectionStartLine, selectionStartCol, selectionEndLine, selectionEndCol,
                                         startLine, endLine, startCol, endCol);
                        
                        if (startLine == endLine) {
                            // Selección en una sola línea
                            lines[startLine].erase(startCol, endCol - startCol);
                        } else {
                            // Selección multi-línea
                            std::string remainingText = lines[endLine].substr(endCol);
                            lines[startLine] = lines[startLine].substr(0, startCol) + remainingText;
                            lines.erase(lines.begin() + startLine + 1, lines.begin() + endLine + 1);
                        }
                        
                        // Posicionar cursor al inicio de la selección
                        currentLine = startLine;
                        currentCol = startCol;
                        isSelecting = false;
                    }
                    
                    // Insertar el nuevo carácter
                    lines[currentLine].insert(currentCol, 1, c);
                    currentCol++;
                }
            }
            else if (auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    // Calcular posición del cursor basada en click del mouse
                    float mouseX = static_cast<float>(mouseEvent->position.x);
                    float mouseY = static_cast<float>(mouseEvent->position.y);
                    
                    // Verificar si click en barra de scroll vertical
                    float scrollBarX = static_cast<float>(windowSize.x) - scrollBarWidth;
                    float horizontalScrollY = static_cast<float>(windowSize.y) - 25 - scrollBarHeight;
                    
                    if (mouseX > scrollBarX && mouseY < windowSize.y - 25 - scrollBarHeight) {
                        // Click en barra de scroll vertical
                        isScrolling = true;
                        
                        // Calcular nueva posición de scroll basada en click
                        float scrollAreaHeight = windowSize.y - 50 - scrollBarHeight; // Sin barra de estado ni scroll horizontal
                        float clickRatio = mouseY / scrollAreaHeight;
                        size_t maxScroll = lines.size() > (windowSize.y - 50 - scrollBarHeight) / 24 ? 
                                         lines.size() - (windowSize.y - 50 - scrollBarHeight) / 24 : 0;
                        scrollLine = static_cast<size_t>(clickRatio * maxScroll);
                        
                        // Asegurar límites
                        if (scrollLine > maxScroll) scrollLine = maxScroll;
                    }
                    else if (mouseY > horizontalScrollY && mouseY < windowSize.y - 25 && 
                             mouseX > textStartX && mouseX < scrollBarX) {
                        // Click en barra de scroll horizontal
                        isScrollingHorizontal = true;
                        
                        // Calcular nueva posición de scroll horizontal basada en click
                        float scrollAreaWidth = scrollBarX - textStartX;
                        float clickRatio = (mouseX - textStartX) / scrollAreaWidth;
                        
                        // Encontrar la línea más larga para calcular el máximo scroll
                        size_t maxLineLength = 0;
                        for (const auto& line : lines) {
                            maxLineLength = std::max(maxLineLength, line.length());
                        }
                        
                        size_t visibleCols = static_cast<size_t>(scrollAreaWidth / 9.6f);
                        size_t maxScrollCol = maxLineLength > visibleCols ? maxLineLength - visibleCols : 0;
                        scrollCol = static_cast<size_t>(clickRatio * maxScrollCol);
                        
                        // Asegurar límites
                        if (scrollCol > maxScrollCol) scrollCol = maxScrollCol;
                    }
                    else if (mouseX > textStartX && mouseX < scrollBarX && mouseY < windowSize.y - 25 - scrollBarHeight) {
                        // Dentro del área de texto
                        size_t clickedLine = scrollLine + static_cast<size_t>((mouseY - 20.0f) / 24.0f);
                        size_t clickedCol = scrollCol + static_cast<size_t>((mouseX - textStartX) / 9.6f);
                        
                        if (clickedLine < lines.size()) {
                            currentLine = clickedLine;
                            currentCol = std::min(clickedCol, lines[currentLine].length());
                            
                            // Iniciar selección
                            isSelecting = true;
                            selectionStartLine = currentLine;
                            selectionStartCol = currentCol;
                            selectionEndLine = currentLine;
                            selectionEndCol = currentCol;
                        }
                    }
                }
            }
            else if (auto* mouseEvent = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    if (isScrolling) {
                        isScrolling = false;
                    }
                    else if (isScrollingHorizontal) {
                        isScrollingHorizontal = false;
                    }
                    else if (isSelecting) {
                        // Finalizar selección si no hay texto seleccionado
                        if (selectionStartLine == selectionEndLine && selectionStartCol == selectionEndCol) {
                            isSelecting = false;
                        }
                    }
                }
            }
            else if (auto* mouseEvent = event->getIf<sf::Event::MouseMoved>()) {
                if (isScrolling && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    // Arrastrar barra de scroll vertical
                    float mouseY = static_cast<float>(mouseEvent->position.y);
                    float scrollAreaHeight = windowSize.y - 50 - scrollBarHeight;
                    float dragRatio = mouseY / scrollAreaHeight;
                    size_t maxScroll = lines.size() > (windowSize.y - 50 - scrollBarHeight) / 24 ? 
                                     lines.size() - (windowSize.y - 50 - scrollBarHeight) / 24 : 0;
                    scrollLine = static_cast<size_t>(dragRatio * maxScroll);
                    
                    // Asegurar límites
                    if (scrollLine > maxScroll) scrollLine = maxScroll;
                }
                else if (isScrollingHorizontal && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    // Arrastrar barra de scroll horizontal
                    float mouseX = static_cast<float>(mouseEvent->position.x);
                    float scrollBarX = static_cast<float>(windowSize.x) - scrollBarWidth;
                    float scrollAreaWidth = scrollBarX - textStartX;
                    float dragRatio = (mouseX - textStartX) / scrollAreaWidth;
                    
                    // Encontrar la línea más larga para calcular el máximo scroll
                    size_t maxLineLength = 0;
                    for (const auto& line : lines) {
                        maxLineLength = std::max(maxLineLength, line.length());
                    }
                    
                    size_t visibleCols = static_cast<size_t>(scrollAreaWidth / 9.6f);
                    size_t maxScrollCol = maxLineLength > visibleCols ? maxLineLength - visibleCols : 0;
                    scrollCol = static_cast<size_t>(dragRatio * maxScrollCol);
                    
                    // Asegurar límites
                    if (scrollCol > maxScrollCol) scrollCol = maxScrollCol;
                }
                else if (isSelecting && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    // Actualizar selección mientras se arrastra
                    float mouseX = static_cast<float>(mouseEvent->position.x);
                    float mouseY = static_cast<float>(mouseEvent->position.y);
                    float scrollBarX = static_cast<float>(windowSize.x) - scrollBarWidth;
                    
                    if (mouseX > textStartX && mouseX < scrollBarX && mouseY < windowSize.y - 25 - scrollBarHeight) {
                        size_t dragLine = scrollLine + static_cast<size_t>((mouseY - 20.0f) / 24.0f);
                        size_t dragCol = scrollCol + static_cast<size_t>((mouseX - textStartX) / 9.6f);
                        
                        if (dragLine < lines.size()) {
                            selectionEndLine = dragLine;
                            selectionEndCol = std::min(dragCol, lines[dragLine].length());
                        }
                    }
                }
            }
            else if (auto* scrollEvent = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (scrollEvent->wheel == sf::Mouse::Wheel::Vertical) {
                    // Verificar si se mantiene presionado Shift para scroll horizontal
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || 
                        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)) {
                        // Scroll horizontal con Shift + rueda del mouse (lógica de Mac)
                        if (scrollEvent->delta > 0) {
                            // Rueda hacia arriba = scroll hacia la izquierda (Mac)
                            if (scrollCol > 0) {
                                scrollCol = scrollCol > 3 ? scrollCol - 3 : 0;
                            }
                        } else {
                            // Rueda hacia abajo = scroll hacia la derecha (Mac)
                            scrollCol += 3;
                        }
                    } else {
                        // Scroll vertical normal (lógica de Mac invertida)
                        if (scrollEvent->delta > 0) {
                            // Rueda hacia arriba = scroll hacia arriba (Mac)
                            if (scrollLine > 0) {
                                scrollLine--;
                            }
                        } else {
                            // Rueda hacia abajo = scroll hacia abajo (Mac)
                            size_t visibleLines = calculateVisibleLines(windowSize);
                            if (visibleLines > 0 && scrollLine + visibleLines < lines.size()) {
                                scrollLine++;
                            }
                        }
                    }
                }
                else if (scrollEvent->wheel == sf::Mouse::Wheel::Horizontal) {
                    // Scroll horizontal nativo del mouse (trackpad Mac)
                    if (scrollEvent->delta > 0) {
                        // Scroll hacia la derecha
                        scrollCol += 3;
                    } else {
                        // Scroll hacia la izquierda
                        if (scrollCol > 0) {
                            scrollCol = scrollCol > 3 ? scrollCol - 3 : 0;
                        }
                    }
                }
            }
            else if (auto* resizeEvent = event->getIf<sf::Event::Resized>()) {
                // Actualizar tamaño de ventana
                windowSize = sf::Vector2u(resizeEvent->size.x, resizeEvent->size.y);
                
                // Crear una vista que mapee 1:1 con los pixels de la ventana
                sf::View view;
                view.setSize(sf::Vector2f(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)));
                view.setCenter(sf::Vector2f(static_cast<float>(windowSize.x) / 2.0f, static_cast<float>(windowSize.y) / 2.0f));
                window.setView(view);
            }
        }
        
        // Renderizar
        window.clear(backgroundColor);
        
        // Obtener tamaño actual de ventana
        sf::Vector2u currentWindowSize = window.getSize();
        
        // Solo actualizar windowSize si realmente cambió
        if (currentWindowSize.x != windowSize.x || currentWindowSize.y != windowSize.y) {
            windowSize = currentWindowSize;
        }
        
        // Calcular posiciones una vez por frame
        float statusBarHeight = 25.0f;
        float statusBarY = static_cast<float>(windowSize.y) - statusBarHeight;
        float lineNumberWidth = 50.0f;
        
        // Configurar elementos con posiciones estables
        lineNumberArea.setSize(sf::Vector2f(lineNumberWidth, statusBarY));
        lineNumberArea.setPosition(sf::Vector2f(0.0f, 0.0f));
        
        statusBar.setSize(sf::Vector2f(static_cast<float>(windowSize.x), statusBarHeight));
        statusBar.setPosition(sf::Vector2f(0.0f, statusBarY));
        
        // Dibujar área de números de línea
        window.draw(lineNumberArea);
        
        // Dibujar barra de estado
        window.draw(statusBar);
        
        // Calcular áreas de trabajo con validaciones para ventanas pequeñas
        float textAreaHeight = std::max(0.0f, statusBarY - scrollBarHeight);
        float textAreaWidth = std::max(0.0f, static_cast<float>(windowSize.x) - lineNumberWidth - scrollBarWidth);
        size_t visibleLines = textAreaHeight > 24.0f ? static_cast<size_t>(textAreaHeight / 24.0f) : 0;
        
        // Dibujar barra de scroll vertical (solo si es necesario)
        if (lines.size() > visibleLines && visibleLines > 0) {
            float scrollBarX = static_cast<float>(windowSize.x) - scrollBarWidth;
            
            scrollBar.setSize(sf::Vector2f(scrollBarWidth, textAreaHeight));
            scrollBar.setPosition(sf::Vector2f(scrollBarX, 0.0f));
            window.draw(scrollBar);
            
            // Calcular y dibujar el thumb vertical
            if (lines.size() > visibleLines) {
                float maxScroll = static_cast<float>(lines.size() - visibleLines);
                float scrollRatio = maxScroll > 0 ? static_cast<float>(scrollLine) / maxScroll : 0.0f;
                float thumbHeight = (textAreaHeight * visibleLines) / lines.size();
                float thumbY = scrollRatio * (textAreaHeight - thumbHeight);
                
                scrollThumb.setSize(sf::Vector2f(scrollBarWidth - 2.0f, thumbHeight));
                scrollThumb.setPosition(sf::Vector2f(scrollBarX + 1.0f, thumbY));
                window.draw(scrollThumb);
            }
        }
        
        // Calcular si necesitamos barra de scroll horizontal
        size_t maxLineLength = 0;
        for (const auto& line : lines) {
            maxLineLength = std::max(maxLineLength, line.length());
        }
        
        size_t visibleCols = textAreaWidth > 9.6f ? static_cast<size_t>(textAreaWidth / 9.6f) : 0;
        if (maxLineLength > visibleCols && visibleCols > 0) {
            float horizontalScrollY = statusBarY - scrollBarHeight;
            
            horizontalScrollBar.setSize(sf::Vector2f(textAreaWidth, scrollBarHeight));
            horizontalScrollBar.setPosition(sf::Vector2f(lineNumberWidth, horizontalScrollY));
            window.draw(horizontalScrollBar);
            
            // Calcular y dibujar el thumb horizontal
            if (maxLineLength > visibleCols) {
                size_t maxScrollCol = maxLineLength - visibleCols;
                float scrollColRatio = maxScrollCol > 0 ? static_cast<float>(scrollCol) / static_cast<float>(maxScrollCol) : 0.0f;
                float thumbWidth = (textAreaWidth * visibleCols) / maxLineLength;
                float thumbX = lineNumberWidth + scrollColRatio * (textAreaWidth - thumbWidth);
                
                horizontalScrollThumb.setSize(sf::Vector2f(thumbWidth, scrollBarHeight - 2.0f));
                horizontalScrollThumb.setPosition(sf::Vector2f(thumbX, horizontalScrollY + 1.0f));
                window.draw(horizontalScrollThumb);
            }
        }
        
        // Mostrar texto con syntax highlighting
        if (fontLoaded) {
            float yPos = 20.0f;
            size_t linesToShow = std::min(lines.size() - scrollLine, visibleLines);
            
            for (size_t i = 0; i < linesToShow; ++i) {
                size_t actualLineNum = scrollLine + i;
                
                // Validar que la línea existe
                if (actualLineNum >= lines.size()) {
                    break;
                }
                
                // Dibujar número de línea
                sf::Text lineNumber(font, std::to_string(actualLineNum + 1), 14);
                lineNumber.setPosition(sf::Vector2f(5.0f, yPos));
                lineNumber.setFillColor(lineNumberColor);
                window.draw(lineNumber);
                
                // Dibujar selección si existe
                if (isSelecting) {
                    size_t selStart, selEnd, startCol, endCol;
                    getSelectionBounds(selectionStartLine, selectionStartCol, selectionEndLine, selectionEndCol,
                                     selStart, selEnd, startCol, endCol);
                    
                    if (actualLineNum >= selStart && actualLineNum <= selEnd) {
                        float selectionX = textStartX;
                        float selectionWidth = 0;
                        
                        if (actualLineNum == selStart && actualLineNum == selEnd) {
                            // Selección en una sola línea - ajustar por scroll horizontal
                            if (startCol >= scrollCol) {
                                selectionX += (startCol - scrollCol) * 9.6f;
                                size_t visibleStart = startCol - scrollCol;
                                size_t visibleEnd = endCol > scrollCol ? endCol - scrollCol : 0;
                                selectionWidth = (visibleEnd > visibleStart ? visibleEnd - visibleStart : 0) * 9.6f;
                            }
                        } else if (actualLineNum == selStart) {
                            // Primera línea de selección múltiple
                            if (startCol >= scrollCol) {
                                selectionX += (startCol - scrollCol) * 9.6f;
                                size_t visibleStart = startCol - scrollCol;
                                size_t lineLength = lines[actualLineNum].length();
                                size_t visibleLength = lineLength > scrollCol ? lineLength - scrollCol : 0;
                                selectionWidth = (visibleLength > visibleStart ? visibleLength - visibleStart : 0) * 9.6f;
                            }
                        } else if (actualLineNum == selEnd) {
                            // Última línea de selección múltiple
                            size_t visibleEnd = endCol > scrollCol ? endCol - scrollCol : 0;
                            selectionWidth = visibleEnd * 9.6f;
                        } else {
                            // Línea completa seleccionada
                            size_t lineLength = lines[actualLineNum].length();
                            selectionWidth = (lineLength > scrollCol ? lineLength - scrollCol : 0) * 9.6f;
                        }
                        
                        if (selectionWidth > 0) {
                            sf::RectangleShape selection(sf::Vector2f(selectionWidth, 20.0f));
                            selection.setPosition(sf::Vector2f(selectionX, yPos));
                            selection.setFillColor(selectionColor);
                            window.draw(selection);
                        }
                    }
                }
                
                float xPos = textStartX;
                
                // Limitar el ancho del texto para no superponerse con la barra de scroll
                float maxTextWidth = static_cast<float>(windowSize.x) - scrollBarWidth - textStartX - 10.0f;
                
                // Procesar línea para syntax highlighting con scroll horizontal
                const std::string& lineContent = lines[actualLineNum];
                
                // Aplicar scroll horizontal: extraer la parte visible de la línea
                std::string visibleContent;
                if (lineContent.length() > scrollCol) {
                    visibleContent = lineContent.substr(scrollCol);
                } else {
                    visibleContent = ""; // Línea más corta que el scroll
                }
                
                auto coloredWords = processLine(visibleContent);
                
                for (const auto& wordPair : coloredWords) {
                    sf::Text text(font, wordPair.first, 16);
                    text.setPosition(sf::Vector2f(xPos, yPos));
                    text.setFillColor(wordPair.second);
                    
                    // Solo dibujar si el texto está dentro del área visible
                    if (xPos < maxTextWidth) {
                        window.draw(text);
                    }
                    
                    // Calcular ancho aproximado del texto
                    xPos += wordPair.first.length() * 9.6f; // Ancho promedio por carácter
                    
                    // Si ya salimos del área visible, parar de renderizar
                    if (xPos > maxTextWidth) {
                        break;
                    }
                }
                
                yPos += 24.0f;
            }
        }
        
        // Mostrar indicador de línea actual (en el borde izquierdo) - solo si está visible
        if (currentLine >= scrollLine && currentLine < scrollLine + (windowSize.y - 50) / 24) {
            float indicatorY = 20.0f + (currentLine - scrollLine) * 24.0f;
            lineIndicator.setPosition(sf::Vector2f(1.0f, indicatorY));
            window.draw(lineIndicator);
        }
        
        // Mostrar cursor - solo si está visible en pantalla
        if (currentLine >= scrollLine && currentLine < scrollLine + (windowSize.y - 50) / 24) {
            // Verificar que el cursor esté visible horizontalmente también
            if (currentCol >= scrollCol) {
                float cursorX = textStartX + (currentCol - scrollCol) * 9.6f;
                float cursorY = 20.0f + (currentLine - scrollLine) * 24.0f;
                
                // Solo mostrar cursor si está dentro del área visible
                float maxTextWidth = static_cast<float>(windowSize.x) - scrollBarWidth - textStartX - 10.0f;
                if (cursorX < maxTextWidth) {
                    cursor.setPosition(sf::Vector2f(cursorX, cursorY));
                    window.draw(cursor);
                }
            }
        }
        
        // Crear información de la barra de estado
        if (fontLoaded) {
            std::stringstream statusInfo;
            statusInfo << "Línea: " << (currentLine + 1) 
                      << "  Columna: " << (currentCol + 1)
                      << "  Total líneas: " << lines.size()
                      << "  Caracteres: " << lines[currentLine].length();
            
            if (isSelecting) {
                // Calcular información de selección
                size_t selStart, selEnd, startCol, endCol;
                getSelectionBounds(selectionStartLine, selectionStartCol, selectionEndLine, selectionEndCol,
                                 selStart, selEnd, startCol, endCol);
                
                size_t selectedLines = selEnd - selStart + 1;
                size_t selectedChars = 0;
                
                if (selStart == selEnd) {
                    selectedChars = endCol - startCol;
                } else {
                    selectedChars += lines[selStart].length() - startCol + 1; // +1 for newline
                    for (size_t i = selStart + 1; i < selEnd; ++i) {
                        selectedChars += lines[i].length() + 1; // +1 for newline
                    }
                    selectedChars += endCol;
                }
                
                statusInfo << "  |  SELECCIÓN - Líneas: " << selectedLines 
                          << "  Caracteres: " << selectedChars;
            }
            
            statusInfo << "  |  Scroll H: " << scrollCol 
                      << "  |  Cmd+C: Copiar  Cmd+V: Pegar  Cmd+Z: Undo  Cmd+Shift+Z: Redo  ⚡: Ctrl+Flechas  🔄: Rueda/Trackpad";
            
            sf::Text statusText(font, statusInfo.str(), 12);
            // Posicionar texto dinámicamente en la barra de estado
            float statusTextY = static_cast<float>(windowSize.y - 25) + 5.0f; // 5px desde el borde superior de la barra
            statusText.setPosition(sf::Vector2f(10.0f, statusTextY));
            statusText.setFillColor(sf::Color(200, 200, 200));
            window.draw(statusText);
        }
        
        window.display();
    }
    
    // Mostrar contenido final
    std::cout << "\n📄 Contenido final del editor:" << std::endl;
    for (size_t i = 0; i < lines.size(); ++i) {
        std::cout << "Línea " << i << ": " << lines[i] << std::endl;
    }
    
    return 0;
}