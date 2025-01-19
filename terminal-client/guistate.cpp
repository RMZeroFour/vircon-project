#include "guistate.h"

namespace
{
ncplane* create_plane(ncplane* parent, unsigned int rows, unsigned int cols)
{
    ncplane_options opts
    {
        .rows = rows,
        .cols = cols
    };
    return ncplane_create(parent, &opts);
}

void fill_rect(ncplane* plane, int y, int x, int h, int w, const nccell* c)
{
    for (int yoff{ 0 }; yoff < h; ++yoff)
    {
        ncplane_cursor_move_yx(plane, y + yoff, x);
        for (int xoff{ 0 }; xoff < w; ++xoff)
        {
            ncplane_putc(plane, c);
        }
    }
}

void draw_controller(ncplane* plane, int y, ncalign_e align, bool leftjs, bool rightjs)
{
    const unsigned int ctrl_width{ 56 }, ctrl_height{ 12 };
    ncplane* ctrl_plane{ create_plane(plane, ctrl_height, ctrl_width ) };
    
    nccell box{};
    nccell_load(ctrl_plane, &box, "█");
    
    // Left side of controller
    ncplane_cursor_move_yx(ctrl_plane, 0, 0);
    ncplane_rounded_box_sized(ctrl_plane, 0, 0, 3, 8, 0);
    ncplane_putstr_yx(ctrl_plane, 1, 3, "L1");
    
    ncplane_cursor_move_yx(ctrl_plane, 0, 8);
    ncplane_rounded_box_sized(ctrl_plane, 0, 0, 3, 8, 0);
    ncplane_putstr_yx(ctrl_plane, 1, 11, "L2");

    if (leftjs)
    {
        ncplane_cursor_move_yx(ctrl_plane, 4, 0);
        ncplane_rounded_box_sized(ctrl_plane, 0, 0, 8, 16, 0);
        fill_rect(ctrl_plane, 7, 6, 2, 4, &box);
    }
    else
    {
        ncplane_cursor_move_yx(ctrl_plane, 4, 5);
        ncplane_rounded_box_sized(ctrl_plane, 0, 0, 3, 5, 0);
        ncplane_putstr_yx(ctrl_plane, 5, 7, "⮝");

        ncplane_cursor_move_yx(ctrl_plane, 6, 0);
        ncplane_rounded_box_sized(ctrl_plane, 0, 0, 3, 5, 0);
        ncplane_putstr_yx(ctrl_plane, 7, 2, "⮜");

        ncplane_cursor_move_yx(ctrl_plane, 6, 10);
        ncplane_rounded_box_sized(ctrl_plane, 0, 0, 3, 5, 0);
        ncplane_putstr_yx(ctrl_plane, 7, 12, "⮞");

        ncplane_cursor_move_yx(ctrl_plane, 8, 5);
        ncplane_rounded_box_sized(ctrl_plane, 0, 0, 3, 5, 0);
        ncplane_putstr_yx(ctrl_plane, 9, 7, "⮟");
    }
    
    // // Center of gamepad
    ncplane_cursor_move_yx(ctrl_plane, 5, 23);
    ncplane_rounded_box_sized(ctrl_plane, 0, 0, 3, 10, 0);
    ncplane_putstr_yx(ctrl_plane, 6, 25, "Start");

    ncplane_cursor_move_yx(ctrl_plane, 8, 23);
    ncplane_rounded_box_sized(ctrl_plane, 0, 0, 3, 10, 0);
    ncplane_putstr_yx(ctrl_plane, 9, 25, "Select");

    // Right side of gamepad
    ncplane_cursor_move_yx(ctrl_plane, 0, 40);
    ncplane_rounded_box_sized(ctrl_plane, 0, 0, 3, 8, 0);
    ncplane_putstr_yx(ctrl_plane, 1, 43, "R2");
    
    ncplane_cursor_move_yx(ctrl_plane, 0, 48);
    ncplane_rounded_box_sized(ctrl_plane, 0, 0, 3, 8, 0);
    ncplane_putstr_yx(ctrl_plane, 1, 51, "R1");
    if (rightjs)
    {
        ncplane_cursor_move_yx(ctrl_plane, 4, 40);
        ncplane_rounded_box_sized(ctrl_plane, 0, 0, 8, 16, 0);        
        fill_rect(ctrl_plane, 7, 46, 2, 4, &box);
    }
    else
    {
        ncplane_cursor_move_yx(ctrl_plane, 4, 45);
        ncplane_rounded_box_sized(ctrl_plane, 0, 0, 3, 5, 0);
        ncplane_putstr_yx(ctrl_plane, 5, 47, "Y");

        ncplane_cursor_move_yx(ctrl_plane, 6, 40);
        ncplane_rounded_box_sized(ctrl_plane, 0, 0, 3, 5, 0);
        ncplane_putstr_yx(ctrl_plane, 7, 42, "X");

        ncplane_cursor_move_yx(ctrl_plane, 6, 50);
        ncplane_rounded_box_sized(ctrl_plane, 0, 0, 3, 5, 0);
        ncplane_putstr_yx(ctrl_plane, 7, 52, "B");

        ncplane_cursor_move_yx(ctrl_plane, 8, 45);
        ncplane_rounded_box_sized(ctrl_plane, 0, 0, 3, 5, 0);
        ncplane_putstr_yx(ctrl_plane, 9, 47, "A");
    }

    nccell_release(ctrl_plane, &box);

    ncplane_move_yx(ctrl_plane, y, ncplane_halign(plane, align, ctrl_width));
    ncplane_mergedown_simple(ctrl_plane, plane);

    ncplane_destroy(ctrl_plane);
}
}

GuiState::GuiState(notcurses* nc)
    : _std_plane{ notcurses_stdplane(nc) }
    , _title_plane{ create_plane(_std_plane, 1, 1) }
    , _main_plane{ create_plane(_std_plane, 1, 1) }
    , _client_name_input{ _main_plane }
    , _server_addr_input{ _main_plane }
    , _server_port_input{ _main_plane }
    , _in_options{ true }
    , _menu_index{ 0 }
    , _controller_layout{ 0 }
{ }

GuiState::~GuiState()
{
    ncplane_destroy(_title_plane);
    ncplane_destroy(_main_plane);
}

void GuiState::size_and_place()
{
    const unsigned int title_height{ 3 };
    
    unsigned int std_width{}, std_height{};
    ncplane_dim_yx(_std_plane, &std_height, &std_width);

    ncplane_resize_simple(_title_plane, title_height, std_width - 2);
    ncplane_resize_simple(_main_plane, std_height - title_height - 3, std_width - 2);

    ncplane_move_yx(_title_plane, 1, 1);
    ncplane_move_yx(_main_plane, title_height + 2, 1);

    _client_name_input.resize(std_width - 24);
    _server_addr_input.resize(std_width - 24);
    _server_port_input.resize(std_width - 24);
}

void GuiState::handle_input(char32_t key, const ncinput* input)
{
    if (_in_options)
    {
        if (_menu_index == 0)
        {
            if (_client_name_input.provide_input(input))
            {
                return;
            }
        }
        else if (_menu_index == 1)
        {
            if (_server_addr_input.provide_input(input))
            {
                return;
            }
        }
        else if (_menu_index == 2)
        {
            if (_server_port_input.provide_input(input))
            {
                return;
            }
        }
                
        switch (key)
        {
        case NCKEY_UP:
            _menu_index = (_menu_index == 0) ? 3 : _menu_index - 1;
            break;
        case NCKEY_DOWN:
            _menu_index = (_menu_index == 3) ? 0 : _menu_index + 1;
            break;

        case NCKEY_ENTER:
            if (_menu_index == 3)
            {
                _in_options = false;
                _menu_index = 1;
            }
            else
            {
                ++_menu_index;
            }
            break;
        }
    }
    else
    {
        switch (key)
        {
        case NCKEY_LEFT:
        case NCKEY_RIGHT:
            _menu_index = 1 - _menu_index;
            break;

        case NCKEY_ENTER:
            if (_menu_index == 0)
            {
                _controller_layout = (_controller_layout + 1) % 4;
            }
            else
            {
                _in_options = true;
                _menu_index = 3;
            }
            break;
        }
    }
}

void GuiState::render()
{
    render_background();
    render_title();
    render_main_screen();
}

void GuiState::render_background()
{
    ncplane_erase(_std_plane);

    unsigned int std_width{}, std_height{};
    ncplane_dim_yx(_std_plane, &std_height, &std_width);

    unsigned int title_height{ ncplane_dim_y(_title_plane) };

    nccell ew{};
    nccell_load(_std_plane, &ew, "─");

    ncplane_perimeter_rounded(_std_plane, 0, 0, 0);
    ncplane_cursor_move_yx(_std_plane, title_height + 1, 0);
    ncplane_putstr(_std_plane, "├");
    ncplane_hline(_std_plane, &ew, std_width - 2);
    ncplane_putstr(_std_plane, "┤");

    nccell_release(_std_plane, &ew);
}

void GuiState::render_title()
{
    ncplane_erase(_title_plane);
    
    ncplane_putstr_aligned(_title_plane, 0, NCALIGN_CENTER, "┬  ┬┬┬─┐┌─┐┌─┐┌┐┌");
    ncplane_putstr_aligned(_title_plane, 1, NCALIGN_CENTER, "└┐┌┘│├┬┘│  │ ││││");
    ncplane_putstr_aligned(_title_plane, 2, NCALIGN_CENTER, " └┘ ┴┴└─└─┘└─┘┘└┘");
}

void GuiState::render_main_screen()
{
    ncplane_erase(_main_plane);
    
    if (_in_options)
    {
        ncplane_putstr_yx(_main_plane, 1, 2, "Client Name    :");
        _client_name_input.render(1, 20, _menu_index == 0);
        ncplane_putstr_yx(_main_plane, 2, 2, "Server Address :");
        _server_addr_input.render(2, 20, _menu_index == 1);
        ncplane_putstr_yx(_main_plane, 3, 2, "Server Port    :");
        _server_port_input.render(3, 20, _menu_index == 2);

        ncplane_printf_aligned(_main_plane, ncplane_dim_y(_main_plane) - 2, NCALIGN_RIGHT,
            "%c Connect To Server    ", _menu_index == 3 ? '>' : ' ');
    }
    else
    {
        switch (_controller_layout)
        {
        case 0:
            ncplane_putstr_aligned(_main_plane, 1, NCALIGN_LEFT, "    Layout A");
            draw_controller(_main_plane, 3, NCALIGN_CENTER, true, false);
            break;
        case 1:
            ncplane_putstr_aligned(_main_plane, 1, NCALIGN_LEFT, "    Layout B");
            draw_controller(_main_plane, 3, NCALIGN_CENTER, false, true);
            break;
        case 2:
            ncplane_putstr_aligned(_main_plane, 1, NCALIGN_LEFT, "    Layout C");
            draw_controller(_main_plane, 3, NCALIGN_CENTER, false, false);
            break;
        case 3:
            ncplane_putstr_aligned(_main_plane, 1, NCALIGN_LEFT, "    Layout D");
            draw_controller(_main_plane, 3, NCALIGN_CENTER, true, true);
            break;
        }

        ncplane_printf_aligned(_main_plane, ncplane_dim_y(_main_plane) - 2, NCALIGN_LEFT,
            "    %c Toggle Layout", _menu_index == 0 ? '>' : ' ');

        ncplane_printf_aligned(_main_plane, ncplane_dim_y(_main_plane) - 2, NCALIGN_RIGHT,
            "%c Return To Options    ", _menu_index == 1 ? '>' : ' ');
    }
}