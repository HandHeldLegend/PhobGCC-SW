#ifndef PHOB_USB_TYPES_H
#define PHOB_USB_TYPES_H

#include <inttypes.h>

typedef enum
{
    PUSB_MODE_NS,
    PUSB_MODE_GC,
    PUSB_MODE_XI,
} phob_usb_mode_t;

typedef enum
{
  NS_HAT_TOP          = 0x00,
  NS_HAT_TOP_RIGHT    = 0x01,
  NS_HAT_RIGHT        = 0x02,
  NS_HAT_BOTTOM_RIGHT = 0x03,
  NS_HAT_BOTTOM       = 0x04,
  NS_HAT_BOTTOM_LEFT  = 0x05,
  NS_HAT_LEFT         = 0x06,
  NS_HAT_TOP_LEFT     = 0x07,
  NS_HAT_CENTER       = 0x08,
} ns_input_hat_dir_t;

typedef enum
{
  XI_HAT_TOP          = 0x01,
  XI_HAT_TOP_RIGHT    = 0x02,
  XI_HAT_RIGHT        = 0x03,
  XI_HAT_BOTTOM_RIGHT = 0x04,
  XI_HAT_BOTTOM       = 0x05,
  XI_HAT_BOTTOM_LEFT  = 0x06,
  XI_HAT_LEFT         = 0x07,
  XI_HAT_TOP_LEFT     = 0x08,
  XI_HAT_CENTER       = 0x00,
} xi_input_hat_dir_t;

typedef enum
{
    HAT_MODE_NS,
    HAT_MODE_XI,
} hat_mode_t;

#define NS_REPORT_LEN 8

typedef struct
{
    union
        {
            struct
            {
                uint8_t button_y    : 1;
                uint8_t button_b    : 1;
                uint8_t button_a    : 1;
                uint8_t button_x    : 1;
                uint8_t trigger_l   : 1;
                uint8_t trigger_r   : 1;
                uint8_t trigger_zl  : 1;
                uint8_t trigger_zr  : 1;
            };
            uint8_t buttons_1;
        };

        union
        {
            struct
            {
                uint8_t button_minus  : 1;
                uint8_t button_plus   : 1;
                uint8_t stick_left    : 1;
                uint8_t stick_right   : 1;
                uint8_t button_home   : 1;
                uint8_t button_capture: 1;
                uint8_t dummy_1       : 2;
            }; 
            uint8_t buttons_2;
        };

    uint8_t dpad_hat;
    uint8_t stick_left_x;
    uint8_t stick_left_y;
    uint8_t stick_right_x;
    uint8_t stick_right_y;
    uint8_t dummy_2;

} __attribute__ ((packed)) ns_input_s;

#endif