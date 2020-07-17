package com.renfei.ffmpegproject.act;

import android.widget.TextView;

import com.renfei.ffmpegproject.Player.MinePlayer;
import com.renfei.ffmpegproject.R;
import com.renfei.ffmpegproject.base.BaseActivity;
import com.renfei.ffmpegproject.listener.MineOnParparedListener;
import com.renfei.ffmpegproject.util.MineLog;

import java.io.File;

import butterknife.BindView;
import butterknife.OnClick;

public class DecodeAudioDataActivity extends BaseActivity {

    @BindView(R.id.tv_ffmpeg_log)
    TextView tvFfmpegLog;

    private MinePlayer minePlayer;
    @Override
    protected int getLayoutId() {
        return R.layout.activity_decode_audio_data;
    }

    @Override
    protected void initListener() {
        minePlayer=new MinePlayer();
        minePlayer.setMineOnParparedListener(new MineOnParparedListener() {
            @Override
            public void onParpared() {
                MineLog.d("MineOnParparedListener   onParpared");
                minePlayer.start();
            }
        });

    }

    @OnClick(R.id.tv_ffmpeg_log)
    public void onViewClicked() {

        String filePath=BASE_PATH+"xjj.mp3";
        File file=new File(filePath);

        if (!file.exists()){
            showToast("文件不存在");
            return;
        }

        minePlayer.setSource(filePath);
        minePlayer.parpared();
    }
}
