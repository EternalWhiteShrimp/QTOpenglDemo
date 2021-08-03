#include "vbotorus.h"
#include <QOpenGLVertexArrayObject>
#include <QtMath>

VBOTorus::VBOTorus(float outerRaius, float innerRadius, float nrings, int nsides):rings(nrings),sides(nsides)
{
    initializeOpenGLFunctions();
    faces=sides*rings;
    int nVerts= sides*(rings+1);
    //顶点
    float *v=new float[3*nVerts];
    //法向
    float *n=new float[3*nVerts];
    //贴图对齐
    float *tex=new float[2*nVerts];
    //元素
    unsigned int *el=new unsigned int[6*faces];

    //生成顶点数据
    generateVerts(v,n,tex,el,outerRaius,innerRadius);

    //创造和插入缓存对象
    unsigned int handle[4];
    glGenBuffers(4,handle);
    glBindBuffer(GL_ARRAY_BUFFER,handle[0]);
    glBufferData(GL_ARRAY_BUFFER,(3*nVerts)*sizeof(float),v,GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,handle[1]);
    glBufferData(GL_ARRAY_BUFFER,(3*nVerts)*sizeof(float),n,GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,handle[2]);
    glBufferData(GL_ARRAY_BUFFER,(2*nVerts)*sizeof(float),tex,GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,handle[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,(6*faces)*sizeof(unsigned int),el,GL_STATIC_DRAW);

    delete[] v;
    delete[] tex;
    delete[] n;
    delete[] el;

    vao.create();
    vao.bind();
    //绑定顶点位置数据
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,handle[0]);
    glVertexAttribPointer((GLuint)0,3,GL_FLOAT,GL_FALSE,0,((GLubyte*)NULL+(0)));
    //绑定顶点法向
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,handle[1]);
    glVertexAttribPointer((GLuint)1,3,GL_FLOAT,GL_FALSE,0,((GLubyte*)NULL+(0)));
    //绑定贴图
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER,handle[2]);
    glVertexAttribPointer((GLuint)2,2,GL_FLOAT,GL_FALSE,0,((GLubyte*)NULL+(0)));
    //绑定节点
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,handle[3]);

    vao.release();
}
VBOTorus::~VBOTorus()
{

}

void VBOTorus::render()
{
    vao.bind();
    //绘制结点
    glDrawElements(GL_TRIANGLES,6*faces,GL_UNSIGNED_INT,((GLbyte*)NULL+(0)));
    vao.release();
}

void VBOTorus::generateVerts(float *verts, float *norms, float *tex, unsigned int *el, float outerRadius, float innerRadius)
{
    float ringFactor = 2.0f * M_PI / rings;
       float sideFactor = 2.0f * M_PI / sides;
       int idx = 0, tidx = 0;
       for( int ring = 0; ring <= rings; ring++ ) {
           float u = ring * ringFactor;
           float cu = cos(u);
           float su = sin(u);
           for( int side = 0; side < sides; side++ ) {
               float v = side * sideFactor;
               float cv = cos(v);
               float sv = sin(v);
               float r = (outerRadius + innerRadius * cv);
               verts[idx] = r * cu;
               verts[idx + 1] = r * su;
               verts[idx + 2] = innerRadius * sv;
               norms[idx] = cv * cu * r;
               norms[idx + 1] = cv * su * r;
               norms[idx + 2] = sv * r;
               tex[tidx] = u / (2.0 * M_PI);
               tex[tidx + 1] = v / (2.0 * M_PI);
               tidx += 2;
               // Normalize
               float len = qSqrt( norms[idx] * norms[idx] +
                                  norms[idx+1] * norms[idx+1] +
                       norms[idx+2] * norms[idx+2] );
               norms[idx] /= len;
               norms[idx+1] /= len;
               norms[idx+2] /= len;
               idx += 3;
           }
       }

       idx = 0;
       for( int ring = 0; ring < rings; ring++ ) {
           int ringStart = ring * sides;
           int nextRingStart = (ring + 1) * sides;
           for( int side = 0; side < sides; side++ ) {
               int nextSide = (side+1) % sides;
               // The quad
               el[idx] = (ringStart + side);
               el[idx+1] = (nextRingStart + side);
               el[idx+2] = (nextRingStart + nextSide);
               el[idx+3] = ringStart + side;
               el[idx+4] = nextRingStart + nextSide;
               el[idx+5] = (ringStart + nextSide);
               idx += 6;
           }
       }

}

