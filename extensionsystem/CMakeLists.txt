cmake_minimum_required(VERSION 3.20)

project(ExtensionSystem)

set(CMAKE_CXX_STANDARD 23)

find_package(nlohmann_json REQUIRED)
add_library(${PROJECT_NAME} SHARED
    pluginspecification.h
    pluginspecification.cpp
    iplugin.h
    iplugin.cpp
    extensionsystemglobal.h
    extensionsystemtr.h
    pluginmanager.h
    pluginmanager.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/>
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        EXTENSIONSYSTEM_LIBRARY
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt6::Core
        Utils
        nlohmann_json::nlohmann_json
)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(
    EXPORT ${PROJECT_NAME}
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE PluginSystem::
    DESTINATION lib/cmake/PluginSystem
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/extensionsystem
    FILES_MATCHING PATTERN "*.h"
)
