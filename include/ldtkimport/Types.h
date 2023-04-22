#ifndef LDTK_IMPORT_TYPES_H
#define LDTK_IMPORT_TYPES_H

#include <cstdint>


namespace ldtkimport
{

/**
 *  @brief int values used to uniquely identify parts of an LDtk file.
 */
using uid_t = uint16_t;

/**
 *  @brief int values used to represent different types of cells.
 */
using intgridvalue_t = uint16_t;

#define INT_GRID_VALUE_MAX UINT16_MAX

/**
 *  @brief Int type used when comparing with an IntGridValue.
 *  This needs to be signed since a negative value has special meaning (inverted check).
 *  It also needs to be large enough to be able to hold the special value 1,000,001 (one million one)
 *  for indicating an "any" check.
 */
using pattern_t = int32_t;

/**
 *  @brief int values used to represent a Tile Id.
 *  Tile Id is the number assigned to one tile in the tile image atlas (TileSet).
 *  The value is basically the tile's position in that atlas, with 0 starting at the top-left.
 */
using tileid_t = uint16_t;

/**
 *  @brief For width/height values.
 */
using dimensions_t = uint16_t;

#define DIMENSIONS_VALUE_MAX UINT16_MAX

} // namespace ldtkimport

#endif // LDTK_IMPORT_TYPES_H
