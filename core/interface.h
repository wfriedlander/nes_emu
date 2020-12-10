#pragma once

#include <cstdint>


namespace Core {

    namespace Video {

    using ScreenBuffer = uint8_t[240][256];

        class Interface
        {
        public:
            virtual ~Interface() {}
            virtual void VideoUpdate(const ScreenBuffer& buffer) = 0;
        };

    }



    class Blip_Buffer;

    namespace Audio {

        class Interface
        {
        public:
            virtual ~Interface() {}
            virtual void SetBuffer(Blip_Buffer* buffer) = 0;
        };

    }

    namespace Input {
        enum Button
        {
            Up = 0,
            Down,
            Left,
            Right,
            Start,
            Select,
            A,
            B,
            LAST_BUTTON
        };

        struct State
        {
            bool& operator[](int b) {return button_state[b];}
        private:
            bool button_state[8];
        };

        class Interface
        {
        public:
            virtual ~Interface() {}
            virtual State State(int controller) = 0;
        };
    }
}
