cmake_minimum_required(VERSION 3.7)
project(Parsival)

set(CMAKE_CXX_STANDARD 11)

find_package (Threads)

include_directories(${CMAKE_SOURCE_DIR}/parsival_robot)
include_directories(${CMAKE_SOURCE_DIR})
set(SOURCE_FILES main.cpp parsival_robot/DirectPose.h parsival_robot/JointController.h parsival_robot/Pose.h parsival_robot/Movement.h parsival_robot/MovementController.h parsival_robot/MovementList.h parsival_robot/MovementTurnLeft.h parsival_robot/MovementTurnRight.h parsival_robot/Parsival.h parsival_robot/RelaxPoseMovement.h parsival_robot/StaticTimePose.h parsival_robot/TimedPose.h HardwareSerial.h Stream.h Arduino.h)
add_executable(Parsival ${SOURCE_FILES})
target_link_libraries (Parsival ${CMAKE_THREAD_LIBS_INIT})
