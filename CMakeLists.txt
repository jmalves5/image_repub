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
cv_bridge
OpenCV
message_generation
genmsg)


include_directories()

add_message_files(
	FILES
	CharArray.msg
	Bool.msg
	Byte.msg
	ByteMultiArray.msg
	Char.msg
	ColorRGBA.msg
	Duration.msg
	Empty.msg
	Float32.msg
	Float32MultiArray.msg
	Float64.msg
	Float64MultiArray.msg
	Header.msg
	Int16.msg
	Int16MultiArray.msg
	Int32.msg
	Int32MultiArray.msg
	Int64.msg
	Int64MultiArray.msg
	Int8.msg
	Int8MultiArray.msg
	MultiArrayDimension.msg
	MultiArrayLayout.msg
	String.msg
	Time.msg
	UInt16.msg
	UInt16MultiArray.msg
	UInt32.msg
	UInt32MultiArray.msg
	UInt64.msg
	UInt64MultiArray.msg
	UInt8.msg
	UInt8MultiArray.msg
)


generate_messages(
	DEPENDENCIES
	std_msgs
)

## Declare a catkin package
catkin_package(
CATKIN_DEPENDS message_runtime
)

include_directories(${catkin_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

add_executable(compress src/image_com.cpp)
add_executable(decompress src/image_decom.cpp)
target_link_libraries(compress ${catkin_LIBRARIES})
target_link_libraries(decompress ${catkin_LIBRARIES})
add_dependencies(compress cv_bridge depth_image_proc)
add_dependencies(decompress cv_bridge depth_image_proc)