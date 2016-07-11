##################################################
#  Create gradle build macro
##################################################

macro(gradle_build)
    configure_file("${CMAKE_DIRECTORY}/build.gradle" "${APK_INSTALL_DIR}/build.gradle")
    
    set(APK_BUILD_PATH build/outputs/apk)
    if(UNIX )
        if(NOT EXISTS "${WORKING_DIR}/gradlew")
            message("Creating gradle wrapper:")
            execute_process(
                COMMAND ${GRADLE_PRG} wrapper
                WORKING_DIRECTORY "${WORKING_DIR}"
            )
        endif()
        
        set(WRAPPER "${WORKING_DIR}/gradlew")
        
    elseif()
        
        if(NOT EXISTS "${WORKING_DIR}/gradlew.bat")
            message("Creating gradle wrapper:")
            execute_process(
                COMMAND ${GRADLE_PRG} wrapper
                WORKING_DIRECTORY "${WORKING_DIR}"
            )
        endif()
        
        set(WRAPPER "${WORKING_DIR}/gradlew.bat")
        
    endif()
    
    message("ANDROID_APK_KEYSTORE= ${ANDROID_APK_KEYSTORE}")
    
    # Build the apk file
    if(CMAKE_BUILD_TYPE MATCHES Release)
        # Lets gralde create the apk file
        message("\nAPK Release build :")
        execute_process(
            COMMAND ${WRAPPER} assembleRelease --daemon --parallel
            WORKING_DIRECTORY "${APK_INSTALL_DIR}"
        )
        
        set(APK_FINAL_NAME ${BASE_APP_NAME}-release.apk)

    else()
        message("\nAPK Debug build :")
        # Lets gradle create the apk file
        execute_process(
            COMMAND ${WRAPPER} assembleDebug --daemon --parallel
            WORKING_DIRECTORY "${APK_INSTALL_DIR}"
        )

        set(APK_FINAL_NAME ${BASE_APP_NAME}-debug.apk)
        
    endif()
    
    # Install current version on the device/emulator
    execute_process(
        COMMAND ${ADB_PRG} install -rp ${APK_BUILD_PATH}/${APK_FINAL_NAME}
        WORKING_DIRECTORY "${APK_INSTALL_DIR}"
)
        
endmacro(gradle_build)