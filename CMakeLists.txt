cmake_minimum_required(VERSION 2.8.3)
project(image_repub)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS 
roscpp 
rospy 
std_msgs 
genmsg 
image_transport 
depth_image_proc
cv_bridge)

## Declare a catkin package
catkin_package()

include_directories(${catkin_INCLUDE_DIRS})

add_executable(repub src/image_repub.cpp)
target_link_libraries(repub ${catkin_LIBRARIES})
add_dependencies(repub depth_image_proc cv_bridge)