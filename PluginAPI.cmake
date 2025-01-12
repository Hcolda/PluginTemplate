function(add_plugin target_name)
    cmake_parse_arguments(arg_plugin "SOURCES;DEPENDS" ${ARGN})

    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${name}.json.in")
      list(APPEND _arg_SOURCES ${name}.json.in)
      configure_file
  endif()
    add_library(${target_name} SHARED ${arg_plugin_SOURCES})
endfunction()
