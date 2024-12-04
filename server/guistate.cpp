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
    , _submenu_index{ 0 }
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
    const unsigned int menu_width{ 18 };
    
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
        _menu_index = (_menu_index == 0) ? _server.count() : _menu_index - 1;
        break;
    case NCKEY_DOWN:
        _menu_index = (_menu_index == _server.count()) ? 0 : _menu_index + 1;
        break;
    
    case NCKEY_LEFT:
    case NCKEY_RIGHT:
        _submenu_index = 1 - _submenu_index;
        break;

    case NCKEY_ENTER:
        if (_menu_index == 0)
        {
            if (_submenu_index == 0)
            {
                _server.add_gamepad();
            }
            else if (_server.count() > 0)
            {
                _server.remove_gamepad();
            }
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

    unsigned int std_width{}, std_height{};
    ncplane_dim_yx(_std_plane, &std_height, &std_width);

    unsigned int title_height{ ncplane_dim_y(_title_plane) };
    unsigned int menu_width{ ncplane_dim_x(_menu_plane) };

    nccell ns{}, ew{};
    nccell_load(_std_plane, &ns, "│");
    nccell_load(_std_plane, &ew, "─");

    ncplane_perimeter_rounded(_std_plane, 0, 0, 0);
    ncplane_cursor_move_yx(_std_plane, title_height + 1, 0);
    ncplane_putstr(_std_plane, "├");
    ncplane_hline(_std_plane, &ew, std_width - 2);
    ncplane_putstr(_std_plane, "┤");
    ncplane_cursor_move_yx(_std_plane, title_height + 1, menu_width + 1);
    ncplane_putstr(_std_plane, "┬");
    ncplane_cursor_move_yx(_std_plane, title_height + 2, menu_width + 1);
    ncplane_vline(_std_plane, &ns, std_height - 2);
    ncplane_putstr(_std_plane, "┴");

    nccell_release(_std_plane, &ns);
    nccell_release(_std_plane, &ew);
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
    
    for (int i{ 0 }; i <= _server.count(); ++i)
    {
        ncplane_printf_aligned(_menu_plane, i * 2 + 1, NCALIGN_LEFT, 
            " %c ", _menu_index == i ? '>' : ' ');

        if (i == 0)
        {
            ncplane_putstr(_menu_plane, "Server Info");
        }
        else
        {
            ncplane_printf(_menu_plane, "Controller #%d", i);
        }
    }
}

void GuiState::render_info()
{
    ncplane_erase(_info_plane);

    if (_menu_index == 0)
    {
        ncplane_printf_aligned(_info_plane, ncplane_dim_y(_info_plane) - 2, NCALIGN_LEFT,
            "    %s Add Controller", _submenu_index == 0 ? ">" : " ");
        ncplane_printf_aligned(_info_plane, ncplane_dim_y(_info_plane) - 2, NCALIGN_RIGHT,
            "%s Remove Controller    ", _submenu_index == 1 ? ">" : " ");
    }
    else
    {
        ncplane_printf_aligned(_info_plane, ncplane_dim_y(_info_plane) - 2, NCALIGN_LEFT,
            "    %s Lock Controller", _submenu_index == 0 ? ">" : " ");
        ncplane_printf_aligned(_info_plane, ncplane_dim_y(_info_plane) - 2, NCALIGN_RIGHT,
            "%s Disconnect Controller    ", _submenu_index == 1 ? ">" : " ");
    }
}