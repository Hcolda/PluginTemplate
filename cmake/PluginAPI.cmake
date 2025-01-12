function (add_my_plugin name)
    cmake_parse_arguments(my_args "" "" "SOURCES;DEPENDS" ${ARGN})
    add_library(${name} SHARED ${my_args_SOURCES})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${name}.json.in ${CMAKE_CURRENT_BINARY_DIR}/${name}.json)
    target_link_libraries(${name} PUBLIC ${my_args_DEPENDS})
    target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
    install(TARGETS ${name}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endfunction()
