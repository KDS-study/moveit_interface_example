cmake_minimum_required(VERSION 2.8.3)
project(moveit_interface_example)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  moveit_commander
  moveit_core
  moveit_msgs
  moveit_ros_planning_interface
  moveit_visual_tools
  roscpp
)

catkin_package(INCLUDE_DIRS)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(moveit_interface_example
  src/moveit_interface_example.cpp
)

add_dependencies(moveit_interface_example ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(moveit_interface_example
  ${catkin_LIBRARIES}
)
