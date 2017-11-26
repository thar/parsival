cmake_minimum_required(VERSION 3.5)
project(Parsival)

set(CMAKE_CXX_STANDARD 11)

find_package (Threads)

include_directories(${CMAKE_SOURCE_DIR}/parsival_robot)
include_directories(${CMAKE_SOURCE_DIR}/parsival_robot_pc_debug)
add_definitions(-DPC_VERSION)
set(SOURCE_FILES parsival_robot_pc_debug/main.cpp parsival_robot/DirectPose.h parsival_robot/JointController.h parsival_robot/Pose.h parsival_robot/Movement.h parsival_robot/MovementController.h parsival_robot/MovementList.h parsival_robot/movements/MovementTurnLeft.h parsival_robot/movements/MovementTurnRight.h parsival_robot/Parsival.h parsival_robot/RelaxPoseMovement.h parsival_robot/StaticTimePose.h parsival_robot/TimedPose.h parsival_robot_pc_debug/HardwareSerial.h parsival_robot_pc_debug/Stream.h parsival_robot_pc_debug/Arduino.h parsival_robot/JointControllerInterface.h parsival_robot/SinusoidMovementJoint.h parsival_robot_pc_debug/PololuMaestro.h parsival_robot/JointInterface.h parsival_robot_pc_debug/Arduino.cpp)
add_executable(Parsival ${SOURCE_FILES})
target_link_libraries (Parsival ${CMAKE_THREAD_LIBS_INIT})
