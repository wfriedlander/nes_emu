#include "keyboard.h"

#include <QApplication>
#include <QDebug>
#include <windows.h>


Keyboard::Keyboard(QObject *parent) : QObject(parent)
{
    qApp->installNativeEventFilter(this);
}

bool Keyboard::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(result);

    unsigned int vkey = 0;

    MSG* msg = (MSG*)message;
    switch (msg->message)
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if (!(msg->lParam & 0x40000000)) {
            vkey = MapLeftRightKeys(msg->wParam, msg->lParam);
//            qDebug() << "KEY DOWN" << KeyToString(NativeToKey(vkey));
            emit KeyDown(NativeToKey(vkey));
        }
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        vkey = MapLeftRightKeys(msg->wParam, msg->lParam);
//        qDebug() << "KEY UP" << KeyToString(NativeToKey(msg->wParam));
        emit KeyUp(NativeToKey(vkey));
        break;
    }

    return false;
}

bool Keyboard::KeyIsDown(KEY key)
{
    auto vkey = KeyToNative(key);
    auto state = GetAsyncKeyState(vkey);
    return state & 0x8000;
}

unsigned int Keyboard::MapLeftRightKeys(unsigned int wparam, long lparam)
{
    UINT scancode = (lparam & 0x00ff0000) >> 16;
    bool extended  = (lparam & 0x01000000);

    switch (wparam) {
    case VK_SHIFT:   return MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
    case VK_CONTROL: return extended ? VK_RCONTROL : VK_LCONTROL;
    case VK_MENU:    return extended ? VK_RMENU : VK_LMENU;
    default:         return wparam;
    }
}

KEY Keyboard::NativeToKey(unsigned int vkey)
{
    switch (vkey)
    {
    case 'A':             return KEY::A;
    case 'B':             return KEY::B;
    case 'C':             return KEY::C;
    case 'D':             return KEY::D;
    case 'E':             return KEY::E;
    case 'F':             return KEY::F;
    case 'G':             return KEY::G;
    case 'H':             return KEY::H;
    case 'I':             return KEY::I;
    case 'J':             return KEY::J;
    case 'K':             return KEY::K;
    case 'L':             return KEY::L;
    case 'M':             return KEY::M;
    case 'N':             return KEY::N;
    case 'O':             return KEY::O;
    case 'P':             return KEY::P;
    case 'Q':             return KEY::Q;
    case 'R':             return KEY::R;
    case 'S':             return KEY::S;
    case 'T':             return KEY::T;
    case 'U':             return KEY::U;
    case 'V':             return KEY::V;
    case 'W':             return KEY::W;
    case 'X':             return KEY::X;
    case 'Y':             return KEY::Y;
    case 'Z':             return KEY::Z;
    case '0':             return KEY::Num0;
    case '1':             return KEY::Num1;
    case '2':             return KEY::Num2;
    case '3':             return KEY::Num3;
    case '4':             return KEY::Num4;
    case '5':             return KEY::Num5;
    case '6':             return KEY::Num6;
    case '7':             return KEY::Num7;
    case '8':             return KEY::Num8;
    case '9':             return KEY::Num9;
    case VK_ESCAPE:       return KEY::Escape;
    case VK_LSHIFT:       return KEY::LShift;
    case VK_RSHIFT:       return KEY::RShift;
    case VK_LCONTROL:     return KEY::LControl;
    case VK_RCONTROL:     return KEY::RControl;
    case VK_LMENU:        return KEY::LAlt;
    case VK_RMENU:        return KEY::RAlt;
    case VK_LWIN:         return KEY::LSystem;
    case VK_RWIN:         return KEY::RSystem;
    case VK_APPS:         return KEY::Menu;
    case VK_OEM_4:        return KEY::LBracket;
    case VK_OEM_6:        return KEY::RBracket;
    case VK_OEM_1:        return KEY::Semicolon;
    case VK_OEM_COMMA:    return KEY::Comma;
    case VK_OEM_PERIOD:   return KEY::Period;
    case VK_OEM_7:        return KEY::Quote;
    case VK_OEM_2:        return KEY::Slash;
    case VK_OEM_5:        return KEY::Backslash;
    case VK_OEM_3:        return KEY::Tilde;
    case VK_OEM_PLUS:     return KEY::Equal;
    case VK_OEM_MINUS:    return KEY::Hyphen;
    case VK_SPACE:        return KEY::Space;
    case VK_RETURN:       return KEY::Enter;
    case VK_BACK:         return KEY::Backspace;
    case VK_TAB:          return KEY::Tab;
    case VK_PRIOR:        return KEY::PageUp;
    case VK_NEXT:         return KEY::PageDown;
    case VK_END:          return KEY::End;
    case VK_HOME:         return KEY::Home;
    case VK_INSERT:       return KEY::Insert;
    case VK_DELETE:       return KEY::Delete;
    case VK_ADD:          return KEY::Add;
    case VK_SUBTRACT:     return KEY::Subtract;
    case VK_MULTIPLY:     return KEY::Multiply;
    case VK_DIVIDE:       return KEY::Divide;
    case VK_DECIMAL:      return KEY::Decimal;
    case VK_LEFT:         return KEY::Left;
    case VK_RIGHT:        return KEY::Right;
    case VK_UP:           return KEY::Up;
    case VK_DOWN:         return KEY::Down;
    case VK_NUMPAD0:      return KEY::Numpad0;
    case VK_NUMPAD1:      return KEY::Numpad1;
    case VK_NUMPAD2:      return KEY::Numpad2;
    case VK_NUMPAD3:      return KEY::Numpad3;
    case VK_NUMPAD4:      return KEY::Numpad4;
    case VK_NUMPAD5:      return KEY::Numpad5;
    case VK_NUMPAD6:      return KEY::Numpad6;
    case VK_NUMPAD7:      return KEY::Numpad7;
    case VK_NUMPAD8:      return KEY::Numpad8;
    case VK_NUMPAD9:      return KEY::Numpad9;
    case VK_F1:           return KEY::F1;
    case VK_F2:           return KEY::F2;
    case VK_F3:           return KEY::F3;
    case VK_F4:           return KEY::F4;
    case VK_F5:           return KEY::F5;
    case VK_F6:           return KEY::F6;
    case VK_F7:           return KEY::F7;
    case VK_F8:           return KEY::F8;
    case VK_F9:           return KEY::F9;
    case VK_F10:          return KEY::F10;
    case VK_F11:          return KEY::F11;
    case VK_F12:          return KEY::F12;
    case VK_PAUSE:        return KEY::Pause;
    case VK_SCROLL:       return KEY::ScrollLock;
    case VK_NUMLOCK:      return KEY::NumLock;
    case VK_CAPITAL:      return KEY::CapsLock;
    }
    return KEY::Unknown;
}

unsigned int Keyboard::KeyToNative(KEY key)
{
    switch (key)
    {
    case KEY::A:            return 'A';
    case KEY::B:            return 'B';
    case KEY::C:            return 'C';
    case KEY::D:            return 'D';
    case KEY::E:            return 'E';
    case KEY::F:            return 'F';
    case KEY::G:            return 'G';
    case KEY::H:            return 'H';
    case KEY::I:            return 'I';
    case KEY::J:            return 'J';
    case KEY::K:            return 'K';
    case KEY::L:            return 'L';
    case KEY::M:            return 'M';
    case KEY::N:            return 'N';
    case KEY::O:            return 'O';
    case KEY::P:            return 'P';
    case KEY::Q:            return 'Q';
    case KEY::R:            return 'R';
    case KEY::S:            return 'S';
    case KEY::T:            return 'T';
    case KEY::U:            return 'U';
    case KEY::V:            return 'V';
    case KEY::W:            return 'W';
    case KEY::X:            return 'X';
    case KEY::Y:            return 'Y';
    case KEY::Z:            return 'Z';
    case KEY::Num0:         return '0';
    case KEY::Num1:         return '1';
    case KEY::Num2:         return '2';
    case KEY::Num3:         return '3';
    case KEY::Num4:         return '4';
    case KEY::Num5:         return '5';
    case KEY::Num6:         return '6';
    case KEY::Num7:         return '7';
    case KEY::Num8:         return '8';
    case KEY::Num9:         return '9';
    case KEY::Escape:       return VK_ESCAPE;
    case KEY::LShift:       return VK_LSHIFT;
    case KEY::RShift:       return VK_RSHIFT;
    case KEY::LControl:     return VK_LCONTROL;
    case KEY::RControl:     return VK_RCONTROL;
    case KEY::LAlt:         return VK_LMENU;
    case KEY::RAlt:         return VK_RMENU;
    case KEY::LSystem:      return VK_LWIN;
    case KEY::RSystem:      return VK_RWIN;
    case KEY::Menu:         return VK_APPS;
    case KEY::LBracket:     return VK_OEM_4;
    case KEY::RBracket:     return VK_OEM_6;
    case KEY::Semicolon:    return VK_OEM_1;
    case KEY::Comma:        return VK_OEM_COMMA;
    case KEY::Period:       return VK_OEM_PERIOD;
    case KEY::Quote:        return VK_OEM_7;
    case KEY::Slash:        return VK_OEM_2;
    case KEY::Backslash:    return VK_OEM_5;
    case KEY::Tilde:        return VK_OEM_3;
    case KEY::Equal:        return VK_OEM_PLUS;
    case KEY::Hyphen:       return VK_OEM_MINUS;
    case KEY::Space:        return VK_SPACE;
    case KEY::Enter:        return VK_RETURN;
    case KEY::Backspace:    return VK_BACK;
    case KEY::Tab:          return VK_TAB;
    case KEY::PageUp:       return VK_PRIOR;
    case KEY::PageDown:     return VK_NEXT;
    case KEY::End:          return VK_END;
    case KEY::Home:         return VK_HOME;
    case KEY::Insert:       return VK_INSERT;
    case KEY::Delete:       return VK_DELETE;
    case KEY::Add:          return VK_ADD;
    case KEY::Subtract:     return VK_SUBTRACT;
    case KEY::Multiply:     return VK_MULTIPLY;
    case KEY::Divide:       return VK_DIVIDE;
    case KEY::Decimal:      return VK_DECIMAL;
    case KEY::Left:         return VK_LEFT;
    case KEY::Right:        return VK_RIGHT;
    case KEY::Up:           return VK_UP;
    case KEY::Down:         return VK_DOWN;
    case KEY::Numpad0:      return VK_NUMPAD0;
    case KEY::Numpad1:      return VK_NUMPAD1;
    case KEY::Numpad2:      return VK_NUMPAD2;
    case KEY::Numpad3:      return VK_NUMPAD3;
    case KEY::Numpad4:      return VK_NUMPAD4;
    case KEY::Numpad5:      return VK_NUMPAD5;
    case KEY::Numpad6:      return VK_NUMPAD6;
    case KEY::Numpad7:      return VK_NUMPAD7;
    case KEY::Numpad8:      return VK_NUMPAD8;
    case KEY::Numpad9:      return VK_NUMPAD9;
    case KEY::F1:           return VK_F1;
    case KEY::F2:           return VK_F2;
    case KEY::F3:           return VK_F3;
    case KEY::F4:           return VK_F4;
    case KEY::F5:           return VK_F5;
    case KEY::F6:           return VK_F6;
    case KEY::F7:           return VK_F7;
    case KEY::F8:           return VK_F8;
    case KEY::F9:           return VK_F9;
    case KEY::F10:          return VK_F10;
    case KEY::F11:          return VK_F11;
    case KEY::F12:          return VK_F12;
    case KEY::Pause:        return VK_PAUSE;
    case KEY::ScrollLock:   return VK_SCROLL;
    case KEY::NumLock:      return VK_NUMLOCK;
    case KEY::CapsLock:     return VK_CAPITAL;
    case KEY::Unknown:      return 0;
    }
}

QString Keyboard::KeyToString(KEY key)
{
    switch (key)
    {
    case KEY::Unknown:      return "Unknown";
    case KEY::A:            return "A";
    case KEY::B:            return "B";
    case KEY::C:            return "C";
    case KEY::D:            return "D";
    case KEY::E:            return "E";
    case KEY::F:            return "F";
    case KEY::G:            return "G";
    case KEY::H:            return "H";
    case KEY::I:            return "I";
    case KEY::J:            return "J";
    case KEY::K:            return "K";
    case KEY::L:            return "L";
    case KEY::M:            return "M";
    case KEY::N:            return "N";
    case KEY::O:            return "O";
    case KEY::P:            return "P";
    case KEY::Q:            return "Q";
    case KEY::R:            return "R";
    case KEY::S:            return "S";
    case KEY::T:            return "T";
    case KEY::U:            return "U";
    case KEY::V:            return "V";
    case KEY::W:            return "W";
    case KEY::X:            return "X";
    case KEY::Y:            return "Y";
    case KEY::Z:            return "Z";
    case KEY::Num0:         return "0";
    case KEY::Num1:         return "1";
    case KEY::Num2:         return "2";
    case KEY::Num3:         return "3";
    case KEY::Num4:         return "4";
    case KEY::Num5:         return "5";
    case KEY::Num6:         return "6";
    case KEY::Num7:         return "7";
    case KEY::Num8:         return "8";
    case KEY::Num9:         return "9";
    case KEY::Escape:       return "Escape";
    case KEY::LControl:     return "Left Control";
    case KEY::LShift:       return "Left Shift";
    case KEY::LAlt:         return "Left Alt";
    case KEY::LSystem:      return "Left Windows";
    case KEY::RControl:     return "Right Control";
    case KEY::RShift:       return "Right Shift";
    case KEY::RAlt:         return "Right Alt";
    case KEY::RSystem:      return "Right Windows";
    case KEY::Menu:         return "Menu";
    case KEY::LBracket:     return "[";
    case KEY::RBracket:     return "]";
    case KEY::Semicolon:    return ";";
    case KEY::Comma:        return ",";
    case KEY::Period:       return ".";
    case KEY::Quote:        return "'";
    case KEY::Slash:        return "/";
    case KEY::Backslash:    return "\\";
    case KEY::Tilde:        return "~";
    case KEY::Equal:        return "=";
    case KEY::Hyphen:       return "-";
    case KEY::Space:        return "Space";
    case KEY::Enter:        return "Enter";
    case KEY::Backspace:    return "Backspace";
    case KEY::Tab:          return "Tab";
    case KEY::PageUp:       return "Page Up";
    case KEY::PageDown:     return "Page Down";
    case KEY::End:          return "End";
    case KEY::Home:         return "Home";
    case KEY::Insert:       return "Insert";
    case KEY::Delete:       return "Delete";
    case KEY::Add:          return "Numpad +";
    case KEY::Subtract:     return "Numpad -";
    case KEY::Multiply:     return "Numpad *";
    case KEY::Divide:       return "Numpad /";
    case KEY::Decimal:      return "Numpad .";
    case KEY::Left:         return "Left";
    case KEY::Right:        return "Right";
    case KEY::Up:           return "Up";
    case KEY::Down:         return "Down";
    case KEY::Numpad0:      return "Numpad 0";
    case KEY::Numpad1:      return "Numpad 1";
    case KEY::Numpad2:      return "Numpad 2";
    case KEY::Numpad3:      return "Numpad 3";
    case KEY::Numpad4:      return "Numpad 4";
    case KEY::Numpad5:      return "Numpad 5";
    case KEY::Numpad6:      return "Numpad 6";
    case KEY::Numpad7:      return "Numpad 7";
    case KEY::Numpad8:      return "Numpad 8";
    case KEY::Numpad9:      return "Numpad 9";
    case KEY::F1:           return "F1";
    case KEY::F2:           return "F2";
    case KEY::F3:           return "F3";
    case KEY::F4:           return "F4";
    case KEY::F5:           return "F5";
    case KEY::F6:           return "F6";
    case KEY::F7:           return "F7";
    case KEY::F8:           return "F8";
    case KEY::F9:           return "F9";
    case KEY::F10:          return "F10";
    case KEY::F11:          return "F11";
    case KEY::F12:          return "F12";
    case KEY::Pause:        return "Pause";
    case KEY::ScrollLock:   return "Scroll Lock";
    case KEY::NumLock:      return "Num Lock";
    case KEY::CapsLock:     return "Caps Lock";
    }
}
