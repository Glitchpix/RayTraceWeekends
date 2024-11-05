#include "color.hpp"
#include <iostream>

int main() {
  constexpr int imageWidth = 256;
  constexpr int imageHeight = 256;

  std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

  for (int yIndex = 0; yIndex < imageHeight; ++yIndex) {
    std::clog << "\rScanlines remaining: " << (imageHeight - yIndex) << ' '
              << std::flush;
    for (int xIndex = 0; xIndex < imageWidth; ++xIndex) {
      auto r = double(xIndex) / (imageWidth - 1);
      auto g = double(yIndex) / (imageHeight - 1);
      auto b = 0.0;

      auto pixelColor = Color(r, g, b);
      color::write(std::cout, pixelColor);
    }
  }

  std::clog << "\n\rDone.\n";
}