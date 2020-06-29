# CMake file for source directory

set(CMAKE_FOLDER "${CMAKE_FOLDER}/src")

file(GLOB_RECURSE ALL_CXX_SOURCE_FILES *.cpp *.cxx)

add_executable(${PROJECT_NAME} ${ALL_CXX_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME}
  pthread
  ${Boost_LIBRARIES}
)

# install to bin folder our binaries
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
