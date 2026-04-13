/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_screen_trend(lv_ui *ui)
{
    //Write codes screen_trend
    ui->screen_trend = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_trend, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_trend, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_trend, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_trend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_trend_img_1
    ui->screen_trend_img_1 = lv_image_create(ui->screen_trend);
    lv_obj_set_pos(ui->screen_trend_img_1, 0, 0);
    lv_obj_set_size(ui->screen_trend_img_1, 240, 320);
    lv_obj_add_flag(ui->screen_trend_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_trend_img_1, &_Control_Mode_Background_RGB565A8_240x320);
    lv_image_set_pivot(ui->screen_trend_img_1, 50,50);
    lv_image_set_rotation(ui->screen_trend_img_1, 0);

    //Write style for screen_trend_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_trend_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_trend_img_1, 181, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_trend.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_trend);

    //Init events for screen.
    events_init_screen_trend(ui);
}
