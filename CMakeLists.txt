cmake_minimum_required(VERSION 3.20)

project(TaskManager)
include_directories(include)
find_package(oatpp 1.4.0 REQUIRED)

add_library(apiMod 
    src/api_logic/task_api.cpp 
)

target_link_libraries(apiMod PRIVATE oatpp::oatpp)

add_library(taskMod
    src/task_logic/task_manager.cpp 
)
target_link_libraries(taskMod PRIVATE apiMod)
target_link_libraries(apiMod PRIVATE taskMod oatpp::oatpp)

add_executable(TaskManager src/main.cpp)

target_link_libraries(TaskManager 
    PRIVATE 
    taskMod
    apiMod 
    oatpp::oatpp
)
