#include "guistate.h"
#include "version.h"

#include <notcurses/notcurses.h>

#include <locale.h>
#include <time.h>
#include <unistd.h>

#include <iostream>

void app_main(notcurses* nc)
{
    GuiState gui{ nc };
    gui.size_and_place();
    gui.render();

    notcurses_render(nc);

    char32_t key{};
    ncinput input{};
    while (true)
    {
        key = notcurses_get_blocking(nc, &input); 
        if (key == static_cast<char32_t>(-1))
            break;

        if(input.evtype == NCTYPE_RELEASE)
        {
            continue;
        }

        if (key == NCKEY_RESIZE)
        {
            notcurses_refresh(nc, nullptr, nullptr);
            gui.size_and_place();
        }
        else
        {
            gui.handle_input(key, &input);
        }

        gui.render();
        notcurses_render(nc);
    }
}

int main(int argc, char** argv)
{
    if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 'v')
    {
        std::cout << "vircon-terminal-client v" << VIRCON_CLIENT_VERSION << " © 2024 by RMZeroFour\n";
        return 0;
    }

    setlocale(LC_ALL, "");

    notcurses_options opts{};
    notcurses* nc{ notcurses_core_init(&opts, stdout) };

    // separate scope to ensure state is deleted before notcurses_stop
    // else, pointers are freed twice
    app_main(nc);

    notcurses_stop(nc);

    return 0;
}
