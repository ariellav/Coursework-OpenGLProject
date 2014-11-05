#include "GLDisplay.h"
#include "Core3_2_context.h"
#include "glheader.h"
#include <QCoreApplication>
#include <iostream>
#include <QtGui>
#include <QDebug>

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <QQuaternion>
#include <QTimer>

#include "Sphere.h"
using namespace glm;

void scale_normals(float x, float y, float z, std::vector<glm::vec3> &normals);


// Constructor: Set everything to zero and ensure we have an OpenGL 3.2 context (on Mac)
GLDisplay::GLDisplay(QWidget *parent)
  : QGLWidget(new Core3_2_context(QGLFormat::defaultFormat()), parent),
    mVertexArrayObjectID(0),
    mTranslateX(0.0), mTranslateY(0.0), mTranslateZ(0.0)
{
    sOneBufferID[0] = 0;
    sOneBufferID[1] = 0;
    sOneBufferID[2] = 0;

    mKAR = 0.0,mKAG = 0.0,mKAB = 1.0;
    mKDR = 0.0,mKDG = 0.0,mKDB = 0.0;
    mKSR = 0.0,mKSG = 0.0,mKSB = 0.0;

    C_X = 0,C_Y = 0,C_Z = 1.0;

    R_X = 0, R_Y = 0, R_Z = 0;

    LX = 5.0, LY = 0.0, LZ = -5.0;

    shininess = 50;

    smoothShader = false;

    QFileDialog Open;
    fileName = Open.getOpenFileName();

    lastPos = QPoint(0,0);

    R_tot = QQuaternion();
    Model = glm::mat4(1.0f);
    Model = translate(Model,vec3(0,0,-10));

    lookObject = glm::vec3(0.0,0.0,C_Z-100);
    tilt = glm::vec3(0.0,1.0,0.0);

    Model2 = glm::mat4(1.0f);

    scale_normals(0.01,0.01,0.01,sphere_2.normals);

    //Add Timer
        o_x = 0; R_A = 0; R_T =0;t_z=0;
        timer = new QTimer(this);
        connect(timer,SIGNAL(timeout()), this, SLOT(tick()));
        timer->start(10);
}


void GLDisplay::initializeGL(){
    glSetup();

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    glGenVertexArrays(1, &mVertexArrayObjectID);
    glBindVertexArray(mVertexArrayObjectID);

    const char* f = fileName.toStdString().c_str();
    load_obj("sphere.obj", &sphere_1);
    load_obj(f, &sphere_2);


    //-----------------------MY EARTH----------------------
    init_buffers(sOneBufferID,sphere_1);

    //-----------------------MY MOON----------------------
    init_buffers(sTwoBufferID,sphere_2);


    // Load and compile shaders
#if defined(Q_OS_MAC)
    QString shaderDir = QCoreApplication::applicationDirPath() + QString("/../Resources/");
    QString vertexShaderFile = shaderDir + QString("VertexShaderSmooth.vertexshader");
    QString fragmentShaderFile = shaderDir + QString("FragmentShaderSmooth.fragmentshader");
    mSmoothShaderProgramID = LoadShaders(vertexShaderFile.toLatin1().constData(), fragmentShaderFile.toLatin1().constData());
    vertexShaderFile = shaderDir + QString("VertexShaderPhong.vertexshader");
    fragmentShaderFile = shaderDir + QString("FragmentShaderPhong.fragmentshader");
    mPhongShaderProgramID = LoadShaders(vertexShaderFile.toLatin1().constData(), fragmentShaderFile.toLatin1().constData());
#else
    // t = LoadShaders("VertexShader.vertexshader", "FragmentShader.fragmentshader");
    mPhongShaderProgramID = LoadShaders("VertexShaderPhong.vertexshader", "FragmentShaderPhong.fragmentshader");
#endif
}


void GLDisplay::init_buffers(GLuint BID[3], Mesh mesh){
    // Create three buffers, one for vertices, one for normals, one for indices
    glGenBuffers(3, BID);

    // Load vertex data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, BID[0]);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size()*sizeof(glm::vec4), &mesh.vertices[0], GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, BID[1]);
    glBufferData(GL_ARRAY_BUFFER, mesh.normals.size()*sizeof(glm::vec3), &mesh.normals[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BID[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.elements.size()*sizeof(GLuint), &mesh.elements[0], GL_STATIC_DRAW);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_index_buffer_data), g_index_buffer_data, GL_STATIC_DRAW);
}

void GLDisplay::resizeGL(int w, int h)
{
    // setup viewport, projection etc.:
    glViewport(0, 0, (GLint)w, (GLint)h);
    // float mAspect = GLfloat(w) / GLfloat(h);
}

void GLDisplay::paintGL(){
  
    float mAspect = this->width()/this->height();
    // Clear Screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable appropriate shader progream
    GLuint shaderProgramID =  mPhongShaderProgramID;
    glUseProgram(shaderProgramID);


    glm::mat4 myView = glm::lookAt(
                glm::vec3(C_X,C_Y,C_Z), // eye position
                         lookObject, // and looks at the origin
                         tilt  // Head is up (set to 0,-1,0 to look upside-down)
                         );

    glm::mat4 projectionMatrix = glm::perspective(45.0f, mAspect, 0.5f, 1000.0f);

    //Model 1
    glm::mat4 Model1 = translate(Model, vec3(5,0,0));
    Model1 = rotate(Model1,t_z,vec3(0,1,0));
    glm::mat4 modelViewMatrix = myView * Model1;
    GLfloat Amb[3] = {mKAR,mKAG,mKAB};
    GLfloat Dif[3] = {mKDR,mKDG,mKDB};
    GLfloat Spc[3] = {mKSR,mKSG,mKSB};
    setUp_GL(modelViewMatrix,projectionMatrix,myView,Model1,Amb,Dif,Spc,sOneBufferID,sphere_1,shaderProgramID);

    //Model 2
    Model2 = glm::rotate(Model1, o_x, glm::vec3(0, 1, 0));
    Model2 =  translate(Model2,vec3(3,0,0));
    Model2 = scale(Model2,vec3(0.01,0.01,0.01));
    modelViewMatrix = myView * Model2;
    GLfloat Amb2[3] = {0.5,0.5,0};
    GLfloat Dif2[3] = {mKDR,mKDG,mKDB};
    GLfloat Spc2[3] = {mKSR,mKSG,mKSB};
    setUp_GL(modelViewMatrix,projectionMatrix,myView,Model2,Amb2,Dif2,Spc2,sTwoBufferID,sphere_2,shaderProgramID);

    R_tot = QQuaternion();

}

void scale_normals(float x, float y, float z, std::vector<glm::vec3> &normals){
    for(uint i =0; i<normals.size();i++){
        normals[i].x = normals[i].x * x;
        normals[i].y = normals[i].y * y;
        normals[i].z = normals[i].z * z;
    }
}

void GLDisplay::setUp_GL(glm::mat4 mVM, glm::mat4 pM, glm::mat4 mV,glm::mat4 MM,
                         GLfloat matA[3],GLfloat matD[3],GLfloat matS[3],
                         GLuint BufferID[3], Mesh mesh, GLuint pID){
                           
    glUniformMatrix4fv(
            glGetUniformLocation(pID, "modelViewMatrix"),
            1, false, glm::value_ptr(mVM));
    glUniformMatrix4fv(glGetUniformLocation(pID, "projectionMatrix"),
            1, false, glm::value_ptr(pM));
    glUniformMatrix4fv(glGetUniformLocation(pID, "viewMatrix"),
            1, false, glm::value_ptr(mV));
    glUniformMatrix4fv(glGetUniformLocation(pID, "Model"),
            1, false, glm::value_ptr(MM));

    // Transmit lighting parameters
    glUniform4f(glGetUniformLocation(pID, "ka"), matA[0], matA[1], matA[2], 1.0f);
    glUniform4f(glGetUniformLocation(pID, "kd"), matD[0], matD[1], matD[2], 1.0f);
    glUniform4f(glGetUniformLocation(pID, "ks"), matS[0], matS[1], matS[2], 1.0f);
    glUniform4f(glGetUniformLocation(pID, "lightPos_modelspace"), 0.0f, 0.0f, LZ,1.0f);
    glUniform1f(glGetUniformLocation(pID, "Shine"), shininess);

    // Bind attributes for position and normal
    GLuint positionAttribute = glGetAttribLocation(pID, "vertexPos_modelspace");
    glEnableVertexAttribArray(positionAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, BufferID[0]);
    glVertexAttribPointer(positionAttribute, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Bind normals to attributes in shader
    GLuint normals = glGetAttribLocation(pID, "Normal");
    glEnableVertexAttribArray(normals);
    glBindBuffer(GL_ARRAY_BUFFER, BufferID[1]);
    glVertexAttribPointer(normals, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);

    // Render triangles
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferID[2]);
    glDrawElements(GL_TRIANGLES, mesh.elements.size(), GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(positionAttribute);
}


void GLDisplay::setSize(float W, float H){
    W_H = H;
    W_W = W;

    this->setFixedSize(W_W,W_H);
}

void GLDisplay::setTranslate(double tx, double ty, double tz)
{
    // Set translation of object
    mTranslateX += tx;
    mTranslateY += ty;
    mTranslateZ += tz;
    // Trigger redraw in OpenGL
    updateGL();
}
/*
void GLDisplay::setPerspective(float N,float F,float FY){


}
*/

void GLDisplay::setRotate(float X,float Y,float Z){
    R_X = X;
    R_Y = Y;
    R_Z = Z;
    updateGL();
}

void GLDisplay::setCamera(float D,float T,float P){
    C_X = D * sin(T)*cos(P);
    C_Y = D * sin(P);
    C_Z = D * cos(T) * cos(P) ;
    updateGL();
}

void GLDisplay::setLightPosition(float X,float Y,float Z){
    LX = X;
    LY = Y;
    LZ = Z;
    updateGL();
}

void GLDisplay::setAmbientColor(GLfloat R, GLfloat G, GLfloat B){
    mKAR = R/10;
    mKAG = G/10;
    mKAB = B/10;
    updateGL();
}

void GLDisplay::setDiffuseColor(GLfloat R, GLfloat G, GLfloat B){
    mKDR  = R/10;
    mKDG  = G/10;
    mKDB  = B/10;
    updateGL();
}

void GLDisplay::setSpecularColor(GLfloat R, GLfloat G, GLfloat B){
    mKSR = R/10;
    mKSG = G/10;
    mKSB = B/10;
    updateGL();
}

void GLDisplay::setShine(GLfloat Shine){
    shininess = Shine;
    updateGL();
}

glm::vec3 get_Z(float x, float y, QPoint center){
    glm::vec3 Point;

    float p_x = (x - center.x())/250;
    float p_y = -((y - center.y())/250);

    float d, a;

        d = sqrt(p_x*p_x + p_y*p_y);
        float p_z = cos((M_PI/2.0) * ((d < 1.0) ? d : 1.0));
        a = 1.0 / sqrt(p_x*p_x + p_y*p_y + p_z*p_z);

        p_x *= a;
        p_y *= a;
        p_z *= a;

        Point =  glm::vec3(p_x,p_y,p_z);
        return normalize(Point);
}

QQuaternion GLDisplay::get_Quat(glm::vec3 u, glm::vec3 v){
    QQuaternion q;

    glm::vec3 w = cross(u, v);
    q = QQuaternion(1.f + dot(u, v), w.x, w.y, w.z);
    return q.normalized();
}

void GLDisplay::rotate_LR(QPoint pre, QPoint Cur){
    float dif = (Cur.x()-pre.x());
//    std::cout<<Cur.x()<<" "<<pre.x()<<std::endl;
    if(dif>0){ R_A=1;}
    else{ R_A=-1;}
//    std::cout<<R_A<<std::endl;
    new_look(R_A);
}

void GLDisplay::tilt_LR(QPoint pre, QPoint Cur){
    float dif = (Cur.x()-pre.x());
//    std::cout<<Cur.x()<<" "<<pre.x()<<std::endl;
    if(dif>0){ R_T=1;}
    else{ R_T=-1;}
//    std::cout<<R_A<<std::endl;
    new_tilt(R_T);
}


void GLDisplay::new_tilt(GLfloat R_T)
{
    float theta = R_T*0.0174532925;
    float cs = cos(theta);
    float sn = sin(theta);

    double PX = tilt.x * cs - tilt.y * sn;
    double PY = tilt.x * sn + tilt.y * cs;

//    std::cout<<PX<<" "<<PZ<<std::endl;

    tilt = glm::vec3(PX,PY,0);
}


void GLDisplay::new_look(GLfloat R_A){
    float theta = R_A*0.0174532925;
    float cs = cos(theta);
    float sn = sin(theta);

    double PX = lookObject.x * cs - lookObject.z * sn;
    double PZ = lookObject.x * sn + lookObject.z * cs;

//    std::cout<<PX<<" "<<PZ<<std::endl;

    lookObject = glm::vec3(PX,0,PZ);
}


void GLDisplay::mousePressEvent(QMouseEvent *event){
    //if (event->button()==Qt::LeftButton){
        lastPos = event->pos();
    //}
}


void GLDisplay::mouseMoveEvent(QMouseEvent *event){

    if (event->buttons() & Qt::LeftButton) {
        rotate_LR(lastPos,event->pos());
        lastPos = event->pos();
    }

    if(event->buttons() & Qt::RightButton){
       tilt_LR(lastPos,event->pos());
       lastPos = event->pos();
    }
    
    QGLWidget::mouseMoveEvent(event);

    updateGL();
}



void GLDisplay::tick(){
    o_x++;
    t_z+=0.5;
    updateGL();
}

void GLDisplay::reset(){
    C_X = 0,C_Y = 0,C_Z = 1.0;

    R_X = 0, R_Y = 0, R_Z = 0;

    LX = 5.0, LY = 0.0, LZ = -25.0;

//    lightColor = vec4(1.0f,1.0f,1.0f,1.0);

    shininess = 50;

    smoothShader = false;

    QFileDialog Open;
//    fileName = Open.getOpenFileName();

    lastPos = QPoint(0,0);

    R_tot = QQuaternion();
    Model = glm::mat4(1.0f);
    Model = translate(Model,vec3(0,0,-30));

    lookObject = glm::vec3(0.0,0.0,C_Z-100);
    tilt = glm::vec3(0.0,1.0,0.0);

    Model2 = glm::mat4(1.0f);

    //Add Timer
    o_x = 0; R_A = 0;t_z = -30;

}

void GLDisplay::w_pressed(){
    float dx = lookObject.x - C_X;
    float dz = lookObject.z - C_Z;

    bool negx = (dx < 0.0f);
    bool negz = (dz < 0.0f);


//    std::cout<<dx<<" "<<dz<<std::endl;

    if(fabs(dx)>fabs(dz)){
        if(negx&&!negz){
            dz = -(dz/dx);
            dx = -1;
        }
        else{
            dz = (dz/dx);
            dx = 1;
        }
    }
    else{
        if(negz&&!negx){
            dx = -(dx/dz);
            dz = -1;
        }
        else{
            dx = dx/dz;
            dz = 1;
        }
    }


    if(negx&&negz){dx = -dx; dz = -dz;}

    C_X = C_X + dx;
    C_Z = C_Z + dz;
    
//    std::cout<<dx<<" "<<dz<<std::endl<<std::endl;
}

void GLDisplay::s_pressed(){
    float dx = lookObject.x - C_X;
    float dz = lookObject.z - C_Z;

    bool negx = (dx < 0.0f);
    bool negz = (dz < 0.0f);

//    std::cout<<dx<<" "<<dz<<std::endl;

    if(fabs(dx)>fabs(dz)){

        if(negx&&!negz){
            dz = (dz/dx);
            dx = 1;
        }
        else{
            dz = -(dz/dx);
            dx = -1;
        }
    }

    else{
        if(negz&&!negx){
            dx = (dx/dz);
            dz = 1;
        }
        else{
            dx = -dx/dz;
            dz = -1;
        }
    }


    if(negx&&negz){dx = -dx; dz = -dz;}

    C_X = C_X + dx;
    C_Z = C_Z + dz;
//    std::cout<<dx<<" "<<dz<<std::endl<<std::endl;
}

void GLDisplay::d_pressed(){
    float dx = lookObject.x - C_X;
    float dz = lookObject.z - C_Z;
    float lx = lookObject.x;
    float lz = lookObject.z;

    bool negx = (dx < 0.0f);
    bool negz = (dz < 0.0f);


//    std::cout<<dx<<" "<<dz<<std::endl;

    if(fabs(dx)>fabs(dz)){
        if(negx&&negz){
            dx = (dz/dx);
            dz = -1;
        }
        else if(negx&&!negz){
            dx = (dz/dx);
            dz = -1;
        }
        else if(negx||negz){
            dx = -(dz/dx);
            dz = 1;
        }
        else{
            dx = -(dz/dx);
            dz = 1;
        }
    }

    else{
        if(negx&&!negz){
            dz = (dx/dz);
            dx = -1;
        }

        else if(negz||negx){
            dz = -(dx/dz);
            dx = 1;
        }
        else{
            dz = dx/dz;
            dx = -1;
        }
    }


//    if(negx&&negz){dx = -dx; dz = -dz;}

    C_X = C_X + dx;
    C_Z = C_Z + dz;
    lookObject = glm::vec3(lx+dx,0.0,lz+dz);
//    std::cout<<dx<<" "<<dz<<std::endl<<std::endl;

}


void GLDisplay::a_pressed(){
    float dx = lookObject.x - C_X;
    float dz = lookObject.z - C_Z;
    float lx = lookObject.x;
    float lz = lookObject.z;

    bool negx = (dx < 0.0f);
    bool negz = (dz < 0.0f);


//    std::cout<<dx<<" "<<dz<<std::endl;

    if(fabs(dx)>fabs(dz)){
        if(negx&&negz){
            dx = (dz/dx);
            dz = -1;
        }
        else if(negx&&!negz){
            dx = (dz/dx);
            dz = -1;
        }
        else if(negx||negz){
            dx = -(dz/dx);
            dz = 1;
        }
        else{
            dx = -(dz/dx);
            dz = 1;
        }
    }

    else{
        if(negx&&!negz){
            dz = (dx/dz);
            dx = -1;
        }

        else if(negz||negx){
            dz = -(dx/dz);
            dx = 1;
        }
        else{
            dz = dx/dz;
            dx = -1;
        }
    }
//    if(negx&&negz){dx = -dx; dz = -dz;}

    C_X = C_X - dx;
    C_Z = C_Z - dz;
    lookObject = glm::vec3(lx-dx,0.0,lz-dz);
//    std::cout<<dx<<" "<<dz<<std::endl<<std::endl;

}

void GLDisplay::f_pressed(){
    C_Y+=.5;
    updateGL();
}


void GLDisplay::v_pressed(){
    C_Y-=.5;
    updateGL();
}

