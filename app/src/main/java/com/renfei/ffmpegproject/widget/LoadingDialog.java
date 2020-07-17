package com.renfei.ffmpegproject.widget;

import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.view.Display;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;

import com.renfei.ffmpegproject.R;


/**
 * Created by Luo on 2016/9/23.
 * desc:加载等待的Dialog
 */
public class LoadingDialog {
    private Dialog dialog;
    private Context context;
    private TextView tv_text;

    public LoadingDialog(Context context) {
        this.context = context;
    }

    public Dialog show(String message) {
        View view = LayoutInflater.from(this.context).inflate(R.layout.dg_loading,
                null);
        this.tv_text = ((TextView) view.findViewById(R.id.tv_text));
        this.tv_text.setText(message);
        this.dialog = new Dialog(this.context, R.style.custom_dialog);
        this.dialog.setContentView(view);
        this.dialog.setCanceledOnTouchOutside(false);
        this.dialog.setCancelable(false);
        this.dialog.show();
        WindowManager m = ((Activity) this.context).getWindowManager();
        Display d = m.getDefaultDisplay();
        Window dialogWindow = this.dialog.getWindow();
        dialogWindow.setGravity(Gravity.CENTER);
        WindowManager.LayoutParams p = dialogWindow.getAttributes();
        p.height = -1;
        p.width = -1;
        dialogWindow.setAttributes(p);
        return this.dialog;
    }

//    public void setMessage(String message) {
//        if (this.tv_text != null)
//            this.tv_text.setText(message);
//    }

    public boolean isShowing() {
        if (null==this.dialog){
            return false;
        }else {
            return this.dialog.isShowing();
        }
    }

    public void dismiss() {
        if ((this.dialog != null) && (this.dialog.isShowing()))
            this.dialog.dismiss();
    }

}