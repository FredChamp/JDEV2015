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
#include <vtkProperty.h>
#include <vtkInteractorStyleMultiTouchCamera.h>
#include <vtkAndroidRenderWindowInteractor.h>

#include <android/log.h>
#include <android_native_app_glue.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "NativeVTK", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "NativeVTK", __VA_ARGS__))

#define CLASS_NAME "com/jdev2015/NativeVTK/LauncherActivity"

static vtkActor* s_cubeActor = NULL;
static vtkAndroidRenderWindowInteractor* iren = NULL;
static struct android_app* s_app = NULL;


//
//
// class ActorObserver : public vtkCommand
// {
// public:
//   static ActorObserver *New()
//     {
//     return new ActorObserver;
//     }
//
//   void SetRenderWindow( vtkRenderWindow *renWin)
//     {
//     if (this->RenderWindow)
//       {
//       this->RenderWindow->UnRegister(this);
//       }
//     this->RenderWindow = renWin;
//     this->RenderWindow->Register(this);
//
//     }
//   virtual void Execute(vtkObject *vtkNotUsed(caller),
//                        unsigned long event,
//                        void *vtkNotUsed(calldata))
//     {
//     if(this->RenderWindow != 0)
//       {
//       switch(event)
//         {
//         case vtkCommand::ModifiedEvent:
//           this->RenderWindow->Render();
//           break;
//         }
//       }
//     }
//
// protected:
//   ActorObserver()
//     {
//     this->RenderWindow = 0;
//     }
//   ~ActorObserver()
//     {
//     if(this->RenderWindow)
//       {
//       this->RenderWindow->UnRegister(this);
//       this->RenderWindow = 0;
//       }
//     }
//   vtkRenderWindow *RenderWindow;
// };
 

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

static void changeOpacity(JNIEnv *env, jobject thiz, int value)
{
    
    LOGI(" OPACITY = %d",value);
    double opacity = static_cast< double >(value) / 100.;
    s_cubeActor->GetProperty()->SetOpacity(opacity);
    s_cubeActor->Modified();
    iren->Modified();
    

}

//------------------------------------------------------------------------------

static void changeColor(JNIEnv *env, jobject thiz, bool color)
{
    
    if(color)
    {
        LOGI(" COLOR = BLUE");
        s_cubeActor->GetProperty()->SetColor(0., 0., 1.);
    }
    else
    {
        LOGI(" COLOR = RED");
        s_cubeActor->GetProperty()->SetColor(1., 0., 0.); 
    }
       
    s_cubeActor->Modified();
    iren->Modified();

}

//------------------------------------------------------------------------------

 void actorCallbackMethod(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData)
 {
     vtkActor *iren = static_cast<vtkActor*>(caller);
      LOGI(" ACTOR MODIFIED !!!!");
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
        {"changeColor", "(Z)V", reinterpret_cast<void *>(changeColor)}
    };
    registerNatives(app,methods, sizeof(methods) / sizeof(methods[0]));

    vtkNew<vtkRenderWindow> renWin;
    vtkNew<vtkRenderer> renderer;
    // vtkNew<vtkAndroidRenderWindowInteractor> iren;
    iren = vtkAndroidRenderWindowInteractor::New();
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
    s_cubeActor = vtkActor::New();
    s_cubeActor->SetMapper(cubeMapper.Get());

    renderer->AddActor(s_cubeActor);
    renderer->SetBackground(0.4,0.5,0.6);
    renderer->ResetCamera();
    
    vtkNew<vtkCallbackCommand> actorCallback;
    actorCallback->SetCallback ( actorCallbackMethod );
    s_cubeActor->AddObserver ( vtkCommand::ModifiedEvent, actorCallback.Get() );

    renWin->Render();

    callActivityVoidMethod(app,"showUI");
    callActivityVoidMethod(app,"showSlider");
 
 
    iren->Start();
    
}
