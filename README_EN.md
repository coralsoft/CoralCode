# 🚀 CoralCode - Fast & Accessible Code Editor

A **free and open-source code editor** built with low-level technologies (C++ + SFML) designed to be **extremely fast and lightweight**. Our mission is to create an editor that works perfectly on older hardware, enabling people with limited resources to access modern and efficient development tools.

## 🎯 Mission

**Democratizing software development** by providing an editor that:
- 🖥️ **Works on old hardware** - Optimized for 5-10 year old computers
- ⚡ **Minimal resource usage** - Built with low-level technologies
- 🌍 **Completely free** - Open source under MIT license
- 🚀 **Maintains high speed** - Without sacrificing functionality for performance

## ✨ Current Features

- **🎨 Syntax Highlighting** for multiple languages (C++, Java, JavaScript, Python, C#, HTML, JSON)
- **📊 Line Numbers** with visual indicators
- **🖱️ Text Selection** with mouse (click and drag)
- **📋 System Clipboard** integration (Cmd+C/V on Mac, Ctrl+C/V on Windows/Linux)
- **↶ Undo/Redo** robust system (Cmd+Z, Cmd+Shift+Z) with 100 change limit
- **🔄 Advanced Scrolling** vertical and horizontal with mouse wheel
- **⚡ Quick Navigation** with modifiers (Ctrl/Cmd+Arrows)
- **📱 Resizable Window** without text distortion
- **🎯 Contextual Cursors** (text, hand, arrow)
- **📊 Status Bar** with real-time information

## 🚀 Quick Start

### Prerequisites
- **C++17** or higher
- **SFML 3.0**

### Installation

#### macOS
```bash
brew install sfml
git clone https://github.com/your-username/coralcode.git
cd coralcode
make legacy
./coralcode_legacy
```

#### Ubuntu/Debian
```bash
sudo apt-get install libsfml-dev
git clone https://github.com/your-username/coralcode.git
cd coralcode
make legacy
./coralcode_legacy
```

#### Windows (MinGW)
```bash
# Install SFML via package manager or download
git clone https://github.com/your-username/coralcode.git
cd coralcode
make legacy
./coralcode_legacy.exe
```

## ⌨️ Controls

### Basic Editing
- **Type** any ASCII character
- **Enter** new line
- **Backspace/Delete** delete characters
- **Arrows** navigate text
- **ESC** exit editor

### Advanced Navigation
- **Ctrl/Cmd + ↑/↓** scroll 10 lines
- **Ctrl/Cmd + ←/→** go to start/end of line
- **PageUp/PageDown** page scroll
- **Home/End** start/end of line

### Copy/Paste
- **Cmd+C** (Mac) / **Ctrl+C** (Windows/Linux): Copy selection
- **Cmd+V** (Mac) / **Ctrl+V** (Windows/Linux): Paste from clipboard
- **Cmd+Z** (Mac) / **Ctrl+Z** (Windows/Linux): Undo
- **Cmd+Shift+Z** (Mac) / **Ctrl+Y** (Windows/Linux): Redo

### Mouse
- **Click** position cursor
- **Click + Drag** select text
- **Mouse wheel** vertical scroll
- **Shift + Mouse wheel** horizontal scroll
- **Trackpad horizontal** native horizontal scroll (Mac)

## 🔧 Architecture

CoralCode uses a **modern modular architecture** following C++ best practices:

```
src/
├── core/                    # Core logic
│   ├── Editor.cpp          # Main orchestrator
│   ├── TextBuffer.cpp      # Content management
│   └── Viewport.cpp        # Scroll and visualization
├── ui/                     # User interface
│   ├── Window.cpp          # SFML window management
│   ├── EventHandler.cpp    # Event processing
│   └── Renderer.cpp        # Rendering system
├── syntax/                 # Syntax highlighting
│   ├── SyntaxHighlighter.cpp
│   ├── LanguageDetector.cpp
│   └── TokenParser.cpp
├── utils/                  # Utilities
│   ├── ClipboardManager.cpp
│   ├── UndoRedoManager.cpp
│   ├── FileHandler.cpp
│   └── ConfigManager.cpp
└── main.cpp               # Entry point
```

### Available Versions
- **Modular** (`make`): Professional architecture for development
- **Legacy** (`make legacy`): Single file (`coralcode.cpp`) functional but not maintainable

See [ARCHITECTURE_EN.md](ARCHITECTURE_EN.md) for detailed architecture documentation.

## 🚀 Upcoming Features

### File System
- [ ] Open files (.cpp, .h, .py, .js, .java, .html, .json, etc.)
- [ ] Save files (Ctrl/Cmd+S)
- [ ] Save as (Ctrl/Cmd+Shift+S)
- [ ] New file (Ctrl/Cmd+N)

### Multiple Tabs
- [ ] Tab system for multiple files
- [ ] Switch between tabs (Ctrl/Cmd+Tab)
- [ ] Close tabs (Ctrl/Cmd+W)

### Multiple Themes
- [ ] Light theme (white background, black text)
- [ ] Blue theme (dark blue, light text)
- [ ] Green theme (dark green, terminal style)
- [ ] Theme selector (Ctrl/Cmd+T)

### Language Module
- [ ] Detect language by file extension
- [ ] Language-specific keywords
- [ ] Language-specific comments (//, /* */, #, <!-- -->)
- [ ] Language-specific strings (", ', `, """)

## 🤝 Join the Mission

Help us democratize software development! We're looking for contributors who share our vision of **accessible technology for everyone**.

### Types of Contributions Needed

#### Performance Optimization
- Profiling and improving existing algorithms
- Memory usage reduction
- Rendering optimization
- Startup time improvements

#### Accessibility
- Support for older hardware
- Dependency reduction
- Energy efficiency improvements
- Optimization for slow SSDs/HDDs

#### New Features
- Efficient file system
- Multiple tabs without overhead
- Hardware-optimized themes
- Ultra-fast search

#### Documentation
- Performance benchmarks
- Limited hardware guides
- Optimization tutorials
- Community use cases

### How to Contribute

1. **Fork** the repository
2. **Create** a descriptive branch (`git checkout -b optimize/text-rendering`)
3. **Implement** changes with focus on efficiency
4. **Test** on limited hardware if possible
5. **Document** performance improvements
6. **Open** a Pull Request with metrics

### Contribution Criteria

- **Performance first:** Any new functionality must maintain or improve performance
- **Memory efficient:** Prefer stack over heap, avoid leaks
- **Compatibility:** Must work on 5-10 year old hardware
- **Simplicity:** Readable and maintainable code
- **Testing:** Include benchmarks when relevant

## 📜 License

This project is under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🎯 Project Philosophy

> **"Accessible technology for everyone"**

### Democratizing Development

We believe **everyone** should have access to efficient development tools, regardless of their economic resources or hardware age.

### Efficiency Over Luxury

While other editors add features that consume more resources, we focus on:
- **Constant optimization** of existing code
- **Efficient algorithms** over beautiful but slow interfaces
- **Essential functionality** without unnecessary bloatware
- **Consistent performance** on any hardware

### Technology with Purpose

**C++ and SFML** aren't just "cool" technologies - they're the right tools for:
- **Maximum control** over system resources
- **Predictable performance**
- **Software longevity** (no frameworks becoming obsolete)
- **Real portability** across platforms

### Social Impact

A fast and efficient editor can:
- **Extend the lifespan** of old computers
- **Reduce the digital divide** by enabling development on limited hardware
- **Foster learning** with tools that don't frustrate due to slowness
- **Democratize programming** for communities with limited resources

---

## ⭐ Help Us Grow!

If you believe in our mission to **democratize software development** and create accessible technology for everyone:

- 🌟 **Give us a star** to increase project visibility
- 🔄 **Share** with developers who have limited hardware
- 💬 **Tell us** your experience using CoralCode on old hardware
- 🤝 **Contribute** with optimizations or efficient new features

### Message to the Community

> *"Every optimized line of code can extend the lifespan of thousands of computers and allow more people to access the world of programming. Together we can make a difference."*

**CoralCode: Open source, fast, and accessible for everyone. 🚀** 