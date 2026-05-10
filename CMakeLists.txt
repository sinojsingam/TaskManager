cmake_minimum_required(VERSION 3.20)

# create project
project(TaskManager)

# get hpp files from include dir
# specify the include dir
include_directories(include)

# find the OATPP package
find_package(oatpp 1.4.0 REQUIRED)

# create library from cpp file
add_library(apiMod 
    src/api_logic/task_api.cpp 
)

# link that library with oatpp
target_link_libraries(apiMod PRIVATE oatpp::oatpp)

# create library from cpp file
add_library(taskMod
    src/task_logic/task_manager.cpp 
)
# link other libraries as follows
target_link_libraries(taskMod PRIVATE oatpp::oatpp)
target_link_libraries(taskMod PRIVATE apiMod)
target_link_libraries(apiMod PRIVATE taskMod oatpp::oatpp)

add_executable(TaskManager src/main.cpp)

target_link_libraries(TaskManager 
    PRIVATE 
    taskMod
    apiMod 
    oatpp::oatpp
)
