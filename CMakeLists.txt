cmake_minimum_required(VERSION 3.5)

project(cli-snake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# cli-snake
set(EXECUTABLE_DIR "${CMAKE_SOURCE_DIR}/bin")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

set(ENTRY_POINT "${CMAKE_SOURCE_DIR}/main.cpp")
file(GLOB_RECURSE INCLUDE_FILES "${INCLUDE_DIR}/*")
file(GLOB_RECURSE SOURCE_FILES "${SOURCE_DIR}/*")

# Combining all project files
set(PROJECT_SOURCES
	${ENTRY_POINT}
	${INCLUDE_FILES} ${SOURCE_FILES}
)

# Executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
set_target_properties(${PROJECT_NAME}
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_DIR}
)

# Resources
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})


# Boost
find_package(Boost COMPONENTS program_options REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
