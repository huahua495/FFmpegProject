package com.renfei.ffmpegproject.Player;

import android.text.TextUtils;

import com.renfei.ffmpegproject.listener.MineOnParparedListener;
import com.renfei.ffmpegproject.util.MineLog;

public class MinePlayer {

    static {
        System.loadLibrary("native-lib");
    }


    private String source;
    private MineOnParparedListener mineOnParparedListener;

    public MinePlayer() {

    }

    public void setSource(String source) {
        this.source = source;
    }

    public void setMineOnParparedListener(MineOnParparedListener mineOnParparedListener) {
        this.mineOnParparedListener = mineOnParparedListener;
    }

    public void parpared() {
        if (TextUtils.isEmpty(source)) {
            MineLog.d("music source is empty");
            return;
        }

        /**
         * 在子线程中开启 准备操作
         */
        new Thread(new Runnable() {
            @Override
            public void run() {
                    n_parpared(source);
            }
        }).start();
    }

    /**
     * 此方法是供C++调用
     */
    public void onCallParpared(){
        if (null!=mineOnParparedListener){
            mineOnParparedListener.onParpared();
        }
    }


    public void start(){
        if (TextUtils.isEmpty(source)){
            MineLog.d("source is empty");
            return;
        }

        n_start();
    }



    public native void  n_parpared(String source);
    public native void  n_start();
}
