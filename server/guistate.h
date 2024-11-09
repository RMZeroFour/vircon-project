#pragma once

#include <notcurses/notcurses.h>

class GuiState
{
public:
    GuiState(notcurses* nc);
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
    ncplane* _std_plane;
    ncplane* _title_plane;
    ncplane* _menu_plane;
    ncplane* _info_plane;
};