package com.renfei.ffmpegproject.ffmpeg;

public class FfmpegUtil {

    static {
        System.loadLibrary("native-lib");
    }


    public FfmpegUtil() {
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */


    public native String getFFmepgVersion();



}
