/****************************************************************************
 * @file
 * @brief Core application logic.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#include "app.h"

#include "app_assert.h"
#include "app_log.h"
#include "app_timer.h"
#include "custom_adv.h"
#include "em_cmu.h"
#include "em_common.h"
#include "em_gpio.h"
#include "gatt_db.h"
#include "sl_bluetooth.h"
#include "sl_sleeptimer.h"

// === CHỌN BÀI TẬP TẠI ĐÂY ===
#define LAB_EXERCISE_1
// #define LAB_EXERCISE_2

CustomAdv_t sData;
//static app_timer_t update_timer;
static uint8_t advertising_set_handle = 0xff;
#ifdef LAB_EXERCISE_1
static uint8_t led0_state = 0, led1_state = 0;
#endif

// BUTTON AND LEDS
#define BSP_GPIO_LED0_PORT gpioPortD
#define BSP_GPIO_LED0_PIN 2
#define BSP_GPIO_LED1_PORT gpioPortD
#define BSP_GPIO_LED1_PIN 3
#define BSP_GPIO_PB0_PORT gpioPortB
#define BSP_GPIO_PB0_PIN 0
#define BSP_GPIO_PB1_PORT gpioPortB
#define BSP_GPIO_PB1_PIN 1

#ifdef LAB_EXERCISE_1
void initLED_BUTTON() {
    // Enable GPIO clock
    CMU_ClockEnable(cmuClock_GPIO, true);
    // Configure PB0 and PB1 as input with glitch filter enabled
    GPIO_PinModeSet(BSP_GPIO_PB0_PORT, BSP_GPIO_PB0_PIN,
                    gpioModeInputPullFilter, 1);
    GPIO_PinModeSet(BSP_GPIO_PB1_PORT, BSP_GPIO_PB1_PIN,
                    gpioModeInputPullFilter, 1);
    // Configure LED0 and LED1 as output
    GPIO_PinModeSet(BSP_GPIO_LED0_PORT, BSP_GPIO_LED0_PIN,
                    gpioModePushPull, 0);
    GPIO_PinModeSet(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN,
                    gpioModePushPull, 0);
    // Enable IRQ for even numbered GPIO pins
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);
    // Enable IRQ for odd numbered GPIO pins
    NVIC_EnableIRQ(GPIO_ODD_IRQn);
    // Enable falling-edge interrupts for PB pins
    GPIO_ExtIntConfig(BSP_GPIO_PB0_PORT,
                      BSP_GPIO_PB0_PIN, BSP_GPIO_PB0_PIN, 0, 1, true);
    GPIO_ExtIntConfig(BSP_GPIO_PB1_PORT, BSP_GPIO_PB1_PIN,
                      BSP_GPIO_PB1_PIN, 0, 1, true);
}

// Button 0 -> LED0: Red
void GPIO_EVEN_IRQHandler(void) {
    // Clear all even pin interrupt flags
    GPIO_IntClear(0x5555);
    // Code here
    // uint8_t* buff = (uint8_t*)"Button 0 Pressed!\n\r";
    // UART_TransmitMultiple(USART0, buff, strlen((char*)buff));
    app_log("Button 0 Pressed!\r\n");
    led0_state = !led0_state;
    // GPIO_PinOutToggle(BSP_GPIO_LED0_PORT, BSP_GPIO_LED0_PIN);
    if (led0_state) {
        GPIO_PinOutSet(BSP_GPIO_LED0_PORT, BSP_GPIO_LED0_PIN);
        app_log("LED0 ON\r\n");
    } else {
        GPIO_PinOutClear(BSP_GPIO_LED0_PORT, BSP_GPIO_LED0_PIN);
        app_log("LED0 OFF\r\n");
    }
}

// Button 1 -> LED1: Green
void GPIO_ODD_IRQHandler(void) {
    // Clear all odd pin interrupt flags
    GPIO_IntClear(0xAAAA);
    // Code here
    // uint8_t* buff = (uint8_t*)"Button 1 Pressed!\n\r";
    // UART_TransmitMultiple(USART0, buff, strlen((char*)buff));
    // GPIO_PinOutToggle(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN);
    app_log("Button 1 Pressed!\r\n");
    led1_state = !led1_state;
    if (led1_state) {
        GPIO_PinOutSet(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN);
        app_log("LED1 ON\r\n");
    } else {
        GPIO_PinOutClear(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN);
        app_log("LED1 OFF\r\n");
    }
}
#else

static uint8_t hours = 0;
static uint8_t minutes = 0;
static uint8_t seconds = 0;
static char current_time[9] = "00:00:00";
static app_timer_t uptime_timer;

static void update_time_cb(app_timer_t *timer, void *data)
{
  (void)timer; (void)data;

  // Tăng giây
  seconds++;

  // Xử lý tràn
  if (seconds >= 60) {
    seconds = 0;
    minutes++;
  }
  if (minutes >= 60) {
    minutes = 0;
    hours++;
  }
  if (hours >= 24) {
    hours = 0;  // Reset về 00 nếu cần (tùy yêu cầu)
  }

  // Tạo chuỗi HH:MM:SS
  sprintf(current_time, "%02d:%02d:%02d", hours, minutes, seconds);

  // Cập nhật quảng bá
  update_adv_data(&sData, advertising_set_handle, current_time);

  // In log
  app_log("Uptime: %s\r\n", current_time);
}

#endif

SL_WEAK void app_init(void) {
  initLED_BUTTON();

#ifdef LAB_EXERCISE_2
    sl_sleeptimer_init();
    app_timer_start(&update_timer, 1000, update_time_cb, NULL, true);
#endif
}

SL_WEAK void app_process_action(void) {}

void sl_bt_on_event(sl_bt_msg_t* evt) {
    sl_status_t sc;
    bd_addr address;
    uint8_t address_type;
    uint8_t system_id[8];

    switch (SL_BT_MSG_ID(evt->header)) {
        case sl_bt_evt_system_boot_id:
            sc = sl_bt_system_get_identity_address(&address, &address_type);
            app_assert_status(sc);

            system_id[0] = address.addr[5];
            system_id[1] = address.addr[4];
            system_id[2] = address.addr[3];
            system_id[3] = 0xFF;
            system_id[4] = 0xFE;
            system_id[5] = address.addr[2];
            system_id[6] = address.addr[1];
            system_id[7] = address.addr[0];

            sc = sl_bt_gatt_server_write_attribute_value(gattdb_system_id, 0, 8, system_id);
            app_assert_status(sc);

            sc = sl_bt_advertiser_create_set(&advertising_set_handle);
            app_assert_status(sc);

            sc = sl_bt_advertiser_set_timing(advertising_set_handle, 1600, 1600, 0, 0);
            app_assert_status(sc);

            sc = sl_bt_advertiser_set_channel_map(advertising_set_handle, 7);
            app_assert_status(sc);

#ifdef LAB_EXERCISE_1
            uint8_t init_data[2] = {0, 0};
            fill_adv_packet(&sData, FLAG, COMPANY_ID, init_data, "CustomADV");
#else
            fill_adv_packet(&sData, FLAG, COMPANY_ID, "00:00:00", "CustomADV");
#endif

            start_adv(&sData, advertising_set_handle);
            app_log("Advertising started\r\n");
            break;

        case sl_bt_evt_connection_closed_id:
            sc = sl_bt_legacy_advertiser_start(advertising_set_handle, sl_bt_advertiser_connectable_scannable);
            app_assert_status(sc);
            break;

        default:
            break;
    }
}
