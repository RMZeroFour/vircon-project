#include "textinput.h"

namespace
{
void pop_back_utf8(std::string& text)
{
    if (!text.empty())
    {
        for (size_t i{ text.size() - 1 }; i >= 0; --i)
        {
            if ((text[i] & 0xC0) != 0x80)
            {
                text.erase(i);
                return;
            }
        }
    }
}
}

TextInput::TextInput(ncplane *plane)
    : _plane{ plane }
    , _width{}
    , _text{}
    , _index{}
{
}

std::string TextInput::text()
{
    return _text;
}

void TextInput::resize(int width)
{
    _width = width;
}

bool TextInput::provide_input(const ncinput *input)
{
    if (input->id == NCKEY_ENTER || input->id == NCKEY_UP || input->id == NCKEY_DOWN)
    {
        return false;
    }
    
    if (input->id == NCKEY_BACKSPACE)
    {
        pop_back_utf8(_text);
    }
    else
    {
        _text += input->utf8;
    }
    
    return true;
}

void TextInput::render(int y, int x, bool focused)
{
    int offset{ 0 };
    if (_text.size() > _width - 1)
    {
        offset = _text.size() - _width + 1;
    }
    
    ncplane_printf_yx(_plane, y, x, "%s%s", 
        _text.c_str() + offset, focused ? "█" : "");
}
