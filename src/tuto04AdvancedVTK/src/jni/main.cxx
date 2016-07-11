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

#include <vtkNew.h>
#include <vtkActor.h>
#include <vtkCallbackCommand.h>
#include <vtkCamera.h>
#include "vtkCommand.h"
#include <vtkDebugLeaks.h>
#include <vtkGlyph3D.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCubeSource.h>
#include <vtkSphereSource.h>
#include <vtkProperty.h>
#include <vtkInteractorStyleMultiTouchCamera.h>
#include <vtkAndroidRenderWindowInteractor.h>

#include <unistd.h>


#include <android/log.h>
#include <android/input.h>
#include <android_native_app_glue.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "NativeVTK", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "NativeVTK", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "NativeVTK", __VA_ARGS__))

#define CLASS_NAME "com/jdev2015/NativeVTK/LauncherActivity"

static android_app* s_app = NULL;
static vtkActor* s_cubeActor = NULL;
static vtkActor* s_sphereActor = NULL;
static vtkRenderer* renderer = NULL;

static double s_opacity = 1.;

//------------------------------------------------------------------------------

void callActivityVoidMethod(struct android_app* app, const char* method_name )
{
    JNIEnv *jni;
    app->activity->vm->AttachCurrentThread( &jni, NULL );

    //Default class retrieval
    jclass clazz = jni->GetObjectClass( app->activity->clazz );
    jmethodID methodID = jni->GetMethodID( clazz, method_name, "()V" );
    jni->CallVoidMethod( app->activity->clazz, methodID );
    
    app->activity->vm->DetachCurrentThread();
}

//------------------------------------------------------------------------------

void registerNatives(struct android_app* app,JNINativeMethod* methods, int nbMethods)
{
    JNIEnv *jni;
    app->activity->vm->AttachCurrentThread( &jni, NULL );
    jclass clazz = jni->GetObjectClass( app->activity->clazz );
    jni->RegisterNatives(clazz, methods, nbMethods);
    app->activity->vm->DetachCurrentThread();
}

//------------------------------------------------------------------------------

void cubeActorCallbackMethod(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData)
{
    vtkActor *iren = static_cast<vtkActor*>(caller);
     LOGI("Callback: vtkActor modified");
     
     int8_t cmd = APP_CMD_WINDOW_REDRAW_NEEDED;
     if (write(s_app->msgwrite, &cmd, sizeof(cmd)) != sizeof(cmd)) {
         LOGE("Failure writing android_app cmd: %s\n", strerror(errno));
     }
}

//------------------------------------------------------------------------------

static void changeOpacity(JNIEnv *env, jobject thiz, int value)
{
    LOGI("C++ opacity = %d",value);
    
    vtkActor* current;
    if(s_cubeActor->GetVisibility())
    {
        current = s_cubeActor;
    }
    else if(s_sphereActor->GetVisibility())
    {
        current = s_sphereActor;
    }
    
    s_opacity = static_cast< double >(value) / 100.;
    current->GetProperty()->SetOpacity(s_opacity);
    current->Modified();
}

//------------------------------------------------------------------------------

static void changeColor(JNIEnv *env, jobject thiz, bool color)
{
    vtkActor* current;
    if(s_cubeActor->GetVisibility())
    {
        current = s_cubeActor;
    }
    else if(s_sphereActor->GetVisibility())
    {
        current = s_sphereActor;
    }
    
    if(color)
    {
        LOGI("C++ color = BLUE");
        current->GetProperty()->SetColor(0.2, 0.2, 1.);
    }
    else
    {
        LOGI(" C++ color = RED");
        current->GetProperty()->SetColor(1., 0.2, 0.2); 
    }
    current->Modified();
}

//------------------------------------------------------------------------------

static void showSphere(JNIEnv *env, jobject thiz)
{
    LOGI("C++ showSphere");
     
    s_cubeActor->VisibilityOff();
    s_sphereActor->VisibilityOn();
    s_sphereActor->GetProperty()->SetOpacity(s_opacity);
    
    s_cubeActor->Modified();
    s_sphereActor->Modified();
}

//------------------------------------------------------------------------------

static void showCube(JNIEnv *env, jobject thiz)
{
    LOGI("C++ showCube");
    
    s_sphereActor->VisibilityOff();
    s_cubeActor->VisibilityOn();
    s_cubeActor->GetProperty()->SetOpacity(s_opacity);
    
    s_sphereActor->Modified();
    s_cubeActor->Modified();
}

//------------------------------------------------------------------------------

 
/**
* This is the main entry point of a native application that is using
* android_native_app_glue.  It runs in its own thread, with its own
* event loop for receiving input events and doing other things.
*/
void android_main(struct android_app* app)
{
    // Make sure glue isn't stripped.
    app_dummy();
    s_app = app;
    
    JNINativeMethod methods[] =
    {
        {"changeOpacity", "(I)V", reinterpret_cast<void *>(changeOpacity)},
        {"changeColor", "(Z)V", reinterpret_cast<void *>(changeColor)},
        {"showSphere", "()V", reinterpret_cast<void *>(showSphere)},
        {"showCube", "()V", reinterpret_cast<void *>(showCube)}
    };
    registerNatives(app,methods, sizeof(methods) / sizeof(methods[0]));

    vtkNew<vtkRenderWindow> renWin;
    // vtkNew<vtkRenderer> renderer;
    renderer = vtkRenderer::New();
    vtkNew<vtkAndroidRenderWindowInteractor> iren;
    iren->SetInteractorStyle( vtkInteractorStyleMultiTouchCamera::New() );

    iren->SetAndroidApplication(app);

    renWin->AddRenderer(renderer);
    iren->SetRenderWindow(renWin.Get());

    vtkNew<vtkCubeSource> cube;
    cube->SetXLength(8);
    cube->SetYLength(8);
    cube->SetZLength(8);
    
    vtkNew<vtkSphereSource> sphere;
    sphere->SetRadius(5);
    sphere->SetThetaResolution(20);
    sphere->SetPhiResolution(20);

    vtkNew<vtkPolyDataMapper> cubeMapper;
    cubeMapper->SetInputConnection(cube->GetOutputPort());
    
    vtkNew<vtkPolyDataMapper> sphereMapper;
    sphereMapper->SetInputConnection(sphere->GetOutputPort());
    
    vtkNew<vtkActor> cubeActor;
    cubeActor->SetMapper(cubeMapper.Get());
    cubeActor->VisibilityOff();
    
    vtkNew<vtkActor> sphereActor;
    sphereActor->SetMapper(sphereMapper.Get());
    
    s_cubeActor = cubeActor.Get();
    s_sphereActor = sphereActor.Get();

    
    renderer->AddActor(sphereActor.Get());
    renderer->AddActor(cubeActor.Get());
    renderer->SetBackground(0.26,0.28,0.41);
    renderer->ResetCamera();
    renderer->SetLightFollowCamera(true);
    
    vtkNew<vtkCallbackCommand> actorCallback;
    actorCallback->SetCallback ( cubeActorCallbackMethod );
    cubeActor->AddObserver ( vtkCommand::ModifiedEvent, actorCallback.Get() );
    sphereActor->AddObserver ( vtkCommand::ModifiedEvent, actorCallback.Get() );

    renWin->Render();

    callActivityVoidMethod(app,"showUI");
    callActivityVoidMethod(app,"showSlider");
    callActivityVoidMethod(app,"showRadioButton");
 
    iren->Start();
}
