cmake_minimum_required(VERSION 3.20)

project(Utils)

set(CMAKE_CXX_STANDARD 23)

add_library(${PROJECT_NAME} SHARED
    utilsglobal.h
    hostinfo.h
    hostinfo.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/>
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        UTILS_LIBRARY
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt6::Core
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
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/utils
)
