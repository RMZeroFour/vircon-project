#include "guistate.h"

#include <notcurses/notcurses.h>

#include <locale.h>
#include <unistd.h>

#include <iostream>

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "");

    notcurses_options opts{};
    notcurses* nc{ notcurses_core_init(&opts, stdout) };

    GuiState gui{ nc };
    gui.size_and_place();
    gui.render();

    notcurses_render(nc);
    
    char32_t key{};
    ncinput input{};
    while (true)
    {
        key = notcurses_get_blocking(nc, &input);
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
