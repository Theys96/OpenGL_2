#include "mainview.h"
#include "math.h"
#include "shapes/vertex.h"
#include "shapes/multipolygon.h"
#include "shapes/simpleshapes.h"

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

    // Kind of hackish way to be able to control the scale slider
    scaleSlider = this->parent()->findChild<QSlider*>("ScaleSlider");
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

    // Cube, from definition
    Shape cubeShape = SimpleShapes::getCube();
    cubeShape.transformation.translate(2,-1.5,-6);

    // Pyramid, from definition
    Shape pyramidShape = SimpleShapes::getPyramid();
    pyramidShape.transformation.translate(-2,-0.5,-6);

    // Sphere, from file
    Shape sphereShape = Shape(":/models/sphere.obj", 0.04);
    sphereShape.transformation.translate(0,4,-15);

    // Combine shapes in a vector
    shapes.append(sphereShape);
    shapes.append(pyramidShape);
    shapes.append(cubeShape);

    // Bind shape array buffers
    for (int i = 0; i < shapes.length(); i++) {
        glGenBuffers(1, &(shapes[i].vbo));
        glGenVertexArrays(1, &(shapes[i].vao));
        glBindVertexArray(shapes[i].vao);
        glBindBuffer(GL_ARRAY_BUFFER, shapes[i].vbo);
        glBufferData(GL_ARRAY_BUFFER, shapes[i].getBufferSize(), shapes[i].getBufferData(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(float) * 3));
    }

    // Define perspective, allocate transformation matrices
    projection.perspective(60, (float) width()/height(), nearPlane, farPlane);
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

    // Transformations
    glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE, projection.data());
    glUniformMatrix4fv(rotateMatrix, 1, GL_FALSE, rotation.data());
    glUniformMatrix4fv(scalingMatrix, 1, GL_FALSE, scaling.data());

    // Shapes
    for (int i = 0; i < shapes.length(); i++) {
        glUniformMatrix4fv(transformMatrix, 1, GL_FALSE, shapes[i].transformation.data());
        glBindVertexArray(shapes[i].vao);
        glDrawArrays(GL_TRIANGLES, 0, shapes[i].numTriangles());
    }

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
    projection.setToIdentity();
    projection.perspective(60, (float) newWidth/newHeight, nearPlane, farPlane);
}

// --- Public interface

void MainView::setRotation(int rotateX, int rotateY, int rotateZ)
{
    rotation.setToIdentity();
    rotation.rotate(rotateX, 1, 0, 0);
    rotation.rotate(rotateY, 0, 1, 0);
    rotation.rotate(rotateZ, 0, 0, 1);
    update();
}

void MainView::setScale(int newScale)
{
    scale = fmax(1,fmin(200, newScale));
    scaling.setToIdentity();
    scaling.scale((float)scale/100);
    scaleSlider->setValue(scale);
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
