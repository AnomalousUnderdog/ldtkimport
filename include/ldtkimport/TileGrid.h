#ifndef LDTK_IMPORT_TILE_GRID_H
#define LDTK_IMPORT_TILE_GRID_H

#include <cstdint>
#include <string>
#include <vector>
#include <sstream>

#include "ldtkimport/AssertUtility.h"
#include "ldtkimport/GridUtility.h"
#include "ldtkimport/Types.h"
#include "ldtkimport/TileInCell.h"


namespace ldtkimport
{

/**
 *  @brief Tiles that will be drawn in one cell.
 *  This is prioritized, first element should be
 *  visually on top.
 */
using tiles_t = std::vector<TileInCell>;

/**
 *  @brief A grid of Tile Id values to be drawn on-screen.
 *
 *  Unlike IntGrid, which defines what "type" of thing is in the cell,
 *  a TileGrid only defines what is visually displayed in the cell.
 *
 *  TileGrid allows stacking of tiles in one cell.
 *
 *  The values here are basically tiles that have been placed down after Rules are applied.
 */
class TileGrid
{
public:

   TileGrid() :
      m_layerUid(0),
      m_width(0),
      m_height(0),
      m_grid()
   {
   }

   TileGrid(dimensions_t width, dimensions_t height) :
      m_layerUid(0),
      m_width(width),
      m_height(height),
      m_grid(width * height, tiles_t())
   {
   }

   void setLayerUid(uid_t newLayerUid)
   {
      m_layerUid = newLayerUid;
   }

   /**
    *  @brief Place a tile, to be draw in the indicated location.
    *
    *  @param[in] tileId The Tile Id to place at the location.
    *  @param[in] cellX X-coordinate of the location. This value is in "grid-space", not pixels. Starts at 0, which is at the left edge of the grid.
    *  @param[in] cellY Y-coordinate of the location. This value is in "grid-space", not pixels. Starts at 0, which is at the top edge of the grid.
    *  @param[in] flags Bitwise flags of indicators how the tile should be drawn.
    *  @param[in] priority Only used in the rule matching process to fix problems with z-order of stamp tiles.
    */
   void putTile(tileid_t tileId, int cellX, int cellY, uint8_t flags, uint8_t priority)
   {
      ASSERT_THROW(cellX >= 0, std::out_of_range, "supplied cellX index is negative: " << cellX);
      ASSERT_THROW(cellY >= 0, std::out_of_range, "supplied cellY index is negative: " << cellY);
      ASSERT_THROW(cellX < m_width, std::out_of_range, "supplied cellX index is beyond width: " << cellX << " (width: " << m_width << ")");
      ASSERT_THROW(cellY < m_height, std::out_of_range, "supplied cellY index is beyond height: " << cellY << " (height: " << m_height << ")");

      size_t cellIdx = GridUtility::getIndex(cellX, cellY, m_width);

      ASSERT_THROW(cellIdx >= 0, std::out_of_range, "supplied index is negative: " << cellIdx);
      ASSERT_THROW(cellIdx < m_grid.size(), std::out_of_range, "supplied index is beyond size: " << cellIdx << " (size: " << m_grid.size() << ")");

      m_grid[cellIdx].push_back(TileInCell(tileId, flags, priority));
   }

   /**
    *  @brief Checks if location is still allowed to have more tiles on it.
    *  ldtkimport::Rule::breakOnMatch controls this behaviour.
    *
    *  @param[in] cellX X-coordinate of the location. This value is in "grid-space", not pixels. Starts at 0, which is at the left edge of the grid.
    *  @param[in] cellY Y-coordinate of the location. This value is in "grid-space", not pixels. Starts at 0, which is at the top edge of the grid.
    */
   bool canStillPlaceTiles(int cellX, int cellY)
   {
      ASSERT_THROW(m_width > 0, std::runtime_error,
         "Trying to use an AppliedRules that has 0 width. It's probably not initialized yet. grid size: " << m_width << "x" << m_height);
      ASSERT_THROW(m_height > 0, std::runtime_error,
         "Trying to use an AppliedRules that has 0 height. It's probably not initialized yet. grid size: " << m_width << "x" << m_height);

      ASSERT_THROW(cellX >= 0, std::out_of_range,
         "supplied cellX index to an AppliedRules is negative: " << cellX);
      ASSERT_THROW(cellY >= 0, std::out_of_range,
         "supplied cellY index to an AppliedRules is negative: " << cellY);
      ASSERT_THROW(cellX < m_width, std::out_of_range,
         "supplied cellX index to an AppliedRules is beyond width: " << cellX << " (width: " << m_width << ")");
      ASSERT_THROW(cellY < m_height, std::out_of_range,
         "supplied cellY index to an AppliedRules is beyond height: " << cellY << " (height: " << m_height << ")");

      size_t cellIdx = GridUtility::getIndex(cellX, cellY, m_width);

      ASSERT_THROW(cellIdx >= 0, std::out_of_range, "supplied index to an AppliedRules is negative: " << cellIdx);
      ASSERT_THROW(cellIdx < m_grid.size(), std::out_of_range, "supplied index to an AppliedRules is beyond size: " << cellIdx << " (size: " << m_grid.size() << ")");

      tiles_t &tiles = m_grid[cellIdx];
      for (auto t = tiles.data(), end = tiles.data() + tiles.size(); t != end; ++t)
      {
         if (t->isFinal())
         {
            return false;
         }
      }

      return true;
   }

   /**
    *  @brief Gets the value of the highest priority placed on the location.
    *  The priority determines if the tile should be placed higher than
    *  other tiles that might obscure it (like stamp tiles that are positioned with 0.5 pivot).
    *  This is used to fix z-order of such tiles during the rule matching process.
    *
    *  @param[in] cellX X-coordinate of the location. This value is in "grid-space", not pixels. Starts at 0, which is at the left edge of the grid.
    *  @param[in] cellY Y-coordinate of the location. This value is in "grid-space", not pixels. Starts at 0, which is at the top edge of the grid.
    */
   uint8_t getHighestPriority(int cellX, int cellY)
   {
      ASSERT_THROW(m_width > 0, std::runtime_error,
         "Trying to use an AppliedRules that has 0 width. It's probably not initialized yet. grid size: " << m_width << "x" << m_height);
      ASSERT_THROW(m_height > 0, std::runtime_error,
         "Trying to use an AppliedRules that has 0 height. It's probably not initialized yet. grid size: " << m_width << "x" << m_height);

      ASSERT_THROW(cellX >= 0, std::out_of_range,
         "supplied cellX index to an AppliedRules is negative: " << cellX);
      ASSERT_THROW(cellY >= 0, std::out_of_range,
         "supplied cellY index to an AppliedRules is negative: " << cellY);
      ASSERT_THROW(cellX < m_width, std::out_of_range,
         "supplied cellX index to an AppliedRules is beyond width: " << cellX << " (width: " << m_width << ")");
      ASSERT_THROW(cellY < m_height, std::out_of_range,
         "supplied cellY index to an AppliedRules is beyond height: " << cellY << " (height: " << m_height << ")");

      size_t cellIdx = GridUtility::getIndex(cellX, cellY, m_width);

      ASSERT_THROW(cellIdx >= 0, std::out_of_range, "supplied index to an AppliedRules is negative: " << cellIdx);
      ASSERT_THROW(cellIdx < m_grid.size(), std::out_of_range, "supplied index to an AppliedRules is beyond size: " << cellIdx << " (size: " << m_grid.size() << ")");

      tiles_t &tiles = m_grid[cellIdx];
      uint8_t result = UINT8_MAX;
      for (auto t = tiles.data(), end = tiles.data() + tiles.size(); t != end; ++t)
      {
         if (t->priority < result)
         {
            result = t->priority;
         }
      }

      return result;
   }

   tiles_t &operator()(size_t idx);
   const tiles_t &operator()(size_t idx) const;

   size_t size() const;

   tiles_t &operator()(int x, int y);
   const tiles_t &operator()(int x, int y) const;

   dimensions_t getWidth() const;
   dimensions_t getHeight() const;

   /**
    *  @brief Change grid's size. This tries to retain the previous values as much as it can.
    */
   void setSize(int width, int height)
   {
      ASSERT(width > 0, "desired width for an AppliedRules should be greater than zero, but is " << width);
      ASSERT(height > 0, "desired height for an AppliedRules should be greater than zero, but is " << height);

      if (m_width == width && m_height == height)
      {
         // already at that size
         return;
      }

      /// @todo properly move values since we are enlarging/shrinking
      m_grid.resize(width * height);
      m_width = width;
      m_height = height;
   }

   /**
    *  @brief Removes all previously placed tiles in the grid. The width and height stay the same.
    */
   void cleanUp()
   {
      for (auto tiles = m_grid.begin(), end = m_grid.end(); tiles != end; ++tiles)
      {
         tiles->clear();
      }
   }

   friend std::ostream &operator<<(std::ostream &os, const TileGrid &tileGrid);

private:

   /**
    *  @brief Which Layer this TileGrid corresponds to.
    */
   uid_t m_layerUid;

   dimensions_t m_width;
   dimensions_t m_height;
   std::vector<tiles_t> m_grid;
};

inline tiles_t &TileGrid::operator()(size_t idx)
{
   ASSERT_THROW(idx >= 0, std::out_of_range,
      "supplied index to an AppliedRules is negative: " << idx);
   ASSERT_THROW(idx < m_grid.size(), std::out_of_range,
      "supplied index to an AppliedRules is beyond total number of cells. idx: " << idx << " (total number of cells: " << m_grid.size() << ")");

   return m_grid[idx];
}

inline const tiles_t &TileGrid::operator()(size_t idx) const
{
   ASSERT_THROW(idx >= 0, std::out_of_range,
      "supplied index to an AppliedRules is negative: " << idx);
   ASSERT_THROW(idx < m_grid.size(), std::out_of_range,
      "supplied index to an AppliedRules is beyond total number of cells. idx: " << idx << " (total number of cells: " << m_grid.size() << ")");

   return m_grid[idx];
}

inline size_t TileGrid::size() const
{
   return m_grid.size();
}

inline tiles_t &TileGrid::operator()(int x, int y)
{
   ASSERT_THROW(x >= 0, std::out_of_range,
      "supplied x index to an AppliedRules is negative: " << x);
   ASSERT_THROW(y >= 0, std::out_of_range,
      "supplied y index to an AppliedRules is negative: " << y);

   ASSERT_THROW(x < m_width, std::out_of_range,
      "supplied x index to an AppliedRules is beyond width: " << x << " (width: " << m_width << ")");
   ASSERT_THROW(y < m_height, std::out_of_range,
      "supplied y index to an AppliedRules is beyond height: " << y << " (height: " << m_height << ")");

   return m_grid[GridUtility::getIndex(x, y, m_width)];
}

inline const tiles_t &TileGrid::operator()(int x, int y) const
{
   ASSERT_THROW(x >= 0, std::out_of_range,
      "supplied x index to an AppliedRules is negative: " << x);
   ASSERT_THROW(y >= 0, std::out_of_range,
      "supplied y index to an AppliedRules is negative: " << y);

   ASSERT_THROW(x < m_width, std::out_of_range,
      "supplied x index to an AppliedRules is beyond width: " << x << " (width: " << m_width << ")");
   ASSERT_THROW(y < m_height, std::out_of_range,
      "supplied y index to an AppliedRules is beyond height: " << y << " (height: " << m_height << ")");

   return m_grid[GridUtility::getIndex(x, y, m_width)];
}

inline dimensions_t TileGrid::getWidth() const
{
   return m_width;
}

inline dimensions_t TileGrid::getHeight() const
{
   return m_height;
}

inline std::ostream &operator<<(std::ostream &os, const TileGrid &tileGrid)
{
   os << "TileGrid: " << tileGrid.size() << std::endl;
   os << "Layer Uid: " << tileGrid.m_layerUid << std::endl;

   os << "  " << std::endl;
   for (int y = 0, height = tileGrid.getHeight(); y < height; ++y)
   {
      for (int x = 0, width = tileGrid.getWidth(); x < width; ++x)
      {
         const tiles_t &tilesInCell = tileGrid(x, y);

         auto tileLen = tilesInCell.size();

         if (tileLen == 0)
         {
            os << "[";
         }
         else
         {
            os << "[" << x << "," << y << ":";
            for (size_t n = 0; n < tileLen; ++n)
            {
               os << "(" << tilesInCell[n].tileId << ")";
               if (n < tileLen - 1)
               {
                  os << ", ";
               }
            }
         }

         if (x != width - 1)
         {
            os << "], ";
         }
         else
         {
            os << "]";
         }
      }

      if (y != height - 1)
      {
         os << std::endl;
         os << "  ";
      }
      else
      {
         os << std::endl;
      }
   }

   return os;
}

} // namespace ldtkimport

#endif // LDTK_IMPORT_TILE_GRID_H
