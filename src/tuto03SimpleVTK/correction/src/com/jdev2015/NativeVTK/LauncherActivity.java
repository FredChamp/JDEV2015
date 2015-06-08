package com.jdev2015.NativeVTK;

public class LauncherActivity extends android.app.NativeActivity {
    static 
    {
       System.loadLibrary("gnustl_shared");
       System.loadLibrary("vtkglew-6.2");
       System.loadLibrary("vtksys-6.2");
       System.loadLibrary("vtkCommonCore-6.2");
       System.loadLibrary("vtkCommonMath-6.2");
       System.loadLibrary("vtkCommonMisc-6.2");
       System.loadLibrary("vtkCommonSystem-6.2");
       System.loadLibrary("vtkCommonTransforms-6.2");
       System.loadLibrary("vtkCommonDataModel-6.2");
       System.loadLibrary("vtkCommonExecutionModel-6.2");
       System.loadLibrary("vtkCommonComputationalGeometry-6.2");
       System.loadLibrary("vtkIOCore-6.2");
       System.loadLibrary("vtkmetaio-6.2");
       System.loadLibrary("vtkDICOMParser-6.2");
       System.loadLibrary("tiff");
       System.loadLibrary("jpeg");
       System.loadLibrary("png16d");
       System.loadLibrary("vtkIOImage-6.2");
       System.loadLibrary("vtkImagingCore-6.2");
       System.loadLibrary("vtkImagingHybrid-6.2");
       
       System.loadLibrary("vtkalglib-6.2");
       System.loadLibrary("vtkImagingFourier-6.2");
       System.loadLibrary("vtkFiltersCore-6.2");
       System.loadLibrary("vtkFiltersGeneral-6.2");
       System.loadLibrary("vtkFiltersStatistics-6.2");
       System.loadLibrary("vtkFiltersSources-6.2");
       System.loadLibrary("vtkFiltersGeometry-6.2");
       System.loadLibrary("vtkFiltersExtraction-6.2");
       // System.loadLibrary("vtkFiltersTexture-6.2");
       
       System.loadLibrary("vtkRenderingCore-6.2");
       System.loadLibrary("vtkRenderingOpenGL2-6.2");
       System.loadLibrary("NativeVTK"); 
       
    }
 }
