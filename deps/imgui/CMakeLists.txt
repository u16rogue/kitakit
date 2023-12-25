cmake_minimum_required(VERSION 3.18)

message("-- kitakit > imgui > Loading package via CPM...")
CPMAddPackage("gh:ocornut/imgui@1.90")
if (NOT imgui_ADDED)
  message(FATAL_ERROR "-- kitakit > imgui > Failed to load package via CPM!")
endif()

project(imgui)

add_library(imgui STATIC
  "${imgui_SOURCE_DIR}/imconfig.h"
  "${imgui_SOURCE_DIR}/imgui.h"
  "${imgui_SOURCE_DIR}/imgui.cpp"
  "${imgui_SOURCE_DIR}/imgui_draw.cpp"
  "${imgui_SOURCE_DIR}/imgui_internal.h"
  "${imgui_SOURCE_DIR}/imgui_tables.cpp"
  "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
  "${imgui_SOURCE_DIR}/imstb_rectpack.h"
  "${imgui_SOURCE_DIR}/imstb_textedit.h"
  "${imgui_SOURCE_DIR}/imstb_truetype.h"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
)

target_include_directories(imgui
  PUBLIC
    "${imgui_SOURCE_DIR}/"
    "${imgui_SOURCE_DIR}/backends/"
)

target_link_libraries(imgui
  PUBLIC glfw
)
