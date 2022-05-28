cmake_minimum_required(VERSION 3.5)
project(sh_find_common)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sh_common REQUIRED)
find_package(Boost REQUIRED)
find_package(jsoncpp REQUIRED)

include_directories(
    include
    ${Boost_INCLUDE_DIR}
)

add_library(find_base_node SHARED src/find_base_node.cpp)
ament_target_dependencies(find_base_node
    rclcpp
    sh_common
)
target_link_libraries(find_base_node jsoncpp_lib)

install(
    DIRECTORY include/
    DESTINATION include
)

install(
    TARGETS find_base_node
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

ament_export_include_directories(include)
ament_export_dependencies(ament_cmake rclcpp sh_common Boost jsoncpp)
ament_export_libraries(find_base_node)

ament_package()
