#pragma once

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
