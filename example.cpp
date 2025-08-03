#include <iostream>
#include <string>
#include <vector>

// Clase de ejemplo para demostrar syntax highlighting
class ExampleClass {
private:
    std::string name;
    int value;
    std::vector<double> data;

public:
    // Constructor
    ExampleClass(const std::string& n, int v) : name(n), value(v) {
        std::cout << "Constructor llamado para: " << name << std::endl;
    }
    
    // Destructor
    ~ExampleClass() {
        std::cout << "Destructor llamado para: " << name << std::endl;
    }
    
    // Método público
    void processData() {
        for (int i = 0; i < 10; ++i) {
            data.push_back(i * 3.14159);
        }
        
        if (value > 100) {
            std::cout << "Valor alto detectado: " << value << std::endl;
        } else {
            std::cout << "Valor normal: " << value << std::endl;
        }
    }
    
    // Getter
    const std::string& getName() const {
        return name;
    }
    
    // Setter
    void setValue(int newValue) {
        value = newValue;
    }
};

// Función principal
int main() {
    std::cout << "=== Ejemplo de Syntax Highlighting ===" << std::endl;
    
    // Crear instancia
    ExampleClass obj("TestObject", 42);
    
    // Procesar datos
    obj.processData();
    
    // Usar strings
    std::string message = "Hola, mundo!";
    std::cout << message << std::endl;
    
    // Números y operaciones
    double pi = 3.14159265359;
    int radius = 5;
    double area = pi * radius * radius;
    
    std::cout << "Área del círculo: " << area << std::endl;
    
    // Arrays y loops
    int numbers[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; ++i) {
        std::cout << "Número " << i << ": " << numbers[i] << std::endl;
    }
    
    // Comentarios de una línea
    // Este es un comentario simple
    
    /* 
     * Comentario de bloque
     * Múltiples líneas
     */
    
    return 0;
} 