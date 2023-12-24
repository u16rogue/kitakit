function (kk_load_cpm)
  file(DOWNLOAD https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
    EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
  )
  include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)
endfunction()

macro (kk_load_dependency target)
  if (NOT TARGET ${target})
    message("-- kitakit > ${target} is not an available target! Automatically resolving dependency...")
    add_subdirectory("deps/${target}/")
  else()
    message("-- kitakit > ${target} target available. Using already existing target and skipping dependency resolution!")
  endif()
endmacro()

macro (kk_target_pedantic target)
  target_compile_options(${target}
    PRIVATE
      $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>: -Wall -Wextra -Wpedantic -Werror -Wsign-conversion>
      $<$<CXX_COMPILER_ID:MSVC>: /W4 /WX>
  )
endmacro()
