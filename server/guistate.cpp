#include "guistate.h"

#include <array>
#include <format>
#include <limits>
#include <sstream>

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

std::string get_compressed_address(const std::string& host, uint16_t port)
{
    std::array<uint8_t, 4> host_bytes{};
    int index{ 0 };
    
    std::istringstream host_parts{ host };
    std::string part{};
    while (std::getline(host_parts, part, '.'))
    {
        host_bytes[index++] = std::stoi(part);
    }

    return std::format("{:2X}{:2X}{:2X}{:2X}{:4X}", 
        host_bytes[0], host_bytes[1], host_bytes[2], host_bytes[3], port);
}

void draw_qrcode(ncplane* plane, int y, int x, const char* data)
{
    unsigned int qr_width{ 64 }, qr_height{ 64 };
    ncplane* qr_plane{ create_plane(plane, qr_height, qr_width ) };

    ncplane_qrcode(qr_plane, &qr_height, &qr_width, data, strlen(data));

    ncplane_move_yx(qr_plane, y, x);
    ncplane_mergedown_simple(qr_plane, plane);
    
    ncplane_destroy(qr_plane);
}

void draw_joystick(ncplane* plane, nccell* indicator, int y, int x, int16_t ver, int16_t hor)
{
    const unsigned int js_width{ 9 }, js_height{ 5 };
    ncplane* js_plane{ create_plane(plane, js_height, js_width ) };

    const int16_t threshold{ std::numeric_limits<int16_t>::max() / 3 };
    int yoff = ver > threshold ? +1 : (ver < -threshold ? -1 : 0);
    int xoff = hor > threshold ? +1 : (hor < -threshold ? -1 : 0);

    ncplane_perimeter_rounded(js_plane, 0, 0, 0);
    ncplane_putc_yx(js_plane, 2 + yoff, 4 + xoff * 2, indicator);

    ncplane_move_yx(js_plane, y, x);
    ncplane_mergedown_simple(js_plane, plane);
    
    ncplane_destroy(js_plane);
}

void draw_snapshot(ncplane* plane, int y, ncalign_e align, const Snapshot& ss)
{
    const unsigned int ss_width{ 36 }, ss_height{ 8 };
    ncplane* ss_plane{ create_plane(plane, ss_height, ss_width ) };
    
    nccell filled{}, empty{};
    nccell_load(ss_plane, &filled, "●");
    nccell_load(ss_plane, &empty, "○");
    
    // Left side of gamepad
    ncplane_putc_yx(ss_plane, 0, 2, ss.l1 ? &filled : &empty);
    ncplane_printf(ss_plane, " L1");
    ncplane_putc_yx(ss_plane, 0, 8, ss.l2 ? &filled : &empty);
    ncplane_printf(ss_plane, " L2");

    draw_joystick(ss_plane, &filled, 1, 2, ss.ly, ss.lx);

    ncplane_putc_yx(ss_plane, 6, 0, ss.up ? &filled : &empty);
    ncplane_printf(ss_plane, " Up");
    ncplane_putc_yx(ss_plane, 7, 0, ss.down ? &filled : &empty);
    ncplane_printf(ss_plane, " Down");
    ncplane_putc_yx(ss_plane, 6, 7, ss.left ? &filled : &empty);
    ncplane_printf(ss_plane, " Left");
    ncplane_putc_yx(ss_plane, 7, 7, ss.right ? &filled : &empty);
    ncplane_printf(ss_plane, " Right");
    
    // Center of gamepad
    ncplane_putc_yx(ss_plane, 2, 14, ss.select ? &filled : &empty);
    ncplane_printf(ss_plane, " Select");
    ncplane_putc_yx(ss_plane, 3, 14, ss.start ? &filled : &empty);
    ncplane_printf(ss_plane, " Start");

    // Right side of gamepad
    ncplane_putc_yx(ss_plane, 0, 24, ss.r1 ? &filled : &empty);
    ncplane_printf(ss_plane, " R1");
    ncplane_putc_yx(ss_plane, 0, 30, ss.r2 ? &filled : &empty);
    ncplane_printf(ss_plane, " R2");

    draw_joystick(ss_plane, &filled, 1, 24, ss.ry, ss.rx);

    ncplane_putc_yx(ss_plane, 6, 24, ss.a ? &filled : &empty);
    ncplane_printf(ss_plane, " A");
    ncplane_putc_yx(ss_plane, 7, 24, ss.b ? &filled : &empty);
    ncplane_printf(ss_plane, " B");
    ncplane_putc_yx(ss_plane, 6, 31, ss.x ? &filled : &empty);
    ncplane_printf(ss_plane, " X");
    ncplane_putc_yx(ss_plane, 7, 31, ss.y ? &filled : &empty);            
    ncplane_printf(ss_plane, " Y");

    nccell_release(ss_plane, &filled);
    nccell_release(ss_plane, &empty);

    ncplane_move_yx(ss_plane, y, ncplane_halign(plane, align, ss_width));
    ncplane_mergedown_simple(ss_plane, plane);

    ncplane_destroy(ss_plane);
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

void GuiState::handle_input(char32_t key, const ncinput* input)
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
        else
        {
            int gp_index{ _menu_index - 1 };
        
            if (_submenu_index == 0)
            {
                _server.toggle_locked(gp_index);
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
        const std::string& host{ _server.server_host() };
        uint16_t port{ _server.server_port() };

        ncplane_printf_aligned(_info_plane, 1, NCALIGN_LEFT, "    Host: %s", host.c_str());
        ncplane_printf_aligned(_info_plane, 2, NCALIGN_LEFT, "    Port: %d", port);
        
        ncplane_printf_aligned(_info_plane, 4, NCALIGN_LEFT, "    Enabled: %d", _server.count());
        ncplane_printf_aligned(_info_plane, 5, NCALIGN_LEFT, "    Connected: %d", _server.count());
        ncplane_printf_aligned(_info_plane, 6, NCALIGN_LEFT, "    Unlocked: %d", _server.count());
        
        draw_qrcode(_info_plane, 1, ncplane_dim_x(_info_plane) - 29, get_compressed_address(host, port).c_str());

        ncplane_printf_aligned(_info_plane, ncplane_dim_y(_info_plane) - 2, NCALIGN_LEFT,
            "    %c Add Controller", _submenu_index == 0 ? '>' : ' ');
        ncplane_printf_aligned(_info_plane, ncplane_dim_y(_info_plane) - 2, NCALIGN_RIGHT,
            "%c Remove Controller    ", _submenu_index == 1 ? '>' : ' ');
    }
    else
    {
        int gp_index{ _menu_index - 1 };

        if (_server.is_connected(gp_index))
        {
            ncplane_putstr_aligned(_info_plane, 1, NCALIGN_CENTER, 
                _server.client_name(gp_index).c_str());
            ncplane_putstr_aligned(_info_plane, 2, NCALIGN_CENTER, 
                _server.is_locked(gp_index) ? "Locked" : "Unlocked");

            draw_snapshot(_info_plane, 5, NCALIGN_CENTER, _server.latest_snapshot(gp_index));
        }
        else
        {
            ncplane_putstr_aligned(_info_plane, 1, NCALIGN_CENTER, "Not Connected");
        }

        ncplane_printf_aligned(_info_plane, ncplane_dim_y(_info_plane) - 2, NCALIGN_LEFT,
            "    %c %s Controller", _submenu_index == 0 ? '>' : ' ', _server.is_locked(gp_index) ? "Unlock" : "Lock");
        ncplane_printf_aligned(_info_plane, ncplane_dim_y(_info_plane) - 2, NCALIGN_RIGHT,
            "%c Disconnect Controller    ", _submenu_index == 1 ? '>' : ' ');
    }
}