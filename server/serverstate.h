#pragma once

#include "gamepad.h"

#include <string>
#include <vector>

class ServerState
{
public:
    ServerState();

public:
    size_t new_gamepad();

private:
    std::vector<Gamepad> _gamepads;
};