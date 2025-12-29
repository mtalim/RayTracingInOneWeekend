#include "Vec3.hpp"
#include "BitMap.hpp"

#include <fstream>
#include <iostream>

void renderBmpFile(std::ofstream &bmp_file_stream, unsigned w, unsigned h) {
    const unsigned W_BYTES = w * 3;
    const unsigned W_PAD = BitMap::calc_row_padding(W_BYTES);

    BitMap::FileHeader file_header = {
        'B', 'M',
        (sizeof(BitMap::FileHeader) + sizeof(BitMap::InfoHeader) + ((W_BYTES + W_PAD) * h)),
        0, 0,
        (sizeof(BitMap::FileHeader) + sizeof(BitMap::InfoHeader))
    };
    BitMap::InfoHeader info_header = {
        sizeof(BitMap::InfoHeader),
        w, h,
        1,
        24,
        0,
        ((W_BYTES + W_PAD) * h),
        0, 0, 0, 0
    };

    BitMap::write_file_header(bmp_file_stream, file_header);
    BitMap::write_info_header(bmp_file_stream, info_header);

    Color pix_data;
    pix_data[2] = 0.0L;
    for (auto j = h; j > 0; j--) {
        std::clog << "\rScanlines remaining: " << j << ' ' << std::flush;

        pix_data[1] = double(j-1) / (h-1);

        for (auto i = 0; i < w; i++) {
            pix_data[0] = double(i) / (w-1);

            BitMap::write_color(bmp_file_stream, pix_data);
        }

        // Pad row size to 4 byte multiple
        BitMap::end_row(bmp_file_stream, W_PAD);
    }
}

int main() {
    // Image
    constexpr auto IMAGE_WIDTH = 256;
    constexpr auto IMAGE_HEIGHT = 256;

    // Render
    // Create file in binary mode
    std::ofstream output_file_stream("image.bmp", std::ios::out | std::ios::binary);
    if (!output_file_stream.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }


    renderBmpFile(output_file_stream, IMAGE_WIDTH, IMAGE_HEIGHT);

    // Close the file
    output_file_stream.close();
    std::clog << "\rDone.                 \n";

    return 0;
}