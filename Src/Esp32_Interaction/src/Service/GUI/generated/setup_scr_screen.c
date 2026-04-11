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



void setup_scr_screen(lv_ui *ui)
{
    //Write codes screen
    ui->screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_slider_1
    ui->screen_slider_1 = lv_slider_create(ui->screen);
    lv_obj_set_pos(ui->screen_slider_1, 42, 116);
    lv_obj_set_size(ui->screen_slider_1, 160, 8);
    lv_slider_set_range(ui->screen_slider_1, 0, 100);
    lv_slider_set_mode(ui->screen_slider_1, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_slider_1, 50, LV_ANIM_OFF);

    //Write style for screen_slider_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_slider_1, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_slider_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_slider_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_slider_1, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_slider_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_slider_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_slider_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_slider_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_slider_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_slider_1, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_slider_1, 8, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_slider_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_slider_1, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_slider_1, lv_color_hex(0x2195f6), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_slider_1, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_slider_1, 8, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_arc_1
    ui->screen_arc_1 = lv_arc_create(ui->screen);
    lv_obj_set_pos(ui->screen_arc_1, 62, 164);
    lv_obj_set_size(ui->screen_arc_1, 100, 100);
    lv_arc_set_mode(ui->screen_arc_1, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(ui->screen_arc_1, 0, 100);
    lv_arc_set_bg_angles(ui->screen_arc_1, 135, 45);
    lv_arc_set_value(ui->screen_arc_1, 70);
    lv_arc_set_rotation(ui->screen_arc_1, 0);

    //Write style for screen_arc_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_arc_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_arc_1, lv_color_hex(0xf6f6f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_arc_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->screen_arc_1, 12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->screen_arc_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->screen_arc_1, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->screen_arc_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_arc_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_arc_1, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_arc_1, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_arc_1, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_arc_1, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_arc_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->screen_arc_1, 12, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->screen_arc_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->screen_arc_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->screen_arc_1, true, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_arc_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_arc_1, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_arc_1, lv_color_hex(0x2195f6), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_arc_1, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->screen_arc_1, 5, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_led_1
    ui->screen_led_1 = lv_led_create(ui->screen);
    lv_obj_set_pos(ui->screen_led_1, 93, 45);
    lv_obj_set_size(ui->screen_led_1, 40, 40);
    lv_led_set_brightness(ui->screen_led_1, 255);
    lv_led_set_color(ui->screen_led_1, lv_color_hex(0x00a1b5));

    //The custom code of screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen);

    //Init events for screen.
    events_init_screen(ui);
}
