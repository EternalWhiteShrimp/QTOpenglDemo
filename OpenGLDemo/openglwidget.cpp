#include "openglwidget.h"
#include<QOpenGLShader>
#include<QOpenGLShaderProgram>
#include<exception>
#include<QDebug>
#include<QDir>
#include<QMouseEvent>
#include "vbotorus.h"
void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    qDebug()<<QDir::currentPath();
    //顶点着色器
    QOpenGLShader *vShader=new QOpenGLShader(QOpenGLShader::Vertex,this);
    vShader->compileSourceFile("F:\\QtProject\\QTLearning\\OpenGLDemo/Shader/geometry.vert");//需要在运行目录下放置Shader文件


    //片元着色器
    QOpenGLShader *fShader=new QOpenGLShader(QOpenGLShader::Fragment,this);
    fShader->compileSourceFile("F:\\QtProject\\QTLearning\\OpenGLDemo/Shader/geometry.frag");
    
    program=new QOpenGLShaderProgram;
    program->addShader(vShader);
    program->addShader(fShader);
    program->link();
    program->bind();
    
    glClearColor(0.5f, 0.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    //顶点数组
    torus=new VBOTorus(0.7f,0.3f,30,30);

    //视角矩阵
    view.setToIdentity();
    view.lookAt(QVector3D(0.0f,0.0f,3.0f),QVector3D(0.0f,0.0f,0.0f),QVector3D(0.0f,1.0f,0.0f));
    //设置 漫反射系数、光强、灯光位置
    program->setUniformValue("Kd",QVector3D(0.9f,0.5f,0.3f));
    program->setUniformValue("Ld",QVector3D(1.0,1.0f,1.0f));//设置为白光
    program->setUniformValue("LightPosition",view*QVector4D(0.0f,0.0f,5.0f,1.0f));
    
}

void OpenGLWidget::resizeGL(int w, int h)
{
    //设置视口大小
    glViewport(0,0,w/2,h/2);
    //设置投影矩阵
    Projection.setToIdentity();
    Projection.perspective(60.0f,(GLfloat)w/(GLfloat)h,1.0f,100.0f);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    model.setToIdentity();
    model.translate(xtrans,ytrans,ztrans);//移动上的变换
    model.rotate(rotation);//旋转上的变换
    QMatrix4x4 mv= view * model;
    //给顶点着色器中的各种矩阵赋值
    program->setUniformValue("ModelViewMatrix",mv);
    program->setUniformValue("NormalMatrix",mv.normalMatrix());
    program->setUniformValue("MVP",Projection*mv);
    program->bind();
    //绘制torus
    torus->render();
}
void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    //获取鼠标位置
    mousePos=QVector2D(event->pos());
    event->accept();
}
void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{

    if(event->buttons()==Qt::LeftButton)//注意这里需要buttons,不是button
    {
        QVector2D newPos=(QVector2D) event->pos();
        QVector2D diff=newPos-mousePos;//获取差值
        qreal angle=(diff.length())/3.6;//计算角度
        //旋转轴等于diff的垂直向量
        QVector3D rotationAxis=QVector3D(diff.y(),diff.x(),0.0).normalized();
        rotation=QQuaternion::fromAxisAndAngle(rotationAxis,angle)*rotation;//在原本的角度上做旋转
        mousePos=newPos;//更新鼠标位置
        this->update();
    }
    event->accept();
}
void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees=event->angleDelta()/8;
    if(numDegrees.y()>0)//中键前推
    {
        ztrans+=0.25f;//放大
    }
    else if(numDegrees.y()<0)
    {
        ztrans-=0.25f;//缩小
    }
    this->update();
    event->accept();
}
/*
非几何体的openGL绘制
*/
/*
void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //顶点数组
    GLfloat positionData[]={
        -0.8f, -0.8f, 0.0f,
        0.8f, -0.8f, 0.0f,
        0.0f, 0.8f, 0.0f
    };
    GLfloat colorData[]={
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
    };
    //顶点缓冲区对象
    vbo.create();
    vbo.bind();
    //顶点位置
    vbo.allocate(positionData,18*sizeof(GLfloat));
    GLuint vPosition=program->attributeLocation("VertexPosition");
    program->setAttributeBuffer(vPosition,GL_FLOAT,0,3,0);
    glEnableVertexAttribArray(vPosition);
    //顶点颜色
    vbo.write(9*sizeof(GLfloat), colorData, 9*sizeof(GLfloat));
    GLuint vColor = program->attributeLocation("VertexColor");
    program->setAttributeBuffer(vColor,GL_FLOAT,9*sizeof(GLfloat),3,0);
    glEnableVertexAttribArray(vColor);


    int w=width();
    int h=height();
    QMatrix4x4 matrix;
    matrix.perspective(45.0f,(GLfloat)w/(GLfloat)h,1.0f,100.0f);
    matrix.translate(0.0,0.0,-3.0);
    matrix.rotate(90.0f,QVector3D(0.0f,0.0f,1.0f));//第一个值是角度，后面的是轴
    program->setUniformValue("RotationMatrix",matrix);

    glDrawArrays(GL_TRIANGLES,0,3);
}
*/
OpenGLWidget::~OpenGLWidget()
{
    delete torus;
    delete program;
}



OpenGLWidget::OpenGLWidget(QWidget *parent):QOpenGLWidget(parent),xtrans(0),ytrans(0),ztrans(0)
{

}
