#pragma once

#include "snapshot.h"

#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>

#include <memory>
#include <string>

class Gamepad
{
public:
	Gamepad();

public:
    void send_input(const Snapshot& ss);

private:
    std::unique_ptr<libevdev_uinput, decltype(&libevdev_uinput_destroy)> _input_device;
};
