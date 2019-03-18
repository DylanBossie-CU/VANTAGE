cmake_minimum_required(VERSION 3.5)
project(IFM3D_EXAMPLES)

set(CMAKE_MODULE_PATH
    ${IFM3D_EXAMPLES_SOURCE_DIR}/cmake/modules
    ${CMAKE_MODULE_PATH})

# force an out-of-source build of the code
include(MacroOutOfSourceBuild)
macro_ensure_out_of_source_build(
  "Please build ${PROJECT_NAME} out-of-source")

# Conditionally turn on/off parts of the build (global-level)
option(BUILD_MODULE_SIMPLEIMAGE "Build the simpleimage module and example" OFF)
option(BUILD_EXAMPLE_EXPOSURE_TIMES "Build ex-exposure-times example" OFF)
option(BUILD_EXAMPLE_FAST_APP_SWITCH "Build fast-app-switch example" OFF)
option(BUILD_EXAMPLE_FILE_IO "Build opencv based file-io example" ON)
option(BUILD_EXAMPLE_GETMAC "Build getmac example" OFF)
option(BUILD_EXAMPLE_MULTI_CAMERA_GRABBER "Build multi-camera-grabber example" OFF)
option(BUILD_EXAMPLE_PCICCLIENT_ASYNC_MESSAGES "Build pcicclient-async-messages example" OFF)
option(BUILD_EXAMPLE_PCICCLIENT_SET_IO "Build pcicclient-set-io example" OFF)
option(BUILD_EXAMPLE_TIMESTAMP "Build timestamp example" OFF)

################################################
## Bring in dependent projects
################################################
find_package(ifm3d 0.11.0 CONFIG
    REQUIRED COMPONENTS camera framegrabber
    OPTIONAL_COMPONENTS image pcicclient
    )

find_package(PCL 1.7.1 REQUIRED COMPONENTS common io)
find_package(Boost REQUIRED COMPONENTS system)

find_library(LIB_boost_system NAMES boost_system)


# Fix this: https://bugs.launchpad.net/ubuntu/+source/vtk6/+bug/1573234
list(FIND PCL_IO_LIBRARIES "vtkproj4" VTKPROJ4_REQUIRED)
if(NOT VTKPROJ4_REQUIRED EQUAL -1)
    find_library(LIBVTKPROJ4_LIBRARY NAMES vtkproj4)
    if(NOT ${LIBVTKPROJ4_LIBRARY})
        list(REMOVE_ITEM PCL_IO_LIBRARIES "vtkproj4")
    endif()
endif()

link_directories(
  ${PCL_LIBRARY_DIRS}
  ${OpenCV_LIBRARY_DIRS}
  )
add_definitions(
  ${PCL_DEFINITIONS}
  ${OpenCV_DEFINITIONS}
)

################################################
## Build the Modules
################################################
if(BUILD_MODULE_SIMPLEIMAGE)
  add_definitions(-DBUILD_MODULE_SIMPLEIMAGE=ON)
  add_subdirectory(simpleimage)
endif()

################################################
## Build the examples
################################################

if(BUILD_EXAMPLE_GETMAC)
 add_definitions(-DBUILD_EXAMPLE_GETMAC=ON)
 add_subdirectory(getmac)
endif()

if((TARGET ifm3d::pcicclient) AND (TARGET ifm3d::image))
  if(BUILD_EXAMPLE_FAST_APP_SWITCH)
    add_subdirectory(fast_app_switch)
  endif()
endif()

if(TARGET ifm3d::image)
  if(BUILD_EXAMPLE_EXPOSURE_TIMES)
    add_subdirectory(exposure_time)
  endif()

  if(BUILD_EXAMPLE_FILE_IO)
    add_subdirectory(file_io)
  endif()

  if(BUILD_EXAMPLE_MULTI_CAMERA_GRABBER)
    add_subdirectory(multi_camera_grabber)
  endif()

  if(BUILD_EXAMPLE_TIMESTAMP)
    add_subdirectory(timestamp)
  endif()
endif()

if(TARGET ifm3d::pcicclient)
  if(BUILD_EXAMPLE_PCICCLIENT_ASYNC_MESSAGES)
    add_subdirectory(pcicclient_async_messages)
  endif()

  if(BUILD_EXAMPLE_PCICCLIENT_SET_IO)
    add_subdirectory(pcicclient_set_io)
  endif()
endif()

