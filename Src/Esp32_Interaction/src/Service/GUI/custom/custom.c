/*
* Copyright 2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/


/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lvgl.h"
#include "custom.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// 引入外部定义的队列句柄（稍后我们在 main.cpp 中定义它）
extern QueueHandle_t slider_queue;

void custom_slider_event_send(int value) {
    // 如果队列已经创建，就把数值用非阻塞的方式发进队列里
    // 这里的 0 表示如果队列满了，不等待，直接丢弃（防止把 UI 卡死）
    if (slider_queue != NULL) {
        xQueueSend(slider_queue, &value, 0);
    }
}
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**
 * Create a demo application
 */

void custom_init(lv_ui *ui)
{
    /* Add your codes here */
}

