#ifndef LDTK_IMPORT_TILE_SET_H
#define LDTK_IMPORT_TILE_SET_H

#include <string>

#include "ldtkimport/GridUtility.h"


namespace ldtkimport
{

/**
 *  @brief A TileSet is basically a spritesheet/image atlas for tiles (images laid out in a grid).
 *  This struct only holds metadata for the image
 *  (width/height, number of tiles, size of tiles, spacing between tiles, etc.)
 *  Actual image data should be handled by the game that is making use of this library.
 *
 *  @see https://ldtk.io/json/#ldtk-TilesetDefJson
 */
struct TileSet
{
   TileSet() :
      name(),
      uid(),
      imagePath(),
      imageWidth(0),
      imageHeight(0),
      tileSize(0),
      tileCountWidth(0),
      tileCountHeight(0),
      margin(0),
      spacing(0)
   {
   }

   /**
    *  @brief User-defined name of the tileset.
    *
    *  @see https://ldtk.io/json/#ldtk-TilesetDefJson;identifier
    */
   std::string name;

   /**
    *  @brief Automatically assigned unique-identifier for this tileset.
    *  Used when other parts of the data need to refer to this tileset.
    *
    *  @see https://ldtk.io/json/#ldtk-TilesetDefJson;uid
    */
   uid_t uid;

   /**
    *  @brief https://ldtk.io/json/#ldtk-TilesetDefJson;relPath
    */
   std::string imagePath;

   /**
    *  @brief https://ldtk.io/json/#ldtk-TilesetDefJson;pxWid
    */
   dimensions_t imageWidth;

   /**
    *  @brief https://ldtk.io/json/#ldtk-TilesetDefJson;pxHei
    */
   dimensions_t imageHeight;

   /**
    *  @brief How big a tile is, in pixels.
    *  This is both width and height (tiles are assumed to be squares).
    *
    *  @see https://ldtk.io/json/#ldtk-TilesetDefJson;tileGridSize
    */
   dimensions_t tileSize;

   /**
    *  @brief How many tiles the image has, horizontally.
    *  This is basically (imageWidth-(margin*2))/tileSize.
    *
    *  @see https://ldtk.io/json/#ldtk-TilesetDefJson;__cWid
    */
   dimensions_t tileCountWidth;

   /**
    *  @brief How many tiles the image has, vertically.
    *  This is basically (imageHeight-(margin*2))/tileSize.
    *
    *  @see https://ldtk.io/json/#ldtk-TilesetDefJson;__cHei
    */
   dimensions_t tileCountHeight;

   /**
    *  @brief Where do the tiles start in the image, relative to the edges of the image.
    *  This is assuming the top margin and left margin have the same size.
    *  Some artists put a border on their tilesheet so that's why this is needed.
    *
    *  @see https://ldtk.io/json/#ldtk-TilesetDefJson;padding
    */
   dimensions_t margin;

   /**
    *  @brief Space in pixels between all tiles.
    *  Some artists put a grid on their tiles so that's why this is needed.
    *
    *  @see https://ldtk.io/json/#ldtk-TilesetDefJson;spacing
    */
   dimensions_t spacing;

   /**
    *  @brief Given a Tile Id (which is also the index within the tile array),
    *  this returns the x and y coordinates of that Tile Id.
    *  The coordinates are in "grid-space", not in pixels, so
    *  Tile Id 0 will return the coordinates (0, 0),
    *  Tile Id 1 will return the coordinates (1, 0), etc.
    */
   void getCoordinates(int index, int16_t &x, int16_t &y) const
   {
      GridUtility::getCoordinates(index, tileCountWidth, x, y);
   }
};

} // namespace ldtkimport

#endif // LDTK_IMPORT_TILE_SET_H
