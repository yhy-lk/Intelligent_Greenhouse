/* custom.h */
#ifndef __CUSTOM_H_
#define __CUSTOM_H_
#ifdef __cplusplus
extern "C" {
#endif
#ifndef _WIN32

#include "gui_guider.h"

// 你之前的自定义函数声明...
void custom_init(lv_ui *ui);
void custom_update_screen_overview(void);

void custom_ui_handle_awning_sunshade_motor_switch_value_changed_event(lv_event_t * e);
void custom_ui_handle_water_pump_switch_value_changed_event(lv_event_t * e);
void custom_ui_handle_humidifier_switch_value_changed_event(lv_event_t * e);
void custom_ui_handle_ventilation_fan_speed_slider_value_changed_event(lv_event_t * e);

void custom_ui_send_manual_control_mode_command_on_screen_enter(void);
void custom_ui_send_auto_control_mode_command_on_screen_enter(void);
void custom_ui_handle_auto_mode_temperature_range_slider_value_changed_event(lv_event_t * e);
void custom_ui_handle_auto_mode_humidity_range_slider_value_changed_event(lv_event_t * e);
void custom_ui_handle_auto_mode_soil_moisture_range_slider_value_changed_event(lv_event_t * e);
void custom_ui_handle_auto_mode_light_intensity_range_slider_value_changed_event(lv_event_t * e);
void custom_ui_handle_light_brightness_slider_value_changed_event(lv_event_t * e);
void custom_ui_handle_light_color_slider_value_changed_event(lv_event_t * e);

#endif


#ifdef __cplusplus
}
#endif
#endif /* __CUSTOM_H_ */