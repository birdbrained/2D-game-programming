
package com.SDL2_Application;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;
import org.libsdl.app.SDLActivity;

public class SDL2_Application extends SDLActivity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        /* Create a TextView and set its text to "Hello world" */
        TextView  tv = new TextView(this);
        tv.setText("Hello World!");
        setContentView(tv);
    }
}
