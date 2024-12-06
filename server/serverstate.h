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
    
    bool is_connected(int index) const;
    bool is_locked(int index) const;
    const std::string& client_name(int index) const;
    const Snapshot& latest_snapshot(int index) const;

    void toggle_locked(int index);

    void add_gamepad();
    void remove_gamepad();

    void update_gamepads();

private:
    struct GamepadState
    {
        Gamepad gamepad;
        bool is_connected = true;
        bool is_locked;
        std::string client_name = "Samsoong Noot Nein";
        Snapshot latest_snapshot;
    };

private:
    std::vector<GamepadState> _gamepad_states;
};