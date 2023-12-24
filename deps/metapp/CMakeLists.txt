cmake_minimum_required(VERSION 3.18)

message("-- kitakit > metapp > Loading package via CPM...")
CPMAddPackage("gh:u16rogue/metapp@0.6.0")
if (NOT metapp_ADDED)
  message(FATAL_ERROR "-- kitakit > metapp > Failed to load package via CPM!")
endif()
