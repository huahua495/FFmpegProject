package com.renfei.ffmpegproject.adapter;

import androidx.annotation.Nullable;

import com.chad.library.adapter.base.BaseQuickAdapter;
import com.chad.library.adapter.base.viewholder.BaseViewHolder;
import com.renfei.ffmpegproject.R;
import com.renfei.ffmpegproject.bean.MenuInfo;


import java.util.List;

public class VideoInfoAdapter extends BaseQuickAdapter<MenuInfo, BaseViewHolder> {
    public VideoInfoAdapter(@Nullable List<MenuInfo> data) {
        super(R.layout.item_video_info, data);
    }

    @Override
    protected void convert(BaseViewHolder baseViewHolder, MenuInfo menuInfo) {


        baseViewHolder
                .setText(R.id.tv_menu_name, menuInfo.getMenuName())
                .setText(R.id.tv_menu_value, menuInfo.getMenuValue());
    }
}
