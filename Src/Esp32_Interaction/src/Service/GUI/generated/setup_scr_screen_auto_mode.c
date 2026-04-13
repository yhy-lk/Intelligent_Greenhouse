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



void setup_scr_screen_auto_mode(lv_ui *ui)
{
    //Write codes screen_auto_mode
    ui->screen_auto_mode = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_auto_mode, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_auto_mode, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_auto_mode, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_img_6
    ui->screen_auto_mode_img_6 = lv_image_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_img_6, 0, 0);
    lv_obj_set_size(ui->screen_auto_mode_img_6, 240, 320);
    lv_obj_add_flag(ui->screen_auto_mode_img_6, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_auto_mode_img_6, &_Dashboard_Background_2_RGB565A8_240x320);
    lv_image_set_pivot(ui->screen_auto_mode_img_6, 50,50);
    lv_image_set_rotation(ui->screen_auto_mode_img_6, 0);

    //Write style for screen_auto_mode_img_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_auto_mode_img_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_auto_mode_img_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_btn_background
    ui->screen_auto_mode_btn_background = lv_button_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_btn_background, 0, 0);
    lv_obj_set_size(ui->screen_auto_mode_btn_background, 240, 320);
    lv_obj_remove_flag(ui->screen_auto_mode_btn_background, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_auto_mode_btn_background_label = lv_label_create(ui->screen_auto_mode_btn_background);
    lv_label_set_text(ui->screen_auto_mode_btn_background_label, "");
    lv_label_set_long_mode(ui->screen_auto_mode_btn_background_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_auto_mode_btn_background_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_auto_mode_btn_background, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_auto_mode_btn_background_label, LV_PCT(100));

    //Write style for screen_auto_mode_btn_background, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_btn_background, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_auto_mode_btn_background, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_auto_mode_btn_background, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_auto_mode_btn_background, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_btn_background, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_btn_background, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_btn_background, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_btn_background, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_btn_background, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_btn_background, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_img_background
    ui->screen_auto_mode_img_background = lv_image_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_img_background, 0, 0);
    lv_obj_set_size(ui->screen_auto_mode_img_background, 240, 320);
    lv_obj_add_flag(ui->screen_auto_mode_img_background, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_auto_mode_img_background, &_Auto_Mode_Background_RGB565A8_240x320);
    lv_image_set_pivot(ui->screen_auto_mode_img_background, 50,50);
    lv_image_set_rotation(ui->screen_auto_mode_img_background, 0);

    //Write style for screen_auto_mode_img_background, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_auto_mode_img_background, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_auto_mode_img_background, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_btn_temperature
    ui->screen_auto_mode_btn_temperature = lv_button_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_btn_temperature, 10, 40);
    lv_obj_set_size(ui->screen_auto_mode_btn_temperature, 220, 40);
    lv_obj_remove_flag(ui->screen_auto_mode_btn_temperature, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_auto_mode_btn_temperature_label = lv_label_create(ui->screen_auto_mode_btn_temperature);
    lv_label_set_text(ui->screen_auto_mode_btn_temperature_label, "");
    lv_label_set_long_mode(ui->screen_auto_mode_btn_temperature_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_auto_mode_btn_temperature_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_auto_mode_btn_temperature, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_auto_mode_btn_temperature_label, LV_PCT(100));

    //Write style for screen_auto_mode_btn_temperature, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_btn_temperature, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_auto_mode_btn_temperature, lv_color_hex(0xe7e7e7), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_auto_mode_btn_temperature, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_auto_mode_btn_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_btn_temperature, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_btn_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_btn_temperature, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_btn_temperature, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_btn_temperature, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_btn_temperature, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_label_auto_mode
    ui->screen_auto_mode_label_auto_mode = lv_label_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_label_auto_mode, 10, 15);
    lv_obj_set_size(ui->screen_auto_mode_label_auto_mode, 125, 15);
    lv_label_set_text(ui->screen_auto_mode_label_auto_mode, "Auto Mode");
    lv_label_set_long_mode(ui->screen_auto_mode_label_auto_mode, LV_LABEL_LONG_WRAP);

    //Write style for screen_auto_mode_label_auto_mode, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_auto_mode_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_label_auto_mode, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_label_auto_mode, &lv_font_montserratMedium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_label_auto_mode, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_auto_mode_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_auto_mode_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_label_auto_mode, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_auto_mode_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_auto_mode_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_auto_mode_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_auto_mode_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_btn_humidity
    ui->screen_auto_mode_btn_humidity = lv_button_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_btn_humidity, 10, 88);
    lv_obj_set_size(ui->screen_auto_mode_btn_humidity, 220, 40);
    lv_obj_remove_flag(ui->screen_auto_mode_btn_humidity, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_auto_mode_btn_humidity_label = lv_label_create(ui->screen_auto_mode_btn_humidity);
    lv_label_set_text(ui->screen_auto_mode_btn_humidity_label, "");
    lv_label_set_long_mode(ui->screen_auto_mode_btn_humidity_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_auto_mode_btn_humidity_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_auto_mode_btn_humidity, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_auto_mode_btn_humidity_label, LV_PCT(100));

    //Write style for screen_auto_mode_btn_humidity, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_btn_humidity, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_auto_mode_btn_humidity, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_auto_mode_btn_humidity, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_auto_mode_btn_humidity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_btn_humidity, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_btn_humidity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_btn_humidity, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_btn_humidity, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_btn_humidity, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_btn_humidity, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_btn_soil_moisture
    ui->screen_auto_mode_btn_soil_moisture = lv_button_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_btn_soil_moisture, 10, 137);
    lv_obj_set_size(ui->screen_auto_mode_btn_soil_moisture, 220, 40);
    lv_obj_remove_flag(ui->screen_auto_mode_btn_soil_moisture, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_auto_mode_btn_soil_moisture_label = lv_label_create(ui->screen_auto_mode_btn_soil_moisture);
    lv_label_set_text(ui->screen_auto_mode_btn_soil_moisture_label, "");
    lv_label_set_long_mode(ui->screen_auto_mode_btn_soil_moisture_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_auto_mode_btn_soil_moisture_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_auto_mode_btn_soil_moisture, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_auto_mode_btn_soil_moisture_label, LV_PCT(100));

    //Write style for screen_auto_mode_btn_soil_moisture, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_btn_soil_moisture, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_auto_mode_btn_soil_moisture, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_auto_mode_btn_soil_moisture, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_auto_mode_btn_soil_moisture, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_btn_soil_moisture, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_btn_soil_moisture, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_btn_soil_moisture, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_btn_soil_moisture, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_btn_soil_moisture, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_btn_soil_moisture, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_btn_light_intensity
    ui->screen_auto_mode_btn_light_intensity = lv_button_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_btn_light_intensity, 10, 186);
    lv_obj_set_size(ui->screen_auto_mode_btn_light_intensity, 220, 40);
    lv_obj_remove_flag(ui->screen_auto_mode_btn_light_intensity, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_auto_mode_btn_light_intensity_label = lv_label_create(ui->screen_auto_mode_btn_light_intensity);
    lv_label_set_text(ui->screen_auto_mode_btn_light_intensity_label, "");
    lv_label_set_long_mode(ui->screen_auto_mode_btn_light_intensity_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_auto_mode_btn_light_intensity_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_auto_mode_btn_light_intensity, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_auto_mode_btn_light_intensity_label, LV_PCT(100));

    //Write style for screen_auto_mode_btn_light_intensity, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_btn_light_intensity, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_auto_mode_btn_light_intensity, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_auto_mode_btn_light_intensity, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_auto_mode_btn_light_intensity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_btn_light_intensity, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_btn_light_intensity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_btn_light_intensity, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_btn_light_intensity, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_btn_light_intensity, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_btn_light_intensity, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_btn_light_color
    ui->screen_auto_mode_btn_light_color = lv_button_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_btn_light_color, 10, 234);
    lv_obj_set_size(ui->screen_auto_mode_btn_light_color, 220, 80);
    lv_obj_remove_flag(ui->screen_auto_mode_btn_light_color, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_auto_mode_btn_light_color_label = lv_label_create(ui->screen_auto_mode_btn_light_color);
    lv_label_set_text(ui->screen_auto_mode_btn_light_color_label, "");
    lv_label_set_long_mode(ui->screen_auto_mode_btn_light_color_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_auto_mode_btn_light_color_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_auto_mode_btn_light_color, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_auto_mode_btn_light_color_label, LV_PCT(100));

    //Write style for screen_auto_mode_btn_light_color, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_btn_light_color, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_auto_mode_btn_light_color, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_auto_mode_btn_light_color, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_auto_mode_btn_light_color, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_btn_light_color, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_btn_light_color, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_btn_light_color, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_btn_light_color, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_btn_light_color, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_btn_light_color, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_img_temperature
    ui->screen_auto_mode_img_temperature = lv_image_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_img_temperature, 20, 44);
    lv_obj_set_size(ui->screen_auto_mode_img_temperature, 15, 15);
    lv_obj_add_flag(ui->screen_auto_mode_img_temperature, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_auto_mode_img_temperature, &_Temperature_Auto_Mode_RGB565A8_15x15);
    lv_image_set_pivot(ui->screen_auto_mode_img_temperature, 50,50);
    lv_image_set_rotation(ui->screen_auto_mode_img_temperature, 0);

    //Write style for screen_auto_mode_img_temperature, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_auto_mode_img_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_auto_mode_img_temperature, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_img_humidity
    ui->screen_auto_mode_img_humidity = lv_image_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_img_humidity, 20, 93);
    lv_obj_set_size(ui->screen_auto_mode_img_humidity, 15, 15);
    lv_obj_add_flag(ui->screen_auto_mode_img_humidity, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_auto_mode_img_humidity, &_Humidity_Auto_Mode_RGB565A8_15x15);
    lv_image_set_pivot(ui->screen_auto_mode_img_humidity, 50,50);
    lv_image_set_rotation(ui->screen_auto_mode_img_humidity, 0);

    //Write style for screen_auto_mode_img_humidity, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_auto_mode_img_humidity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_auto_mode_img_humidity, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_label_temperature_range
    ui->screen_auto_mode_label_temperature_range = lv_label_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_label_temperature_range, 42, 47);
    lv_obj_set_size(ui->screen_auto_mode_label_temperature_range, 150, 10);
    lv_label_set_text(ui->screen_auto_mode_label_temperature_range, "Temperature range");
    lv_label_set_long_mode(ui->screen_auto_mode_label_temperature_range, LV_LABEL_LONG_WRAP);

    //Write style for screen_auto_mode_label_temperature_range, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_auto_mode_label_temperature_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_label_temperature_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_label_temperature_range, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_label_temperature_range, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_label_temperature_range, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_auto_mode_label_temperature_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_auto_mode_label_temperature_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_label_temperature_range, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_label_temperature_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_auto_mode_label_temperature_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_auto_mode_label_temperature_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_auto_mode_label_temperature_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_auto_mode_label_temperature_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_label_temperature_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_slider_temperature_range
    ui->screen_auto_mode_slider_temperature_range = lv_slider_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_slider_temperature_range, 45, 67);
    lv_obj_set_size(ui->screen_auto_mode_slider_temperature_range, 150, 3);
    lv_slider_set_range(ui->screen_auto_mode_slider_temperature_range, 0, 50);
    lv_slider_set_mode(ui->screen_auto_mode_slider_temperature_range, LV_SLIDER_MODE_RANGE);
    lv_slider_set_value(ui->screen_auto_mode_slider_temperature_range, 30, LV_ANIM_OFF);
    lv_slider_set_left_value(ui->screen_auto_mode_slider_temperature_range, 20, LV_ANIM_OFF);

    //Write style for screen_auto_mode_slider_temperature_range, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_slider_temperature_range, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_auto_mode_slider_temperature_range, lv_color_hex(0xdc143c), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_auto_mode_slider_temperature_range, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_slider_temperature_range, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_auto_mode_slider_temperature_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_slider_temperature_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_auto_mode_slider_temperature_range, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_slider_temperature_range, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_auto_mode_slider_temperature_range, lv_color_hex(0xdc143c), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_auto_mode_slider_temperature_range, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_slider_temperature_range, 8, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_auto_mode_slider_temperature_range, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_slider_temperature_range, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_auto_mode_slider_temperature_range, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_auto_mode_slider_temperature_range, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_slider_temperature_range, 8, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_label_temperature_left_value
    ui->screen_auto_mode_label_temperature_left_value = lv_label_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_label_temperature_left_value, 20, 64);
    lv_obj_set_size(ui->screen_auto_mode_label_temperature_left_value, 25, 10);
    lv_label_set_text(ui->screen_auto_mode_label_temperature_left_value, "20°C");
    lv_label_set_long_mode(ui->screen_auto_mode_label_temperature_left_value, LV_LABEL_LONG_WRAP);

    //Write style for screen_auto_mode_label_temperature_left_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_auto_mode_label_temperature_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_label_temperature_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_label_temperature_left_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_label_temperature_left_value, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_label_temperature_left_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_auto_mode_label_temperature_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_auto_mode_label_temperature_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_label_temperature_left_value, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_label_temperature_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_auto_mode_label_temperature_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_auto_mode_label_temperature_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_auto_mode_label_temperature_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_auto_mode_label_temperature_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_label_temperature_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_label_humidity_range
    ui->screen_auto_mode_label_humidity_range = lv_label_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_label_humidity_range, 42, 97);
    lv_obj_set_size(ui->screen_auto_mode_label_humidity_range, 150, 10);
    lv_label_set_text(ui->screen_auto_mode_label_humidity_range, "Humidity range");
    lv_label_set_long_mode(ui->screen_auto_mode_label_humidity_range, LV_LABEL_LONG_WRAP);

    //Write style for screen_auto_mode_label_humidity_range, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_auto_mode_label_humidity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_label_humidity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_label_humidity_range, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_label_humidity_range, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_label_humidity_range, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_auto_mode_label_humidity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_auto_mode_label_humidity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_label_humidity_range, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_label_humidity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_auto_mode_label_humidity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_auto_mode_label_humidity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_auto_mode_label_humidity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_auto_mode_label_humidity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_label_humidity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_slider_humidity_range
    ui->screen_auto_mode_slider_humidity_range = lv_slider_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_slider_humidity_range, 45, 115);
    lv_obj_set_size(ui->screen_auto_mode_slider_humidity_range, 150, 3);
    lv_slider_set_range(ui->screen_auto_mode_slider_humidity_range, 0, 100);
    lv_slider_set_mode(ui->screen_auto_mode_slider_humidity_range, LV_SLIDER_MODE_RANGE);
    lv_slider_set_value(ui->screen_auto_mode_slider_humidity_range, 70, LV_ANIM_OFF);
    lv_slider_set_left_value(ui->screen_auto_mode_slider_humidity_range, 40, LV_ANIM_OFF);

    //Write style for screen_auto_mode_slider_humidity_range, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_slider_humidity_range, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_auto_mode_slider_humidity_range, lv_color_hex(0xadd8e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_auto_mode_slider_humidity_range, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_slider_humidity_range, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_auto_mode_slider_humidity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_slider_humidity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_auto_mode_slider_humidity_range, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_slider_humidity_range, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_auto_mode_slider_humidity_range, lv_color_hex(0xadd8e6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_auto_mode_slider_humidity_range, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_slider_humidity_range, 8, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_auto_mode_slider_humidity_range, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_slider_humidity_range, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_auto_mode_slider_humidity_range, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_auto_mode_slider_humidity_range, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_slider_humidity_range, 8, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_label_humidity_left_value
    ui->screen_auto_mode_label_humidity_left_value = lv_label_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_label_humidity_left_value, 20, 115);
    lv_obj_set_size(ui->screen_auto_mode_label_humidity_left_value, 20, 10);
    lv_label_set_text(ui->screen_auto_mode_label_humidity_left_value, "40%");
    lv_label_set_long_mode(ui->screen_auto_mode_label_humidity_left_value, LV_LABEL_LONG_WRAP);

    //Write style for screen_auto_mode_label_humidity_left_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_auto_mode_label_humidity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_label_humidity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_label_humidity_left_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_label_humidity_left_value, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_label_humidity_left_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_auto_mode_label_humidity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_auto_mode_label_humidity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_label_humidity_left_value, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_label_humidity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_auto_mode_label_humidity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_auto_mode_label_humidity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_auto_mode_label_humidity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_auto_mode_label_humidity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_label_humidity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_label_temperature_right_value
    ui->screen_auto_mode_label_temperature_right_value = lv_label_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_label_temperature_right_value, 200, 64);
    lv_obj_set_size(ui->screen_auto_mode_label_temperature_right_value, 20, 10);
    lv_label_set_text(ui->screen_auto_mode_label_temperature_right_value, "30°C");
    lv_label_set_long_mode(ui->screen_auto_mode_label_temperature_right_value, LV_LABEL_LONG_WRAP);

    //Write style for screen_auto_mode_label_temperature_right_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_auto_mode_label_temperature_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_label_temperature_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_label_temperature_right_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_label_temperature_right_value, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_label_temperature_right_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_auto_mode_label_temperature_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_auto_mode_label_temperature_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_label_temperature_right_value, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_label_temperature_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_auto_mode_label_temperature_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_auto_mode_label_temperature_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_auto_mode_label_temperature_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_auto_mode_label_temperature_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_label_temperature_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_label_humidity_right_value
    ui->screen_auto_mode_label_humidity_right_value = lv_label_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_label_humidity_right_value, 200, 114);
    lv_obj_set_size(ui->screen_auto_mode_label_humidity_right_value, 20, 10);
    lv_label_set_text(ui->screen_auto_mode_label_humidity_right_value, "70%");
    lv_label_set_long_mode(ui->screen_auto_mode_label_humidity_right_value, LV_LABEL_LONG_WRAP);

    //Write style for screen_auto_mode_label_humidity_right_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_auto_mode_label_humidity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_label_humidity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_label_humidity_right_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_label_humidity_right_value, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_label_humidity_right_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_auto_mode_label_humidity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_auto_mode_label_humidity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_label_humidity_right_value, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_label_humidity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_auto_mode_label_humidity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_auto_mode_label_humidity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_auto_mode_label_humidity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_auto_mode_label_humidity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_label_humidity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_label_soil_moisture_right_value
    ui->screen_auto_mode_label_soil_moisture_right_value = lv_label_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_label_soil_moisture_right_value, 200, 162);
    lv_obj_set_size(ui->screen_auto_mode_label_soil_moisture_right_value, 20, 10);
    lv_label_set_text(ui->screen_auto_mode_label_soil_moisture_right_value, "60%");
    lv_label_set_long_mode(ui->screen_auto_mode_label_soil_moisture_right_value, LV_LABEL_LONG_WRAP);

    //Write style for screen_auto_mode_label_soil_moisture_right_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_auto_mode_label_soil_moisture_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_label_soil_moisture_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_label_soil_moisture_right_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_label_soil_moisture_right_value, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_label_soil_moisture_right_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_auto_mode_label_soil_moisture_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_auto_mode_label_soil_moisture_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_label_soil_moisture_right_value, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_label_soil_moisture_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_auto_mode_label_soil_moisture_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_auto_mode_label_soil_moisture_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_auto_mode_label_soil_moisture_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_auto_mode_label_soil_moisture_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_label_soil_moisture_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_label_soil_moisture_range
    ui->screen_auto_mode_label_soil_moisture_range = lv_label_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_label_soil_moisture_range, 42, 145);
    lv_obj_set_size(ui->screen_auto_mode_label_soil_moisture_range, 150, 10);
    lv_label_set_text(ui->screen_auto_mode_label_soil_moisture_range, "Soil moisture range");
    lv_label_set_long_mode(ui->screen_auto_mode_label_soil_moisture_range, LV_LABEL_LONG_WRAP);

    //Write style for screen_auto_mode_label_soil_moisture_range, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_auto_mode_label_soil_moisture_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_label_soil_moisture_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_label_soil_moisture_range, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_label_soil_moisture_range, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_label_soil_moisture_range, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_auto_mode_label_soil_moisture_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_auto_mode_label_soil_moisture_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_label_soil_moisture_range, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_label_soil_moisture_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_auto_mode_label_soil_moisture_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_auto_mode_label_soil_moisture_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_auto_mode_label_soil_moisture_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_auto_mode_label_soil_moisture_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_label_soil_moisture_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_slider_soil_moisture_range
    ui->screen_auto_mode_slider_soil_moisture_range = lv_slider_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_slider_soil_moisture_range, 45, 165);
    lv_obj_set_size(ui->screen_auto_mode_slider_soil_moisture_range, 150, 3);
    lv_slider_set_range(ui->screen_auto_mode_slider_soil_moisture_range, 0, 100);
    lv_slider_set_mode(ui->screen_auto_mode_slider_soil_moisture_range, LV_SLIDER_MODE_RANGE);
    lv_slider_set_value(ui->screen_auto_mode_slider_soil_moisture_range, 60, LV_ANIM_OFF);
    lv_slider_set_left_value(ui->screen_auto_mode_slider_soil_moisture_range, 40, LV_ANIM_OFF);

    //Write style for screen_auto_mode_slider_soil_moisture_range, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_slider_soil_moisture_range, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_auto_mode_slider_soil_moisture_range, lv_color_hex(0x804000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_auto_mode_slider_soil_moisture_range, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_slider_soil_moisture_range, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_auto_mode_slider_soil_moisture_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_slider_soil_moisture_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_auto_mode_slider_soil_moisture_range, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_slider_soil_moisture_range, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_auto_mode_slider_soil_moisture_range, lv_color_hex(0x804000), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_auto_mode_slider_soil_moisture_range, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_slider_soil_moisture_range, 8, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_auto_mode_slider_soil_moisture_range, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_slider_soil_moisture_range, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_auto_mode_slider_soil_moisture_range, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_auto_mode_slider_soil_moisture_range, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_slider_soil_moisture_range, 8, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_label_soil_moisture_left_value
    ui->screen_auto_mode_label_soil_moisture_left_value = lv_label_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_label_soil_moisture_left_value, 20, 162);
    lv_obj_set_size(ui->screen_auto_mode_label_soil_moisture_left_value, 20, 10);
    lv_label_set_text(ui->screen_auto_mode_label_soil_moisture_left_value, "40%");
    lv_label_set_long_mode(ui->screen_auto_mode_label_soil_moisture_left_value, LV_LABEL_LONG_WRAP);

    //Write style for screen_auto_mode_label_soil_moisture_left_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_auto_mode_label_soil_moisture_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_label_soil_moisture_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_label_soil_moisture_left_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_label_soil_moisture_left_value, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_label_soil_moisture_left_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_auto_mode_label_soil_moisture_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_auto_mode_label_soil_moisture_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_label_soil_moisture_left_value, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_label_soil_moisture_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_auto_mode_label_soil_moisture_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_auto_mode_label_soil_moisture_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_auto_mode_label_soil_moisture_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_auto_mode_label_soil_moisture_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_label_soil_moisture_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_img_soil_moisture
    ui->screen_auto_mode_img_soil_moisture = lv_image_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_img_soil_moisture, 20, 142);
    lv_obj_set_size(ui->screen_auto_mode_img_soil_moisture, 15, 15);
    lv_obj_add_flag(ui->screen_auto_mode_img_soil_moisture, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_auto_mode_img_soil_moisture, &_Soil_Moisture_Auto_Mode_RGB565A8_15x15);
    lv_image_set_pivot(ui->screen_auto_mode_img_soil_moisture, 50,50);
    lv_image_set_rotation(ui->screen_auto_mode_img_soil_moisture, 0);

    //Write style for screen_auto_mode_img_soil_moisture, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_auto_mode_img_soil_moisture, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_auto_mode_img_soil_moisture, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_img_light_intensity
    ui->screen_auto_mode_img_light_intensity = lv_image_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_img_light_intensity, 20, 190);
    lv_obj_set_size(ui->screen_auto_mode_img_light_intensity, 15, 15);
    lv_obj_add_flag(ui->screen_auto_mode_img_light_intensity, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_auto_mode_img_light_intensity, &_Ligth_Intensity_Auto_Mode_RGB565A8_15x15);
    lv_image_set_pivot(ui->screen_auto_mode_img_light_intensity, 50,50);
    lv_image_set_rotation(ui->screen_auto_mode_img_light_intensity, 0);

    //Write style for screen_auto_mode_img_light_intensity, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_auto_mode_img_light_intensity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_auto_mode_img_light_intensity, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_label_light_intensity_range
    ui->screen_auto_mode_label_light_intensity_range = lv_label_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_label_light_intensity_range, 42, 193);
    lv_obj_set_size(ui->screen_auto_mode_label_light_intensity_range, 150, 10);
    lv_label_set_text(ui->screen_auto_mode_label_light_intensity_range, "Light intensity range");
    lv_label_set_long_mode(ui->screen_auto_mode_label_light_intensity_range, LV_LABEL_LONG_WRAP);

    //Write style for screen_auto_mode_label_light_intensity_range, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_auto_mode_label_light_intensity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_label_light_intensity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_label_light_intensity_range, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_label_light_intensity_range, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_label_light_intensity_range, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_auto_mode_label_light_intensity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_auto_mode_label_light_intensity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_label_light_intensity_range, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_label_light_intensity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_auto_mode_label_light_intensity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_auto_mode_label_light_intensity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_auto_mode_label_light_intensity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_auto_mode_label_light_intensity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_label_light_intensity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_slider_light_intensity_range
    ui->screen_auto_mode_slider_light_intensity_range = lv_slider_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_slider_light_intensity_range, 52, 214);
    lv_obj_set_size(ui->screen_auto_mode_slider_light_intensity_range, 140, 3);
    lv_slider_set_range(ui->screen_auto_mode_slider_light_intensity_range, 0, 1000);
    lv_slider_set_mode(ui->screen_auto_mode_slider_light_intensity_range, LV_SLIDER_MODE_RANGE);
    lv_slider_set_value(ui->screen_auto_mode_slider_light_intensity_range, 600, LV_ANIM_OFF);
    lv_slider_set_left_value(ui->screen_auto_mode_slider_light_intensity_range, 200, LV_ANIM_OFF);

    //Write style for screen_auto_mode_slider_light_intensity_range, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_slider_light_intensity_range, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_auto_mode_slider_light_intensity_range, lv_color_hex(0xffff00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_auto_mode_slider_light_intensity_range, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_slider_light_intensity_range, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_auto_mode_slider_light_intensity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_slider_light_intensity_range, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_auto_mode_slider_light_intensity_range, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_slider_light_intensity_range, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_auto_mode_slider_light_intensity_range, lv_color_hex(0xffff00), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_auto_mode_slider_light_intensity_range, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_slider_light_intensity_range, 8, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_auto_mode_slider_light_intensity_range, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_slider_light_intensity_range, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_auto_mode_slider_light_intensity_range, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_auto_mode_slider_light_intensity_range, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_slider_light_intensity_range, 8, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_label_light_intensity_left_value
    ui->screen_auto_mode_label_light_intensity_left_value = lv_label_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_label_light_intensity_left_value, 20, 211);
    lv_obj_set_size(ui->screen_auto_mode_label_light_intensity_left_value, 25, 10);
    lv_label_set_text(ui->screen_auto_mode_label_light_intensity_left_value, "200 lx");
    lv_label_set_long_mode(ui->screen_auto_mode_label_light_intensity_left_value, LV_LABEL_LONG_WRAP);

    //Write style for screen_auto_mode_label_light_intensity_left_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_auto_mode_label_light_intensity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_label_light_intensity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_label_light_intensity_left_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_label_light_intensity_left_value, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_label_light_intensity_left_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_auto_mode_label_light_intensity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_auto_mode_label_light_intensity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_label_light_intensity_left_value, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_label_light_intensity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_auto_mode_label_light_intensity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_auto_mode_label_light_intensity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_auto_mode_label_light_intensity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_auto_mode_label_light_intensity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_label_light_intensity_left_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_label_light_intensity_right_value
    ui->screen_auto_mode_label_light_intensity_right_value = lv_label_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_label_light_intensity_right_value, 195, 211);
    lv_obj_set_size(ui->screen_auto_mode_label_light_intensity_right_value, 30, 10);
    lv_label_set_text(ui->screen_auto_mode_label_light_intensity_right_value, "600 lx");
    lv_label_set_long_mode(ui->screen_auto_mode_label_light_intensity_right_value, LV_LABEL_LONG_WRAP);

    //Write style for screen_auto_mode_label_light_intensity_right_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_auto_mode_label_light_intensity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_label_light_intensity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_label_light_intensity_right_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_label_light_intensity_right_value, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_label_light_intensity_right_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_auto_mode_label_light_intensity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_auto_mode_label_light_intensity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_label_light_intensity_right_value, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_label_light_intensity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_auto_mode_label_light_intensity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_auto_mode_label_light_intensity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_auto_mode_label_light_intensity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_auto_mode_label_light_intensity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_label_light_intensity_right_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_img_light_color
    ui->screen_auto_mode_img_light_color = lv_image_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_img_light_color, 20, 239);
    lv_obj_set_size(ui->screen_auto_mode_img_light_color, 15, 15);
    lv_obj_add_flag(ui->screen_auto_mode_img_light_color, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_auto_mode_img_light_color, &_Light_Color_Auto_Mode_RGB565A8_15x15);
    lv_image_set_pivot(ui->screen_auto_mode_img_light_color, 50,50);
    lv_image_set_rotation(ui->screen_auto_mode_img_light_color, 0);

    //Write style for screen_auto_mode_img_light_color, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_auto_mode_img_light_color, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_auto_mode_img_light_color, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_auto_mode_label_light_color_value
    ui->screen_auto_mode_label_light_color_value = lv_label_create(ui->screen_auto_mode);
    lv_obj_set_pos(ui->screen_auto_mode_label_light_color_value, 42, 242);
    lv_obj_set_size(ui->screen_auto_mode_label_light_color_value, 150, 10);
    lv_label_set_text(ui->screen_auto_mode_label_light_color_value, "Light Color value");
    lv_label_set_long_mode(ui->screen_auto_mode_label_light_color_value, LV_LABEL_LONG_WRAP);

    //Write style for screen_auto_mode_label_light_color_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_auto_mode_label_light_color_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_auto_mode_label_light_color_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_auto_mode_label_light_color_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_auto_mode_label_light_color_value, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_auto_mode_label_light_color_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_auto_mode_label_light_color_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_auto_mode_label_light_color_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_auto_mode_label_light_color_value, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_auto_mode_label_light_color_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_auto_mode_label_light_color_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_auto_mode_label_light_color_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_auto_mode_label_light_color_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_auto_mode_label_light_color_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_auto_mode_label_light_color_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_auto_mode.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_auto_mode);

    //Init events for screen.
    events_init_screen_auto_mode(ui);
}
