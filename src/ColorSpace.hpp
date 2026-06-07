#pragma once

#include <cstdint>
#include <QColor>
#include <QColorSpace>

enum class Transfer
{
    BT601,
    BT709,
    LINEAR,
    SRGB,
};

enum class Primaries
{
    // NTSC
    BT470_M,
    SMPTE_C,    // D65

    // PAL
    BT470_BG,    // D65
    BT470_6,

    BT709,
};

enum class Matrix
{
    Linear,
    BT601,
    BT709,
};

QColorSpace generateColorSpace(Primaries primaries, Transfer transfer);

struct YPbPr
{
    float Y;
    float Pb;
    float Pr;

public:
    explicit YPbPr(float Y, float Pb, float Pr) : Y(Y), Pb(Pb), Pr(Pr) { }
    QColor toQColor(Matrix matrix);

private:
    QColor toRGB(float Kr, float Kg, float Kb);
};

struct YCbCr
{
    uint16_t Y;
    uint16_t Cb;
    uint16_t Cr;

    int bits;

public:
    explicit YCbCr(uint16_t Y, uint16_t Cb, uint16_t Cr, int bits = 10)
        : Y(Y), Cb(Cb), Cr(Cr), bits(bits)
    {
    }
    QColor toQColor(Matrix matrix);
};
