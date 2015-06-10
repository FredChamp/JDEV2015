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
import android.widget.SeekBar;
import android.widget.Button;
import android.util.Log;

public class LauncherActivity extends NativeActivity {
    
    private static final String TAG = "LauncherActivity";
    
    private SeekBar seekBar;
    // private Button blueButton;
    // private Button redButton;
    // private ImageView m_logo;
    PopupWindow m_popupWindow;
    PopupWindow m_popupSlider;
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
    
    
    public void showSlider()
    {
        if( m_popupSlider != null )
            return;

        m_activity = this;

        this.runOnUiThread(new Runnable()  {
            @Override
            public void run()  {
                LayoutInflater layoutInflater
                = (LayoutInflater)getBaseContext()
                .getSystemService(LAYOUT_INFLATER_SERVICE);
                View popupView = layoutInflater.inflate(R.layout.activity_slider, null);
                m_popupSlider = new PopupWindow(
                        popupView,
                            LayoutParams.WRAP_CONTENT,
                            LayoutParams.MATCH_PARENT);

                LinearLayout mainLayout = new LinearLayout(m_activity);
                // mainLayout.setPadding(0, 0, 0, 0);
                m_activity.setContentView(mainLayout);

                // Show our UI over NativeActivity window
                m_popupSlider.showAtLocation(mainLayout, Gravity.TOP | Gravity.LEFT, 0,0);
                m_popupSlider.update();
                
                seekBar = (SeekBar) popupView.findViewById(R.id.seekBar_opacity);
                seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() 
                {

                    @Override
                    public void onStopTrackingTouch(SeekBar seekBar) {
                        // TODO Auto-generated method stub
                    }

                    @Override
                    public void onStartTrackingTouch(SeekBar seekBar) {
                        // TODO Auto-generated method stub
                    }

                    @Override
                    public void onProgressChanged(SeekBar seekBar, int progress,boolean fromUser) 
                    {
                        Log.i(TAG, "progress =" + progress);
                        changeOpacity(progress);

                    }
                });
                
                    final Button blueButton = (Button) popupView.findViewById(R.id.blue_button);
                    blueButton.setOnClickListener(new View.OnClickListener() {
                        public void onClick(View v) {
                            changeColor(true);
                        }
                    });
                

                    final Button redButton = (Button) popupView.findViewById(R.id.red_button);
                    redButton.setOnClickListener(new View.OnClickListener() {
                        public void onClick(View v) {
                            changeColor(false);
                        }
                    });

            }});
            
         
    }
    
    public static native void changeOpacity(int value);
    public static native void changeColor(boolean color);
 
 }
