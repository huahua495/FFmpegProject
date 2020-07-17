package com.renfei.ffmpegproject.base;

import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.renfei.ffmpegproject.R;
import com.renfei.ffmpegproject.widget.LoadingDialog;

import butterknife.ButterKnife;
import butterknife.Unbinder;

public abstract class BaseActivity extends AppCompatActivity {

    public static final String PROJECT_TAG = "renfei666";

    private Unbinder unbinder;

    protected LoadingDialog loadingDialog = null;

    protected String BASE_PATH = Environment.getExternalStorageDirectory().getPath() + "/ffmpeg/";

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
            setContentView(getLayoutId());
        unbinder = ButterKnife.bind(this);
        initDialog();
        initListener();
    }

    protected abstract int getLayoutId();


    protected abstract void initListener();




    private void initDialog() {
        loadingDialog = new LoadingDialog(this);
    }


    /**
     * 显示对话框
     */
    protected void showLoadingDialog(String msg) {
        if (isEmpty(msg)) {
            msg = getResources().getString(R.string.loading_wait);
        }
        if (loadingDialog == null) {
            loadingDialog = new LoadingDialog(this);
            loadingDialog.show(msg);
        } else {
            loadingDialog.show(msg);
        }
    }

    protected void showLoadingDialog() {
        String msg = getResources().getString(R.string.loading_wait);
        if (loadingDialog == null) {
            loadingDialog = new LoadingDialog(this);
            loadingDialog.show(msg);
        } else {
            if (!loadingDialog.isShowing()) {
                loadingDialog.show(msg);
            }
        }
    }

    /**
     * 隐藏Dialog弹出框
     */
    protected void dismissDialog() {
        if (loadingDialog != null) {
            loadingDialog.dismiss();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unbinder.unbind();
        if (null != loadingDialog) {
            loadingDialog.dismiss();
        }
    }


    protected void showToast(String msg) {
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }

    public static boolean isEmpty(CharSequence str) {
        return (str == null || "".equals(str) || str.length() == 0 || "null".equals(str));
    }

    protected void loge(String simpleName, String msg) {
        Log.e(simpleName, msg);
    }

    /**
     * 页面跳转
     *
     * @param clazz 要跳转的Activity类
     */
    protected void startActivity(Class clazz) {
        startActivity(new Intent(this, clazz));
    }
}
