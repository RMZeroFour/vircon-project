#include "gamepad.h"

namespace
{
libevdev_uinput* create_gamepad_device(const std::string& name)
{
    libevdev* device = libevdev_new();

    libevdev_set_name(device, name.c_str());
    
    libevdev_enable_event_type(device, EV_KEY);
    libevdev_enable_event_code(device, EV_KEY, BTN_X, nullptr); // X
    libevdev_enable_event_code(device, EV_KEY, BTN_A, nullptr); // A
    libevdev_enable_event_code(device, EV_KEY, BTN_Y, nullptr); // Y
    libevdev_enable_event_code(device, EV_KEY, BTN_B, nullptr); // B
    libevdev_enable_event_code(device, EV_KEY, BTN_TL, nullptr); // L1
    libevdev_enable_event_code(device, EV_KEY, BTN_TR, nullptr); // R1
    libevdev_enable_event_code(device, EV_KEY, BTN_TL2, nullptr); // L2
    libevdev_enable_event_code(device, EV_KEY, BTN_TR2, nullptr); // R2
    libevdev_enable_event_code(device, EV_KEY, BTN_SELECT, nullptr); // SELECT
    libevdev_enable_event_code(device, EV_KEY, BTN_START, nullptr); // START
    libevdev_enable_event_code(device, EV_KEY, BTN_DPAD_UP, nullptr); // UP
    libevdev_enable_event_code(device, EV_KEY, BTN_DPAD_DOWN, nullptr); // DOWN
    libevdev_enable_event_code(device, EV_KEY, BTN_DPAD_LEFT, nullptr); // LEFT
    libevdev_enable_event_code(device, EV_KEY, BTN_DPAD_RIGHT, nullptr); // RIGHT

    input_absinfo axis_info{
        .minimum = -32768,
        .maximum = 32767,
    };
    libevdev_enable_event_type(device, EV_ABS);
    libevdev_enable_event_code(device, EV_ABS, ABS_X, &axis_info); // ANALOG LX
    libevdev_enable_event_code(device, EV_ABS, ABS_Y, &axis_info); // ANALOG LY
    libevdev_enable_event_code(device, EV_ABS, ABS_RX, &axis_info); // ANALOG RX
    libevdev_enable_event_code(device, EV_ABS, ABS_RY, &axis_info); // ANALOG RY

    libevdev_uinput* joystick_device = nullptr;
    libevdev_uinput_create_from_device(device, LIBEVDEV_UINPUT_OPEN_MANAGED, &joystick_device);

    libevdev_free(device);

    return joystick_device;
}
}

Gamepad::Gamepad(const std::string& name)
    : _input_device{ create_gamepad_device(name), &libevdev_uinput_destroy }
{
}