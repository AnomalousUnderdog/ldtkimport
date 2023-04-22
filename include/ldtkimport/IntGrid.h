#ifndef LDTK_IMPORT_INT_GRID_H
#define LDTK_IMPORT_INT_GRID_H

#include <cstdint>
#include <vector>
#include <sstream>
#include <iomanip>

#include "ldtkimport/AssertUtility.h"
#include "ldtkimport/Types.h"
#include "ldtkimport/GridUtility.h"
#include "ldtkimport/IntGridValue.h"


namespace ldtkimport
{

/**
 *  @brief Stores a vector of ints, accessed like a 2d grid.
 *  This represents a level, each int value inside acting as what type of "thing" is in each cell.
 *
 *  Most commonly, this is used as the collision map.
 *  For example, a level designer might assign the value of 1 as walkable areas and 2 as walls,
 *  or perhaps the values could be different types of terrain.
 *  It's up to the game using this to determine what exactly they are.
 *
 *  In an LdtkDefFile, a Layer of type IntGrid will have a vector of IntGridValue that can be used
 *  to at least give you user-readable names for each int value.
 *
 *  The only value hardcoded to have a built-in meaning here is that a value of 0 means nothing
 *  has been placed in that location.
 */
class IntGrid
{
public:

   IntGrid() :
      m_width(0),
      m_height(0),
      m_cells()
   {
   }

   /**
    *  @brief  Initialize the IntGrid and fill it with value 0.
    *
    *  @param  width Desired number of cells horizontally.
    *  @param  height Desired number of cells vertically.
    *
    *  @details The IntGrid will have width * height number of cells.
    */
   IntGrid(dimensions_t width, dimensions_t height) :
      m_width(width),
      m_height(height),
      m_cells(width * height, 0)
   {
   }

   /**
    *  @brief  Initialize the IntGrid and give it a vector of int values.
    *
    *  @param  width Desired number of cells horizontally.
    *  @param  height Desired number of cells vertically.
    *  @param  values Values to place inside the cells.
    *
    *  @details Size of values must be width * height.
    */
   IntGrid(dimensions_t width, dimensions_t height, std::vector<intgridvalue_t> &&values) :
      m_width(width),
      m_height(height),
      m_cells(values)
   {
   }

   intgridvalue_t &operator()(size_t idx);
   intgridvalue_t operator()(size_t idx) const;

   size_t size() const;

   intgridvalue_t &operator()(int x, int y);
   intgridvalue_t operator()(int x, int y) const;

   dimensions_t getWidth() const;
   dimensions_t getHeight() const;

   bool isWithinHorizontalBounds(int x) const;
   bool isWithinVerticalBounds(int y) const;
   bool isWithinBounds(int x, int y) const;

   void setSize(dimensions_t width, dimensions_t height)
   {
      ASSERT(width > 0, "desired width for an AppliedRules should be greater than zero, but is " << width);
      ASSERT(height > 0, "desired height for an AppliedRules should be greater than zero, but is " << height);

      if (m_width == width && m_height == height)
      {
         // already at that size
         return;
      }

      /// @todo properly move values since we are enlarging/shrinking
      m_cells.resize(static_cast<size_t>(width) * height);
      m_width = width;
      m_height = height;
   }

   void set(dimensions_t width, dimensions_t height, std::vector<intgridvalue_t> &&values)
   {
      m_width = width;
      m_height = height;

      m_cells = std::move(values);
   }

   void cleanUp()
   {
      for (auto cell = m_cells.begin(), end = m_cells.end(); cell != end; ++cell)
      {
         *cell = 0;
      }
   }

   friend std::ostream &operator<<(std::ostream &os, const IntGrid &intGrid);

private:
   dimensions_t m_width;
   dimensions_t m_height;
   std::vector<intgridvalue_t> m_cells;
};

inline intgridvalue_t &IntGrid::operator()(size_t idx)
{
   ASSERT_THROW(idx >= 0, std::out_of_range,
      "supplied index is negative: " << idx);
   ASSERT_THROW(idx < m_cells.size(), std::out_of_range,
      "supplied index is beyond size: " << idx << " (size: " << m_cells.size() << ")");

   return m_cells[idx];
}

inline intgridvalue_t IntGrid::operator()(size_t idx) const
{
   ASSERT_THROW(idx >= 0, std::out_of_range,
      "supplied index is negative: " << idx);
   ASSERT_THROW(idx < m_cells.size(), std::out_of_range,
      "supplied index is beyond size: " << idx << " (size: " << m_cells.size() << ")");

   return m_cells[idx];
}

inline size_t IntGrid::size() const
{
   return m_cells.size();
}

inline intgridvalue_t &IntGrid::operator()(int x, int y)
{
   ASSERT_THROW(x >= 0, std::out_of_range,
      "supplied x index is negative: " << x);
   ASSERT_THROW(y >= 0, std::out_of_range,
      "supplied y index is negative: " << y);
   ASSERT_THROW(x < m_width, std::out_of_range,
      "supplied x index is beyond width: " << x << " (width: " << m_width << ")");
   ASSERT_THROW(y < m_height, std::out_of_range,
      "supplied y index is beyond height: " << y << " (height: " << m_height << ")");

   return m_cells[GridUtility::getIndex(x, y, m_width)];
}

inline intgridvalue_t IntGrid::operator()(int x, int y) const
{
   ASSERT_THROW(x >= 0, std::out_of_range,
      "supplied x index is negative: " << x);
   ASSERT_THROW(y >= 0, std::out_of_range,
      "supplied y index is negative: " << y);
   ASSERT_THROW(x < m_width, std::out_of_range,
      "supplied x index is beyond width: " << x << " (width: " << m_width << ")");
   ASSERT_THROW(y < m_height, std::out_of_range,
      "supplied y index is beyond height: " << y << " (height: " << m_height << ")");

   return m_cells[GridUtility::getIndex(x, y, m_width)];
}

inline dimensions_t IntGrid::getWidth() const
{
   return m_width;
}

inline dimensions_t IntGrid::getHeight() const
{
   return m_height;
}

inline bool IntGrid::isWithinHorizontalBounds(int x) const
{
   return GridUtility::isWithinHorizontalBounds(x, m_width);
}

inline bool IntGrid::isWithinVerticalBounds(int y) const
{
   return GridUtility::isWithinVerticalBounds(y, m_height);
}

inline bool IntGrid::isWithinBounds(int x, int y) const
{
   return GridUtility::isWithinBounds(x, y, m_width, m_height);
}

inline std::ostream &operator<<(std::ostream &os, const IntGrid &intGrid)
{
   // first pass: we check the max amount of digits
   size_t maxDigitCount = 1;
   for (size_t n = 0, len = intGrid.size(); n < len; ++n)
   {
      auto digitCount = std::to_string(intGrid(n)).length();
      maxDigitCount = std::max(maxDigitCount, digitCount);
   }

   // second pass: finally display the values
   os << std::setw(maxDigitCount);
   uint8_t columnCount = 0;
   for (size_t n = 0, len = intGrid.size(); n < len; ++n)
   {
      os << +intGrid(n) << ", ";

      ++columnCount;
      if (columnCount == intGrid.getWidth())
      {
         if (n != len - 1)
         {
            os << std::endl;
            columnCount = 0;
         }
         else
         {
            os << std::endl;
         }
      }
   }

   return os;
}

} // namespace ldtkimport

#endif // LDTK_IMPORT_INT_GRID_H
