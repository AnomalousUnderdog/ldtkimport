#ifndef LDTK_IMPORT_TILE_FLAGS_H
#define LDTK_IMPORT_TILE_FLAGS_H

namespace ldtkimport
{

namespace TileFlags
{

/**
 *  @brief Means the tile should be drawn normally.
 */
static const uint8_t NoFlags = 0;

/**
 *  @brief Indication that the tile should be drawn with an offset to the left.
 *  The distance of the offset is half of the grid size.
 */
static const uint8_t LeftOffset = 1 << 0;

/**
 *  @brief Indication that the tile should be drawn with an offset upwards.
 *  The distance of the offset is half of the grid size.
 */
static const uint8_t UpOffset = 1 << 1;

/**
 *  @brief Indication that the tile should be drawn with an offset to the right.
 *  The distance of the offset is half of the grid size.
 */
static const uint8_t RightOffset = 1 << 2;

/**
 *  @brief Indication that the tile should be drawn with an offset downwards.
 *  The distance of the offset is half of the grid size.
 */
static const uint8_t DownOffset = 1 << 3;

/**
 *  @brief Indicates tile should be drawn mirrored horizontally.
 */
static const uint8_t FlippedX = 1 << 4;

/**
 *  @brief Indicates tile should be drawn mirrored vertically.
 */
static const uint8_t FlippedY = 1 << 5;

/**
 *  @brief Indicates tile prevents other rules from being executed in its location.
 */
static const uint8_t Final = 1 << 6;

/**
 *  @see If the tile should be drawn with an offset to the left.
 */
static inline bool hasOffsetLeft(const uint8_t flags)
{
   return (flags & LeftOffset) == LeftOffset;
}

/**
 *  @see If the tile should be drawn with an offset upwards.
 */
static inline bool hasOffsetUp(const uint8_t flags)
{
   return (flags & UpOffset) == UpOffset;
}

/**
 *  @see If the tile should be drawn with an offset to the right.
 */
static inline bool hasOffsetRight(const uint8_t flags)
{
   return (flags & RightOffset) == RightOffset;
}

/**
 *  @see If the tile should be drawn with an offset downwards.
 */
static inline bool hasOffsetDown(const uint8_t flags)
{
   return (flags & DownOffset) == DownOffset;
}

/**
 *  @see If tile should be drawn mirrored horizontally.
 */
static inline bool isFlippedX(const uint8_t flags)
{
   return (flags & FlippedX) == FlippedX;
}

/**
 *  @see If tile should be drawn mirrored vertically.
 */
static inline bool isFlippedY(const uint8_t flags)
{
   return (flags & FlippedY) == FlippedY;
}

/**
 *  @see If tile prevents other rules from being executed in its location.
 */
static inline bool isFinal(const uint8_t flags)
{
   return (flags & Final) == Final;
}

} // namespace tile
} // namespace ldtkimport

#endif // LDTK_IMPORT_TILE_FLAGS_H
