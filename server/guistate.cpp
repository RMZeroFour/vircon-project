#include "guistate.h"

namespace
{
ncplane* create_plane(ncplane* parent, unsigned int rows, unsigned int cols)
{
    ncplane_options opts{
        .rows = rows,
        .cols = cols
    };
    return ncplane_create(parent, &opts);
}

void draw_qrcode(ncplane* plane, int y, ncalign_e align, const char* data)
{
    unsigned int qr_width{ 64 }, qr_height{ 64 };
    ncplane* qr_plane{ create_plane(plane, qr_height, qr_width ) };

    ncplane_qrcode(qr_plane, &qr_height, &qr_width, data, strlen(data));

    ncplane_move_yx(qr_plane, y, ncplane_halign(plane, align, qr_width));
    ncplane_mergedown_simple(qr_plane, plane);
    
    ncplane_destroy(qr_plane);
}
}

GuiState::GuiState(notcurses* nc, ServerState& server)
    : _server{ server }
    , _std_plane{ notcurses_stdplane(nc) }
    , _title_plane{ create_plane(_std_plane, 1, 1) }
    , _menu_plane{ create_plane(_std_plane, 1, 1) }
    , _info_plane{ create_plane(_std_plane, 1, 1) }
    , _menu_index{ 0 }
    , _menu_scroll{ 0 }
    , _menu_items{ "Server Info", "Add Controller" }
    , _enabled_controllers{ 0 }
    , _connected_controllers{ 0 }
{ }

GuiState::~GuiState()
{
    ncplane_destroy(_title_plane);
    ncplane_destroy(_menu_plane);
    ncplane_destroy(_info_plane);
}

void GuiState::size_and_place()
{
    const unsigned int title_height{ 3 };
    const unsigned int menu_width{ 20 };
    
    unsigned int std_width{}, std_height{};
    ncplane_dim_yx(_std_plane, &std_height, &std_width);

    ncplane_resize_simple(_title_plane, title_height, std_width - 2);
    ncplane_resize_simple(_menu_plane, std_height - title_height - 3, menu_width);
    ncplane_resize_simple(_info_plane, std_height - title_height - 3, std_width - menu_width - 3);

    ncplane_move_yx(_title_plane, 1, 1);
    ncplane_move_yx(_menu_plane, title_height + 2, 1);
    ncplane_move_yx(_info_plane, title_height + 2, menu_width + 2);
}

void GuiState::handle_input(char32_t key, ncinput input)
{
    switch (key)
    {
    case NCKEY_UP:
        _menu_index = (_menu_index == 0) ? _menu_items.size() - 1 : _menu_index - 1;
        break;
    case NCKEY_DOWN:
        _menu_index = (_menu_index == _menu_items.size() - 1) ? 0 : _menu_index + 1;
        break;
    case NCKEY_ENTER:
        if (_menu_index == _menu_items.size() - 1)
        {
            ++_enabled_controllers;

            std::string controller_id{ std::to_string(_enabled_controllers) };
            _menu_items.insert(_menu_items.end() - 1, "Controller #" + controller_id);
            _server.new_gamepad("vircon Virtual Controller #" + controller_id);
        }
        break;
    }
}

void GuiState::render()
{
    render_background();
    render_title();
    render_menu();
    render_info();
}

void GuiState::render_background()
{
    ncplane_erase(_std_plane);

    nccell ns{}, ew{}, esw{}, nes{}, swn{}, wne{};
    nccell_load(_std_plane, &ns, "│");
    nccell_load(_std_plane, &ew, "─");
    nccell_load(_std_plane, &esw, "┬");
    nccell_load(_std_plane, &nes, "├");
    nccell_load(_std_plane, &swn, "┤");
    nccell_load(_std_plane, &wne, "┴");

    unsigned int std_width{}, std_height{};
    ncplane_dim_yx(_std_plane, &std_height, &std_width);

    unsigned int title_height{ ncplane_dim_y(_title_plane) };
    unsigned int menu_width{ ncplane_dim_x(_menu_plane) };

    ncplane_perimeter_rounded(_std_plane, 0, 0, 0);
    ncplane_cursor_move_yx(_std_plane, title_height + 1, 0);
    ncplane_putc(_std_plane, &nes);
    ncplane_hline(_std_plane, &ew, std_width - 2);
    ncplane_putc(_std_plane, &swn);
    ncplane_cursor_move_yx(_std_plane, title_height + 1, menu_width + 1);
    ncplane_putc(_std_plane, &esw);
    ncplane_cursor_move_yx(_std_plane, title_height + 2, menu_width + 1);
    ncplane_vline(_std_plane, &ns, std_height - 2);
    ncplane_putc(_std_plane, &wne);

    nccell_release(_std_plane, &ew);
    nccell_release(_std_plane, &esw);
    nccell_release(_std_plane, &nes);
    nccell_release(_std_plane, &swn);
    nccell_release(_std_plane, &wne);
}

void GuiState::render_title()
{
    ncplane_erase(_title_plane);
    
    ncplane_putstr_aligned(_title_plane, 0, NCALIGN_CENTER, "┬  ┬┬┬─┐┌─┐┌─┐┌┐┌");
    ncplane_putstr_aligned(_title_plane, 1, NCALIGN_CENTER, "└┐┌┘│├┬┘│  │ ││││");
    ncplane_putstr_aligned(_title_plane, 2, NCALIGN_CENTER, " └┘ ┴┴└─└─┘└─┘┘└┘");
}

void GuiState::render_menu()
{
    ncplane_erase(_menu_plane);
    
    for (size_t i{ 0 }; i < _menu_items.size(); ++i)
    {
        ncplane_printf_aligned(_menu_plane, i * 2 + 1, NCALIGN_LEFT, 
            i == _menu_index ? " > %s" : "   %s", _menu_items[i].c_str());
    }
}

void GuiState::render_info()
{
    ncplane_erase(_info_plane);

    if (_menu_index == 0)
    {
        // placeholders
        ncplane_printf_aligned(_info_plane, 1, NCALIGN_CENTER, "Connected: %d/%d", _connected_controllers, _enabled_controllers);
        ncplane_printf_aligned(_info_plane, 3, NCALIGN_CENTER, "ws://255.255.255.255:8000");
        draw_qrcode(_info_plane, 4, NCALIGN_CENTER, "FFFFFFFFFFFF");
    }
    else if (_menu_index == _menu_items.size() - 1)
    {
        ncplane_printf_aligned(_info_plane, 1, NCALIGN_CENTER, "Add Controller #%d?", _enabled_controllers + 1);
    }
}
