cmake_minimum_required(VERSION 2.8)
project(NativeActivity)

set(CMAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/../../cmake)

include(${CMAKE_DIRECTORY}/droid_config.cmake REQUIRED)

if(EXTERNAL_LIBRARIES)
    
    include(${CMAKE_DIRECTORY}/droid_setup.cmake REQUIRED)

    set(ANDROID_APK_SUBDOMAIN ${PROJECT_NAME} CACHE STRING
         "Any subdomains (follow the package naming conventions (http://en.wikipedia.org/wiki/Java_package#Package_naming_conventions))"
         FORCE
)
    set(APK_BUILD_DIR ${CMAKE_BINARY_DIR}/apk)



    list(APPEND CMAKE_FIND_ROOT_PATH ${EXTERNAL_LIBRARIES})

    add_subdirectory(jni)
    configure_file("${CMAKE_DIRECTORY}/droid_builder.cmake.in" "${CMAKE_BINARY_DIR}/CMakeFiles/droid_builder.cmake" @ONLY)
    install(SCRIPT "${CMAKE_BINARY_DIR}/CMakeFiles/droid_builder.cmake")
    
endif()




