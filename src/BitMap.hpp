#ifndef BITMAP_H
#define BITMAP_H

#include "Vec3.hpp"

namespace BitMap {
    unsigned padding;

    struct __attribute__((packed)) FileHeader {
        unsigned header0 : 8;
        unsigned header1 : 8;
        unsigned size : 32;
        unsigned reserved0 : 16;
        unsigned reserved1 : 16;
        unsigned offset : 32;
    };

    struct __attribute__((packed)) InfoHeader {
        unsigned size : 32; // Of this header.
        unsigned width : 32; // Image width in pixels.
        unsigned height : 32; // Image height in pixels.
        unsigned planes : 16; // Must be 1.
        unsigned bitCount : 16; // Bits per pixel (e.g., 1, 8, 24).
        unsigned compression : 32; // Compression type (0 for BI_RGB, none).
        unsigned sizeImage : 32; // Size of raw pixel data (optional).
        unsigned xPixelsPerMeter : 32; // Resolution (optional).
        unsigned YPixelsPerMeter : 32; // Resolution (optional).
        unsigned clrUsed : 32; // Color palette info (optional).
        unsigned ClrImportant : 32; // Color palette info (optional).
    };

    struct __attribute__((packed)) PixelData {
        unsigned b : 8;
        unsigned g : 8;
        unsigned r : 8;
    };

    unsigned calc_row_padding(unsigned row_bytes) {
        return (4 - (row_bytes % 4)) % 4; // Pad to 4 byte multiple
    }

    void write_file_header(std::ostream &out, const FileHeader &header) {
        out.write(reinterpret_cast<const char*>(&header), sizeof(BitMap::FileHeader));
    }

    void write_info_header(std::ostream &out, const InfoHeader &header) {
        out.write(reinterpret_cast<const char*>(&header), sizeof(BitMap::InfoHeader));
    }

    void prepare_file(std::ostream &bmp_file_stream, unsigned w, unsigned h) {
        const unsigned W_BYTES = w * 3;
        BitMap::padding = BitMap::calc_row_padding(W_BYTES);

        BitMap::FileHeader file_header = {
            'B', 'M',
            (sizeof(BitMap::FileHeader) + sizeof(BitMap::InfoHeader) + ((W_BYTES + BitMap::padding) * h)),
            0, 0,
            (sizeof(BitMap::FileHeader) + sizeof(BitMap::InfoHeader))
        };
        BitMap::InfoHeader info_header = {
            sizeof(BitMap::InfoHeader),
            w, h,
            1,
            24,
            0,
            ((W_BYTES + BitMap::padding) * h),
            0, 0, 0, 0
        };

        BitMap::write_file_header(bmp_file_stream, file_header);
        BitMap::write_info_header(bmp_file_stream, info_header);
    }

    void write_color(std::ostream &out, const Color &pixel_color) {
        // Translate the [0,1] component values to the byte range [0,255].
        auto d_to_i = [](double d) { return unsigned(255.999 * d);};

        PixelData p_data;
        p_data.r = d_to_i(pixel_color.x());
        p_data.g = d_to_i(pixel_color.y());
        p_data.b = d_to_i(pixel_color.z());


        // Write out the pixel color components.
        out.write(reinterpret_cast<const char*>(&p_data), sizeof(BitMap::PixelData));
    }

    void end_row(std::ostream &out, unsigned w) {
        out.seekp(BitMap::padding, std::ios::cur);
    }
}

#endif