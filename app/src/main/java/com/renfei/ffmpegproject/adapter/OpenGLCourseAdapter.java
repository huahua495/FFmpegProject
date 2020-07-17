package com.renfei.ffmpegproject.adapter;

import androidx.annotation.Nullable;

import com.chad.library.adapter.base.BaseQuickAdapter;
import com.chad.library.adapter.base.viewholder.BaseViewHolder;
import com.renfei.ffmpegproject.R;

import java.util.List;

public class OpenGLCourseAdapter extends BaseQuickAdapter<String, BaseViewHolder> {
    public OpenGLCourseAdapter(@Nullable List<String> data) {
        super(R.layout.item_opengl_course, data);
    }

    @Override
    protected void convert(BaseViewHolder baseViewHolder, String s) {
        baseViewHolder
                .setText(R.id.tv_mediamuxer_video,s);
    }
}
