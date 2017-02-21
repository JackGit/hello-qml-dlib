#include "facefilterrunnable.h"

FaceFilterRunnable::FaceFilterRunnable(FaceFilter *faceFilter) {
    m_pfaceFilter = faceFilter;
}

QVideoFrame FaceFilterRunnable::run (
        QVideoFrame *input,
        const QVideoSurfaceFormat &surfaceFormat,
        QVideoFilterRunnable::RunFlags flags
) {
    m_pfaceFilter->detectFaces(input);
    return *input;
}
