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



void setup_scr_screen_control(lv_ui *ui)
{
    //Write codes screen_control
    ui->screen_control = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_control, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_control, LV_SCROLLBAR_MODE_OFF);
    lv_obj_remove_flag(ui->screen_control, LV_OBJ_FLAG_SCROLLABLE);

    //Write style for screen_control, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_control, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_control, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_control, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_btn_background
    ui->screen_control_btn_background = lv_button_create(ui->screen_control);
    lv_obj_set_pos(ui->screen_control_btn_background, 0, 0);
    lv_obj_set_size(ui->screen_control_btn_background, 240, 320);
    lv_obj_remove_flag(ui->screen_control_btn_background, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_control_btn_background_label = lv_label_create(ui->screen_control_btn_background);
    lv_label_set_text(ui->screen_control_btn_background_label, "");
    lv_label_set_long_mode(ui->screen_control_btn_background_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_control_btn_background_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_control_btn_background, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_control_btn_background_label, LV_PCT(100));

    //Write style for screen_control_btn_background, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_control_btn_background, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_control_btn_background, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_control_btn_background, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_control_btn_background, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_btn_background, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_btn_background, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_btn_background, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_btn_background, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_btn_background, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_btn_background, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_img_background
    ui->screen_control_img_background = lv_image_create(ui->screen_control);
    lv_obj_set_pos(ui->screen_control_img_background, 0, 0);
    lv_obj_set_size(ui->screen_control_img_background, 240, 320);
    lv_obj_remove_flag(ui->screen_control_img_background, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(ui->screen_control_img_background, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_control_img_background, &_Control_Mode_Background_RGB565A8_240x320);
    lv_image_set_pivot(ui->screen_control_img_background, 50,50);
    lv_image_set_rotation(ui->screen_control_img_background, 0);

    //Write style for screen_control_img_background, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_control_img_background, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_control_img_background, 181, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_label_control_mode
    ui->screen_control_label_control_mode = lv_label_create(ui->screen_control);
    lv_obj_set_pos(ui->screen_control_label_control_mode, 20, 20);
    lv_obj_set_size(ui->screen_control_label_control_mode, 200, 32);
    lv_label_set_text(ui->screen_control_label_control_mode, "Control Mode");
    lv_label_set_long_mode(ui->screen_control_label_control_mode, LV_LABEL_LONG_WRAP);

    //Write style for screen_control_label_control_mode, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_control_label_control_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_label_control_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_label_control_mode, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_control_mode, &lv_font_montserratMedium_25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_label_control_mode, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_control_label_control_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_control_label_control_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_label_control_mode, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_control_label_control_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_control_label_control_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_control_label_control_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_control_label_control_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_control_label_control_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_label_control_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_btn_manual_mode
    ui->screen_control_btn_manual_mode = lv_button_create(ui->screen_control);
    lv_obj_set_pos(ui->screen_control_btn_manual_mode, 10, 90);
    lv_obj_set_size(ui->screen_control_btn_manual_mode, 220, 60);
    ui->screen_control_btn_manual_mode_label = lv_label_create(ui->screen_control_btn_manual_mode);
    lv_label_set_text(ui->screen_control_btn_manual_mode_label, "");
    lv_label_set_long_mode(ui->screen_control_btn_manual_mode_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_control_btn_manual_mode_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_control_btn_manual_mode, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_control_btn_manual_mode_label, LV_PCT(100));

    //Write style for screen_control_btn_manual_mode, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_control_btn_manual_mode, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_control_btn_manual_mode, lv_color_hex(0xdcdcdc), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_control_btn_manual_mode, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_control_btn_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_btn_manual_mode, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_btn_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_btn_manual_mode, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_btn_manual_mode, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_btn_manual_mode, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_btn_manual_mode, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_btn_auto_mode
    ui->screen_control_btn_auto_mode = lv_button_create(ui->screen_control);
    lv_obj_set_pos(ui->screen_control_btn_auto_mode, 10, 170);
    lv_obj_set_size(ui->screen_control_btn_auto_mode, 220, 60);
    ui->screen_control_btn_auto_mode_label = lv_label_create(ui->screen_control_btn_auto_mode);
    lv_label_set_text(ui->screen_control_btn_auto_mode_label, "");
    lv_label_set_long_mode(ui->screen_control_btn_auto_mode_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_control_btn_auto_mode_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_control_btn_auto_mode, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_control_btn_auto_mode_label, LV_PCT(100));

    //Write style for screen_control_btn_auto_mode, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_control_btn_auto_mode, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_control_btn_auto_mode, lv_color_hex(0xdcdcdc), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_control_btn_auto_mode, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_control_btn_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_btn_auto_mode, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_btn_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_btn_auto_mode, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_btn_auto_mode, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_btn_auto_mode, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_btn_auto_mode, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_btn_ai_pilot_mode
    ui->screen_control_btn_ai_pilot_mode = lv_button_create(ui->screen_control);
    lv_obj_set_pos(ui->screen_control_btn_ai_pilot_mode, 10, 250);
    lv_obj_set_size(ui->screen_control_btn_ai_pilot_mode, 220, 60);
    ui->screen_control_btn_ai_pilot_mode_label = lv_label_create(ui->screen_control_btn_ai_pilot_mode);
    lv_label_set_text(ui->screen_control_btn_ai_pilot_mode_label, "");
    lv_label_set_long_mode(ui->screen_control_btn_ai_pilot_mode_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_control_btn_ai_pilot_mode_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_control_btn_ai_pilot_mode, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_control_btn_ai_pilot_mode_label, LV_PCT(100));

    //Write style for screen_control_btn_ai_pilot_mode, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_control_btn_ai_pilot_mode, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_control_btn_ai_pilot_mode, lv_color_hex(0xdcdcdc), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_control_btn_ai_pilot_mode, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_control_btn_ai_pilot_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_btn_ai_pilot_mode, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_btn_ai_pilot_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_btn_ai_pilot_mode, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_btn_ai_pilot_mode, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_btn_ai_pilot_mode, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_btn_ai_pilot_mode, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_label_control_mode_contents
    ui->screen_control_label_control_mode_contents = lv_label_create(ui->screen_control);
    lv_obj_set_pos(ui->screen_control_label_control_mode_contents, 20, 52);
    lv_obj_set_size(ui->screen_control_label_control_mode_contents, 200, 32);
    lv_label_set_text(ui->screen_control_label_control_mode_contents, "Select Your Operation Style");
    lv_label_set_long_mode(ui->screen_control_label_control_mode_contents, LV_LABEL_LONG_WRAP);

    //Write style for screen_control_label_control_mode_contents, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_control_label_control_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_label_control_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_label_control_mode_contents, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_control_mode_contents, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_label_control_mode_contents, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_control_label_control_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_control_label_control_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_label_control_mode_contents, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_control_label_control_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_control_label_control_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_control_label_control_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_control_label_control_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_control_label_control_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_label_control_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_img_manual_mode
    ui->screen_control_img_manual_mode = lv_image_create(ui->screen_control);
    lv_obj_set_pos(ui->screen_control_img_manual_mode, 20, 98);
    lv_obj_set_size(ui->screen_control_img_manual_mode, 50, 45);
    lv_obj_add_flag(ui->screen_control_img_manual_mode, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_control_img_manual_mode, &_Manual_Mode_RGB565A8_50x45);
    lv_image_set_pivot(ui->screen_control_img_manual_mode, 50,50);
    lv_image_set_rotation(ui->screen_control_img_manual_mode, 0);

    //Write style for screen_control_img_manual_mode, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_control_img_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_control_img_manual_mode, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_label_manual_mode
    ui->screen_control_label_manual_mode = lv_label_create(ui->screen_control);
    lv_obj_set_pos(ui->screen_control_label_manual_mode, 100, 105);
    lv_obj_set_size(ui->screen_control_label_manual_mode, 100, 20);
    lv_label_set_text(ui->screen_control_label_manual_mode, "Manual Mode");
    lv_label_set_long_mode(ui->screen_control_label_manual_mode, LV_LABEL_LONG_WRAP);

    //Write style for screen_control_label_manual_mode, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_control_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_label_manual_mode, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_manual_mode, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_label_manual_mode, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_control_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_control_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_label_manual_mode, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_control_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_control_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_control_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_control_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_control_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_label_manual_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_label_manual_mode_contents
    ui->screen_control_label_manual_mode_contents = lv_label_create(ui->screen_control);
    lv_obj_set_pos(ui->screen_control_label_manual_mode_contents, 78, 125);
    lv_obj_set_size(ui->screen_control_label_manual_mode_contents, 150, 15);
    lv_label_set_text(ui->screen_control_label_manual_mode_contents, "Full user control over all settings.");
    lv_label_set_long_mode(ui->screen_control_label_manual_mode_contents, LV_LABEL_LONG_WRAP);

    //Write style for screen_control_label_manual_mode_contents, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_control_label_manual_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_label_manual_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_label_manual_mode_contents, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_manual_mode_contents, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_label_manual_mode_contents, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_control_label_manual_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_control_label_manual_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_label_manual_mode_contents, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_control_label_manual_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_control_label_manual_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_control_label_manual_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_control_label_manual_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_control_label_manual_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_label_manual_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_img_auto_mode
    ui->screen_control_img_auto_mode = lv_image_create(ui->screen_control);
    lv_obj_set_pos(ui->screen_control_img_auto_mode, 20, 178);
    lv_obj_set_size(ui->screen_control_img_auto_mode, 50, 45);
    lv_obj_add_flag(ui->screen_control_img_auto_mode, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_control_img_auto_mode, &_Auto_Mode_RGB565A8_50x45);
    lv_image_set_pivot(ui->screen_control_img_auto_mode, 50,50);
    lv_image_set_rotation(ui->screen_control_img_auto_mode, 0);

    //Write style for screen_control_img_auto_mode, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_control_img_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_control_img_auto_mode, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_label_auto_mode
    ui->screen_control_label_auto_mode = lv_label_create(ui->screen_control);
    lv_obj_set_pos(ui->screen_control_label_auto_mode, 110, 185);
    lv_obj_set_size(ui->screen_control_label_auto_mode, 100, 20);
    lv_label_set_text(ui->screen_control_label_auto_mode, "Auto Mode");
    lv_label_set_long_mode(ui->screen_control_label_auto_mode, LV_LABEL_LONG_WRAP);

    //Write style for screen_control_label_auto_mode, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_control_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_label_auto_mode, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_auto_mode, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_label_auto_mode, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_control_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_control_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_label_auto_mode, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_control_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_control_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_control_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_control_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_control_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_label_auto_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_label_auto_mode_contents
    ui->screen_control_label_auto_mode_contents = lv_label_create(ui->screen_control);
    lv_obj_set_pos(ui->screen_control_label_auto_mode_contents, 74, 205);
    lv_obj_set_size(ui->screen_control_label_auto_mode_contents, 157, 15);
    lv_label_set_text(ui->screen_control_label_auto_mode_contents, "System adjusts basedon preset rules.");
    lv_label_set_long_mode(ui->screen_control_label_auto_mode_contents, LV_LABEL_LONG_WRAP);

    //Write style for screen_control_label_auto_mode_contents, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_control_label_auto_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_label_auto_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_label_auto_mode_contents, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_auto_mode_contents, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_label_auto_mode_contents, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_control_label_auto_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_control_label_auto_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_label_auto_mode_contents, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_control_label_auto_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_control_label_auto_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_control_label_auto_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_control_label_auto_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_control_label_auto_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_label_auto_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_img_ai_pilot_mode
    ui->screen_control_img_ai_pilot_mode = lv_image_create(ui->screen_control);
    lv_obj_set_pos(ui->screen_control_img_ai_pilot_mode, 20, 258);
    lv_obj_set_size(ui->screen_control_img_ai_pilot_mode, 50, 45);
    lv_obj_add_flag(ui->screen_control_img_ai_pilot_mode, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_control_img_ai_pilot_mode, &_Ai_Pilot_RGB565A8_50x45);
    lv_image_set_pivot(ui->screen_control_img_ai_pilot_mode, 50,50);
    lv_image_set_rotation(ui->screen_control_img_ai_pilot_mode, 0);

    //Write style for screen_control_img_ai_pilot_mode, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_control_img_ai_pilot_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_control_img_ai_pilot_mode, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_label_ai_pilot_mode
    ui->screen_control_label_ai_pilot_mode = lv_label_create(ui->screen_control);
    lv_obj_set_pos(ui->screen_control_label_ai_pilot_mode, 100, 265);
    lv_obj_set_size(ui->screen_control_label_ai_pilot_mode, 100, 20);
    lv_label_set_text(ui->screen_control_label_ai_pilot_mode, "AI Pilot Mode");
    lv_label_set_long_mode(ui->screen_control_label_ai_pilot_mode, LV_LABEL_LONG_WRAP);

    //Write style for screen_control_label_ai_pilot_mode, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_control_label_ai_pilot_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_label_ai_pilot_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_label_ai_pilot_mode, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_ai_pilot_mode, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_label_ai_pilot_mode, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_control_label_ai_pilot_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_control_label_ai_pilot_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_label_ai_pilot_mode, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_control_label_ai_pilot_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_control_label_ai_pilot_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_control_label_ai_pilot_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_control_label_ai_pilot_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_control_label_ai_pilot_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_label_ai_pilot_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_label_ai_pilot_mode_contents
    ui->screen_control_label_ai_pilot_mode_contents = lv_label_create(ui->screen_control);
    lv_obj_set_pos(ui->screen_control_label_ai_pilot_mode_contents, 78, 285);
    lv_obj_set_size(ui->screen_control_label_ai_pilot_mode_contents, 150, 16);
    lv_label_set_text(ui->screen_control_label_ai_pilot_mode_contents, "Al optimizes for peak performance.");
    lv_label_set_long_mode(ui->screen_control_label_ai_pilot_mode_contents, LV_LABEL_LONG_WRAP);

    //Write style for screen_control_label_ai_pilot_mode_contents, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_control_label_ai_pilot_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_label_ai_pilot_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_label_ai_pilot_mode_contents, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_ai_pilot_mode_contents, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_label_ai_pilot_mode_contents, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_control_label_ai_pilot_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_control_label_ai_pilot_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_label_ai_pilot_mode_contents, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_control_label_ai_pilot_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_control_label_ai_pilot_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_control_label_ai_pilot_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_control_label_ai_pilot_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_control_label_ai_pilot_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_label_ai_pilot_mode_contents, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_control.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_control);

    //Init events for screen.
    events_init_screen_control(ui);
}
