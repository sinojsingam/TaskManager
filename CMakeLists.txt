cmake_minimum_required(VERSION 3.20)

# create project
project(TaskManager)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Use -std=c++17 instead of -std=gnu++17

# get hpp files from include dir
# specify the include dir
include_directories(include)

# find the OATPP package
find_package(oatpp 1.4.0 REQUIRED)
find_package(oatpp-postgresql REQUIRED)

# library create, it as no dependencies (top of the hierarchy)
add_library(taskMod
    src/task_logic/task_manager.cpp 
)

# create library from cpp file
add_library(apiMod 
    src/api_logic/task_api.cpp 
)

# api mod depends on task and oatpp
target_link_libraries(apiMod PRIVATE
	taskMod
	oatpp::oatpp
	oatpp::oatpp-postgresql
)

# depends on both task and apiMod
add_library(cliMod 
    src/cli_logic/cli_manager.cpp 
)

target_link_libraries(cliMod PRIVATE
	taskMod
	apiMod
	oatpp::oatpp
	oatpp::oatpp-postgresql
)

add_executable(TaskManager src/main.cpp)

target_link_libraries(TaskManager 
    PRIVATE 
    taskMod
    apiMod 
    cliMod
    oatpp::oatpp
    oatpp::oatpp-postgresql
)
