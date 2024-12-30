#include "gamepad.h"

namespace
{
libevdev_uinput* create_gamepad_device()
{
    const char* device_name{ "vircon Virtual Controller" };

    libevdev* device = libevdev_new();
    libevdev_set_name(device, device_name);
    libevdev_set_id_product(device, 0);
	libevdev_set_id_vendor(device, 0);
	libevdev_set_id_version(device, 1);
	libevdev_set_id_bustype(device, BUS_VIRTUAL);

    libevdev_enable_event_type(device, EV_KEY);
    libevdev_enable_event_code(device, EV_KEY, BTN_A, nullptr); // A
    libevdev_enable_event_code(device, EV_KEY, BTN_B, nullptr); // B
    libevdev_enable_event_code(device, EV_KEY, BTN_X, nullptr); // X
    libevdev_enable_event_code(device, EV_KEY, BTN_Y, nullptr); // Y
    libevdev_enable_event_code(device, EV_KEY, BTN_TL, nullptr); // L1
    libevdev_enable_event_code(device, EV_KEY, BTN_TR, nullptr); // R1
    libevdev_enable_event_code(device, EV_KEY, BTN_TL2, nullptr); // L2
    libevdev_enable_event_code(device, EV_KEY, BTN_TR2, nullptr); // R2
    libevdev_enable_event_code(device, EV_KEY, BTN_DPAD_UP, nullptr); // UP
    libevdev_enable_event_code(device, EV_KEY, BTN_DPAD_DOWN, nullptr); // DOWN
    libevdev_enable_event_code(device, EV_KEY, BTN_DPAD_LEFT, nullptr); // LEFT
    libevdev_enable_event_code(device, EV_KEY, BTN_DPAD_RIGHT, nullptr); // RIGHT
    libevdev_enable_event_code(device, EV_KEY, BTN_SELECT, nullptr); // SELECT
    libevdev_enable_event_code(device, EV_KEY, BTN_START, nullptr); // START

    input_absinfo axis_info
    {
        .minimum = -32768,
        .maximum = 32767,
    };
    libevdev_enable_event_type(device, EV_ABS);
    libevdev_enable_event_code(device, EV_ABS, ABS_X, &axis_info); // ANALOG LX
    libevdev_enable_event_code(device, EV_ABS, ABS_Y, &axis_info); // ANALOG LY
    libevdev_enable_event_code(device, EV_ABS, ABS_RX, &axis_info); // ANALOG RX
    libevdev_enable_event_code(device, EV_ABS, ABS_RY, &axis_info); // ANALOG RY

    libevdev_uinput* uinput = nullptr;
    libevdev_uinput_create_from_device(device, LIBEVDEV_UINPUT_OPEN_MANAGED, &uinput);

    libevdev_free(device);

    return uinput;
}
}

Gamepad::Gamepad()
    : _input_device{ create_gamepad_device(), &libevdev_uinput_destroy }
{ }

void Gamepad::send_input(const Snapshot& ss)
{
    libevdev_uinput* uinput{ _input_device.get() };
    libevdev_uinput_write_event(uinput, EV_KEY, BTN_A, ss.a);
    libevdev_uinput_write_event(uinput, EV_KEY, BTN_B, ss.b);
    libevdev_uinput_write_event(uinput, EV_KEY, BTN_X, ss.x);
    libevdev_uinput_write_event(uinput, EV_KEY, BTN_Y, ss.y);
    libevdev_uinput_write_event(uinput, EV_KEY, BTN_TL, ss.l1);
    libevdev_uinput_write_event(uinput, EV_KEY, BTN_TR, ss.r1);
    libevdev_uinput_write_event(uinput, EV_KEY, BTN_TL2, ss.l2);
    libevdev_uinput_write_event(uinput, EV_KEY, BTN_TR2, ss.r2);
    libevdev_uinput_write_event(uinput, EV_KEY, BTN_DPAD_UP, ss.up);
    libevdev_uinput_write_event(uinput, EV_KEY, BTN_DPAD_DOWN, ss.down);
    libevdev_uinput_write_event(uinput, EV_KEY, BTN_DPAD_LEFT, ss.left);
    libevdev_uinput_write_event(uinput, EV_KEY, BTN_DPAD_RIGHT, ss.right);
    libevdev_uinput_write_event(uinput, EV_KEY, BTN_SELECT, ss.select);
    libevdev_uinput_write_event(uinput, EV_KEY, BTN_START, ss.start);
    libevdev_uinput_write_event(uinput, EV_ABS, ABS_X, ss.lx);
    libevdev_uinput_write_event(uinput, EV_ABS, ABS_Y, ss.ly);
    libevdev_uinput_write_event(uinput, EV_ABS, ABS_RX, ss.rx);
    libevdev_uinput_write_event(uinput, EV_ABS, ABS_RY, ss.ry);
    libevdev_uinput_write_event(uinput, EV_SYN, SYN_REPORT, 0);
}
