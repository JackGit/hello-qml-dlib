QVideoFilterRunnable *FaceRecogFilter::createFilterRunnable()
{
    return new FaceRecogFilterRunnable(this);
}

QVideoFrame FaceRecogFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
{
    // Convert the input into a suitable OpenCV image format, then run e.g. cv::CascadeClassifier,
    // and finally store the list of rectangles into a QObject exposing a 'rects' property.
    ...
    emit m_filter->finished(result);
    return *input;
}
