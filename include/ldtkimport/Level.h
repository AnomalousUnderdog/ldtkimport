#ifndef LDTK_IMPORT_LEVEL_H
#define LDTK_IMPORT_LEVEL_H

#include <vector>

#include "ldtkimport/IntGrid.h"
#include "ldtkimport/TileGrid.h"


namespace ldtkimport
{

/**
 *  @brief Level holds runtime data of an actual level that can be displayed and used in a game.
 */
class Level
{
public:

   /**
    *  @brief Assign values to the Level's IntGrid. This will resize the TileGrids.
    */
   void setIntGrid(dimensions_t width, dimensions_t height, std::vector<intgridvalue_t> &&values)
   {
      m_intGrid.set(width, height, std::move(values));

      for (auto tileGrid = m_tileGrids.begin(), end = m_tileGrids.end(); tileGrid != end; ++tileGrid)
      {
         tileGrid->setSize(width, height);
      }
   }

   /**
    *  @brief Assign the value in one cell using the specified x and y coordinates.
    *  @param x x-coordinate. Starts at 0 (left edge of grid).
    *  @param y y-coordinate. Starts at 0 (top edge of grid).
    *  @param value Value to assign at the cell.
    */
   void setIntGrid(int x, int y, intgridvalue_t value)
   {
      m_intGrid(x, y) = value;
   }

   void setIntGrid(int idx, intgridvalue_t value)
   {
      m_intGrid(idx) = value;
   }

   dimensions_t getWidth() const
   {
      return m_intGrid.getWidth();
   }

   dimensions_t getHeight() const
   {
      return m_intGrid.getHeight();
   }

   bool isWithinBounds(int x, int y) const
   {
      return m_intGrid.isWithinBounds(x, y);
   }

   const IntGrid &getIntGrid() const
   {
      return m_intGrid;
   }

   size_t getTileGridCount() const
   {
      return m_tileGrids.size();
   }

   /**
    *  @brief Change the number of TileGrids in the Level.
    */
   void setTileGridCount(size_t newCount)
   {
      while (m_tileGrids.size() < newCount)
      {
         m_tileGrids.push_back(TileGrid(m_intGrid.getWidth(), m_intGrid.getHeight()));
      }
      while (m_tileGrids.size() > newCount)
      {
         m_tileGrids.pop_back();
      }
   }

   /**
    *  @brief Assign 0 to all the cells in the IntGrid.
    */
   void cleanUpIntGrid()
   {
      m_intGrid.cleanUp();
   }

   /**
    *  @brief Removes all previously placed tiles in all the TileGrids. The width and height stay the same.
    */
   void cleanUpTileGrids()
   {
      for (auto tileGrid = m_tileGrids.begin(), end = m_tileGrids.end(); tileGrid != end; ++tileGrid)
      {
         tileGrid->cleanUp();
      }
   }

   TileGrid &getTileGrid(int idx)
   {
      return m_tileGrids[idx];
   }

   const TileGrid &getTileGrid(int idx) const
   {
      return m_tileGrids[idx];
   }

   friend std::ostream &operator<<(std::ostream &os, const Level &level);

   void debugPrintTileGrids(std::ostream &os) const;

private:

   IntGrid m_intGrid;

   /**
    * @brief Results of rules applied on the Level are stored here.
    */
   std::vector<TileGrid> m_tileGrids;
};

inline std::ostream &operator<<(std::ostream &os, const Level &level)
{
   os << "size: " << level.getWidth() << "x" << level.getHeight() << std::endl;
   os << "count: " << level.m_intGrid.size() << std::endl;
   os << level.m_intGrid;

   return os;
}

inline void Level::debugPrintTileGrids(std::ostream &os) const
{
   os << "TileGrids: " << m_tileGrids.size() << std::endl;
   for (auto tileGrid = m_tileGrids.cbegin(), end = m_tileGrids.cend(); tileGrid != end; ++tileGrid)
   {
      os << *tileGrid << std::endl;
   }
}

} // namespace ldtkimport

#endif // LDTK_IMPORT_LEVEL_H
