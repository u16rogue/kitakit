cmake_minimum_required(VERSION 3.18)

include("cmake/rut.cmake")
RUTLoadCPM()

#-------------------------------------------------------------------------------

macro (_KKLoadDependency target)
  if (NOT TARGET ${target})
    message("-- kitakit > ${target} is not an available target!
    Automatically resolving dependency...")
    add_subdirectory("deps/${target}/")
  else()
    message("-- kitakit > ${target} target available. Using already existing
    target and skipping dependency resolution!")
  endif()
endmacro()

#-------------------------------------------------------------------------------

message("-- kitakit > Loading dependencies...")
_KKLoadDependency(metapp)
_KKLoadDependency(glfw)
_KKLoadDependency(imgui)

project(kitakit)

add_library(${PROJECT_NAME} STATIC
  "include/kitakit/kitakit.hh"
  "src/kitakit.cc"

  "include/kitakit/kk_kit.hh"
  "include/kitakit/kk_imgui.hh"
  "src/kk_imgui.cc"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 20
)

target_include_directories(${PROJECT_NAME}
  PUBLIC "include/"
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC metapp imgui glfw
)

RUTTargetPedantic(${PROJECT_NAME})
