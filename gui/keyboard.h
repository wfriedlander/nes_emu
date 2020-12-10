//#ifndef KEYBOARD_H
//#define KEYBOARD_H

//#include <QObject>
//#include <QAbstractNativeEventFilter>

//#include "interface_input.h"

//namespace KB
//{
//    enum class Key : int
//    {
//        Unknown = -1,
//        A = 0,
//        B,
//        C,
//        D,
//        E,
//        F,
//        G,
//        H,
//        I,
//        J,
//        K,
//        L,
//        M,
//        N,
//        O,
//        P,
//        Q,
//        R,
//        S,
//        T,
//        U,
//        V,
//        W,
//        X,
//        Y,
//        Z,
//        Num0,
//        Num1,
//        Num2,
//        Num3,
//        Num4,
//        Num5,
//        Num6,
//        Num7,
//        Num8,
//        Num9,
//        Escape,
//        LControl,
//        LShift,
//        LAlt,
//        LSystem,
//        RControl,
//        RShift,
//        RAlt,
//        RSystem,
//        Menu,
//        LBracket,
//        RBracket,
//        Semicolon,
//        Comma,
//        Period,
//        Quote,
//        Slash,
//        Backslash,
//        Tilde,
//        Equal,
//        Hyphen,
//        Space,
//        Enter,
//        Backspace,
//        Tab,
//        PageUp,
//        PageDown,
//        End,
//        Home,
//        Insert,
//        Delete,
//        Add,
//        Subtract,
//        Multiply,
//        Divide,
//        Decimal,
//        Left,
//        Right,
//        Up,
//        Down,
//        Numpad0,
//        Numpad1,
//        Numpad2,
//        Numpad3,
//        Numpad4,
//        Numpad5,
//        Numpad6,
//        Numpad7,
//        Numpad8,
//        Numpad9,
//        F1,
//        F2,
//        F3,
//        F4,
//        F5,
//        F6,
//        F7,
//        F8,
//        F9,
//        F10,
//        F11,
//        F12,
//        Pause,
//        ScrollLock,
//        NumLock,
//        CapsLock
//    };
//}

//class Keyboard : public InputDevice, public QAbstractNativeEventFilter
//{
//    Q_OBJECT
//public:
//    explicit Keyboard(QObject *parent = nullptr);


//public:
//    bool IsCodeActive(int code) override;
//    void EnableImmediateMode(bool enabled) override;
//    QString CodeToString(int code) override;
//    QString DeviceName() override;


//private:
//    bool IsKeyDown(KB::Key key);
//    QString KeyToString(KB::Key key);

//protected:
//    bool nativeEventFilter(const QByteArray&, void*, long*) override;

//private:
//    unsigned int MapLeftRightKeys(unsigned int wparam, long lparam);
//    KB::Key NativeToKey(unsigned int vkey);
//    unsigned int KeyToNative(KB::Key key);

//};

//#endif // KEYBOARD_H
