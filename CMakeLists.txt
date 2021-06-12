cmake_minimum_required(VERSION 3.9)
project(CubicGameEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS src/ui)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES languages/CubicGameEngine_ru_RU.ts)

add_executable(CubicGameEngine
    src/CubicGameEngine.cpp
    src/graphics/CG_Viewport.h
    src/graphics/CG_Viewport.cpp
    src/graphics/CG_Scene.h
    src/graphics/CG_Scene.cpp
    src/graphics/CG_Primitives.h
    src/graphics/CG_Primitives.cpp
    src/graphics/CG_ViewportCamera.h
    src/graphics/CG_ViewportCamera.cpp
    src/graphics/CGFrameTimer.h
    src/graphics/CGFrameTimer.cpp
    src/CG_ConsoleManager.h
    src/CG_ConsoleManager.cpp
    src/CG_SaveStruct.h
    src/CG_SaveStruct.cpp
    src/CG_Filter.h
    src/CG_Filter.inl
    src/CG_IdManager.h
    src/CG_IdManager.inl
    src/CG_Pair.h

    src/windows/CG_CubicGameEngineWindow.cpp
    src/windows/CG_CubicGameEngineWindow.h

    src/ui/CubicGameEngineWindow.ui

    resources/resources.qrc

    ${TS_FILES}

    src/underConsideration/ShaderProgram.h
    src/underConsideration/ShaderProgram.cpp
    src/underConsideration/ShaderProgram.inl
    src/underConsideration/Figures.h
    src/underConsideration/Figures.inl
    src/underConsideration/Camera.h
    src/underConsideration/Camera.cpp
  )

target_include_directories(CubicGameEngine PRIVATE lib/glm/glm)
target_include_directories(CubicGameEngine PRIVATE src/graphics)

target_link_libraries(CubicGameEngine PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
