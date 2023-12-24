cmake_minimum_required(VERSION 3.18)

message("-- kitakit > glfw > Loading package via CPM...")
CPMAddPackage("gh:glfw/glfw#3.3.9")
if (NOT glfw_ADDED)
  message(FATAL_ERROR "-- kitakit > glfw > Failed to load package via CPM!")
endif()
