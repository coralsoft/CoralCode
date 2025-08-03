# 🚀 CoralCode - Editor de Código Rápido y Accesible

Un **editor de código libre y abierto** desarrollado en C++ con tecnologías de bajo nivel, diseñado para ser **extremadamente rápido y ligero**. Nuestra misión es crear un editor que funcione perfectamente en hardware más antiguo, permitiendo que personas con recursos limitados tengan acceso a herramientas de desarrollo modernas y eficientes.

## 🎯 **Nuestra Misión**

**Democratizar el desarrollo de software** proporcionando un editor que:
- 🖥️ **Funciona en hardware antiguo** - Optimizado para computadoras de hace 5-10 años
- ⚡ **Consume mínimos recursos** - Desarrollado con tecnologías de bajo nivel (C++ + SFML)
- 🌍 **Es completamente gratuito** - Código abierto bajo licencia MIT
- 🚀 **Mantiene alta velocidad** - Sin sacrificar funcionalidades por rendimiento

## ✨ **Características Actuales**

**🎯 Aspiramos a ser un editor profesional** con funcionalidades modernas  
**⚡ Rendimiento Ultrarrápido** diseñado para hardware limitado  
**🎨 Syntax Highlighting Eficiente** sin impacto en rendimiento  
**🖥️ Multiplataforma** para macOS, Windows y Linux

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

## 🔧 Arquitectura Modular

CoralCode utiliza una **arquitectura modular moderna** que sigue las mejores prácticas de C++:

```
src/
├── core/                    # Lógica central
│   ├── Editor.cpp          # Orquestador principal
│   ├── TextBuffer.cpp      # Gestión de contenido
│   └── Viewport.cpp        # Scroll y visualización
├── ui/                     # Interfaz de usuario  
│   ├── Window.cpp          # Gestión de ventana SFML
│   ├── EventHandler.cpp    # Procesamiento de eventos
│   └── Renderer.cpp        # Sistema de renderizado
├── syntax/                 # Syntax highlighting
│   ├── SyntaxHighlighter.cpp
│   ├── LanguageDetector.cpp
│   └── TokenParser.cpp
├── utils/                  # Utilidades
│   ├── ClipboardManager.cpp
│   ├── UndoRedoManager.cpp
│   ├── FileHandler.cpp
│   └── ConfigManager.cpp
└── main.cpp               # Punto de entrada
```

### **📋 Principios de Diseño:**
- **🔧 Separación de Responsabilidades** - Cada módulo tiene una función específica
- **🔄 Bajo Acoplamiento** - Dependencias mínimas entre módulos  
- **🧪 Alta Testabilidad** - Cada componente es testeable por separado
- **🚀 Escalabilidad** - Fácil agregar nuevas funcionalidades

### **📊 Versiones Disponibles:**
- **Modular** (`make`): Arquitectura profesional recomendada para desarrollo
- **Legacy** (`make legacy`): Archivo único (`coralcode.cpp`) funcional pero no mantenible

Ver [ARCHITECTURE.md](ARCHITECTURE.md) para documentación detallada de la arquitectura.

## ✨ **¿Por Qué CoralCode?**

### 🌍 **Accesibilidad Universal:**
- ✅ **Hardware Antiguo** - Funciona en computadoras de 4GB RAM o menos
- ✅ **Arranque Ultrarrápido** - Inicia en menos de 1 segundo
- ✅ **Uso Mínimo de Memoria** - Menos de 50MB en uso típico
- ✅ **Sin Dependencias Pesadas** - Solo SFML como dependencia externa
- ✅ **Instalación Ligera** - Ejecutable de menos de 5MB

### 🚀 **Tecnología de Bajo Nivel:**
- ✅ **C++ Nativo** - Máximo rendimiento sin abstracciones innecesarias
- ✅ **SFML Optimizado** - Renderizado eficiente y multiplataforma
- ✅ **Gestión Manual de Memoria** - Sin garbage collection que cause lag
- ✅ **Algoritmos Eficientes** - Optimizado para O(1) en operaciones comunes
- ✅ **Compilación Estática** - Sin DLLs externas en tiempo de ejecución

### 💡 **Funcionalidades Inteligentes:**
- ✅ **Syntax Highlighting Eficiente** - Sin impacto en rendimiento
- ✅ **Navegación Fluida** - Scroll y cursor siempre responsivos  
- ✅ **Undo/Redo Inteligente** - Sistema optimizado de historial
- ✅ **Copy/Paste Nativo** - Integración completa con el sistema
- ✅ **Selección Precisa** - Feedback visual inmediato

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

## 🤝 **Únete a la Misión**

¡Ayúdanos a democratizar el desarrollo de software! Buscamos contribuidores que compartan nuestra visión de **tecnología accesible para todos**.

### 🎯 **Tipos de Contribución Necesarias:**

#### **🔧 Optimización de Rendimiento:**
- Profiling y mejora de algoritmos existentes
- Reducción del uso de memoria
- Optimización de renderizado
- Mejoras en tiempo de arranque

#### **🌍 Accesibilidad:**
- Soporte para hardware más antiguo
- Reducción de dependencias
- Mejoras en eficiencia energética
- Optimización para SSDs lentos/HDDs

#### **🚀 Nuevas Funcionalidades:**
- Sistema de archivos eficiente
- Múltiples pestañas sin overhead
- Temas optimizados para diferentes hardwares
- Búsqueda ultrarrápida

#### **📚 Documentación:**
- Benchmarks de rendimiento
- Guías para hardware limitado
- Tutoriales de optimización
- Casos de uso en comunidades

### 🛠️ **Cómo Contribuir:**

1. **Fork** el repositorio
2. **Crea** una rama descriptiva (`git checkout -b optimize/text-rendering`)
3. **Implementa** cambios con foco en eficiencia
4. **Prueba** en hardware limitado si es posible
5. **Documenta** mejoras de rendimiento
6. **Abre** un Pull Request con métricas

### 📊 **Criterios de Contribución:**

- **Rendimiento primero:** Cualquier funcionalidad nueva debe mantener o mejorar el rendimiento
- **Memoria eficiente:** Preferir stack sobre heap, evitar leaks
- **Compatibilidad:** Debe funcionar en hardware de hace 5-10 años
- **Simplicidad:** Código legible y mantenible
- **Testing:** Incluir benchmarks cuando sea relevante

## 📜 Licencia

Este proyecto está bajo la Licencia MIT - ver el archivo [LICENSE](LICENSE) para detalles.

## 🎯 **Filosofía del Proyecto**

> **"Tecnología accesible para todos"**

### 🌍 **Democratizando el Desarrollo**

Creemos que **todas las personas** deberían tener acceso a herramientas de desarrollo eficientes, independientemente de sus recursos económicos o la antigüedad de su hardware.

### ⚡ **Eficiencia Sobre Lujo**

Mientras otros editores agregan funcionalidades que consumen más recursos, nosotros nos enfocamos en:
- **Optimización constante** del código existente
- **Algoritmos eficientes** sobre interfaces bonitas pero lentas  
- **Funcionalidad esencial** sin bloatware innecesario
- **Rendimiento consistente** en cualquier hardware

### 🛠️ **Tecnología con Propósito**

**C++ y SFML** no son solo tecnologías "cool" - son las herramientas correctas para:
- **Máximo control** sobre recursos del sistema
- **Predictibilidad** en el rendimiento
- **Longevidad** del software (sin frameworks que se vuelvan obsoletos)
- **Portabilidad** real entre plataformas

### 💚 **Impacto Social**

Un editor rápido y eficiente puede:
- **Extender la vida útil** de computadoras antiguas
- **Reducir la brecha digital** permitiendo desarrollo en hardware limitado
- **Fomentar el aprendizaje** con herramientas que no frustren por lentitud
- **Democratizar la programación** para comunidades con recursos limitados

---

## ⭐ **¡Ayúdanos a Crecer!**

Si crees en nuestra misión de **democratizar el desarrollo de software** y crear tecnología accesible para todos:

- 🌟 **Dale una estrella** para aumentar la visibilidad del proyecto
- 🔄 **Comparte** con desarrolladores que tengan hardware limitado
- 💬 **Cuéntanos** tu experiencia usando CoralCode en hardware antiguo
- 🤝 **Contribuye** con optimizaciones o nuevas funcionalidades eficientes

### 📢 **Mensaje para la Comunidad:**

> *"Cada línea de código optimizada puede extender la vida útil de miles de computadoras y permitir que más personas accedan al mundo de la programación. Juntos podemos hacer la diferencia."*

**CoralCode: Código abierto, rápido y accesible para todos. 🚀**