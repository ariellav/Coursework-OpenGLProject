#include <QtGui>

class GLDisplay;
class QLineEdit;

class ViewerMainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        ViewerMainWindow();
        void SliderGroup( QGroupBox *GroupBox, QSlider *RSlider, QSlider *GSlider, QSlider *BSlider, QVBoxLayout *cLayout);
        void keyPressEvent(QKeyEvent *event);

    private:
        GLDisplay *mGLDisplay;
        QLineEdit *mXTranslateLineEdit;
        QLineEdit *mYTranslateLineEdit;
        QLineEdit *mZTranslateLineEdit;

        QLineEdit *mNear;
        QLineEdit *mFar;
        QLineEdit *mFovY;

        QSlider *mXrotateSlider;
        QSlider *mYrotateSlider;
        QSlider *mZrotateSlider;

        QSlider *aRSlider;
        QSlider *aGSlider;
        QSlider *aBSlider;

        QSlider *dRSlider;
        QSlider *dGSlider;
        QSlider *dBSlider;

        QSlider *sRSlider;
        QSlider *sGSlider;
        QSlider *sBSlider;

        QSlider *lRSlider;
        QSlider *lGSlider;
        QSlider *lBSlider;



        QLineEdit *mDcameraLineEdit;
        QLineEdit *mThetacameraLineEdit;
        QLineEdit *mPhicameraLineEdit;

        QPushButton *button;


    public slots:
        void translateChanged();
        void perspectiveChanged();
        void rotateChanged();
        void cameraChanged();
        void lightingModelChanged(int s);
        void lightingChanged();
        void alightingChanged();
        void dlightingChanged();
        void slightingChanged();
        void shineChanged();
        void reset();

};

