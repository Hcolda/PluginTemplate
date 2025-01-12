cmake_minimum_required(VERSION 3.20)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)
add_executable(PluginTemplate
  main.cpp
)
target_link_libraries(PluginTemplate Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets ExtensionSystem Utils)
target_include_directories(PluginTemplate PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ExtensionSystem Utils)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(PluginAPI)

install(TARGETS PluginTemplate
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
add_subdirectory(plugin1)
add_subdirectory(plugin2)
