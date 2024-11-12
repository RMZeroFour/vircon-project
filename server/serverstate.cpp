#include "guistate.h"

ServerState::ServerState()
    : _gamepads{}
{}

size_t ServerState::new_gamepad(const std::string& name)
{
    _gamepads.emplace_back(name);
    return _gamepads.size() - 1;
}
