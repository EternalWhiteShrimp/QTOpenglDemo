#ifndef VBOTORUS_H
#define VBOTORUS_H
#include <QOpenGLFunctions>
#include<QOpenGLVertexArrayObject>
class  QOpenGLVertexArrayObject;

class VBOTorus:protected QOpenGLFunctions
{
public:
    VBOTorus(float outerRaius,float innerRadius,float nrings,int nsides);//初始化
    ~VBOTorus();
    void render();//渲染
    int getVertexArrayHandle();//
protected:
    QOpenGLVertexArrayObject vao;//opengl顶点数组对象
private:
    int faces,rings,sides;//面数,原形,方面
    void generateVerts(float* verts,float *norms,float *tex,
                       unsigned int *el,
                       float outerRadius,float innerRadius);//生成节点
};

#endif // VBOTORUS_H
