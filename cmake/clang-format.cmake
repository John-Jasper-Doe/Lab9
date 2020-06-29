print("Target \"clang-format\" enabled")

file(GLOB_RECURSE
  ALL_CXX_SOURCE_FILES
  src/*.[ch]pp
  src/*.[ch]
  test/*.[ch]pp
  test/*.[ch]
)

# Adding clang-format target if executable is found
if (NOT CLANG_FORMAT_BIN)
  find_program(CLANG_FORMAT_BIN "clang-format")
endif ()

if (CLANG_FORMAT_BIN)
  print(" -- clang-format has been found: ${CLANG_FORMAT_BIN}")
  add_custom_target(
    clang-format
    COMMAND "${CLANG_FORMAT_BIN}"
    -i
    ${ALL_CXX_SOURCE_FILES}
  )
endif ()
