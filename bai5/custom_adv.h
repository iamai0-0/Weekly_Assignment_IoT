#ifndef _CUSTOM_ADV_H_
#define _CUSTOM_ADV_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_assert.h"
#include "sl_bt_api.h"

#define NAME_MAX_LENGTH 14
#define FLAG 0x06
#define COMPANY_ID 0x02FF

typedef struct
{
    // 3 bytes for Flag type
    uint8_t len_flags;
    uint8_t type_flags;
    uint8_t val_flags;

    // Manufacturer Specific Data
    uint8_t len_manuf;
    uint8_t type_manuf;
    uint8_t company_LO;
    uint8_t company_HI;

#ifdef LAB_EXERCISE_1
    // Exercise 1: LED status
    uint8_t led0_status;
    uint8_t led1_status;
#else
    // Exercise 2: Uptime HH:MM:SS
    char time_str[8];  // "HH:MM:SS"
#endif

    // Name
    uint8_t len_name;
    uint8_t type_name;
    char name[NAME_MAX_LENGTH];

    // Bookkeeping
    char dummy;
    uint8_t data_size;
} CustomAdv_t;

// fill_adv_packet: data là uint8_t[2] cho Ex1, char[8] cho Ex2
void fill_adv_packet(CustomAdv_t* pData, uint8_t flags, uint16_t companyID, void* data, char* name);
void start_adv(CustomAdv_t* pData, uint8_t advertising_set_handle);
void update_adv_data(CustomAdv_t* pData, uint8_t advertising_set_handle, void* data);

#ifdef __cplusplus
}
#endif

#endif