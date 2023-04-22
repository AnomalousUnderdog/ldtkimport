#ifndef LDTK_IMPORT_INT_GRID_VALUE_H
#define LDTK_IMPORT_INT_GRID_VALUE_H

#include <cstdint>
#include <string>

#include "ldtkimport/Types.h"

namespace ldtkimport
{

/**
 *  @brief This defines what "type" a cell is, whether it represents
 *  a wall, floor, vegetation, a particular type of terrain, etc.
 *  It's up to the level designer to determine what it exactly is.
 *
 *  This is only the data. IntGridValue only provides identification
 *  for the "type" of cell, and what user-defined name it has.
 *
 *  The Layer struct, on the other hand, tells you which cells in
 *  the level have that "type" of cell.
 *
 *  And finally, what actual image is used to display
 *  each "type" of cell is determined by the Rule struct.
 *
 *  The LDtk file doesn't really enforce any gameplay rules.
 *
 *  For example, maybe want characters to have
 *  slower movement speed when moving through mud or snow tiles,
 *  but the LDtk file isn't where you define that.
 *  What it can do, at the very least, is tell you which cells
 *  in the level have that mud/snow tile.
 *
 *  But it's up to the game using the LDtk file to interpret what
 *  effect each type of cell does, if any.
 */
struct IntGridValue
{
   IntGridValue() :
      id(0),
      name()
   {
   }

   /**
    *  @brief Unique identifier of this IntGridValue.
    *
    *  Though a Rule doesn't really need an IntGridValue struct to operate,
    *  the numerical value of an id is what a Rule refers to in its pattern.
    */
   intgridvalue_t id;

   /**
    *  @brief User-defined name for this IntGridValue.
    */
   std::string name;
};

} // namespace ldtkimport

#endif // LDTK_IMPORT_INT_GRID_VALUE_H
