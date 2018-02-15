#include "mainview.h"
#include "math.h"
#include "vertex.h"

#include <QDateTime>

/**
 * @brief MainView::MainView
 *
 * Constructor of MainView
 *
 * @param parent
 */
MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "MainView constructor";

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
}

/**
 * @brief MainView::~MainView
 *
 * Destructor of MainView
 * This is the last function called, before exit of the program
 * Use this to clean up your variables, buffers etc.
 *
 */
MainView::~MainView() {
    debugLogger->stopLogging();

    qDebug() << "MainView destructor";
}

// --- OpenGL initialization

/**
 * @brief MainView::initializeGL
 *
 * Called upon OpenGL initialization
 * Attaches a debugger and calls other init functions
 */
void MainView::initializeGL() {
    qDebug() << ":: Initializing OpenGL";
    initializeOpenGLFunctions();

    debugLogger = new QOpenGLDebugLogger();
    connect( debugLogger, SIGNAL( messageLogged( QOpenGLDebugMessage ) ),
             this, SLOT( onMessageLogged( QOpenGLDebugMessage ) ), Qt::DirectConnection );

    if ( debugLogger->initialize() ) {
        qDebug() << ":: Logging initialized";
        debugLogger->startLogging( QOpenGLDebugLogger::SynchronousLogging );
        debugLogger->enableMessages();
    }

    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling
    //glEnable(GL_CULL_FACE);

    // Default is GL_LESS
    glDepthFunc(GL_LEQUAL);

    // Set the color of the screen to be black on clear (new frame)
    glClearColor(0.2f, 0.5f, 0.7f, 0.0f);

    createShaderProgram();

    vertex cube[36];
    cube[0] = {-1,-1,-1,0,0,0};
    cube[1] = {-1,-1,1,0,0,1};
    cube[2] = {-1,1,-1,0,1,0};

    cube[3] = {-1,1,1,0,1,1};
    cube[4] = {-1,-1,1,0,0,1};
    cube[5] = {-1,1,-1,0,1,0};

    cube[6] = {1,-1,-1,0,0,0};
    cube[7] = {1,-1,1,0,0,1};
    cube[8] = {1,1,-1,0,1,0};

    cube[9] = {1,1,1,0,1,1};
    cube[10] = {1,-1,1,0,0,1};
    cube[11] = {1,1,-1,0,1,0};

    cube[12] = {-1,-1,-1,0,0,0};
    cube[13] = {-1,-1,1,0,0,1};
    cube[14] = {1,-1,-1,1,0,0};

    cube[15] = {1,-1,1,1,0,1};
    cube[16] = {-1,-1,1,0,0,1};
    cube[17] = {1,-1,-1,1,0,0};

    cube[18] = {-1,1,-1,0,1,0};
    cube[19] = {-1,1,1,0,1,1};
    cube[20] = {1,1,-1,1,1,0};

    cube[21] = {1,1,1,1,1,1};
    cube[22] = {-1,1,1,0,1,1};
    cube[23] = {1,1,-1,1,1,0};

    cube[24] = {-1,-1,-1,0,0,0};
    cube[25] = {-1,1,-1,0,1,0};
    cube[26] = {1,-1,-1,1,0,0};

    cube[27] = {1,1,-1,1,1,0};
    cube[28] = {-1,1,-1,0,1,0};
    cube[29] = {1,-1,-1,1,0,0};

    cube[30] = {-1,-1,1,0,0,1};
    cube[31] = {-1,1,1,0,1,1};
    cube[32] = {1,-1,1,1,0,1};

    cube[33] = {1,1,1,1,1,1};
    cube[34] = {-1,1,1,0,1,1};
    cube[35] = {1,-1,1,1,0,1};

    vertex pyramid[18];
    pyramid[0] = {-1,-1,-1,0,0,0};
    pyramid[1] = {-1,-1,1,0,0,1};
    pyramid[2] = {1,-1,-1,1,0,0};

    pyramid[3] = {1,-1,1,1,0,1};
    pyramid[4] = {-1,-1,1,0,0,1};
    pyramid[5] = {1,-1,-1,1,0,0};

    pyramid[6] = {0,1,0,0,1,0};
    pyramid[7] = {-1,-1,-1,0,0,0};
    pyramid[8] = {-1,-1,1,0,0,1};

    pyramid[9] = {0,1,0,0,1,0};
    pyramid[10] = {-1,-1,-1,0,0,0};
    pyramid[11] = {1,-1,-1,1,0,0};

    pyramid[12] = {0,1,0,0,1,0};
    pyramid[13] = {1,-1,1,1,0,1};
    pyramid[14] = {-1,-1,1,0,0,1};

    pyramid[15] = {0,1,0,0,1,0};
    pyramid[16] = {1,-1,1,1,0,1};
    pyramid[17] = {1,-1,-1,1,0,0};

    cubeTransform.translate(2,0,-6);
    pyramidTransform.translate(-2,0,-6);

    projTransform.perspective(60, (float) width()/height(), nearPlane, farPlane);

    glGenBuffers(1, &vbo1);
    glGenVertexArrays(1, &vao1);

    glBindVertexArray(vao1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, 36*sizeof(vertex), cube, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(float) * 3));

    glGenBuffers(1, &vbo2);
    glGenVertexArrays(1, &vao2);

    glBindVertexArray(vao2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, 18*sizeof(vertex), pyramid, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(float) * 3));

    transformMatrix = shaderProgram.uniformLocation("modelTransform");
    rotateMatrix = shaderProgram.uniformLocation("modelRotate");
    scalingMatrix = shaderProgram.uniformLocation("modelScale");
    projectionMatrix = shaderProgram.uniformLocation("projectionTransform");
}

void MainView::createShaderProgram()
{
    // Create shader program
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader.glsl");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader.glsl");
    shaderProgram.link();
}

// --- OpenGL drawing

/**
 * @brief MainView::paintGL
 *
 * Actual function used for drawing to the screen
 *
 */
void MainView::paintGL() {
    // Clear the screen before rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.bind();

    // Draw here
    glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE, projTransform.data());
    glUniformMatrix4fv(rotateMatrix, 1, GL_FALSE, rotation.data());
    glUniformMatrix4fv(scalingMatrix, 1, GL_FALSE, scaling.data());

    glUniformMatrix4fv(transformMatrix, 1, GL_FALSE, cubeTransform.data());
    glBindVertexArray(vao1);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glUniformMatrix4fv(transformMatrix, 1, GL_FALSE, pyramidTransform.data());
    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 0, 18);

    shaderProgram.release();
}

/**
 * @brief MainView::resizeGL
 *
 * Called upon resizing of the screen
 *
 * @param newWidth
 * @param newHeight
 */
void MainView::resizeGL(int newWidth, int newHeight) 
{
    projTransform.setToIdentity();
    projTransform.perspective(60, (float) newWidth/newHeight, nearPlane, farPlane);
}

// --- Public interface

void MainView::setRotation(int rotateX, int rotateY, int rotateZ)
{
    qDebug() << "Rotation changed to (" << rotateX << "," << rotateY << "," << rotateZ << ")";
    rotation.setToIdentity();
    rotation.rotate(rotateX, 1, 0, 0);
    rotation.rotate(rotateY, 0, 1, 0);
    rotation.rotate(rotateZ, 0, 0, 1);
    update();
    qDebug() << rotation.data();
}

void MainView::setScale(int scale)
{
    qDebug() << "Scale changed to " << scale;
    scaling.setToIdentity();
    scaling.scale((float) scale/100);
    update();
}

void MainView::setShadingMode(ShadingMode shading)
{
    qDebug() << "Changed shading to" << shading;
    Q_UNIMPLEMENTED();
}

// --- Private helpers

/**
 * @brief MainView::onMessageLogged
 *
 * OpenGL logging function, do not change
 *
 * @param Message
 */
void MainView::onMessageLogged( QOpenGLDebugMessage Message ) {
    qDebug() << " → Log:" << Message;
}
