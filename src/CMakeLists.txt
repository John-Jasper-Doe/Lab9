# CMake file for source directory

set(CMAKE_FOLDER "${CMAKE_FOLDER}/src")

file(GLOB_RECURSE ALL_CXX_SOURCE_FILES *.cpp *.cxx)

add_executable(${PROJECT_NAME} ${ALL_CXX_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}
  pthread
  ${Boost_LIBRARIES}
)
