#include <iostream>
#include <QtGui>
#include "GLDisplay.h"
#include <QGLFormat>
#include "ViewerMainWindow.h"

ViewerMainWindow::ViewerMainWindow()
{
    float Window_H = 500, Window_W = 500;
#if 0
    QGLFormat fmt;
    fmt.setProfile(QGLFormat::CoreProfile);
    fmt.setVersion(3,2);
    QGLFormat::setDefaultFormat(fmt);
#endif

    QWidget *central = new QWidget;
    setCentralWidget(central);
    QHBoxLayout *mainLayout = new QHBoxLayout();
    central->setLayout(mainLayout);
    mGLDisplay = new GLDisplay();
    mGLDisplay->setSize(Window_W, Window_H);
    mainLayout->addWidget(mGLDisplay, 1);
    QVBoxLayout *controlsLayout = new QVBoxLayout();
    mainLayout->addLayout(controlsLayout);



    //________RGB SLIDERS______________
     QLabel *RLabel = new QLabel("X");
     QLabel *GLabel = new QLabel("Y");
     QLabel *BLabel = new QLabel("Z");

    //Material sliders
    QGroupBox *lightingGroupBox = new QGroupBox("Light Position");
//    QGroupBox *lightingGroupBox = new QGroupBox("Material Diffuse");

    //        QlightingGroupBox *lightingGroupBox = new QlightingGroupBox(Label);
        controlsLayout->addWidget(lightingGroupBox, 0);
        controlsLayout->addStretch(2);
        QHBoxLayout *lightingGroupBoxLayout = new QHBoxLayout;
        lightingGroupBox->setLayout(lightingGroupBoxLayout);

//            QLabel *RLabel = new QLabel("X");
        lightingGroupBoxLayout->addWidget(RLabel);
        lRSlider = new QSlider(Qt::Horizontal);
        lRSlider->setMaximum(20);
        lRSlider->setMinimum(-20);
        lightingGroupBoxLayout->addWidget(lRSlider, 1);
    //        QObject::connect(lRSlider, SIGNAL(sliderMoved(int)), this, SLOT(Changed()));

//            QLabel *GLabel = new QLabel("Y");
        lightingGroupBoxLayout->addWidget(GLabel);
        lGSlider = new QSlider(Qt::Horizontal);
        lGSlider->setMaximum(20);
        lGSlider->setMinimum(-20);
        lightingGroupBoxLayout->addWidget(lGSlider, 1);
    //        QObject::connect(lGSlider, SIGNAL(sliderMoved(int)), this, SLOT(Changed()));

//            QLabel *BLabel = new QLabel("Z");
        lightingGroupBoxLayout->addWidget(BLabel);
        lBSlider = new QSlider(Qt::Horizontal);
        lBSlider->setMaximum(20);
        lBSlider->setMinimum(-20);
        lBSlider->setValue(10);
        lightingGroupBoxLayout->addWidget(lBSlider, 1);

        QObject::connect(lRSlider, SIGNAL(sliderMoved(int)), this, SLOT(lightingChanged()));
        QObject::connect(lGSlider, SIGNAL(sliderMoved(int)), this, SLOT(lightingChanged()));
        QObject::connect(lBSlider, SIGNAL(sliderMoved(int)), this, SLOT(lightingChanged()));


    //Material sliders
    QGroupBox *MaGroupBox = new QGroupBox("Material Ambient");

        controlsLayout->addWidget(MaGroupBox, 0);
        controlsLayout->addStretch(2);
        QHBoxLayout *MaGroupBoxLayout = new QHBoxLayout;
        MaGroupBox->setLayout(MaGroupBoxLayout);

        QLabel *aRLabel = new QLabel("RED");
        MaGroupBoxLayout->addWidget(aRLabel);
        aRSlider = new QSlider(Qt::Horizontal);
        aRSlider->setMaximum(10);
        aRSlider->setMinimum(0);
        MaGroupBoxLayout->addWidget(aRSlider, 1);


        QLabel *aGLabel = new QLabel("GREEN");
        MaGroupBoxLayout->addWidget(aGLabel);
        aGSlider = new QSlider(Qt::Horizontal);
        aGSlider->setMaximum(10);
        aGSlider->setMinimum(0);
        MaGroupBoxLayout->addWidget(aGSlider, 1);


        QLabel *aBLabel = new QLabel("BLUE");
        MaGroupBoxLayout->addWidget(aBLabel);
        aBSlider = new QSlider(Qt::Horizontal);
        aBSlider->setMaximum(10);
        aBSlider->setMinimum(0);
        aBSlider->setValue(10);
        MaGroupBoxLayout->addWidget(aBSlider, 1);

        QObject::connect(aRSlider, SIGNAL(sliderMoved(int)), this, SLOT(alightingChanged()));
        QObject::connect(aGSlider, SIGNAL(sliderMoved(int)), this, SLOT(alightingChanged()));
        QObject::connect(aBSlider, SIGNAL(sliderMoved(int)), this, SLOT(alightingChanged()));



    QGroupBox *MdGroupBox = new QGroupBox("Material Diffuse");

        controlsLayout->addWidget(MdGroupBox, 0);
        controlsLayout->addStretch(2);
        QHBoxLayout *MdGroupBoxLayout = new QHBoxLayout;
        MdGroupBox->setLayout(MdGroupBoxLayout);

        QLabel *dRLabel = new QLabel("RED");
        MdGroupBoxLayout->addWidget(dRLabel);
        dRSlider = new QSlider(Qt::Horizontal);
        dRSlider->setMaximum(10);
        dRSlider->setMinimum(0);
        MdGroupBoxLayout->addWidget(dRSlider, 1);


        QLabel *dGLabel = new QLabel("GREEN");
        MdGroupBoxLayout->addWidget(dGLabel);
        dGSlider = new QSlider(Qt::Horizontal);
        dGSlider->setMaximum(10);
        dGSlider->setMinimum(0);
        MdGroupBoxLayout->addWidget(dGSlider, 1);


        QLabel *dBLabel = new QLabel("BLUE");
        MdGroupBoxLayout->addWidget(dBLabel);
        dBSlider = new QSlider(Qt::Horizontal);
        dBSlider->setMaximum(10);
        dBSlider->setMinimum(0);
        MdGroupBoxLayout->addWidget(dBSlider, 1);

        QObject::connect(dRSlider, SIGNAL(sliderMoved(int)), this, SLOT(dlightingChanged()));
        QObject::connect(dGSlider, SIGNAL(sliderMoved(int)), this, SLOT(dlightingChanged()));
        QObject::connect(dBSlider, SIGNAL(sliderMoved(int)), this, SLOT(dlightingChanged()));

    QGroupBox *MsGroupBox = new QGroupBox("Material Specular");

        controlsLayout->addWidget(MsGroupBox, 0);
        controlsLayout->addStretch(2);
        QHBoxLayout *MsGroupBoxLayout = new QHBoxLayout;
        MsGroupBox->setLayout(MsGroupBoxLayout);

        QLabel *sRLabel = new QLabel("RED");
        MsGroupBoxLayout->addWidget(sRLabel);
        sRSlider = new QSlider(Qt::Horizontal);
        sRSlider->setMaximum(10);
        sRSlider->setMinimum(0);
        MsGroupBoxLayout->addWidget(sRSlider, 1);


        QLabel *sGLabel = new QLabel("GREEN");
        MsGroupBoxLayout->addWidget(sGLabel);
        sGSlider = new QSlider(Qt::Horizontal);
        sGSlider->setMaximum(10);
        sGSlider->setMinimum(0);
        MsGroupBoxLayout->addWidget(sGSlider, 1);
    //        QObject::connect(sGSlider, SIGNAL(sliderMoved(int)), this, SLOT(Changed()));

        QLabel *sBLabel = new QLabel("BLUE");
        MsGroupBoxLayout->addWidget(sBLabel);
        sBSlider = new QSlider(Qt::Horizontal);
        sBSlider->setMaximum(10);
        sBSlider->setMinimum(0);
        MsGroupBoxLayout->addWidget(sBSlider, 1);

        QObject::connect(sRSlider, SIGNAL(sliderMoved(int)), this, SLOT(slightingChanged()));
        QObject::connect(sGSlider, SIGNAL(sliderMoved(int)), this, SLOT(slightingChanged()));
        QObject::connect(sBSlider, SIGNAL(sliderMoved(int)), this, SLOT(slightingChanged()));



    //---------__CAMERA__-----

    QGroupBox *cameraGroupBox = new QGroupBox("Camera");
    controlsLayout->addWidget(cameraGroupBox, 0);
    controlsLayout->addStretch(1);
    QHBoxLayout *cameraGroupBoxLayout = new QHBoxLayout;
    cameraGroupBox->setLayout(cameraGroupBoxLayout);
    QLabel *cameraDLabel = new QLabel("Distance");
    cameraGroupBoxLayout->addWidget(cameraDLabel);
    mDcameraLineEdit = new QLineEdit;
    mDcameraLineEdit->setText("3.0");
    cameraGroupBoxLayout->addWidget(mDcameraLineEdit, 1);
    QObject::connect(mDcameraLineEdit, SIGNAL(returnPressed()), this, SLOT(cameraChanged()));
    QLabel *camerathetaLabel = new QLabel("Theta");
    cameraGroupBoxLayout->addWidget(camerathetaLabel);
    mThetacameraLineEdit = new QLineEdit;
    mThetacameraLineEdit->setText("0.0");
    cameraGroupBoxLayout->addWidget(mThetacameraLineEdit, 1);
    QObject::connect(mThetacameraLineEdit, SIGNAL(returnPressed()), this, SLOT(cameraChanged()));
    QLabel *cameraPhiLabel = new QLabel("Phi");
    cameraGroupBoxLayout->addWidget(cameraPhiLabel);
    mPhicameraLineEdit = new QLineEdit;
    mPhicameraLineEdit->setText("0.0");
    cameraGroupBoxLayout->addWidget(mPhicameraLineEdit, 1);
    QObject::connect(mPhicameraLineEdit, SIGNAL(returnPressed()), this, SLOT(cameraChanged()));



    button = new QPushButton("&Reset", this);
    controlsLayout->addWidget(button, 0);
    QObject::connect(button, SIGNAL(released()), this, SLOT(reset()));

}

void ViewerMainWindow::lightingModelChanged(int s)
{
    mGLDisplay->setUsePhongShader(s);
}

void ViewerMainWindow::shineChanged()
{
    mGLDisplay->setShine( mZTranslateLineEdit->text().toDouble());
}

void ViewerMainWindow::translateChanged()
{
    mGLDisplay->setTranslate(mXTranslateLineEdit->text().toDouble(), mYTranslateLineEdit->text().toDouble(),  mZTranslateLineEdit->text().toDouble());
}

void ViewerMainWindow::perspectiveChanged(){
    mGLDisplay->setPerspective(mNear->text().toDouble(), mFar->text().toDouble(),  mFovY->text().toDouble());
}

void ViewerMainWindow::rotateChanged(){
    mGLDisplay->setRotate(mXrotateSlider->value(), mYrotateSlider->value(), mZrotateSlider->value());
}

void ViewerMainWindow::cameraChanged(){
    mGLDisplay->setCamera(mDcameraLineEdit->text().toDouble(), mThetacameraLineEdit->text().toDouble(),  mPhicameraLineEdit->text().toDouble());
}

void ViewerMainWindow::lightingChanged(){
    mGLDisplay->setLightPosition(lRSlider->value(), lGSlider->value(), lBSlider->value());
}

void ViewerMainWindow::alightingChanged(){
    mGLDisplay->setAmbientColor(aRSlider->value(), aGSlider->value(), aBSlider->value());
//    qDebug()<<"Amb";
}

void ViewerMainWindow::dlightingChanged(){
    mGLDisplay->setDiffuseColor(dRSlider->value(), dGSlider->value(), dBSlider->value());
//    qDebug()<<"Diffuse";
}

void ViewerMainWindow::slightingChanged(){
    mGLDisplay->setSpecularColor(sRSlider->value(), sGSlider->value(), sBSlider->value());
//    qDebug()<<"Spec";
}

void ViewerMainWindow::reset(){
    mGLDisplay->reset();
}

void ViewerMainWindow::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_W){
       mGLDisplay->w_pressed();
    }

    else if(event->key() == Qt::Key_S){
       mGLDisplay->s_pressed();
    }

    else if(event->key() == Qt::Key_A){
       mGLDisplay->a_pressed();
    }
    else if(event->key() == Qt::Key_D){
       mGLDisplay->d_pressed();
    }
    else if(event->key() == Qt::Key_F){
       mGLDisplay->f_pressed();
    }
    else if(event->key() == Qt::Key_V){
       mGLDisplay->v_pressed();
    }




}

