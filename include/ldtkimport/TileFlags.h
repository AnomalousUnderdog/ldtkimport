#ifndef LDTK_IMPORT_TILE_FLAGS_H
#define LDTK_IMPORT_TILE_FLAGS_H

namespace ldtkimport
{

/**
 *  @brief Means the tile should be drawn normally.
 */
static const uint8_t TILE_NO_FLAGS = 0;

/**
 *  @brief Indication that the tile should be drawn with an offset to the left.
 *  The distance of the offset is half of the grid size.
 */
static const uint8_t TILE_OFFSET_LEFT = 1 << 0;

/**
 *  @brief Indication that the tile should be drawn with an offset upwards.
 *  The distance of the offset is half of the grid size.
 */
static const uint8_t TILE_OFFSET_UP = 1 << 1;

/**
 *  @brief Indication that the tile should be drawn with an offset to the right.
 *  The distance of the offset is half of the grid size.
 */
static const uint8_t TILE_OFFSET_RIGHT = 1 << 2;

/**
 *  @brief Indication that the tile should be drawn with an offset downwards.
 *  The distance of the offset is half of the grid size.
 */
static const uint8_t TILE_OFFSET_DOWN = 1 << 3;

/**
 *  @brief Indicates tile should be drawn mirrored horizontally.
 */
static const uint8_t TILE_FLIPPED_X = 1 << 4;

/**
 *  @brief Indicates tile should be drawn mirrored vertically.
 */
static const uint8_t TILE_FLIPPED_Y = 1 << 5;

/**
 *  @brief Indicates tile prevents other rules from being executed in its location.
 */
static const uint8_t TILE_FINAL = 1 << 6;

namespace tile
{

/**
 *  @see If the tile should be drawn with an offset to the left.
 */
static inline bool hasOffsetLeft(const uint8_t flags)
{
   return (flags & TILE_OFFSET_LEFT) == TILE_OFFSET_LEFT;
}

/**
 *  @see If the tile should be drawn with an offset upwards.
 */
static inline bool hasOffsetUp(const uint8_t flags)
{
   return (flags & TILE_OFFSET_UP) == TILE_OFFSET_UP;
}

/**
 *  @see If the tile should be drawn with an offset to the right.
 */
static inline bool hasOffsetRight(const uint8_t flags)
{
   return (flags & TILE_OFFSET_RIGHT) == TILE_OFFSET_RIGHT;
}

/**
 *  @see If the tile should be drawn with an offset downwards.
 */
static inline bool hasOffsetDown(const uint8_t flags)
{
   return (flags & TILE_OFFSET_DOWN) == TILE_OFFSET_DOWN;
}

/**
 *  @see If tile should be drawn mirrored horizontally.
 */
static inline bool isFlippedX(const uint8_t flags)
{
   return (flags & TILE_FLIPPED_X) == TILE_FLIPPED_X;
}

/**
 *  @see If tile should be drawn mirrored vertically.
 */
static inline bool isFlippedY(const uint8_t flags)
{
   return (flags & TILE_FLIPPED_Y) == TILE_FLIPPED_Y;
}

/**
 *  @see If tile prevents other rules from being executed in its location.
 */
static inline bool isFinal(const uint8_t flags)
{
   return (flags & TILE_FINAL) == TILE_FINAL;
}

} // namespace tile
} // namespace ldtkimport

#endif // LDTK_IMPORT_TILE_FLAGS_H
