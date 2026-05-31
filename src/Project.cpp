#include "Project.hpp"

#include <QFile>
#include <QException>

#include <cstdint>
#include <cmath>

template<std::size_t Length>
QList<uint16_t> create_rec601_transfer()
{
    QList<uint16_t> transfer;
    transfer.reserve(Length);

    double luma_scale = static_cast<double>(Length) - 1.0;

    for (int i = 0; i < Length; i++)
    {
        double luma = static_cast<double>(i) / luma_scale;

        double encoded = 0.0;
        if (luma < 0.018) { encoded = luma * 4.5; }
        else
        {
            encoded = 1.099 * std::pow(luma, 0.45) - 0.099;
        }

        transfer.append(static_cast<uint16_t>(encoded * 65535.0));
    }

    return transfer;
}

Project::Project()
{
}

Project::~Project()
{
}

void Project::read(QJsonObject &json)
{
    // TODO
}

bool Project::has_changed()
{ return this->m_changed; }

void Project::set_video_path(QString &path)
{
    this->m_changed    = true;
    this->m_video_path = path;
}

QColorSpace Project::colorspace()
{
    QColorSpace colorspace;

    switch (this->m_video_primaries)
    {
    case Primaries::BT470_M:
    case Primaries::BT470_6:
        colorspace.setPrimaries(
          QPointF(0.31, 0.316),
          QPointF(0.67, 0.33),
          QPointF(0.21, 0.71),
          QPointF(0.14, 0.08));
        break;
    case Primaries::SMPTE_C:
        colorspace.setPrimaries(
          QPointF(0.3127, 0.3290),
          QPointF(0.63, 0.34),
          QPointF(0.31, 0.595),
          QPointF(0.155, 0.07));
        break;
    case Primaries::BT470_BG:
        colorspace.setPrimaries(
          QPointF(0.3127, 0.3290),
          QPointF(0.64, 0.33),
          QPointF(0.29, 0.60),
          QPointF(0.15, 0.06));
        break;
    case Primaries::BT709:
        colorspace.setPrimaries(
          QPointF(0.3127, 0.3290),
          QPointF(0.640, 0.330),
          QPointF(0.300, 0.600),
          QPointF(0.150, 0.060));
        break;
    }

    switch (this->m_video_transfer)
    {
    case Transfer::BT601:
    case Transfer::BT709: colorspace.setTransferFunction(create_rec601_transfer<1024>()); break;
    case Transfer::LINEAR:
        colorspace.setTransferFunction(QColorSpace::TransferFunction::Linear);
        break;
    case Transfer::SRGB: colorspace.setTransferFunction(QColorSpace::TransferFunction::SRgb); break;
    }

    return colorspace;
}