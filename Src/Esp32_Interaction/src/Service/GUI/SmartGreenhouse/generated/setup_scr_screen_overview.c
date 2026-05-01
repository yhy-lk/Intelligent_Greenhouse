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



void setup_scr_screen_overview(lv_ui *ui)
{
    //Write codes screen_overview
    ui->screen_overview = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_overview, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_overview, LV_SCROLLBAR_MODE_OFF);
    lv_obj_remove_flag(ui->screen_overview, LV_OBJ_FLAG_SCROLLABLE);

    //Write style for screen_overview, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_overview, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overview_img_background
    ui->screen_overview_img_background = lv_image_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_img_background, 0, 0);
    lv_obj_set_size(ui->screen_overview_img_background, 240, 320);
    lv_obj_add_flag(ui->screen_overview_img_background, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_overview_img_background, &_Dashboard_Background_1_RGB565A8_240x320);
    lv_image_set_pivot(ui->screen_overview_img_background, 50,50);
    lv_image_set_rotation(ui->screen_overview_img_background, 0);

    //Write style for screen_overview_img_background, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_overview_img_background, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_overview_img_background, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overview_btn_1
    ui->screen_overview_btn_1 = lv_button_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_btn_1, 0, 0);
    lv_obj_set_size(ui->screen_overview_btn_1, 240, 320);
    lv_obj_remove_flag(ui->screen_overview_btn_1, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_overview_btn_1_label = lv_label_create(ui->screen_overview_btn_1);
    lv_label_set_text(ui->screen_overview_btn_1_label, "");
    lv_label_set_long_mode(ui->screen_overview_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_overview_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_overview_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_overview_btn_1_label, LV_PCT(100));

    //Write style for screen_overview_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_overview_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_overview_btn_1, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_overview_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_overview_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_btn_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overview_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_overview_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_overview_btn_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_overview_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_overview_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overview_img_1
    ui->screen_overview_img_1 = lv_image_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_img_1, 0, 0);
    lv_obj_set_size(ui->screen_overview_img_1, 240, 320);
    lv_obj_add_flag(ui->screen_overview_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_overview_img_1, &_Control_Mode_Background_RGB565A8_240x320);
    lv_image_set_pivot(ui->screen_overview_img_1, 50,50);
    lv_image_set_rotation(ui->screen_overview_img_1, 0);

    //Write style for screen_overview_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_overview_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_overview_img_1, 181, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overview_btn_temperature
    ui->screen_overview_btn_temperature = lv_button_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_btn_temperature, 13, 20);
    lv_obj_set_size(ui->screen_overview_btn_temperature, 100, 80);
    lv_obj_remove_flag(ui->screen_overview_btn_temperature, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_overview_btn_temperature_label = lv_label_create(ui->screen_overview_btn_temperature);
    lv_label_set_text(ui->screen_overview_btn_temperature_label, "");
    lv_label_set_long_mode(ui->screen_overview_btn_temperature_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_overview_btn_temperature_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_overview_btn_temperature, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_overview_btn_temperature_label, LV_PCT(100));

    //Write style for screen_overview_btn_temperature, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_overview_btn_temperature, 200, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_overview_btn_temperature, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_overview_btn_temperature, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_overview_btn_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_btn_temperature, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overview_btn_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_overview_btn_temperature, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_overview_btn_temperature, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_overview_btn_temperature, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_overview_btn_temperature, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overview_btn_humidity_background
    ui->screen_overview_btn_humidity_background = lv_button_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_btn_humidity_background, 126, 20);
    lv_obj_set_size(ui->screen_overview_btn_humidity_background, 100, 80);
    lv_obj_remove_flag(ui->screen_overview_btn_humidity_background, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_overview_btn_humidity_background_label = lv_label_create(ui->screen_overview_btn_humidity_background);
    lv_label_set_text(ui->screen_overview_btn_humidity_background_label, "");
    lv_label_set_long_mode(ui->screen_overview_btn_humidity_background_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_overview_btn_humidity_background_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_overview_btn_humidity_background, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_overview_btn_humidity_background_label, LV_PCT(100));

    //Write style for screen_overview_btn_humidity_background, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_overview_btn_humidity_background, 200, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_overview_btn_humidity_background, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_overview_btn_humidity_background, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_overview_btn_humidity_background, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_btn_humidity_background, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overview_btn_humidity_background, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_overview_btn_humidity_background, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_overview_btn_humidity_background, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_overview_btn_humidity_background, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_overview_btn_humidity_background, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overview_btn_soil_station_background
    ui->screen_overview_btn_soil_station_background = lv_button_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_btn_soil_station_background, 13, 120);
    lv_obj_set_size(ui->screen_overview_btn_soil_station_background, 213, 80);
    lv_obj_remove_flag(ui->screen_overview_btn_soil_station_background, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_overview_btn_soil_station_background_label = lv_label_create(ui->screen_overview_btn_soil_station_background);
    lv_label_set_text(ui->screen_overview_btn_soil_station_background_label, "");
    lv_label_set_long_mode(ui->screen_overview_btn_soil_station_background_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_overview_btn_soil_station_background_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_overview_btn_soil_station_background, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_overview_btn_soil_station_background_label, LV_PCT(100));

    //Write style for screen_overview_btn_soil_station_background, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_overview_btn_soil_station_background, 200, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_overview_btn_soil_station_background, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_overview_btn_soil_station_background, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_overview_btn_soil_station_background, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_btn_soil_station_background, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overview_btn_soil_station_background, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_overview_btn_soil_station_background, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_overview_btn_soil_station_background, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_overview_btn_soil_station_background, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_overview_btn_soil_station_background, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overview_btn_light_intensity_background
    ui->screen_overview_btn_light_intensity_background = lv_button_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_btn_light_intensity_background, 13, 217);
    lv_obj_set_size(ui->screen_overview_btn_light_intensity_background, 100, 80);
    lv_obj_remove_flag(ui->screen_overview_btn_light_intensity_background, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_overview_btn_light_intensity_background_label = lv_label_create(ui->screen_overview_btn_light_intensity_background);
    lv_label_set_text(ui->screen_overview_btn_light_intensity_background_label, "");
    lv_label_set_long_mode(ui->screen_overview_btn_light_intensity_background_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_overview_btn_light_intensity_background_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_overview_btn_light_intensity_background, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_overview_btn_light_intensity_background_label, LV_PCT(100));

    //Write style for screen_overview_btn_light_intensity_background, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_overview_btn_light_intensity_background, 200, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_overview_btn_light_intensity_background, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_overview_btn_light_intensity_background, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_overview_btn_light_intensity_background, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_btn_light_intensity_background, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overview_btn_light_intensity_background, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_overview_btn_light_intensity_background, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_overview_btn_light_intensity_background, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_overview_btn_light_intensity_background, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_overview_btn_light_intensity_background, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overview_btn_ai_recommendations_background
    ui->screen_overview_btn_ai_recommendations_background = lv_button_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_btn_ai_recommendations_background, 126, 217);
    lv_obj_set_size(ui->screen_overview_btn_ai_recommendations_background, 100, 80);
    lv_obj_remove_flag(ui->screen_overview_btn_ai_recommendations_background, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_overview_btn_ai_recommendations_background_label = lv_label_create(ui->screen_overview_btn_ai_recommendations_background);
    lv_label_set_text(ui->screen_overview_btn_ai_recommendations_background_label, "");
    lv_label_set_long_mode(ui->screen_overview_btn_ai_recommendations_background_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_overview_btn_ai_recommendations_background_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_overview_btn_ai_recommendations_background, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_overview_btn_ai_recommendations_background_label, LV_PCT(100));

    //Write style for screen_overview_btn_ai_recommendations_background, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_overview_btn_ai_recommendations_background, 200, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_overview_btn_ai_recommendations_background, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_overview_btn_ai_recommendations_background, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_overview_btn_ai_recommendations_background, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_btn_ai_recommendations_background, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overview_btn_ai_recommendations_background, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_overview_btn_ai_recommendations_background, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_overview_btn_ai_recommendations_background, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_overview_btn_ai_recommendations_background, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_overview_btn_ai_recommendations_background, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overview_arc_temperature_value
    ui->screen_overview_arc_temperature_value = lv_arc_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_arc_temperature_value, 24, 26);
    lv_obj_set_size(ui->screen_overview_arc_temperature_value, 80, 80);
    lv_obj_remove_flag(ui->screen_overview_arc_temperature_value, LV_OBJ_FLAG_CLICKABLE);
    lv_arc_set_mode(ui->screen_overview_arc_temperature_value, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(ui->screen_overview_arc_temperature_value, 0, 100);
    lv_arc_set_bg_angles(ui->screen_overview_arc_temperature_value, 135, 45);
    lv_arc_set_value(ui->screen_overview_arc_temperature_value, 70);
    lv_arc_set_rotation(ui->screen_overview_arc_temperature_value, 0);

    //Write style for screen_overview_arc_temperature_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_overview_arc_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_overview_arc_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->screen_overview_arc_temperature_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->screen_overview_arc_temperature_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->screen_overview_arc_temperature_value, lv_color_hex(0xff0000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->screen_overview_arc_temperature_value, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_arc_temperature_value, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_overview_arc_temperature_value, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_overview_arc_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_overview_arc_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_overview_arc_temperature_value, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overview_arc_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_overview_arc_temperature_value, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->screen_overview_arc_temperature_value, 5, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->screen_overview_arc_temperature_value, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->screen_overview_arc_temperature_value, lv_color_hex(0x960000), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->screen_overview_arc_temperature_value, true, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_overview_arc_temperature_value, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_overview_arc_temperature_value, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_overview_arc_temperature_value, lv_color_hex(0x960000), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_overview_arc_temperature_value, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->screen_overview_arc_temperature_value, 2, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_overview_arc_humidity_value
    ui->screen_overview_arc_humidity_value = lv_arc_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_arc_humidity_value, 136, 26);
    lv_obj_set_size(ui->screen_overview_arc_humidity_value, 80, 80);
    lv_obj_remove_flag(ui->screen_overview_arc_humidity_value, LV_OBJ_FLAG_CLICKABLE);
    lv_arc_set_mode(ui->screen_overview_arc_humidity_value, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(ui->screen_overview_arc_humidity_value, 0, 100);
    lv_arc_set_bg_angles(ui->screen_overview_arc_humidity_value, 135, 45);
    lv_arc_set_value(ui->screen_overview_arc_humidity_value, 70);
    lv_arc_set_rotation(ui->screen_overview_arc_humidity_value, 0);

    //Write style for screen_overview_arc_humidity_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_overview_arc_humidity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_overview_arc_humidity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->screen_overview_arc_humidity_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->screen_overview_arc_humidity_value, 200, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->screen_overview_arc_humidity_value, lv_color_hex(0x0000ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->screen_overview_arc_humidity_value, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_arc_humidity_value, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_overview_arc_humidity_value, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_overview_arc_humidity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_overview_arc_humidity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_overview_arc_humidity_value, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overview_arc_humidity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_overview_arc_humidity_value, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->screen_overview_arc_humidity_value, 5, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->screen_overview_arc_humidity_value, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->screen_overview_arc_humidity_value, lv_color_hex(0x000096), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->screen_overview_arc_humidity_value, true, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_overview_arc_humidity_value, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_overview_arc_humidity_value, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_overview_arc_humidity_value, lv_color_hex(0x000096), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_overview_arc_humidity_value, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->screen_overview_arc_humidity_value, 2, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_overview_label_temperature
    ui->screen_overview_label_temperature = lv_label_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_label_temperature, 21, 73);
    lv_obj_set_size(ui->screen_overview_label_temperature, 80, 20);
    lv_label_set_text(ui->screen_overview_label_temperature, "temp");
    lv_label_set_long_mode(ui->screen_overview_label_temperature, LV_LABEL_LONG_WRAP);

    //Write style for screen_overview_label_temperature, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_overview_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_overview_label_temperature, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_overview_label_temperature, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_overview_label_temperature, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_overview_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_overview_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_overview_label_temperature, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_overview_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_overview_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_overview_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_overview_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_overview_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overview_label_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overview_label_temperature_value
    ui->screen_overview_label_temperature_value = lv_label_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_label_temperature_value, 21, 50);
    lv_obj_set_size(ui->screen_overview_label_temperature_value, 80, 20);
    lv_label_set_text(ui->screen_overview_label_temperature_value, "25°C");
    lv_label_set_long_mode(ui->screen_overview_label_temperature_value, LV_LABEL_LONG_WRAP);

    //Write style for screen_overview_label_temperature_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_overview_label_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_label_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_overview_label_temperature_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_overview_label_temperature_value, &lv_font_montserratMedium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_overview_label_temperature_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_overview_label_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_overview_label_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_overview_label_temperature_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_overview_label_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_overview_label_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_overview_label_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_overview_label_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_overview_label_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overview_label_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overview_label_humidity
    ui->screen_overview_label_humidity = lv_label_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_label_humidity, 130, 73);
    lv_obj_set_size(ui->screen_overview_label_humidity, 80, 20);
    lv_label_set_text(ui->screen_overview_label_humidity, "humi");
    lv_label_set_long_mode(ui->screen_overview_label_humidity, LV_LABEL_LONG_WRAP);

    //Write style for screen_overview_label_humidity, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_overview_label_humidity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_label_humidity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_overview_label_humidity, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_overview_label_humidity, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_overview_label_humidity, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_overview_label_humidity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_overview_label_humidity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_overview_label_humidity, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_overview_label_humidity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_overview_label_humidity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_overview_label_humidity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_overview_label_humidity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_overview_label_humidity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overview_label_humidity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overview_label_humidity_value
    ui->screen_overview_label_humidity_value = lv_label_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_label_humidity_value, 133, 50);
    lv_obj_set_size(ui->screen_overview_label_humidity_value, 80, 20);
    lv_label_set_text(ui->screen_overview_label_humidity_value, "60%");
    lv_label_set_long_mode(ui->screen_overview_label_humidity_value, LV_LABEL_LONG_WRAP);

    //Write style for screen_overview_label_humidity_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_overview_label_humidity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_label_humidity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_overview_label_humidity_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_overview_label_humidity_value, &lv_font_montserratMedium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_overview_label_humidity_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_overview_label_humidity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_overview_label_humidity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_overview_label_humidity_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_overview_label_humidity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_overview_label_humidity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_overview_label_humidity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_overview_label_humidity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_overview_label_humidity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overview_label_humidity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overview_bar_soil_station
    ui->screen_overview_bar_soil_station = lv_bar_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_bar_soil_station, 50, 175);
    lv_obj_set_size(ui->screen_overview_bar_soil_station, 150, 10);
    lv_obj_set_style_anim_duration(ui->screen_overview_bar_soil_station, 1000, 0);
    lv_bar_set_mode(ui->screen_overview_bar_soil_station, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->screen_overview_bar_soil_station, 0, 100);
    lv_bar_set_value(ui->screen_overview_bar_soil_station, 50, LV_ANIM_OFF);

    //Write style for screen_overview_bar_soil_station, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_overview_bar_soil_station, 200, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_overview_bar_soil_station, lv_color_hex(0x00ff00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_overview_bar_soil_station, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_bar_soil_station, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overview_bar_soil_station, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_overview_bar_soil_station, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_overview_bar_soil_station, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_overview_bar_soil_station, lv_color_hex(0x009600), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_overview_bar_soil_station, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_bar_soil_station, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes screen_overview_label_soil_station
    ui->screen_overview_label_soil_station = lv_label_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_label_soil_station, 70, 127);
    lv_obj_set_size(ui->screen_overview_label_soil_station, 100, 20);
    lv_label_set_text(ui->screen_overview_label_soil_station, "soil station");
    lv_label_set_long_mode(ui->screen_overview_label_soil_station, LV_LABEL_LONG_WRAP);

    //Write style for screen_overview_label_soil_station, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_overview_label_soil_station, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_label_soil_station, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_overview_label_soil_station, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_overview_label_soil_station, &lv_font_montserratMedium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_overview_label_soil_station, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_overview_label_soil_station, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_overview_label_soil_station, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_overview_label_soil_station, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_overview_label_soil_station, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_overview_label_soil_station, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_overview_label_soil_station, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_overview_label_soil_station, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_overview_label_soil_station, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overview_label_soil_station, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overview_label_soil_station_value
    ui->screen_overview_label_soil_station_value = lv_label_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_label_soil_station_value, 67, 152);
    lv_obj_set_size(ui->screen_overview_label_soil_station_value, 100, 20);
    lv_label_set_text(ui->screen_overview_label_soil_station_value, "suitable");
    lv_label_set_long_mode(ui->screen_overview_label_soil_station_value, LV_LABEL_LONG_WRAP);

    //Write style for screen_overview_label_soil_station_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_overview_label_soil_station_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_label_soil_station_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_overview_label_soil_station_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_overview_label_soil_station_value, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_overview_label_soil_station_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_overview_label_soil_station_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_overview_label_soil_station_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_overview_label_soil_station_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_overview_label_soil_station_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_overview_label_soil_station_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_overview_label_soil_station_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_overview_label_soil_station_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_overview_label_soil_station_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overview_label_soil_station_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overview_img_light_intensity
    ui->screen_overview_img_light_intensity = lv_image_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_img_light_intensity, 45, 243);
    lv_obj_set_size(ui->screen_overview_img_light_intensity, 30, 30);
    lv_obj_add_flag(ui->screen_overview_img_light_intensity, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_overview_img_light_intensity, &_Light_Intensity_RGB565A8_30x30);
    lv_image_set_pivot(ui->screen_overview_img_light_intensity, 50,50);
    lv_image_set_rotation(ui->screen_overview_img_light_intensity, 0);

    //Write style for screen_overview_img_light_intensity, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_overview_img_light_intensity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_overview_img_light_intensity, 217, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overview_label_light_intensity
    ui->screen_overview_label_light_intensity = lv_label_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_label_light_intensity, 24, 226);
    lv_obj_set_size(ui->screen_overview_label_light_intensity, 79, 15);
    lv_label_set_text(ui->screen_overview_label_light_intensity, "light intensity");
    lv_label_set_long_mode(ui->screen_overview_label_light_intensity, LV_LABEL_LONG_WRAP);

    //Write style for screen_overview_label_light_intensity, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_overview_label_light_intensity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_label_light_intensity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_overview_label_light_intensity, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_overview_label_light_intensity, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_overview_label_light_intensity, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_overview_label_light_intensity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_overview_label_light_intensity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_overview_label_light_intensity, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_overview_label_light_intensity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_overview_label_light_intensity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_overview_label_light_intensity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_overview_label_light_intensity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_overview_label_light_intensity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overview_label_light_intensity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overview_label_light_intensity_value
    ui->screen_overview_label_light_intensity_value = lv_label_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_label_light_intensity_value, 24, 279);
    lv_obj_set_size(ui->screen_overview_label_light_intensity_value, 80, 8);
    lv_label_set_text(ui->screen_overview_label_light_intensity_value, "250 lumen");
    lv_label_set_long_mode(ui->screen_overview_label_light_intensity_value, LV_LABEL_LONG_WRAP);

    //Write style for screen_overview_label_light_intensity_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_overview_label_light_intensity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_label_light_intensity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_overview_label_light_intensity_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_overview_label_light_intensity_value, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_overview_label_light_intensity_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_overview_label_light_intensity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_overview_label_light_intensity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_overview_label_light_intensity_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_overview_label_light_intensity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_overview_label_light_intensity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_overview_label_light_intensity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_overview_label_light_intensity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_overview_label_light_intensity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overview_label_light_intensity_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overview_label_ai_recommendations
    ui->screen_overview_label_ai_recommendations = lv_label_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_label_ai_recommendations, 136, 223);
    lv_obj_set_size(ui->screen_overview_label_ai_recommendations, 80, 20);
    lv_label_set_text(ui->screen_overview_label_ai_recommendations, "AI Recommendations");
    lv_label_set_long_mode(ui->screen_overview_label_ai_recommendations, LV_LABEL_LONG_WRAP);

    //Write style for screen_overview_label_ai_recommendations, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_overview_label_ai_recommendations, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_label_ai_recommendations, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_overview_label_ai_recommendations, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_overview_label_ai_recommendations, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_overview_label_ai_recommendations, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_overview_label_ai_recommendations, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_overview_label_ai_recommendations, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_overview_label_ai_recommendations, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_overview_label_ai_recommendations, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_overview_label_ai_recommendations, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_overview_label_ai_recommendations, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_overview_label_ai_recommendations, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_overview_label_ai_recommendations, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overview_label_ai_recommendations, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_overview_label_ai_contents
    ui->screen_overview_label_ai_contents = lv_label_create(ui->screen_overview);
    lv_obj_set_pos(ui->screen_overview_label_ai_contents, 136, 251);
    lv_obj_set_size(ui->screen_overview_label_ai_contents, 80, 20);
    lv_label_set_text(ui->screen_overview_label_ai_contents, "Don't want to go to work don't want to go to work don't want to go to work don't want to go to work don't want to go to work");
    lv_label_set_long_mode(ui->screen_overview_label_ai_contents, LV_LABEL_LONG_WRAP);

    //Write style for screen_overview_label_ai_contents, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_overview_label_ai_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_overview_label_ai_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_overview_label_ai_contents, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_overview_label_ai_contents, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_overview_label_ai_contents, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_overview_label_ai_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_overview_label_ai_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_overview_label_ai_contents, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_overview_label_ai_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_overview_label_ai_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_overview_label_ai_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_overview_label_ai_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_overview_label_ai_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_overview_label_ai_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_overview.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_overview);

    //Init events for screen.
    events_init_screen_overview(ui);
}
