project(testrun)

file(GLOB SRCS "src/*.cpp")
add_executable(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} ymj)
