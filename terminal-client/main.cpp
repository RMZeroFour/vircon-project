#include "version.h"

#include <iostream>

int main(int argc, char** argv)
{
    if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 'v')
    {
        std::cout << "vircon-terminal-client v" << VIRCON_CLIENT_VERSION << " © 2024 by RMZeroFour\n";
        return 0;
    }

    return 0;
}
