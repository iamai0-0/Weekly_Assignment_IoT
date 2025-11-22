/***************************************************************************/ /**
                                                                               * @file custom_adv.c
                                                                               * @brief customize advertising
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

#include "custom_adv.h"

#include <string.h>

#include "stdio.h"

void fill_adv_packet(CustomAdv_t* pData, uint8_t flags, uint16_t companyID, void* data, char* name) {
    int n = strlen(name);

    // Flags
    pData->len_flags = 0x02;
    pData->type_flags = 0x01;
    pData->val_flags = flags;

    // Manufacturer Data
    pData->type_manuf = 0xFF;
    pData->company_LO = companyID & 0xFF;
    pData->company_HI = (companyID >> 8) & 0xFF;

#ifdef LAB_EXERCISE_1
    uint8_t* led = (uint8_t*)data;
    pData->len_manuf = 5;  // 1+2+2
    pData->led0_status = led[0];
    pData->led1_status = led[1];
#else
    char* time = (char*)data;
    pData->len_manuf = 11;  // 1+2+8
    memcpy(pData->time_str, time, 8);
#endif

    // Name
    pData->type_name = (n > NAME_MAX_LENGTH) ? 0x08 : 0x09;
    strncpy(pData->name, name, NAME_MAX_LENGTH);
    if (n > NAME_MAX_LENGTH) n = NAME_MAX_LENGTH;
    pData->len_name = 1 + n;

    // Total size
    pData->data_size = 3 + (1 + pData->len_manuf) + (1 + pData->len_name);
}

void start_adv(CustomAdv_t* pData, uint8_t advertising_set_handle) {
    sl_status_t sc;
    sc = sl_bt_legacy_advertiser_set_data(advertising_set_handle, 0, pData->data_size, (const uint8_t*)pData);
    app_assert_status(sc);

    sc = sl_bt_legacy_advertiser_start(advertising_set_handle, sl_bt_legacy_advertiser_connectable);
    app_assert_status(sc);
}

void update_adv_data(CustomAdv_t* pData, uint8_t advertising_set_handle, void* data) {
    sl_status_t sc;

#ifdef LAB_EXERCISE_1
    uint8_t* led = (uint8_t*)data;
    pData->led0_status = led[0];
    pData->led1_status = led[1];
#else
    char* time = (char*)data;
    memcpy(pData->time_str, time, 8);
#endif

    sc = sl_bt_legacy_advertiser_set_data(advertising_set_handle, 0, pData->data_size, (const uint8_t*)pData);
    app_assert_status(sc);
}
