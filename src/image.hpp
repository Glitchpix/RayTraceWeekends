#pragma once
// Disable strict warnings for this header from the Microsoft Visual C++
// compiler.
#include <algorithm>
#include <corecrt.h>
#include <string>
#ifdef _MSC_VER
#pragma warning(push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

#include <array>
#include <cstdlib>
#include <iostream>

class Image {
public:
  Image() = default;

  Image(const Image&) = delete;
  Image(Image&&) = delete;
  Image& operator=(const Image&) = delete;
  Image& operator=(Image&&) = delete;
  Image(const char* imageFilename) {
    // Loads image data from the specified file. If the RTW_IMAGES environment
    // variable is defined, looks only in that directory for the image file. If
    // the image was not found, searches for the specified image file first from
    // the current directory, then in the images/ subdirectory, then the
    // _parent's_ images/ subdirectory, and then _that_ parent, on so on, for
    // six levels up. If the image was not loaded successfully, width() and
    // height() will return 0.

    auto filename = std::string(imageFilename);
    size_t envLength = 0;
    constexpr size_t bufferSize = 256;
    std::array<char, bufferSize> buffer{};
    getenv_s(&envLength, buffer.data(), bufferSize, "RTW_IMAGES");

    std::string imageDir =
        std::string(buffer.data(), buffer.data() + envLength);

    // Hunt for the image file in some likely locations.
    if ((envLength != 0) && load(imageDir + "/" + imageFilename)) {
      return;
    }
    if (load(filename)) {
      return;
    }
    if (load("images/" + filename)) {
      return;
    }
    if (load("../images/" + filename)) {
      return;
    }
    if (load("../../images/" + filename)) {
      return;
    }
    if (load("../../../images/" + filename)) {
      return;
    }

    std::cerr << "ERROR: Could not load image file '" << imageFilename
              << "'.\n";
  }

  ~Image() {
    delete[] byteData;
    STBI_FREE(floatData);
  }

  bool load(const std::string& filename) {
    // Loads the linear (gamma=1) image data from the given file name. Returns
    // true if the load succeeded. The resulting data buffer contains the three
    // [0.0, 1.0] floating-point values for the first pixel (red, then green,
    // then blue). Pixels are contiguous, going left to right for the width of
    // the image, followed by the next row below, for the full height of the
    // image.

    auto n =
        bytesPerPixel; // Dummy out parameter: original components per pixel
    floatData = stbi_loadf(filename.c_str(), &mImageWidth, &mImageHeight, &n,
                           bytesPerPixel);
    if (floatData == nullptr) {
      return false;
    }

    mBytesPerScanline = mImageWidth * bytesPerPixel;
    convertToBytes();
    return true;
  }

  [[nodiscard]] int width() const {
    return (floatData == nullptr) ? 0 : mImageWidth;
  }
  [[nodiscard]] int height() const {
    return (floatData == nullptr) ? 0 : mImageHeight;
  }

  [[nodiscard]] float* pixelData(int x, int y) const {
    // Return the address of the three RGB bytes of the pixel at x,y. If there
    // is no image data, returns magenta.
    static float magenta[] = {1.0f, 0, 1.0f};
    if (byteData == nullptr) {
      return magenta;
    }

    x = clamp(x, 0, mImageWidth);
    y = clamp(y, 0, mImageHeight);

    return floatData + y * mBytesPerScanline + x * bytesPerPixel;
  }

private:
  static constexpr int maxIntegerColorValue = 255;
  static constexpr int bytesPerPixel = 3;
  float* floatData{nullptr};        // Linear floating point pixel data
  unsigned char* byteData{nullptr}; // Linear 8-bit pixel data
  int mImageWidth = 0;              // Loaded image width
  int mImageHeight = 0;             // Loaded image height
  int mBytesPerScanline = 0;

  static constexpr int clamp(int x, int low, int high) {
    return std::clamp(x, low, high);
  }

  static constexpr unsigned char floatToByte(float value) {
    if (value <= 0.0) {
      return 0;
    }
    if (1.0 <= value) {
      return maxIntegerColorValue;
    }
    return static_cast<unsigned char>(256.0 * value);
  }

  void convertToBytes() {
    // Convert the linear floating point pixel data to bytes, storing the
    // resulting byte data in the `bdata` member.

    const auto totalBytes =
        static_cast<size_t>(mImageWidth * mImageHeight * bytesPerPixel);
    byteData = new unsigned char[totalBytes];

    // Iterate through all pixel components, converting from [0.0, 1.0] float
    // values to unsigned [0, 255] byte values.

    auto* bptr = byteData;
    auto* fptr = floatData;
    for (size_t i = 0; i < totalBytes; i++, fptr++, bptr++) {
      *bptr = floatToByte(*fptr);
    }
  }
};