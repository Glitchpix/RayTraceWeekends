#include <iostream>
int main() {
  constexpr int imageWidth = 256;
  constexpr int imageHeight = 256;

  constexpr double maxColorValue = 255.0;

  std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

  for (int yIndex = 0; yIndex < imageHeight; ++yIndex) {
    for (int xIndex = 0; xIndex < imageWidth; ++xIndex) {
      auto r = double(xIndex) / (imageWidth - 1);
      auto g = double(yIndex) / (imageHeight - 1);
      auto b = 0.0;

      int ir = static_cast<int>(round(maxColorValue * r));
      int ig = static_cast<int>(round(maxColorValue * g));
      int ib = static_cast<int>(round(maxColorValue * b));

      std::cout << ir << ' ' << ig << ' ' << ib << '\n';
    }
  }
}