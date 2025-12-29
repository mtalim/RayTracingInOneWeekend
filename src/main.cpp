#include "Vec3.hpp"
#include "BitMap.hpp"

#include <fstream>
#include <iostream>

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
    BitMap::prepare_file(output_file_stream, IMAGE_WIDTH, IMAGE_HEIGHT);

    Color pix_data;
    pix_data[2] = 0.0L;
    for (auto j = IMAGE_HEIGHT; j > 0; j--) {
        std::clog << "\rScanlines remaining: " << j << ' ' << std::flush;

        pix_data[1] = double(j-1) / (IMAGE_HEIGHT-1);

        for (auto i = 0; i < IMAGE_WIDTH; i++) {
            pix_data[0] = double(i) / (IMAGE_WIDTH-1);

            BitMap::write_color(output_file_stream, pix_data);
        }

        // Pad row size to 4 byte multiple
        BitMap::end_row(output_file_stream, IMAGE_WIDTH);
    }

    // Close the file
    output_file_stream.close();
    std::clog << "\rDone.                 \n";

    return 0;
}