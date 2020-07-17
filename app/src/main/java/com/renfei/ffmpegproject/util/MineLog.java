package com.renfei.ffmpegproject.util;

import android.util.Log;

import com.renfei.ffmpegproject.Sign;

public class MineLog implements Sign {

    public static void d(String msg) {

        Log.d(LOG_TAG, msg);
    }
}
