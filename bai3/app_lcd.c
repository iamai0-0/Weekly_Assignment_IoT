/*
 * app_lcd.c - Digital Clock Application
 *
 *  Created on: Nov 4, 2024
 *      Author: Phat_Dang
 */
#include <stdio.h>
#include <string.h>

#include "sl_board_control.h"
#include "em_assert.h"
#include "glib.h"
#include "dmd.h"
#include "sl_sleeptimer.h"

#ifndef LCD_MAX_LINES
#define LCD_MAX_LINES      11
#endif

/*******************************************************************************
 ***************************  LOCAL VARIABLES   ********************************
 ******************************************************************************/
static GLIB_Context_t glibContext;

// Biến lưu thời gian
static uint8_t hours = 0;
static uint8_t minutes = 0;
static uint8_t seconds = 0;

// Timer handle
// static sl_sleeptimer_timer_handle_t clock_timer;

// Flag để kiểm tra xem có cần cập nhật màn hình không
static bool update_display = false;

/*******************************************************************************
 **************************   LOCAL FUNCTIONS   ********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Hiển thị đồng hồ lên LCD
 ******************************************************************************/
static void display_clock(void)
{
  char time_str[16];

  // Format chuỗi thời gian HH:MM:SS
  snprintf(time_str, sizeof(time_str), "%02d:%02d:%02d", hours, minutes, seconds);

  // Vẽ thời gian (dòng 3, căn giữa)
  GLIB_drawStringOnLine(&glibContext,
                        time_str,
                        3,
                        GLIB_ALIGN_CENTER,
                        0,
                        0,
                        true);
  // Cập nhật màn hình
  DMD_updateDisplay();

  // Tăng giây
  seconds++;
    // Xử lý tràn giây
  if (seconds >= 60) {
    seconds = 0;
    minutes++;

    // Xử lý tràn phút
    if (minutes >= 60) {
      minutes = 0;
      hours++;

      // Xử lý tràn giờ (24h format)
      if (hours >= 24) {
        hours = 0;
      }
    }
  }

}

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Khởi tạo ứng dụng đồng hồ
 ******************************************************************************/
void memlcd_app_init(void)
{
  uint32_t status;

  /* Enable the memory lcd */
  status = sl_board_enable_display();
  EFM_ASSERT(status == SL_STATUS_OK);

  /* Initialize the DMD support for memory lcd display */
  status = DMD_init(0);
  EFM_ASSERT(status == DMD_OK);

  /* Initialize the glib context */
  status = GLIB_contextInit(&glibContext);
  EFM_ASSERT(status == GLIB_OK);

  glibContext.backgroundColor = White;
  glibContext.foregroundColor = Black;

  /* Fill lcd with background color */
  GLIB_clear(&glibContext);

  /* Use Narrow font */
  GLIB_setFont(&glibContext, (GLIB_Font_t *) &GLIB_FontNarrow6x8);

  // Khởi tạo thời gian ban đầu
    hours = 11;
    minutes = 11;
    seconds = 11;


  // Clear màn hình
    GLIB_clear(&glibContext);

    // Vẽ tiêu đề
    GLIB_drawStringOnLine(&glibContext,
                          "DONG HO",
                          0,
                          GLIB_ALIGN_CENTER,
                          0,
                          0,
                          true);

    GLIB_drawStringOnLine(&glibContext,
                          "22200101",
                          6,
                          GLIB_ALIGN_CENTER,
                          0,
                          0,
                          true);

    GLIB_drawStringOnLine(&glibContext,
                              "22200108",
                              8,
                              GLIB_ALIGN_CENTER,
                              0,
                              0,
                              true);

    EFM_ASSERT(status == SL_STATUS_OK);
  }


/***************************************************************************//**
 * @brief
 *   Hàm xử lý chính - được gọi trong vòng lặp main
 ******************************************************************************/
void memlcd_app_process_action(void)
{
  display_clock();
}

/***************************************************************************//**
 * @brief
 *   Hàm thiết lập thời gian (tùy chọn)
 *
 * @param[in] h
 *   Giờ (0-23)
 * @param[in] m
 *   Phút (0-59)
 * @param[in] s
 *   Giây (0-59)
 ******************************************************************************/
void set_time(uint8_t h, uint8_t m, uint8_t s)
{
  if (h < 24 && m < 60 && s < 60) {
    hours = h;
    minutes = m;
    seconds = s;
    update_display = true;
  }
}

/***************************************************************************//**
 * @brief
 *   Hàm lấy thời gian hiện tại (tùy chọn)
 *
 * @param[out] h
 *   Con trỏ đến biến lưu giờ
 * @param[out] m
 *   Con trỏ đến biến lưu phút
 * @param[out] s
 *   Con trỏ đến biến lưu giây
 ******************************************************************************/
void get_time(uint8_t *h, uint8_t *m, uint8_t *s)
{
  if (h != NULL) *h = hours;
  if (m != NULL) *m = minutes;
  if (s != NULL) *s = seconds;
}
