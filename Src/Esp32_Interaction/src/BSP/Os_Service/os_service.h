#pragma once

// #include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

namespace OsService {

void safe_delay_ms(uint32_t ms);

void safe_printf(const char* format, ...);

}