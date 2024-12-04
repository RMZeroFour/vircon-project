#include "guistate.h"

ServerState::ServerState()
    : _gamepads{}
{}

int ServerState::count() const
{
    return _gamepads.size();
}

void ServerState::add_gamepad()
{
    _gamepads.emplace_back();
}

void ServerState::remove_gamepad()
{
    _gamepads.pop_back();
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

    for (Gamepad& gp : _gamepads)
    {
        gp.send_input(ss);
    }
}
