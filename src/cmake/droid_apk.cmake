##################################################
#  Create APK macro
##################################################

macro(create_apk)
    
    if( ${APK_BUILD_TOOL} STREQUAL "ant" )
        include(${CMAKE_DIRECTORY}/ant_build.cmake REQUIRED)
    elseif( ${APK_BUILD_TOOL} STREQUAL "gradle" )
        include(${CMAKE_DIRECTORY}/gradle_build.cmake REQUIRED)
    endif()

    set(ANDROID_APK_THEME "android:theme=\"@android:style/Theme.NoTitleBar.Fullscreen\"")
        
    execute_process(
        COMMAND ${ANDROID_PRG} update project -t android-${ANDROID_API_LEVEL} --name "${BASE_APP_NAME}" --path "${APK_INSTALL_DIR}"
    )

    message("Coping files to apk build directory:")
    ##################################################
    #  Copy java files
    ##################################################
    # Copy "build.xml", "default.properties", "local.properties" and "proguard.cfg" files
    # and copy Java file which is responsible for loading in the required shared librarie
    set(DIR_LIST ${SOURCE_DIR} ${ADDITIONAL_PROJECTS})
    message("- Coping Java files ...")
    foreach(CURRENT_DIR ${DIR_LIST})
        file(GLOB_RECURSE  FILES_LIST FOLLOW_SYMLINKS ${CURRENT_DIR}/*.java)
        foreach(CURRENT_FILE ${FILES_LIST})
            get_filename_component(CURRENT_NAME ${CURRENT_FILE} NAME)
            configure_file(${CURRENT_FILE}   "${APK_INSTALL_DIR}/src/${ANDROID_APK_TOP_LEVEL_DOMAIN}/${ANDROID_APK_DOMAIN}/${ANDROID_APK_SUBDOMAIN}/${CURRENT_NAME}")
        endforeach()
    endforeach()
    
    ##################################################
    #  Copy libs
    ##################################################
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E make_directory "${APK_INSTALL_DIR}/libs/${ARM_TARGET}"
    )
    
    #JAR files:
    message("- Coping JAR libraries ...")
    foreach(JAR_FILE ${JAR_LIBS})
        execute_process(
             COMMAND ${CMAKE_COMMAND} -E copy_if_different "${JAR_FILE}" "${APK_INSTALL_DIR}/libs"
         )
    endforeach()
    
    #Java files:
    # Path variables are define in droid_setup.cmake
    # message("- Coping Java files ...")
    # foreach(JAVA_FILE ${JAVA_FILES})
    #     get_filename_component(CURRENT_NAME ${JAVA_FILE} NAME)
    #     execute_process(
    #          COMMAND ${CMAKE_COMMAND} -E copy_if_different "${JAVA_FILE}" "${APK_INSTALL_DIR}/src/${QT_JAVA_PATH}/${CURRENT_NAME}"
    #      )
    # endforeach()
    
    message("- Coping libraries ...")
    # Copy the used shared libraries
    foreach(value ${LIBS})
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${value} "${APK_INSTALL_DIR}/libs/${ARM_TARGET}"
        )
    endforeach()
    
    ##################################################
    #  Copy assets
    ##################################################
    message("- Coping assets ...")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E make_directory "${APK_INSTALL_DIR}/assets"
    )

    foreach(value ${LIBS_ASSETS})
        
        if(EXISTS "${CMAKE_BINARY_DIR}/${value}" )
           set(src ${CMAKE_BINARY_DIR}/${value})
        else()
           set(src ${EXTERNAL_LIBRARIES}/${value})
        endif()

        set(dest "${APK_INSTALL_DIR}/assets/${value}")
        #message(" src = ${src}")
        execute_process(
             COMMAND ${CMAKE_COMMAND} -E copy_if_different ${src} ${dest}
         )
         
    endforeach()

    # Uninstall previous version from the device/emulator (else we may get e.g. signature conflicts)
    message("Uninstall previous apk")
    execute_process(
        COMMAND ${ADB_PRG} uninstall "${ANDROID_APK_PACKAGE}"
    )

    # Build the apk file
    if( ${APK_BUILD_TOOL} STREQUAL "ant" )
        ant_build()
    elseif( ${APK_BUILD_TOOL} STREQUAL "gradle" )
        gradle_build()
    endif()

    #Start the application
    execute_process(
        COMMAND ${ADB_PRG} shell am start -n ${ANDROID_APK_PACKAGE}/.LauncherActivity
        WORKING_DIRECTORY "${APK_INSTALL_DIR}"
    )
    
endmacro(create_apk)
