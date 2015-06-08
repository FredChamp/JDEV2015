cmake_minimum_required(VERSION 2.8)
project(NativeActivity)

include(${CMAKE_SOURCE_DIR}/../cmake/droid_config.cmake REQUIRED)

set(ANDROID_APK_SUBDOMAIN ${PROJECT_NAME} CACHE STRING
     "Any subdomains (follow the package naming conventions (http://en.wikipedia.org/wiki/Java_package#Package_naming_conventions))"
    FORCE
)

if(EXTERNAL_LIBRARIES)
    list(APPEND CMAKE_FIND_ROOT_PATH ${EXTERNAL_LIBRARIES})
    
    add_subdirectory(jni)

    include(${CMAKE_SOURCE_DIR}/../cmake/droid_setup.cmake REQUIRED)

    set(CMAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/../cmake)
    set(APK_BUILD_DIR ${CMAKE_BINARY_DIR}/apk)

    configure_file("${CMAKE_DIRECTORY}/droid_builder.cmake.in" "${CMAKE_BINARY_DIR}/CMakeFiles/droid_builder.cmake" @ONLY)
    install(SCRIPT "${CMAKE_BINARY_DIR}/CMakeFiles/droid_builder.cmake")
endif()




