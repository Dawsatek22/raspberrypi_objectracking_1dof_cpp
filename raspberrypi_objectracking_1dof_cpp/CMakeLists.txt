cmake_minimum_required(VERSION 3.25.1) # the current cmake version used
project(raspberry_tracking_robot_dof1)

set(CMAKE_CXX_STANDARD 26) # current standard



# below are the current packages used
find_package(PiPCA9685 REQUIRED)
find_package(OpenCV REQUIRED) # opencv pkg setup
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCAMERA REQUIRED libcamera)

include_directories(${LIBCAMERA_INCLUDE_DIRS} ${OPENCV_INCLUDE_DIRS})
include_directories( ${OpenCV_INCLUDE_DIRECTORIES})

# below are the executables with libraries linked



add_executable(calib src/calibrate.cpp)
add_executable(track_usb src/track_usb.cpp)
add_executable(track_picam src/track_picam.cpp)
add_executable(yellow_usb src/yellow_usbcam.cpp)
add_executable(yellow_picam src/yellow_picam.cpp)

target_link_libraries(yellow_picam  -llccv ${OpenCV_LIBS} )
target_link_libraries(yellow_usb ${OpenCV_LIBS} )
target_link_libraries(track_usb PiPCA9685::PiPCA9685 ${OpenCV_LIBS} )
target_link_libraries(track_picam PiPCA9685::PiPCA9685 -llccv ${OpenCV_LIBS} )
target_link_libraries(calib PiPCA9685::PiPCA9685)


