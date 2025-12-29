#include "rtweekend.hpp"
#include "HittableList.hpp"
#include "Sphere.hpp"
#include "BitMap.hpp"

Color ray_color(const Ray &r, const Hittable &world) {
    HitRecord rec;
    if (world.hit(r, Interval(0, INF), rec)) {
        return 0.5*(rec.normal + Color(1,1,1));
    }

    Vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
}

int main() {
    // Image
    constexpr auto ASPECT_RATIO = 16.0L / 9.0L;
    constexpr auto IMAGE_WIDTH = 400;
    // Calculate the image height, and ensure that it's at least 1.
    constexpr auto IMAGE_HEIGHT = std::max(1, int(IMAGE_WIDTH / ASPECT_RATIO));

    // World
    HittableList world;
    world.add(make_shared<Sphere>(Point3(0,0,-1), 0.5));
    world.add(make_shared<Sphere>(Point3(0,-100.5,-1), 100));


    // Camera
    constexpr auto FOCAL_LENGTH = 1.0L;
    constexpr auto VIEWPORT_HEIGHT = 2.0L;
    const auto VIEWPORT_WIDTH = VIEWPORT_HEIGHT * (double(IMAGE_WIDTH)/IMAGE_HEIGHT);
    const auto CAMERA_CENTER = Point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    const auto VIEWPORT_U = Vec3(VIEWPORT_WIDTH, 0, 0);
    const auto VIEWPORT_V = Vec3(0, -VIEWPORT_HEIGHT, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    const auto PIXEL_DELTA_U = VIEWPORT_U / IMAGE_WIDTH;
    const auto PIXEL_DELTA_V = VIEWPORT_V / IMAGE_HEIGHT;

    // Calculate the location of the upper left pixel.
    const auto VIEWPORT_UPPER_LEFT = CAMERA_CENTER
                             - Vec3(0, 0, FOCAL_LENGTH) - VIEWPORT_U/2 - VIEWPORT_V/2;
    const auto PIXEL00_LOC = VIEWPORT_UPPER_LEFT + 0.5 * (PIXEL_DELTA_U + PIXEL_DELTA_V);


    // Render
    // Create file in binary mode
    std::ofstream output_file_stream("image.bmp", std::ios::out | std::ios::binary);
    if (!output_file_stream.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }
    BitMap::prepare_file(output_file_stream, IMAGE_WIDTH, IMAGE_HEIGHT);

    for (auto j = IMAGE_HEIGHT; j > 0; j--) {
        std::clog << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (auto i = 0; i < IMAGE_WIDTH; i++) {
            auto pixel_center = PIXEL00_LOC + (i * PIXEL_DELTA_U) + (j * PIXEL_DELTA_V);
            auto ray_direction = pixel_center - CAMERA_CENTER;

            Ray r(CAMERA_CENTER, ray_direction);

            Color pixel_color = ray_color(r, world);

            BitMap::write_color(output_file_stream, pixel_color);
        }

        // Pad row size to 4 byte multiple
        BitMap::end_row(output_file_stream, IMAGE_WIDTH);
    }

    // Close the file
    output_file_stream.close();
    std::clog << "\rDone.                 \n";

    return 0;
}