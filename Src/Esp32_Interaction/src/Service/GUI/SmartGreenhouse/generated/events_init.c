/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

#include "custom.h"
// #include "custom.h"


static void screen_home_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOAD_START:
    {

        break;
    }
    default:
        break;
    }
}

static void screen_home_img_control_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_control, guider_ui.screen_control_del, &guider_ui.screen_home_del, setup_scr_screen_control, LV_SCR_LOAD_ANIM_FADE_ON, 250, 50, true, true);
        break;
    }
    default:
        break;
    }
}

static void screen_home_img_overview_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_overview, guider_ui.screen_overview_del, &guider_ui.screen_home_del, setup_scr_screen_overview, LV_SCR_LOAD_ANIM_FADE_ON, 250, 50, true, true);
        break;
    }
    default:
        break;
    }
}

static void screen_home_img_setting_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_setting, guider_ui.screen_setting_del, &guider_ui.screen_home_del, setup_scr_screen_setting, LV_SCR_LOAD_ANIM_FADE_ON, 250, 50, true, true);
        break;
    }
    default:
        break;
    }
}

static void screen_home_img_trend_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_trend, guider_ui.screen_trend_del, &guider_ui.screen_home_del, setup_scr_screen_trend, LV_SCR_LOAD_ANIM_FADE_ON, 250, 50, true, true);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_home (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_home, screen_home_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_home_img_control, screen_home_img_control_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_home_img_overview, screen_home_img_overview_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_home_img_setting, screen_home_img_setting_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_home_img_trend, screen_home_img_trend_event_handler, LV_EVENT_ALL, ui);
}

static void screen_overview_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_TOP:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_home, guider_ui.screen_home_del, &guider_ui.screen_overview_del, setup_scr_screen_home, LV_SCR_LOAD_ANIM_FADE_ON, 200, 100, false, false);
            break;
        }
        case LV_DIR_LEFT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_control, guider_ui.screen_control_del, &guider_ui.screen_overview_del, setup_scr_screen_control, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

void events_init_screen_overview (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_overview, screen_overview_event_handler, LV_EVENT_ALL, ui);
}

static void screen_control_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_TOP:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_home, guider_ui.screen_home_del, &guider_ui.screen_control_del, setup_scr_screen_home, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, false);
            break;
        }
        case LV_DIR_LEFT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_trend, guider_ui.screen_trend_del, &guider_ui.screen_control_del, setup_scr_screen_trend, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false, false);
            break;
        }
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_overview, guider_ui.screen_overview_del, &guider_ui.screen_control_del, setup_scr_screen_overview, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, false, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void screen_control_btn_manual_mode_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_manual_mode, guider_ui.screen_manual_mode_del, &guider_ui.screen_control_del, setup_scr_screen_manual_mode, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 200, true, true);
        break;
    }
    default:
        break;
    }
}

static void screen_control_btn_auto_mode_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_auto_mode, guider_ui.screen_auto_mode_del, &guider_ui.screen_control_del, setup_scr_screen_auto_mode, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 200, true, true);
        break;
    }
    default:
        break;
    }
}

static void screen_control_btn_ai_pilot_mode_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_ai_pilot_mode, guider_ui.screen_ai_pilot_mode_del, &guider_ui.screen_control_del, setup_scr_screen_ai_pilot_mode, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 200, true, true);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_control (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_control, screen_control_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_control_btn_manual_mode, screen_control_btn_manual_mode_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_control_btn_auto_mode, screen_control_btn_auto_mode_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_control_btn_ai_pilot_mode, screen_control_btn_ai_pilot_mode_event_handler, LV_EVENT_ALL, ui);
}

static void screen_trend_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_TOP:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_home, guider_ui.screen_home_del, &guider_ui.screen_trend_del, setup_scr_screen_home, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, false);
            break;
        }
        case LV_DIR_LEFT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_setting, guider_ui.screen_setting_del, &guider_ui.screen_trend_del, setup_scr_screen_setting, LV_SCR_LOAD_ANIM_OVER_LEFT, 200, 200, false, false);
            break;
        }
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_control, guider_ui.screen_control_del, &guider_ui.screen_trend_del, setup_scr_screen_control, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 200, false, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

void events_init_screen_trend (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_trend, screen_trend_event_handler, LV_EVENT_ALL, ui);
}

static void screen_setting_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_TOP:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_home, guider_ui.screen_home_del, &guider_ui.screen_setting_del, setup_scr_screen_home, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, false);
            break;
        }
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_trend, guider_ui.screen_trend_del, &guider_ui.screen_setting_del, setup_scr_screen_trend, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 200, false, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

void events_init_screen_setting (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_setting, screen_setting_event_handler, LV_EVENT_ALL, ui);
}

static void screen_manual_mode_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOAD_START:
    {
        custom_ui_send_manual_control_mode_command_on_screen_enter();
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_control, guider_ui.screen_control_del, &guider_ui.screen_manual_mode_del, setup_scr_screen_control, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 200, false, false);
            break;
        }
        case LV_DIR_TOP:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_home, guider_ui.screen_home_del, &guider_ui.screen_manual_mode_del, setup_scr_screen_home, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void screen_manual_mode_sw_awning_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        lv_obj_t * status_obj = lv_event_get_target(e);
        int status = lv_obj_has_state(status_obj, LV_STATE_CHECKED) ? true : false;
        custom_ui_handle_awning_sunshade_motor_switch_value_changed_event(e);


        break;
    }
    default:
        break;
    }
}

static void screen_manual_mode_sw_water_pump_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        lv_obj_t * status_obj = lv_event_get_target(e);
        int status = lv_obj_has_state(status_obj, LV_STATE_CHECKED) ? true : false;
        custom_ui_handle_water_pump_switch_value_changed_event(e);
        break;
    }
    default:
        break;
    }
}

static void screen_manual_mode_sw_humidifier_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        lv_obj_t * status_obj = lv_event_get_target(e);
        int status = lv_obj_has_state(status_obj, LV_STATE_CHECKED) ? true : false;
        custom_ui_handle_humidifier_switch_value_changed_event(e);
        break;
    }
    default:
        break;
    }
}

static void screen_manual_mode_slider_ventilation_fan_speed_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        int32_t val = lv_slider_get_value(guider_ui.screen_manual_mode_slider_ventilation_fan_speed);

        char str[32];

        sprintf(str, "%d rpm", val);

        lv_label_set_text(guider_ui.screen_manual_mode_label_ventilation_fan_speed_value, str);

        custom_ui_handle_ventilation_fan_speed_slider_value_changed_event(e);
        break;
    }
    default:
        break;
    }
}

static void screen_manual_mode_slider_brightness_value_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        int32_t val = lv_slider_get_value(guider_ui.screen_manual_mode_slider_brightness_value);

        char str[32];

        sprintf(str, "%d", val);

        lv_label_set_text(guider_ui.screen_manual_mode_label_brightness_value, str);
        custom_ui_handle_light_brightness_slider_value_changed_event(e);
        break;
    }
    default:
        break;
    }
}

static void screen_manual_mode_slider_color_adjustment_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        custom_ui_handle_light_color_slider_value_changed_event(e);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_manual_mode (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_manual_mode, screen_manual_mode_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_manual_mode_sw_awning, screen_manual_mode_sw_awning_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_manual_mode_sw_water_pump, screen_manual_mode_sw_water_pump_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_manual_mode_sw_humidifier, screen_manual_mode_sw_humidifier_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_manual_mode_slider_ventilation_fan_speed, screen_manual_mode_slider_ventilation_fan_speed_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_manual_mode_slider_brightness_value, screen_manual_mode_slider_brightness_value_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_manual_mode_slider_color_adjustment, screen_manual_mode_slider_color_adjustment_event_handler, LV_EVENT_ALL, ui);
}

static void screen_auto_mode_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOAD_START:
    {
        custom_ui_send_auto_control_mode_command_on_screen_enter();
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_control, guider_ui.screen_control_del, &guider_ui.screen_auto_mode_del, setup_scr_screen_control, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 200, false, false);
            break;
        }
        case LV_DIR_TOP:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_home, guider_ui.screen_home_del, &guider_ui.screen_auto_mode_del, setup_scr_screen_home, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void screen_auto_mode_slider_temperature_range_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        int left_val = lv_slider_get_left_value(guider_ui.screen_auto_mode_slider_temperature_range);
        char left_str[32];
        sprintf(left_str, "%d°C", left_val);
        lv_label_set_text(guider_ui.screen_auto_mode_label_temperature_left_value, left_str);

        int right_val = lv_slider_get_value(guider_ui.screen_auto_mode_slider_temperature_range);
        char right_str[32];
        sprintf(right_str, "%d°C", right_val);
        lv_label_set_text(guider_ui.screen_auto_mode_label_temperature_right_value, right_str);

        custom_ui_handle_auto_mode_temperature_range_slider_value_changed_event(e);


        break;
    }
    default:
        break;
    }
}

static void screen_auto_mode_slider_humidity_range_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        int left_val = lv_slider_get_left_value(guider_ui.screen_auto_mode_slider_humidity_range);
        char left_str[32];
        sprintf(left_str, "%d%%", left_val);
        lv_label_set_text(guider_ui.screen_auto_mode_label_humidity_left_value, left_str);

        int right_val = lv_slider_get_value(guider_ui.screen_auto_mode_slider_humidity_range);
        char right_str[32];
        sprintf(right_str, "%d%%", right_val);
        lv_label_set_text(guider_ui.screen_auto_mode_label_humidity_right_value, right_str);



        custom_ui_handle_auto_mode_humidity_range_slider_value_changed_event(e);
        break;
    }
    default:
        break;
    }
}

static void screen_auto_mode_slider_soil_moisture_range_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        int left_val = lv_slider_get_left_value(guider_ui.screen_auto_mode_slider_soil_moisture_range);
        char left_str[32];
        sprintf(left_str, "%d%%", left_val);
        lv_label_set_text(guider_ui.screen_auto_mode_label_soil_moisture_left_value, left_str);

        int right_val = lv_slider_get_value(guider_ui.screen_auto_mode_slider_soil_moisture_range);
        char right_str[32];
        sprintf(right_str, "%d%%", right_val);
        lv_label_set_text(guider_ui.screen_auto_mode_label_soil_moisture_right_value, right_str);


        custom_ui_handle_auto_mode_soil_moisture_range_slider_value_changed_event(e);

        break;
    }
    default:
        break;
    }
}

static void screen_auto_mode_slider_light_intensity_range_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {
        int left_val = lv_slider_get_left_value(guider_ui.screen_auto_mode_slider_light_intensity_range);
        char left_str[32];
        sprintf(left_str, "%d lx", left_val);
        lv_label_set_text(guider_ui.screen_auto_mode_label_light_intensity_left_value, left_str);

        int right_val = lv_slider_get_value(guider_ui.screen_auto_mode_slider_light_intensity_range);
        char right_str[32];
        sprintf(right_str, "%d lx", right_val);
        lv_label_set_text(guider_ui.screen_auto_mode_label_light_intensity_right_value, right_str);


        custom_ui_handle_auto_mode_light_intensity_range_slider_value_changed_event(e);

        break;
    }
    default:
        break;
    }
}

void events_init_screen_auto_mode (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_auto_mode, screen_auto_mode_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_auto_mode_slider_temperature_range, screen_auto_mode_slider_temperature_range_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_auto_mode_slider_humidity_range, screen_auto_mode_slider_humidity_range_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_auto_mode_slider_soil_moisture_range, screen_auto_mode_slider_soil_moisture_range_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_auto_mode_slider_light_intensity_range, screen_auto_mode_slider_light_intensity_range_event_handler, LV_EVENT_ALL, ui);
}

static void screen_ai_pilot_mode_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_control, guider_ui.screen_control_del, &guider_ui.screen_ai_pilot_mode_del, setup_scr_screen_control, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 200, false, false);
            break;
        }
        case LV_DIR_TOP:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_home, guider_ui.screen_home_del, &guider_ui.screen_ai_pilot_mode_del, setup_scr_screen_home, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

void events_init_screen_ai_pilot_mode (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_ai_pilot_mode, screen_ai_pilot_mode_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}
