#ifndef _LPS22HB_H_
#define _LPS22HB_H_

#include "driver/i2c.h"
#include "esp_err.h"

#define LPS22HB_ADDRESS               0x5C
#define LPS22HB_WHO_AM_I_REG          0x0F
#define LPS22HB_WHO_AM_I_RESPONSE     0xB1

esp_err_t lps22hb_init(void);
esp_err_t lps22hb_who_am_i(void);
esp_err_t lps22hb_read_pressure(float *pressure);

#endif // _LPS22HB_H_
