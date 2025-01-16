#pragma once

#include <notcurses/notcurses.h>

#include <string>

class TextInput
{
public:
    TextInput(ncplane* plane);

public:
    std::string text();

    void resize(int width);
    bool provide_input(const ncinput* input);
    void render(int y, int x, bool focused);

private:
    ncplane* _plane;
    int _width;

    std::string _text;
    int _index;
};