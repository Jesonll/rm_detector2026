cmake_minimum_required(VERSION 3.8)
project(rm_detector2026)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(image_transport REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rm_interfaces REQUIRED)
find_package(rm_utils REQUIRED)
find_package(OpenCV REQUIRED)
find_package(OpenVINO REQUIRED)

include_directories(include)

# Add library target (component)
add_library(${PROJECT_NAME}_lib SHARED
  src/pipeline/pipeline.cpp
  src/pipeline/preprocessors/yolo_preprocessor.cpp
  src/pipeline/engines/openvino_engine.cpp
  src/pipeline/postprocessors/nms_postprocessor.cpp
  src/pipeline/decoders/yolo_decoder.cpp
  src/factories/preprocessor_factory.cpp
  src/factories/engine_factory.cpp
  src/factories/decoder_factory.cpp
  src/factories/postprocessor_factory.cpp
  src/node/detector_node.cpp
)

target_include_directories(${PROJECT_NAME}_lib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  # Add src directory to include path to find private headers (e.g. concrete implementations)
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

ament_target_dependencies(${PROJECT_NAME}_lib
  rclcpp
  cv_bridge
  sensor_msgs
  image_transport
  std_msgs
  rm_interfaces
  rm_utils
  OpenCV
)
target_link_libraries(${PROJECT_NAME}_lib
  openvino::runtime
)

# Create standalone executable
add_executable(${PROJECT_NAME}_node src/node/main.cpp)
target_link_libraries(${PROJECT_NAME}_node
  ${PROJECT_NAME}_lib
)
ament_target_dependencies(${PROJECT_NAME}_node rclcpp)

# Install standalone executable
install(TARGETS ${PROJECT_NAME}_node
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

# Install targets
install(TARGETS ${PROJECT_NAME}_lib
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# Install config files
install(DIRECTORY config
  DESTINATION share/${PROJECT_NAME}
)

# Install launch files
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

# Install test script
install(PROGRAMS scripts/test_image_feed.py
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and you want to use cpplint
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()

  find_package(ament_cmake_gtest REQUIRED)
  
  ament_add_gtest(test_preprocessors test/test_preprocessors.cpp)
  target_link_libraries(test_preprocessors ${PROJECT_NAME}_lib)

  ament_add_gtest(test_engines test/test_engines.cpp)
  target_link_libraries(test_engines ${PROJECT_NAME}_lib)

  ament_add_gtest(test_postprocessors test/test_postprocessors.cpp)
  target_link_libraries(test_postprocessors ${PROJECT_NAME}_lib)
endif()

ament_package()
