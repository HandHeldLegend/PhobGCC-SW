#include "phob_usb.h"

#include <stdint.h>
#include <inttypes.h>
#include "bsp/board.h"
#include "variables.h"
#include "tusb.h"

#define NS_USB_VID 0x0000
#define NS_USB_PID 0x0000

uint8_t ns_buffer[NS_REPORT_LEN];
ns_input_s ns_input;

/** NS HID MODE **/
// 0. String Descriptor
// 1. Device Descriptor
// 2. HID Report Descriptor
// 3. Configuration Descriptor
// 4. TinyUSB Config
/**--------------------------**/

static const tusb_desc_device_t ns_device_descriptor = {
    .bLength = 18,
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = 0x00,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,

    .bMaxPacketSize0 = 64,
    .idVendor = 0x20d6,
    .idProduct = 0xa713,

    .bcdDevice = 0x0100,
    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,
    .bNumConfigurations = 0x01
};

const uint8_t ns_hid_report_descriptor[] = {
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x05,        // Usage (Game Pad)
    0xA1, 0x01,        // Collection (Application)

    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0x01,        //   Logical Maximum (1)
    0x35, 0x00,        //   Physical Minimum (0)
    0x45, 0x01,        //   Physical Maximum (1)
    0x75, 0x01,        //   Report Size (1)
    0x95, 0x0E,        //   Report Count (14)
    0x05, 0x09,        //   Usage Page (Button)

    0x19, 0x01,        //   Usage Minimum (0x01)
    0x29, 0x0E,        //   Usage Maximum (0x0E)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x02,        //   Report Count (2)
    0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x01,        //   Usage Page (Generic Desktop Ctrls)

    0x25, 0x07,        //   Logical Maximum (7)
    0x46, 0x3B, 0x01,  //   Physical Maximum (315)
    0x75, 0x04,        //   Report Size (4)
    0x95, 0x01,        //   Report Count (1)
    0x65, 0x14,        //   Unit (System: English Rotation, Length: Centimeter)
    0x09, 0x39,        //   Usage (Hat switch)
    0x81, 0x42,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
    0x65, 0x00,        //   Unit (None)
    0x95, 0x01,        //   Report Count (1)
    0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x46, 0xFF, 0x00,  //   Physical Maximum (255)
    0x09, 0x30,        //   Usage (X)
    0x09, 0x31,        //   Usage (Y)
    0x09, 0x32,        //   Usage (Z)
    0x09, 0x35,        //   Usage (Rz)

    0x75, 0x08,        //   Report Size (8)
    0x95, 0x04,        //   Report Count (4)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x75, 0x08,        //   Report Size (8)
    0x95, 0x01,        //   Report Count (1)
    0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x0C,        //   Usage Page (Consumer)

    0x09, 0x00,        //   Usage (Unassigned)
    0x15, 0x80,        //   Logical Minimum (-128)
    0x25, 0x7F,        //   Logical Maximum (127)
    0x75, 0x08,        //   Report Size (8)
    0x95, 0x40,        //   Report Count (64)
    0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0xC0,              // End Collection
    // 94 bytes
};

#define TUSB_DESC_TOTAL_LEN      (TUD_CONFIG_DESC_LEN + CFG_TUD_HID * TUD_HID_DESC_LEN)

static const uint8_t ns_configuration_descriptor[] = {
    // Configuration number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 500),

    // Interface number, string index, boot protocol, report descriptor len, EP In address, size & polling interval
    TUD_HID_DESCRIPTOR(0, 0, false, sizeof(ns_hid_report_descriptor), 0x81, 8, 1),
};

const char* global_string_descriptor[] = {
    // array of pointer to string descriptors
    (char[]){0x09, 0x04},                // 0: is supported language is English (0x0409)
    "PhobGCC", // 1: Manufacturer
    "PhobGCC USB Gamepad",      // 2: Product
    "000000",       // 3: Serials, should use chip ID
};

/*
const tinyusb_config_t ns_cfg = {
    .device_descriptor = &ns_device_descriptor,
    .string_descriptor = global_string_descriptor,
    .external_phy = false,
    .configuration_descriptor = ns_configuration_descriptor,
};*/
/**--------------------------**/
/**--------------------------**/

// Implement TinyUSB Callbacks

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
uint8_t const* tud_descriptor_device_cb(void) {
  return (uint8_t const*) &ns_device_descriptor;
}

// Invoked when received GET HID REPORT DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const* tud_hid_descriptor_report_cb(uint8_t itf) {
  (void)itf;
  return ns_hid_report_descriptor;
}

// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const* tud_descriptor_configuration_cb(uint8_t index) {
  (void)index;  // for multiple configurations
  return ns_configuration_descriptor;
}


static uint16_t _desc_str[32];

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
  (void) langid;

  uint8_t chr_count;

  if ( index == 0)
  {
    memcpy(&_desc_str[1], global_string_descriptor[0], 2);
    chr_count = 1;
  }else
  {
    // Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
    // https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors

    if ( !(index < sizeof(global_string_descriptor)/sizeof(global_string_descriptor[0])) ) return NULL;

    const char* str = global_string_descriptor[index];

    // Cap at max char
    chr_count = strlen(str);
    if ( chr_count > 31 ) chr_count = 31;

    // Convert ASCII string into UTF-16
    for(uint8_t i=0; i<chr_count; i++)
    {
      _desc_str[1+i] = str[i];
    }
  }

  // first byte is length (including header), second byte is string type
  _desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2*chr_count + 2);

  return _desc_str;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

/**
 * @brief Takes in directions and outputs a byte output appropriate for
 * HID Hat usage.
 * @param hat_type hat_mode_t type - The type of controller you're converting for.
 * @param leftRight 0 through 2 (2 is right) to indicate the direction left/right
 * @param upDown 0 through 2 (2 is up) to indicate the direction up/down
*/
uint8_t dir_to_hat(hat_mode_t hat_type, uint8_t leftRight, uint8_t upDown)
{
    uint8_t ret = 0;
    switch(hat_type)
    {
        default:
        case HAT_MODE_NS:
            ret = NS_HAT_CENTER;

        if (leftRight == 2)
        {
            ret = NS_HAT_RIGHT;
            if (upDown == 2)
            {
                ret = NS_HAT_TOP_RIGHT;
            }
            else if (upDown == 0)
            {
                ret = NS_HAT_BOTTOM_RIGHT;
            }
        }
        else if (leftRight == 0)
        {
            ret = NS_HAT_LEFT;
            if (upDown == 2)
            {
                ret = NS_HAT_TOP_LEFT;
            }
            else if (upDown == 0)
            {
                ret = NS_HAT_BOTTOM_LEFT;
            }
        }

        else if (upDown == 2)
        {
            ret = NS_HAT_TOP;
        }
        else if (upDown == 0)
        {
            ret = NS_HAT_BOTTOM;
        }

        return ret;
        break;

        case HAT_MODE_XI:
                ret = XI_HAT_CENTER;

            if (leftRight == 2)
            {
                ret = XI_HAT_RIGHT;
                if (upDown == 2)
                {
                    ret = XI_HAT_TOP_RIGHT;
                }
                else if (upDown == 0)
                {
                    ret = XI_HAT_BOTTOM_RIGHT;
                }
            }
            else if (leftRight == 0)
            {
                ret = XI_HAT_LEFT;
                if (upDown == 2)
                {
                    ret = XI_HAT_TOP_LEFT;
                }
                else if (upDown == 0)
                {
                    ret = XI_HAT_BOTTOM_LEFT;
                }
            }

            else if (upDown == 2)
            {
                ret = XI_HAT_TOP;
            }
            else if (upDown == 0)
            {
                ret = XI_HAT_BOTTOM;
            }

            return ret;
            break;
    }
}

#define ANALOG_SCALER (float) 1.28
uint8_t scale_axis(int input)
{
    int res = input;
    if (input > 129)
    {
        float tmp = (float) input - 128;
        tmp = tmp * ANALOG_SCALER;
        res = (int) tmp + 128;
    }
    else if (input < 127)
    {
        float tmp = 128 - (float) input;
        tmp = tmp * ANALOG_SCALER;
        res = 128 - (int) tmp;    
    }
    
    if (res >= 255)
    {
        res = 255;
    }
    if (res <= 0)
    {
        res = 0;
    }
    return (uint8_t) res;
}

uint8_t scale_trigger(int input)
{
    if (input < 0)
    {
        return (uint8_t) 0;
    }
    else if ( input <= 255)
    {
        return (uint8_t) input;
    }
    else
    {
        return (uint8_t) 255;
    }
}

void hid_task(void)
{
    // skip if hid is not ready yet
    if ( !tud_hid_ready() ) return;

    // Convert input data
    ns_input.button_a = _btn.A;
    ns_input.button_b = _btn.B;
    ns_input.button_x = _btn.X;
    ns_input.button_y = _btn.Y;

    uint8_t lr = 1 - _btn.Dl + _btn.Dr;
    uint8_t ud = 1 - _btn.Dd + _btn.Du;

    ns_input.dpad_hat = dir_to_hat(HAT_MODE_NS, lr, ud);

    // ns_input.button_home;
    // ns_input.button_capture;

    ns_input.button_plus = _btn.S;

    ns_input.trigger_r = _btn.Z;

    ns_input.trigger_zl = (_btn.La <= 128) | _btn.L;
    ns_input.trigger_zr = (_btn.Ra <= 128) | _btn.R;

    ns_input.stick_left_x   = scale_axis(_btn.Ax);
    ns_input.stick_left_y   = scale_axis(_btn.Ay);
    ns_input.stick_right_x  = scale_axis(_btn.Cx);
    ns_input.stick_right_y  = scale_axis(_btn.Cy);

    memcpy(&ns_buffer, &ns_input, NS_REPORT_LEN);

    // Send report
    tud_hid_report(0, &ns_buffer, NS_REPORT_LEN);
}

void start_usb_mode(phob_usb_mode_t usb_mode)
{
    board_init();
    tusb_init();

    while(1)
    {
        tud_task();
        hid_task();
        sleep_ms(1);
    }

}