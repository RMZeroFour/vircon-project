#include "guistate.h"

ServerState::ServerState()
    : _gamepad_states{}
{}

int ServerState::count() const
{
    return _gamepad_states.size();
}

bool ServerState::is_connected(int index) const
{
    return _gamepad_states.at(index).is_connected;
}

bool ServerState::is_locked(int index) const
{
    return _gamepad_states.at(index).is_locked;
}

const std::string& ServerState::client_name(int index) const
{
    return _gamepad_states.at(index).client_name;
}

const Snapshot& ServerState::latest_snapshot(int index) const
{
    return _gamepad_states.at(index).latest_snapshot;
}

void ServerState::toggle_locked(int index)
{
    _gamepad_states[index].is_locked = !_gamepad_states[index].is_locked;
}

void ServerState::add_gamepad()
{
    _gamepad_states.emplace_back();
}

void ServerState::remove_gamepad()
{
    _gamepad_states.pop_back();
}

void ServerState::update_gamepads()
{
    bool on = rand() % 10 > 5;
    int16_t xoff = rand() % 10 > 5 ? 30000 : (rand() % 10 > 5 ? -30000 : 0);
    int16_t yoff = rand() % 10 > 5 ? 30000 : (rand() % 10 > 5 ? -30000 : 0);

    Snapshot ss{
        .a = on, 
        .l2 = on, 
        .down = on,
        .lx = xoff, 
        .ly = yoff, 
        .rx = xoff, 
        .ry = yoff
    };
    
    for (GamepadState& gps : _gamepad_states)
    {
        gps.latest_snapshot = ss;
        if (!gps.is_locked)
        {
            gps.gamepad.send_input(ss);
        }
    }
}
