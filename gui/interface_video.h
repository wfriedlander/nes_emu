#pragma once

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <array>

#include "core/interface.h"



enum class Filter
{
    None,
    NTSC,
    xBRZ,
    HQ,
    Scale,
    SaI2x,
    SuperEagle,
    SuperSaI2x
};

enum class AspectRatio
{
    Standard,
    Widescreen,
    Custom
};



class Video : public QOpenGLWidget, protected QOpenGLExtraFunctions, public IVideo
{
    Q_OBJECT

    struct Color {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    using FrameBuffer = std::array<std::array<Color, 256>, 240>;

public:
    explicit Video(QWidget *parent = nullptr);
    ~Video();

public:
    void VideoUpdate(const ScreenBuffer& buffer) override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    bool mInitialized = false;

    int mCount = 0;

    GLuint mVBO;
    GLuint mVAO;
    GLuint mShader;
    GLuint mTexture;

    FrameBuffer mFrame;

    Color mPalette[64] = {
        Color {101, 101, 101},
        Color {0,   45,  105},
        Color {19,  31,  127},
        Color {60,  19,  124},
        Color {96,  11,  98},
        Color {115, 10,  55},
        Color {113, 15,  7},
        Color {90,  26,  0},
        Color {52,  40,  0},
        Color {11,  52,  0},
        Color {0,   60,  0},
        Color {0,   61,  16},
        Color {0,   56,  64},
        Color {0,   0,   0},
        Color {0,   0,   0},
        Color {0,   0,   0},
        Color {174, 174, 174},
        Color {15,  99,  179},
        Color {64,  81,  208},
        Color {120, 65,  204},
        Color {167, 54,  169},
        Color {192, 52,  112},
        Color {189, 60,  48},
        Color {159, 74,  0},
        Color {109, 92,  0},
        Color {54,  109, 0},
        Color {7,   119, 4},
        Color {0,   121, 61},
        Color {0,   114, 125},
        Color {0,   0,   0},
        Color {0,   0,   0},
        Color {0,   0,   0},
        Color {254, 254, 255},
        Color {93,  179, 255},
        Color {143, 161, 255},
        Color {200, 144, 255},
        Color {247, 133, 250},
        Color {255, 131, 192},
        Color {255, 139, 127},
        Color {239, 154, 73},
        Color {189, 172, 44},
        Color {133, 188, 47},
        Color {85,  199, 83},
        Color {60,  201, 140},
        Color {62,  194, 205},
        Color {78,  78,  78},
        Color {0,   0,   0},
        Color {0,   0,   0},
        Color {254, 254, 255},
        Color {188, 223, 255},
        Color {209, 216, 255},
        Color {232, 209, 255},
        Color {251, 205, 253},
        Color {255, 204, 229},
        Color {255, 207, 202},
        Color {248, 213, 180},
        Color {228, 220, 168},
        Color {204, 227, 169},
        Color {185, 232, 184},
        Color {174, 232, 208},
        Color {175, 229, 234},
        Color {182, 182, 182},
        Color {0,   0,   0},
        Color {0,   0,   0}
    };
};
