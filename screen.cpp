#include "screen.h"
#include <QDebug>
#include <QImage>

const GLchar* vertexSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 TexCoord;

    void main()
    {
        gl_Position = vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }
)glsl";

const GLchar* fragmentSource = R"glsl(
    #version 330 core
    out vec4 FragColor;

    in vec2 TexCoord;

    uniform sampler2D iTexture;

    void main()
    {
        FragColor = texture(iTexture, TexCoord);
        //FragColor = vec4(0.0, 0.0, 0.6, 1.0);
        //FragColor = vec4(TexCoord, 0.0, 1.0);
    }
)glsl";



Screen::Screen(QWidget *parent) : QOpenGLWidget(parent)
{

}

Screen::~Screen()
{
    if (mInitialized)
    {
        glDeleteProgram(mShader);
        glDeleteBuffers(1, &mVBO);
        glDeleteVertexArrays(1, &mVAO);
    }
}

void Screen::VideoUpdate(const ScreenBuffer &buffer)
{
    for (int y = 0; y < 240; y++)
    {
        for (int x = 0; x < 256; x++)
        {
            mFrame[239-y][x] = mPalette[buffer[y][x]];
        }
    }

    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 240, 0, GL_RGB, GL_UNSIGNED_BYTE, &mFrame);
    glGenerateMipmap(GL_TEXTURE_2D);
    update();
}

void Screen::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.75f, 0.0f, 0.0f, 1.0f);

//    float vertices[] = {
//        // positions         // tex coords
//        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
//         1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
//        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
//         1.0f,  1.0f, 0.0f,  1.0f, 1.0f
//    };
    float vertices[] = {
        // first triangle
         1.0f,  1.0f, 0.0f,    1.0f, 1.0f,  // top right
         1.0f, -1.0f, 0.0f,    1.0f, 0.0f,  // bottom right
        -1.0f,  1.0f, 0.0f,    0.0f, 1.0f,// top left
        // second triangle
         1.0f, -1.0f, 0.0f,    1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,    0.0f, 0.0f,  // bottom left
        -1.0f,  1.0f, 0.0f,    0.0f, 1.0f   // top left
    };

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    int  success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        return;
    }

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        return;
    }

    mShader = glCreateProgram();
    glAttachShader(mShader, vertexShader);
    glAttachShader(mShader, fragmentShader);
    glLinkProgram(mShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 240, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glGenerateMipmap(GL_TEXTURE_2D);
    mInitialized = true;
}
void Screen::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Screen::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glUseProgram(mShader);
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
