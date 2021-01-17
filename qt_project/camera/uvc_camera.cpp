#include <QKeyEvent>
#include <QTimer>
#include <QMessageBox>

#include <QCamera>
#include <QCameraInfo>
#include <QMediaMetaData>

#include "uvc_camera.h"
#include "ui_uvc_camera.h"
#include "log/log.h"
#include "camera/video_settings.h"
#include "camera/image_settings.h"

Q_DECLARE_METATYPE(QCameraInfo)

const QString UVCCamera::TAG = "uvc";


UVCCamera::UVCCamera(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::UVCCamera),
    mCamera(0), mCapture(0), mMediaRecorder(0),
    isCapturingImage(false), isAppExiting(false)
{
    ui->setupUi(this);
    initComponentts();

    QActionGroup *camDevicesGrp = new QActionGroup(this);
    camDevicesGrp->setExclusive(true);

    foreach (const QCameraInfo &camInfo, QCameraInfo::availableCameras()) {
        QAction *camAction = new QAction(camInfo.description(), camDevicesGrp);
        camAction->setCheckable(true);
        camAction->setData(QVariant::fromValue(camInfo));
        if (camInfo == QCameraInfo::defaultCamera())
            camAction->setChecked(true);

        mMenuDevices->addAction(camAction);
    }

    connect(camDevicesGrp, SIGNAL(triggered(QAction*)), SLOT(updateCameraDevice(QAction*)));
    connect(mCapTabWidget, SIGNAL(currentChanged(int)), SLOT(updateCaptureMode()));
    const QCameraInfo &camDef = QCameraInfo::defaultCamera();
    Logger::info(TAG, "defaultCamera name: " + camDef.deviceName());
    Logger::info(TAG, "defaultCamera description: " + camDef.description());
    setCamera(QCameraInfo::defaultCamera());
}

UVCCamera::~UVCCamera()
{
    delete mMediaRecorder;
    delete mCapture;
    delete mCamera;
    delete ui;
}

void UVCCamera::initComponentts()
{
    mMenuDevices = ui->menuDevices;
    mMenuFile = ui->menuFile;

    mBtnFocus = ui->lockButton;
    mCapTabWidget = ui->captureWidget;
    mExpoComSlider = ui->exposureCompensation;
    mBtnRecord = ui->recordButton;
    mBtnPause = ui->pauseButton;
    mBtnStop = ui->stopButton;
    mBtnMute = ui->muteButton;

    mCameraViewfinder = ui->viewfinder;
}

void UVCCamera::setCamera(const QCameraInfo &cameraInfo)
{
    delete mCapture;
    delete mMediaRecorder;
    delete mCamera;

    mCamera = new QCamera(cameraInfo);

    connect(mCamera, SIGNAL(stateChanged(QCamera::State)), this, SLOT(updateCameraState(QCamera::State)));
    connect(mCamera, SIGNAL(error(QCamera::Error)), this, SLOT(dispCameraError()));

    mMediaRecorder = new QMediaRecorder(mCamera);
    connect(mMediaRecorder, SIGNAL(stateChanged(QMediaRecorder::State)), this, SLOT(updateRecorderState(QMediaRecorder::State)));

    mCapture = new QCameraImageCapture(mCamera);

    connect(mMediaRecorder, SIGNAL(durationChanged(qint64)), this, SLOT(updateRecordTime()));
    connect(mMediaRecorder, SIGNAL(error(QMediaRecorder::Error)), this, SLOT(dispRecordError()));

    mMediaRecorder->setMetaData(QMediaMetaData::Title, QVariant(QLatin1String("Test Title")));

    connect(mExpoComSlider, SIGNAL(valueChanged(int)), SLOT(setExposureCompensation(int)));

    mCamera->setViewfinder(mCameraViewfinder);

    updateCameraState(mCamera->state());
    updateLockStatus(mCamera->lockStatus(), QCamera::UserRequest);
    updateRecorderState(mMediaRecorder->state());

    connect(mCapture, SIGNAL(readyForCaptureChanged(bool)), this, SLOT(readyForCapture(bool)));
    connect(mCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(processCapturedImage(int,QImage)));
    connect(mCapture, SIGNAL(imageSaved(int,QString)), this, SLOT(imageSaved(int,QString)));
    connect(mCapture, SIGNAL(error(int,QCameraImageCapture::Error,QString)), this,
            SLOT(dispCaptureError(int,QCameraImageCapture::Error,QString)));

    connect(mCamera, SIGNAL(lockStatusChanged(QCamera::LockStatus,QCamera::LockChangeReason)),
            this, SLOT(updateLockStatus(QCamera::LockStatus,QCamera::LockChangeReason)));

    mCapTabWidget->setTabEnabled(0, (mCamera->isCaptureModeSupported(QCamera::CaptureStillImage)));
    mCapTabWidget->setTabEnabled(1, (mCamera->isCaptureModeSupported(QCamera::CaptureVideo)));

    updateCaptureMode();
    mCamera->start();
}

void UVCCamera::startCamera()
{
    mCamera->start();
}

void UVCCamera::stopCamera()
{
    mCamera->stop();
}

void UVCCamera::record()
{
    mMediaRecorder->record();
    updateRecordTime();
}

void UVCCamera::pause()
{
    mMediaRecorder->pause();
}

void UVCCamera::stop()
{
    mMediaRecorder->stop();
}

void UVCCamera::setMuted(bool muted)
{
    mMediaRecorder->setMuted(muted);
}

void UVCCamera::toggleLock()
{
    switch (mCamera->lockStatus()) {
    case QCamera::Searching:
    case QCamera::Locked:
        mCamera->unlock();
        break;
    case QCamera::Unlocked:
        mCamera->searchAndLock();
    }
}

void UVCCamera::takeImage()
{
    isCapturingImage = true;
    mCapture->capture();
}

void UVCCamera::dispCaptureError(int id, QCameraImageCapture::Error err, const QString &errString)
{
    Q_UNUSED(id);
    Q_UNUSED(err);
    QMessageBox::warning(this, tr("Image Capture Error"), errString);
    isCapturingImage = false;
}

void UVCCamera::configCaptureSettings()
{
    switch (mCamera->captureMode()) {
    case QCamera::CaptureStillImage:
        configImageSettings();
        break;
    case QCamera::CaptureVideo:
        configVideoSettings();
        break;
    default:
        break;
    }
}

void UVCCamera::configVideoSettings()
{
    VideoSettings settingsDialog(mMediaRecorder);
    settingsDialog.setAudioSettings(mAudioSettings);
    //settingsDialog.setVideoSettings(mVideoSettings);
    settingsDialog.setFormat(mVideoFormat);

    if (settingsDialog.exec()) {
        //mAudioSettings = settingsDialog.audioSettings();
        //mVideoSettings = settingsDialog.videoSettings();
        //mVideoFormat = settingsDialog.format();

        //mMediaRecorder->setEncodingSettings(mAudioSettings, mVideoSettings, mVideoFormat);
    }
}

void UVCCamera::configImageSettings()
{
    ImageSettings settingsDialog(mCapture);

    settingsDialog.setImageSettings(mImageSettings);

    if (settingsDialog.exec()) {
        mImageSettings = settingsDialog.imageSettings();
        mCapture->setEncodingSettings(mImageSettings);
    }
}

void UVCCamera::dispRecordError()
{
    QMessageBox::warning(this, tr("Capture error"), mMediaRecorder->errorString());
}

void UVCCamera::dispCameraError()
{
    QMessageBox::warning(this, tr("Camera error"), mCamera->errorString());
}

void UVCCamera::updateCameraDevice(QAction *act)
{
    setCamera(qvariant_cast<QCameraInfo>(act->data()));
}

void UVCCamera::updateCameraState(QCamera::State state)
{
    switch (state) {
    case QCamera::ActiveState:
        ui->actionStartCamera->setEnabled(false);
        ui->actionStopCamera->setEnabled(true);
        ui->captureWidget->setEnabled(true);
        ui->actionSettings->setEnabled(true);
        break;
    case QCamera::UnloadedState:
    case QCamera::LoadedState:
        ui->actionStartCamera->setEnabled(true);
        ui->actionStopCamera->setEnabled(false);
        ui->captureWidget->setEnabled(false);
        ui->actionSettings->setEnabled(false);
    }
}

void UVCCamera::updateCaptureMode()
{
    int tabIndex = mCapTabWidget->currentIndex();
    QCamera::CaptureModes captureMode = tabIndex == 0 ? QCamera::CaptureStillImage : QCamera::CaptureVideo;

    if (mCamera->isCaptureModeSupported(captureMode))
        mCamera->setCaptureMode(captureMode);
}

void UVCCamera::updateRecorderState(QMediaRecorder::State state)
{
    switch (state) {
    case QMediaRecorder::StoppedState:
        mBtnRecord->setEnabled(true);
        mBtnPause->setEnabled(true);
        mBtnStop->setEnabled(false);
        break;
    case QMediaRecorder::PausedState:
        mBtnRecord->setEnabled(true);
        mBtnPause->setEnabled(false);
        mBtnStop->setEnabled(true);
        break;
    case QMediaRecorder::RecordingState:
        mBtnRecord->setEnabled(false);
        mBtnPause->setEnabled(true);
        mBtnStop->setEnabled(true);
        break;
    }
}

void UVCCamera::setExposureCompensation(int index)
{
    mCamera->exposure()->setExposureCompensation(index*0.5);
}

void UVCCamera::updateRecordTime()
{
    QString s = QString("Record %l sec").arg(mMediaRecorder->duration()/1000);
    ui->statusbar->showMessage(s);
}

void UVCCamera::processCapturedImage(int reqId, const QImage &img)
{
    Q_UNUSED(reqId);

    QImage scaledImg = img.scaled(mCameraViewfinder->size(),
            Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->lastImagePreviewLabel->setPixmap(QPixmap::fromImage(scaledImg));

    // Display captured image for 4 seconds.
    dispCapturedImage();
    QTimer::singleShot(4000, this, SLOT(dispViewfinder()));
}

void UVCCamera::updateLockStatus(QCamera::LockStatus status, QCamera::LockChangeReason reason)
{
    QColor indicationColor = Qt::black;

    switch (status) {
    case QCamera::Searching:
        indicationColor = Qt::yellow;
        ui->statusbar->showMessage(tr("Focusing..."));
        mBtnFocus->setText(tr("Focusing..."));
        break;
    case QCamera::Locked:
        indicationColor = Qt::darkGreen;
        mBtnFocus->setText(tr("Unlock"));
        ui->statusbar->showMessage(tr("Focused"), 2000);
        break;
    case QCamera::Unlocked:
        indicationColor = reason == QCamera::LockFailed ? Qt::red : Qt::black;
        mBtnFocus->setText(tr("Focus"));
        if (reason == QCamera::LockFailed)
            ui->statusbar->showMessage(tr("Focus Failed"), 2000);
    }

    QPalette palette = ui->lockButton->palette();
    palette.setColor(QPalette::ButtonText, indicationColor);
    mBtnFocus->setPalette(palette);
}

void UVCCamera::dispViewfinder()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void UVCCamera::dispCapturedImage()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void UVCCamera::readyForCapture(bool ready)
{
    ui->takeImageButton->setEnabled(ready);
}

void UVCCamera::imageSaved(int id, const QString &fileName)
{
	Q_UNUSED(id);
    Q_UNUSED(fileName);

    isCapturingImage = false;
    if (isAppExiting)
        close();
}

void UVCCamera::keyPressEvent(QKeyEvent *e)
{
    if (e->isAutoRepeat())
        return;
    switch (e->key()) {
    case Qt::Key_CameraFocus:
        dispViewfinder();
        mCamera->searchAndLock();
        e->accept();
        break;
    case Qt::Key_Camera:
        if (mCamera->captureMode() == QCamera::CaptureStillImage) {
            takeImage();
        } else {
            if (mMediaRecorder->state() == QMediaRecorder::RecordingState)
                stop();
            else
                record();
        }
        e->accept();
        break;
    default:
        QMainWindow::keyPressEvent(e);
        break;
    }
}

void UVCCamera::keyReleaseEvent(QKeyEvent *e)
{
    if (e->isAutoRepeat())
        return;

    switch (e->key()) {
    case Qt::Key_CameraFocus:
        mCamera->unlock();
        break;
    default:
        QMainWindow::keyReleaseEvent(e);
    }
}

void UVCCamera::closeEvent(QCloseEvent *e)
{
	if (isCapturingImage) {
        setEnabled(false);
        isAppExiting = true;
        e->ignore();
    } else {
        e->accept();
    }
}

