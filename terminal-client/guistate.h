#pragma once

#include "textinput.h"

#include <notcurses/notcurses.h>

class GuiState
{
public:
    GuiState(notcurses* nc);
    ~GuiState();

public:
    void size_and_place();
    void handle_input(char32_t key, const ncinput* input);
    void render();

private:
    void render_background();
    void render_title();
    void render_main_screen();

private:
    ncplane* _std_plane;
    ncplane* _title_plane;
    ncplane* _main_plane;

    TextInput _client_name_input;
    TextInput _server_addr_input;
    TextInput _server_port_input;

    bool _in_options;
    int _menu_index;
};