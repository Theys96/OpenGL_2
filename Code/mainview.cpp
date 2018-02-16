#include "mainview.h"
#include "math.h"
#include "shapes/vertex.h"
#include "shapes/multipolygon.h"

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
    glEnable(GL_CULL_FACE);

    // Default is GL_LESS
    glDepthFunc(GL_LEQUAL);

    // Set the color of the screen to be black on clear (new frame)
    glClearColor(0.2f, 0.5f, 0.7f, 0.0f);

    createShaderProgram();

    MultiPolygon cube = MultiPolygon(8);
    cube.setPoint(0, -1, -1, 1);
    cube.setPoint(1, 1, -1, 1);
    cube.setPoint(2, 1, -1, -1);
    cube.setPoint(3, -1, -1, -1);
    cube.setPoint(4, -1, 1, 1);
    cube.setPoint(5, 1, 1, 1);
    cube.setPoint(6, 1, 1, -1);
    cube.setPoint(7, -1, 1, -1);
    cube.addFace(0, 3, 2, 1);
    cube.addFace(0, 1, 5, 4);
    cube.addFace(7, 4, 5, 6);
    cube.addFace(7, 6, 2, 3);
    cube.addFace(7, 3, 0, 4);
    cube.addFace(5, 1, 2, 6);

    MultiPolygon pyramid = MultiPolygon(5);
    pyramid.setPoint(0, -1, -1, 1);
    pyramid.setPoint(1, 1, -1, 1);
    pyramid.setPoint(2, 1, -1, -1);
    pyramid.setPoint(3, -1, -1, -1);
    pyramid.setPoint(4, 0, 1, 0);
    pyramid.addFace(0, 3, 2, 1);
    pyramid.addPolygon(0, 1, 4);
    pyramid.addPolygon(1, 2, 4);
    pyramid.addPolygon(2, 3, 4);
    pyramid.addPolygon(3, 0, 4);

    Model sphere = Model(":/models/sphere.obj", 0.04);
    QVector<vertex> sphereVertices = sphere.getVertexStructs();
    sphereLen = sphereVertices.length();

    cubeTransform.translate(2,-1.5,-6);
    pyramidTransform.translate(-2,-0.5,-6);
    sphereTransform.translate(0,4,-15);
    projTransform.perspective(60, (float) width()/height(), nearPlane, farPlane);

    glGenBuffers(1, &vbo1);
    glGenVertexArrays(1, &vao1);

    glBindVertexArray(vao1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, cube.numFloats()*sizeof(vertex), cube.getArrayVector().data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(float) * 3));

    glGenBuffers(1, &vbo2);
    glGenVertexArrays(1, &vao2);

    glBindVertexArray(vao2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, pyramid.numFloats()*sizeof(float), pyramid.getArrayVector().data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(float) * 3));

    glGenBuffers(1, &vbo3);
    glGenVertexArrays(1, &vao3);

    glBindVertexArray(vao3);
    glBindBuffer(GL_ARRAY_BUFFER, vbo3);
    glBufferData(GL_ARRAY_BUFFER, sphereLen*sizeof(vertex), sphereVertices.data(), GL_STATIC_DRAW);
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

    glUniformMatrix4fv(transformMatrix, 1, GL_FALSE, sphereTransform.data());
    glBindVertexArray(vao3);
    glDrawArrays(GL_TRIANGLES, 0, sphereLen);

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
