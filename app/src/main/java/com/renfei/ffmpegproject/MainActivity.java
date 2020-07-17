package com.renfei.ffmpegproject;

import android.Manifest;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.chad.library.adapter.base.BaseQuickAdapter;
import com.chad.library.adapter.base.listener.OnItemClickListener;
import com.renfei.ffmpegproject.act.DecodeAudioDataActivity;
import com.renfei.ffmpegproject.adapter.OpenGLCourseAdapter;
import com.renfei.ffmpegproject.base.BaseActivity;

import java.util.ArrayList;
import java.util.List;

import butterknife.BindView;
import pub.devrel.easypermissions.EasyPermissions;

public class MainActivity extends BaseActivity implements EasyPermissions.PermissionCallbacks {
    private ArrayList<String> courseList;
    private OpenGLCourseAdapter openGLCourseAdapter;


    static final String[] PERMISSIONS = new String[]{
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.MODIFY_AUDIO_SETTINGS

    };
    private static final int PERMISSIONS_READ_STORAGE = 100;

    @BindView(R.id.rv_opengl_Course)
    RecyclerView rvOpenglCourse;

    @Override
    protected int getLayoutId() {
        return R.layout.activity_main;
    }

    @Override
    protected void initListener() {
        courseList = new ArrayList<>();
        courseList.add("1.C++多线程解码音频数据");

        rvOpenglCourse.setLayoutManager(new LinearLayoutManager(this));

        openGLCourseAdapter = new OpenGLCourseAdapter(courseList);

        openGLCourseAdapter.setOnItemClickListener(new OnItemClickListener() {
            @Override
            public void onItemClick(@NonNull BaseQuickAdapter<?, ?> adapter, @NonNull View view, int position) {
                switch (position) {
                    case 0: checkReadStoragePermission();       break;


                }
            }
        });

        rvOpenglCourse.setAdapter(openGLCourseAdapter);
    }


    private void checkReadStoragePermission() {
        if (EasyPermissions.hasPermissions(this, PERMISSIONS)) {
            gotoDecodeAudioPage();
        } else {
            applyPermission();
        }

    }

    private void gotoDecodeAudioPage() {
        startActivity(DecodeAudioDataActivity.class);
    }


    private void applyPermission() {
        EasyPermissions.requestPermissions(this, "申请存储卡权限", PERMISSIONS_READ_STORAGE, PERMISSIONS);
    }


    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        EasyPermissions.onRequestPermissionsResult(requestCode, permissions, grantResults, this);
    }

    @Override
    public void onPermissionsGranted(int requestCode, @NonNull List<String> perms) {
        if (requestCode == PERMISSIONS_READ_STORAGE) {
            gotoDecodeAudioPage();
        }
    }

    @Override
    public void onPermissionsDenied(int requestCode, @NonNull List<String> perms) {
        if (requestCode == PERMISSIONS_READ_STORAGE) {
            showToast("需要获取存储卡读取权限");
        }
    }
}
