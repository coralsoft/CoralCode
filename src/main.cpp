/**
 * @file main.cpp
 * @brief Punto de entrada principal de CoralCode
 * 
 * Este archivo contiene el punto de entrada principal del editor CoralCode.
 * Se encarga de:
 * - Inicializar el sistema de logging
 * - Procesar argumentos de línea de comandos
 * - Crear e inicializar el editor
 * - Manejar excepciones globales
 */

#include "Editor.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <memory>

#ifdef CORALCODE_WINDOWS
    #include <windows.h>
    #include <io.h>
    #include <fcntl.h>
#endif

namespace CoralCode {
    
    /**
     * @brief Configuración de línea de comandos
     */
    struct CommandLineArgs {
        std::vector<std::string> filesToOpen;
        std::string theme = "dark";
        std::string language = "auto";
        bool showHelp = false;
        bool showVersion = false;
        bool verbose = false;
        
        static CommandLineArgs parse(int argc, char* argv[]);
        void printHelp() const;
        void printVersion() const;
    };
    
    CommandLineArgs CommandLineArgs::parse(int argc, char* argv[]) {
        CommandLineArgs args;
        
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            
            if (arg == "--help" || arg == "-h") {
                args.showHelp = true;
            }
            else if (arg == "--version" || arg == "-v") {
                args.showVersion = true;
            }
            else if (arg == "--verbose") {
                args.verbose = true;
            }
            else if (arg == "--theme" && i + 1 < argc) {
                args.theme = argv[++i];
            }
            else if (arg == "--language" && i + 1 < argc) {
                args.language = argv[++i];
            }
            else if (arg.front() != '-') {
                // Es un archivo
                args.filesToOpen.push_back(arg);
            }
            else {
                std::cerr << "Argumento desconocido: " << arg << std::endl;
            }
        }
        
        return args;
    }
    
    void CommandLineArgs::printHelp() const {
        std::cout << "CoralCode - Editor de Código Profesional v1.0.0\n\n";
        std::cout << "Uso: coralcode [opciones] [archivos...]\n\n";
        std::cout << "Opciones:\n";
        std::cout << "  -h, --help           Mostrar esta ayuda\n";
        std::cout << "  -v, --version        Mostrar versión\n";
        std::cout << "  --verbose            Modo verboso\n";
        std::cout << "  --theme <tema>       Establecer tema (dark, light, blue, green)\n";
        std::cout << "  --language <lang>    Forzar lenguaje (auto, cpp, python, javascript, etc.)\n\n";
        std::cout << "Ejemplos:\n";
        std::cout << "  coralcode                          # Abrir editor vacío\n";
        std::cout << "  coralcode main.cpp                 # Abrir archivo específico\n";
        std::cout << "  coralcode --theme light *.cpp     # Abrir con tema claro\n";
        std::cout << "  coralcode --language python *.py  # Forzar highlighting de Python\n\n";
        std::cout << "Controles:\n";
        std::cout << "  Ctrl/Cmd+N      Nuevo archivo\n";
        std::cout << "  Ctrl/Cmd+O      Abrir archivo\n";
        std::cout << "  Ctrl/Cmd+S      Guardar\n";
        std::cout << "  Ctrl/Cmd+Z      Deshacer\n";
        std::cout << "  Ctrl/Cmd+Y      Rehacer\n";
        std::cout << "  Ctrl/Cmd+C/V    Copiar/Pegar\n";
        std::cout << "  Ctrl/Cmd+F      Buscar\n";
        std::cout << "  ESC             Salir\n\n";
        std::cout << "Para más información: https://github.com/tu-usuario/coralcode\n";
    }
    
    void CommandLineArgs::printVersion() const {
        std::cout << "CoralCode v1.0.0\n";
        std::cout << "Editor de código profesional desarrollado en C++\n";
        std::cout << "Copyright (c) 2024 - Licencia MIT\n";
        
        #ifdef CORALCODE_DEBUG
            std::cout << "Build: Debug\n";
        #else
            std::cout << "Build: Release\n";
        #endif
        
        #ifdef CORALCODE_WINDOWS
            std::cout << "Plataforma: Windows\n";
        #elif defined(CORALCODE_MACOS)
            std::cout << "Plataforma: macOS\n";
        #elif defined(CORALCODE_LINUX)
            std::cout << "Plataforma: Linux\n";
        #else
            std::cout << "Plataforma: Desconocida\n";
        #endif
    }
    
    /**
     * @brief Configura el entorno del sistema
     */
    void setupEnvironment() {
        #ifdef CORALCODE_WINDOWS
            // Configurar UTF-8 en Windows
            SetConsoleOutputCP(CP_UTF8);
            SetConsoleCP(CP_UTF8);
            
            // Habilitar colores ANSI en cmd/PowerShell
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD dwMode = 0;
            GetConsoleMode(hOut, &dwMode);
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        #endif
        
        // Configurar locale para caracteres especiales
        std::locale::global(std::locale(""));
    }
    
    /**
     * @brief Inicializa el logging del sistema
     */
    void setupLogging(bool verbose) {
        if (verbose) {
            std::cout << "🚀 Iniciando CoralCode Editor...\n";
            std::cout << "📁 Directorio de trabajo: " << std::filesystem::current_path() << "\n";
        }
    }
    
    /**
     * @brief Función principal del editor
     */
    int runEditor(const CommandLineArgs& args) {
        try {
            // Crear e inicializar el editor
            auto editor = std::make_unique<Editor>();
            
            if (!editor->initialize()) {
                std::cerr << "❌ Error: No se pudo inicializar el editor\n";
                return 1;
            }
            
            if (args.verbose) {
                std::cout << "✅ Editor inicializado correctamente\n";
            }
            
            // Configurar tema
            if (!args.theme.empty() && args.theme != "auto") {
                editor->setTheme(args.theme);
                if (args.verbose) {
                    std::cout << "🎨 Tema configurado: " << args.theme << "\n";
                }
            }
            
            // Abrir archivos especificados
            bool filesOpened = false;
            for (const auto& filepath : args.filesToOpen) {
                if (editor->openFile(filepath)) {
                    filesOpened = true;
                    if (args.verbose) {
                        std::cout << "📂 Archivo abierto: " << filepath << "\n";
                    }
                } else {
                    std::cerr << "⚠️  Advertencia: No se pudo abrir " << filepath << "\n";
                }
            }
            
            // Si no se abrió ningún archivo, crear uno nuevo
            if (!filesOpened) {
                editor->newFile();
                if (args.verbose) {
                    std::cout << "📝 Nuevo archivo creado\n";
                }
            }
            
            // Configurar lenguaje si se especificó
            if (!args.language.empty() && args.language != "auto") {
                editor->setLanguage(args.language);
                if (args.verbose) {
                    std::cout << "🌐 Lenguaje configurado: " << args.language << "\n";
                }
            }
            
            if (args.verbose) {
                std::cout << "🎯 Editor listo. ¡Comienza a programar!\n\n";
            }
            
            // Ejecutar loop principal
            editor->run();
            
            // Limpieza
            editor->shutdown();
            
            if (args.verbose) {
                std::cout << "\n👋 CoralCode terminado correctamente\n";
            }
            
            return 0;
            
        } catch (const std::exception& e) {
            std::cerr << "💥 Error fatal: " << e.what() << std::endl;
            return 1;
        } catch (...) {
            std::cerr << "💥 Error desconocido" << std::endl;
            return 1;
        }
    }
    
} // namespace CoralCode

/**
 * @brief Punto de entrada principal
 */
int main(int argc, char* argv[]) {
    using namespace CoralCode;
    
    // Configurar entorno
    setupEnvironment();
    
    // Procesar argumentos
    auto args = CommandLineArgs::parse(argc, argv);
    
    // Configurar logging
    setupLogging(args.verbose);
    
    // Mostrar ayuda o versión si se solicita
    if (args.showHelp) {
        args.printHelp();
        return 0;
    }
    
    if (args.showVersion) {
        args.printVersion();
        return 0;
    }
    
    // Ejecutar editor
    return runEditor(args);
}