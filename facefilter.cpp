#include "facefilter.h"
#include "facefilterrunnable.h"

FaceFilter::FaceFilter() {
    m_detector = dlib::get_frontal_face_detector();
    m_scale = 2;
}

void FaceFilter::start() {
    m_started = true;
    emit faceDetectStarted();
}

void FaceFilter::stop() {
    m_started = false;
    emit faceDetectStopped();
}

bool FaceFilter::isActive() {
    return m_started;
}

QString FaceFilter::predictorPath() const {
    return m_predictorPath;
}

void FaceFilter::setPredictorPath(const QString &path) {
    m_started = false;
    m_predictorPath = path;
    dlib::deserialize(m_predictorPath.toStdString()) >> m_predictor;
    m_started = true;
}

float FaceFilter::scale() const {
    return m_scale;
}

void FaceFilter::setScale(float scale) {
    m_scale = scale;
}

QVideoFilterRunnable *FaceFilter::createFilterRunnable() {
    return new FaceFilterRunnable(this);
}



void FaceFilter::detectFaces(QVideoFrame *input) {
    if (!m_started) {
        return;
    }

    // if predictor is not ready
    input->map(QAbstractVideoBuffer::ReadWrite);

    // conver QVideoframe to Mat
    QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(input->pixelFormat());
    QImage qImage(input->bits(), input->width(), input->height(), input->bytesPerLine(), imageFormat);
    QImage rgbImage = qImage.convertToFormat(QImage::Format_RGB888);

    cv::Mat frameMat(rgbImage.height(), rgbImage.width(), CV_8UC3, rgbImage.bits(), rgbImage.bytesPerLine());

    // resize Mat
    cv::Mat smallFrameMat(rgbImage.height() / m_scale, rgbImage.width() / m_scale, CV_8UC1);
    cv::resize(frameMat, smallFrameMat, smallFrameMat.size(), 0, 0, cv::INTER_LINEAR);

    // vertical flip
    // for unknow reason, after conversion, image got vertical flipped, so here we flip it back
    cv::Mat flippedMat;
    cv::flip(smallFrameMat, flippedMat, 0);

    // set color
    cv::cvtColor(flippedMat, flippedMat, CV_BGR2RGB);

    cv::imshow("cv", flippedMat);

    // convert mat to dlib image
    dlib::cv_image<dlib::bgr_pixel> cimg(flippedMat);

    // detect face
    std::vector<dlib::rectangle> faces = m_detector(cimg);
    std::vector<dlib::full_object_detection> shapes;
    dlib::full_object_detection detectedObject;

    // points of all faces, which is the multiple of 68
    QVariantList pointList;

    for (unsigned long i = 0; i < faces.size(); ++i) {
        detectedObject = m_predictor(cimg, faces[i]);
        shapes.push_back(detectedObject);

        for (unsigned long j = 0; j < detectedObject.num_parts(); ++j) {
            dlib::point p = detectedObject.part(j);
            pointList.append(QPoint(p.x() * m_scale, p.y() * m_scale));
        }
    }

    win.clear_overlay();
    win.set_image(cimg);
    win.add_overlay(dlib::render_face_detections(shapes));

    input->unmap();

    // emit signal with result
    emit finished(pointList);
}
