#include "guistate.h"
#include "serverstate.h"
#include "version.h"

#include <notcurses/notcurses.h>

#include <locale.h>
#include <time.h>
#include <unistd.h>

#include <iostream>

void add_milliseconds(const timespec& source, timespec& target, int delay_ms)
{
    target.tv_nsec = source.tv_nsec + 1'000'000 * delay_ms;
    target.tv_sec = source.tv_sec + target.tv_nsec / 1'000'000'000;
    target.tv_nsec %= 1'000'000'000;
}

int main(int argc, char** argv)
{
    if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 'v')
    {
        std::cout << "vircon-server v" << VIRCON_SERVER_VERSION << " © 2024 by RMZeroFour\n";
        return 0;
    }

    setlocale(LC_ALL, "");

    notcurses_options opts{};
    notcurses* nc{ notcurses_core_init(&opts, stdout) };

    ServerState server{};

    GuiState gui{ nc, server };
    gui.size_and_place();
    gui.render();

    notcurses_render(nc);

    const int timeout_ms{ 250 };
    timespec now{}, deadline{};
    
    char32_t key{};
    ncinput input{};
    while (true)
    {
        clock_gettime(CLOCK_MONOTONIC, &now);
        add_milliseconds(now, deadline, timeout_ms);
        
        key = notcurses_get(nc, &deadline, &input);
        if (key == (char32_t)-1)
            break;

        if (key == NCKEY_RESIZE)
        {
            notcurses_refresh(nc, nullptr, nullptr);
            gui.size_and_place();
        }
        else
        {
            gui.handle_input(key, input);
        }

        gui.render();

        notcurses_render(nc);
    }

    notcurses_stop(nc);

    return 0;
}
