##################################################
#  set usefull variable
##################################################

set(CMAKE_TOOLCHAIN_FILE CACHE FILEPATH "Path to the toolchain file")
set(EXTERNAL_LIBRARIES    ""   CACHE PATH    "Path to external libraries install directory")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(LIBRARY_OUTPUT_DIR lib)

set(APK_BUILD_TOOL    "gradle" CACHE STRING    "Build tool name will be used to create the apk (ant or gradle)")
set(ANDROID_APK_RELEASE          "OFF"                     CACHE BOOL      "Create apk file ready for release? (signed, you have to enter a password during build, do also setup \"ANDROID_APK_SIGNER_KEYSTORE\" and \"ANDROID_APK_SIGNER_ALIAS\")")
set(ANDROID_APK_TOP_LEVEL_DOMAIN "com"                       CACHE STRING    "Top level domain name of the organization (follow the package naming conventions (http://en.wikipedia.org/wiki/Java_package#Package_naming_conventions))")
set(ANDROID_APK_DOMAIN           "jdev2015"                    CACHE STRING    "Organization's domain (follow the package naming conventions (http://en.wikipedia.org/wiki/Java_package#Package_naming_conventions))")
set(ANDROID_APK_SUBDOMAIN        "MyAppName"                 CACHE STRING    "Any subdomains (follow the package naming conventions (http://en.wikipedia.org/wiki/Java_package#Package_naming_conventions))")
set(ANDROID_APK_SIGNER_KEYSTORE  "~/my-release-key.keystore" CACHE STRING    "Keystore for signing the apk file (only required for release apk)")
set(ANDROID_APK_SIGNER_ALIAS     "myalias"                   CACHE STRING    "Alias for signing the apk file (only required for release apk)")
    
add_definitions(-DANDROID_APK_PACKAGE=\"${ANDROID_APK_TOP_LEVEL_DOMAIN}/${ANDROID_APK_DOMAIN}/${ANDROID_APK_SUBDOMAIN}\")

##################################################
#  Find program
##################################################
if(CMAKE_TOOLCHAIN_FILE)
    file(TO_CMAKE_PATH "$ENV{ANDROID_SDK}" ANDROID_SDK_PATH)
    
    #Find android
    find_host_program(ANDROID_PRG
        NAMES android android.bat
        PATHS "${ANDROID_SDK_PATH}/tools/"
        DOC   "The android command-line tool"
    )
    if(NOT ANDROID_PRG)
        message(FATAL_ERROR "Can not find android command line tool: android")
    endif()

    if( ${APK_BUILD_TOOL} STREQUAL "ant" )
        #find ant
        find_host_program(ANT_PRG
            NAMES ant.bat ant
            PATHS /usr/bin/
        DOC   "The ant build tool")
        if(NOT ANT_PRG)
            message(FATAL_ERROR "Can not find ant build tool: ant")
        endif()

    elseif( ${APK_BUILD_TOOL} STREQUAL "gradle" )
        #find gradle
        find_host_program(GRADLE_PRG
            NAMES gradle.bat gradle
            PATHS /usr/bin/
        DOC   "The gradle build tool")
        if(NOT GRADLE_PRG)
            message(FATAL_ERROR "Can not find ant build tool: gradle")
        endif()

    else()
        message(FATAL_ERROR "Unknown apk build tool \"${APK_BUILD_TOOL}\". Set ant or gradle")
    endif()

    #find adb
    find_host_program(ADB_PRG
        NAMES adb adb.exe
        PATHS "${ANDROID_SDK_PATH}/platform-tools/"
    DOC   "The adb install tool")
    if(NOT ADB_PRG)
        message(FATAL_ERROR "Can not find adb install tool: adb")
    endif()

    file(TO_CMAKE_PATH "$ENV{JAVA_HOME}" JAVA_HOME_PATH)
    
    #find jarsigner
    find_host_program(JARSIGNER_PRG
        NAMES jarsigner jarsigner.exe
        PATHS "${JAVA_HOME_PATH}/bin/"
        DOC  "The jarsigner tool (in jdk/bin/)"
    )
    if(NOT JARSIGNER_PRG)
        message(FATAL_ERROR "Can not find jarsigner tool")
    endif()

    #find zipalign
    file(GLOB CHILDREN "${ANDROID_SDK_PATH}/build-tools/*")
    set(DIRLIST "")
    foreach(CHILD ${CHILDREN})
        if(IS_DIRECTORY "${CHILD}")
            list(APPEND DIRLIST ${CHILD})
        endif()
    endforeach()

    find_host_program(ZIPALIGN_PRG
        NAMES zipalign zipalign.exe
        PATHS ${DIRLIST}
    DOC "The zipalign tool")
    if (NOT ZIPALIGN_PRG)
        message(FATAL_ERROR "zipalign command is not found.")
    endif()
endif()

