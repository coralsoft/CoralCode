# 🚀 CoralCode - Editor de Código Profesional

Un **editor de código moderno y funcional** desarrollado en C++ con SFML, diseñado para ser **rápido, ligero y profesional**. Perfecto para desarrollo diario con syntax highlighting, múltiples temas y navegación avanzada.

## ✨ **Características Principales**

**🎯 Editor Profesional** con todas las funcionalidades modernas  
**⚡ Rendimiento Optimizado** sin bloqueos ni lag  
**🎨 Syntax Highlighting** para múltiples lenguajes  
**🖥️ Interfaz Nativa** para macOS, Windows y Linux

## 🎯 Características

### ✅ **Funciones Implementadas:**
- **🎨 Syntax Highlighting Profesional** para múltiples lenguajes
- **📊 Números de Línea Automáticos** con indicador visual
- **🖱️ Selección de Texto** con mouse (click y arrastra)
- **📋 Copy/Paste del Sistema** (Cmd+C/V en Mac, Ctrl+C/V en Windows/Linux)
- **↶ Undo/Redo Robusto** (Cmd+Z, Cmd+Shift+Z) con límite de 100 cambios
- **🔄 Scroll Avanzado** vertical y horizontal con rueda del mouse
- **⚡ Navegación Rápida** con modificadores (Ctrl/Cmd+Flechas)
- **📱 Ventana Redimensionable** sin distorsión de texto
- **🎯 Cursos Contextuales** (texto, mano, flecha)
- **📊 Barra de Estado Informativa** con estadísticas en tiempo real

### 🌐 **Lenguajes Soportados:**
- **C/C++**: `int`, `for`, `if`, `class`, `return`, `virtual`, etc.
- **Java**: `package`, `extends`, `interface`, `final`, etc.
- **JavaScript**: `var`, `let`, `function`, `typeof`, etc.
- **Python**: `def`, `import`, `from`, `lambda`, etc.
- **C#**: `readonly`, `override`, `sealed`, etc.
- **HTML**: `div`, `span`, `class`, `id`, etc.
- **JSON**: `true`, `false`, `null`, etc.

### 🎨 **Colores Optimizados:**
- 🔵 **Azul** para palabras reservadas
- 🟢 **Verde** para comentarios `//` y `/* */`
- 🟠 **Naranja** para strings `"text"` y `'text'`
- ⚪ **Blanco** para texto normal

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

### **📝 Edición Básica:**
- **Escribir**: Cualquier carácter ASCII
- **Enter**: Nueva línea
- **Backspace**: Borrar carácter/fusionar líneas
- **Delete**: Borrar carácter siguiente
- **Flechas**: Navegar por el texto
- **ESC**: Salir del editor

### **⚡ Navegación Avanzada:**
- **Ctrl/Cmd + ↑/↓**: Scroll rápido de 10 líneas
- **Ctrl/Cmd + ←/→**: Ir al inicio/fin de línea
- **PageUp/PageDown**: Scroll de página completa
- **Home/End**: Inicio/fin de línea

### **📋 Copy/Paste:**
- **Cmd+C** (Mac) / **Ctrl+C** (Windows/Linux): Copiar selección
- **Cmd+V** (Mac) / **Ctrl+V** (Windows/Linux): Pegar del clipboard
- **Cmd+Z** (Mac) / **Ctrl+Z** (Windows/Linux): Deshacer
- **Cmd+Shift+Z** (Mac) / **Ctrl+Y** (Windows/Linux): Rehacer

### **🖱️ Mouse:**
- **Click**: Posicionar cursor
- **Click + Arrastra**: Seleccionar texto
- **Rueda del mouse**: Scroll vertical
- **Shift + Rueda**: Scroll horizontal
- **Trackpad horizontal**: Scroll horizontal nativo (Mac)

## 🔧 Arquitectura Moderna

```
coralcode.cpp
├── std::vector<std::string> lines    // Almacenamiento eficiente
├── size_t currentLine, currentCol    // Posición del cursor
├── size_t scrollLine, scrollCol      // Posición del scroll
├── isKeyword()                       // Detección de palabras reservadas
├── processLine()                     // Análisis de syntax highlighting
├── calculateVisibleLines()           // Cálculo de líneas visibles
├── getSelectionBounds()              // Lógica de selección
├── saveState() / undo() / redo()     // Sistema de historial
├── getClipboard() / setClipboard()   // Integración con sistema
└── SFML rendering                    // Visualización moderna
```

**Arquitectura optimizada y robusta** con todas las funcionalidades modernas.

## ✨ Ventajas del Sistema

### 🟢 **Características Destacadas:**
- ✅ **Rendimiento Optimizado** - Sin bloqueos ni lag
- ✅ **Syntax Highlighting Profesional** - Múltiples lenguajes
- ✅ **Navegación Avanzada** - Scroll rápido y preciso
- ✅ **Interfaz Nativa** - Comportamiento nativo en cada plataforma
- ✅ **Undo/Redo Robusto** - Historial de 100 cambios
- ✅ **Copy/Paste del Sistema** - Integración completa
- ✅ **Selección de Texto** - Click y arrastra intuitivo
- ✅ **Ventana Redimensionable** - Sin distorsión de texto
- ✅ **Cursos Contextuales** - UX profesional
- ✅ **Barra de Estado Informativa** - Estadísticas en tiempo real

## 🚀 Próximas Funcionalidades

### **📁 Sistema de Archivos:**
- [ ] Abrir archivos (.cpp, .h, .py, .js, .java, .html, .json, etc.)
- [ ] Guardar archivos (Ctrl/Cmd+S)
- [ ] Guardar como (Ctrl/Cmd+Shift+S)
- [ ] Nuevo archivo (Ctrl/Cmd+N)

### **📑 Múltiples Pestañas:**
- [ ] Sistema de pestañas para múltiples archivos
- [ ] Cambiar entre pestañas (Ctrl/Cmd+Tab)
- [ ] Cerrar pestañas (Ctrl/Cmd+W)

### **🎨 Múltiples Temas:**
- [ ] Tema Light (fondo blanco, texto negro)
- [ ] Tema Blue (azul oscuro, texto claro)
- [ ] Tema Green (verde oscuro, terminal style)
- [ ] Selector de temas (Ctrl/Cmd+T)

### **🌐 Módulo de Lenguajes:**
- [ ] Detectar lenguaje por extensión de archivo
- [ ] Keywords específicas por lenguaje
- [ ] Comentarios específicos (//, /* */, #, <!-- -->)
- [ ] Strings específicos (", ', `, """)

## 🤝 Contribuir

¡Las contribuciones son bienvenidas! Si quieres contribuir:

1. **Fork** el repositorio
2. **Crea** una rama para tu feature (`git checkout -b feature/AmazingFeature`)
3. **Commit** tus cambios (`git commit -m 'Add some AmazingFeature'`)
4. **Push** a la rama (`git push origin feature/AmazingFeature`)
5. **Abre** un Pull Request

## 📜 Licencia

Este proyecto está bajo la Licencia MIT - ver el archivo [LICENSE](LICENSE) para detalles.

## 🎯 Filosofía del Proyecto

> **"Simple, funcional, confiable"**

CoralCode demuestra que **no necesitas arquitecturas complejas** para crear software funcional. A veces, **la simplicidad es la sofisticación suprema**.

---

## ⭐ ¡Dale una estrella si te gusta!

Si este editor te resulta útil, ¡considera darle una estrella en GitHub!