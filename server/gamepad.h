#pragma once

#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>

#include <string>
#include <memory>

class Gamepad
{
public:
	Gamepad(const std::string& name);

private:
    std::unique_ptr<libevdev_uinput, decltype(&libevdev_uinput_destroy)> _input_device;
};
