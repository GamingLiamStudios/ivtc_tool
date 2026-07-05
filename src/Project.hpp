#pragma once

#include <QString>
#include <QColorSpace>
#include <QImage>
#include <QJsonObject>

#include "ColorSpace.hpp"

#include <cstdint>

extern "C"
{
#include <libavformat/avformat.h>
}

class Project : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QImage currentFrame MEMBER m_currentFrame NOTIFY frameChanged)
    Q_PROPERTY(int64_t timestamp MEMBER m_timestamp)
    Q_PROPERTY(qreal pixelAspectRatio MEMBER m_pixelAspectRatio NOTIFY pixelAspectChanged)

public:
    Project(const QString &video_path);
    ~Project();

    static Project from_json(const QString &filepath);
    void           save();

    bool has_changed();

public slots:
    void stepFrame(int frames = 1);
    void setTimestamp(int64_t timestamp);

signals:
    void frameChanged(const QImage &frame);
    void pixelAspectChanged(const qreal pixelAspectRatio);

private:
    QColorSpace colorspace();

private:
    QString m_save_path;
    bool    m_changed = true;

    // Video Parameters
    const QString m_videoPath;
    Transfer      m_videoTransfer;
    Primaries     m_videoPrimaries;
    qreal         m_pixelAspectRatio = 1.0;

    AVFormatContext *m_ctx = nullptr;
    QImage           m_currentFrame;
    int64_t          m_timestamp;
};

Q_DECLARE_METATYPE(Project)
