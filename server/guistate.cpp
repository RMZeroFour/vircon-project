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
}

GuiState::GuiState(notcurses* nc)
    : _std_plane{ notcurses_stdplane(nc) }
    , _title_plane{ create_plane(_std_plane, 1, 1) }
    , _menu_plane{ create_plane(_std_plane, 1, 1) }
    , _info_plane{ create_plane(_std_plane, 1, 1) }
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
    
}

void GuiState::render_info()
{
    
}
