package com.jdev2015.NativeVTK;

import android.app.NativeActivity;
import android.content.res.Configuration;
import android.os.Bundle;
import android.view.Window;

import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.WindowManager.LayoutParams;
import android.widget.LinearLayout;
import android.widget.PopupWindow;
import android.os.Handler;


public class LauncherActivity extends NativeActivity {
    
    // private ImageView m_logo;
    PopupWindow m_popupWindow;
    LauncherActivity m_activity;

    static 
    {
       System.loadLibrary("gnustl_shared");
       System.loadLibrary("NativeVTK"); 
       
    }

    @Override
      protected void onCreate(Bundle savedInstanceState) 
      {

          this.requestWindowFeature(Window.FEATURE_NO_TITLE);
          super.onCreate(savedInstanceState);
         // getWindow().takeSurface( null );
       //   getWindow().setContentView( R.layout.activity_launcher );
    } 
    
    
    public void showUI()
    {
        if( m_popupWindow != null )
            return;

        m_activity = this;

        this.runOnUiThread(new Runnable()  {
            @Override
            public void run()  {
                LayoutInflater layoutInflater
                = (LayoutInflater)getBaseContext()
                .getSystemService(LAYOUT_INFLATER_SERVICE);
                View popupView = layoutInflater.inflate(R.layout.activity_launcher, null);
                m_popupWindow = new PopupWindow(
                        popupView,
                        LayoutParams.WRAP_CONTENT,
                        LayoutParams.WRAP_CONTENT);

                LinearLayout mainLayout = new LinearLayout(m_activity);
                mainLayout.setPadding(0, 0, 0, 0);
                m_activity.setContentView(mainLayout);

                // Show our UI over NativeActivity window
                m_popupWindow.showAtLocation(mainLayout, Gravity.BOTTOM | Gravity.RIGHT, 10, 0);
                m_popupWindow.update();

            }});
    }
 
 }
