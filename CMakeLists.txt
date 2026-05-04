cmake_minimum_required(VERSION 3.20)

project(TaskManager)
find_package(oatpp 1.4.0 REQUIRED)

add_executable(TaskManager task_manager.cpp)

target_link_libraries(TaskManager  oatpp::oatpp)
