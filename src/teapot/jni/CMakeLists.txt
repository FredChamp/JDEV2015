################# Find lib #################

include_directories(
  "${ANDROID_NDK}/sources/android/cpufeatures"
  "${ANDROID_NDK}/sources/android/ndk_helper"
)

find_library(ANDROID_NATIVE_APP_GLUE_LIB NAMES android_native_app REQUIRED)

set(sources
  TeapotNativeActivity.cpp
  TeapotRenderer.cpp
  ${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c
  ${ANDROID_NDK}/sources/android/ndk_helper/gestureDetector.cpp
  ${ANDROID_NDK}/sources/android/ndk_helper/gl3stub.c
  ${ANDROID_NDK}/sources/android/ndk_helper/GLContext.cpp
  ${ANDROID_NDK}/sources/android/ndk_helper/interpolator.cpp
  ${ANDROID_NDK}/sources/android/ndk_helper/JNIHelper.cpp
  ${ANDROID_NDK}/sources/android/ndk_helper/perfMonitor.cpp
  ${ANDROID_NDK}/sources/android/ndk_helper/shader.cpp
  ${ANDROID_NDK}/sources/android/ndk_helper/tapCamera.cpp
  ${ANDROID_NDK}/sources/android/ndk_helper/vecmath.cpp
)

set(headers
    TeapotRenderer.h
    teapot.inl
    ${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.h
    ${ANDROID_NDK}/sources/android/ndk_helper/gestureDetector.h
    ${ANDROID_NDK}/sources/android/ndk_helper/gl3stub.h
    ${ANDROID_NDK}/sources/android/ndk_helper/GLContext.h
    ${ANDROID_NDK}/sources/android/ndk_helper/interpolator.h
    ${ANDROID_NDK}/sources/android/ndk_helper/JNIHelper.h
    ${ANDROID_NDK}/sources/android/ndk_helper/perfMonitor.h
    ${ANDROID_NDK}/sources/android/ndk_helper/shader.h
    ${ANDROID_NDK}/sources/android/ndk_helper/tapCamera.h
    ${ANDROID_NDK}/sources/android/ndk_helper/vecmath.h
)

add_library(${PROJECT_NAME} SHARED ${sources} ${header})

target_link_libraries(${PROJECT_NAME}
  ${ANDROID_NATIVE_APP_GLUE_LIB}
  android
  log
  EGL
  GLESv2
)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${EXTERNAL_LIBRARIES}/include)
