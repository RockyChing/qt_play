#ifndef UVC_CAMERA_H
#define UVC_CAMERA_H

#include <QPushButton>
#include <QSlider>
#include <QMenu>
#include <QCamera>
#include <QCameraImageCapture>
#include <QCameraViewfinder>
#include <QCameraCaptureBufferFormatControl>
#include <QMediaRecorder>

#include <QMainWindow>

namespace Ui {
class UVCCamera;
}

class UVCCamera : public QMainWindow
{
    Q_OBJECT

public:
    explicit UVCCamera(QWidget *parent = 0);
    ~UVCCamera();

    static const QString TAG;
private:
    void initComponentts();

private slots:
    void setCamera(const QCameraInfo &cameraInfo);

    void startCamera();
    void stopCamera();

    void record();
    void pause();
    void stop();
    void setMuted(bool muted);

    void toggleLock();
    void takeImage();
    void dispCaptureError(int id, QCameraImageCapture::Error err, const QString &errString);

    void configCaptureSettings();
    void configVideoSettings();
    void configImageSettings();

    void dispRecordError();
    void dispCameraError();

    void updateCameraDevice(QAction *act);

    void updateCameraState(QCamera::State state);
    void updateCaptureMode();
    void updateRecorderState(QMediaRecorder::State state);
    void setExposureCompensation(int index);

    void updateRecordTime();

    void processCapturedImage(int reqId, const QImage &img);
    void updateLockStatus(QCamera::LockStatus status, QCamera::LockChangeReason reason);

    void dispViewfinder();
    void dispCapturedImage();

    void readyForCapture(bool ready);
    void imageSaved(int id, const QString &fileName);

protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void closeEvent(QCloseEvent *e);

private:
    Ui::UVCCamera *ui;

    QMenu *mMenuDevices;
    QMenu *mMenuFile;

    QPushButton *mBtnFocus;
    QTabWidget *mCapTabWidget;
    QSlider *mExpoComSlider;
    QPushButton *mBtnRecord;
    QPushButton *mBtnPause;
    QPushButton *mBtnStop;
    QPushButton *mBtnMute;

    QCameraViewfinder *mCameraViewfinder;



private:
    QCamera *mCamera;
    QCameraImageCapture *mCapture;
    QMediaRecorder *mMediaRecorder;

    QImageEncoderSettings mImageSettings;
    QAudioEncoderSettings mAudioSettings;
    QVideoEncoderSettings mVideoSettings;

    QString mVideoFormat;
    bool isCapturingImage;
    bool isAppExiting;
};

#endif // UVC_CAMERA_H
