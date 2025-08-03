# CMakeLists.txt - CoralCode Editor
cmake_minimum_required(VERSION 3.16)

# Información del proyecto
project(CoralCode 
    VERSION 1.0.0
    DESCRIPTION "Editor de código profesional y ligero"
    LANGUAGES CXX
)

# Configuración de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configuración de build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Opciones del proyecto
option(CORALCODE_BUILD_TESTS "Build tests" OFF)
option(CORALCODE_BUILD_DOCS "Build documentation" OFF)
option(CORALCODE_ENABLE_WARNINGS "Enable compiler warnings" ON)
option(CORALCODE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

# Configuración de directorios
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Encontrar SFML
find_package(SFML 3.0 COMPONENTS graphics window system REQUIRED)

# Archivos fuente
set(CORE_SOURCES
    src/core/TextBuffer.cpp
    src/core/Viewport.cpp
    src/core/Editor.cpp
)

set(UI_SOURCES
    src/ui/Window.cpp
    src/ui/EventHandler.cpp
    src/ui/Renderer.cpp
)

set(SYNTAX_SOURCES
    src/syntax/SyntaxHighlighter.cpp
    src/syntax/LanguageDetector.cpp
    src/syntax/TokenParser.cpp
)

set(UTILS_SOURCES
    src/utils/ClipboardManager.cpp
    src/utils/UndoRedoManager.cpp
    src/utils/FileHandler.cpp
    src/utils/ConfigManager.cpp
)

set(ALL_SOURCES
    ${CORE_SOURCES}
    ${UI_SOURCES}
    ${SYNTAX_SOURCES}
    ${UTILS_SOURCES}
    src/main.cpp
)

# Crear ejecutable
add_executable(${PROJECT_NAME} ${ALL_SOURCES})

# Configurar directorios de include
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Enlazar librerías
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        sfml-graphics 
        sfml-window 
        sfml-system
)

# Configuración específica por plataforma
if(WIN32)
    # Windows específico
    target_compile_definitions(${PROJECT_NAME} PRIVATE CORALCODE_WINDOWS)
    if(MINGW)
        target_link_libraries(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
    endif()
elseif(APPLE)
    # macOS específico
    target_compile_definitions(${PROJECT_NAME} PRIVATE CORALCODE_MACOS)
    find_library(COCOA_LIBRARY Cocoa)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COCOA_LIBRARY})
elseif(UNIX)
    # Linux específico
    target_compile_definitions(${PROJECT_NAME} PRIVATE CORALCODE_LINUX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${GTK3_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${GTK3_INCLUDE_DIRS})
endif()

# Configurar warnings del compilador
if(CORALCODE_ENABLE_WARNINGS)
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /W4)
        if(CORALCODE_WARNINGS_AS_ERRORS)
            target_compile_options(${PROJECT_NAME} PRIVATE /WX)
        endif()
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE 
            -Wall -Wextra -Wpedantic
            -Wcast-align -Wcast-qual -Wctor-dtor-privacy
            -Wdisabled-optimization -Wformat=2 -Winit-self
            -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs
            -Wnoexcept -Wold-style-cast -Woverloaded-virtual
            -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo
            -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default
            -Wundef -Wno-unused
        )
        if(CORALCODE_WARNINGS_AS_ERRORS)
            target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
        endif()
    endif()
endif()

# Configuración de Debug/Release
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:CORALCODE_DEBUG>
    $<$<CONFIG:Release>:CORALCODE_RELEASE>
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -O3 -DNDEBUG)
endif()

# Tests (opcional)
if(CORALCODE_BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)
    
    add_executable(${PROJECT_NAME}_tests
        tests/test_textbuffer.cpp
        tests/test_viewport.cpp
        tests/test_syntax.cpp
        ${CORE_SOURCES}
        ${SYNTAX_SOURCES}
        ${UTILS_SOURCES}
    )
    
    target_include_directories(${PROJECT_NAME}_tests PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${GTEST_INCLUDE_DIRS}
    )
    
    target_link_libraries(${PROJECT_NAME}_tests 
        PRIVATE 
            ${GTEST_LIBRARIES}
            sfml-system
    )
    
    add_test(NAME CoralCodeTests COMMAND ${PROJECT_NAME}_tests)
endif()

# Documentación (opcional)
if(CORALCODE_BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
        
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        
        add_custom_target(docs ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
        )
    endif()
endif()

# Instalación
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Crear script de build
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/build.sh.in
    ${CMAKE_CURRENT_BINARY_DIR}/build.sh
    @ONLY
)

# Información de build
message(STATUS "CoralCode Editor Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Build Tests: ${CORALCODE_BUILD_TESTS}")
message(STATUS "  Build Docs: ${CORALCODE_BUILD_DOCS}")
message(STATUS "  Warnings Enabled: ${CORALCODE_ENABLE_WARNINGS}")
message(STATUS "  Warnings as Errors: ${CORALCODE_WARNINGS_AS_ERRORS}")