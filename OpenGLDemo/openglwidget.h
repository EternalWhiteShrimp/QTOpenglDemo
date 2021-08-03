#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include<QOpenGLWidget>
#include<QOpenGLFunctions>
#include<QOpenGLBuffer>

#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
class QOpenGLShaderProgram;
class VBOTorus;

class OpenGLWidget:public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT
public:
    OpenGLWidget(QWidget *parent=nullptr);
    ~OpenGLWidget();
protected:
    void initializeGL() override;//初始化Opengl
    void resizeGL(int w,int h) override;//重置OpenGL窗口
    void paintGL() override;//绘制OpenGL

    void mousePressEvent(QMouseEvent *event)override;//鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event)override;//鼠标移动事件
    void wheelEvent(QWheelEvent *event)override;//中键移动事件
private:
    QOpenGLShaderProgram *program;//shader处理器
    QOpenGLBuffer vbo;//缓冲区
    VBOTorus *torus;
    QMatrix4x4 model;//模型矩阵
    QMatrix4x4 view;//视角矩阵
    QMatrix4x4 Projection;//投影矩阵
    GLfloat xtrans,ytrans,ztrans;//每轴上的偏移量
    QVector2D mousePos;//鼠标位置

    QQuaternion rotation;//旋转角度
};

#endif // OPENGLWIDGET_H
