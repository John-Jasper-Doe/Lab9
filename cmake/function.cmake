# Functions for dressing "life" :)

##
function(disable_clang_tidy TARGET)
  set_target_properties(${TARGET} PROPERTIES
      C_CLANG_TIDY ""
      CXX_CLANG_TIDY ""
      )
endfunction()

##
function(addtest TESTNAME FIRSTSOURCE)
  add_executable(${TESTNAME} ${FIRSTSOURCE} ${ARGN})
  target_link_libraries(${TESTNAME} gtest)
  add_test(
    NAME ${TESTNAME}
    COMMAND ${CMAKE_COMMAND} --build . --target ${TESTNAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
  set_target_properties(${TESTNAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test_bin
  )
  add_custom_command(
    TARGET ${TESTNAME}
    POST_BUILD
    COMMAND ${TESTNAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT ">>> Running ${TESTNAME}" VERBATIM
  )

  disable_clang_tidy(${TESTNAME})
endfunction(addtest)

