cmake_minimum_required(VERSION 3.19)
project(CodeforcesArena LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools Sql Svg)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

set(app_icon_resource "")
if(WIN32)
    set(app_icon_resource "app.rc")
endif()

# 项目文件目录
qt_add_executable(CodeforcesArena
    WIN32 MACOSX_BUNDLE
    main.cpp
    class/mainwindow.h class/mainwindow.cpp
    class/profiles.h class/profiles.cpp
    class/handledialog.h class/handledialog.cpp
    R.qrc
    ${app_icon_resource}
)

qt_add_translations(
    TARGETS CodeforcesArena
    TS_FILES CodeforcesArena_zh_CN.ts
)

target_link_libraries(CodeforcesArena
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::Sql
        Qt6::Svg
)

install(TARGETS CodeforcesArena
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET CodeforcesArena
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
