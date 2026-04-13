# Copyright 2026 NXP
# NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
# accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
# activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
# comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
# terms, then you may not retain, install, activate or otherwise use the software.

import utime as time
import usys as sys
import lvgl as lv
import ustruct
import fs_driver

lv.init()

# Register display driver.
disp_drv = lv.sdl_window_create(240, 320)
lv.sdl_window_set_resizeable(disp_drv, False)
lv.sdl_window_set_title(disp_drv, "Simulator (MicroPython)")

# Regsiter input driver
mouse = lv.sdl_mouse_create()

# Add default theme for bottom layer
bottom_layer = lv.layer_bottom()
lv.theme_apply(bottom_layer)

fs_drv = lv.fs_drv_t()
fs_driver.fs_register(fs_drv, 'Z')

def anim_x_cb(obj, v):
    obj.set_x(v)

def anim_y_cb(obj, v):
    obj.set_y(v)

def anim_width_cb(obj, v):
    obj.set_width(v)

def anim_height_cb(obj, v):
    obj.set_height(v)

def anim_img_zoom_cb(obj, v):
    obj.set_scale(v)

def anim_img_rotate_cb(obj, v):
    obj.set_rotation(v)

global_font_cache = {}
def test_font(font_family, font_size):
    global global_font_cache
    if font_family + str(font_size) in global_font_cache:
        return global_font_cache[font_family + str(font_size)]
    if font_size % 2:
        candidates = [
            (font_family, font_size),
            (font_family, font_size-font_size%2),
            (font_family, font_size+font_size%2),
            ("montserrat", font_size-font_size%2),
            ("montserrat", font_size+font_size%2),
            ("montserrat", 16)
        ]
    else:
        candidates = [
            (font_family, font_size),
            ("montserrat", font_size),
            ("montserrat", 16)
        ]
    for (family, size) in candidates:
        try:
            if eval(f'lv.font_{family}_{size}'):
                global_font_cache[font_family + str(font_size)] = eval(f'lv.font_{family}_{size}')
                if family != font_family or size != font_size:
                    print(f'WARNING: lv.font_{family}_{size} is used!')
                return eval(f'lv.font_{family}_{size}')
        except AttributeError:
            try:
                load_font = lv.binfont_create(f"Z:MicroPython/lv_font_{family}_{size}.fnt")
                global_font_cache[font_family + str(font_size)] = load_font
                return load_font
            except:
                if family == font_family and size == font_size:
                    print(f'WARNING: lv.font_{family}_{size} is NOT supported!')

global_image_cache = {}
def load_image(file):
    global global_image_cache
    if file in global_image_cache:
        return global_image_cache[file]
    try:
        with open(file,'rb') as f:
            data = f.read()
    except:
        print(f'Could not open {file}')
        sys.exit()

    img = lv.image_dsc_t({
        'data_size': len(data),
        'data': data
    })
    global_image_cache[file] = img
    return img

def calendar_event_handler(e,obj):
    code = e.get_code()

    if code == lv.EVENT.VALUE_CHANGED:
        source = lv.calendar.__cast__(e.get_current_target())
        date = lv.calendar_date_t()
        if source.get_pressed_date(date) == lv.RESULT.OK:
            source.set_highlighted_dates([date], 1)

def spinbox_increment_event_cb(e, obj):
    code = e.get_code()
    if code == lv.EVENT.SHORT_CLICKED or code == lv.EVENT.LONG_PRESSED_REPEAT:
        obj.increment()
def spinbox_decrement_event_cb(e, obj):
    code = e.get_code()
    if code == lv.EVENT.SHORT_CLICKED or code == lv.EVENT.LONG_PRESSED_REPEAT:
        obj.decrement()

def digital_clock_cb(timer, obj, current_time, show_second, use_ampm):
    hour = int(current_time[0])
    minute = int(current_time[1])
    second = int(current_time[2])
    ampm = current_time[3]
    second = second + 1
    if second == 60:
        second = 0
        minute = minute + 1
        if minute == 60:
            minute = 0
            hour = hour + 1
            if use_ampm:
                if hour == 12:
                    if ampm == 'AM':
                        ampm = 'PM'
                    elif ampm == 'PM':
                        ampm = 'AM'
                if hour > 12:
                    hour = hour % 12
    hour = hour % 24
    if use_ampm:
        if show_second:
            obj.set_text("%d:%02d:%02d %s" %(hour, minute, second, ampm))
        else:
            obj.set_text("%d:%02d %s" %(hour, minute, ampm))
    else:
        if show_second:
            obj.set_text("%d:%02d:%02d" %(hour, minute, second))
        else:
            obj.set_text("%d:%02d" %(hour, minute))
    current_time[0] = hour
    current_time[1] = minute
    current_time[2] = second
    current_time[3] = ampm

def analog_clock_cb(timer, obj):
    datetime = time.localtime()
    hour = datetime[3]
    if hour >= 12: hour = hour - 12
    obj.set_time(hour, datetime[4], datetime[5])

def datetext_event_handler(e, obj):
    code = e.get_code()
    datetext = lv.label.__cast__(e.get_target())
    if code == lv.EVENT.FOCUSED:
        if obj is None:
            bg = lv.layer_top()
            bg.add_flag(lv.obj.FLAG.CLICKABLE)
            obj = lv.calendar(bg)
            scr = lv.screen_active()
            scr_height = scr.get_height()
            scr_width = scr.get_width()
            obj.set_size(int(scr_width * 0.8), int(scr_height * 0.8))
            datestring = datetext.get_text()
            year = int(datestring.split('/')[0])
            month = int(datestring.split('/')[1])
            day = int(datestring.split('/')[2])
            obj.set_showed_date(year, month)
            highlighted_days=[lv.calendar_date_t({'year':year, 'month':month, 'day':day})]
            obj.set_highlighted_dates(highlighted_days, 1)
            obj.align(lv.ALIGN.CENTER, 0, 0)
            lv.calendar_header_arrow(obj)
            obj.add_event_cb(lambda e: datetext_calendar_event_handler(e, datetext), lv.EVENT.ALL, None)
            scr.update_layout()

def datetext_calendar_event_handler(e, obj):
    code = e.get_code()
    calendar = lv.calendar.__cast__(e.get_current_target())
    if code == lv.EVENT.VALUE_CHANGED:
        date = lv.calendar_date_t()
        if calendar.get_pressed_date(date) == lv.RESULT.OK:
            obj.set_text(f"{date.year}/{date.month}/{date.day}")
            bg = lv.layer_top()
            bg.remove_flag(lv.obj.FLAG.CLICKABLE)
            bg.set_style_bg_opa(lv.OPA.TRANSP, 0)
            calendar.delete()

# Create screen_home
screen_home = lv.obj()
screen_home.set_size(240, 320)
screen_home.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_home.remove_flag(lv.obj.FLAG.SCROLLABLE)
# Set style for screen_home, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_home.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_home_img_background
screen_home_img_background = lv.image(screen_home)
screen_home_img_background.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\BackGround_240_320.png"))
screen_home_img_background.add_flag(lv.obj.FLAG.CLICKABLE)
screen_home_img_background.set_pivot(50,50)
screen_home_img_background.set_rotation(0)
screen_home_img_background.set_pos(0, 0)
screen_home_img_background.set_size(240, 320)
# Set style for screen_home_img_background, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_home_img_background.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_home_img_control
screen_home_img_control = lv.image(screen_home)
screen_home_img_control.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Control_60_60.png"))
screen_home_img_control.add_flag(lv.obj.FLAG.CLICKABLE)
screen_home_img_control.set_pivot(50,50)
screen_home_img_control.set_rotation(0)
screen_home_img_control.set_pos(140, 100)
screen_home_img_control.set_size(60, 60)
# Set style for screen_home_img_control, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_home_img_control.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_home_img_overview
screen_home_img_overview = lv.image(screen_home)
screen_home_img_overview.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Overview_60_60.png"))
screen_home_img_overview.add_flag(lv.obj.FLAG.CLICKABLE)
screen_home_img_overview.set_pivot(50,50)
screen_home_img_overview.set_rotation(0)
screen_home_img_overview.set_pos(40, 100)
screen_home_img_overview.set_size(60, 60)
# Set style for screen_home_img_overview, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_home_img_overview.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_home_img_setting
screen_home_img_setting = lv.image(screen_home)
screen_home_img_setting.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Settings_60_60.png"))
screen_home_img_setting.add_flag(lv.obj.FLAG.CLICKABLE)
screen_home_img_setting.set_pivot(50,50)
screen_home_img_setting.set_rotation(0)
screen_home_img_setting.set_pos(140, 190)
screen_home_img_setting.set_size(60, 60)
# Set style for screen_home_img_setting, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_home_img_setting.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_home_img_trend
screen_home_img_trend = lv.image(screen_home)
screen_home_img_trend.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Data_Trend_60_60.png"))
screen_home_img_trend.add_flag(lv.obj.FLAG.CLICKABLE)
screen_home_img_trend.set_pivot(50,50)
screen_home_img_trend.set_rotation(0)
screen_home_img_trend.set_pos(40, 190)
screen_home_img_trend.set_size(60, 60)
# Set style for screen_home_img_trend, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_home_img_trend.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_home_img_home
screen_home_img_home = lv.image(screen_home)
screen_home_img_home.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Home_30_30.png"))
screen_home_img_home.add_flag(lv.obj.FLAG.CLICKABLE)
screen_home_img_home.set_pivot(50,50)
screen_home_img_home.set_rotation(0)
screen_home_img_home.set_pos(105, 290)
screen_home_img_home.set_size(30, 30)
# Set style for screen_home_img_home, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_home_img_home.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_home_label_overview
screen_home_label_overview = lv.label(screen_home)
screen_home_label_overview.set_text("overview")
screen_home_label_overview.set_long_mode(lv.label.LONG.WRAP)
screen_home_label_overview.set_width(lv.pct(100))
screen_home_label_overview.set_pos(40, 170)
screen_home_label_overview.set_size(60, 20)
# Set style for screen_home_label_overview, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_home_label_overview.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_overview.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_overview.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_overview.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_overview.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_overview.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_overview.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_overview.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_overview.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_overview.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_overview.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_overview.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_overview.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_overview.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_home_label_setting
screen_home_label_setting = lv.label(screen_home)
screen_home_label_setting.set_text("setting")
screen_home_label_setting.set_long_mode(lv.label.LONG.WRAP)
screen_home_label_setting.set_width(lv.pct(100))
screen_home_label_setting.set_pos(140, 260)
screen_home_label_setting.set_size(60, 20)
# Set style for screen_home_label_setting, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_home_label_setting.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_setting.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_setting.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_setting.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_setting.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_setting.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_setting.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_setting.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_setting.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_setting.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_setting.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_setting.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_setting.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_setting.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_home_label_trend
screen_home_label_trend = lv.label(screen_home)
screen_home_label_trend.set_text("trend")
screen_home_label_trend.set_long_mode(lv.label.LONG.WRAP)
screen_home_label_trend.set_width(lv.pct(100))
screen_home_label_trend.set_pos(40, 260)
screen_home_label_trend.set_size(60, 20)
# Set style for screen_home_label_trend, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_home_label_trend.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_trend.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_trend.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_trend.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_trend.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_trend.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_trend.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_trend.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_trend.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_trend.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_trend.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_trend.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_trend.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_trend.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_home_label_control
screen_home_label_control = lv.label(screen_home)
screen_home_label_control.set_text("control")
screen_home_label_control.set_long_mode(lv.label.LONG.WRAP)
screen_home_label_control.set_width(lv.pct(100))
screen_home_label_control.set_pos(140, 170)
screen_home_label_control.set_size(60, 20)
# Set style for screen_home_label_control, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_home_label_control.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_control.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_control.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_control.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_control.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_control.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_control.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_control.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_control.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_control.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_control.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_control.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_control.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_label_control.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_home_line_1
screen_home_line_1 = lv.line(screen_home)
screen_home_line_1_line_points = [{"x":0, "y":0},{"x":240, "y":0},]
screen_home_line_1.set_points(screen_home_line_1_line_points, 2)
screen_home_line_1.set_pos(0, 285)
screen_home_line_1.set_size(247, 4)
# Set style for screen_home_line_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_home_line_1.set_style_line_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_line_1.set_style_line_color(lv.color_hex(0x757575), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_line_1.set_style_line_opa(100, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_line_1.set_style_line_rounded(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_home_datetext_data_time
screen_home_datetext_data_time = lv.label(screen_home)
screen_home_datetext_data_time.set_text("2026/01/27")
screen_home_datetext_data_time.set_style_text_align(lv.TEXT_ALIGN.CENTER, 0)
screen_home_datetext_data_time.add_flag(lv.obj.FLAG.CLICKABLE)
screen_home_datetext_data_time_calendar = None
screen_home_datetext_data_time.add_event_cb(lambda e: datetext_event_handler(e, screen_home_datetext_data_time_calendar), lv.EVENT.ALL, None)
screen_home_datetext_data_time.set_pos(40, 40)
screen_home_datetext_data_time.set_size(160, 40)
# Set style for screen_home_datetext_data_time, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_home_datetext_data_time.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_datetext_data_time.set_style_text_font(test_font("Alatsi_Regular", 22), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_datetext_data_time.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_datetext_data_time.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_datetext_data_time.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_datetext_data_time.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_datetext_data_time.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_datetext_data_time.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_datetext_data_time.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_datetext_data_time.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_datetext_data_time.set_style_pad_top(7, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_datetext_data_time.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_datetext_data_time.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_datetext_data_time.set_style_shadow_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_datetext_data_time.set_style_shadow_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_datetext_data_time.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_datetext_data_time.set_style_shadow_spread(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_datetext_data_time.set_style_shadow_offset_x(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_home_datetext_data_time.set_style_shadow_offset_y(0, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_home.update_layout()
# Create screen_overview
screen_overview = lv.obj()
screen_overview.set_size(240, 320)
screen_overview.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_overview.remove_flag(lv.obj.FLAG.SCROLLABLE)
# Set style for screen_overview, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overview_img_background
screen_overview_img_background = lv.image(screen_overview)
screen_overview_img_background.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Dashboard_Background_1_240_320.png"))
screen_overview_img_background.add_flag(lv.obj.FLAG.CLICKABLE)
screen_overview_img_background.set_pivot(50,50)
screen_overview_img_background.set_rotation(0)
screen_overview_img_background.set_pos(0, 0)
screen_overview_img_background.set_size(240, 320)
# Set style for screen_overview_img_background, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_img_background.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overview_btn_1
screen_overview_btn_1 = lv.button(screen_overview)
screen_overview_btn_1_label = lv.label(screen_overview_btn_1)
screen_overview_btn_1_label.set_text("")
screen_overview_btn_1_label.set_long_mode(lv.label.LONG.WRAP)
screen_overview_btn_1_label.set_width(lv.pct(100))
screen_overview_btn_1_label.align(lv.ALIGN.CENTER, 0, 0)
screen_overview_btn_1.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_overview_btn_1.set_pos(0, 0)
screen_overview_btn_1.set_size(240, 320)
screen_overview_btn_1.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_overview_btn_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_btn_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_1.set_style_bg_color(lv.color_hex(0xe6e6e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_1.set_style_radius(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_1.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overview_img_1
screen_overview_img_1 = lv.image(screen_overview)
screen_overview_img_1.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Control_Mode_Background_240_320.png"))
screen_overview_img_1.add_flag(lv.obj.FLAG.CLICKABLE)
screen_overview_img_1.set_pivot(50,50)
screen_overview_img_1.set_rotation(0)
screen_overview_img_1.set_pos(0, 0)
screen_overview_img_1.set_size(240, 320)
# Set style for screen_overview_img_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_img_1.set_style_image_opa(181, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overview_btn_temperature
screen_overview_btn_temperature = lv.button(screen_overview)
screen_overview_btn_temperature_label = lv.label(screen_overview_btn_temperature)
screen_overview_btn_temperature_label.set_text("")
screen_overview_btn_temperature_label.set_long_mode(lv.label.LONG.WRAP)
screen_overview_btn_temperature_label.set_width(lv.pct(100))
screen_overview_btn_temperature_label.align(lv.ALIGN.CENTER, 0, 0)
screen_overview_btn_temperature.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_overview_btn_temperature.set_pos(13, 20)
screen_overview_btn_temperature.set_size(100, 80)
screen_overview_btn_temperature.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_overview_btn_temperature, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_btn_temperature.set_style_bg_opa(200, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_temperature.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_temperature.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_temperature.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_temperature.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_temperature.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_temperature.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_temperature.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_temperature.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_temperature.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overview_btn_humidity_background
screen_overview_btn_humidity_background = lv.button(screen_overview)
screen_overview_btn_humidity_background_label = lv.label(screen_overview_btn_humidity_background)
screen_overview_btn_humidity_background_label.set_text("")
screen_overview_btn_humidity_background_label.set_long_mode(lv.label.LONG.WRAP)
screen_overview_btn_humidity_background_label.set_width(lv.pct(100))
screen_overview_btn_humidity_background_label.align(lv.ALIGN.CENTER, 0, 0)
screen_overview_btn_humidity_background.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_overview_btn_humidity_background.set_pos(126, 20)
screen_overview_btn_humidity_background.set_size(100, 80)
screen_overview_btn_humidity_background.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_overview_btn_humidity_background, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_btn_humidity_background.set_style_bg_opa(200, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_humidity_background.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_humidity_background.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_humidity_background.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_humidity_background.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_humidity_background.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_humidity_background.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_humidity_background.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_humidity_background.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_humidity_background.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overview_btn_soil_station_background
screen_overview_btn_soil_station_background = lv.button(screen_overview)
screen_overview_btn_soil_station_background_label = lv.label(screen_overview_btn_soil_station_background)
screen_overview_btn_soil_station_background_label.set_text("")
screen_overview_btn_soil_station_background_label.set_long_mode(lv.label.LONG.WRAP)
screen_overview_btn_soil_station_background_label.set_width(lv.pct(100))
screen_overview_btn_soil_station_background_label.align(lv.ALIGN.CENTER, 0, 0)
screen_overview_btn_soil_station_background.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_overview_btn_soil_station_background.set_pos(13, 120)
screen_overview_btn_soil_station_background.set_size(213, 80)
screen_overview_btn_soil_station_background.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_overview_btn_soil_station_background, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_btn_soil_station_background.set_style_bg_opa(200, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_soil_station_background.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_soil_station_background.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_soil_station_background.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_soil_station_background.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_soil_station_background.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_soil_station_background.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_soil_station_background.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_soil_station_background.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_soil_station_background.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overview_btn_light_intensity_background
screen_overview_btn_light_intensity_background = lv.button(screen_overview)
screen_overview_btn_light_intensity_background_label = lv.label(screen_overview_btn_light_intensity_background)
screen_overview_btn_light_intensity_background_label.set_text("")
screen_overview_btn_light_intensity_background_label.set_long_mode(lv.label.LONG.WRAP)
screen_overview_btn_light_intensity_background_label.set_width(lv.pct(100))
screen_overview_btn_light_intensity_background_label.align(lv.ALIGN.CENTER, 0, 0)
screen_overview_btn_light_intensity_background.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_overview_btn_light_intensity_background.set_pos(13, 217)
screen_overview_btn_light_intensity_background.set_size(100, 80)
screen_overview_btn_light_intensity_background.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_overview_btn_light_intensity_background, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_btn_light_intensity_background.set_style_bg_opa(200, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_light_intensity_background.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_light_intensity_background.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_light_intensity_background.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_light_intensity_background.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_light_intensity_background.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_light_intensity_background.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_light_intensity_background.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_light_intensity_background.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_light_intensity_background.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overview_btn_ai_recommendations_background
screen_overview_btn_ai_recommendations_background = lv.button(screen_overview)
screen_overview_btn_ai_recommendations_background_label = lv.label(screen_overview_btn_ai_recommendations_background)
screen_overview_btn_ai_recommendations_background_label.set_text("")
screen_overview_btn_ai_recommendations_background_label.set_long_mode(lv.label.LONG.WRAP)
screen_overview_btn_ai_recommendations_background_label.set_width(lv.pct(100))
screen_overview_btn_ai_recommendations_background_label.align(lv.ALIGN.CENTER, 0, 0)
screen_overview_btn_ai_recommendations_background.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_overview_btn_ai_recommendations_background.set_pos(126, 217)
screen_overview_btn_ai_recommendations_background.set_size(100, 80)
screen_overview_btn_ai_recommendations_background.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_overview_btn_ai_recommendations_background, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_btn_ai_recommendations_background.set_style_bg_opa(200, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_ai_recommendations_background.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_ai_recommendations_background.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_ai_recommendations_background.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_ai_recommendations_background.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_ai_recommendations_background.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_ai_recommendations_background.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_ai_recommendations_background.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_ai_recommendations_background.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_btn_ai_recommendations_background.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overview_arc_temperature_value
screen_overview_arc_temperature_value = lv.arc(screen_overview)
screen_overview_arc_temperature_value.set_mode(lv.arc.MODE.NORMAL)
screen_overview_arc_temperature_value.set_range(0, 100)
screen_overview_arc_temperature_value.set_bg_angles(135, 45)
screen_overview_arc_temperature_value.set_value(70)
screen_overview_arc_temperature_value.set_rotation(0)
screen_overview_arc_temperature_value.set_pos(24, 26)
screen_overview_arc_temperature_value.set_size(80, 80)
screen_overview_arc_temperature_value.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_overview_arc_temperature_value, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_arc_temperature_value.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_temperature_value.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_temperature_value.set_style_arc_width(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_temperature_value.set_style_arc_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_temperature_value.set_style_arc_color(lv.color_hex(0xff0000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_temperature_value.set_style_radius(8, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_temperature_value.set_style_pad_top(8, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_temperature_value.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_temperature_value.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_temperature_value.set_style_pad_right(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_temperature_value.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_overview_arc_temperature_value, Part: lv.PART.INDICATOR, State: lv.STATE.DEFAULT.
screen_overview_arc_temperature_value.set_style_arc_width(5, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_overview_arc_temperature_value.set_style_arc_opa(255, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_overview_arc_temperature_value.set_style_arc_color(lv.color_hex(0x960000), lv.PART.INDICATOR|lv.STATE.DEFAULT)

# Set style for screen_overview_arc_temperature_value, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_overview_arc_temperature_value.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_overview_arc_temperature_value.set_style_bg_color(lv.color_hex(0x960000), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_overview_arc_temperature_value.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_overview_arc_temperature_value.set_style_pad_all(2, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_overview_arc_humidity_value
screen_overview_arc_humidity_value = lv.arc(screen_overview)
screen_overview_arc_humidity_value.set_mode(lv.arc.MODE.NORMAL)
screen_overview_arc_humidity_value.set_range(0, 100)
screen_overview_arc_humidity_value.set_bg_angles(135, 45)
screen_overview_arc_humidity_value.set_value(70)
screen_overview_arc_humidity_value.set_rotation(0)
screen_overview_arc_humidity_value.set_pos(136, 26)
screen_overview_arc_humidity_value.set_size(80, 80)
screen_overview_arc_humidity_value.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_overview_arc_humidity_value, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_arc_humidity_value.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_humidity_value.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_humidity_value.set_style_arc_width(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_humidity_value.set_style_arc_opa(200, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_humidity_value.set_style_arc_color(lv.color_hex(0x0000ff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_humidity_value.set_style_radius(8, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_humidity_value.set_style_pad_top(8, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_humidity_value.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_humidity_value.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_humidity_value.set_style_pad_right(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_arc_humidity_value.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_overview_arc_humidity_value, Part: lv.PART.INDICATOR, State: lv.STATE.DEFAULT.
screen_overview_arc_humidity_value.set_style_arc_width(5, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_overview_arc_humidity_value.set_style_arc_opa(255, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_overview_arc_humidity_value.set_style_arc_color(lv.color_hex(0x000096), lv.PART.INDICATOR|lv.STATE.DEFAULT)

# Set style for screen_overview_arc_humidity_value, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_overview_arc_humidity_value.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_overview_arc_humidity_value.set_style_bg_color(lv.color_hex(0x000096), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_overview_arc_humidity_value.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_overview_arc_humidity_value.set_style_pad_all(2, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_overview_label_temperature
screen_overview_label_temperature = lv.label(screen_overview)
screen_overview_label_temperature.set_text("temp")
screen_overview_label_temperature.set_long_mode(lv.label.LONG.WRAP)
screen_overview_label_temperature.set_width(lv.pct(100))
screen_overview_label_temperature.set_pos(21, 73)
screen_overview_label_temperature.set_size(80, 20)
# Set style for screen_overview_label_temperature, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_label_temperature.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overview_label_temperature_value
screen_overview_label_temperature_value = lv.label(screen_overview)
screen_overview_label_temperature_value.set_text("25°C")
screen_overview_label_temperature_value.set_long_mode(lv.label.LONG.WRAP)
screen_overview_label_temperature_value.set_width(lv.pct(100))
screen_overview_label_temperature_value.set_pos(21, 50)
screen_overview_label_temperature_value.set_size(80, 20)
# Set style for screen_overview_label_temperature_value, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_label_temperature_value.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature_value.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature_value.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature_value.set_style_text_font(test_font("montserratMedium", 15), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature_value.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature_value.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature_value.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature_value.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature_value.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature_value.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature_value.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature_value.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature_value.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_temperature_value.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overview_label_humidity
screen_overview_label_humidity = lv.label(screen_overview)
screen_overview_label_humidity.set_text("humi")
screen_overview_label_humidity.set_long_mode(lv.label.LONG.WRAP)
screen_overview_label_humidity.set_width(lv.pct(100))
screen_overview_label_humidity.set_pos(130, 73)
screen_overview_label_humidity.set_size(80, 20)
# Set style for screen_overview_label_humidity, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_label_humidity.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overview_label_humidity_value
screen_overview_label_humidity_value = lv.label(screen_overview)
screen_overview_label_humidity_value.set_text("60%")
screen_overview_label_humidity_value.set_long_mode(lv.label.LONG.WRAP)
screen_overview_label_humidity_value.set_width(lv.pct(100))
screen_overview_label_humidity_value.set_pos(133, 50)
screen_overview_label_humidity_value.set_size(80, 20)
# Set style for screen_overview_label_humidity_value, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_label_humidity_value.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity_value.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity_value.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity_value.set_style_text_font(test_font("montserratMedium", 15), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity_value.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity_value.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity_value.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity_value.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity_value.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity_value.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity_value.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity_value.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity_value.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_humidity_value.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overview_bar_soil_station
screen_overview_bar_soil_station = lv.bar(screen_overview)
screen_overview_bar_soil_station.set_style_anim_duration(1000, 0)
screen_overview_bar_soil_station.set_mode(lv.bar.MODE.NORMAL)
screen_overview_bar_soil_station.set_range(0, 100)
screen_overview_bar_soil_station.set_value(50, lv.ANIM.OFF)
screen_overview_bar_soil_station.set_pos(50, 175)
screen_overview_bar_soil_station.set_size(150, 10)
# Set style for screen_overview_bar_soil_station, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_bar_soil_station.set_style_bg_opa(200, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_bar_soil_station.set_style_bg_color(lv.color_hex(0x00ff00), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_bar_soil_station.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_bar_soil_station.set_style_radius(20, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_bar_soil_station.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_overview_bar_soil_station, Part: lv.PART.INDICATOR, State: lv.STATE.DEFAULT.
screen_overview_bar_soil_station.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_overview_bar_soil_station.set_style_bg_color(lv.color_hex(0x009600), lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_overview_bar_soil_station.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_overview_bar_soil_station.set_style_radius(10, lv.PART.INDICATOR|lv.STATE.DEFAULT)

# Create screen_overview_label_soil_station
screen_overview_label_soil_station = lv.label(screen_overview)
screen_overview_label_soil_station.set_text("soil station")
screen_overview_label_soil_station.set_long_mode(lv.label.LONG.WRAP)
screen_overview_label_soil_station.set_width(lv.pct(100))
screen_overview_label_soil_station.set_pos(70, 127)
screen_overview_label_soil_station.set_size(100, 20)
# Set style for screen_overview_label_soil_station, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_label_soil_station.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station.set_style_text_font(test_font("montserratMedium", 15), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overview_label_soil_station_value
screen_overview_label_soil_station_value = lv.label(screen_overview)
screen_overview_label_soil_station_value.set_text("suitable")
screen_overview_label_soil_station_value.set_long_mode(lv.label.LONG.WRAP)
screen_overview_label_soil_station_value.set_width(lv.pct(100))
screen_overview_label_soil_station_value.set_pos(67, 152)
screen_overview_label_soil_station_value.set_size(100, 20)
# Set style for screen_overview_label_soil_station_value, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_label_soil_station_value.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station_value.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station_value.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station_value.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station_value.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station_value.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station_value.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station_value.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station_value.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station_value.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station_value.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station_value.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station_value.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_soil_station_value.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overview_img_light_intensity
screen_overview_img_light_intensity = lv.image(screen_overview)
screen_overview_img_light_intensity.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Light_Intensity_30_30.png"))
screen_overview_img_light_intensity.add_flag(lv.obj.FLAG.CLICKABLE)
screen_overview_img_light_intensity.set_pivot(50,50)
screen_overview_img_light_intensity.set_rotation(0)
screen_overview_img_light_intensity.set_pos(45, 243)
screen_overview_img_light_intensity.set_size(30, 30)
# Set style for screen_overview_img_light_intensity, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_img_light_intensity.set_style_image_opa(217, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overview_label_light_intensity
screen_overview_label_light_intensity = lv.label(screen_overview)
screen_overview_label_light_intensity.set_text("light intensity")
screen_overview_label_light_intensity.set_long_mode(lv.label.LONG.WRAP)
screen_overview_label_light_intensity.set_width(lv.pct(100))
screen_overview_label_light_intensity.set_pos(24, 226)
screen_overview_label_light_intensity.set_size(79, 15)
# Set style for screen_overview_label_light_intensity, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_label_light_intensity.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overview_label_light_intensity_value
screen_overview_label_light_intensity_value = lv.label(screen_overview)
screen_overview_label_light_intensity_value.set_text("250 lumen")
screen_overview_label_light_intensity_value.set_long_mode(lv.label.LONG.WRAP)
screen_overview_label_light_intensity_value.set_width(lv.pct(100))
screen_overview_label_light_intensity_value.set_pos(24, 279)
screen_overview_label_light_intensity_value.set_size(80, 8)
# Set style for screen_overview_label_light_intensity_value, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_label_light_intensity_value.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity_value.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity_value.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity_value.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity_value.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity_value.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity_value.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity_value.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity_value.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity_value.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity_value.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity_value.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity_value.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_light_intensity_value.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overview_label_ai_recommendations
screen_overview_label_ai_recommendations = lv.label(screen_overview)
screen_overview_label_ai_recommendations.set_text("AI Recommendations")
screen_overview_label_ai_recommendations.set_long_mode(lv.label.LONG.WRAP)
screen_overview_label_ai_recommendations.set_width(lv.pct(100))
screen_overview_label_ai_recommendations.set_pos(136, 223)
screen_overview_label_ai_recommendations.set_size(80, 20)
# Set style for screen_overview_label_ai_recommendations, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_label_ai_recommendations.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_recommendations.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_recommendations.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_recommendations.set_style_text_font(test_font("montserratMedium", 8), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_recommendations.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_recommendations.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_recommendations.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_recommendations.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_recommendations.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_recommendations.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_recommendations.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_recommendations.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_recommendations.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_recommendations.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_overview_label_ai_contents
screen_overview_label_ai_contents = lv.label(screen_overview)
screen_overview_label_ai_contents.set_text("Don't want to go to work don't want to go to work don't want to go to work don't want to go to work don't want to go to work")
screen_overview_label_ai_contents.set_long_mode(lv.label.LONG.WRAP)
screen_overview_label_ai_contents.set_width(lv.pct(100))
screen_overview_label_ai_contents.set_pos(136, 251)
screen_overview_label_ai_contents.set_size(80, 20)
# Set style for screen_overview_label_ai_contents, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_overview_label_ai_contents.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_contents.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_contents.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_contents.set_style_text_font(test_font("montserratMedium", 8), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_contents.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_contents.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_contents.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_contents.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_contents.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_contents.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_contents.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_contents.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_contents.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_overview_label_ai_contents.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_overview.update_layout()
# Create screen_control
screen_control = lv.obj()
screen_control.set_size(240, 320)
screen_control.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_control.remove_flag(lv.obj.FLAG.SCROLLABLE)
# Set style for screen_control, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_control.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_control_btn_background
screen_control_btn_background = lv.button(screen_control)
screen_control_btn_background_label = lv.label(screen_control_btn_background)
screen_control_btn_background_label.set_text("")
screen_control_btn_background_label.set_long_mode(lv.label.LONG.WRAP)
screen_control_btn_background_label.set_width(lv.pct(100))
screen_control_btn_background_label.align(lv.ALIGN.CENTER, 0, 0)
screen_control_btn_background.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_control_btn_background.set_pos(0, 0)
screen_control_btn_background.set_size(240, 320)
screen_control_btn_background.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_control_btn_background, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_control_btn_background.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_background.set_style_bg_color(lv.color_hex(0xe6e6e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_background.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_background.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_background.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_background.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_background.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_background.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_background.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_background.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_control_img_background
screen_control_img_background = lv.image(screen_control)
screen_control_img_background.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Control_Mode_Background_240_320.png"))
screen_control_img_background.add_flag(lv.obj.FLAG.CLICKABLE)
screen_control_img_background.set_pivot(50,50)
screen_control_img_background.set_rotation(0)
screen_control_img_background.set_pos(0, 0)
screen_control_img_background.set_size(240, 320)
screen_control_img_background.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_control_img_background, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_control_img_background.set_style_image_opa(181, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_control_label_control_mode
screen_control_label_control_mode = lv.label(screen_control)
screen_control_label_control_mode.set_text("Control Mode")
screen_control_label_control_mode.set_long_mode(lv.label.LONG.WRAP)
screen_control_label_control_mode.set_width(lv.pct(100))
screen_control_label_control_mode.set_pos(20, 20)
screen_control_label_control_mode.set_size(200, 32)
# Set style for screen_control_label_control_mode, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_control_label_control_mode.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode.set_style_text_font(test_font("montserratMedium", 25), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_control_btn_manual_mode
screen_control_btn_manual_mode = lv.button(screen_control)
screen_control_btn_manual_mode_label = lv.label(screen_control_btn_manual_mode)
screen_control_btn_manual_mode_label.set_text("")
screen_control_btn_manual_mode_label.set_long_mode(lv.label.LONG.WRAP)
screen_control_btn_manual_mode_label.set_width(lv.pct(100))
screen_control_btn_manual_mode_label.align(lv.ALIGN.CENTER, 0, 0)
screen_control_btn_manual_mode.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_control_btn_manual_mode.set_pos(10, 90)
screen_control_btn_manual_mode.set_size(220, 60)
# Set style for screen_control_btn_manual_mode, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_control_btn_manual_mode.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_manual_mode.set_style_bg_color(lv.color_hex(0xdcdcdc), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_manual_mode.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_manual_mode.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_manual_mode.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_manual_mode.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_manual_mode.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_manual_mode.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_manual_mode.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_manual_mode.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_control_btn_auto_mode
screen_control_btn_auto_mode = lv.button(screen_control)
screen_control_btn_auto_mode_label = lv.label(screen_control_btn_auto_mode)
screen_control_btn_auto_mode_label.set_text("")
screen_control_btn_auto_mode_label.set_long_mode(lv.label.LONG.WRAP)
screen_control_btn_auto_mode_label.set_width(lv.pct(100))
screen_control_btn_auto_mode_label.align(lv.ALIGN.CENTER, 0, 0)
screen_control_btn_auto_mode.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_control_btn_auto_mode.set_pos(10, 170)
screen_control_btn_auto_mode.set_size(220, 60)
# Set style for screen_control_btn_auto_mode, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_control_btn_auto_mode.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_auto_mode.set_style_bg_color(lv.color_hex(0xdcdcdc), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_auto_mode.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_auto_mode.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_auto_mode.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_auto_mode.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_auto_mode.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_auto_mode.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_auto_mode.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_auto_mode.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_control_btn_ai_pilot_mode
screen_control_btn_ai_pilot_mode = lv.button(screen_control)
screen_control_btn_ai_pilot_mode_label = lv.label(screen_control_btn_ai_pilot_mode)
screen_control_btn_ai_pilot_mode_label.set_text("")
screen_control_btn_ai_pilot_mode_label.set_long_mode(lv.label.LONG.WRAP)
screen_control_btn_ai_pilot_mode_label.set_width(lv.pct(100))
screen_control_btn_ai_pilot_mode_label.align(lv.ALIGN.CENTER, 0, 0)
screen_control_btn_ai_pilot_mode.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_control_btn_ai_pilot_mode.set_pos(10, 250)
screen_control_btn_ai_pilot_mode.set_size(220, 60)
# Set style for screen_control_btn_ai_pilot_mode, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_control_btn_ai_pilot_mode.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_ai_pilot_mode.set_style_bg_color(lv.color_hex(0xdcdcdc), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_ai_pilot_mode.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_ai_pilot_mode.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_ai_pilot_mode.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_ai_pilot_mode.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_ai_pilot_mode.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_ai_pilot_mode.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_ai_pilot_mode.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_btn_ai_pilot_mode.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_control_label_control_mode_contents
screen_control_label_control_mode_contents = lv.label(screen_control)
screen_control_label_control_mode_contents.set_text("Select Your Operation Style")
screen_control_label_control_mode_contents.set_long_mode(lv.label.LONG.WRAP)
screen_control_label_control_mode_contents.set_width(lv.pct(100))
screen_control_label_control_mode_contents.set_pos(20, 52)
screen_control_label_control_mode_contents.set_size(200, 32)
# Set style for screen_control_label_control_mode_contents, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_control_label_control_mode_contents.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode_contents.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode_contents.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode_contents.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode_contents.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode_contents.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode_contents.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode_contents.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode_contents.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode_contents.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode_contents.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode_contents.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode_contents.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_control_mode_contents.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_control_img_manual_mode
screen_control_img_manual_mode = lv.image(screen_control)
screen_control_img_manual_mode.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Manual_Mode_50_45.png"))
screen_control_img_manual_mode.add_flag(lv.obj.FLAG.CLICKABLE)
screen_control_img_manual_mode.set_pivot(50,50)
screen_control_img_manual_mode.set_rotation(0)
screen_control_img_manual_mode.set_pos(20, 98)
screen_control_img_manual_mode.set_size(50, 45)
# Set style for screen_control_img_manual_mode, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_control_img_manual_mode.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_control_label_manual_mode
screen_control_label_manual_mode = lv.label(screen_control)
screen_control_label_manual_mode.set_text("Manual Mode")
screen_control_label_manual_mode.set_long_mode(lv.label.LONG.WRAP)
screen_control_label_manual_mode.set_width(lv.pct(100))
screen_control_label_manual_mode.set_pos(100, 105)
screen_control_label_manual_mode.set_size(100, 20)
# Set style for screen_control_label_manual_mode, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_control_label_manual_mode.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_control_label_manual_mode_contents
screen_control_label_manual_mode_contents = lv.label(screen_control)
screen_control_label_manual_mode_contents.set_text("Full user control over all settings.")
screen_control_label_manual_mode_contents.set_long_mode(lv.label.LONG.WRAP)
screen_control_label_manual_mode_contents.set_width(lv.pct(100))
screen_control_label_manual_mode_contents.set_pos(78, 125)
screen_control_label_manual_mode_contents.set_size(150, 15)
# Set style for screen_control_label_manual_mode_contents, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_control_label_manual_mode_contents.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode_contents.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode_contents.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode_contents.set_style_text_font(test_font("montserratMedium", 8), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode_contents.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode_contents.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode_contents.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode_contents.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode_contents.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode_contents.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode_contents.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode_contents.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode_contents.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_manual_mode_contents.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_control_img_auto_mode
screen_control_img_auto_mode = lv.image(screen_control)
screen_control_img_auto_mode.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Auto_Mode_50_45.png"))
screen_control_img_auto_mode.add_flag(lv.obj.FLAG.CLICKABLE)
screen_control_img_auto_mode.set_pivot(50,50)
screen_control_img_auto_mode.set_rotation(0)
screen_control_img_auto_mode.set_pos(20, 178)
screen_control_img_auto_mode.set_size(50, 45)
# Set style for screen_control_img_auto_mode, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_control_img_auto_mode.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_control_label_auto_mode
screen_control_label_auto_mode = lv.label(screen_control)
screen_control_label_auto_mode.set_text("Auto Mode")
screen_control_label_auto_mode.set_long_mode(lv.label.LONG.WRAP)
screen_control_label_auto_mode.set_width(lv.pct(100))
screen_control_label_auto_mode.set_pos(110, 185)
screen_control_label_auto_mode.set_size(100, 20)
# Set style for screen_control_label_auto_mode, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_control_label_auto_mode.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_control_label_auto_mode_contents
screen_control_label_auto_mode_contents = lv.label(screen_control)
screen_control_label_auto_mode_contents.set_text("System adjusts basedon preset rules.")
screen_control_label_auto_mode_contents.set_long_mode(lv.label.LONG.WRAP)
screen_control_label_auto_mode_contents.set_width(lv.pct(100))
screen_control_label_auto_mode_contents.set_pos(74, 205)
screen_control_label_auto_mode_contents.set_size(157, 15)
# Set style for screen_control_label_auto_mode_contents, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_control_label_auto_mode_contents.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode_contents.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode_contents.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode_contents.set_style_text_font(test_font("montserratMedium", 8), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode_contents.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode_contents.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode_contents.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode_contents.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode_contents.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode_contents.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode_contents.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode_contents.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode_contents.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_auto_mode_contents.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_control_img_ai_pilot_mode
screen_control_img_ai_pilot_mode = lv.image(screen_control)
screen_control_img_ai_pilot_mode.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Ai_Pilot_50_45.png"))
screen_control_img_ai_pilot_mode.add_flag(lv.obj.FLAG.CLICKABLE)
screen_control_img_ai_pilot_mode.set_pivot(50,50)
screen_control_img_ai_pilot_mode.set_rotation(0)
screen_control_img_ai_pilot_mode.set_pos(20, 258)
screen_control_img_ai_pilot_mode.set_size(50, 45)
# Set style for screen_control_img_ai_pilot_mode, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_control_img_ai_pilot_mode.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_control_label_ai_pilot_mode
screen_control_label_ai_pilot_mode = lv.label(screen_control)
screen_control_label_ai_pilot_mode.set_text("AI Pilot Mode")
screen_control_label_ai_pilot_mode.set_long_mode(lv.label.LONG.WRAP)
screen_control_label_ai_pilot_mode.set_width(lv.pct(100))
screen_control_label_ai_pilot_mode.set_pos(100, 265)
screen_control_label_ai_pilot_mode.set_size(100, 20)
# Set style for screen_control_label_ai_pilot_mode, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_control_label_ai_pilot_mode.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_control_label_ai_pilot_mode_contents
screen_control_label_ai_pilot_mode_contents = lv.label(screen_control)
screen_control_label_ai_pilot_mode_contents.set_text("Al optimizes for peak performance.")
screen_control_label_ai_pilot_mode_contents.set_long_mode(lv.label.LONG.WRAP)
screen_control_label_ai_pilot_mode_contents.set_width(lv.pct(100))
screen_control_label_ai_pilot_mode_contents.set_pos(78, 285)
screen_control_label_ai_pilot_mode_contents.set_size(150, 16)
# Set style for screen_control_label_ai_pilot_mode_contents, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_control_label_ai_pilot_mode_contents.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode_contents.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode_contents.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode_contents.set_style_text_font(test_font("montserratMedium", 8), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode_contents.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode_contents.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode_contents.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode_contents.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode_contents.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode_contents.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode_contents.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode_contents.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode_contents.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_control_label_ai_pilot_mode_contents.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_control.update_layout()
# Create screen_trend
screen_trend = lv.obj()
screen_trend.set_size(240, 320)
screen_trend.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_trend, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_trend.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_trend_img_1
screen_trend_img_1 = lv.image(screen_trend)
screen_trend_img_1.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Control_Mode_Background_240_320.png"))
screen_trend_img_1.add_flag(lv.obj.FLAG.CLICKABLE)
screen_trend_img_1.set_pivot(50,50)
screen_trend_img_1.set_rotation(0)
screen_trend_img_1.set_pos(0, 0)
screen_trend_img_1.set_size(240, 320)
# Set style for screen_trend_img_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_trend_img_1.set_style_image_opa(181, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_trend.update_layout()
# Create screen_setting
screen_setting = lv.obj()
screen_setting.set_size(240, 320)
screen_setting.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_setting, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_setting.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_setting_img_1
screen_setting_img_1 = lv.image(screen_setting)
screen_setting_img_1.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Control_Mode_Background_240_320.png"))
screen_setting_img_1.add_flag(lv.obj.FLAG.CLICKABLE)
screen_setting_img_1.set_pivot(50,50)
screen_setting_img_1.set_rotation(0)
screen_setting_img_1.set_pos(0, 0)
screen_setting_img_1.set_size(240, 320)
# Set style for screen_setting_img_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_setting_img_1.set_style_image_opa(181, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_setting.update_layout()
# Create screen_manual_mode
screen_manual_mode = lv.obj()
screen_manual_mode.set_size(240, 320)
screen_manual_mode.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_manual_mode.remove_flag(lv.obj.FLAG.SCROLLABLE)
# Set style for screen_manual_mode, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_img_background
screen_manual_mode_img_background = lv.image(screen_manual_mode)
screen_manual_mode_img_background.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Dashboard_Background_2_240_320.png"))
screen_manual_mode_img_background.add_flag(lv.obj.FLAG.CLICKABLE)
screen_manual_mode_img_background.set_pivot(50,50)
screen_manual_mode_img_background.set_rotation(0)
screen_manual_mode_img_background.set_pos(0, 0)
screen_manual_mode_img_background.set_size(240, 320)
# Set style for screen_manual_mode_img_background, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_img_background.set_style_image_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_btn_manual_control
screen_manual_mode_btn_manual_control = lv.button(screen_manual_mode)
screen_manual_mode_btn_manual_control_label = lv.label(screen_manual_mode_btn_manual_control)
screen_manual_mode_btn_manual_control_label.set_text("")
screen_manual_mode_btn_manual_control_label.set_long_mode(lv.label.LONG.WRAP)
screen_manual_mode_btn_manual_control_label.set_width(lv.pct(100))
screen_manual_mode_btn_manual_control_label.align(lv.ALIGN.CENTER, 0, 0)
screen_manual_mode_btn_manual_control.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_manual_mode_btn_manual_control.set_pos(0, 0)
screen_manual_mode_btn_manual_control.set_size(240, 320)
screen_manual_mode_btn_manual_control.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_manual_mode_btn_manual_control, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_btn_manual_control.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_manual_control.set_style_bg_color(lv.color_hex(0xe6e6e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_manual_control.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_manual_control.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_manual_control.set_style_radius(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_manual_control.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_manual_control.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_manual_control.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_manual_control.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_manual_control.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_img_1
screen_manual_mode_img_1 = lv.image(screen_manual_mode)
screen_manual_mode_img_1.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Auto_Mode_Background_240_320.png"))
screen_manual_mode_img_1.add_flag(lv.obj.FLAG.CLICKABLE)
screen_manual_mode_img_1.set_pivot(50,50)
screen_manual_mode_img_1.set_rotation(0)
screen_manual_mode_img_1.set_pos(0, 0)
screen_manual_mode_img_1.set_size(240, 320)
# Set style for screen_manual_mode_img_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_img_1.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_btn_grow_light_control
screen_manual_mode_btn_grow_light_control = lv.button(screen_manual_mode)
screen_manual_mode_btn_grow_light_control_label = lv.label(screen_manual_mode_btn_grow_light_control)
screen_manual_mode_btn_grow_light_control_label.set_text("")
screen_manual_mode_btn_grow_light_control_label.set_long_mode(lv.label.LONG.WRAP)
screen_manual_mode_btn_grow_light_control_label.set_width(lv.pct(100))
screen_manual_mode_btn_grow_light_control_label.align(lv.ALIGN.CENTER, 0, 0)
screen_manual_mode_btn_grow_light_control.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_manual_mode_btn_grow_light_control.set_pos(10, 210)
screen_manual_mode_btn_grow_light_control.set_size(220, 100)
screen_manual_mode_btn_grow_light_control.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_manual_mode_btn_grow_light_control, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_btn_grow_light_control.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_grow_light_control.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_grow_light_control.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_grow_light_control.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_grow_light_control.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_grow_light_control.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_grow_light_control.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_grow_light_control.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_grow_light_control.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_grow_light_control.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_btn_ventilation_fan_control
screen_manual_mode_btn_ventilation_fan_control = lv.button(screen_manual_mode)
screen_manual_mode_btn_ventilation_fan_control_label = lv.label(screen_manual_mode_btn_ventilation_fan_control)
screen_manual_mode_btn_ventilation_fan_control_label.set_text("")
screen_manual_mode_btn_ventilation_fan_control_label.set_long_mode(lv.label.LONG.WRAP)
screen_manual_mode_btn_ventilation_fan_control_label.set_width(lv.pct(100))
screen_manual_mode_btn_ventilation_fan_control_label.align(lv.ALIGN.CENTER, 0, 0)
screen_manual_mode_btn_ventilation_fan_control.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_manual_mode_btn_ventilation_fan_control.set_pos(10, 145)
screen_manual_mode_btn_ventilation_fan_control.set_size(220, 60)
screen_manual_mode_btn_ventilation_fan_control.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_manual_mode_btn_ventilation_fan_control, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_btn_ventilation_fan_control.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_ventilation_fan_control.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_ventilation_fan_control.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_ventilation_fan_control.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_ventilation_fan_control.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_ventilation_fan_control.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_ventilation_fan_control.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_ventilation_fan_control.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_ventilation_fan_control.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_ventilation_fan_control.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_btn_quich_switches_group
screen_manual_mode_btn_quich_switches_group = lv.button(screen_manual_mode)
screen_manual_mode_btn_quich_switches_group_label = lv.label(screen_manual_mode_btn_quich_switches_group)
screen_manual_mode_btn_quich_switches_group_label.set_text("")
screen_manual_mode_btn_quich_switches_group_label.set_long_mode(lv.label.LONG.WRAP)
screen_manual_mode_btn_quich_switches_group_label.set_width(lv.pct(100))
screen_manual_mode_btn_quich_switches_group_label.align(lv.ALIGN.CENTER, 0, 0)
screen_manual_mode_btn_quich_switches_group.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_manual_mode_btn_quich_switches_group.set_pos(10, 40)
screen_manual_mode_btn_quich_switches_group.set_size(220, 100)
screen_manual_mode_btn_quich_switches_group.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_manual_mode_btn_quich_switches_group, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_btn_quich_switches_group.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_quich_switches_group.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_quich_switches_group.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_quich_switches_group.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_quich_switches_group.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_quich_switches_group.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_quich_switches_group.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_quich_switches_group.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_quich_switches_group.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_btn_quich_switches_group.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_label_manual_mode
screen_manual_mode_label_manual_mode = lv.label(screen_manual_mode)
screen_manual_mode_label_manual_mode.set_text("Manual Mode")
screen_manual_mode_label_manual_mode.set_long_mode(lv.label.LONG.WRAP)
screen_manual_mode_label_manual_mode.set_width(lv.pct(100))
screen_manual_mode_label_manual_mode.set_pos(10, 15)
screen_manual_mode_label_manual_mode.set_size(125, 15)
# Set style for screen_manual_mode_label_manual_mode, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_label_manual_mode.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_manual_mode.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_manual_mode.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_manual_mode.set_style_text_font(test_font("montserratMedium", 15), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_manual_mode.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_manual_mode.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_manual_mode.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_manual_mode.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_manual_mode.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_manual_mode.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_manual_mode.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_manual_mode.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_manual_mode.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_manual_mode.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_img_awning
screen_manual_mode_img_awning = lv.image(screen_manual_mode)
screen_manual_mode_img_awning.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Awning_15_15.png"))
screen_manual_mode_img_awning.add_flag(lv.obj.FLAG.CLICKABLE)
screen_manual_mode_img_awning.set_pivot(50,50)
screen_manual_mode_img_awning.set_rotation(0)
screen_manual_mode_img_awning.set_pos(20, 65)
screen_manual_mode_img_awning.set_size(15, 15)
# Set style for screen_manual_mode_img_awning, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_img_awning.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_img_humidifier
screen_manual_mode_img_humidifier = lv.image(screen_manual_mode)
screen_manual_mode_img_humidifier.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Humidifier_15_15.png"))
screen_manual_mode_img_humidifier.add_flag(lv.obj.FLAG.CLICKABLE)
screen_manual_mode_img_humidifier.set_pivot(50,50)
screen_manual_mode_img_humidifier.set_rotation(0)
screen_manual_mode_img_humidifier.set_pos(20, 120)
screen_manual_mode_img_humidifier.set_size(15, 15)
# Set style for screen_manual_mode_img_humidifier, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_img_humidifier.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_img_water_pump
screen_manual_mode_img_water_pump = lv.image(screen_manual_mode)
screen_manual_mode_img_water_pump.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Water_pump_15_15.png"))
screen_manual_mode_img_water_pump.add_flag(lv.obj.FLAG.CLICKABLE)
screen_manual_mode_img_water_pump.set_pivot(50,50)
screen_manual_mode_img_water_pump.set_rotation(0)
screen_manual_mode_img_water_pump.set_pos(20, 93)
screen_manual_mode_img_water_pump.set_size(15, 15)
# Set style for screen_manual_mode_img_water_pump, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_img_water_pump.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_label_quick_switches_groups
screen_manual_mode_label_quick_switches_groups = lv.label(screen_manual_mode)
screen_manual_mode_label_quick_switches_groups.set_text("Quick Switches Group")
screen_manual_mode_label_quick_switches_groups.set_long_mode(lv.label.LONG.WRAP)
screen_manual_mode_label_quick_switches_groups.set_width(lv.pct(100))
screen_manual_mode_label_quick_switches_groups.set_pos(20, 47)
screen_manual_mode_label_quick_switches_groups.set_size(200, 15)
# Set style for screen_manual_mode_label_quick_switches_groups, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_label_quick_switches_groups.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_quick_switches_groups.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_quick_switches_groups.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_quick_switches_groups.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_quick_switches_groups.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_quick_switches_groups.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_quick_switches_groups.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_quick_switches_groups.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_quick_switches_groups.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_quick_switches_groups.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_quick_switches_groups.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_quick_switches_groups.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_quick_switches_groups.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_quick_switches_groups.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_label_awning
screen_manual_mode_label_awning = lv.label(screen_manual_mode)
screen_manual_mode_label_awning.set_text("Awning")
screen_manual_mode_label_awning.set_long_mode(lv.label.LONG.WRAP)
screen_manual_mode_label_awning.set_width(lv.pct(100))
screen_manual_mode_label_awning.set_pos(40, 67)
screen_manual_mode_label_awning.set_size(50, 15)
# Set style for screen_manual_mode_label_awning, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_label_awning.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_awning.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_awning.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_awning.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_awning.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_awning.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_awning.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_awning.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_awning.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_awning.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_awning.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_awning.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_awning.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_awning.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_sw_awning
screen_manual_mode_sw_awning = lv.switch(screen_manual_mode)
screen_manual_mode_sw_awning.set_pos(180, 65)
screen_manual_mode_sw_awning.set_size(30, 15)
# Set style for screen_manual_mode_sw_awning, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_sw_awning.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_sw_awning.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_sw_awning.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_sw_awning.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_sw_awning.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_sw_awning.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_manual_mode_sw_awning, Part: lv.PART.INDICATOR, State: lv.STATE.CHECKED.
screen_manual_mode_sw_awning.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_manual_mode_sw_awning.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_manual_mode_sw_awning.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_manual_mode_sw_awning.set_style_border_width(0, lv.PART.INDICATOR|lv.STATE.CHECKED)

# Set style for screen_manual_mode_sw_awning, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_manual_mode_sw_awning.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_sw_awning.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_sw_awning.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_sw_awning.set_style_border_width(0, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_sw_awning.set_style_radius(10, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_manual_mode_line_1
screen_manual_mode_line_1 = lv.line(screen_manual_mode)
screen_manual_mode_line_1_line_points = [{"x":0, "y":0},{"x":185, "y":0},]
screen_manual_mode_line_1.set_points(screen_manual_mode_line_1_line_points, 2)
screen_manual_mode_line_1.set_pos(45, 85)
screen_manual_mode_line_1.set_size(185, 10)
# Set style for screen_manual_mode_line_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_line_1.set_style_line_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_line_1.set_style_line_color(lv.color_hex(0x757575), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_line_1.set_style_line_opa(100, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_line_1.set_style_line_rounded(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_line_2
screen_manual_mode_line_2 = lv.line(screen_manual_mode)
screen_manual_mode_line_2_line_points = [{"x":0, "y":0},{"x":185, "y":0},]
screen_manual_mode_line_2.set_points(screen_manual_mode_line_2_line_points, 2)
screen_manual_mode_line_2.set_pos(45, 113)
screen_manual_mode_line_2.set_size(185, 11)
# Set style for screen_manual_mode_line_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_line_2.set_style_line_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_line_2.set_style_line_color(lv.color_hex(0x757575), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_line_2.set_style_line_opa(100, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_line_2.set_style_line_rounded(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_sw_water_pump
screen_manual_mode_sw_water_pump = lv.switch(screen_manual_mode)
screen_manual_mode_sw_water_pump.set_pos(180, 93)
screen_manual_mode_sw_water_pump.set_size(30, 15)
# Set style for screen_manual_mode_sw_water_pump, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_sw_water_pump.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_sw_water_pump.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_sw_water_pump.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_sw_water_pump.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_sw_water_pump.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_sw_water_pump.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_manual_mode_sw_water_pump, Part: lv.PART.INDICATOR, State: lv.STATE.CHECKED.
screen_manual_mode_sw_water_pump.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_manual_mode_sw_water_pump.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_manual_mode_sw_water_pump.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_manual_mode_sw_water_pump.set_style_border_width(0, lv.PART.INDICATOR|lv.STATE.CHECKED)

# Set style for screen_manual_mode_sw_water_pump, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_manual_mode_sw_water_pump.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_sw_water_pump.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_sw_water_pump.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_sw_water_pump.set_style_border_width(0, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_sw_water_pump.set_style_radius(10, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_manual_mode_label_water_pump
screen_manual_mode_label_water_pump = lv.label(screen_manual_mode)
screen_manual_mode_label_water_pump.set_text("Water pump")
screen_manual_mode_label_water_pump.set_long_mode(lv.label.LONG.WRAP)
screen_manual_mode_label_water_pump.set_width(lv.pct(100))
screen_manual_mode_label_water_pump.set_pos(45, 95)
screen_manual_mode_label_water_pump.set_size(81, 10)
# Set style for screen_manual_mode_label_water_pump, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_label_water_pump.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_water_pump.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_water_pump.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_water_pump.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_water_pump.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_water_pump.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_water_pump.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_water_pump.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_water_pump.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_water_pump.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_water_pump.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_water_pump.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_water_pump.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_water_pump.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_label_humidifier
screen_manual_mode_label_humidifier = lv.label(screen_manual_mode)
screen_manual_mode_label_humidifier.set_text("Humidifier")
screen_manual_mode_label_humidifier.set_long_mode(lv.label.LONG.WRAP)
screen_manual_mode_label_humidifier.set_width(lv.pct(100))
screen_manual_mode_label_humidifier.set_pos(42, 123)
screen_manual_mode_label_humidifier.set_size(62, 11)
# Set style for screen_manual_mode_label_humidifier, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_label_humidifier.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_humidifier.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_humidifier.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_humidifier.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_humidifier.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_humidifier.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_humidifier.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_humidifier.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_humidifier.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_humidifier.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_humidifier.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_humidifier.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_humidifier.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_humidifier.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_sw_humidifier
screen_manual_mode_sw_humidifier = lv.switch(screen_manual_mode)
screen_manual_mode_sw_humidifier.set_pos(180, 120)
screen_manual_mode_sw_humidifier.set_size(30, 15)
# Set style for screen_manual_mode_sw_humidifier, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_sw_humidifier.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_sw_humidifier.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_sw_humidifier.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_sw_humidifier.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_sw_humidifier.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_sw_humidifier.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_manual_mode_sw_humidifier, Part: lv.PART.INDICATOR, State: lv.STATE.CHECKED.
screen_manual_mode_sw_humidifier.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_manual_mode_sw_humidifier.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_manual_mode_sw_humidifier.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_manual_mode_sw_humidifier.set_style_border_width(0, lv.PART.INDICATOR|lv.STATE.CHECKED)

# Set style for screen_manual_mode_sw_humidifier, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_manual_mode_sw_humidifier.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_sw_humidifier.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_sw_humidifier.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_sw_humidifier.set_style_border_width(0, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_sw_humidifier.set_style_radius(10, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_manual_mode_img_ventilation_fan
screen_manual_mode_img_ventilation_fan = lv.image(screen_manual_mode)
screen_manual_mode_img_ventilation_fan.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Ventilation_Fan_15_15.png"))
screen_manual_mode_img_ventilation_fan.add_flag(lv.obj.FLAG.CLICKABLE)
screen_manual_mode_img_ventilation_fan.set_pivot(50,50)
screen_manual_mode_img_ventilation_fan.set_rotation(0)
screen_manual_mode_img_ventilation_fan.set_pos(20, 153)
screen_manual_mode_img_ventilation_fan.set_size(15, 15)
# Set style for screen_manual_mode_img_ventilation_fan, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_img_ventilation_fan.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_label_ventilation_fan_control
screen_manual_mode_label_ventilation_fan_control = lv.label(screen_manual_mode)
screen_manual_mode_label_ventilation_fan_control.set_text("Ventilation Fan Control")
screen_manual_mode_label_ventilation_fan_control.set_long_mode(lv.label.LONG.WRAP)
screen_manual_mode_label_ventilation_fan_control.set_width(lv.pct(100))
screen_manual_mode_label_ventilation_fan_control.set_pos(37, 156)
screen_manual_mode_label_ventilation_fan_control.set_size(135, 14)
# Set style for screen_manual_mode_label_ventilation_fan_control, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_label_ventilation_fan_control.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_control.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_control.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_control.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_control.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_control.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_control.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_control.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_control.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_control.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_control.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_control.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_control.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_control.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_label_ventilation_fan_speed
screen_manual_mode_label_ventilation_fan_speed = lv.label(screen_manual_mode)
screen_manual_mode_label_ventilation_fan_speed.set_text("speed")
screen_manual_mode_label_ventilation_fan_speed.set_long_mode(lv.label.LONG.WRAP)
screen_manual_mode_label_ventilation_fan_speed.set_width(lv.pct(100))
screen_manual_mode_label_ventilation_fan_speed.set_pos(20, 177)
screen_manual_mode_label_ventilation_fan_speed.set_size(57, 15)
# Set style for screen_manual_mode_label_ventilation_fan_speed, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_label_ventilation_fan_speed.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_label_ventilation_fan_speed_value
screen_manual_mode_label_ventilation_fan_speed_value = lv.label(screen_manual_mode)
screen_manual_mode_label_ventilation_fan_speed_value.set_text("60 rpm")
screen_manual_mode_label_ventilation_fan_speed_value.set_long_mode(lv.label.LONG.WRAP)
screen_manual_mode_label_ventilation_fan_speed_value.set_width(lv.pct(100))
screen_manual_mode_label_ventilation_fan_speed_value.set_pos(160, 177)
screen_manual_mode_label_ventilation_fan_speed_value.set_size(60, 15)
# Set style for screen_manual_mode_label_ventilation_fan_speed_value, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_label_ventilation_fan_speed_value.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed_value.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed_value.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed_value.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed_value.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed_value.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed_value.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed_value.set_style_text_align(lv.TEXT_ALIGN.RIGHT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed_value.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed_value.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed_value.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed_value.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed_value.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_ventilation_fan_speed_value.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_slider_ventilation_fan_speed
screen_manual_mode_slider_ventilation_fan_speed = lv.slider(screen_manual_mode)
screen_manual_mode_slider_ventilation_fan_speed.set_range(0, 300)
screen_manual_mode_slider_ventilation_fan_speed.set_mode(lv.slider.MODE.NORMAL)
screen_manual_mode_slider_ventilation_fan_speed.set_value(60, lv.ANIM.OFF)
screen_manual_mode_slider_ventilation_fan_speed.set_pos(20, 195)
screen_manual_mode_slider_ventilation_fan_speed.set_size(200, 3)
# Set style for screen_manual_mode_slider_ventilation_fan_speed, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_slider_ventilation_fan_speed.set_style_bg_opa(200, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_slider_ventilation_fan_speed.set_style_bg_color(lv.color_hex(0xdcdcdc), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_slider_ventilation_fan_speed.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_slider_ventilation_fan_speed.set_style_radius(3, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_slider_ventilation_fan_speed.set_style_outline_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_slider_ventilation_fan_speed.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_manual_mode_slider_ventilation_fan_speed, Part: lv.PART.INDICATOR, State: lv.STATE.DEFAULT.
screen_manual_mode_slider_ventilation_fan_speed.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_manual_mode_slider_ventilation_fan_speed.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_manual_mode_slider_ventilation_fan_speed.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_manual_mode_slider_ventilation_fan_speed.set_style_radius(3, lv.PART.INDICATOR|lv.STATE.DEFAULT)

# Set style for screen_manual_mode_slider_ventilation_fan_speed, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_manual_mode_slider_ventilation_fan_speed.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_slider_ventilation_fan_speed.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_slider_ventilation_fan_speed.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_slider_ventilation_fan_speed.set_style_radius(8, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_manual_mode_img_grow_light
screen_manual_mode_img_grow_light = lv.image(screen_manual_mode)
screen_manual_mode_img_grow_light.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Fill_Light_15_15.png"))
screen_manual_mode_img_grow_light.add_flag(lv.obj.FLAG.CLICKABLE)
screen_manual_mode_img_grow_light.set_pivot(15,15)
screen_manual_mode_img_grow_light.set_rotation(0)
screen_manual_mode_img_grow_light.set_pos(20, 220)
screen_manual_mode_img_grow_light.set_size(15, 15)
# Set style for screen_manual_mode_img_grow_light, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_img_grow_light.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_label_grow_light_control
screen_manual_mode_label_grow_light_control = lv.label(screen_manual_mode)
screen_manual_mode_label_grow_light_control.set_text("Grow Light Control")
screen_manual_mode_label_grow_light_control.set_long_mode(lv.label.LONG.WRAP)
screen_manual_mode_label_grow_light_control.set_width(lv.pct(100))
screen_manual_mode_label_grow_light_control.set_pos(43, 223)
screen_manual_mode_label_grow_light_control.set_size(135, 14)
# Set style for screen_manual_mode_label_grow_light_control, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_label_grow_light_control.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_grow_light_control.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_grow_light_control.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_grow_light_control.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_grow_light_control.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_grow_light_control.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_grow_light_control.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_grow_light_control.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_grow_light_control.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_grow_light_control.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_grow_light_control.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_grow_light_control.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_grow_light_control.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_grow_light_control.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_label_brightness
screen_manual_mode_label_brightness = lv.label(screen_manual_mode)
screen_manual_mode_label_brightness.set_text("brightness")
screen_manual_mode_label_brightness.set_long_mode(lv.label.LONG.WRAP)
screen_manual_mode_label_brightness.set_width(lv.pct(100))
screen_manual_mode_label_brightness.set_pos(20, 243)
screen_manual_mode_label_brightness.set_size(57, 15)
# Set style for screen_manual_mode_label_brightness, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_label_brightness.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_slider_brightness_value
screen_manual_mode_slider_brightness_value = lv.slider(screen_manual_mode)
screen_manual_mode_slider_brightness_value.set_range(0, 1000)
screen_manual_mode_slider_brightness_value.set_mode(lv.slider.MODE.NORMAL)
screen_manual_mode_slider_brightness_value.set_value(300, lv.ANIM.OFF)
screen_manual_mode_slider_brightness_value.set_pos(20, 260)
screen_manual_mode_slider_brightness_value.set_size(200, 3)
# Set style for screen_manual_mode_slider_brightness_value, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_slider_brightness_value.set_style_bg_opa(200, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_slider_brightness_value.set_style_bg_color(lv.color_hex(0xdcdcdc), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_slider_brightness_value.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_slider_brightness_value.set_style_radius(3, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_slider_brightness_value.set_style_outline_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_slider_brightness_value.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_manual_mode_slider_brightness_value, Part: lv.PART.INDICATOR, State: lv.STATE.DEFAULT.
screen_manual_mode_slider_brightness_value.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_manual_mode_slider_brightness_value.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_manual_mode_slider_brightness_value.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_manual_mode_slider_brightness_value.set_style_radius(3, lv.PART.INDICATOR|lv.STATE.DEFAULT)

# Set style for screen_manual_mode_slider_brightness_value, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_manual_mode_slider_brightness_value.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_slider_brightness_value.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_slider_brightness_value.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_slider_brightness_value.set_style_radius(8, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_manual_mode_label_brightness_value
screen_manual_mode_label_brightness_value = lv.label(screen_manual_mode)
screen_manual_mode_label_brightness_value.set_text("300 lx")
screen_manual_mode_label_brightness_value.set_long_mode(lv.label.LONG.WRAP)
screen_manual_mode_label_brightness_value.set_width(lv.pct(100))
screen_manual_mode_label_brightness_value.set_pos(169, 243)
screen_manual_mode_label_brightness_value.set_size(50, 15)
# Set style for screen_manual_mode_label_brightness_value, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_label_brightness_value.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness_value.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness_value.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness_value.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness_value.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness_value.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness_value.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness_value.set_style_text_align(lv.TEXT_ALIGN.RIGHT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness_value.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness_value.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness_value.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness_value.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness_value.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_brightness_value.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_label_color_adjustment
screen_manual_mode_label_color_adjustment = lv.label(screen_manual_mode)
screen_manual_mode_label_color_adjustment.set_text("color adjustment")
screen_manual_mode_label_color_adjustment.set_long_mode(lv.label.LONG.WRAP)
screen_manual_mode_label_color_adjustment.set_width(lv.pct(100))
screen_manual_mode_label_color_adjustment.set_pos(20, 275)
screen_manual_mode_label_color_adjustment.set_size(100, 15)
# Set style for screen_manual_mode_label_color_adjustment, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_label_color_adjustment.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_color_adjustment.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_color_adjustment.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_color_adjustment.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_color_adjustment.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_color_adjustment.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_color_adjustment.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_color_adjustment.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_color_adjustment.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_color_adjustment.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_color_adjustment.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_color_adjustment.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_color_adjustment.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_label_color_adjustment.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_manual_mode_slider_color_adjustment
screen_manual_mode_slider_color_adjustment = lv.slider(screen_manual_mode)
screen_manual_mode_slider_color_adjustment.set_range(0, 100)
screen_manual_mode_slider_color_adjustment.set_mode(lv.slider.MODE.NORMAL)
screen_manual_mode_slider_color_adjustment.set_value(50, lv.ANIM.OFF)
screen_manual_mode_slider_color_adjustment.set_pos(20, 295)
screen_manual_mode_slider_color_adjustment.set_size(200, 3)
# Set style for screen_manual_mode_slider_color_adjustment, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_manual_mode_slider_color_adjustment.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_slider_color_adjustment.set_style_radius(3, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_slider_color_adjustment.set_style_outline_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_slider_color_adjustment.set_style_bg_image_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Rainbow_Slider_200_3.png"), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_slider_color_adjustment.set_style_bg_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_slider_color_adjustment.set_style_bg_image_recolor_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_manual_mode_slider_color_adjustment.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_manual_mode_slider_color_adjustment, Part: lv.PART.INDICATOR, State: lv.STATE.DEFAULT.
screen_manual_mode_slider_color_adjustment.set_style_bg_opa(0, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_manual_mode_slider_color_adjustment.set_style_radius(3, lv.PART.INDICATOR|lv.STATE.DEFAULT)

# Set style for screen_manual_mode_slider_color_adjustment, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_manual_mode_slider_color_adjustment.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_slider_color_adjustment.set_style_bg_color(lv.color_hex(0xc8c8c8), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_slider_color_adjustment.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_manual_mode_slider_color_adjustment.set_style_radius(8, lv.PART.KNOB|lv.STATE.DEFAULT)

screen_manual_mode.update_layout()
# Create screen_auto_mode
screen_auto_mode = lv.obj()
screen_auto_mode.set_size(240, 320)
screen_auto_mode.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_auto_mode, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_img_6
screen_auto_mode_img_6 = lv.image(screen_auto_mode)
screen_auto_mode_img_6.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Dashboard_Background_2_240_320.png"))
screen_auto_mode_img_6.add_flag(lv.obj.FLAG.CLICKABLE)
screen_auto_mode_img_6.set_pivot(50,50)
screen_auto_mode_img_6.set_rotation(0)
screen_auto_mode_img_6.set_pos(0, 0)
screen_auto_mode_img_6.set_size(240, 320)
# Set style for screen_auto_mode_img_6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_img_6.set_style_image_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_btn_background
screen_auto_mode_btn_background = lv.button(screen_auto_mode)
screen_auto_mode_btn_background_label = lv.label(screen_auto_mode_btn_background)
screen_auto_mode_btn_background_label.set_text("")
screen_auto_mode_btn_background_label.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_btn_background_label.set_width(lv.pct(100))
screen_auto_mode_btn_background_label.align(lv.ALIGN.CENTER, 0, 0)
screen_auto_mode_btn_background.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_auto_mode_btn_background.set_pos(0, 0)
screen_auto_mode_btn_background.set_size(240, 320)
screen_auto_mode_btn_background.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_auto_mode_btn_background, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_btn_background.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_background.set_style_bg_color(lv.color_hex(0xe6e6e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_background.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_background.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_background.set_style_radius(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_background.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_background.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_background.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_background.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_background.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_img_background
screen_auto_mode_img_background = lv.image(screen_auto_mode)
screen_auto_mode_img_background.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Auto_Mode_Background_240_320.png"))
screen_auto_mode_img_background.add_flag(lv.obj.FLAG.CLICKABLE)
screen_auto_mode_img_background.set_pivot(50,50)
screen_auto_mode_img_background.set_rotation(0)
screen_auto_mode_img_background.set_pos(0, 0)
screen_auto_mode_img_background.set_size(240, 320)
# Set style for screen_auto_mode_img_background, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_img_background.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_btn_temperature
screen_auto_mode_btn_temperature = lv.button(screen_auto_mode)
screen_auto_mode_btn_temperature_label = lv.label(screen_auto_mode_btn_temperature)
screen_auto_mode_btn_temperature_label.set_text("")
screen_auto_mode_btn_temperature_label.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_btn_temperature_label.set_width(lv.pct(100))
screen_auto_mode_btn_temperature_label.align(lv.ALIGN.CENTER, 0, 0)
screen_auto_mode_btn_temperature.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_auto_mode_btn_temperature.set_pos(10, 40)
screen_auto_mode_btn_temperature.set_size(220, 40)
screen_auto_mode_btn_temperature.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_auto_mode_btn_temperature, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_btn_temperature.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_temperature.set_style_bg_color(lv.color_hex(0xe7e7e7), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_temperature.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_temperature.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_temperature.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_temperature.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_temperature.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_temperature.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_temperature.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_temperature.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_label_auto_mode
screen_auto_mode_label_auto_mode = lv.label(screen_auto_mode)
screen_auto_mode_label_auto_mode.set_text("Auto Mode")
screen_auto_mode_label_auto_mode.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_label_auto_mode.set_width(lv.pct(100))
screen_auto_mode_label_auto_mode.set_pos(10, 15)
screen_auto_mode_label_auto_mode.set_size(125, 15)
# Set style for screen_auto_mode_label_auto_mode, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_label_auto_mode.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_auto_mode.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_auto_mode.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_auto_mode.set_style_text_font(test_font("montserratMedium", 15), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_auto_mode.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_auto_mode.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_auto_mode.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_auto_mode.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_auto_mode.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_auto_mode.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_auto_mode.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_auto_mode.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_auto_mode.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_auto_mode.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_btn_humidity
screen_auto_mode_btn_humidity = lv.button(screen_auto_mode)
screen_auto_mode_btn_humidity_label = lv.label(screen_auto_mode_btn_humidity)
screen_auto_mode_btn_humidity_label.set_text("")
screen_auto_mode_btn_humidity_label.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_btn_humidity_label.set_width(lv.pct(100))
screen_auto_mode_btn_humidity_label.align(lv.ALIGN.CENTER, 0, 0)
screen_auto_mode_btn_humidity.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_auto_mode_btn_humidity.set_pos(10, 88)
screen_auto_mode_btn_humidity.set_size(220, 40)
screen_auto_mode_btn_humidity.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_auto_mode_btn_humidity, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_btn_humidity.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_humidity.set_style_bg_color(lv.color_hex(0xe6e6e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_humidity.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_humidity.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_humidity.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_humidity.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_humidity.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_humidity.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_humidity.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_humidity.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_btn_soil_moisture
screen_auto_mode_btn_soil_moisture = lv.button(screen_auto_mode)
screen_auto_mode_btn_soil_moisture_label = lv.label(screen_auto_mode_btn_soil_moisture)
screen_auto_mode_btn_soil_moisture_label.set_text("")
screen_auto_mode_btn_soil_moisture_label.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_btn_soil_moisture_label.set_width(lv.pct(100))
screen_auto_mode_btn_soil_moisture_label.align(lv.ALIGN.CENTER, 0, 0)
screen_auto_mode_btn_soil_moisture.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_auto_mode_btn_soil_moisture.set_pos(10, 137)
screen_auto_mode_btn_soil_moisture.set_size(220, 40)
screen_auto_mode_btn_soil_moisture.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_auto_mode_btn_soil_moisture, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_btn_soil_moisture.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_soil_moisture.set_style_bg_color(lv.color_hex(0xe6e6e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_soil_moisture.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_soil_moisture.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_soil_moisture.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_soil_moisture.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_soil_moisture.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_soil_moisture.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_soil_moisture.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_soil_moisture.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_btn_light_intensity
screen_auto_mode_btn_light_intensity = lv.button(screen_auto_mode)
screen_auto_mode_btn_light_intensity_label = lv.label(screen_auto_mode_btn_light_intensity)
screen_auto_mode_btn_light_intensity_label.set_text("")
screen_auto_mode_btn_light_intensity_label.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_btn_light_intensity_label.set_width(lv.pct(100))
screen_auto_mode_btn_light_intensity_label.align(lv.ALIGN.CENTER, 0, 0)
screen_auto_mode_btn_light_intensity.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_auto_mode_btn_light_intensity.set_pos(10, 186)
screen_auto_mode_btn_light_intensity.set_size(220, 40)
screen_auto_mode_btn_light_intensity.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_auto_mode_btn_light_intensity, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_btn_light_intensity.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_light_intensity.set_style_bg_color(lv.color_hex(0xe6e6e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_light_intensity.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_light_intensity.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_light_intensity.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_light_intensity.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_light_intensity.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_light_intensity.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_light_intensity.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_light_intensity.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_btn_light_color
screen_auto_mode_btn_light_color = lv.button(screen_auto_mode)
screen_auto_mode_btn_light_color_label = lv.label(screen_auto_mode_btn_light_color)
screen_auto_mode_btn_light_color_label.set_text("")
screen_auto_mode_btn_light_color_label.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_btn_light_color_label.set_width(lv.pct(100))
screen_auto_mode_btn_light_color_label.align(lv.ALIGN.CENTER, 0, 0)
screen_auto_mode_btn_light_color.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_auto_mode_btn_light_color.set_pos(10, 234)
screen_auto_mode_btn_light_color.set_size(220, 80)
screen_auto_mode_btn_light_color.remove_flag(lv.obj.FLAG.CLICKABLE)
# Set style for screen_auto_mode_btn_light_color, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_btn_light_color.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_light_color.set_style_bg_color(lv.color_hex(0xe6e6e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_light_color.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_light_color.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_light_color.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_light_color.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_light_color.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_light_color.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_light_color.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_btn_light_color.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_img_temperature
screen_auto_mode_img_temperature = lv.image(screen_auto_mode)
screen_auto_mode_img_temperature.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Temperature_Auto_Mode_15_15.png"))
screen_auto_mode_img_temperature.add_flag(lv.obj.FLAG.CLICKABLE)
screen_auto_mode_img_temperature.set_pivot(50,50)
screen_auto_mode_img_temperature.set_rotation(0)
screen_auto_mode_img_temperature.set_pos(20, 44)
screen_auto_mode_img_temperature.set_size(15, 15)
# Set style for screen_auto_mode_img_temperature, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_img_temperature.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_img_humidity
screen_auto_mode_img_humidity = lv.image(screen_auto_mode)
screen_auto_mode_img_humidity.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Humidity_Auto_Mode_15_15.png"))
screen_auto_mode_img_humidity.add_flag(lv.obj.FLAG.CLICKABLE)
screen_auto_mode_img_humidity.set_pivot(50,50)
screen_auto_mode_img_humidity.set_rotation(0)
screen_auto_mode_img_humidity.set_pos(20, 93)
screen_auto_mode_img_humidity.set_size(15, 15)
# Set style for screen_auto_mode_img_humidity, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_img_humidity.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_label_temperature_range
screen_auto_mode_label_temperature_range = lv.label(screen_auto_mode)
screen_auto_mode_label_temperature_range.set_text("Temperature range")
screen_auto_mode_label_temperature_range.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_label_temperature_range.set_width(lv.pct(100))
screen_auto_mode_label_temperature_range.set_pos(42, 47)
screen_auto_mode_label_temperature_range.set_size(150, 10)
# Set style for screen_auto_mode_label_temperature_range, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_label_temperature_range.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_range.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_range.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_range.set_style_text_font(test_font("montserratMedium", 8), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_range.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_range.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_range.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_range.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_range.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_range.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_range.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_range.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_range.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_range.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_slider_temperature_range
screen_auto_mode_slider_temperature_range = lv.slider(screen_auto_mode)
screen_auto_mode_slider_temperature_range.set_range(0, 50)
screen_auto_mode_slider_temperature_range.set_mode(lv.slider.MODE.RANGE)
screen_auto_mode_slider_temperature_range.set_value(30, lv.ANIM.OFF)
screen_auto_mode_slider_temperature_range.set_left_value(20, lv.ANIM.OFF);screen_auto_mode_slider_temperature_range.set_pos(45, 67)
screen_auto_mode_slider_temperature_range.set_size(150, 3)
# Set style for screen_auto_mode_slider_temperature_range, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_slider_temperature_range.set_style_bg_opa(50, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_temperature_range.set_style_bg_color(lv.color_hex(0xdc143c), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_temperature_range.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_temperature_range.set_style_radius(8, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_temperature_range.set_style_outline_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_temperature_range.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_auto_mode_slider_temperature_range, Part: lv.PART.INDICATOR, State: lv.STATE.DEFAULT.
screen_auto_mode_slider_temperature_range.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_auto_mode_slider_temperature_range.set_style_bg_color(lv.color_hex(0xdc143c), lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_auto_mode_slider_temperature_range.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_auto_mode_slider_temperature_range.set_style_radius(8, lv.PART.INDICATOR|lv.STATE.DEFAULT)

# Set style for screen_auto_mode_slider_temperature_range, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_auto_mode_slider_temperature_range.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_auto_mode_slider_temperature_range.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_auto_mode_slider_temperature_range.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_auto_mode_slider_temperature_range.set_style_radius(8, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_auto_mode_label_temperature_left_value
screen_auto_mode_label_temperature_left_value = lv.label(screen_auto_mode)
screen_auto_mode_label_temperature_left_value.set_text("20°C")
screen_auto_mode_label_temperature_left_value.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_label_temperature_left_value.set_width(lv.pct(100))
screen_auto_mode_label_temperature_left_value.set_pos(20, 64)
screen_auto_mode_label_temperature_left_value.set_size(25, 10)
# Set style for screen_auto_mode_label_temperature_left_value, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_label_temperature_left_value.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_left_value.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_left_value.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_left_value.set_style_text_font(test_font("montserratMedium", 8), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_left_value.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_left_value.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_left_value.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_left_value.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_left_value.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_left_value.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_left_value.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_left_value.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_left_value.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_left_value.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_label_humidity_range
screen_auto_mode_label_humidity_range = lv.label(screen_auto_mode)
screen_auto_mode_label_humidity_range.set_text("Humidity range")
screen_auto_mode_label_humidity_range.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_label_humidity_range.set_width(lv.pct(100))
screen_auto_mode_label_humidity_range.set_pos(42, 97)
screen_auto_mode_label_humidity_range.set_size(150, 10)
# Set style for screen_auto_mode_label_humidity_range, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_label_humidity_range.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_range.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_range.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_range.set_style_text_font(test_font("montserratMedium", 8), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_range.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_range.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_range.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_range.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_range.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_range.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_range.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_range.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_range.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_range.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_slider_humidity_range
screen_auto_mode_slider_humidity_range = lv.slider(screen_auto_mode)
screen_auto_mode_slider_humidity_range.set_range(0, 100)
screen_auto_mode_slider_humidity_range.set_mode(lv.slider.MODE.RANGE)
screen_auto_mode_slider_humidity_range.set_value(70, lv.ANIM.OFF)
screen_auto_mode_slider_humidity_range.set_left_value(40, lv.ANIM.OFF);screen_auto_mode_slider_humidity_range.set_pos(45, 115)
screen_auto_mode_slider_humidity_range.set_size(150, 3)
# Set style for screen_auto_mode_slider_humidity_range, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_slider_humidity_range.set_style_bg_opa(50, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_humidity_range.set_style_bg_color(lv.color_hex(0xadd8e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_humidity_range.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_humidity_range.set_style_radius(8, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_humidity_range.set_style_outline_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_humidity_range.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_auto_mode_slider_humidity_range, Part: lv.PART.INDICATOR, State: lv.STATE.DEFAULT.
screen_auto_mode_slider_humidity_range.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_auto_mode_slider_humidity_range.set_style_bg_color(lv.color_hex(0xadd8e6), lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_auto_mode_slider_humidity_range.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_auto_mode_slider_humidity_range.set_style_radius(8, lv.PART.INDICATOR|lv.STATE.DEFAULT)

# Set style for screen_auto_mode_slider_humidity_range, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_auto_mode_slider_humidity_range.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_auto_mode_slider_humidity_range.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_auto_mode_slider_humidity_range.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_auto_mode_slider_humidity_range.set_style_radius(8, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_auto_mode_label_humidity_left_value
screen_auto_mode_label_humidity_left_value = lv.label(screen_auto_mode)
screen_auto_mode_label_humidity_left_value.set_text("40%")
screen_auto_mode_label_humidity_left_value.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_label_humidity_left_value.set_width(lv.pct(100))
screen_auto_mode_label_humidity_left_value.set_pos(20, 115)
screen_auto_mode_label_humidity_left_value.set_size(20, 10)
# Set style for screen_auto_mode_label_humidity_left_value, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_label_humidity_left_value.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_left_value.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_left_value.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_left_value.set_style_text_font(test_font("montserratMedium", 8), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_left_value.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_left_value.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_left_value.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_left_value.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_left_value.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_left_value.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_left_value.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_left_value.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_left_value.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_left_value.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_label_temperature_right_value
screen_auto_mode_label_temperature_right_value = lv.label(screen_auto_mode)
screen_auto_mode_label_temperature_right_value.set_text("30°C")
screen_auto_mode_label_temperature_right_value.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_label_temperature_right_value.set_width(lv.pct(100))
screen_auto_mode_label_temperature_right_value.set_pos(200, 64)
screen_auto_mode_label_temperature_right_value.set_size(20, 10)
# Set style for screen_auto_mode_label_temperature_right_value, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_label_temperature_right_value.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_right_value.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_right_value.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_right_value.set_style_text_font(test_font("montserratMedium", 8), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_right_value.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_right_value.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_right_value.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_right_value.set_style_text_align(lv.TEXT_ALIGN.RIGHT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_right_value.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_right_value.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_right_value.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_right_value.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_right_value.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_temperature_right_value.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_label_humidity_right_value
screen_auto_mode_label_humidity_right_value = lv.label(screen_auto_mode)
screen_auto_mode_label_humidity_right_value.set_text("70%")
screen_auto_mode_label_humidity_right_value.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_label_humidity_right_value.set_width(lv.pct(100))
screen_auto_mode_label_humidity_right_value.set_pos(200, 114)
screen_auto_mode_label_humidity_right_value.set_size(20, 10)
# Set style for screen_auto_mode_label_humidity_right_value, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_label_humidity_right_value.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_right_value.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_right_value.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_right_value.set_style_text_font(test_font("montserratMedium", 8), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_right_value.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_right_value.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_right_value.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_right_value.set_style_text_align(lv.TEXT_ALIGN.RIGHT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_right_value.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_right_value.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_right_value.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_right_value.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_right_value.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_humidity_right_value.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_label_soil_moisture_right_value
screen_auto_mode_label_soil_moisture_right_value = lv.label(screen_auto_mode)
screen_auto_mode_label_soil_moisture_right_value.set_text("60%")
screen_auto_mode_label_soil_moisture_right_value.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_label_soil_moisture_right_value.set_width(lv.pct(100))
screen_auto_mode_label_soil_moisture_right_value.set_pos(200, 162)
screen_auto_mode_label_soil_moisture_right_value.set_size(20, 10)
# Set style for screen_auto_mode_label_soil_moisture_right_value, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_label_soil_moisture_right_value.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_right_value.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_right_value.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_right_value.set_style_text_font(test_font("montserratMedium", 8), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_right_value.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_right_value.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_right_value.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_right_value.set_style_text_align(lv.TEXT_ALIGN.RIGHT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_right_value.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_right_value.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_right_value.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_right_value.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_right_value.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_right_value.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_label_soil_moisture_range
screen_auto_mode_label_soil_moisture_range = lv.label(screen_auto_mode)
screen_auto_mode_label_soil_moisture_range.set_text("Soil moisture range")
screen_auto_mode_label_soil_moisture_range.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_label_soil_moisture_range.set_width(lv.pct(100))
screen_auto_mode_label_soil_moisture_range.set_pos(42, 145)
screen_auto_mode_label_soil_moisture_range.set_size(150, 10)
# Set style for screen_auto_mode_label_soil_moisture_range, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_label_soil_moisture_range.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_range.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_range.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_range.set_style_text_font(test_font("montserratMedium", 8), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_range.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_range.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_range.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_range.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_range.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_range.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_range.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_range.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_range.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_range.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_slider_soil_moisture_range
screen_auto_mode_slider_soil_moisture_range = lv.slider(screen_auto_mode)
screen_auto_mode_slider_soil_moisture_range.set_range(0, 100)
screen_auto_mode_slider_soil_moisture_range.set_mode(lv.slider.MODE.RANGE)
screen_auto_mode_slider_soil_moisture_range.set_value(60, lv.ANIM.OFF)
screen_auto_mode_slider_soil_moisture_range.set_left_value(40, lv.ANIM.OFF);screen_auto_mode_slider_soil_moisture_range.set_pos(45, 165)
screen_auto_mode_slider_soil_moisture_range.set_size(150, 3)
# Set style for screen_auto_mode_slider_soil_moisture_range, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_slider_soil_moisture_range.set_style_bg_opa(50, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_soil_moisture_range.set_style_bg_color(lv.color_hex(0x804000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_soil_moisture_range.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_soil_moisture_range.set_style_radius(8, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_soil_moisture_range.set_style_outline_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_soil_moisture_range.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_auto_mode_slider_soil_moisture_range, Part: lv.PART.INDICATOR, State: lv.STATE.DEFAULT.
screen_auto_mode_slider_soil_moisture_range.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_auto_mode_slider_soil_moisture_range.set_style_bg_color(lv.color_hex(0x804000), lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_auto_mode_slider_soil_moisture_range.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_auto_mode_slider_soil_moisture_range.set_style_radius(8, lv.PART.INDICATOR|lv.STATE.DEFAULT)

# Set style for screen_auto_mode_slider_soil_moisture_range, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_auto_mode_slider_soil_moisture_range.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_auto_mode_slider_soil_moisture_range.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_auto_mode_slider_soil_moisture_range.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_auto_mode_slider_soil_moisture_range.set_style_radius(8, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_auto_mode_label_soil_moisture_left_value
screen_auto_mode_label_soil_moisture_left_value = lv.label(screen_auto_mode)
screen_auto_mode_label_soil_moisture_left_value.set_text("40%")
screen_auto_mode_label_soil_moisture_left_value.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_label_soil_moisture_left_value.set_width(lv.pct(100))
screen_auto_mode_label_soil_moisture_left_value.set_pos(20, 162)
screen_auto_mode_label_soil_moisture_left_value.set_size(20, 10)
# Set style for screen_auto_mode_label_soil_moisture_left_value, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_label_soil_moisture_left_value.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_left_value.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_left_value.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_left_value.set_style_text_font(test_font("montserratMedium", 8), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_left_value.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_left_value.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_left_value.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_left_value.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_left_value.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_left_value.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_left_value.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_left_value.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_left_value.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_soil_moisture_left_value.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_img_soil_moisture
screen_auto_mode_img_soil_moisture = lv.image(screen_auto_mode)
screen_auto_mode_img_soil_moisture.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Soil_Moisture_Auto_Mode_15_15.png"))
screen_auto_mode_img_soil_moisture.add_flag(lv.obj.FLAG.CLICKABLE)
screen_auto_mode_img_soil_moisture.set_pivot(50,50)
screen_auto_mode_img_soil_moisture.set_rotation(0)
screen_auto_mode_img_soil_moisture.set_pos(20, 142)
screen_auto_mode_img_soil_moisture.set_size(15, 15)
# Set style for screen_auto_mode_img_soil_moisture, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_img_soil_moisture.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_img_light_intensity
screen_auto_mode_img_light_intensity = lv.image(screen_auto_mode)
screen_auto_mode_img_light_intensity.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Ligth_Intensity_Auto_Mode_15_15.png"))
screen_auto_mode_img_light_intensity.add_flag(lv.obj.FLAG.CLICKABLE)
screen_auto_mode_img_light_intensity.set_pivot(50,50)
screen_auto_mode_img_light_intensity.set_rotation(0)
screen_auto_mode_img_light_intensity.set_pos(20, 190)
screen_auto_mode_img_light_intensity.set_size(15, 15)
# Set style for screen_auto_mode_img_light_intensity, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_img_light_intensity.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_label_light_intensity_range
screen_auto_mode_label_light_intensity_range = lv.label(screen_auto_mode)
screen_auto_mode_label_light_intensity_range.set_text("Light intensity range")
screen_auto_mode_label_light_intensity_range.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_label_light_intensity_range.set_width(lv.pct(100))
screen_auto_mode_label_light_intensity_range.set_pos(42, 193)
screen_auto_mode_label_light_intensity_range.set_size(150, 10)
# Set style for screen_auto_mode_label_light_intensity_range, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_label_light_intensity_range.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_range.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_range.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_range.set_style_text_font(test_font("montserratMedium", 8), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_range.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_range.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_range.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_range.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_range.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_range.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_range.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_range.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_range.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_range.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_slider_light_intensity_range
screen_auto_mode_slider_light_intensity_range = lv.slider(screen_auto_mode)
screen_auto_mode_slider_light_intensity_range.set_range(0, 1000)
screen_auto_mode_slider_light_intensity_range.set_mode(lv.slider.MODE.RANGE)
screen_auto_mode_slider_light_intensity_range.set_value(600, lv.ANIM.OFF)
screen_auto_mode_slider_light_intensity_range.set_left_value(200, lv.ANIM.OFF);screen_auto_mode_slider_light_intensity_range.set_pos(52, 214)
screen_auto_mode_slider_light_intensity_range.set_size(140, 3)
# Set style for screen_auto_mode_slider_light_intensity_range, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_slider_light_intensity_range.set_style_bg_opa(50, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_light_intensity_range.set_style_bg_color(lv.color_hex(0xffff00), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_light_intensity_range.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_light_intensity_range.set_style_radius(8, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_light_intensity_range.set_style_outline_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_slider_light_intensity_range.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_auto_mode_slider_light_intensity_range, Part: lv.PART.INDICATOR, State: lv.STATE.DEFAULT.
screen_auto_mode_slider_light_intensity_range.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_auto_mode_slider_light_intensity_range.set_style_bg_color(lv.color_hex(0xffff00), lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_auto_mode_slider_light_intensity_range.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.DEFAULT)
screen_auto_mode_slider_light_intensity_range.set_style_radius(8, lv.PART.INDICATOR|lv.STATE.DEFAULT)

# Set style for screen_auto_mode_slider_light_intensity_range, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_auto_mode_slider_light_intensity_range.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_auto_mode_slider_light_intensity_range.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_auto_mode_slider_light_intensity_range.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_auto_mode_slider_light_intensity_range.set_style_radius(8, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_auto_mode_label_light_intensity_left_value
screen_auto_mode_label_light_intensity_left_value = lv.label(screen_auto_mode)
screen_auto_mode_label_light_intensity_left_value.set_text("200 lx")
screen_auto_mode_label_light_intensity_left_value.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_label_light_intensity_left_value.set_width(lv.pct(100))
screen_auto_mode_label_light_intensity_left_value.set_pos(20, 211)
screen_auto_mode_label_light_intensity_left_value.set_size(25, 10)
# Set style for screen_auto_mode_label_light_intensity_left_value, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_label_light_intensity_left_value.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_left_value.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_left_value.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_left_value.set_style_text_font(test_font("montserratMedium", 8), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_left_value.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_left_value.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_left_value.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_left_value.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_left_value.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_left_value.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_left_value.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_left_value.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_left_value.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_left_value.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_label_light_intensity_right_value
screen_auto_mode_label_light_intensity_right_value = lv.label(screen_auto_mode)
screen_auto_mode_label_light_intensity_right_value.set_text("600 lx")
screen_auto_mode_label_light_intensity_right_value.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_label_light_intensity_right_value.set_width(lv.pct(100))
screen_auto_mode_label_light_intensity_right_value.set_pos(195, 211)
screen_auto_mode_label_light_intensity_right_value.set_size(30, 10)
# Set style for screen_auto_mode_label_light_intensity_right_value, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_label_light_intensity_right_value.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_right_value.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_right_value.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_right_value.set_style_text_font(test_font("montserratMedium", 8), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_right_value.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_right_value.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_right_value.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_right_value.set_style_text_align(lv.TEXT_ALIGN.RIGHT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_right_value.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_right_value.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_right_value.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_right_value.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_right_value.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_intensity_right_value.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_img_light_color
screen_auto_mode_img_light_color = lv.image(screen_auto_mode)
screen_auto_mode_img_light_color.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Light_Color_Auto_Mode_15_15.png"))
screen_auto_mode_img_light_color.add_flag(lv.obj.FLAG.CLICKABLE)
screen_auto_mode_img_light_color.set_pivot(50,50)
screen_auto_mode_img_light_color.set_rotation(0)
screen_auto_mode_img_light_color.set_pos(20, 239)
screen_auto_mode_img_light_color.set_size(15, 15)
# Set style for screen_auto_mode_img_light_color, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_img_light_color.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_auto_mode_label_light_color_value
screen_auto_mode_label_light_color_value = lv.label(screen_auto_mode)
screen_auto_mode_label_light_color_value.set_text("Light Color value")
screen_auto_mode_label_light_color_value.set_long_mode(lv.label.LONG.WRAP)
screen_auto_mode_label_light_color_value.set_width(lv.pct(100))
screen_auto_mode_label_light_color_value.set_pos(42, 242)
screen_auto_mode_label_light_color_value.set_size(150, 10)
# Set style for screen_auto_mode_label_light_color_value, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_auto_mode_label_light_color_value.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_color_value.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_color_value.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_color_value.set_style_text_font(test_font("montserratMedium", 8), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_color_value.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_color_value.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_color_value.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_color_value.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_color_value.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_color_value.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_color_value.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_color_value.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_color_value.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_auto_mode_label_light_color_value.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_auto_mode.update_layout()
# Create screen_ai_pilot_mode
screen_ai_pilot_mode = lv.obj()
screen_ai_pilot_mode.set_size(240, 320)
screen_ai_pilot_mode.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_ai_pilot_mode, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_ai_pilot_mode.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_ai_pilot_mode_img_1
screen_ai_pilot_mode_img_1 = lv.image(screen_ai_pilot_mode)
screen_ai_pilot_mode_img_1.set_src(load_image(r"E:\Project\NXP\GUI-Guider-Projects\SmartGreenhouse\generated\MicroPython\Auto_Mode_Background_240_320.png"))
screen_ai_pilot_mode_img_1.add_flag(lv.obj.FLAG.CLICKABLE)
screen_ai_pilot_mode_img_1.set_pivot(50,50)
screen_ai_pilot_mode_img_1.set_rotation(0)
screen_ai_pilot_mode_img_1.set_pos(0, 0)
screen_ai_pilot_mode_img_1.set_size(240, 320)
# Set style for screen_ai_pilot_mode_img_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_ai_pilot_mode_img_1.set_style_image_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

screen_ai_pilot_mode.update_layout()

def screen_home_img_control_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.screen_load_anim(screen_control, lv.SCR_LOAD_ANIM.FADE_ON, 250, 50, False)
screen_home_img_control.add_event_cb(lambda e: screen_home_img_control_event_handler(e), lv.EVENT.ALL, None)

def screen_home_img_overview_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.screen_load_anim(screen_overview, lv.SCR_LOAD_ANIM.FADE_ON, 250, 50, False)
screen_home_img_overview.add_event_cb(lambda e: screen_home_img_overview_event_handler(e), lv.EVENT.ALL, None)

def screen_home_img_setting_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.screen_load_anim(screen_setting, lv.SCR_LOAD_ANIM.FADE_ON, 250, 50, False)
screen_home_img_setting.add_event_cb(lambda e: screen_home_img_setting_event_handler(e), lv.EVENT.ALL, None)

def screen_home_img_trend_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.screen_load_anim(screen_trend, lv.SCR_LOAD_ANIM.FADE_ON, 250, 50, False)
screen_home_img_trend.add_event_cb(lambda e: screen_home_img_trend_event_handler(e), lv.EVENT.ALL, None)

def screen_overview_event_handler(e):
    code = e.get_code()
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.TOP == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_home, lv.SCR_LOAD_ANIM.FADE_ON, 200, 100, False)
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.LEFT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_control, lv.SCR_LOAD_ANIM.MOVE_LEFT, 300, 0, False)
screen_overview.add_event_cb(lambda e: screen_overview_event_handler(e), lv.EVENT.ALL, None)

def screen_control_event_handler(e):
    code = e.get_code()
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.TOP == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_home, lv.SCR_LOAD_ANIM.FADE_ON, 200, 200, False)
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.LEFT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_trend, lv.SCR_LOAD_ANIM.MOVE_LEFT, 300, 0, False)
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.RIGHT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_overview, lv.SCR_LOAD_ANIM.MOVE_RIGHT, 300, 0, False)
screen_control.add_event_cb(lambda e: screen_control_event_handler(e), lv.EVENT.ALL, None)

def screen_control_btn_manual_mode_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.screen_load_anim(screen_manual_mode, lv.SCR_LOAD_ANIM.MOVE_LEFT, 200, 200, False)
screen_control_btn_manual_mode.add_event_cb(lambda e: screen_control_btn_manual_mode_event_handler(e), lv.EVENT.ALL, None)

def screen_control_btn_auto_mode_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.screen_load_anim(screen_auto_mode, lv.SCR_LOAD_ANIM.MOVE_LEFT, 200, 200, False)
screen_control_btn_auto_mode.add_event_cb(lambda e: screen_control_btn_auto_mode_event_handler(e), lv.EVENT.ALL, None)

def screen_control_btn_ai_pilot_mode_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.screen_load_anim(screen_ai_pilot_mode, lv.SCR_LOAD_ANIM.MOVE_LEFT, 200, 200, False)
screen_control_btn_ai_pilot_mode.add_event_cb(lambda e: screen_control_btn_ai_pilot_mode_event_handler(e), lv.EVENT.ALL, None)

def screen_trend_event_handler(e):
    code = e.get_code()
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.TOP == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_home, lv.SCR_LOAD_ANIM.FADE_ON, 200, 200, False)
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.LEFT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_setting, lv.SCR_LOAD_ANIM.OVER_LEFT, 200, 200, False)
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.RIGHT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_control, lv.SCR_LOAD_ANIM.MOVE_RIGHT, 200, 200, False)
screen_trend.add_event_cb(lambda e: screen_trend_event_handler(e), lv.EVENT.ALL, None)

def screen_setting_event_handler(e):
    code = e.get_code()
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.TOP == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_home, lv.SCR_LOAD_ANIM.FADE_ON, 200, 200, False)
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.RIGHT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_trend, lv.SCR_LOAD_ANIM.MOVE_RIGHT, 200, 200, False)
screen_setting.add_event_cb(lambda e: screen_setting_event_handler(e), lv.EVENT.ALL, None)

def screen_manual_mode_event_handler(e):
    code = e.get_code()
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.RIGHT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_control, lv.SCR_LOAD_ANIM.MOVE_RIGHT, 200, 200, False)
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.TOP == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_home, lv.SCR_LOAD_ANIM.FADE_ON, 200, 200, False)
screen_manual_mode.add_event_cb(lambda e: screen_manual_mode_event_handler(e), lv.EVENT.ALL, None)

def screen_manual_mode_slider_ventilation_fan_speed_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.VALUE_CHANGED):
        pass
        

screen_manual_mode_slider_ventilation_fan_speed.add_event_cb(lambda e: screen_manual_mode_slider_ventilation_fan_speed_event_handler(e), lv.EVENT.ALL, None)

def screen_manual_mode_slider_brightness_value_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.VALUE_CHANGED):
        pass
        

screen_manual_mode_slider_brightness_value.add_event_cb(lambda e: screen_manual_mode_slider_brightness_value_event_handler(e), lv.EVENT.ALL, None)

def screen_auto_mode_event_handler(e):
    code = e.get_code()
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.RIGHT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_control, lv.SCR_LOAD_ANIM.MOVE_RIGHT, 200, 200, False)
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.TOP == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_home, lv.SCR_LOAD_ANIM.FADE_ON, 200, 200, False)
screen_auto_mode.add_event_cb(lambda e: screen_auto_mode_event_handler(e), lv.EVENT.ALL, None)

def screen_auto_mode_slider_temperature_range_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.VALUE_CHANGED):
        pass
        

screen_auto_mode_slider_temperature_range.add_event_cb(lambda e: screen_auto_mode_slider_temperature_range_event_handler(e), lv.EVENT.ALL, None)

def screen_auto_mode_slider_humidity_range_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.VALUE_CHANGED):
        pass
        

screen_auto_mode_slider_humidity_range.add_event_cb(lambda e: screen_auto_mode_slider_humidity_range_event_handler(e), lv.EVENT.ALL, None)

def screen_auto_mode_slider_soil_moisture_range_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.VALUE_CHANGED):
        pass
        

screen_auto_mode_slider_soil_moisture_range.add_event_cb(lambda e: screen_auto_mode_slider_soil_moisture_range_event_handler(e), lv.EVENT.ALL, None)

def screen_auto_mode_slider_light_intensity_range_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.VALUE_CHANGED):
        pass
        

screen_auto_mode_slider_light_intensity_range.add_event_cb(lambda e: screen_auto_mode_slider_light_intensity_range_event_handler(e), lv.EVENT.ALL, None)

def screen_ai_pilot_mode_event_handler(e):
    code = e.get_code()
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.RIGHT == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_control, lv.SCR_LOAD_ANIM.MOVE_RIGHT, 200, 200, False)
    indev = lv.indev_active()
    gestureDir = lv.DIR.NONE
    if indev is not None: gestureDir = indev.get_gesture_dir()
    if (code == lv.EVENT.GESTURE and lv.DIR.TOP == gestureDir):
        if indev is not None: indev.wait_release()
        pass
        lv.screen_load_anim(screen_home, lv.SCR_LOAD_ANIM.FADE_ON, 200, 200, False)
screen_ai_pilot_mode.add_event_cb(lambda e: screen_ai_pilot_mode_event_handler(e), lv.EVENT.ALL, None)

# content from custom.py

# Load the default screen
lv.screen_load(screen_home)

if __name__ == '__main__':
    while True:
        lv.task_handler()
        time.sleep_ms(5)
