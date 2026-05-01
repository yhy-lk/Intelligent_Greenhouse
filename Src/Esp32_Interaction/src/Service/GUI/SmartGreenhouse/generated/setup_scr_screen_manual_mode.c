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



void setup_scr_screen_manual_mode(lv_ui *ui)
{
    //Write codes screen_manual_mode
    ui->screen_manual_mode = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_manual_mode, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_manual_mode, LV_SCROLLBAR_MODE_OFF);
    lv_obj_remove_flag(ui->screen_manual_mode, LV_OBJ_FLAG_SCROLLABLE);

    //Write style for screen_manual_mode, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_img_background
    ui->screen_manual_mode_img_background = lv_image_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_img_background, 0, 0);
    lv_obj_set_size(ui->screen_manual_mode_img_background, 240, 320);
    lv_obj_add_flag(ui->screen_manual_mode_img_background, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_manual_mode_img_background, &_Dashboard_Background_2_RGB565A8_240x320);
    lv_image_set_pivot(ui->screen_manual_mode_img_background, 50,50);
    lv_image_set_rotation(ui->screen_manual_mode_img_background, 0);

    //Write style for screen_manual_mode_img_background, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_manual_mode_img_background, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_manual_mode_img_background, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_btn_manual_control
    ui->screen_manual_mode_btn_manual_control = lv_button_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_btn_manual_control, 0, 0);
    lv_obj_set_size(ui->screen_manual_mode_btn_manual_control, 240, 320);
    lv_obj_remove_flag(ui->screen_manual_mode_btn_manual_control, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_manual_mode_btn_manual_control_label = lv_label_create(ui->screen_manual_mode_btn_manual_control);
    lv_label_set_text(ui->screen_manual_mode_btn_manual_control_label, "");
    lv_label_set_long_mode(ui->screen_manual_mode_btn_manual_control_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_manual_mode_btn_manual_control_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_manual_mode_btn_manual_control, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_manual_mode_btn_manual_control_label, LV_PCT(100));

    //Write style for screen_manual_mode_btn_manual_control, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_btn_manual_control, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_btn_manual_control, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_btn_manual_control, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_manual_mode_btn_manual_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_btn_manual_control, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_btn_manual_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_manual_mode_btn_manual_control, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_manual_mode_btn_manual_control, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_manual_mode_btn_manual_control, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_manual_mode_btn_manual_control, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_img_1
    ui->screen_manual_mode_img_1 = lv_image_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_img_1, 0, 0);
    lv_obj_set_size(ui->screen_manual_mode_img_1, 240, 320);
    lv_obj_add_flag(ui->screen_manual_mode_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_manual_mode_img_1, &_Auto_Mode_Background_RGB565A8_240x320);
    lv_image_set_pivot(ui->screen_manual_mode_img_1, 50,50);
    lv_image_set_rotation(ui->screen_manual_mode_img_1, 0);

    //Write style for screen_manual_mode_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_manual_mode_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_manual_mode_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_btn_grow_light_control
    ui->screen_manual_mode_btn_grow_light_control = lv_button_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_btn_grow_light_control, 10, 210);
    lv_obj_set_size(ui->screen_manual_mode_btn_grow_light_control, 220, 100);
    lv_obj_remove_flag(ui->screen_manual_mode_btn_grow_light_control, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_manual_mode_btn_grow_light_control_label = lv_label_create(ui->screen_manual_mode_btn_grow_light_control);
    lv_label_set_text(ui->screen_manual_mode_btn_grow_light_control_label, "");
    lv_label_set_long_mode(ui->screen_manual_mode_btn_grow_light_control_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_manual_mode_btn_grow_light_control_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_manual_mode_btn_grow_light_control, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_manual_mode_btn_grow_light_control_label, LV_PCT(100));

    //Write style for screen_manual_mode_btn_grow_light_control, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_btn_grow_light_control, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_btn_grow_light_control, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_btn_grow_light_control, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_manual_mode_btn_grow_light_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_btn_grow_light_control, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_btn_grow_light_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_manual_mode_btn_grow_light_control, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_manual_mode_btn_grow_light_control, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_manual_mode_btn_grow_light_control, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_manual_mode_btn_grow_light_control, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_btn_ventilation_fan_control
    ui->screen_manual_mode_btn_ventilation_fan_control = lv_button_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_btn_ventilation_fan_control, 10, 145);
    lv_obj_set_size(ui->screen_manual_mode_btn_ventilation_fan_control, 220, 60);
    lv_obj_remove_flag(ui->screen_manual_mode_btn_ventilation_fan_control, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_manual_mode_btn_ventilation_fan_control_label = lv_label_create(ui->screen_manual_mode_btn_ventilation_fan_control);
    lv_label_set_text(ui->screen_manual_mode_btn_ventilation_fan_control_label, "");
    lv_label_set_long_mode(ui->screen_manual_mode_btn_ventilation_fan_control_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_manual_mode_btn_ventilation_fan_control_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_manual_mode_btn_ventilation_fan_control, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_manual_mode_btn_ventilation_fan_control_label, LV_PCT(100));

    //Write style for screen_manual_mode_btn_ventilation_fan_control, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_btn_ventilation_fan_control, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_btn_ventilation_fan_control, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_btn_ventilation_fan_control, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_manual_mode_btn_ventilation_fan_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_btn_ventilation_fan_control, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_btn_ventilation_fan_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_manual_mode_btn_ventilation_fan_control, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_manual_mode_btn_ventilation_fan_control, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_manual_mode_btn_ventilation_fan_control, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_manual_mode_btn_ventilation_fan_control, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_btn_quich_switches_group
    ui->screen_manual_mode_btn_quich_switches_group = lv_button_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_btn_quich_switches_group, 10, 40);
    lv_obj_set_size(ui->screen_manual_mode_btn_quich_switches_group, 220, 100);
    lv_obj_remove_flag(ui->screen_manual_mode_btn_quich_switches_group, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_manual_mode_btn_quich_switches_group_label = lv_label_create(ui->screen_manual_mode_btn_quich_switches_group);
    lv_label_set_text(ui->screen_manual_mode_btn_quich_switches_group_label, "");
    lv_label_set_long_mode(ui->screen_manual_mode_btn_quich_switches_group_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_manual_mode_btn_quich_switches_group_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_manual_mode_btn_quich_switches_group, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_manual_mode_btn_quich_switches_group_label, LV_PCT(100));

    //Write style for screen_manual_mode_btn_quich_switches_group, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_btn_quich_switches_group, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_btn_quich_switches_group, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_btn_quich_switches_group, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_manual_mode_btn_quich_switches_group, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_btn_quich_switches_group, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_btn_quich_switches_group, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_manual_mode_btn_quich_switches_group, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_manual_mode_btn_quich_switches_group, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_manual_mode_btn_quich_switches_group, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_manual_mode_btn_quich_switches_group, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_label_manual_mode
    ui->screen_manual_mode_label_manual_mode = lv_label_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_label_manual_mode, 10, 15);
    lv_obj_set_size(ui->screen_manual_mode_label_manual_mode, 125, 15);
    lv_label_set_text(ui->screen_manual_mode_label_manual_mode, "Manual Mode");
    lv_label_set_long_mode(ui->screen_manual_mode_label_manual_mode, LV_LABEL_LONG_WRAP);

    //Write style for screen_manual_mode_label_manual_mode, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_manual_mode_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_manual_mode_label_manual_mode, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_manual_mode_label_manual_mode, &lv_font_montserratMedium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_manual_mode_label_manual_mode, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_manual_mode_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_manual_mode_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_manual_mode_label_manual_mode, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_manual_mode_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_manual_mode_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_manual_mode_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_manual_mode_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_img_awning
    ui->screen_manual_mode_img_awning = lv_image_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_img_awning, 20, 65);
    lv_obj_set_size(ui->screen_manual_mode_img_awning, 15, 15);
    lv_obj_add_flag(ui->screen_manual_mode_img_awning, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_manual_mode_img_awning, &_Awning_RGB565A8_15x15);
    lv_image_set_pivot(ui->screen_manual_mode_img_awning, 50,50);
    lv_image_set_rotation(ui->screen_manual_mode_img_awning, 0);

    //Write style for screen_manual_mode_img_awning, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_manual_mode_img_awning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_manual_mode_img_awning, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_img_humidifier
    ui->screen_manual_mode_img_humidifier = lv_image_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_img_humidifier, 20, 120);
    lv_obj_set_size(ui->screen_manual_mode_img_humidifier, 15, 15);
    lv_obj_add_flag(ui->screen_manual_mode_img_humidifier, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_manual_mode_img_humidifier, &_Humidifier_RGB565A8_15x15);
    lv_image_set_pivot(ui->screen_manual_mode_img_humidifier, 50,50);
    lv_image_set_rotation(ui->screen_manual_mode_img_humidifier, 0);

    //Write style for screen_manual_mode_img_humidifier, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_manual_mode_img_humidifier, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_manual_mode_img_humidifier, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_img_water_pump
    ui->screen_manual_mode_img_water_pump = lv_image_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_img_water_pump, 20, 93);
    lv_obj_set_size(ui->screen_manual_mode_img_water_pump, 15, 15);
    lv_obj_add_flag(ui->screen_manual_mode_img_water_pump, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_manual_mode_img_water_pump, &_Water_pump_RGB565A8_15x15);
    lv_image_set_pivot(ui->screen_manual_mode_img_water_pump, 50,50);
    lv_image_set_rotation(ui->screen_manual_mode_img_water_pump, 0);

    //Write style for screen_manual_mode_img_water_pump, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_manual_mode_img_water_pump, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_manual_mode_img_water_pump, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_label_quick_switches_groups
    ui->screen_manual_mode_label_quick_switches_groups = lv_label_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_label_quick_switches_groups, 20, 47);
    lv_obj_set_size(ui->screen_manual_mode_label_quick_switches_groups, 200, 15);
    lv_label_set_text(ui->screen_manual_mode_label_quick_switches_groups, "Quick Switches Group");
    lv_label_set_long_mode(ui->screen_manual_mode_label_quick_switches_groups, LV_LABEL_LONG_WRAP);

    //Write style for screen_manual_mode_label_quick_switches_groups, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_manual_mode_label_quick_switches_groups, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_label_quick_switches_groups, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_manual_mode_label_quick_switches_groups, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_manual_mode_label_quick_switches_groups, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_manual_mode_label_quick_switches_groups, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_manual_mode_label_quick_switches_groups, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_manual_mode_label_quick_switches_groups, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_manual_mode_label_quick_switches_groups, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_label_quick_switches_groups, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_manual_mode_label_quick_switches_groups, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_manual_mode_label_quick_switches_groups, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_manual_mode_label_quick_switches_groups, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_manual_mode_label_quick_switches_groups, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_label_quick_switches_groups, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_label_awning
    ui->screen_manual_mode_label_awning = lv_label_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_label_awning, 40, 67);
    lv_obj_set_size(ui->screen_manual_mode_label_awning, 50, 15);
    lv_label_set_text(ui->screen_manual_mode_label_awning, "Awning");
    lv_label_set_long_mode(ui->screen_manual_mode_label_awning, LV_LABEL_LONG_WRAP);

    //Write style for screen_manual_mode_label_awning, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_manual_mode_label_awning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_label_awning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_manual_mode_label_awning, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_manual_mode_label_awning, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_manual_mode_label_awning, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_manual_mode_label_awning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_manual_mode_label_awning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_manual_mode_label_awning, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_label_awning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_manual_mode_label_awning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_manual_mode_label_awning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_manual_mode_label_awning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_manual_mode_label_awning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_label_awning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_sw_awning
    ui->screen_manual_mode_sw_awning = lv_switch_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_sw_awning, 180, 65);
    lv_obj_set_size(ui->screen_manual_mode_sw_awning, 30, 15);

    //Write style for screen_manual_mode_sw_awning, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_sw_awning, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_sw_awning, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_sw_awning, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_manual_mode_sw_awning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_sw_awning, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_sw_awning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_manual_mode_sw_awning, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_sw_awning, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_sw_awning, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_sw_awning, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->screen_manual_mode_sw_awning, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for screen_manual_mode_sw_awning, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_sw_awning, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_sw_awning, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_sw_awning, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_manual_mode_sw_awning, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_sw_awning, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_line_1
    ui->screen_manual_mode_line_1 = lv_line_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_line_1, 45, 85);
    lv_obj_set_size(ui->screen_manual_mode_line_1, 185, 10);
    static lv_point_precise_t screen_manual_mode_line_1[] = {{0, 0},{185, 0}};
    lv_line_set_points(ui->screen_manual_mode_line_1, screen_manual_mode_line_1, 2);

    //Write style for screen_manual_mode_line_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_manual_mode_line_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_manual_mode_line_1, lv_color_hex(0x757575), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_manual_mode_line_1, 100, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_manual_mode_line_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_line_2
    ui->screen_manual_mode_line_2 = lv_line_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_line_2, 45, 113);
    lv_obj_set_size(ui->screen_manual_mode_line_2, 185, 11);
    static lv_point_precise_t screen_manual_mode_line_2[] = {{0, 0},{185, 0}};
    lv_line_set_points(ui->screen_manual_mode_line_2, screen_manual_mode_line_2, 2);

    //Write style for screen_manual_mode_line_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->screen_manual_mode_line_2, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_manual_mode_line_2, lv_color_hex(0x757575), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_manual_mode_line_2, 100, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->screen_manual_mode_line_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_sw_water_pump
    ui->screen_manual_mode_sw_water_pump = lv_switch_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_sw_water_pump, 180, 93);
    lv_obj_set_size(ui->screen_manual_mode_sw_water_pump, 30, 15);

    //Write style for screen_manual_mode_sw_water_pump, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_sw_water_pump, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_sw_water_pump, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_sw_water_pump, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_manual_mode_sw_water_pump, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_sw_water_pump, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_sw_water_pump, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_manual_mode_sw_water_pump, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_sw_water_pump, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_sw_water_pump, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_sw_water_pump, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->screen_manual_mode_sw_water_pump, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for screen_manual_mode_sw_water_pump, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_sw_water_pump, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_sw_water_pump, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_sw_water_pump, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_manual_mode_sw_water_pump, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_sw_water_pump, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_label_water_pump
    ui->screen_manual_mode_label_water_pump = lv_label_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_label_water_pump, 45, 95);
    lv_obj_set_size(ui->screen_manual_mode_label_water_pump, 81, 10);
    lv_label_set_text(ui->screen_manual_mode_label_water_pump, "Water pump");
    lv_label_set_long_mode(ui->screen_manual_mode_label_water_pump, LV_LABEL_LONG_WRAP);

    //Write style for screen_manual_mode_label_water_pump, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_manual_mode_label_water_pump, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_label_water_pump, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_manual_mode_label_water_pump, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_manual_mode_label_water_pump, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_manual_mode_label_water_pump, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_manual_mode_label_water_pump, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_manual_mode_label_water_pump, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_manual_mode_label_water_pump, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_label_water_pump, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_manual_mode_label_water_pump, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_manual_mode_label_water_pump, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_manual_mode_label_water_pump, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_manual_mode_label_water_pump, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_label_water_pump, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_label_humidifier
    ui->screen_manual_mode_label_humidifier = lv_label_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_label_humidifier, 42, 123);
    lv_obj_set_size(ui->screen_manual_mode_label_humidifier, 62, 11);
    lv_label_set_text(ui->screen_manual_mode_label_humidifier, "Humidifier");
    lv_label_set_long_mode(ui->screen_manual_mode_label_humidifier, LV_LABEL_LONG_WRAP);

    //Write style for screen_manual_mode_label_humidifier, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_manual_mode_label_humidifier, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_label_humidifier, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_manual_mode_label_humidifier, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_manual_mode_label_humidifier, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_manual_mode_label_humidifier, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_manual_mode_label_humidifier, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_manual_mode_label_humidifier, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_manual_mode_label_humidifier, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_label_humidifier, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_manual_mode_label_humidifier, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_manual_mode_label_humidifier, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_manual_mode_label_humidifier, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_manual_mode_label_humidifier, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_label_humidifier, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_sw_humidifier
    ui->screen_manual_mode_sw_humidifier = lv_switch_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_sw_humidifier, 180, 120);
    lv_obj_set_size(ui->screen_manual_mode_sw_humidifier, 30, 15);

    //Write style for screen_manual_mode_sw_humidifier, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_sw_humidifier, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_sw_humidifier, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_sw_humidifier, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_manual_mode_sw_humidifier, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_sw_humidifier, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_sw_humidifier, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_manual_mode_sw_humidifier, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_sw_humidifier, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_sw_humidifier, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_sw_humidifier, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->screen_manual_mode_sw_humidifier, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for screen_manual_mode_sw_humidifier, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_sw_humidifier, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_sw_humidifier, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_sw_humidifier, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_manual_mode_sw_humidifier, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_sw_humidifier, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_img_ventilation_fan
    ui->screen_manual_mode_img_ventilation_fan = lv_image_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_img_ventilation_fan, 20, 153);
    lv_obj_set_size(ui->screen_manual_mode_img_ventilation_fan, 15, 15);
    lv_obj_add_flag(ui->screen_manual_mode_img_ventilation_fan, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_manual_mode_img_ventilation_fan, &_Ventilation_Fan_RGB565A8_15x15);
    lv_image_set_pivot(ui->screen_manual_mode_img_ventilation_fan, 50,50);
    lv_image_set_rotation(ui->screen_manual_mode_img_ventilation_fan, 0);

    //Write style for screen_manual_mode_img_ventilation_fan, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_manual_mode_img_ventilation_fan, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_manual_mode_img_ventilation_fan, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_label_ventilation_fan_control
    ui->screen_manual_mode_label_ventilation_fan_control = lv_label_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_label_ventilation_fan_control, 37, 156);
    lv_obj_set_size(ui->screen_manual_mode_label_ventilation_fan_control, 135, 14);
    lv_label_set_text(ui->screen_manual_mode_label_ventilation_fan_control, "Ventilation Fan Control");
    lv_label_set_long_mode(ui->screen_manual_mode_label_ventilation_fan_control, LV_LABEL_LONG_WRAP);

    //Write style for screen_manual_mode_label_ventilation_fan_control, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_manual_mode_label_ventilation_fan_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_label_ventilation_fan_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_manual_mode_label_ventilation_fan_control, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_manual_mode_label_ventilation_fan_control, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_manual_mode_label_ventilation_fan_control, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_manual_mode_label_ventilation_fan_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_manual_mode_label_ventilation_fan_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_manual_mode_label_ventilation_fan_control, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_label_ventilation_fan_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_manual_mode_label_ventilation_fan_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_manual_mode_label_ventilation_fan_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_manual_mode_label_ventilation_fan_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_manual_mode_label_ventilation_fan_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_label_ventilation_fan_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_label_ventilation_fan_speed
    ui->screen_manual_mode_label_ventilation_fan_speed = lv_label_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_label_ventilation_fan_speed, 20, 177);
    lv_obj_set_size(ui->screen_manual_mode_label_ventilation_fan_speed, 57, 15);
    lv_label_set_text(ui->screen_manual_mode_label_ventilation_fan_speed, "speed");
    lv_label_set_long_mode(ui->screen_manual_mode_label_ventilation_fan_speed, LV_LABEL_LONG_WRAP);

    //Write style for screen_manual_mode_label_ventilation_fan_speed, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_manual_mode_label_ventilation_fan_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_label_ventilation_fan_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_manual_mode_label_ventilation_fan_speed, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_manual_mode_label_ventilation_fan_speed, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_manual_mode_label_ventilation_fan_speed, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_manual_mode_label_ventilation_fan_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_manual_mode_label_ventilation_fan_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_manual_mode_label_ventilation_fan_speed, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_label_ventilation_fan_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_manual_mode_label_ventilation_fan_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_manual_mode_label_ventilation_fan_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_manual_mode_label_ventilation_fan_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_manual_mode_label_ventilation_fan_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_label_ventilation_fan_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_label_ventilation_fan_speed_value
    ui->screen_manual_mode_label_ventilation_fan_speed_value = lv_label_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_label_ventilation_fan_speed_value, 160, 177);
    lv_obj_set_size(ui->screen_manual_mode_label_ventilation_fan_speed_value, 60, 15);
    lv_label_set_text(ui->screen_manual_mode_label_ventilation_fan_speed_value, "0 rpm");
    lv_label_set_long_mode(ui->screen_manual_mode_label_ventilation_fan_speed_value, LV_LABEL_LONG_WRAP);

    //Write style for screen_manual_mode_label_ventilation_fan_speed_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_manual_mode_label_ventilation_fan_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_label_ventilation_fan_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_manual_mode_label_ventilation_fan_speed_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_manual_mode_label_ventilation_fan_speed_value, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_manual_mode_label_ventilation_fan_speed_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_manual_mode_label_ventilation_fan_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_manual_mode_label_ventilation_fan_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_manual_mode_label_ventilation_fan_speed_value, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_label_ventilation_fan_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_manual_mode_label_ventilation_fan_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_manual_mode_label_ventilation_fan_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_manual_mode_label_ventilation_fan_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_manual_mode_label_ventilation_fan_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_label_ventilation_fan_speed_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_slider_ventilation_fan_speed
    ui->screen_manual_mode_slider_ventilation_fan_speed = lv_slider_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_slider_ventilation_fan_speed, 20, 195);
    lv_obj_set_size(ui->screen_manual_mode_slider_ventilation_fan_speed, 200, 3);
    lv_slider_set_range(ui->screen_manual_mode_slider_ventilation_fan_speed, 0, 5000);
    lv_slider_set_mode(ui->screen_manual_mode_slider_ventilation_fan_speed, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_manual_mode_slider_ventilation_fan_speed, 0, LV_ANIM_OFF);

    //Write style for screen_manual_mode_slider_ventilation_fan_speed, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_slider_ventilation_fan_speed, 200, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_slider_ventilation_fan_speed, lv_color_hex(0xdcdcdc), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_slider_ventilation_fan_speed, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_slider_ventilation_fan_speed, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_manual_mode_slider_ventilation_fan_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_slider_ventilation_fan_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_manual_mode_slider_ventilation_fan_speed, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_slider_ventilation_fan_speed, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_slider_ventilation_fan_speed, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_slider_ventilation_fan_speed, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_slider_ventilation_fan_speed, 3, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_manual_mode_slider_ventilation_fan_speed, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_slider_ventilation_fan_speed, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_slider_ventilation_fan_speed, lv_color_hex(0x2195f6), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_slider_ventilation_fan_speed, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_slider_ventilation_fan_speed, 8, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_img_grow_light
    ui->screen_manual_mode_img_grow_light = lv_image_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_img_grow_light, 20, 220);
    lv_obj_set_size(ui->screen_manual_mode_img_grow_light, 15, 15);
    lv_obj_add_flag(ui->screen_manual_mode_img_grow_light, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_manual_mode_img_grow_light, &_Fill_Light_RGB565A8_15x15);
    lv_image_set_pivot(ui->screen_manual_mode_img_grow_light, 15,15);
    lv_image_set_rotation(ui->screen_manual_mode_img_grow_light, 0);

    //Write style for screen_manual_mode_img_grow_light, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_manual_mode_img_grow_light, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_manual_mode_img_grow_light, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_label_grow_light_control
    ui->screen_manual_mode_label_grow_light_control = lv_label_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_label_grow_light_control, 43, 223);
    lv_obj_set_size(ui->screen_manual_mode_label_grow_light_control, 135, 14);
    lv_label_set_text(ui->screen_manual_mode_label_grow_light_control, "Grow Light Control");
    lv_label_set_long_mode(ui->screen_manual_mode_label_grow_light_control, LV_LABEL_LONG_WRAP);

    //Write style for screen_manual_mode_label_grow_light_control, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_manual_mode_label_grow_light_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_label_grow_light_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_manual_mode_label_grow_light_control, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_manual_mode_label_grow_light_control, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_manual_mode_label_grow_light_control, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_manual_mode_label_grow_light_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_manual_mode_label_grow_light_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_manual_mode_label_grow_light_control, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_label_grow_light_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_manual_mode_label_grow_light_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_manual_mode_label_grow_light_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_manual_mode_label_grow_light_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_manual_mode_label_grow_light_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_label_grow_light_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_label_brightness
    ui->screen_manual_mode_label_brightness = lv_label_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_label_brightness, 20, 243);
    lv_obj_set_size(ui->screen_manual_mode_label_brightness, 57, 15);
    lv_label_set_text(ui->screen_manual_mode_label_brightness, "brightness");
    lv_label_set_long_mode(ui->screen_manual_mode_label_brightness, LV_LABEL_LONG_WRAP);

    //Write style for screen_manual_mode_label_brightness, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_manual_mode_label_brightness, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_label_brightness, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_manual_mode_label_brightness, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_manual_mode_label_brightness, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_manual_mode_label_brightness, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_manual_mode_label_brightness, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_manual_mode_label_brightness, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_manual_mode_label_brightness, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_label_brightness, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_manual_mode_label_brightness, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_manual_mode_label_brightness, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_manual_mode_label_brightness, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_manual_mode_label_brightness, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_label_brightness, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_slider_brightness_value
    ui->screen_manual_mode_slider_brightness_value = lv_slider_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_slider_brightness_value, 20, 260);
    lv_obj_set_size(ui->screen_manual_mode_slider_brightness_value, 200, 3);
    lv_slider_set_range(ui->screen_manual_mode_slider_brightness_value, 0, 100);
    lv_slider_set_mode(ui->screen_manual_mode_slider_brightness_value, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_manual_mode_slider_brightness_value, 0, LV_ANIM_OFF);

    //Write style for screen_manual_mode_slider_brightness_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_slider_brightness_value, 200, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_slider_brightness_value, lv_color_hex(0xdcdcdc), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_slider_brightness_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_slider_brightness_value, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_manual_mode_slider_brightness_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_slider_brightness_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_manual_mode_slider_brightness_value, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_slider_brightness_value, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_slider_brightness_value, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_slider_brightness_value, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_slider_brightness_value, 3, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_manual_mode_slider_brightness_value, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_slider_brightness_value, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_slider_brightness_value, lv_color_hex(0x2195f6), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_slider_brightness_value, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_slider_brightness_value, 8, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_label_brightness_value
    ui->screen_manual_mode_label_brightness_value = lv_label_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_label_brightness_value, 169, 243);
    lv_obj_set_size(ui->screen_manual_mode_label_brightness_value, 50, 15);
    lv_label_set_text(ui->screen_manual_mode_label_brightness_value, "0");
    lv_label_set_long_mode(ui->screen_manual_mode_label_brightness_value, LV_LABEL_LONG_WRAP);

    //Write style for screen_manual_mode_label_brightness_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_manual_mode_label_brightness_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_label_brightness_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_manual_mode_label_brightness_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_manual_mode_label_brightness_value, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_manual_mode_label_brightness_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_manual_mode_label_brightness_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_manual_mode_label_brightness_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_manual_mode_label_brightness_value, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_label_brightness_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_manual_mode_label_brightness_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_manual_mode_label_brightness_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_manual_mode_label_brightness_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_manual_mode_label_brightness_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_label_brightness_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_label_color_adjustment
    ui->screen_manual_mode_label_color_adjustment = lv_label_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_label_color_adjustment, 20, 275);
    lv_obj_set_size(ui->screen_manual_mode_label_color_adjustment, 100, 15);
    lv_label_set_text(ui->screen_manual_mode_label_color_adjustment, "color adjustment");
    lv_label_set_long_mode(ui->screen_manual_mode_label_color_adjustment, LV_LABEL_LONG_WRAP);

    //Write style for screen_manual_mode_label_color_adjustment, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_manual_mode_label_color_adjustment, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_label_color_adjustment, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_manual_mode_label_color_adjustment, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_manual_mode_label_color_adjustment, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_manual_mode_label_color_adjustment, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_manual_mode_label_color_adjustment, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_manual_mode_label_color_adjustment, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_manual_mode_label_color_adjustment, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_label_color_adjustment, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_manual_mode_label_color_adjustment, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_manual_mode_label_color_adjustment, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_manual_mode_label_color_adjustment, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_manual_mode_label_color_adjustment, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_label_color_adjustment, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_manual_mode_slider_color_adjustment
    ui->screen_manual_mode_slider_color_adjustment = lv_slider_create(ui->screen_manual_mode);
    lv_obj_set_pos(ui->screen_manual_mode_slider_color_adjustment, 20, 295);
    lv_obj_set_size(ui->screen_manual_mode_slider_color_adjustment, 200, 3);
    lv_slider_set_range(ui->screen_manual_mode_slider_color_adjustment, 0, 360);
    lv_slider_set_mode(ui->screen_manual_mode_slider_color_adjustment, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_manual_mode_slider_color_adjustment, 0, LV_ANIM_OFF);

    //Write style for screen_manual_mode_slider_color_adjustment, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_slider_color_adjustment, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_slider_color_adjustment, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_manual_mode_slider_color_adjustment, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_src(ui->screen_manual_mode_slider_color_adjustment, &_Rainbow_Slider_RGB565A8_200x3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_opa(ui->screen_manual_mode_slider_color_adjustment, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_recolor_opa(ui->screen_manual_mode_slider_color_adjustment, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_manual_mode_slider_color_adjustment, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_manual_mode_slider_color_adjustment, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_slider_color_adjustment, 0, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_slider_color_adjustment, 3, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_manual_mode_slider_color_adjustment, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_manual_mode_slider_color_adjustment, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_manual_mode_slider_color_adjustment, lv_color_hex(0xc8c8c8), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_manual_mode_slider_color_adjustment, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_manual_mode_slider_color_adjustment, 8, LV_PART_KNOB|LV_STATE_DEFAULT);

    //The custom code of screen_manual_mode.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_manual_mode);

    //Init events for screen.
    events_init_screen_manual_mode(ui);
}
