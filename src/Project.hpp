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

    Q_PROPERTY(QImage currentFrame MEMBER m_current_frame NOTIFY frameChanged)
    Q_PROPERTY(int64_t timestamp MEMBER m_timestamp)

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

private:
    QColorSpace colorspace();

private:
    QString m_save_path;
    bool    m_changed = true;

    // Video Parameters
    const QString m_video_path;
    Transfer      m_video_transfer;
    Primaries     m_video_primaries;

    AVFormatContext *m_ctx = nullptr;
    QImage           m_current_frame;
    int64_t          m_timestamp;
};

Q_DECLARE_METATYPE(Project)
