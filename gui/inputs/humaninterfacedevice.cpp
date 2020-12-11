#include "humaninterfacedevice.h"

#include <QApplication>
#include <Windows.h>


namespace Input {

    HIDProvider::HIDProvider(Interface* iface, int id) : QObject(iface), DeviceProvider(iface, id)
    {
        mDevice = new HumanInterfaceDevice(this);
    }


    QString HIDProvider::Name()
    {
        return "Human Interface Device Provider";
    }

    Device *HIDProvider::Device(int)
    {
        return mDevice;
    }




    namespace HID {
        enum Code : int
        {
            Unknown = -1,
            A = 0,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            J,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z,
            Num0,
            Num1,
            Num2,
            Num3,
            Num4,
            Num5,
            Num6,
            Num7,
            Num8,
            Num9,
            Escape,
            LControl,
            LShift,
            LAlt,
            LSystem,
            RControl,
            RShift,
            RAlt,
            RSystem,
            Menu,
            LBracket,
            RBracket,
            Semicolon,
            Comma,
            Period,
            Quote,
            Slash,
            Backslash,
            Tilde,
            Equal,
            Hyphen,
            Space,
            Enter,
            Backspace,
            Tab,
            PageUp,
            PageDown,
            End,
            Home,
            Insert,
            Delete,
            Add,
            Subtract,
            Multiply,
            Divide,
            Decimal,
            Left,
            Right,
            Up,
            Down,
            Numpad0,
            Numpad1,
            Numpad2,
            Numpad3,
            Numpad4,
            Numpad5,
            Numpad6,
            Numpad7,
            Numpad8,
            Numpad9,
            F1,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            F11,
            F12,
            Pause,
            ScrollLock,
            NumLock,
            CapsLock
        };

        Code NativeToCode(unsigned int vkey)
        {
            switch (vkey)
            {
            case 'A':             return Code::A;
            case 'B':             return Code::B;
            case 'C':             return Code::C;
            case 'D':             return Code::D;
            case 'E':             return Code::E;
            case 'F':             return Code::F;
            case 'G':             return Code::G;
            case 'H':             return Code::H;
            case 'I':             return Code::I;
            case 'J':             return Code::J;
            case 'K':             return Code::K;
            case 'L':             return Code::L;
            case 'M':             return Code::M;
            case 'N':             return Code::N;
            case 'O':             return Code::O;
            case 'P':             return Code::P;
            case 'Q':             return Code::Q;
            case 'R':             return Code::R;
            case 'S':             return Code::S;
            case 'T':             return Code::T;
            case 'U':             return Code::U;
            case 'V':             return Code::V;
            case 'W':             return Code::W;
            case 'X':             return Code::X;
            case 'Y':             return Code::Y;
            case 'Z':             return Code::Z;
            case '0':             return Code::Num0;
            case '1':             return Code::Num1;
            case '2':             return Code::Num2;
            case '3':             return Code::Num3;
            case '4':             return Code::Num4;
            case '5':             return Code::Num5;
            case '6':             return Code::Num6;
            case '7':             return Code::Num7;
            case '8':             return Code::Num8;
            case '9':             return Code::Num9;
            case VK_ESCAPE:       return Code::Escape;
            case VK_LSHIFT:       return Code::LShift;
            case VK_RSHIFT:       return Code::RShift;
            case VK_LCONTROL:     return Code::LControl;
            case VK_RCONTROL:     return Code::RControl;
            case VK_LMENU:        return Code::LAlt;
            case VK_RMENU:        return Code::RAlt;
            case VK_LWIN:         return Code::LSystem;
            case VK_RWIN:         return Code::RSystem;
            case VK_APPS:         return Code::Menu;
            case VK_OEM_4:        return Code::LBracket;
            case VK_OEM_6:        return Code::RBracket;
            case VK_OEM_1:        return Code::Semicolon;
            case VK_OEM_COMMA:    return Code::Comma;
            case VK_OEM_PERIOD:   return Code::Period;
            case VK_OEM_7:        return Code::Quote;
            case VK_OEM_2:        return Code::Slash;
            case VK_OEM_5:        return Code::Backslash;
            case VK_OEM_3:        return Code::Tilde;
            case VK_OEM_PLUS:     return Code::Equal;
            case VK_OEM_MINUS:    return Code::Hyphen;
            case VK_SPACE:        return Code::Space;
            case VK_RETURN:       return Code::Enter;
            case VK_BACK:         return Code::Backspace;
            case VK_TAB:          return Code::Tab;
            case VK_PRIOR:        return Code::PageUp;
            case VK_NEXT:         return Code::PageDown;
            case VK_END:          return Code::End;
            case VK_HOME:         return Code::Home;
            case VK_INSERT:       return Code::Insert;
            case VK_DELETE:       return Code::Delete;
            case VK_ADD:          return Code::Add;
            case VK_SUBTRACT:     return Code::Subtract;
            case VK_MULTIPLY:     return Code::Multiply;
            case VK_DIVIDE:       return Code::Divide;
            case VK_DECIMAL:      return Code::Decimal;
            case VK_LEFT:         return Code::Left;
            case VK_RIGHT:        return Code::Right;
            case VK_UP:           return Code::Up;
            case VK_DOWN:         return Code::Down;
            case VK_NUMPAD0:      return Code::Numpad0;
            case VK_NUMPAD1:      return Code::Numpad1;
            case VK_NUMPAD2:      return Code::Numpad2;
            case VK_NUMPAD3:      return Code::Numpad3;
            case VK_NUMPAD4:      return Code::Numpad4;
            case VK_NUMPAD5:      return Code::Numpad5;
            case VK_NUMPAD6:      return Code::Numpad6;
            case VK_NUMPAD7:      return Code::Numpad7;
            case VK_NUMPAD8:      return Code::Numpad8;
            case VK_NUMPAD9:      return Code::Numpad9;
            case VK_F1:           return Code::F1;
            case VK_F2:           return Code::F2;
            case VK_F3:           return Code::F3;
            case VK_F4:           return Code::F4;
            case VK_F5:           return Code::F5;
            case VK_F6:           return Code::F6;
            case VK_F7:           return Code::F7;
            case VK_F8:           return Code::F8;
            case VK_F9:           return Code::F9;
            case VK_F10:          return Code::F10;
            case VK_F11:          return Code::F11;
            case VK_F12:          return Code::F12;
            case VK_PAUSE:        return Code::Pause;
            case VK_SCROLL:       return Code::ScrollLock;
            case VK_NUMLOCK:      return Code::NumLock;
            case VK_CAPITAL:      return Code::CapsLock;
            }
            return Code::Unknown;
        }

        unsigned int CodeToNative(Code code)
        {
            switch (code)
            {
            case Code::A:            return 'A';
            case Code::B:            return 'B';
            case Code::C:            return 'C';
            case Code::D:            return 'D';
            case Code::E:            return 'E';
            case Code::F:            return 'F';
            case Code::G:            return 'G';
            case Code::H:            return 'H';
            case Code::I:            return 'I';
            case Code::J:            return 'J';
            case Code::K:            return 'K';
            case Code::L:            return 'L';
            case Code::M:            return 'M';
            case Code::N:            return 'N';
            case Code::O:            return 'O';
            case Code::P:            return 'P';
            case Code::Q:            return 'Q';
            case Code::R:            return 'R';
            case Code::S:            return 'S';
            case Code::T:            return 'T';
            case Code::U:            return 'U';
            case Code::V:            return 'V';
            case Code::W:            return 'W';
            case Code::X:            return 'X';
            case Code::Y:            return 'Y';
            case Code::Z:            return 'Z';
            case Code::Num0:         return '0';
            case Code::Num1:         return '1';
            case Code::Num2:         return '2';
            case Code::Num3:         return '3';
            case Code::Num4:         return '4';
            case Code::Num5:         return '5';
            case Code::Num6:         return '6';
            case Code::Num7:         return '7';
            case Code::Num8:         return '8';
            case Code::Num9:         return '9';
            case Code::Escape:       return VK_ESCAPE;
            case Code::LShift:       return VK_LSHIFT;
            case Code::RShift:       return VK_RSHIFT;
            case Code::LControl:     return VK_LCONTROL;
            case Code::RControl:     return VK_RCONTROL;
            case Code::LAlt:         return VK_LMENU;
            case Code::RAlt:         return VK_RMENU;
            case Code::LSystem:      return VK_LWIN;
            case Code::RSystem:      return VK_RWIN;
            case Code::Menu:         return VK_APPS;
            case Code::LBracket:     return VK_OEM_4;
            case Code::RBracket:     return VK_OEM_6;
            case Code::Semicolon:    return VK_OEM_1;
            case Code::Comma:        return VK_OEM_COMMA;
            case Code::Period:       return VK_OEM_PERIOD;
            case Code::Quote:        return VK_OEM_7;
            case Code::Slash:        return VK_OEM_2;
            case Code::Backslash:    return VK_OEM_5;
            case Code::Tilde:        return VK_OEM_3;
            case Code::Equal:        return VK_OEM_PLUS;
            case Code::Hyphen:       return VK_OEM_MINUS;
            case Code::Space:        return VK_SPACE;
            case Code::Enter:        return VK_RETURN;
            case Code::Backspace:    return VK_BACK;
            case Code::Tab:          return VK_TAB;
            case Code::PageUp:       return VK_PRIOR;
            case Code::PageDown:     return VK_NEXT;
            case Code::End:          return VK_END;
            case Code::Home:         return VK_HOME;
            case Code::Insert:       return VK_INSERT;
            case Code::Delete:       return VK_DELETE;
            case Code::Add:          return VK_ADD;
            case Code::Subtract:     return VK_SUBTRACT;
            case Code::Multiply:     return VK_MULTIPLY;
            case Code::Divide:       return VK_DIVIDE;
            case Code::Decimal:      return VK_DECIMAL;
            case Code::Left:         return VK_LEFT;
            case Code::Right:        return VK_RIGHT;
            case Code::Up:           return VK_UP;
            case Code::Down:         return VK_DOWN;
            case Code::Numpad0:      return VK_NUMPAD0;
            case Code::Numpad1:      return VK_NUMPAD1;
            case Code::Numpad2:      return VK_NUMPAD2;
            case Code::Numpad3:      return VK_NUMPAD3;
            case Code::Numpad4:      return VK_NUMPAD4;
            case Code::Numpad5:      return VK_NUMPAD5;
            case Code::Numpad6:      return VK_NUMPAD6;
            case Code::Numpad7:      return VK_NUMPAD7;
            case Code::Numpad8:      return VK_NUMPAD8;
            case Code::Numpad9:      return VK_NUMPAD9;
            case Code::F1:           return VK_F1;
            case Code::F2:           return VK_F2;
            case Code::F3:           return VK_F3;
            case Code::F4:           return VK_F4;
            case Code::F5:           return VK_F5;
            case Code::F6:           return VK_F6;
            case Code::F7:           return VK_F7;
            case Code::F8:           return VK_F8;
            case Code::F9:           return VK_F9;
            case Code::F10:          return VK_F10;
            case Code::F11:          return VK_F11;
            case Code::F12:          return VK_F12;
            case Code::Pause:        return VK_PAUSE;
            case Code::ScrollLock:   return VK_SCROLL;
            case Code::NumLock:      return VK_NUMLOCK;
            case Code::CapsLock:     return VK_CAPITAL;
            default:                 return 0;
            }
        }

        QString CodeToString(Code code)
        {
            switch (code)
            {
            case Code::A:            return "A";
            case Code::B:            return "B";
            case Code::C:            return "C";
            case Code::D:            return "D";
            case Code::E:            return "E";
            case Code::F:            return "F";
            case Code::G:            return "G";
            case Code::H:            return "H";
            case Code::I:            return "I";
            case Code::J:            return "J";
            case Code::K:            return "K";
            case Code::L:            return "L";
            case Code::M:            return "M";
            case Code::N:            return "N";
            case Code::O:            return "O";
            case Code::P:            return "P";
            case Code::Q:            return "Q";
            case Code::R:            return "R";
            case Code::S:            return "S";
            case Code::T:            return "T";
            case Code::U:            return "U";
            case Code::V:            return "V";
            case Code::W:            return "W";
            case Code::X:            return "X";
            case Code::Y:            return "Y";
            case Code::Z:            return "Z";
            case Code::Num0:         return "0";
            case Code::Num1:         return "1";
            case Code::Num2:         return "2";
            case Code::Num3:         return "3";
            case Code::Num4:         return "4";
            case Code::Num5:         return "5";
            case Code::Num6:         return "6";
            case Code::Num7:         return "7";
            case Code::Num8:         return "8";
            case Code::Num9:         return "9";
            case Code::Escape:       return "Escape";
            case Code::LControl:     return "Left Control";
            case Code::LShift:       return "Left Shift";
            case Code::LAlt:         return "Left Alt";
            case Code::LSystem:      return "Left Windows";
            case Code::RControl:     return "Right Control";
            case Code::RShift:       return "Right Shift";
            case Code::RAlt:         return "Right Alt";
            case Code::RSystem:      return "Right Windows";
            case Code::Menu:         return "Menu";
            case Code::LBracket:     return "[";
            case Code::RBracket:     return "]";
            case Code::Semicolon:    return ";";
            case Code::Comma:        return ",";
            case Code::Period:       return ".";
            case Code::Quote:        return "'";
            case Code::Slash:        return "/";
            case Code::Backslash:    return "\\";
            case Code::Tilde:        return "~";
            case Code::Equal:        return "=";
            case Code::Hyphen:       return "-";
            case Code::Space:        return "Space";
            case Code::Enter:        return "Enter";
            case Code::Backspace:    return "Backspace";
            case Code::Tab:          return "Tab";
            case Code::PageUp:       return "Page Up";
            case Code::PageDown:     return "Page Down";
            case Code::End:          return "End";
            case Code::Home:         return "Home";
            case Code::Insert:       return "Insert";
            case Code::Delete:       return "Delete";
            case Code::Add:          return "Numpad +";
            case Code::Subtract:     return "Numpad -";
            case Code::Multiply:     return "Numpad *";
            case Code::Divide:       return "Numpad /";
            case Code::Decimal:      return "Numpad .";
            case Code::Left:         return "Left";
            case Code::Right:        return "Right";
            case Code::Up:           return "Up";
            case Code::Down:         return "Down";
            case Code::Numpad0:      return "Numpad 0";
            case Code::Numpad1:      return "Numpad 1";
            case Code::Numpad2:      return "Numpad 2";
            case Code::Numpad3:      return "Numpad 3";
            case Code::Numpad4:      return "Numpad 4";
            case Code::Numpad5:      return "Numpad 5";
            case Code::Numpad6:      return "Numpad 6";
            case Code::Numpad7:      return "Numpad 7";
            case Code::Numpad8:      return "Numpad 8";
            case Code::Numpad9:      return "Numpad 9";
            case Code::F1:           return "F1";
            case Code::F2:           return "F2";
            case Code::F3:           return "F3";
            case Code::F4:           return "F4";
            case Code::F5:           return "F5";
            case Code::F6:           return "F6";
            case Code::F7:           return "F7";
            case Code::F8:           return "F8";
            case Code::F9:           return "F9";
            case Code::F10:          return "F10";
            case Code::F11:          return "F11";
            case Code::F12:          return "F12";
            case Code::Pause:        return "Pause";
            case Code::ScrollLock:   return "Scroll Lock";
            case Code::NumLock:      return "Num Lock";
            case Code::CapsLock:     return "Caps Lock";
            default:                 return "Unknown";
            }
        }


        unsigned int MapLeftRightKeys(unsigned int wparam, long lparam)
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
    }





HumanInterfaceDevice::HumanInterfaceDevice(HIDProvider* provider) : QObject(provider), Device(provider)
{

}


bool HumanInterfaceDevice::Active(int code)
{
    auto vkey = HID::CodeToNative((HID::Code)code);
    auto state = GetAsyncKeyState(vkey);
    return state & 0x8000;
}

void HumanInterfaceDevice::GetImmediate()
{
    if (!mImmediate) {
        qApp->installNativeEventFilter(this);
        mImmediate = true;
    }
}

void HumanInterfaceDevice::StopImmediate()
{
    if (mImmediate) {
        qApp->removeNativeEventFilter(this);
        mImmediate = false;
    }
}

QString HumanInterfaceDevice::CodeToString(int code)
{
    return HID::CodeToString((HID::Code)code);
}

QString HumanInterfaceDevice::Name()
{
    return "Keyboard";
}

bool HumanInterfaceDevice::nativeEventFilter(const QByteArray &, void *message, long *)
{
    unsigned int vkey = 0;

    MSG* msg = (MSG*)message;
    switch (msg->message)
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if (!(msg->lParam & 0x40000000)) {
            vkey = HID::MapLeftRightKeys(msg->wParam, msg->lParam);
            mReceiver->Activated({-1, 0, HID::NativeToCode(vkey)});
            return true;
        }
    }

    return false;
}


}
