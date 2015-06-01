################# Find lib #################

find_library(ANDROID_NATIVE_APP_GLUE_LIB NAMES android_native_app REQUIRED)
find_library(EGL_LIB NAME EGL REQUIRED)
message(" EGL = ${egl}")
set(sources
  main.c
)

add_library(${PROJECT_NAME} SHARED ${sources})

target_link_libraries(${PROJECT_NAME}
  ${ANDROID_NATIVE_APP_GLUE_LIB}
  android
  log
  EGL
  GLESv1_CM
)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${EXTERNAL_LIBRARIES}/include)
