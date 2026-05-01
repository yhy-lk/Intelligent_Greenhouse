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



void setup_scr_screen_home(lv_ui *ui)
{
    //Write codes screen_home
    ui->screen_home = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_home, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_home, LV_SCROLLBAR_MODE_OFF);
    lv_obj_remove_flag(ui->screen_home, LV_OBJ_FLAG_SCROLLABLE);

    //Write style for screen_home, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_background
    ui->screen_home_img_background = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_background, 0, 0);
    lv_obj_set_size(ui->screen_home_img_background, 240, 320);
    lv_obj_add_flag(ui->screen_home_img_background, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_background, &_BackGround_RGB565A8_240x320);
    lv_image_set_pivot(ui->screen_home_img_background, 50,50);
    lv_image_set_rotation(ui->screen_home_img_background, 0);

    //Write style for screen_home_img_background, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_background, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_background, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_control
    ui->screen_home_img_control = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_control, 140, 100);
    lv_obj_set_size(ui->screen_home_img_control, 60, 60);
    lv_obj_add_flag(ui->screen_home_img_control, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_control, &_Control_RGB565A8_60x60);
    lv_image_set_pivot(ui->screen_home_img_control, 50,50);
    lv_image_set_rotation(ui->screen_home_img_control, 0);

    //Write style for screen_home_img_control, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_control, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_overview
    ui->screen_home_img_overview = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_overview, 40, 100);
    lv_obj_set_size(ui->screen_home_img_overview, 60, 60);
    lv_obj_add_flag(ui->screen_home_img_overview, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_overview, &_Overview_RGB565A8_60x60);
    lv_image_set_pivot(ui->screen_home_img_overview, 50,50);
    lv_image_set_rotation(ui->screen_home_img_overview, 0);

    //Write style for screen_home_img_overview, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_overview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_overview, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_setting
    ui->screen_home_img_setting = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_setting, 140, 190);
    lv_obj_set_size(ui->screen_home_img_setting, 60, 60);
    lv_obj_add_flag(ui->screen_home_img_setting, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_setting, &_Settings_RGB565A8_60x60);
    lv_image_set_pivot(ui->screen_home_img_setting, 50,50);
    lv_image_set_rotation(ui->screen_home_img_setting, 0);

    //Write style for screen_home_img_setting, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_setting, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_trend
    ui->screen_home_img_trend = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_trend, 40, 190);
    lv_obj_set_size(ui->screen_home_img_trend, 60, 60);
    lv_obj_add_flag(ui->screen_home_img_trend, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_trend, &_Data_Trend_RGB565A8_60x60);
    lv_image_set_pivot(ui->screen_home_img_trend, 50,50);
    lv_image_set_rotation(ui->screen_home_img_trend, 0);

    //Write style for screen_home_img_trend, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_trend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_trend, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_home
    ui->screen_home_img_home = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_home, 105, 290);
    lv_obj_set_size(ui->screen_home_img_home, 30, 30);
    lv_obj_add_flag(ui->screen_home_img_home, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_home, &_Home_RGB565A8_30x30);
    lv_image_set_pivot(ui->screen_home_img_home, 50,50);
    lv_image_set_rotation(ui->screen_home_img_home, 0);

    //Write style for screen_home_img_home, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_home, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_home, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_label_overview
    ui->screen_home_label_overview = lv_label_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_label_overview, 40, 170);
    lv_obj_set_size(ui->screen_home_label_overview, 60, 20);
    lv_label_set_text(ui->screen_home_label_overview, "overview");
    lv_label_set_long_mode(ui->screen_home_label_overview, LV_LABEL_LONG_WRAP);

    //Write style for screen_home_label_overview, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_overview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_overview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_overview, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_overview, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_overview, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_overview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_overview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_overview, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_overview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_overview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_overview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_overview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_overview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_overview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_label_setting
    ui->screen_home_label_setting = lv_label_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_label_setting, 140, 260);
    lv_obj_set_size(ui->screen_home_label_setting, 60, 20);
    lv_label_set_text(ui->screen_home_label_setting, "setting");
    lv_label_set_long_mode(ui->screen_home_label_setting, LV_LABEL_LONG_WRAP);

    //Write style for screen_home_label_setting, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_setting, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_setting, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_setting, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_setting, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_label_trend
    ui->screen_home_label_trend = lv_label_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_label_trend, 40, 260);
    lv_obj_set_size(ui->screen_home_label_trend, 60, 20);
    lv_label_set_text(ui->screen_home_label_trend, "trend");
    lv_label_set_long_mode(ui->screen_home_label_trend, LV_LABEL_LONG_WRAP);

    //Write style for screen_home_label_trend, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_trend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_trend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_trend, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_trend, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_trend, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_trend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_trend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_trend, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_trend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_trend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_trend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_trend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_trend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_trend, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_label_control
    ui->screen_home_label_control = lv_label_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_label_control, 140, 170);
    lv_obj_set_size(ui->screen_home_label_control, 60, 20);
    lv_label_set_text(ui->screen_home_label_control, "control");
    lv_label_set_long_mode(ui->screen_home_label_control, LV_LABEL_LONG_WRAP);

    //Write style for screen_home_label_control, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_control, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_control, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_control, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_control, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_line_1
    ui->screen_home_line_1 = lv_line_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_line_1, 0, 285);
    lv_obj_set_size(ui->screen_home_line_1, 247, 4);
    static lv_point_precise_t screen_home_line_1[] = {{0, 0},{240, 0}};
    lv_line_set_points(ui->screen_home_line_1, screen_home_line_1, 2);

    //Write style for screen_home_line_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_home_line_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_home_line_1, lv_color_hex(0x757575), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_home_line_1, 100, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_home_line_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_datetext_data_time
    ui->screen_home_datetext_data_time = lv_label_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_datetext_data_time, 40, 40);
    lv_obj_set_size(ui->screen_home_datetext_data_time, 160, 40);
    lv_label_set_text(ui->screen_home_datetext_data_time, "2026/01/27");
    lv_obj_set_style_text_align(ui->screen_home_datetext_data_time, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_add_flag(ui->screen_home_datetext_data_time, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ui->screen_home_datetext_data_time, screen_home_datetext_data_time_event_handler, LV_EVENT_ALL, NULL);

    //Write style for screen_home_datetext_data_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_home_datetext_data_time, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_datetext_data_time, &lv_font_Alatsi_Regular_22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_datetext_data_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_datetext_data_time, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_datetext_data_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_datetext_data_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_home_datetext_data_time, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_home_datetext_data_time, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_home_datetext_data_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_datetext_data_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_datetext_data_time, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_datetext_data_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_datetext_data_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_datetext_data_time, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->screen_home_datetext_data_time, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->screen_home_datetext_data_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->screen_home_datetext_data_time, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui->screen_home_datetext_data_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui->screen_home_datetext_data_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_home.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_home);

    //Init events for screen.
    events_init_screen_home(ui);
}
