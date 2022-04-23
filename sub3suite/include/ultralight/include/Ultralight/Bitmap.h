///
/// @file Bitmap.h
///
/// @brief The header for the Bitmap class.
///
/// @author
///
/// This file is a part of Ultralight, a fast, lightweight, HTML UI engine
///
/// Website: <http://ultralig.ht>
///
/// Copyright (C) 2020 Ultralight, Inc. All rights reserved.
///
#pragma once
#include <Ultralight/Defines.h>
#include <Ultralight/RefPtr.h>
#include <Ultralight/Geometry.h>

namespace ultralight {

#pragma pack(push, 1)

///
/// The various Bitmap formats.
///
enum UExport BitmapFormat {
  /**
   * Alpha channel only, 8-bits per pixel.
   *
   * Encoding: 8-bits per channel, unsigned normalized.
   *
   * Color-space: Linear (no gamma), alpha-coverage only.
   */
  kBitmapFormat_A8_UNORM,

  /**
   * Blue Green Red Alpha channels, 32-bits per pixel.
   *
   * Encoding: 8-bits per channel, unsigned normalized.
   *
   * Color-space: sRGB gamma with premultiplied linear alpha channel.
   *
   * NOTE: Alpha is premultiplied with BGR channels _before_ sRGB gamma is
   *       applied so we can use sRGB conversion hardware and perform all
   *       blending in linear space on GPU.
   */
  kBitmapFormat_BGRA8_UNORM_SRGB,
};

///
/// Macro to get the bytes per pixel from a BitmapFormat
///
#define GetBytesPerPixel(x) (x == kBitmapFormat_A8_UNORM? 1 : 4)

///
/// @brief  Bitmap container with basic blitting and conversion routines.
///
class UExport Bitmap : public RefCounted {
 public:
  ///
  /// Create an empty Bitmap. No pixels will be allocated.
  ///
  static Ref<Bitmap> Create();

  ///
  /// Create a Bitmap with a certain configuration. Pixels will be allocated
  /// but not initialized.
  ///
  /// @param  width   The width in pixels.
  ///
  /// @param  height  The height in pixels.
  ///
  /// @param  format  The pixel format to use.
  ///
  /// @return  A ref-pointer to a new Bitmap instance.
  ///
  static Ref<Bitmap> Create(uint32_t width, uint32_t height,
                            BitmapFormat format);

  ///
  /// Create a Bitmap with existing pixels and configuration.
  ///
  /// @param  width        The width in pixels.
  ///
  /// @param  height       The height in pixels.
  ///
  /// @param  format       The pixel format to use.
  ///
  /// @param  row_bytes    The number of bytes between each row (note that this
  ///                      value should be >= width * bytes_per_pixel).
  ///
  /// @param  pixels       Pointer to raw pixel buffer.
  ///
  /// @param  size         Size of the raw pixel buffer.
  ///
  /// @param  should_copy  Whether or not a copy should be made of the pixels.
  ///                      If this is false, the returned Bitmap will use the
  ///                      raw pixels passed in as its own, but you are still
  ///                      responsible for destroying your buffer afterwards.
  ///
  /// @param  fixup_gamma  Whether or not we should reinterpret the source
  ///                      as an sRGB bitmap with premultiplied alpha applied
  ///                      after the gamma function (typical of PNGs). We
  ///                      expect all premultiplication to be applied before
  ///                      the gamma function so we can blend properly in
  ///                      linear space. Only valid for
  ///                      kBitmapFormat_BGRA8_UNORM_SRGB.
  ///
  /// @return  A ref-pointer to a new Bitmap instance.
  ///
  static Ref<Bitmap> Create(uint32_t width, uint32_t height,
                            BitmapFormat format, uint32_t row_bytes,
                            const void* pixels, size_t size,
                            bool should_copy = true, bool fixup_gamma = false);

  ///
  /// Create a bitmap from a deep copy of another Bitmap.
  ///
  static Ref<Bitmap> Create(const Bitmap& bitmap);

  ///
  /// Get the width in pixels.
  ///
  virtual uint32_t width() const = 0;

  ///
  /// Get the height in pixels.
  ///
  virtual uint32_t height() const = 0;

  ///
  /// Get the bounds as an IntRect
  ///
  virtual IntRect bounds() const = 0;

  ///
  /// Get the pixel format.
  ///
  virtual BitmapFormat format() const = 0;

  ///
  /// Get the number of bytes per pixel.
  ///
  virtual uint32_t bpp() const = 0;

  ///
  /// Get the number of bytes between each row of pixels.
  ///
  /// @note  This value is usually calculated as width * bytes_per_pixel (bpp) 
  ///        but it may be larger due to alignment rules in the allocator.
  ///
  virtual uint32_t row_bytes() const = 0;

  ///
  /// Get the size in bytes of the pixel buffer.
  ///
  /// @note  Size is calculated as row_bytes() * height().
  ///
  virtual size_t size() const = 0;

  ///
  /// Whether or not this Bitmap owns the pixel buffer and will destroy it
  /// at the end of its lifetime.
  ///
  virtual bool owns_pixels() const = 0;

  ///
  /// Lock the pixel buffer for reading/writing. 
  ///
  /// @return  A pointer to the pixel buffer.
  ///
  virtual void* LockPixels() = 0;

  ///
  /// Unlock the pixel buffer.
  ///
  virtual void UnlockPixels() = 0;

  ///
  /// Lock the pixel buffer for reading/writing. (const)
  ///
  /// @return  A const pointer to the pixel buffer.
  ///
  virtual const void* LockPixels() const = 0;

  ///
  /// Unlock the pixel buffer. (const)
  ///
  virtual void UnlockPixels() const = 0;

  ///
  /// Get the raw pixel buffer.
  ///
  /// @note  You should only call this if pixels are already locked.
  ///
  virtual void* raw_pixels() = 0;

  ///
  /// Whether or not this Bitmap is empty (no pixels allocated).
  ///
  virtual bool IsEmpty() const = 0;

  ///
  /// Erase the Bitmap (set all pixels to 0).
  ///
  virtual void Erase() = 0;

  ///
  /// Assign another bitmap to this one.
  ///
  /// @param  bitmap  The bitmap to copy from.
  ///
  virtual void Set(Ref<Bitmap> bitmap) = 0;

  ///
  /// Draw another bitmap to this bitmap.
  ///
  /// @note  Formats do not need to match. Bitmap formats will be converted
  ///        to one another automatically. Note that when converting from
  ///        BGRA8 to A8, only the Blue channel will be used.
  ///
  /// @param  src_rect    The source rectangle, relative to src bitmap.
  ///
  /// @param  dest_rect   The destination rectangle, relative to this bitmap.
  /// 
  /// @param  src         The source bitmap.
  /// 
  /// @param  pad_repeat  Whether or not we should pad the drawn bitmap by one
  ///                     pixel of repeated edge pixels from the source bitmap.
  ///
  /// @return  Whether or not the operation succeeded (this can fail if the
  ///          src_rect and/or dest_rect are invalid, or if their total
  ///          dimensions do not match).
  ///
  virtual bool DrawBitmap(IntRect src_rect, IntRect dest_rect,
    Ref<Bitmap> src, bool pad_repeat) = 0;

  ///
  /// Write this Bitmap out to a PNG image. (mainly used for Debug)
  ///
  /// @param  path  The filepath to write to (opened with fopen())
  ///
  /// @return  Whether or not the operation succeeded.
  ///
  virtual bool WritePNG(const char* path) = 0;

  ///
  /// Make a resized copy of this bitmap by writing to a pre-allocated
  /// destination bitmap.
  ///
  /// @param  destination  The bitmap to store the result in, the width and
  ///                      height of the destination will be used.
  ///
  /// @param  high_quality  Whether or not a high quality resampling will be
  ///                       used during the resize. (Otherwise, just uses fast
  ///                       nearest-neighbor sampling)
  ///
  /// @return  Whether or not the operation succeeded. This operation is only
  ///          valid if both formats are kBitmapFormat_BGRA8_UNORM_SRGB and
  ///          both the source and destination are non-empty.
  ///
  virtual bool Resample(Ref<Bitmap> destination, bool high_quality) = 0;

  ///
  /// This converts a BGRA bitmap to RGBA bitmap and vice-versa by swapping
  /// the red and blue channels.
  ///
  virtual void SwapRedBlueChannels() = 0;

protected:
  Bitmap();
  virtual ~Bitmap();
  Bitmap(const Bitmap&);
  void operator=(const Bitmap&);
};

#pragma pack(pop)

}  // namespace ultralight
