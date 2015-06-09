/*
* Copyright (C) 2010 The Android Open Source Project
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

#include <jni.h>
#include <errno.h>

#include "JNIHelper/JNIHelper.hpp"

#include <vtkNew.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkDebugLeaks.h>
#include <vtkGlyph3D.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCubeSource.h>
#include <vtkProperty.h>
#include <vtkInteractorStyleMultiTouchCamera.h>
#include <vtkAndroidRenderWindowInteractor.h>

#include <android/log.h>
#include <android_native_app_glue.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "NativeVTK", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "NativeVTK", __VA_ARGS__))

#define CLASS_NAME "com/jdev2015/NativeVTK/LauncherActivity"

/**
* This is the main entry point of a native application that is using
* android_native_app_glue.  It runs in its own thread, with its own
* event loop for receiving input events and doing other things.
*/

static void callActivityVoidMethod(struct android_app* app, const char* method_name )
{
    JNIEnv *jni;
    app->activity->vm->AttachCurrentThread( &jni, NULL );

    //Default class retrieval
    jclass clazz = jni->GetObjectClass( app->activity->clazz );
    jmethodID methodID = jni->GetMethodID( clazz, method_name, "()V" );
    jni->CallVoidMethod( app->activity->clazz, methodID );
    
    app->activity->vm->DetachCurrentThread();
}

void android_main(struct android_app* app)
{
    // Make sure glue isn't stripped.
    app_dummy();

    vtkNew<vtkRenderWindow> renWin;
    vtkNew<vtkRenderer> renderer;
    vtkNew<vtkAndroidRenderWindowInteractor> iren;
    iren->SetInteractorStyle( vtkInteractorStyleMultiTouchCamera::New() );

    iren->SetAndroidApplication(app);

    renWin->AddRenderer(renderer.Get());
    iren->SetRenderWindow(renWin.Get());

    vtkNew<vtkCubeSource> cube;
    cube->SetXLength(8);
    cube->SetYLength(8);
    cube->SetZLength(8);

    vtkNew<vtkPolyDataMapper> cubeMapper;
    cubeMapper->SetInputConnection(cube->GetOutputPort());
    vtkNew<vtkActor> cubeActor;
    cubeActor->SetMapper(cubeMapper.Get());

    renderer->AddActor(cubeActor.Get());
    renderer->SetBackground(0.4,0.5,0.6);
    renderer->ResetCamera();

    renWin->Render();

    callActivityVoidMethod(app,"showUI");
 
    iren->Start();
    
}
