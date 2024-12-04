#pragma once

#include "gamepad.h"

#include <string>
#include <vector>

class ServerState
{
public:
    ServerState();

public:
    int count() const;

    void add_gamepad();
    void remove_gamepad();

    void update_gamepads();

private:
    std::vector<Gamepad> _gamepads;
};