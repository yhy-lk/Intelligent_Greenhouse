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



void setup_scr_screen_ai_pilot_mode(lv_ui *ui)
{
    //Write codes screen_ai_pilot_mode
    ui->screen_ai_pilot_mode = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_ai_pilot_mode, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_ai_pilot_mode, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_ai_pilot_mode, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_ai_pilot_mode, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_ai_pilot_mode_img_1
    ui->screen_ai_pilot_mode_img_1 = lv_image_create(ui->screen_ai_pilot_mode);
    lv_obj_set_pos(ui->screen_ai_pilot_mode_img_1, 0, 0);
    lv_obj_set_size(ui->screen_ai_pilot_mode_img_1, 240, 320);
    lv_obj_add_flag(ui->screen_ai_pilot_mode_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_ai_pilot_mode_img_1, &_Auto_Mode_Background_RGB565A8_240x320);
    lv_image_set_pivot(ui->screen_ai_pilot_mode_img_1, 50,50);
    lv_image_set_rotation(ui->screen_ai_pilot_mode_img_1, 0);

    //Write style for screen_ai_pilot_mode_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_ai_pilot_mode_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_ai_pilot_mode_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_ai_pilot_mode_label_1
    ui->screen_ai_pilot_mode_label_1 = lv_label_create(ui->screen_ai_pilot_mode);
    lv_obj_set_pos(ui->screen_ai_pilot_mode_label_1, 0, 160);
    lv_obj_set_size(ui->screen_ai_pilot_mode_label_1, 240, 40);
    lv_label_set_text(ui->screen_ai_pilot_mode_label_1, "Voice service started");
    lv_label_set_long_mode(ui->screen_ai_pilot_mode_label_1, LV_LABEL_LONG_WRAP);

    //Write style for screen_ai_pilot_mode_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_ai_pilot_mode_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_ai_pilot_mode_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_ai_pilot_mode_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_ai_pilot_mode_label_1, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_ai_pilot_mode_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_ai_pilot_mode_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_ai_pilot_mode_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_ai_pilot_mode_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_ai_pilot_mode_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_ai_pilot_mode_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_ai_pilot_mode_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_ai_pilot_mode_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_ai_pilot_mode_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_ai_pilot_mode_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_ai_pilot_mode.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_ai_pilot_mode);

    //Init events for screen.
    events_init_screen_ai_pilot_mode(ui);
}
