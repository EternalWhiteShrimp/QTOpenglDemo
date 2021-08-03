#include "glslwidget.h"
#include "ui_glslwidget.h"
#include <QGridLayout>
#include"openglwidget.h"
GLSLWidget::GLSLWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GLSLWidget)
{
    ui->setupUi(this);
    QGridLayout *layout=new QGridLayout();//通过layout将OpenglWidget放置到窗口中
    glWidget=new OpenGLWidget();
    layout->addWidget(glWidget);
    setLayout(layout);
}

GLSLWidget::~GLSLWidget()
{
    delete ui;
}
