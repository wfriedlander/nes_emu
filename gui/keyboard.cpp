#include "keyboard.h"

#include <QApplication>
#include <QDebug>
#include <windows.h>


using namespace KB;


Keyboard::Keyboard(QObject *parent) : InputDevice(parent)
{

}

QString Keyboard::DeviceName()
{
    return "Keyboard";
}

void Keyboard::EnableImmediateMode(bool enabled)
{
    if (enabled)
        qApp->installNativeEventFilter(this);
    else
        qApp->removeNativeEventFilter(this);
}

bool Keyboard::IsCodeActive(int code)
{
    Key key = static_cast<Key>(code);
    return IsKeyDown(key);
}

QString Keyboard::CodeToString(int code)
{
    Key key = static_cast<Key>(code);
    return KeyToString(key);
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
            emit PushInput({this, static_cast<int>(NativeToKey(vkey))});
            return true;
        }
//    case WM_KEYUP:
//    case WM_SYSKEYUP:
//        vkey = MapLeftRightKeys(msg->wParam, msg->lParam);
//        qDebug() << "KEY UP" << KeyToString(NativeToKey(msg->wParam));
//        emit KeyUp(NativeToKey(vkey));
//        break;
    }

    return false;
}



bool Keyboard::IsKeyDown(Key key)
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

Key Keyboard::NativeToKey(unsigned int vkey)
{
    switch (vkey)
    {
    case 'A':             return Key::A;
    case 'B':             return Key::B;
    case 'C':             return Key::C;
    case 'D':             return Key::D;
    case 'E':             return Key::E;
    case 'F':             return Key::F;
    case 'G':             return Key::G;
    case 'H':             return Key::H;
    case 'I':             return Key::I;
    case 'J':             return Key::J;
    case 'K':             return Key::K;
    case 'L':             return Key::L;
    case 'M':             return Key::M;
    case 'N':             return Key::N;
    case 'O':             return Key::O;
    case 'P':             return Key::P;
    case 'Q':             return Key::Q;
    case 'R':             return Key::R;
    case 'S':             return Key::S;
    case 'T':             return Key::T;
    case 'U':             return Key::U;
    case 'V':             return Key::V;
    case 'W':             return Key::W;
    case 'X':             return Key::X;
    case 'Y':             return Key::Y;
    case 'Z':             return Key::Z;
    case '0':             return Key::Num0;
    case '1':             return Key::Num1;
    case '2':             return Key::Num2;
    case '3':             return Key::Num3;
    case '4':             return Key::Num4;
    case '5':             return Key::Num5;
    case '6':             return Key::Num6;
    case '7':             return Key::Num7;
    case '8':             return Key::Num8;
    case '9':             return Key::Num9;
    case VK_ESCAPE:       return Key::Escape;
    case VK_LSHIFT:       return Key::LShift;
    case VK_RSHIFT:       return Key::RShift;
    case VK_LCONTROL:     return Key::LControl;
    case VK_RCONTROL:     return Key::RControl;
    case VK_LMENU:        return Key::LAlt;
    case VK_RMENU:        return Key::RAlt;
    case VK_LWIN:         return Key::LSystem;
    case VK_RWIN:         return Key::RSystem;
    case VK_APPS:         return Key::Menu;
    case VK_OEM_4:        return Key::LBracket;
    case VK_OEM_6:        return Key::RBracket;
    case VK_OEM_1:        return Key::Semicolon;
    case VK_OEM_COMMA:    return Key::Comma;
    case VK_OEM_PERIOD:   return Key::Period;
    case VK_OEM_7:        return Key::Quote;
    case VK_OEM_2:        return Key::Slash;
    case VK_OEM_5:        return Key::Backslash;
    case VK_OEM_3:        return Key::Tilde;
    case VK_OEM_PLUS:     return Key::Equal;
    case VK_OEM_MINUS:    return Key::Hyphen;
    case VK_SPACE:        return Key::Space;
    case VK_RETURN:       return Key::Enter;
    case VK_BACK:         return Key::Backspace;
    case VK_TAB:          return Key::Tab;
    case VK_PRIOR:        return Key::PageUp;
    case VK_NEXT:         return Key::PageDown;
    case VK_END:          return Key::End;
    case VK_HOME:         return Key::Home;
    case VK_INSERT:       return Key::Insert;
    case VK_DELETE:       return Key::Delete;
    case VK_ADD:          return Key::Add;
    case VK_SUBTRACT:     return Key::Subtract;
    case VK_MULTIPLY:     return Key::Multiply;
    case VK_DIVIDE:       return Key::Divide;
    case VK_DECIMAL:      return Key::Decimal;
    case VK_LEFT:         return Key::Left;
    case VK_RIGHT:        return Key::Right;
    case VK_UP:           return Key::Up;
    case VK_DOWN:         return Key::Down;
    case VK_NUMPAD0:      return Key::Numpad0;
    case VK_NUMPAD1:      return Key::Numpad1;
    case VK_NUMPAD2:      return Key::Numpad2;
    case VK_NUMPAD3:      return Key::Numpad3;
    case VK_NUMPAD4:      return Key::Numpad4;
    case VK_NUMPAD5:      return Key::Numpad5;
    case VK_NUMPAD6:      return Key::Numpad6;
    case VK_NUMPAD7:      return Key::Numpad7;
    case VK_NUMPAD8:      return Key::Numpad8;
    case VK_NUMPAD9:      return Key::Numpad9;
    case VK_F1:           return Key::F1;
    case VK_F2:           return Key::F2;
    case VK_F3:           return Key::F3;
    case VK_F4:           return Key::F4;
    case VK_F5:           return Key::F5;
    case VK_F6:           return Key::F6;
    case VK_F7:           return Key::F7;
    case VK_F8:           return Key::F8;
    case VK_F9:           return Key::F9;
    case VK_F10:          return Key::F10;
    case VK_F11:          return Key::F11;
    case VK_F12:          return Key::F12;
    case VK_PAUSE:        return Key::Pause;
    case VK_SCROLL:       return Key::ScrollLock;
    case VK_NUMLOCK:      return Key::NumLock;
    case VK_CAPITAL:      return Key::CapsLock;
    }
    return Key::Unknown;
}

unsigned int Keyboard::KeyToNative(Key key)
{
    switch (key)
    {
    case Key::A:            return 'A';
    case Key::B:            return 'B';
    case Key::C:            return 'C';
    case Key::D:            return 'D';
    case Key::E:            return 'E';
    case Key::F:            return 'F';
    case Key::G:            return 'G';
    case Key::H:            return 'H';
    case Key::I:            return 'I';
    case Key::J:            return 'J';
    case Key::K:            return 'K';
    case Key::L:            return 'L';
    case Key::M:            return 'M';
    case Key::N:            return 'N';
    case Key::O:            return 'O';
    case Key::P:            return 'P';
    case Key::Q:            return 'Q';
    case Key::R:            return 'R';
    case Key::S:            return 'S';
    case Key::T:            return 'T';
    case Key::U:            return 'U';
    case Key::V:            return 'V';
    case Key::W:            return 'W';
    case Key::X:            return 'X';
    case Key::Y:            return 'Y';
    case Key::Z:            return 'Z';
    case Key::Num0:         return '0';
    case Key::Num1:         return '1';
    case Key::Num2:         return '2';
    case Key::Num3:         return '3';
    case Key::Num4:         return '4';
    case Key::Num5:         return '5';
    case Key::Num6:         return '6';
    case Key::Num7:         return '7';
    case Key::Num8:         return '8';
    case Key::Num9:         return '9';
    case Key::Escape:       return VK_ESCAPE;
    case Key::LShift:       return VK_LSHIFT;
    case Key::RShift:       return VK_RSHIFT;
    case Key::LControl:     return VK_LCONTROL;
    case Key::RControl:     return VK_RCONTROL;
    case Key::LAlt:         return VK_LMENU;
    case Key::RAlt:         return VK_RMENU;
    case Key::LSystem:      return VK_LWIN;
    case Key::RSystem:      return VK_RWIN;
    case Key::Menu:         return VK_APPS;
    case Key::LBracket:     return VK_OEM_4;
    case Key::RBracket:     return VK_OEM_6;
    case Key::Semicolon:    return VK_OEM_1;
    case Key::Comma:        return VK_OEM_COMMA;
    case Key::Period:       return VK_OEM_PERIOD;
    case Key::Quote:        return VK_OEM_7;
    case Key::Slash:        return VK_OEM_2;
    case Key::Backslash:    return VK_OEM_5;
    case Key::Tilde:        return VK_OEM_3;
    case Key::Equal:        return VK_OEM_PLUS;
    case Key::Hyphen:       return VK_OEM_MINUS;
    case Key::Space:        return VK_SPACE;
    case Key::Enter:        return VK_RETURN;
    case Key::Backspace:    return VK_BACK;
    case Key::Tab:          return VK_TAB;
    case Key::PageUp:       return VK_PRIOR;
    case Key::PageDown:     return VK_NEXT;
    case Key::End:          return VK_END;
    case Key::Home:         return VK_HOME;
    case Key::Insert:       return VK_INSERT;
    case Key::Delete:       return VK_DELETE;
    case Key::Add:          return VK_ADD;
    case Key::Subtract:     return VK_SUBTRACT;
    case Key::Multiply:     return VK_MULTIPLY;
    case Key::Divide:       return VK_DIVIDE;
    case Key::Decimal:      return VK_DECIMAL;
    case Key::Left:         return VK_LEFT;
    case Key::Right:        return VK_RIGHT;
    case Key::Up:           return VK_UP;
    case Key::Down:         return VK_DOWN;
    case Key::Numpad0:      return VK_NUMPAD0;
    case Key::Numpad1:      return VK_NUMPAD1;
    case Key::Numpad2:      return VK_NUMPAD2;
    case Key::Numpad3:      return VK_NUMPAD3;
    case Key::Numpad4:      return VK_NUMPAD4;
    case Key::Numpad5:      return VK_NUMPAD5;
    case Key::Numpad6:      return VK_NUMPAD6;
    case Key::Numpad7:      return VK_NUMPAD7;
    case Key::Numpad8:      return VK_NUMPAD8;
    case Key::Numpad9:      return VK_NUMPAD9;
    case Key::F1:           return VK_F1;
    case Key::F2:           return VK_F2;
    case Key::F3:           return VK_F3;
    case Key::F4:           return VK_F4;
    case Key::F5:           return VK_F5;
    case Key::F6:           return VK_F6;
    case Key::F7:           return VK_F7;
    case Key::F8:           return VK_F8;
    case Key::F9:           return VK_F9;
    case Key::F10:          return VK_F10;
    case Key::F11:          return VK_F11;
    case Key::F12:          return VK_F12;
    case Key::Pause:        return VK_PAUSE;
    case Key::ScrollLock:   return VK_SCROLL;
    case Key::NumLock:      return VK_NUMLOCK;
    case Key::CapsLock:     return VK_CAPITAL;
    default:                return 0;
    }
}

QString Keyboard::KeyToString(Key key)
{
    switch (key)
    {
    case Key::A:            return "A";
    case Key::B:            return "B";
    case Key::C:            return "C";
    case Key::D:            return "D";
    case Key::E:            return "E";
    case Key::F:            return "F";
    case Key::G:            return "G";
    case Key::H:            return "H";
    case Key::I:            return "I";
    case Key::J:            return "J";
    case Key::K:            return "K";
    case Key::L:            return "L";
    case Key::M:            return "M";
    case Key::N:            return "N";
    case Key::O:            return "O";
    case Key::P:            return "P";
    case Key::Q:            return "Q";
    case Key::R:            return "R";
    case Key::S:            return "S";
    case Key::T:            return "T";
    case Key::U:            return "U";
    case Key::V:            return "V";
    case Key::W:            return "W";
    case Key::X:            return "X";
    case Key::Y:            return "Y";
    case Key::Z:            return "Z";
    case Key::Num0:         return "0";
    case Key::Num1:         return "1";
    case Key::Num2:         return "2";
    case Key::Num3:         return "3";
    case Key::Num4:         return "4";
    case Key::Num5:         return "5";
    case Key::Num6:         return "6";
    case Key::Num7:         return "7";
    case Key::Num8:         return "8";
    case Key::Num9:         return "9";
    case Key::Escape:       return "Escape";
    case Key::LControl:     return "Left Control";
    case Key::LShift:       return "Left Shift";
    case Key::LAlt:         return "Left Alt";
    case Key::LSystem:      return "Left Windows";
    case Key::RControl:     return "Right Control";
    case Key::RShift:       return "Right Shift";
    case Key::RAlt:         return "Right Alt";
    case Key::RSystem:      return "Right Windows";
    case Key::Menu:         return "Menu";
    case Key::LBracket:     return "[";
    case Key::RBracket:     return "]";
    case Key::Semicolon:    return ";";
    case Key::Comma:        return ",";
    case Key::Period:       return ".";
    case Key::Quote:        return "'";
    case Key::Slash:        return "/";
    case Key::Backslash:    return "\\";
    case Key::Tilde:        return "~";
    case Key::Equal:        return "=";
    case Key::Hyphen:       return "-";
    case Key::Space:        return "Space";
    case Key::Enter:        return "Enter";
    case Key::Backspace:    return "Backspace";
    case Key::Tab:          return "Tab";
    case Key::PageUp:       return "Page Up";
    case Key::PageDown:     return "Page Down";
    case Key::End:          return "End";
    case Key::Home:         return "Home";
    case Key::Insert:       return "Insert";
    case Key::Delete:       return "Delete";
    case Key::Add:          return "Numpad +";
    case Key::Subtract:     return "Numpad -";
    case Key::Multiply:     return "Numpad *";
    case Key::Divide:       return "Numpad /";
    case Key::Decimal:      return "Numpad .";
    case Key::Left:         return "Left";
    case Key::Right:        return "Right";
    case Key::Up:           return "Up";
    case Key::Down:         return "Down";
    case Key::Numpad0:      return "Numpad 0";
    case Key::Numpad1:      return "Numpad 1";
    case Key::Numpad2:      return "Numpad 2";
    case Key::Numpad3:      return "Numpad 3";
    case Key::Numpad4:      return "Numpad 4";
    case Key::Numpad5:      return "Numpad 5";
    case Key::Numpad6:      return "Numpad 6";
    case Key::Numpad7:      return "Numpad 7";
    case Key::Numpad8:      return "Numpad 8";
    case Key::Numpad9:      return "Numpad 9";
    case Key::F1:           return "F1";
    case Key::F2:           return "F2";
    case Key::F3:           return "F3";
    case Key::F4:           return "F4";
    case Key::F5:           return "F5";
    case Key::F6:           return "F6";
    case Key::F7:           return "F7";
    case Key::F8:           return "F8";
    case Key::F9:           return "F9";
    case Key::F10:          return "F10";
    case Key::F11:          return "F11";
    case Key::F12:          return "F12";
    case Key::Pause:        return "Pause";
    case Key::ScrollLock:   return "Scroll Lock";
    case Key::NumLock:      return "Num Lock";
    case Key::CapsLock:     return "Caps Lock";
    default:                return "Unknown";
    }
}
