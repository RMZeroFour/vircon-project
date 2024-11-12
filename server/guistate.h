#pragma once

#include "serverstate.h"

#include <notcurses/notcurses.h>

#include <string>
#include <vector>

class GuiState
{
public:
    GuiState(notcurses* nc, ServerState& server);
    ~GuiState();

public:
    void size_and_place();
    void handle_input(char32_t key, ncinput input);
    void render();

private:
    void render_background();
    void render_title();
    void render_menu();
    void render_info();

private:
    ServerState& _server;

    ncplane* _std_plane;
    ncplane* _title_plane;
    ncplane* _menu_plane;
    ncplane* _info_plane;

    int _menu_index;
    int _menu_scroll;
    std::vector<std::string> _menu_items;

    int _enabled_controllers;
    int _connected_controllers;
};