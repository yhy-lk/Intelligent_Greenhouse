/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"


typedef struct
{
  
	lv_obj_t *screen_home;
	bool screen_home_del;
	lv_obj_t *screen_home_img_background;
	lv_obj_t *screen_home_img_control;
	lv_obj_t *screen_home_img_overview;
	lv_obj_t *screen_home_img_setting;
	lv_obj_t *screen_home_img_trend;
	lv_obj_t *screen_home_img_home;
	lv_obj_t *screen_home_label_overview;
	lv_obj_t *screen_home_label_setting;
	lv_obj_t *screen_home_label_trend;
	lv_obj_t *screen_home_label_control;
	lv_obj_t *screen_home_line_1;
	lv_obj_t *screen_home_datetext_data_time;
	lv_obj_t *screen_overview;
	bool screen_overview_del;
	lv_obj_t *screen_overview_img_background;
	lv_obj_t *screen_overview_btn_1;
	lv_obj_t *screen_overview_btn_1_label;
	lv_obj_t *screen_overview_img_1;
	lv_obj_t *screen_overview_btn_temperature;
	lv_obj_t *screen_overview_btn_temperature_label;
	lv_obj_t *screen_overview_btn_humidity_background;
	lv_obj_t *screen_overview_btn_humidity_background_label;
	lv_obj_t *screen_overview_btn_soil_station_background;
	lv_obj_t *screen_overview_btn_soil_station_background_label;
	lv_obj_t *screen_overview_btn_light_intensity_background;
	lv_obj_t *screen_overview_btn_light_intensity_background_label;
	lv_obj_t *screen_overview_btn_ai_recommendations_background;
	lv_obj_t *screen_overview_btn_ai_recommendations_background_label;
	lv_obj_t *screen_overview_arc_temperature_value;
	lv_obj_t *screen_overview_arc_humidity_value;
	lv_obj_t *screen_overview_label_temperature;
	lv_obj_t *screen_overview_label_temperature_value;
	lv_obj_t *screen_overview_label_humidity;
	lv_obj_t *screen_overview_label_humidity_value;
	lv_obj_t *screen_overview_bar_soil_station;
	lv_obj_t *screen_overview_label_soil_station;
	lv_obj_t *screen_overview_label_soil_station_value;
	lv_obj_t *screen_overview_img_light_intensity;
	lv_obj_t *screen_overview_label_light_intensity;
	lv_obj_t *screen_overview_label_light_intensity_value;
	lv_obj_t *screen_overview_label_ai_recommendations;
	lv_obj_t *screen_overview_label_ai_contents;
	lv_obj_t *screen_control;
	bool screen_control_del;
	lv_obj_t *screen_control_btn_background;
	lv_obj_t *screen_control_btn_background_label;
	lv_obj_t *screen_control_img_background;
	lv_obj_t *screen_control_label_control_mode;
	lv_obj_t *screen_control_btn_manual_mode;
	lv_obj_t *screen_control_btn_manual_mode_label;
	lv_obj_t *screen_control_btn_auto_mode;
	lv_obj_t *screen_control_btn_auto_mode_label;
	lv_obj_t *screen_control_btn_ai_pilot_mode;
	lv_obj_t *screen_control_btn_ai_pilot_mode_label;
	lv_obj_t *screen_control_label_control_mode_contents;
	lv_obj_t *screen_control_img_manual_mode;
	lv_obj_t *screen_control_label_manual_mode;
	lv_obj_t *screen_control_label_manual_mode_contents;
	lv_obj_t *screen_control_img_auto_mode;
	lv_obj_t *screen_control_label_auto_mode;
	lv_obj_t *screen_control_label_auto_mode_contents;
	lv_obj_t *screen_control_img_ai_pilot_mode;
	lv_obj_t *screen_control_label_ai_pilot_mode;
	lv_obj_t *screen_control_label_ai_pilot_mode_contents;
	lv_obj_t *screen_trend;
	bool screen_trend_del;
	lv_obj_t *screen_trend_img_1;
	lv_obj_t *screen_setting;
	bool screen_setting_del;
	lv_obj_t *screen_setting_img_1;
	lv_obj_t *screen_manual_mode;
	bool screen_manual_mode_del;
	lv_obj_t *screen_manual_mode_img_background;
	lv_obj_t *screen_manual_mode_btn_manual_control;
	lv_obj_t *screen_manual_mode_btn_manual_control_label;
	lv_obj_t *screen_manual_mode_img_1;
	lv_obj_t *screen_manual_mode_btn_grow_light_control;
	lv_obj_t *screen_manual_mode_btn_grow_light_control_label;
	lv_obj_t *screen_manual_mode_btn_ventilation_fan_control;
	lv_obj_t *screen_manual_mode_btn_ventilation_fan_control_label;
	lv_obj_t *screen_manual_mode_btn_quich_switches_group;
	lv_obj_t *screen_manual_mode_btn_quich_switches_group_label;
	lv_obj_t *screen_manual_mode_label_manual_mode;
	lv_obj_t *screen_manual_mode_img_awning;
	lv_obj_t *screen_manual_mode_img_humidifier;
	lv_obj_t *screen_manual_mode_img_water_pump;
	lv_obj_t *screen_manual_mode_label_quick_switches_groups;
	lv_obj_t *screen_manual_mode_label_awning;
	lv_obj_t *screen_manual_mode_sw_awning;
	lv_obj_t *screen_manual_mode_line_1;
	lv_obj_t *screen_manual_mode_line_2;
	lv_obj_t *screen_manual_mode_sw_water_pump;
	lv_obj_t *screen_manual_mode_label_water_pump;
	lv_obj_t *screen_manual_mode_label_humidifier;
	lv_obj_t *screen_manual_mode_sw_humidifier;
	lv_obj_t *screen_manual_mode_img_ventilation_fan;
	lv_obj_t *screen_manual_mode_label_ventilation_fan_control;
	lv_obj_t *screen_manual_mode_label_ventilation_fan_speed;
	lv_obj_t *screen_manual_mode_label_ventilation_fan_speed_value;
	lv_obj_t *screen_manual_mode_slider_ventilation_fan_speed;
	lv_obj_t *screen_manual_mode_img_grow_light;
	lv_obj_t *screen_manual_mode_label_grow_light_control;
	lv_obj_t *screen_manual_mode_label_brightness;
	lv_obj_t *screen_manual_mode_slider_brightness_value;
	lv_obj_t *screen_manual_mode_label_brightness_value;
	lv_obj_t *screen_manual_mode_label_color_adjustment;
	lv_obj_t *screen_manual_mode_slider_color_adjustment;
	lv_obj_t *screen_auto_mode;
	bool screen_auto_mode_del;
	lv_obj_t *screen_auto_mode_img_6;
	lv_obj_t *screen_auto_mode_btn_background;
	lv_obj_t *screen_auto_mode_btn_background_label;
	lv_obj_t *screen_auto_mode_img_background;
	lv_obj_t *screen_auto_mode_btn_temperature;
	lv_obj_t *screen_auto_mode_btn_temperature_label;
	lv_obj_t *screen_auto_mode_label_auto_mode;
	lv_obj_t *screen_auto_mode_btn_humidity;
	lv_obj_t *screen_auto_mode_btn_humidity_label;
	lv_obj_t *screen_auto_mode_btn_soil_moisture;
	lv_obj_t *screen_auto_mode_btn_soil_moisture_label;
	lv_obj_t *screen_auto_mode_btn_light_intensity;
	lv_obj_t *screen_auto_mode_btn_light_intensity_label;
	lv_obj_t *screen_auto_mode_btn_light_color;
	lv_obj_t *screen_auto_mode_btn_light_color_label;
	lv_obj_t *screen_auto_mode_img_temperature;
	lv_obj_t *screen_auto_mode_img_humidity;
	lv_obj_t *screen_auto_mode_label_temperature_range;
	lv_obj_t *screen_auto_mode_slider_temperature_range;
	lv_obj_t *screen_auto_mode_label_temperature_left_value;
	lv_obj_t *screen_auto_mode_label_humidity_range;
	lv_obj_t *screen_auto_mode_slider_humidity_range;
	lv_obj_t *screen_auto_mode_label_humidity_left_value;
	lv_obj_t *screen_auto_mode_label_temperature_right_value;
	lv_obj_t *screen_auto_mode_label_humidity_right_value;
	lv_obj_t *screen_auto_mode_label_soil_moisture_right_value;
	lv_obj_t *screen_auto_mode_label_soil_moisture_range;
	lv_obj_t *screen_auto_mode_slider_soil_moisture_range;
	lv_obj_t *screen_auto_mode_label_soil_moisture_left_value;
	lv_obj_t *screen_auto_mode_img_soil_moisture;
	lv_obj_t *screen_auto_mode_img_light_intensity;
	lv_obj_t *screen_auto_mode_label_light_intensity_range;
	lv_obj_t *screen_auto_mode_slider_light_intensity_range;
	lv_obj_t *screen_auto_mode_label_light_intensity_left_value;
	lv_obj_t *screen_auto_mode_label_light_intensity_right_value;
	lv_obj_t *screen_auto_mode_img_light_color;
	lv_obj_t *screen_auto_mode_label_light_color_value;
	lv_obj_t *screen_ai_pilot_mode;
	bool screen_ai_pilot_mode_del;
	lv_obj_t *screen_ai_pilot_mode_img_1;
	lv_obj_t *screen_ai_pilot_mode_label_1;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_screen_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, uint32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                  uint32_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                  lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_completed_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_bottom_layer(void);

void setup_ui(lv_ui *ui);

void video_play(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_screen_home(lv_ui *ui);
void setup_scr_screen_overview(lv_ui *ui);
void setup_scr_screen_control(lv_ui *ui);
void setup_scr_screen_trend(lv_ui *ui);
void setup_scr_screen_setting(lv_ui *ui);
void setup_scr_screen_manual_mode(lv_ui *ui);
void setup_scr_screen_auto_mode(lv_ui *ui);
void setup_scr_screen_ai_pilot_mode(lv_ui *ui);
LV_IMAGE_DECLARE(_BackGround_RGB565A8_240x320);
LV_IMAGE_DECLARE(_Control_RGB565A8_60x60);
LV_IMAGE_DECLARE(_Overview_RGB565A8_60x60);
LV_IMAGE_DECLARE(_Settings_RGB565A8_60x60);
LV_IMAGE_DECLARE(_Data_Trend_RGB565A8_60x60);
LV_IMAGE_DECLARE(_Home_RGB565A8_30x30);
LV_IMAGE_DECLARE(_Dashboard_Background_1_RGB565A8_240x320);
LV_IMAGE_DECLARE(_Control_Mode_Background_RGB565A8_240x320);
LV_IMAGE_DECLARE(_Light_Intensity_RGB565A8_30x30);
LV_IMAGE_DECLARE(_Manual_Mode_RGB565A8_50x45);
LV_IMAGE_DECLARE(_Auto_Mode_RGB565A8_50x45);
LV_IMAGE_DECLARE(_Ai_Pilot_RGB565A8_50x45);
LV_IMAGE_DECLARE(_Dashboard_Background_2_RGB565A8_240x320);
LV_IMAGE_DECLARE(_Auto_Mode_Background_RGB565A8_240x320);
LV_IMAGE_DECLARE(_Awning_RGB565A8_15x15);
LV_IMAGE_DECLARE(_Humidifier_RGB565A8_15x15);
LV_IMAGE_DECLARE(_Water_pump_RGB565A8_15x15);
LV_IMAGE_DECLARE(_Ventilation_Fan_RGB565A8_15x15);
LV_IMAGE_DECLARE(_Fill_Light_RGB565A8_15x15);

LV_IMAGE_DECLARE(_Rainbow_Slider_RGB565A8_200x3);
LV_IMAGE_DECLARE(_Temperature_Auto_Mode_RGB565A8_15x15);
LV_IMAGE_DECLARE(_Humidity_Auto_Mode_RGB565A8_15x15);
LV_IMAGE_DECLARE(_Soil_Moisture_Auto_Mode_RGB565A8_15x15);
LV_IMAGE_DECLARE(_Ligth_Intensity_Auto_Mode_RGB565A8_15x15);
LV_IMAGE_DECLARE(_Light_Color_Auto_Mode_RGB565A8_15x15);

LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_Alatsi_Regular_22)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_montserratMedium_15)
LV_FONT_DECLARE(lv_font_montserratMedium_10)
LV_FONT_DECLARE(lv_font_montserratMedium_8)
LV_FONT_DECLARE(lv_font_montserratMedium_25)
LV_FONT_DECLARE(lv_font_montserratMedium_18)


#ifdef __cplusplus
}
#endif
#endif
