#ifndef CAMERA_H
#define CAMERA_H

#include "Hittable.hpp"
#include "Material.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"
#include "Interval.hpp"
#include "BitMap.hpp"

class Camera {
private:
    unsigned image_height; // Rendered image height
    double pixel_samples_scale; // Color scale factor for a sum of pixel samples
    Point3 center; // Camera center
    Point3 pixel00_loc; // Location of pixel 0, 0
    Vec3 pixel_delta_u; // Offset to pixel to the right
    Vec3 pixel_delta_v; // Offset to pixel below

    void initialize() {
        // Calculate the image height, and ensure that it's at least 1.
        image_height = std::max(1, int(image_width / aspect_ratio));

        pixel_samples_scale = 1.0L / samples_per_pixel;

        center = Point3(0, 0, 0);

        // Determine viewport dimensions.
        constexpr auto FOCAL_LENGTH = 1.0L;
        constexpr auto VIEWPORT_HEIGHT = 2.0L;
        const auto viewport_width = VIEWPORT_HEIGHT * (double(image_width)/image_height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        const auto viewport_u = Vec3(viewport_width, 0, 0);
        const auto viewport_v = Vec3(0, -VIEWPORT_HEIGHT, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        const auto viewport_upper_left =
            center - Vec3(0, 0, FOCAL_LENGTH) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    Ray get_ray(int i, int j) const {
        // Construct a camera ray originating from the origin and directed at randomly sampled
        // point around the pixel location i, j.

        auto offset = sample_square();
        auto pixel_sample = pixel00_loc
                          + ((i + offset.x()) * pixel_delta_u)
                          + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;

        return Ray(ray_origin, ray_direction);
    }

    Vec3 sample_square() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return Vec3(random_double(-0.5L, 0.5L), random_double(-0.5L, 0.5L), 0);
    }

    Color ray_color(const Ray &r, unsigned depth, const Hittable &world) const {
        HitRecord rec;
        if (world.hit(r, Interval(0.001, INF), rec)) {
            Ray scattered;
            Color attenuation;
            // If we've exceeded the ray bounce limit, no more light is gathered.
            // And if not scattered, then fully absorbed by material.
            return depth && rec.mat->scatter(r, rec, attenuation, scattered) ?
                    attenuation * ray_color(scattered, depth-1, world) : Color(0,0,0);
        }

        // Sky default
        Vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
    }

public:
    double aspect_ratio = 1.0L; // Ratio of image width over height
    unsigned image_width = 100; // Rendered image width in pixel count
    unsigned samples_per_pixel = 10; // Count of random samples for each pixel
    unsigned max_depth = 10; // Maximum number of ray bounces into scene

    void render(const Hittable &world) {
        initialize();

        // Create file in binary mode
        std::ofstream output_file_stream("image.bmp", std::ios::out | std::ios::binary);
        if (!output_file_stream.is_open()) {
            std::cerr << "Error opening file!" << std::endl;
            return;
        }
        BitMap::prepare_file(output_file_stream, image_width, image_height);

        for (auto j = image_height; j > 0;) {
            std::clog << "\rScanlines remaining: " << j-- << ' ' << std::flush;
            for (auto i = 0; i < image_width; i++) {
                auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                auto ray_direction = pixel_center - center;


                Color pixel_color(0,0,0);
                for (auto sample = 0; sample < samples_per_pixel; sample++) {
                    Ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }

                BitMap::write_color(output_file_stream, pixel_samples_scale * pixel_color);
            }

            // Pad row size to 4 byte multiple
            BitMap::end_row(output_file_stream, image_width);
        }

        // Close the file
        output_file_stream.close();
        std::clog << "\rDone.                 \n";
    }
};

#endif