#ifndef LDTK_IMPORT_TILE_IN_CELL_H
#define LDTK_IMPORT_TILE_IN_CELL_H

#include <cstdint>

#include "ldtkimport/Types.h"
#include "ldtkimport/TileFlags.h"


namespace ldtkimport
{

/**
 *  @brief Represents one Tile placed on a cell by a Rule.
 *  Many of these can be on top of each other in one cell.
 */
struct TileInCell
{
   TileInCell() :
      tileId(0),
      flags(TILE_NO_FLAGS),
      priority(0)
   {
   }

   TileInCell(tileid_t newTileId, uint8_t newFlag, uint8_t newPriority) :
      tileId(newTileId),
      flags(newFlag),
      priority(newPriority)
   {
   }

   /**
    *  @brief Index of which tile is displayed for this cell.
    *
    *  This is in terms of tile size, not pixels.
    *  So a value of 0 means the upper-left tile.
    */
   tileid_t tileId;

   /**
    *  @brief Various bool flags for how the tile is drawn.
    */
   uint8_t flags;

   /**
    *  @brief Only used in the rule matching process to fix problems with z-order of stamp tiles.
    */
   uint8_t priority;

   /**
    *  @brief Get pixel values of the X offset for this tile.
    *  Will take into account if tile is flipped horizontally.
    */
   float getOffsetX(float halfWidth) const
   {
      if (isFlippedX())
      {
         if (hasOffsetRight())
         {
            return -halfWidth;
         }
         else if (hasOffsetLeft())
         {
            return halfWidth;
         }
         else
         {
            return 0;
         }
      }
      else
      {
         if (hasOffsetRight())
         {
            return halfWidth;
         }
         else if (hasOffsetLeft())
         {
            return -halfWidth;
         }
         else
         {
            return 0;
         }
      }
   }

   /**
    *  @brief Get pixel values of the Y offset for this tile.
    *  Will take into account if tile is flipped vertically.
    */
   float getOffsetY(float halfHeight) const
   {
      if (isFlippedY())
      {
         if (hasOffsetDown())
         {
            return -halfHeight;
         }
         else if (hasOffsetUp())
         {
            return halfHeight;
         }
         else
         {
            return 0;
         }
      }
      else
      {
         if (hasOffsetDown())
         {
            return halfHeight;
         }
         else if (hasOffsetUp())
         {
            return -halfHeight;
         }
         else
         {
            return 0;
         }
      }
   }

   bool hasOffsetLeft() const
   {
      return tile::hasOffsetLeft(flags);
   }

   bool hasOffsetUp() const
   {
      return tile::hasOffsetUp(flags);
   }

   bool hasOffsetRight() const
   {
      return tile::hasOffsetRight(flags);
   }

   bool hasOffsetDown() const
   {
      return tile::hasOffsetDown(flags);
   }

   bool isFlippedX() const
   {
      return tile::isFlippedX(flags);
   }

   bool isFlippedY() const
   {
      return tile::isFlippedY(flags);
   }

   bool isFinal() const
   {
      return tile::isFinal(flags);
   }
};

} // namespace ldtkimport

#endif // LDTK_IMPORT_TILE_IN_CELL_H
