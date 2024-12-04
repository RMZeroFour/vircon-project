#include "guistate.h"

ServerState::ServerState()
    : _gamepads{}
{}

size_t ServerState::new_gamepad()
{
    _gamepads.emplace_back();
    return _gamepads.size() - 1;
}
