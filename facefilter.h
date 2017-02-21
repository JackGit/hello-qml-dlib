#ifndef FACEFILTER_H
#define FACEFILTER_H

#include <QObject>
#include <QImage>
#include <QVideoFilterRunnable>
#include <QVideoFrame>
#include <QAbstractVideoFilter>
#include <QAbstractVideoBuffer>
#include <QVariantList>
#include <QJsonArray>
#include <QJsonValue>
#include <QPoint>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <dlib/opencv.h>
#include <dlib/image_io.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

#include <iostream>

class FaceFilter : public QAbstractVideoFilter {

    Q_OBJECT
    Q_PROPERTY(QString predictorPath READ predictorPath WRITE setPredictorPath)
    Q_PROPERTY(float scale READ scale WRITE setScale)

signals:
    void finished(const QVariantList &list);
    void faceDetectStarted();
    void faceDetectStopped();

public:
    FaceFilter();

    QVideoFilterRunnable *createFilterRunnable();
    void detectFaces(QVideoFrame *input);

    QString predictorPath() const;
    void setPredictorPath(const QString &predictorPath);

    float scale() const;
    void setScale(float scale);

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE bool isActive();

private:
    bool m_started = false;
    int m_scale = false;
    QString m_predictorPath;
    dlib::frontal_face_detector m_detector;
    dlib::shape_predictor m_predictor;
    dlib::image_window win;
};

#endif // FACEFILTER_H
