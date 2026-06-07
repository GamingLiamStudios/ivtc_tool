#include "ColorSpace.hpp"

#include <QPointF>
#include <QtAssert>
#include <QMatrix3x3>

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

QColorSpace generateColorSpace(Primaries primaries, Transfer transfer)
{
    QColorSpace colorspace;

    switch (primaries)
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

    switch (transfer)
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

QColor YCbCr::toQColor(Matrix matrix)
{
    Q_ASSERT(this->bits >= 8);
    const double SHIFT_CONSTANT = static_cast<float>(1 << (bits - 8));

    // BT601 & BT709 both define it as this
    float Y  = (static_cast<float>(this->Y) / SHIFT_CONSTANT - 16.0) / 219.0;
    float Pb = (static_cast<float>(this->Cb) / SHIFT_CONSTANT - 128.0) / 224.0;
    float Pr = (static_cast<float>(this->Cr) / SHIFT_CONSTANT - 128.0) / 224.0;

    return YPbPr(Y, Pb, Pr).toQColor(matrix);
}

QColor YPbPr::toQColor(Matrix matrix)
{
    switch (matrix)
    {
    case Matrix::Linear: return QColor::fromRgbF(Y, Pb, Pr);
    case Matrix::BT601: return toRGB(0.299, 0.587, 0.114);
    case Matrix::BT709: return toRGB(0.2126, 0.7152, 0.0722);
    }
}

QColor YPbPr::toRGB(float Kr, float Kg, float Kb)
{
    return QColor::fromRgbF(
      Y + 0.0 * Pb + (2.0 - 2.0 * Kr) * Pr,
      Y - Kb / Kg * (2.0 - 2.0 * Kb) * Pb - Kr / Kg * (2.0 - 2.0 * Kr) * Pr,
      Y + (2.0 - 2.0 * Kb) * Pb + 0.0 * Pr);
}
