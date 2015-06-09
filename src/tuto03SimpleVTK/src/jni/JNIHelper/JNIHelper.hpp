/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2014-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __JNIHELPER_HPP__
#define __JNIHELPER_HPP__

#include <jni.h>

#include <android/log.h>
#include <android_native_app_glue.h>

#include <string>


class JNIHelper
{
    
public:
    /// This methods need to be called before any call to the helper class.
   static void init(ANativeActivity* activity, const std::string& helper_class_name);

    /// Retrieve the singleton object of the helper.
    static JNIHelper* getInstance();


    int callIntMethod(const std::string& method);
    void callMethodInt(const std::string& method, int arg1);
    void callStaticMethodInt(const std::string& method, int arg1);

    bool callBoolMethodString(const std::string& method, const std::string& arg1);
    void callMethodBool(const std::string& method, bool arg1);
    void callStaticMethodBool(const std::string& method, bool arg1);

    void callMethod(const std::string& method);

    void registerNatives(JNINativeMethod* methods, int nbMethods);

private:
    JNIHelper();
    virtual ~JNIHelper();
    jclass retrieveClass( JNIEnv *jni, const std::string& class_name );

    ANativeActivity* m_activity;
    jobject m_javaRef;
    jclass m_javaClass;

    /// mutex for synchronization
    mutable pthread_mutex_t m_mutex;
};

#endif /* __JNIHELPER_HPP__ */
