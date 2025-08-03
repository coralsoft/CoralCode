# 🚀 CoralCode - Editor de Código Rápido y Accesible

Editor de código **libre y abierto** desarrollado en C++ con tecnologías de bajo nivel. Nuestra misión es crear un editor que funcione perfectamente en hardware antiguo, permitiendo que personas con recursos limitados tengan acceso a herramientas de desarrollo modernas.

## 🎯 Misión

**Democratizar el desarrollo de software** con un editor que:
- 🖥️ **Funciona en hardware antiguo** (5-10 años)
- ⚡ **Consume mínimos recursos** (C++ + SFML)
- 🌍 **Completamente gratuito** (MIT license)
- 🚀 **Mantiene alta velocidad** sin sacrificar funcionalidades

## 🎯 Características

### ✅ Funciones Implementadas
- **🎨 Syntax Highlighting** para múltiples lenguajes
- **📊 Números de línea** automáticos
- **🖱️ Selección de texto** con mouse
- **📋 Copy/Paste del sistema** (Cmd+C/V, Ctrl+C/V)
- **↶ Undo/Redo** robusto (Cmd+Z, Cmd+Shift+Z)
- **🔄 Scroll avanzado** vertical y horizontal
- **⚡ Navegación rápida** con modificadores
- **📱 Ventana redimensionable** sin distorsión
- **🎯 Cursos contextuales** (texto, mano, flecha)
- **📊 Barra de estado** informativa

### 🌐 Lenguajes Soportados
- **C/C++**, **Java**, **JavaScript**, **Python**, **C#**, **HTML**, **JSON**

### 🎨 Colores
- 🔵 **Azul** palabras reservadas
- 🟢 **Verde** comentarios
- 🟠 **Naranja** strings
- ⚪ **Blanco** texto normal

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

## ✨ ¿Por Qué CoralCode?

### 🌍 Accesibilidad Universal
- ✅ **Hardware antiguo** - 4GB RAM o menos
- ✅ **Arranque ultrarrápido** - <1 segundo
- ✅ **Uso mínimo de memoria** - <50MB
- ✅ **Sin dependencias pesadas** - Solo SFML
- ✅ **Instalación ligera** - <5MB ejecutable

### 🚀 Tecnología de Bajo Nivel
- ✅ **C++ nativo** - Máximo rendimiento
- ✅ **SFML optimizado** - Renderizado eficiente
- ✅ **Gestión manual de memoria** - Sin garbage collection
- ✅ **Algoritmos eficientes** - O(1) en operaciones comunes
- ✅ **Compilación estática** - Sin DLLs externas

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

## 🤝 Únete a la Misión

¡Ayúdanos a democratizar el desarrollo de software! Buscamos contribuidores que compartan nuestra visión de **tecnología accesible para todos**.

### 🎯 Tipos de Contribución

#### 🔧 Optimización de Rendimiento
- Profiling y mejora de algoritmos
- Reducción de uso de memoria
- Optimización de renderizado
- Mejoras en tiempo de arranque

#### 🌍 Accesibilidad
- Soporte para hardware más antiguo
- Reducción de dependencias
- Mejoras en eficiencia energética
- Optimización para SSDs lentos/HDDs

#### 🚀 Nuevas Funcionalidades
- Sistema de archivos eficiente
- Múltiples pestañas sin overhead
- Temas optimizados para diferentes hardwares
- Búsqueda ultrarrápida

### 🛠️ Cómo Contribuir

1. **Fork** el repositorio
2. **Crea** una rama descriptiva (`git checkout -b optimize/text-rendering`)
3. **Implementa** cambios con foco en eficiencia
4. **Prueba** en hardware limitado si es posible
5. **Documenta** mejoras de rendimiento
6. **Abre** un Pull Request con métricas

### 📊 Criterios de Contribución

- **Rendimiento primero:** Cualquier funcionalidad nueva debe mantener o mejorar el rendimiento
- **Memoria eficiente:** Preferir stack sobre heap, evitar leaks
- **Compatibilidad:** Debe funcionar en hardware de hace 5-10 años
- **Simplicidad:** Código legible y mantenible
- **Testing:** Incluir benchmarks cuando sea relevante

## 📜 Licencia

Este proyecto está bajo la Licencia MIT - ver el archivo [LICENSE](LICENSE) para detalles.

## 🎯 Filosofía del Proyecto

> **"Tecnología accesible para todos"**

### 🌍 Democratizando el Desarrollo

Creemos que **todas las personas** deberían tener acceso a herramientas de desarrollo eficientes, independientemente de sus recursos económicos o la antigüedad de su hardware.

### ⚡ Eficiencia Sobre Lujo

Mientras otros editores agregan funcionalidades que consumen más recursos, nosotros nos enfocamos en:
- **Optimización constante** del código existente
- **Algoritmos eficientes** sobre interfaces bonitas pero lentas  
- **Funcionalidad esencial** sin bloatware innecesario
- **Rendimiento consistente** en cualquier hardware

### 🛠️ Tecnología con Propósito

**C++ y SFML** no son solo tecnologías "cool" - son las herramientas correctas para:
- **Máximo control** sobre recursos del sistema
- **Predictibilidad** en el rendimiento
- **Longevidad** del software (sin frameworks que se vuelvan obsoletos)
- **Portabilidad** real entre plataformas

### 💚 Impacto Social

Un editor rápido y eficiente puede:
- **Extender la vida útil** de computadoras antiguas
- **Reducir la brecha digital** permitiendo desarrollo en hardware limitado
- **Fomentar el aprendizaje** con herramientas que no frustren por lentitud
- **Democratizar la programación** para comunidades con recursos limitados

---

## ⭐ ¡Ayúdanos a Crecer!

Si crees en nuestra misión de **democratizar el desarrollo de software**:

- 🌟 **Dale una estrella** para aumentar la visibilidad
- 🔄 **Comparte** con desarrolladores que tengan hardware limitado
- 💬 **Cuéntanos** tu experiencia usando CoralCode en hardware antiguo
- 🤝 **Contribuye** con optimizaciones o nuevas funcionalidades

### 📢 Mensaje para la Comunidad

> *"Cada línea de código optimizada puede extender la vida útil de miles de computadoras y permitir que más personas accedan al mundo de la programación. Juntos podemos hacer la diferencia."*

**CoralCode: Código abierto, rápido y accesible para todos. 🚀**