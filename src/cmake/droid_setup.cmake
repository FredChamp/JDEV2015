##################################################
#  Variables usefull for apk buld
##############################################
set(LIB_PREFIX "lib")
set(QT_JAVA_ROOT src/android/java/src)
set(QT_JAVA_PATH org/qtproject/qt5/android/bindings)

##################################################
#  Macro
##################################################

# Setup dependencies for apk build
macro(setup_dependencies)
    unset(LIBS)
    # Find programs android, adb, ant, jarsigner and zipalign
    find_program(NDK_DEPENDS_PRG ndk-depends NAMES ndk-depends.bat PATHS ${ANDROID_NDK} NO_CMAKE_FIND_ROOT_PATH )
    if (NOT NDK_DEPENDS_PRG)
        message(FATAL_ERROR "ndk-depends command is not found.")
    endif()
    
    file(GLOB_RECURSE DEPENDENCIES ${WORKING_DIR}/lib/*)
    foreach(CURRENT_FILE ${DEPENDENCIES})
        get_filename_component(EXTENSION ${CURRENT_FILE} EXT)
        if("${EXTENSION}" STREQUAL ".so")
            execute_process(
                COMMAND ${NDK_DEPENDS_PRG} --print-paths -L ${EXTERNAL_LIBRARIES}/lib ${CURRENT_FILE}
                OUTPUT_VARIABLE DEPENDS_OUTPUT
            ) 
        endif()
        if(DEPENDS_OUTPUT)
            string(REPLACE "\n" ";" DEPENDS_LINE "${DEPENDS_OUTPUT}")
            
            string(LENGTH "${DEPENDS_LINE}" LINE_LENGHT)
            MATH( EXPR FINAL_LENGTH "${LINE_LENGHT} - 1 " )
            string(SUBSTRING "${DEPENDS_LINE}" 0 ${FINAL_LENGTH} DEPENDS_LINE)

            foreach(CURRENT_DEPS ${DEPENDS_LINE})
                string(LENGTH "${CURRENT_DEPS}" DEP_LENGHT)
                string(FIND "${CURRENT_DEPS}" "->" ARROW_POSITION)
                MATH( EXPR BEGIN_POSTION "${ARROW_POSITION} + 3 " )
                MATH( EXPR FINAL_LENGTH "${DEP_LENGHT} - ${BEGIN_POSTION} " )
                string(SUBSTRING "${CURRENT_DEPS}" ${BEGIN_POSTION} ${FINAL_LENGTH} DEPS_PATH)
                string(FIND "${DEPS_PATH}" "$" TEST_SYSTEM)
                string(FIND "${DEPS_PATH}" "!!" TEST_EXT)

                if(${TEST_SYSTEM} EQUAL -1)
                    if(NOT ${TEST_EXT} EQUAL -1)
                        MATH( EXPR FINAL_POSTION "${ARROW_POSITION} - 1 " )
                        string(SUBSTRING "${CURRENT_DEPS}" 0 ${FINAL_POSTION} EXT_PATH)
                    else()
                        list(APPEND LIBS "${DEPS_PATH}")
                    endif()
                endif()
            endforeach()
            
        endif()
    endforeach()
    if(LIBS)
        list(REMOVE_DUPLICATES LIBS)
    else()
        message( FATAL_ERROR "No libraries found !")
    endif()
    
endmacro(setup_dependencies)