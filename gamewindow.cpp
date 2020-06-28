//#include "gamewindow.h"

//#include <QDebug>
//#include <QKeyEvent>


//// Shader sources
//const GLchar* vertexSource = R"glsl(
//    #version 330 core
//    layout (location = 0) in vec3 aPos;
//    layout (location = 1) in vec2 aTexCoord;

//    out vec2 TexCoord;

//    void main()
//    {
//        gl_Position = vec4(aPos, 1.0);
//        TexCoord = aTexCoord;
//    }
//)glsl";

//const GLchar* fragmentSource = R"glsl(
//    #version 330 core
//    out vec4 FragColor;

//    in vec2 TexCoord;

//    uniform sampler2D iTexture;

//    void main()
//    {
//        FragColor = texture(iTexture, TexCoord);
//        //FragColor = vec4(1.0, 0.0, 0.6, 1.0);
//        //FragColor = vec4(TexCoord, 0.0, 1.0);
//    }
//)glsl";

//unsigned char texData[256*256*3];
//unsigned char texData2[256*256*3];


//GameWindow::GameWindow(QWidget *parent) : QOpenGLWidget(parent)
//{

////    setStyleSheet("background: rgb(85, 170, 255)");
////    setGeometry(0, 0, 512, 480);
////    update();
////    new QPushButton("THIS IS A TEST", this);

//    for (int i = 0; i < 240; i++)
//    {
//        for (int j = 0; j < 256; j++)
//        {
//            if (i < 128 && j < 128)
//            {
//                texData[(i*256*3)+(j*3)+0] = 255;
//                texData[(i*256*3)+(j*3)+1] = 0;
//                texData[(i*256*3)+(j*3)+2] = 0;
//            }
//            else if (i < 128)
//            {
//                texData[(i*256*3)+(j*3)+0] = 0;
//                texData[(i*256*3)+(j*3)+1] = 255;
//                texData[(i*256*3)+(j*3)+2] = 0;
//            }
//            else if (j < 128)
//            {
//                texData[(i*256*3)+(j*3)+0] = 0;
//                texData[(i*256*3)+(j*3)+1] = 0;
//                texData[(i*256*3)+(j*3)+2] = 255;
//            }
//            else
//            {
//                texData[(i*256*3)+(j*3)+0] = 30;
//                texData[(i*256*3)+(j*3)+1] = 60;
//                texData[(i*256*3)+(j*3)+2] = 150;
//            }
//        }
//    }
//    for (int i = 0; i < 240; i++)
//    {
//        for (int j = 0; j < 256; j++)
//        {
//            if (i < 128 && j < 128)
//            {
//                texData2[(i*256*3)+(j*3)+0] = 150;
//                texData2[(i*256*3)+(j*3)+1] = 60;
//                texData2[(i*256*3)+(j*3)+2] = 30;
//            }
//            else if (i < 128)
//            {
//                texData2[(i*256*3)+(j*3)+0] = 90;
//                texData2[(i*256*3)+(j*3)+1] = 12;
//                texData2[(i*256*3)+(j*3)+2] = 106;
//            }
//            else if (j < 128)
//            {
//                texData2[(i*256*3)+(j*3)+0] = 45;
//                texData2[(i*256*3)+(j*3)+1] = 240;
//                texData2[(i*256*3)+(j*3)+2] = 80;
//            }
//            else
//            {
//                texData2[(i*256*3)+(j*3)+0] = 190;
//                texData2[(i*256*3)+(j*3)+1] = 24;
//                texData2[(i*256*3)+(j*3)+2] = 85;
//            }
//        }
//    }
//}

//GameWindow::~GameWindow()
//{
//    glDeleteProgram(mShader);
//    glDeleteBuffers(1, &mVBO);
//    glDeleteVertexArrays(1, &mVAO);
//}

//void GameWindow::setNes(NES *nes)
//{
//    mNes = nes;
//}

//void GameWindow::VideoUpdate(const ScreenBuffer &buffer)
//{
//    for (int y = 0; y < 240; y++)
//    {
//        for (int x = 0; x < 256; x++)
//        {
//            mFrame[239-y][x] = mPalette[buffer[y][x]];
//        }
//    }

//    glBindTexture(GL_TEXTURE_2D, mTexture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 240, 0, GL_RGB, GL_UNSIGNED_BYTE, &mFrame);
//    update();
//}

//const ButtonState &GameWindow::InputUpdate()
//{
//    return mInput;
//}

//void GameWindow::initializeGL()
//{
//    initializeOpenGLFunctions();
//    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

//    float vertices[] = {
//        // positions         // tex coords
//        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
//         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
//        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
//         1.0f,  1.0f, 0.0f,  1.0f, 1.0f
//    };

//    glGenVertexArrays(1, &mVAO);
//    glBindVertexArray(mVAO);
//    glGenBuffers(1, &mVBO);
//    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);

//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexSource, NULL);
//    glCompileShader(vertexShader);

//    int  success;
//    char infoLog[512];
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//    if(!success)
//    {
//        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog;
//    }

//    GLuint fragmentShader;
//    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
//    glCompileShader(fragmentShader);

//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//    if(!success)
//    {
//        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
//    }

//    mShader = glCreateProgram();
//    glAttachShader(mShader, vertexShader);
//    glAttachShader(mShader, fragmentShader);
//    glLinkProgram(mShader);

//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);

//    glGenTextures(1, &mTexture);
//    glBindTexture(GL_TEXTURE_2D, mTexture);
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, &texData[0]);

//    startTimer(16, Qt::TimerType::PreciseTimer);
//}

//void GameWindow::resizeGL(int w, int h)
//{
//    glViewport(0, 0, w, h);
//}

//void GameWindow::paintGL()
//{
//    glClear(GL_COLOR_BUFFER_BIT);

//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, mTexture);
//    glUseProgram(mShader);
//    glBindVertexArray(mVAO);
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//}

//void GameWindow::timerEvent(QTimerEvent *)
//{
//    while(!mNes->Step());
//}

//void GameWindow::keyPressEvent(QKeyEvent *event)
//{
////    qDebug() << "Pressed" << event;
////    if (event->key() == Qt::Key_F && !event->isAutoRepeat())
////    {
////        glBindTexture(GL_TEXTURE_2D, mTexture);
////        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, &texData2[0]);
////        update();
////    }
////    else if (event->key() == Qt::Key_D && !event->isAutoRepeat())
////    {
////        glBindTexture(GL_TEXTURE_2D, mTexture);
////        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, &texData[0]);
////        update();
////    }
////    else if (event->key() == Qt::Key_Space)
////    {
////        qDebug() << "step start";
////        for (int i = 0; i < 50000; i++)
////            mNes->Step();
////        qDebug() << "step end";
////    }
//    switch (event->key()) {
//    case Qt::Key_Up:
//        mInput.up = true;
//        break;
//    case Qt::Key_Down:
//        mInput.down = true;
//        break;
//    case Qt::Key_Left:
//        mInput.left = true;
//        break;
//    case Qt::Key_Right:
//        mInput.right = true;
//        break;
//    case Qt::Key_F:
//        mInput.a = true;
//        break;
//    case Qt::Key_D:
//        mInput.b = true;
//        break;
//    case Qt::Key_Return:
//        mInput.start = true;
//        break;
//    case Qt::Key_Shift:
//        mInput.select = true;
//        break;
//    }
//}

//void GameWindow::keyReleaseEvent(QKeyEvent *event)
//{
////    qDebug() << "Released" << event;
//    switch (event->key()) {
//    case Qt::Key_Up:
//        mInput.up = false;
//        break;
//    case Qt::Key_Down:
//        mInput.down = false;
//        break;
//    case Qt::Key_Left:
//        mInput.left = false;
//        break;
//    case Qt::Key_Right:
//        mInput.right = false;
//        break;
//    case Qt::Key_F:
//        mInput.a = false;
//        break;
//    case Qt::Key_D:
//        mInput.b = false;
//        break;
//    case Qt::Key_Return:
//        mInput.start = false;
//        break;
//    case Qt::Key_Shift:
//        mInput.select = false;
//        break;
//    }
//}

