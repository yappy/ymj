project(ymj)

file(GLOB SRCS "src/*.cpp")
add_library(${PROJECT_NAME} SHARED ${SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC include)
