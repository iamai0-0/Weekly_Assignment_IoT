/****************************************************************************
 * @file main.c
 * @brief main() function.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include "app.h"
#include "sl_component_catalog.h"
#include "sl_system_init.h"
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif
#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "sl_system_kernel.h"
#else  // SL_CATALOG_KERNEL_PRESENT
#include "sl_system_process_action.h"
#endif  // SL_CATALOG_KERNEL_PRESENT

#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"

// --- Added for timing measurement ---
#include "em_timer.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
// ------------------------------------

#define BSP_TXPORT gpioPortA
#define BSP_RXPORT gpioPortA
#define BSP_TXPIN 5
#define BSP_RXPIN 6
#define BSP_ENABLE_PORT gpioPortD
#define BSP_ENABLE_PIN 4

// BUTTON AND LEDS
#define BSP_GPIO_LEDS
#define BSP_GPIO_LED0_PORT gpioPortD
#define BSP_GPIO_LED0_PIN 2
#define BSP_GPIO_LED1_PORT gpioPortD
#define BSP_GPIO_LED1_PIN 3
#define BSP_GPIO_PB0_PORT gpioPortB
#define BSP_GPIO_PB0_PIN 0
#define BSP_GPIO_PB1_PORT gpioPortB
#define BSP_GPIO_PB1_PIN 1

void UART_TransmitMultiple(USART_TypeDef* uart, uint8_t* data, uint32_t length);
void UART_ReceiveMultiple(USART_TypeDef* uart, uint8_t* buffer, uint32_t length);
void initGPIO(void);
void initUSART0(void);
void initLED_BUTTON();
// void GPIO_PinOutToggle(GPIO_Port_TypeDef port, unsigned int pin)
// void GPIO_PinOutSet(GPIO_Port_TypeDef port, unsigned int pin)

// --- Added for timing measurement ---
void initTimer0(void);
uint32_t timer_get_us(void);
// ------------------------------------

/**************************************************************************/ /**
                                                                              * @brief
                                                                              *    GPIO initialization
                                                                              *****************************************************************************/
void initGPIO(void) {
    // Configure the USART TX pin to the board controller as an output
    GPIO_PinModeSet(BSP_TXPORT, BSP_TXPIN, gpioModePushPull, 1);

    // Configure the USART RX pin to the board controller as an input
    GPIO_PinModeSet(BSP_RXPORT, BSP_RXPIN, gpioModeInput, 0);

    /*
     * Configure the BCC_ENABLE pin as output and set high.  This enables
     * the virtual COM port (VCOM) connection to the board controller and
     * permits serial port traffic over the debug connection to the host
     * PC.
     *
     * To disable the VCOM connection and use the pins on the kit
     * expansion (EXP) header, comment out the following line.
     */
    GPIO_PinModeSet(BSP_ENABLE_PORT, BSP_ENABLE_PIN, gpioModePushPull, 1);

#if defined(BSP_GPIO_LEDS)
    // Initialize LED and BUTTON
    initLED_BUTTON();
#endif
}

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
    uint8_t* buff = (uint8_t*)"Button 0 Pressed!\n";
    UART_TransmitMultiple(USART0, buff, strlen((char*)buff));
    GPIO_PinOutToggle(BSP_GPIO_LED0_PORT, BSP_GPIO_LED0_PIN);
}
// Button 1 -> LED1: Green
void GPIO_ODD_IRQHandler(void) {
    // Clear all odd pin interrupt flags
    GPIO_IntClear(0xAAAA);
    // Code here
    uint8_t* buff = (uint8_t*)"Button 1 Pressed!\n";
    UART_TransmitMultiple(USART0, buff, strlen((char*)buff));
    GPIO_PinOutToggle(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN);
}

/***************************************************************************
 * @brief
 *    USART0 initialization
 *****************************************************************************/
void initUSART0(void) {
    // Default asynchronous initializer (115.2 Kbps, 8N1, no flow control)
    // USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
    USART_InitAsync_TypeDef init;

    init.enable = usartEnable;
    init.refFreq = 0;
    init.baudrate = 115200;
    init.oversampling = usartOVS16;
    init.databits = usartDatabits8;
    init.parity = USART_FRAME_PARITY_NONE;
    init.stopbits = usartStopbits1;

    init.mvdis = false;
    init.prsRxEnable = false;
    init.prsRxCh = 0;

    init.autoCsEnable = false;
    init.csInv = false;
    init.autoCsHold = 0;
    init.autoCsSetup = 0;
    init.hwFlowControl = usartHwFlowControlNone;

    // Route USART0 TX and RX to the board controller TX and RX pins
    GPIO->USARTROUTE[0].TXROUTE = (BSP_TXPORT << _GPIO_USART_TXROUTE_PORT_SHIFT) | (BSP_TXPIN << _GPIO_USART_TXROUTE_PIN_SHIFT);
    GPIO->USARTROUTE[0].RXROUTE = (BSP_RXPORT << _GPIO_USART_RXROUTE_PORT_SHIFT) | (BSP_RXPIN << _GPIO_USART_RXROUTE_PIN_SHIFT);

    // Enable RX and TX signals now that they have been routed
    GPIO->USARTROUTE[0].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN | GPIO_USART_ROUTEEN_TXPEN;

    // Configure and enable USART0
    USART_InitAsync(USART0, &init);
}

// --- Added for timing measurement ---
void initTimer0(void) {
    CMU_ClockEnable(cmuClock_TIMER0, true);
    TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
    timerInit.prescale = timerPrescale1;
    timerInit.enable = true;
    TIMER_Init(TIMER0, &timerInit);
    TIMER_TopSet(TIMER0, 0xFFFF);
}

uint32_t timer_get_us(void) {
    return TIMER_CounterGet(TIMER0);
}
// ------------------------------------

void UART_TransmitMultiple(USART_TypeDef* uart, uint8_t* data, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        USART_Tx(uart, data[i]);
    }
}

void UART_ReceiveMultiple(USART_TypeDef* uart, uint8_t* buffer, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        buffer[i] = USART_Rx(uart);
    }
}

void handle_command(const char* cmd) {
    // Example command: "led1:on led2:off"
    bool led1_on = false;
    bool led2_on = false;

    if (strstr(cmd, "led1:on")) {
        GPIO_PinOutSet(BSP_GPIO_LED0_PORT, BSP_GPIO_LED0_PIN);
        led1_on = true;
    }
    if (strstr(cmd, "led1:off")) {
        GPIO_PinOutClear(BSP_GPIO_LED0_PORT, BSP_GPIO_LED0_PIN);
        led1_on = false;
    }
    if (strstr(cmd, "led2:on")) {
        GPIO_PinOutSet(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN);
        led2_on = true;
    }
    if (strstr(cmd, "led2:off")) {
        GPIO_PinOutClear(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN);
        led2_on = false;
    }

    // Echo result to terminal
    char response[64];
    snprintf(response, sizeof(response),
             "Led1:%s, Led2:%s\n",
             led1_on ? "on" : "off",
             led2_on ? "on" : "off");
    UART_TransmitMultiple(USART0, (uint8_t*)response, strlen(response));
}

int main(void) {
    // Initialize Silicon Labs device, system, service(s) and protocol stack(s).
    // Note that if the kernel is present, processing task(s) will be created by
    // this call.
    sl_system_init();

    // Initialize the application. For example, create periodic timer(s) or
    // task(s) if the kernel is present.
    app_init();

#if defined(SL_CATALOG_KERNEL_PRESENT)
    // Start the kernel. Task(s) created in app_init() will start running.
    sl_system_kernel_start();
#else  // SL_CATALOG_KERNEL_PRESENT

    uint8_t buffer[100];
    uint8_t index = 0, rx_flag = 0;

    // --- Added for timing measurement ---
    uint32_t start_ticks = 0, end_ticks = 0;
    initTimer0();
    // ------------------------------------

    // Initialize GPIO and USART0
    initGPIO();
    initUSART0();

    while (1) {
        // Do not remove this call: Silicon Labs components process action routine
        // must be called from the super loop.
        sl_system_process_action();

        // Application process.
        app_process_action();

        // Receive 1 byte
        uint8_t ch = USART_Rx(USART0);

        // --- Added for timing measurement ---
        if (index == 0) {
            start_ticks = timer_get_us(); // start time when first byte received
        }
        // ------------------------------------

        // Store until newline or buffer full
        if (ch == '\r' || ch == '\n') {
            buffer[index] = '\0';
            rx_flag = true;

            // --- Added for timing measurement ---
            end_ticks = timer_get_us(); // stop time when newline received
            // ------------------------------------
        } else if (index < 100 - 1) {
            buffer[index++] = ch;
        }

        if (rx_flag) {
            rx_flag = false;

            // --- Added for timing measurement ---
            uint32_t elapsed_us = (end_ticks >= start_ticks)
                                  ? (end_ticks - start_ticks)
                                  : (0xFFFF - start_ticks + end_ticks);
            float elapsed_ms = elapsed_us / 1000.0f;

            char time_msg[80];
            snprintf(time_msg, sizeof(time_msg),
                     "Message received in %lu ms\r\n", elapsed_ms);
            UART_TransmitMultiple(USART0, (uint8_t*)time_msg, strlen(time_msg));
            // ------------------------------------

            index = 0;
            handle_command((char*)buffer);
        }

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
        // Let the CPU go to sleep if the system allows it.
        sl_power_manager_sleep();
#endif
    }
#endif  // SL_CATALOG_KERNEL_PRESENT
}
