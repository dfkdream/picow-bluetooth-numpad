//
// Created by dfkdream on 23. 11. 27.
//

#include "keyboard.h"
#include <inttypes.h>

#include "btstack.h"
#include "pico/cyw43_arch.h"
#include "pico/btstack_cyw43.h"

// When not set to 0xffff, sniff and sniff subrating are enabled
static uint16_t host_max_latency = 1600;
static uint16_t host_min_timeout = 3200;

#define REPORT_ID 0x01

// close to USB HID Specification 1.1, Appendix B.1
const uint8_t hid_descriptor_keyboard[] = {

        0x05, 0x01,                    // Usage Page (Generic Desktop)
        0x09, 0x06,                    // Usage (Keyboard)
        0xa1, 0x01,                    // Collection (Application)

        // Report ID

        0x85, REPORT_ID,               // Report ID

        // Modifier byte (input)

        0x75, 0x01,                    //   Report Size (1)
        0x95, 0x08,                    //   Report Count (8)
        0x05, 0x07,                    //   Usage Page (Key codes)
        0x19, 0xe0,                    //   Usage Minimum (Keyboard LeftControl)
        0x29, 0xe7,                    //   Usage Maximum (Keyboard Right GUI)
        0x15, 0x00,                    //   Logical Minimum (0)
        0x25, 0x01,                    //   Logical Maximum (1)
        0x81, 0x02,                    //   Input (Data, Variable, Absolute)

        // Reserved byte (input)

        0x75, 0x01,                    //   Report Size (1)
        0x95, 0x08,                    //   Report Count (8)
        0x81, 0x03,                    //   Input (Constant, Variable, Absolute)

        // LED report + padding (output)

        0x95, 0x05,                    //   Report Count (5)
        0x75, 0x01,                    //   Report Size (1)
        0x05, 0x08,                    //   Usage Page (LEDs)
        0x19, 0x01,                    //   Usage Minimum (Num Lock)
        0x29, 0x05,                    //   Usage Maximum (Kana)
        0x91, 0x02,                    //   Output (Data, Variable, Absolute)

        0x95, 0x01,                    //   Report Count (1)
        0x75, 0x03,                    //   Report Size (3)
        0x91, 0x03,                    //   Output (Constant, Variable, Absolute)

        // Keycodes (input)

        0x95, 0x06,                    //   Report Count (6)
        0x75, 0x08,                    //   Report Size (8)
        0x15, 0x00,                    //   Logical Minimum (0)
        0x25, 0xff,                    //   Logical Maximum (1)
        0x05, 0x07,                    //   Usage Page (Key codes)
        0x19, 0x00,                    //   Usage Minimum (Reserved (no event indicated))
        0x29, 0xff,                    //   Usage Maximum (Reserved)
        0x81, 0x00,                    //   Input (Data, Array)

        0xc0,                          // End collection
};

// STATE

static uint8_t hid_service_buffer[300];
static uint8_t device_id_sdp_service_buffer[100];
static const char hid_device_name[] = "BTstack HID Keyboard";
static btstack_packet_callback_registration_t hci_event_callback_registration;
static uint16_t hid_cid;
static uint8_t hid_boot_device = 0;

// HID Report sending
static uint8_t                send_buffer_storage[16];
static btstack_ring_buffer_t  send_buffer;

static enum {
    APP_BOOTING,
    APP_NOT_CONNECTED,
    APP_CONNECTING,
    APP_CONNECTED
} app_state = APP_BOOTING;

void send_keys(int modifier, const int *keycodes){
    if (app_state!=APP_CONNECTED){
        printf("HID Not connected\n");
        return;
    }

    uint8_t report[] = {0xa1, REPORT_ID, modifier, 0,
                        keycodes[0], keycodes[1], keycodes[2], keycodes[3], keycodes[4], keycodes[5]};
    hid_device_send_interrupt_message(hid_cid, &report[0], sizeof(report));
}

static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t * packet, uint16_t packet_size){
    UNUSED(channel);
    UNUSED(packet_size);
    uint8_t status;
    switch (packet_type){
        case HCI_EVENT_PACKET:
            switch (hci_event_packet_get_type(packet)){
                case BTSTACK_EVENT_STATE:
                    if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING) return;
                    app_state = APP_NOT_CONNECTED;
                    break;

                case HCI_EVENT_USER_CONFIRMATION_REQUEST:
                    // ssp: inform about user confirmation request
                    log_info("SSP User Confirmation Request with numeric value '%06"PRIu32"'\n", hci_event_user_confirmation_request_get_numeric_value(packet));
                    log_info("SSP User Confirmation Auto accept\n");
                    break;

                case HCI_EVENT_HID_META:
                    switch (hci_event_hid_meta_get_subevent_code(packet)){
                        case HID_SUBEVENT_CONNECTION_OPENED:
                            status = hid_subevent_connection_opened_get_status(packet);
                            if (status != ERROR_CODE_SUCCESS) {
                                // outgoing connection failed
                                printf("Connection failed, status 0x%x\n", status);
                                app_state = APP_NOT_CONNECTED;
                                hid_cid = 0;
                                return;
                            }
                            app_state = APP_CONNECTED;
                            hid_cid = hid_subevent_connection_opened_get_hid_cid(packet);

                            printf("HID Connected\n");

                            break;
                        case HID_SUBEVENT_CONNECTION_CLOSED:
                            printf("HID Disconnected\n");
                            app_state = APP_NOT_CONNECTED;
                            hid_cid = 0;
                            break;
                        case HID_SUBEVENT_CAN_SEND_NOW:
                            printf("HID Can send now\n");
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void keyboard_start(){
    cyw43_arch_init();

    // allow to get found by inquiry
    gap_discoverable_control(1);
    // use Limited Discoverable Mode; Peripheral; Keyboard as CoD
    gap_set_class_of_device(0x2540);
    // set local name to be identified - zeroes will be replaced by actual BD ADDR
    gap_set_local_name("HID Keyboard Demo 00:00:00:00:00:00");
    // allow for role switch in general and sniff mode
    gap_set_default_link_policy_settings( LM_LINK_POLICY_ENABLE_ROLE_SWITCH | LM_LINK_POLICY_ENABLE_SNIFF_MODE );
    // allow for role switch on outgoing connections - this allow HID Host to become master when we re-connect to it
    gap_set_allow_role_switch(true);

    // L2CAP
    l2cap_init();

    // SDP Server
    sdp_init();
    memset(hid_service_buffer, 0, sizeof(hid_service_buffer));

    uint8_t hid_virtual_cable = 0;
    uint8_t hid_remote_wake = 1;
    uint8_t hid_reconnect_initiate = 1;
    uint8_t hid_normally_connectable = 1;

    hid_sdp_record_t hid_params = {
            // hid service subclass 2540 Keyboard, hid country code 33 US
            0x2540, 33,
            hid_virtual_cable, hid_remote_wake,
            hid_reconnect_initiate, hid_normally_connectable,
            hid_boot_device,
            host_max_latency, host_min_timeout,
            3200,
            hid_descriptor_keyboard,
            sizeof(hid_descriptor_keyboard),
            hid_device_name
    };

    hid_create_sdp_record(hid_service_buffer, 0x10001, &hid_params);

    printf("HID service record size: %u\n", de_get_len( hid_service_buffer));
    sdp_register_service(hid_service_buffer);

    // See https://www.bluetooth.com/specifications/assigned-numbers/company-identifiers if you don't have a USB Vendor ID and need a Bluetooth Vendor ID
    // device info: BlueKitchen GmbH, product 1, version 1
    device_id_create_sdp_record(device_id_sdp_service_buffer, 0x10003, DEVICE_ID_VENDOR_ID_SOURCE_BLUETOOTH, BLUETOOTH_COMPANY_ID_BLUEKITCHEN_GMBH, 1, 1);
    printf("Device ID SDP service record size: %u\n", de_get_len((uint8_t*)device_id_sdp_service_buffer));
    sdp_register_service(device_id_sdp_service_buffer);

    // HID Device
    hid_device_init(hid_boot_device, sizeof(hid_descriptor_keyboard), hid_descriptor_keyboard);

    // register for HCI events
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    // register for HID events
    hid_device_register_packet_handler(&packet_handler);

    btstack_ring_buffer_init(&send_buffer, send_buffer_storage, sizeof(send_buffer_storage));

    // turn on!
    hci_power_control(HCI_POWER_ON);

    btstack_run_loop_execute();
}