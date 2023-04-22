#ifndef LDTK_IMPORT_GRID_UTILITY_H
#define LDTK_IMPORT_GRID_UTILITY_H

#include <cstddef>
#include <cstdint>

#include "ldtkimport/Types.h"


namespace ldtkimport
{
namespace GridUtility
{

// https://github.com/deepnight/deepnightLibs/blob/7dd158925f02873d4bf751e1cdc953d98d77ad0b/src/dn/M.hx#L526
static inline size_t getRandomIndex(int seed, int x, int y, size_t max)
{
   // Based on xxhash
   // Source: https://stackoverflow.com/a/37221804/1377948
   // Note: h is meant to overflow on purpose
   int32_t h = seed + (x * 374761393) + (y * 668265263); // all constants are prime
   h = (h ^ (h >> 13)) * 1274126177;
   return (h ^ (h >> 16)) % max;
}

static inline int16_t getRandomIndex(int seed, int x, int y, int16_t max)
{
   // Based on xxhash
   // Source: https://stackoverflow.com/a/37221804/1377948
   // Note: h is meant to overflow on purpose
   int32_t h = seed + (x * 374761393) + (y * 668265263); // all constants are prime
   h = (h ^ (h >> 13)) * 1274126177;
   return (h ^ (h >> 16)) % max;
}

/**
 *  @brief Assuming you have a 1-dimensional array used as a 2d grid,
 *  this converts an x, y coordinate to the array index to allow accessing it with the [] operator.
 */
static inline size_t getIndex(int x, int y, dimensions_t width)
{
   return (static_cast<size_t>(y) * width) + x;
}

/**
 *  @brief Assuming you have a 1-dimensional array used as a 2d grid,
 *  this converts an array index to x, y coordinates.
 */
static inline void getCoordinates(int index, dimensions_t width, int &x, int &y)
{
   y = static_cast<int>(index) / width;
   x = static_cast<int>(index) - (width * y);
}

/**
 *  @brief Assuming you have a 1-dimensional array used as a 2d grid,
 *  this converts an array index to x, y coordinates.
 */
static inline void getCoordinates(int index, dimensions_t width, int16_t &x, int16_t &y)
{
   y = static_cast<int16_t>(index / width);
   x = static_cast<int16_t>(index - (width * y));
}

/**
 *  @brief Are x and y within the grid specified by width and height?
 *  The grid is considered to be anchored to the origin of (0, 0), so
 *  any negative x/y value is immediately considered out-of-bounds.
 */
static inline bool isWithinBounds(int x, int y, dimensions_t width, dimensions_t height)
{
   return x >= 0 && x < width && y >= 0 && y < height;
}

/**
 *  @brief Is x within the grid specified by width?
 *  The grid is considered to be anchored to the origin of (0, 0), so
 *  any negative x value is immediately considered out-of-bounds.
 */
static inline bool isWithinHorizontalBounds(int x, dimensions_t width)
{
   return x >= 0 && x < width;
}

/**
 *  @brief Is y within the grid specified by height?
 *  The grid is considered to be anchored to the origin of (0, 0), so
 *  any negative y value is immediately considered out-of-bounds.
 */
static inline bool isWithinVerticalBounds(int y, dimensions_t height)
{
   return y >= 0 && y < height;
}

} // namespace GridUtility
} // namespace ldtkimport

#endif // LDTK_IMPORT_GRID_UTILITY_H
