package com.renfei.ffmpegproject.bean;

public class MenuInfo {
    private String menuName;
    private String menuValue;

    public MenuInfo(String menuName, String menuValue) {
        this.menuName = menuName;
        this.menuValue = menuValue;
    }

    public String getMenuName() {
        return menuName;
    }

    public void setMenuName(String menuName) {
        this.menuName = menuName;
    }

    public String getMenuValue() {
        return menuValue;
    }

    public void setMenuValue(String menuValue) {
        this.menuValue = menuValue;
    }
}
