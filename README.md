# CoralCode - Editor de Código C++ 🚀

Un editor de código **súper simple y funcional** desarrollado en C++, basado en el `simple_editor` que funciona perfecto, ahora con **syntax highlighting** de múltiples lenguajes.

## ✨ **NUEVO - Editor Simplificado**

**❌ Eliminé el sistema complejo** que causaba problemas  
**✅ Editor basado en `simple_editor`** que funciona perfecto  
**🎨 Syntax highlighting** para palabras reservadas  
**⚡ Sin bloqueos** con `=`, `/`, o cualquier carácter

## 🎯 Características

### ✅ **Funciones Implementadas:**
- **Edición de texto fluida** - basada en `std::vector<std::string>`
- **Syntax highlighting** para palabras reservadas
- **Soporte para múltiples lenguajes:**
  - **C++**: `int`, `for`, `if`, `class`, `return`, `virtual`, etc.
  - **C**: `printf`, `scanf`, `malloc`, `sizeof`, etc.
  - **Java**: `package`, `extends`, `interface`, `final`, etc.
  - **JavaScript**: `var`, `let`, `function`, `typeof`, etc.
  - **Python**: `def`, `import`, `from`, `lambda`, etc.
  - **C#**: `readonly`, `override`, `sealed`, etc.
- **Colores optimizados:**
  - 🔵 **Azul** para palabras reservadas
  - 🟢 **Verde** para comentarios `//`
  - 🟠 **Naranja** para strings `"text"`
  - ⚪ **Blanco** para texto normal
- **Indicador visual de línea actual** (barra roja)
- **Cursor amarillo** siempre visible
- **Navegación completa:** flechas, Enter, Backspace
- **Fuente del sistema** (Monaco, Menlo, etc.)

## 🚀 Cómo Usar

### Prerrequisitos
- **C++17** o superior
- **SFML 3.0** 

### Instalación de SFML en macOS
```bash
brew install sfml
```

### Compilación (Una Línea)
```bash
g++ -std=c++17 coralcode.cpp -lsfml-graphics -lsfml-window -lsfml-system -I/opt/homebrew/include -L/opt/homebrew/lib -o coralcode
```

### Ejecución
```bash
./coralcode
```

## 🎨 Syntax Highlighting

El editor detecta automáticamente palabras reservadas y las colorea:

```cpp
int main() {                    // 'int' y 'main' en azul
    for (int i = 0; i < 10; i++) {  // 'for' e 'int' en azul
        printf("Hello World");      // 'printf' en azul, "Hello World" en naranja
        // Este es un comentario      // Línea completa en verde
    }
    return 0;                   // 'return' en azul
}
```

## ⌨️ Controles

- **Escribir**: Cualquier carácter ASCII (a-z, 0-9, =, /, etc.)
- **Enter**: Nueva línea
- **Backspace**: Borrar carácter/fusionar líneas
- **Flechas**: Navegar por el texto
- **ESC**: Salir del editor

## 🔧 Arquitectura Simple

```
coralcode.cpp
├── std::vector<std::string> lines    // Almacenamiento simple
├── size_t currentLine, currentCol    // Posición del cursor
├── isKeyword()                       // Detección de palabras reservadas
├── processLine()                     // Análisis de syntax highlighting
└── SFML rendering                    // Visualización
```

**Sin sistemas complejos, sin TextBuffer, sin clases pesadas.**  
**Solo lógica directa que FUNCIONA.**

## ✨ Ventajas del Nuevo Sistema

### 🟢 **Lo que FUNCIONA:**
- ✅ **Todos los caracteres** (`=`, `/`, `@`, `#`, etc.)
- ✅ **Sin bloqueos** al escribir
- ✅ **Cursor siempre sincronizado**
- ✅ **Rendimiento fluido**
- ✅ **Syntax highlighting funcional**
- ✅ **Interfaz clara y limpia**

### 🔴 **Lo que ELIMINÉ:**
- ❌ Sistema complejo de TextBuffer
- ❌ Clases pesadas de SyntaxHighlighter
- ❌ Arquitectura multi-archivo problemática
- ❌ Actualizaciones forzadas que causaban lag
- ❌ Parsing complejo de tokens

## 📊 Comparación

| Aspecto | Editor Complejo (Anterior) | Editor Simple (Nuevo) |
|---------|----------------------------|----------------------|
| **Funcionalidad** | ❌ Bloqueos con `=` y `/` | ✅ Todos los caracteres |
| **Rendimiento** | 🐌 Lento, lag | ⚡ Fluido, responsive |
| **Syntax Highlighting** | 🔴 Bugueado | 🎨 Funcional |
| **Compilación** | 😵 CMake complejo | 😊 Una línea |
| **Mantenibilidad** | 📚 Múltiples archivos | 📄 Un archivo |
| **Confiabilidad** | ❓ Inestable | ✅ Sólido |

## 🎯 Filosofía del Proyecto

> **"Simple, funcional, confiable"**

Este editor demuestra que **no necesitas arquitecturas complejas** para crear software funcional. A veces, **la simplicidad es la sofisticación suprema**.

**El `simple_editor` funcionaba perfecto → Agregué syntax highlighting → Ahora `coralcode` funciona perfecto.**

## 🚀 Próximos Pasos (Opcionales)

Si quieres extender el editor manteniendo la simplicidad:

- **Guardar/Abrir archivos** - agregar funciones simples
- **Búsqueda de texto** - con Ctrl+F
- **Múltiples pestañas** - array de `std::vector<std::string>`
- **Más colores** - extender `processLine()`
- **Autocompletado básico** - suggestions simples

Pero recuerda: **mantenlo simple.**

---

## 📜 Licencia

MIT License - Úsalo libremente.

## 🎉 ¡Disfruta tu editor que FUNCIONA!

**No más bloqueos, no más problemas. Solo un editor simple y funcional con syntax highlighting.**