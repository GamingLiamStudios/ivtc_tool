#pragma once

#include <QString>
#include <QColorSpace>
#include "ColorSpace.hpp"
#include <QJsonObject>

class Project
{
public:
    Project();
    ~Project();

    void read(QJsonObject &json);
    void save(QJsonObject &json);

    bool has_changed();
    void set_video_path(QString &path);

private:
    QColorSpace colorspace();

private:
    QString m_save_path;
    bool    m_changed = false;

    // Video Parameters
    QString   m_video_path;
    Transfer  m_video_transfer;
    Primaries m_video_primaries;
};