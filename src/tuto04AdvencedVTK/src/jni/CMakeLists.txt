################# VTK #################

find_package(VTK COMPONENTS
  vtkRenderingOpenGL2
  vtkInteractionStyle
  vtkIOLegacy
  REQUIRED
)
include(${VTK_USE_FILE})

find_library(ANDROID_NATIVE_APP_GLUE_LIB NAMES android_native_app REQUIRED)

set(sources
  main.cxx
)

add_library(${PROJECT_NAME} SHARED ${sources})

target_link_libraries(${PROJECT_NAME}
  ${VTK_LIBRARIES}
  ${ANDROID_NATIVE_APP_GLUE_LIB}
  android
  log
)
