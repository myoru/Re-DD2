#include "Keyboard.h"

void Keyboard::Update()
{
    KeyboardButton _new_input_key = 0;

    if (GetAsyncKeyState('W') & 0x8000)_new_input_key |= W;
    if (GetAsyncKeyState('A') & 0x8000)_new_input_key |= A;
    if (GetAsyncKeyState('S') & 0x8000)_new_input_key |= S;
    if (GetAsyncKeyState('D') & 0x8000)_new_input_key |= D;
    if (GetAsyncKeyState('Z') & 0x8000)_new_input_key |= Z;
    if (GetAsyncKeyState('X') & 0x8000)_new_input_key |= X;
    if (GetAsyncKeyState('C') & 0x8000)_new_input_key |= C;
    if (GetAsyncKeyState('V') & 0x8000)_new_input_key |= V;
    if (GetAsyncKeyState('Q') & 0x8000)_new_input_key |= Q;
    if (GetAsyncKeyState('E') & 0x8000)_new_input_key |= E;
    if (GetAsyncKeyState('R') & 0x8000)_new_input_key |= R;
    if (GetAsyncKeyState('F') & 0x8000)_new_input_key |= F;
    if (GetAsyncKeyState('V') & 0x8000)_new_input_key |= V;
    if (GetAsyncKeyState('I') & 0x8000)_new_input_key |= I;
    if (GetAsyncKeyState('J') & 0x8000)_new_input_key |= J;
    if (GetAsyncKeyState('K') & 0x8000)_new_input_key |= K;
    if (GetAsyncKeyState('L') & 0x8000)_new_input_key |= L;
    if (GetAsyncKeyState('U') & 0x8000)_new_input_key |= U;
    if (GetAsyncKeyState('O') & 0x8000)_new_input_key |= O;
    if (GetAsyncKeyState('P') & 0x8000)_new_input_key |= P;
    if (GetAsyncKeyState('N') & 0x8000)_new_input_key |= N;
    if (GetAsyncKeyState(VK_SPACE) & 0x8000)_new_input_key |= SPACE;
    if (GetAsyncKeyState(VK_SHIFT) & 0x8000)_new_input_key |= SHIFT;
    if (GetAsyncKeyState(VK_TAB) & 0x8000)_new_input_key |= TAB;
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)_new_input_key |= ESC;
    if (GetAsyncKeyState(VK_RETURN) & 0x8000)_new_input_key |= ENTER;
    if (GetAsyncKeyState(VK_CONTROL) & 0x8000)_new_input_key |= CTRL;
    if (GetAsyncKeyState(VK_UP) & 0x8000)_new_input_key |= UP;
    if (GetAsyncKeyState(VK_DOWN) & 0x8000)_new_input_key |= DOWN;
    if (GetAsyncKeyState(VK_LEFT) & 0x8000)_new_input_key |= LEFT;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)_new_input_key |= RIGHT;
    if (GetAsyncKeyState(VK_F1) & 0x8000)_new_input_key |= F1;
    if (GetAsyncKeyState(VK_F2) & 0x8000)_new_input_key |= F2;
    if (GetAsyncKeyState(VK_F3) & 0x8000)_new_input_key |= F3;
    if (GetAsyncKeyState(VK_F4) & 0x8000)_new_input_key |= F4;
    if (GetAsyncKeyState(VK_F5) & 0x8000)_new_input_key |= F5;
    if (GetAsyncKeyState(VK_F6) & 0x8000)_new_input_key |= F6;
    if (GetAsyncKeyState(VK_F7) & 0x8000)_new_input_key |= F7;
    if (GetAsyncKeyState('1') & 0x8000)_new_input_key |= Key1;
    if (GetAsyncKeyState('2') & 0x8000)_new_input_key |= Key2;
    if (GetAsyncKeyState('3') & 0x8000)_new_input_key |= Key3;
    if (GetAsyncKeyState('4') & 0x8000)_new_input_key |= Key4;
    if (GetAsyncKeyState('5') & 0x8000)_new_input_key |= Key5;
    if (GetAsyncKeyState('6') & 0x8000)_new_input_key |= Key6;
    if (GetAsyncKeyState('7') & 0x8000)_new_input_key |= Key7;
    if (GetAsyncKeyState('8') & 0x8000)_new_input_key |= Key8;
    if (GetAsyncKeyState('9') & 0x8000)_new_input_key |= Key9;
    if (GetAsyncKeyState('0') & 0x8000)_new_input_key |= Key0;

    input_key[1] = input_key[0];
    input_key[0] = _new_input_key;
}

bool Keyboard::GetKeyInput(KeyboardButton button, int state)
{
    switch (state)
    {
    case Up:
        if (~input_key[0] & ~input_key[1] & button)
        {
            return true;
        }
        break;
    case Down:
        if (input_key[0] & input_key[1] & button)
        {
            return true;
        }
        break;
    case UpMoment:
        if (~input_key[0] & input_key[1] & button)
        {
            return true;
        }
        break;
    case DownMoment:
        if (input_key[0] & ~input_key[1] & button)
        {
            return true;
        }
        break;
    }

    return false;
}
