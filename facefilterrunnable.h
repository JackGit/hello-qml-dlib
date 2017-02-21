#ifndef FACEFILTERRUNNABLE_H
#define FACEFILTERRUNNABLE_H

#include <QVideoFilterRunnable>
#include <QVideoFrame>
#include <QVideoSurfaceFormat>

#include "facefilter.h"

class FaceFilterRunnable : public QVideoFilterRunnable {
private:
    FaceFilter *m_pfaceFilter;

public:
    FaceFilterRunnable(FaceFilter *faceFilter = 0);
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, QVideoFilterRunnable::RunFlags flags);
};

#endif // FACEFILTERRUNNABLE_H
