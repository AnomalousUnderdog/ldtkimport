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
      posXOffset(0),
      posYOffset(0),
      flags(TileFlags::NoFlags),
      priority(0)
   {
   }

   TileInCell(tileid_t newTileId, int8_t newPosXOffset, int8_t newPosYOffset, uint8_t newFlag, uint8_t newPriority) :
      tileId(newTileId),
      posXOffset(newPosXOffset),
      posYOffset(newPosYOffset),
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
    *  @brief Additional offset to the tile's x position,
    *  caused by Rule::posXOffset and the random offset
    *  (Rule::randomPosXOffsetMin to Rule::randomPosXOffsetMax).
    *  This value is in pixels.
    */
   int8_t posXOffset;

   /**
    *  @brief Additional offset to the tile's y position,
    *  caused by Rule::posYOffset and the random offset
    *  (Rule::randomPosYOffsetMin to Rule::randomPosYOffsetMax).
    *  This value is in pixels. Negative values move the tile up,
    *  positive values move the tile down.
    */
   int8_t posYOffset;

   /**
    *  @brief Various bool flags for how the tile is drawn.
    */
   uint8_t flags;

   /**
    *  @brief Only used in the rule matching process to fix problems with z-order of stamp tiles.
    */
   uint8_t priority;

   /**
    *  @brief Get the combined pixel values of the X offset for this tile.
    *  Will take into account if tile is flipped horizontally.
    */
   float getOffsetX(float halfWidth) const
   {
      if (isFlippedX())
      {
         if (hasOffsetRight())
         {
            // Even though the offset is to the right,
            // since it's flipped horizontally,
            // the offset will actually be to the left.
            return -halfWidth + posXOffset;
         }
         else if (hasOffsetLeft())
         {
            // Even though the offset is to the left
            // since it's flipped horizontally,
            // the offset will actually be to the right.
            return halfWidth + posXOffset;
         }
         else
         {
            return posXOffset;
         }
      }
      else
      {
         if (hasOffsetRight())
         {
            return halfWidth + posXOffset;
         }
         else if (hasOffsetLeft())
         {
            return -halfWidth + posXOffset;
         }
         else
         {
            return posXOffset;
         }
      }
   }

   /**
    *  @brief Get the combined pixel values of the Y offset for this tile.
    *  Will take into account if tile is flipped vertically.
    */
   float getOffsetY(float halfHeight) const
   {
      if (isFlippedY())
      {
         if (hasOffsetDown())
         {
            // Even though the offset is downward,
            // since it's flipped vertically,
            // the offset will actually be upward.
            return -halfHeight + posYOffset;
         }
         else if (hasOffsetUp())
         {
            // Even though the offset is upward,
            // since it's flipped vertically,
            // the offset will actually be downward.
            return halfHeight + posYOffset;
         }
         else
         {
            return posYOffset;
         }
      }
      else
      {
         if (hasOffsetDown())
         {
            return halfHeight + posYOffset;
         }
         else if (hasOffsetUp())
         {
            return -halfHeight + posYOffset;
         }
         else
         {
            return posYOffset;
         }
      }
   }

   bool hasOffsetLeft() const
   {
      return TileFlags::hasOffsetLeft(flags);
   }

   bool hasOffsetUp() const
   {
      return TileFlags::hasOffsetUp(flags);
   }

   bool hasOffsetRight() const
   {
      return TileFlags::hasOffsetRight(flags);
   }

   bool hasOffsetDown() const
   {
      return TileFlags::hasOffsetDown(flags);
   }

   bool isFlippedX() const
   {
      return TileFlags::isFlippedX(flags);
   }

   bool isFlippedY() const
   {
      return TileFlags::isFlippedY(flags);
   }

   bool isFinal() const
   {
      return TileFlags::isFinal(flags);
   }
};

} // namespace ldtkimport

#endif // LDTK_IMPORT_TILE_IN_CELL_H
