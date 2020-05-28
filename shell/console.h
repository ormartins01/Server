#pragma once
#include <fstream>
#include <streambuf>
#include <windows.h>

class console
{
private:
    std::wstreambuf* cin_buffer, * cout_buffer, * cerr_buffer;
    std::wfstream console_input, console_output, console_error;
    const int allocated_;
    const HWND h_window_;
public:
    console(bool hide_on_start);
    ~console();
    void terminate();
};
