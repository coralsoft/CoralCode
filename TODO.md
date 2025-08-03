# 📋 CoralCode - Lista de Tareas Pendientes

## 🎯 **Próximas Mejoras Prioritarias**

### **📜 1. Barra de Scroll Visual**
- **Descripción**: Agregar una barra de scroll tradicional en el lado derecho
- **Funcionalidad**: 
  - Mostrar posición actual en el documento
  - Permitir click y drag para navegar rápidamente
  - Indicador visual del porcentaje de scroll
- **Beneficio**: Navegación más intuitiva en documentos largos

### **🔄 2. Sistema Undo/Redo (Cmd+Z)**
- **Descripción**: Implementar sistema de deshacer/rehacer cambios
- **Funcionalidad**:
  - **Cmd+Z**: Deshacer último cambio
  - **Cmd+Shift+Z**: Rehacer cambio deshecho
  - Historial de cambios (insertar, borrar, reemplazar)
  - Límite de historial (ej: últimos 100 cambios)
- **Beneficio**: Seguridad al editar y recuperación de errores

### **🖼️ 3. Texto Sin Distorsión al Redimensionar**
- **Descripción**: Mantener tamaño de texto constante al cambiar ventana
- **Problema actual**: El texto se estira como imagen
- **Solución**: 
  - Mantener fuente en tamaño fijo (16px)
  - Solo cambiar cantidad de líneas/columnas visibles
  - Recalcular layout sin afectar renderizado de texto
- **Beneficio**: Experiencia visual consistente

## 🔧 **Mejoras Técnicas Adicionales**

### **📏 4. Barra de Scroll Avanzada**
- Thumb (indicador) proporcional al contenido
- Click en área vacía para salto rápido
- Scroll suave con animación

### **💾 5. Historial de Cambios Robusto**
- Guardar estado antes de cada modificación
- Agrupar cambios pequeños (ej: escritura continua)
- Persistencia de historial entre sesiones

### **🎨 6. Renderizado de Fuente Mejorado**
- Fuente vectorial que no se pixela
- Subpixel rendering para claridad
- Diferentes tamaños de fuente opcionales

## 📊 **Estado Actual del Proyecto**

### ✅ **Características Completadas:**
- Editor de texto funcional sin bloqueos
- Syntax highlighting para múltiples lenguajes
- Números de línea automáticos
- Selección de texto con mouse
- Copy/paste con clipboard del sistema (Cmd+C/V)
- Scroll vertical con rueda del mouse
- Ventana redimensionable
- Barra de estado informativa
- Shortcuts nativos de Mac

### 🔄 **En Desarrollo:**
- Ninguna (sesión pausada)

### ⏳ **Pendientes (Próxima Sesión):**
- Barra de scroll visual
- Sistema Undo/Redo
- Corrección de distorsión de texto

## 🎯 **Objetivos de la Próxima Sesión**

1. **Prioridad 1**: Implementar barra de scroll visual
2. **Prioridad 2**: Agregar Cmd+Z (undo) básico
3. **Prioridad 3**: Corregir distorsión de texto al redimensionar

## 📝 **Notas Técnicas**

### **Para Barra de Scroll:**
- Posición: Lado derecho de la ventana
- Ancho: ~15px
- Color: Gris oscuro con thumb más claro
- Interacción: Click, drag, y wheel

### **Para Undo/Redo:**
- Estructura: `std::vector<EditorState>`
- Estado incluye: `lines`, `cursorLine`, `cursorCol`
- Máximo: 100 estados en historial
- Teclas: Cmd+Z (undo), Cmd+Shift+Z (redo)

### **Para Texto Sin Distorsión:**
- Problema: SFML escala todo el contenido
- Solución: Recalcular posiciones en píxeles absolutos
- Mantener: fontSize constante, solo cambiar layout

---

## 🚀 **¡Editor Funcional Completado!**

El editor actual ya es completamente funcional para uso diario:
- ✅ Escribir código sin problemas
- ✅ Syntax highlighting profesional  
- ✅ Copy/paste con sistema
- ✅ Scroll y navegación fluida
- ✅ Ventana redimensionable

**Las mejoras de arriba son para hacerlo aún más profesional y user-friendly.**