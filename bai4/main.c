/****************************************************************************
 * @file main.c
 * @brief 3 Tasks: Select once at startup, stay in mode until reset
 ******************************************************************************/
#include "app.h"
#include "sl_component_catalog.h"
#include "sl_system_init.h"
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif
#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "sl_system_kernel.h"
#else
#include "sl_system_process_action.h"
#endif
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_ldma.h"
#include "em_timer.h"
#include "em_usart.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>      // rand(), srand()
#include <time.h>        // time()
/* --------------------------------------------------------------------- */
#define BSP_TXPORT   gpioPortA
#define BSP_TXPIN    5
#define BSP_RXPORT   gpioPortA
#define BSP_RXPIN    6
#define BSP_ENABLE_PORT gpioPortD
#define BSP_ENABLE_PIN  4

#define RX_LDMA_CHANNEL 0
#define TX_LDMA_CHANNEL 1
#define BUFLEN 1000

uint8_t outbuf[BUFLEN];
uint8_t inbuf[BUFLEN];
volatile uint8_t inbuf_dma;
volatile bool rx_done = false;
volatile bool tx_done = false;

// LDMA descriptor and transfer configuration structures for USART TX channel
LDMA_Descriptor_t ldmaTXDescriptor;
LDMA_TransferCfg_t ldmaTXConfig;

// LDMA descriptor and transfer configuration structures for USART RX channel
LDMA_Descriptor_t ldmaRXDescriptor;
LDMA_TransferCfg_t ldmaRXConfig;

typedef enum { MODE_MAX_FREQ, MODE_DMA, MODE_POLLED } AppMode;
static AppMode app_mode = MODE_MAX_FREQ;  // default

/* --------------------------------------------------------------------- */
static void UART_Transmit(const char *s)
{
    while (*s) {
        while (!(USART0->STATUS & USART_STATUS_TXBL)) ;
        USART_Tx(USART0, *s++);
    }
}

static void initGPIO(void)
{
    GPIO_PinModeSet(BSP_TXPORT, BSP_TXPIN, gpioModePushPull, 1);
    GPIO_PinModeSet(BSP_RXPORT, BSP_RXPIN, gpioModeInput, 0);
    GPIO_PinModeSet(BSP_ENABLE_PORT, BSP_ENABLE_PIN, gpioModePushPull, 1);
}

static void initUSART0(void)
{
    USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
    GPIO->USARTROUTE[0].TXROUTE = (BSP_TXPORT << _GPIO_USART_TXROUTE_PORT_SHIFT) | (BSP_TXPIN << _GPIO_USART_TXROUTE_PIN_SHIFT);
    GPIO->USARTROUTE[0].RXROUTE = (BSP_RXPORT << _GPIO_USART_RXROUTE_PORT_SHIFT) | (BSP_RXPIN << _GPIO_USART_TXROUTE_PIN_SHIFT);
    GPIO->USARTROUTE[0].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN | GPIO_USART_ROUTEEN_TXPEN;
    USART_InitAsync(USART0, &init);
}

void initLDMA(void)
{
  // First, initialize the LDMA unit itself
  LDMA_Init_t ldmaInit = LDMA_INIT_DEFAULT;
  LDMA_Init(&ldmaInit);

  // Source is outbuf, destination is USART0_TXDATA, and length is BUFLEN
  ldmaTXDescriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_SINGLE_M2P_BYTE(outbuf, &(USART0->TXDATA), BUFLEN);

  ldmaTXDescriptor.xfer.blockSize = ldmaCtrlBlockSizeUnit1;

  ldmaTXConfig = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_USART0_TXBL);

  // Source is USART0_RXDATA, destination is inbuf, and length is BUFLEN
  ldmaRXDescriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_SINGLE_P2M_BYTE(&(USART0->RXDATA), &inbuf_dma, 1);

  ldmaRXDescriptor.xfer.blockSize = ldmaCtrlBlockSizeUnit1;

  ldmaRXConfig = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_USART0_RXDATAV);
}

static void initTIMER0(void)
{
    TIMER_Init_TypeDef tinit = TIMER_INIT_DEFAULT;
    TIMER_Init(TIMER0, &tinit);
    TIMER_TopSet(TIMER0, 0xFFFFFFFF);
}

/* --------------------------------------------------------------------- */
void LDMA_IRQHandler(void)
{
    uint32_t flags = LDMA_IntGetEnabled() & LDMA_IntGet();

    if (flags & (1U << TX_LDMA_CHANNEL)) {
        LDMA_IntClear(1U << TX_LDMA_CHANNEL);
        tx_done = true;
    }

    if (flags & (1U << RX_LDMA_CHANNEL)) {
        LDMA_IntClear(1U << RX_LDMA_CHANNEL);

        static uint32_t rx_idx = 0;

        // === ECHO ONLY IN BENCHMARK MODES ===
        if (app_mode == MODE_DMA || app_mode == MODE_POLLED) {
            while (!(USART0->STATUS & USART_STATUS_TXBL)) ;
            USART_Tx(USART0, inbuf_dma);  // echo '1', '2', '3'
        }
        // In MAX_FREQ mode: NO ECHO

        if (inbuf_dma == '\r' || inbuf_dma == '\n' || rx_idx >= (BUFLEN-1)) {
            inbuf[rx_idx] = '\0';
            rx_done = true;
            rx_idx = 0;
        } else {
            inbuf[rx_idx++] = inbuf_dma;
            LDMA_StartTransfer(RX_LDMA_CHANNEL, &ldmaRXConfig, &ldmaRXDescriptor);
        }
    }
}

/* --------------------------------------------------------------------- */
// Task 1: Max frequency
static void task_max_freq(const uint8_t* str)
{
    int freq[256] = {0};
    int max = 0;
    uint8_t maxChar = 0;

    for (int i = 0; str[i]; i++) {
        uint8_t ch = str[i];
        if (++freq[ch] > max) {
            max = freq[ch];
            maxChar = ch;
        }
    }

    char msg[64];
    if (max > 0)
        snprintf(msg, sizeof(msg), "%c : %d\r\n", maxChar, max);
    else
        snprintf(msg, sizeof(msg), "Empty\r\n");
    UART_Transmit(msg);
}

/* --------------------------------------------------------------------- */
// Common: Fill buffer
static uint32_t createBuffer(char level)
{
    uint32_t size;
    switch (level) {
        case '1': size = 10;    break;
        case '2': size = 100;   break;
        case '3': size = 1000;  break;
        default:  size = 100;   break;
    }

    // Pattern
//    const char *pat = "Truc Maii ";
//    uint32_t patlen = strlen(pat);
//    for (uint32_t i = 0; i < size; ++i)
//        outbuf[i] = pat[i % patlen];

    // Random characters
    const int ASCII_MIN = 32;   // space
    const int ASCII_MAX = 126;  // tilde

    for (uint32_t i = 0; i < size; ++i) {
        outbuf[i] = (uint8_t)(rand() % (ASCII_MAX - ASCII_MIN + 1) + ASCII_MIN);
    }
    outbuf[size - 2] = '\r';
    outbuf[size - 1] = '\n';
    outbuf[size] = '\0';

    return size;
}

/* --------------------------------------------------------------------- */
// Task 2 & 3: Measure time
static uint32_t measure_DMA_time(uint32_t bytes)
{
    ldmaTXDescriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_SINGLE_M2P_BYTE(outbuf, &(USART0->TXDATA), bytes);
    ldmaTXDescriptor.xfer.blockSize = ldmaCtrlBlockSizeUnit1;

    TIMER_CounterSet(TIMER0, 0);
    TIMER_Enable(TIMER0, true);

    tx_done = false;
    LDMA_StartTransfer(TX_LDMA_CHANNEL, &ldmaTXConfig, &ldmaTXDescriptor);
    while (!tx_done) EMU_EnterEM1();

    TIMER_Enable(TIMER0, false);
    uint32_t cycles = TIMER_CounterGet(TIMER0);
    uint32_t timerMHz = CMU_ClockFreqGet(cmuClock_TIMER0) / 1000000UL;
    return cycles / timerMHz;
}

static uint32_t measure_polled_time(uint32_t bytes)
{
    TIMER_CounterSet(TIMER0, 0);
    TIMER_Enable(TIMER0, true);

    for (uint32_t i = 0; i < bytes; i++) {
        while (!(USART0->STATUS & USART_STATUS_TXBL)) ;
        USART_Tx(USART0, outbuf[i]);
    }

    TIMER_Enable(TIMER0, false);
    uint32_t cycles = TIMER_CounterGet(TIMER0);
    uint32_t timerMHz = CMU_ClockFreqGet(cmuClock_TIMER0) / 1000000UL;
    return cycles / timerMHz;
}

/* --------------------------------------------------------------------- */
static void run_benchmark(char level)
{
    uint32_t size = createBuffer(level);
    uint32_t us;

    if (app_mode == MODE_DMA) {
        us = measure_DMA_time(size);
        char msg[64];
        snprintf(msg, sizeof(msg), "DMA: %lu us (%lu B)\r\n", (unsigned long)us, (unsigned long)size);
        UART_Transmit(msg);
    } else {
        us = measure_polled_time(size);
        char msg[64];
        snprintf(msg, sizeof(msg), "Polled: %lu us (%lu B)\r\n", (unsigned long)us, (unsigned long)size);
        UART_Transmit(msg);
    }
}

/* --------------------------------------------------------------------- */
int main(void)
{
    sl_system_init();
    app_init();

    initGPIO();
    initUSART0();
    initLDMA();
    initTIMER0();

    //Random
    uint32_t seed = *(volatile uint32_t*)0x0FE081F0;   // DEVINFO->UNIQUEH
    srand(seed);
    UART_Transmit("\r\n=== Select Mode ===\r\n");
    UART_Transmit("1: Max Frequency\r\n");
    UART_Transmit("2: DMA Benchmark\r\n");
    UART_Transmit("3: Polled UART Benchmark\r\n");
    UART_Transmit("Enter choice: ");

    /* ------------------ MODE SELECTION ------------------ */
    rx_done = false;
    LDMA_StartTransfer(RX_LDMA_CHANNEL, &ldmaRXConfig, &ldmaRXDescriptor);
    while (!rx_done) {
        EMU_EnterEM1();
    }

    char choice = inbuf[0];
    if (choice == '1') {
        app_mode = MODE_MAX_FREQ;
        UART_Transmit("\r\n--- Max Frequency Mode ---\r\n> ");
    } else if (choice == '2') {
        app_mode = MODE_DMA;
        UART_Transmit("\r\n--- DMA Benchmark Mode ---\r\n");
        UART_Transmit("Type 1,2,3 + Enter to test 10/100/1000 bytes\r\n> ");
    } else if (choice == '3') {
        app_mode = MODE_POLLED;
        UART_Transmit("\r\n--- Polled UART Benchmark Mode ---\r\n");
        UART_Transmit("Type 1,2,3 + Enter to test 10/100/1000 bytes\r\n> ");
    } else {
        UART_Transmit("\r\nInvalid. Default: Max Frequency\r\n> ");
        app_mode = MODE_MAX_FREQ;
    }

    /* ------------------ MAIN LOOP (STAYS IN MODE) ------------------ */
    while (1) {
        sl_system_process_action();
        app_process_action();

        rx_done = false;
        LDMA_StartTransfer(RX_LDMA_CHANNEL, &ldmaRXConfig, &ldmaRXDescriptor);
        while (!rx_done) {
            EMU_EnterEM1();
        }

        if (app_mode == MODE_MAX_FREQ) {
            task_max_freq(inbuf);
            UART_Transmit("> ");
        } else {
            // Benchmark modes: expect '1','2','3'
            char level = inbuf[0];
            if (level == '1' || level == '2' || level == '3') {
                run_benchmark(level);
            } else {
                UART_Transmit("Type 1,2,3 only\r\n");
            }
        }

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
        sl_power_manager_sleep();
#endif
    }
}
