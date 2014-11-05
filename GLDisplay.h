#include "glheader.h"
#include <QGLWidget>
#include <QString>
#include "readObj.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <QQuaternion>
#include <QTimer>


class GLDisplay : public QGLWidget
{
    Q_OBJECT

    public:
        GLDisplay(QWidget * = 0);

        // Set the displayed objects translation
        void setTranslate(double tx, double ty, double tz);
        void setSize(float, float);
        void setPerspective(float,float,float);
        void setRotate(float,float,float);
        void setCamera(float,float,float);
//        void setLightColor(float,float,float);
        void setAmbientColor(float,float,float);
        void setDiffuseColor(float,float,float);
        void setSpecularColor(float,float,float);
        void setLightPosition(float X,float Y,float Z);

        void setShine(GLfloat Shine);

        void setUp_GL(glm::mat4 mVM, glm::mat4 pM, glm::mat4 mV,glm::mat4 MM,
                                 GLfloat matA[3],GLfloat matD[3],GLfloat matS[3],
                                 GLuint BufferID[3], Mesh mesh, GLuint pID);

        void init_buffers(GLuint BID[3], Mesh mesh);

        void reset();

        void setUsePhongShader(bool usePhong) { smoothShader = usePhong; updateGL(); }
        QQuaternion get_Quat(glm::vec3 u, glm::vec3 v);
        void rotate_LR(QPoint pre, QPoint Cur);
        void new_look(GLfloat R_A);
        void tilt_LR(QPoint pre, QPoint Cur);
        void new_tilt(GLfloat R_T);

        void w_pressed();
         void s_pressed();
         void a_pressed();
         void d_pressed();
         void f_pressed();
         void v_pressed();


    protected:
        virtual void initializeGL();
        virtual void resizeGL(int w, int h);
        virtual void paintGL();
        void mouseMoveEvent(QMouseEvent *);
        void mousePressEvent(QMouseEvent *event);


    private:
        float W_H, W_W;
        GLfloat mKAR,mKAG,mKAB;
        GLfloat mKDR,mKDG,mKDB;
        GLfloat mKSR,mKSG,mKSB;

        float C_X,C_Y,C_Z;

        float t_z;

        float R_X,R_Y,R_Z;

        GLfloat LX,LY,LZ;

        GLuint mVertexArrayObjectID;
        GLuint sOneBufferID[3];
        GLuint sTwoBufferID[3];


        GLuint texturePID;
        GLuint mPhongShaderProgramID;
//        GLuint mShaderProgramID;

        GLuint shininess;

        double mTranslateX;
        double mTranslateY;
        double mTranslateZ;
        Mesh sphere_1;
        Mesh sphere_2;

        bool smoothShader;


        QString fileName;

        QQuaternion R_tot;


        QPoint lastPos;
        glm::mat4 Model;
        glm::mat4 Model2;

        GLfloat o_x;
        GLfloat R_A;
        GLfloat R_T;
        glm::vec3 lookObject;
        glm::vec3 tilt;

        QTimer *timer;

        //QTimer timer;

    public slots:
        void tick();

};
