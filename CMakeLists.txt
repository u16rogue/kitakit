cmake_minimum_required(VERSION 3.18)

include("utils.cmake")

message("-- kitakit > Loading CPM...")
kk_load_cpm()

message("-- kitakit > Loading dependencies...")
kk_load_dependency(metapp)
kk_load_dependency(glfw)
kk_load_dependency(imgui)

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

kk_target_pedantic(${PROJECT_NAME})
