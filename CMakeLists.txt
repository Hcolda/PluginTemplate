cmake_minimum_required(VERSION 3.20)

project(PluginSystem LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(PluginAPI)

add_subdirectory(utils)
add_subdirectory(extensionsystem)
add_subdirectory(test)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/PluginSystemConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/PluginSystemConfig.cmake"
    INSTALL_DESTINATION lib/cmake/PluginSystem
)

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/PluginSystemConfigVersion.cmake"
    VERSION 1.0.0
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${PROJECT_BINARY_DIR}/PluginSystemConfig.cmake"
    "${PROJECT_BINARY_DIR}/PluginSystemConfigVersion.cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/PluginAPI.cmake"
    DESTINATION lib/cmake/PluginSystem
)
