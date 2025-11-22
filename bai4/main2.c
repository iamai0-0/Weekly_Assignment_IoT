///****************************************************************************
// * @file main.c
// * @brief main() function.
// *******************************************************************************
// * # License
// * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
// *******************************************************************************
// *
// * The licensor of this software is Silicon Laboratories Inc. Your use of this
// * software is governed by the terms of Silicon Labs Master Software License
// * Agreement (MSLA) available at
// * www.silabs.com/about-us/legal/master-software-license-agreement. This
// * software is distributed to you in Source Code format and is governed by the
// * sections of the MSLA applicable to Source Code.
// *
// ******************************************************************************/
//#include "app.h"
//#include "sl_component_catalog.h"
//#include "sl_system_init.h"
//#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
//#include "sl_power_manager.h"
//#endif
//#if defined(SL_CATALOG_KERNEL_PRESENT)
//#include "sl_system_kernel.h"
//#else  // SL_CATALOG_KERNEL_PRESENT
//#include "sl_system_process_action.h"
//#endif  // SL_CATALOG_KERNEL_PRESENT
//
//#include "em_chip.h"
//#include "em_cmu.h"
//#include "em_emu.h"
//#include "em_gpio.h"
//#include "em_ldma.h"
//#include "em_timer.h"
//#include "em_usart.h"
//#include "stdio.h"
//#include "string.h"
//
///**************************************************************************
// * DEFINE
// *****************************************************************************/
//
//#define BSP_TXPORT gpioPortA
//#define BSP_RXPORT gpioPortA
//#define BSP_TXPIN 5
//#define BSP_RXPIN 6
//#define BSP_ENABLE_PORT gpioPortD
//#define BSP_ENABLE_PIN 4
//// LDMA channel for receive and transmit servicing
//#define RX_LDMA_CHANNEL 0
//#define TX_LDMA_CHANNEL 1
//
//void initTIMER0(void);
//uint8_t* createBuffer(uint8_t level);
//uint32_t calculatePeriod(uint32_t numClk);
//uint32_t timeMeasure(void);
//
///***************************************************************************
// * STATIC VARIABLES
// *****************************************************************************/
//// LDMA descriptor and transfer configuration structures for USART TX channel
//LDMA_Descriptor_t ldmaTXDescriptor;
//LDMA_TransferCfg_t ldmaTXConfig;
//
//// LDMA descriptor and transfer configuration structures for USART RX channel
//LDMA_Descriptor_t ldmaRXDescriptor;
//LDMA_TransferCfg_t ldmaRXConfig;
//
//// Size of the data buffers
//// #define BUFLEN 100
//// 2 and 3
//#define BUFLEN 1000
//
//// Outgoing data
//uint8_t outbuf[BUFLEN];
//
//// Incoming data
//uint8_t inbuf[BUFLEN];
//uint8_t inbuf_dma;
//
//// Data reception complete
//bool rx_done;
//bool tx_done;
//
//void initTIMER0(void) {
//    // Initialize the timer
////  CMU_ClockEnable(cmuClock_TIMER0, true); // <--- add this
//    TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
//    TIMER_Init(TIMER0, &timerInit);
//    TIMER_TopSet(TIMER0, 0xFFFFFFFF);
//}
//uint8_t* createBuffer(uint8_t level) {
//    int size;
//
//    switch (level) {
//        case '1':
//            size = 10;
//            break;
//        case '2':
//            size = 100;
//            break;
//        case '3':
//            size = 1000;
//            break;
//        default:
//
//            size = 10;
//    }
//    snprintf(outbuf, sizeof(outbuf), "Size = %d!\n\r",size );
//    UART_TransmitMultiple(USART0, outbuf, strlen(outbuf));
//    // Source is outbuf, destination is USART0_TXDATA, and length is BUFLEN
//    ldmaTXDescriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_SINGLE_M2P_BYTE(outbuf, &(USART0->TXDATA), size);
//
//    ldmaTXDescriptor.xfer.blockSize = ldmaCtrlBlockSizeUnit1;
//    const char* pattern = "Truc Maii ";
//    int patternLen = strlen(pattern);
//
//    // Fill buffer with repeated pattern
//    for (int i = 0; i < size; i++) {
//        outbuf[i] = pattern[i % patternLen];
//    }
//
//    outbuf[size] = '\0';
//    UART_TransmitMultiple(USART0, outbuf, strlen(outbuf));
//    return outbuf;
//}
//
//uint32_t calculatePeriod(uint32_t numClk) {
//    uint32_t timerClockMHz = CMU_ClockFreqGet(cmuClock_TIMER0) / 1000000;
//    // Convert the count between edges to a period in microseconds
//    return (numClk / timerClockMHz);
//}
//uint32_t timeMeasure() {
//    uint32_t numClk = 0;
//    uint32_t elapsedTime = 0;
//    // Set counter to 0
//    snprintf(outbuf, sizeof(outbuf), "Counter set to 0\n\r" );
//        UART_TransmitMultiple(USART0, outbuf, strlen(outbuf));
//    TIMER_CounterSet(TIMER0, 0);
//    snprintf(outbuf, sizeof(outbuf), "Counter set to 0\n\r" );
//            UART_TransmitMultiple(USART0, outbuf, strlen(outbuf));
//    // Start the timer
//    TIMER_Enable(TIMER0, true);
//
//    snprintf(outbuf, sizeof(outbuf), "Counter enabled\n\r" );
//    UART_TransmitMultiple(USART0, outbuf, strlen(outbuf));
//    // Code here
//    // 2
////    LDMA_StartTransfer(TX_LDMA_CHANNEL, &ldmaTXConfig, &ldmaTXDescriptor);
////    while (!tx_done) {
////        EMU_EnterEM1();
////    }
//    // 3
//    UART_TransmitMultiple(USART0, outbuf, strlen(outbuf));
//    // Stop the timer
//    TIMER_Enable(TIMER0, false);
//    snprintf(outbuf, sizeof(outbuf), "Counter disabled\n\r" );
//       UART_TransmitMultiple(USART0, outbuf, strlen(outbuf));
//    // Get the number of clock cycles elapsed
//    numClk = TIMER_CounterGet(TIMER0);
//    // Calculate elapsed time in microseconds
//    elapsedTime = calculatePeriod(numClk);
//    return elapsedTime;
//}
//
///****************************************************************************
// * @brief
// *    GPIO initialization
// *****************************************************************************/
//void initGPIO(void) {
//    // Configure the USART TX pin to the board controller as an output
//    GPIO_PinModeSet(BSP_TXPORT, BSP_TXPIN, gpioModePushPull, 1);
//
//    // Configure the USART RX pin to the board controller as an input
//    GPIO_PinModeSet(BSP_RXPORT, BSP_RXPIN, gpioModeInput, 0);
//
//    /*
//     * Configure the BCC_ENABLE pin as output and set high.  This enables
//     * the virtual COM port (VCOM) connection to the board controller and
//     * permits serial port traffic over the debug connection to the host
//     * PC.
//     *
//     * To disable the VCOM connection and use the pins on the kit
//     * expansion (EXP) header, comment out the following line.
//     */
//    GPIO_PinModeSet(BSP_ENABLE_PORT, BSP_ENABLE_PIN, gpioModePushPull, 1);
//}
///***************************************************************************
// * @brief
// *    USART0 initialization
// *****************************************************************************/
//void initUSART0(void) {
//    // Default asynchronous initializer (115.2 Kbps, 8N1, no flow control)
//    USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
//
//    // Route USART0 TX and RX to the board controller TX and RX pins
//    GPIO->USARTROUTE[0].TXROUTE = (BSP_TXPORT << _GPIO_USART_TXROUTE_PORT_SHIFT) | (BSP_TXPIN << _GPIO_USART_TXROUTE_PIN_SHIFT);
//    GPIO->USARTROUTE[0].RXROUTE = (BSP_RXPORT << _GPIO_USART_RXROUTE_PORT_SHIFT) | (BSP_RXPIN << _GPIO_USART_RXROUTE_PIN_SHIFT);
//
//    // Enable RX and TX signals now that they have been routed
//    GPIO->USARTROUTE[0].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN | GPIO_USART_ROUTEEN_TXPEN;
//
//    // Configure and enable USART0
//    USART_InitAsync(USART0, &init);
//}
//
//void UART_TransmitMultiple(USART_TypeDef* uart, uint8_t* data, uint32_t length) {
//    for (uint32_t i = 0; i < length; i++) {
//        USART_Tx(uart, data[i]);
//    }
//}
//
//void UART_ReceiveMultiple(USART_TypeDef* uart, uint8_t* buffer, uint32_t length) {
//    for (uint32_t i = 0; i < length; i++) {
//        buffer[i] = USART_Rx(uart);
//    }
//}
//void initLDMA(void) {
//    // First, initialize the LDMA unit itself
//    LDMA_Init_t ldmaInit = LDMA_INIT_DEFAULT;
//    LDMA_Init(&ldmaInit);
//
//    // Source is outbuf, destination is USART0_TXDATA, and length is BUFLEN
//    ldmaTXDescriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_SINGLE_M2P_BYTE(outbuf, &(USART0->TXDATA), BUFLEN);
//
//    ldmaTXDescriptor.xfer.blockSize = ldmaCtrlBlockSizeUnit1;
//
//    ldmaTXConfig = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_USART0_TXBL);
//
//    // Source is USART0_RXDATA, destination is inbuf, and length is BUFLEN
//    ldmaRXDescriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_SINGLE_P2M_BYTE(&(USART0->RXDATA), &inbuf_dma, 1);
//
//    ldmaRXDescriptor.xfer.blockSize = ldmaCtrlBlockSizeUnit1;
//
//    ldmaRXConfig = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_USART0_RXDATAV);
//}
//
//void LDMA_IRQHandler() {
//    uint32_t flags = LDMA_IntGet();
//
//    // Clear the transmit channel's done flag if set
//    if (flags & (1 << TX_LDMA_CHANNEL)) {
//        LDMA_IntClear(1 << TX_LDMA_CHANNEL);
//        // 2
//        tx_done = true;
//    }
//
////    if (flags & (1 << RX_LDMA_CHANNEL)) {
////        LDMA_IntClear(1 << RX_LDMA_CHANNEL);
////        if(*inbuf)
////        rx_done = true;
////    }
//    if (flags & (1 << RX_LDMA_CHANNEL)) {
//          LDMA_IntClear(1 << RX_LDMA_CHANNEL);
//          static uint32_t rx_index = 0;
//
//          if (inbuf_dma == 13 || rx_index >= BUFLEN - 1) {
//              inbuf[rx_index] = '\0';
//              rx_done = true;
//              rx_index = 0;
//          } else {
////              snprintf(outbuf, sizeof(outbuf), "Truc Mai %d: %d\n\r", rx_index, inbuf_dma);
////              LDMA_StartTransfer(TX_LDMA_CHANNEL, &ldmaTXConfig, &ldmaTXDescriptor);
//              inbuf[rx_index++] = inbuf_dma;
//              // Restart DMA to receive next byte
//              LDMA_StartTransfer(RX_LDMA_CHANNEL, &ldmaRXConfig, &ldmaRXDescriptor);
//          }
//          USART_Tx(USART0, inbuf_dma);
//      }
//}
//
//void max_frequency(const uint8_t* str) {
//    int freq[256] = {0};  // ASCII characters frequency
//    int max = 0;
//    uint8_t maxChar;  // M: freq[77]++;   freq['a']++;
//
//    // Count frequencies of each character
//    for (int i = 0; str[i] != '\0' && str[i] != '\n'; i++) {
//        uint8_t ch = str[i];
//        freq[ch]++;
////        snprintf(outbuf,sizeof(outbuf) , "Outbuf: %c : %d\n\r", maxChar, max);
//    }
//
//    // Find the character with maximum frequency
//    for (int i = 0; i < 256; i++) {
//        if (freq[i] > max) {
//            max = freq[i];
//            maxChar = i;
//        }
//    }
//    snprintf(outbuf,sizeof(outbuf), "\0");
//    snprintf(outbuf,sizeof(outbuf) , "%c : %d\n\r", maxChar, max);
//}
//
//int main(void) {
//    sl_system_init();
//    app_init();
//
//#if defined(SL_CATALOG_KERNEL_PRESENT)
//    sl_system_kernel_start();
//#else  // SL_CATALOG_KERNEL_PRESENT
//
//    initGPIO();
//    initUSART0();
//    initLDMA();
//
//    // Debug
////    snprintf(outbuf, "Truc Mai\n");
//    snprintf(outbuf, sizeof(outbuf), "Truc Mai\n\r");
//    LDMA_StartTransfer(TX_LDMA_CHANNEL, &ldmaTXConfig, &ldmaTXDescriptor);
//    while (1) {
//        sl_system_process_action();
//        app_process_action();
//
//        rx_done = false;
//
//        LDMA_StartTransfer(RX_LDMA_CHANNEL, &ldmaRXConfig, &ldmaRXDescriptor);
//
//        while (!rx_done) {
//            EMU_EnterEM1();
//        }
//        snprintf(outbuf, sizeof(outbuf), "RX done!\n\r");
//        UART_TransmitMultiple(USART0, outbuf, strlen(outbuf));
////         LDMA_StartTransfer(TX_LDMA_CHANNEL, &ldmaTXConfig, &ldmaTXDescriptor);
//
//        // Received data done
//        // 1
////        if(strlen(inbuf)){
////            max_frequency(inbuf);
////        }
////        else
////           continue;
//
//        // 2 and 3
//        createBuffer(inbuf[0]);
//        uint32_t time = timeMeasure();
//
//        snprintf(outbuf, "\n%d us\n", time);
////
////         for (uint32_t i = 0; i < BUFLEN; i++) {
////             outbuf[i] = inbuf[i];
////         }
////        snprintf((char *)outbuf, sizeof(outbuf), "%s", (char *)outbuf);
//        while (!(USART0->STATUS & USART_STATUS_TXBL)) {  //  check Tx Buffer is empty before transmitting?
//        }
//        LDMA_StartTransfer(TX_LDMA_CHANNEL, &ldmaTXConfig, &ldmaTXDescriptor);
//
//#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
//        // Let the CPU go to sleep if the system allows it.
//        sl_power_manager_sleep();
//#endif
//    }
//#endif  // SL_CATALOG_KERNEL_PRESENT
//}
