#ifndef LDTK_IMPORT_RULE_H
#define LDTK_IMPORT_RULE_H

#include <string>
#include <vector>
#include <ostream>

#include "ldtkimport/MiscUtility.h"
#include "ldtkimport/Types.h"
#include "ldtkimport/TileFlags.h"
#include "ldtkimport/IntGrid.h"
#include "ldtkimport/IntGridValue.h"
#include "ldtkimport/TileGrid.h"


namespace ldtkimport
{

static const int RULE_PATTERN_ANYTHING = 1000001;
static const int RULE_PATTERN_NOTHING = -1000001;

/// @warning Any Visual Studio Project making use of ldtkimport should define LDTK_IMPORT_DEBUG_RULE with the same int value as the one defined in the ldtkimport's Solution.props file,
/// or else you'll get an immediate Segmentation Fault on running the executable.
/// Unfortunately, there's no easy way to make a solution-wide define in Visual Studio.
/// The Solution.props file is there to make it a little bit easier, but
/// to make use of it, you'll still have to manually edit the .vcxproj file.
#if !defined(NDEBUG) && defined(LDTK_IMPORT_DEBUG_RULE) && LDTK_IMPORT_DEBUG_RULE > 0
struct DebugMatchCell
{
   int x;
   int y;
   uint8_t flags;
   std::string extra;
};
#endif

/**
 *  @brief Specifies what tile/s to draw for cells that match a specific pattern.
 *
 *  @details Each cells has an IntGridValue, and a Rule looks for a specific combination
 *  of these IntGridValues in a particular shape.
 *
 *  @see Documentation at https://ldtk.io/json/#ldtk-AutoRuleDef
 */
struct Rule
{
public:

   Rule() :
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
      matchedCells(),
      stampDebugInfo(),
#endif
      uid(0),
      active(true),
      chance(1.0f),
      breakOnMatch(true),
      flipX(false),
      flipY(false),
      xModulo(0),
      xModuloOffset(0),
      yModulo(0),
      yModuloOffset(0),
      checker(CheckerMode::None),
      verticalOutOfBoundsValue(-1),
      horizontalOutOfBoundsValue(-1),
      pattern(),
      patternSize(0),
      tileIds(),
      tileMode(TileMode::Single),
      stampPivotX(0.0f),
      stampPivotY(0.0f),
      stampTileOffsets()
   {
   }

   /**
    *  @brief Apply this Rule for the entire IntGrid, assigning the tiles to draw for each cell.
    *
    *  @param[out] tileGrid All rules that successfuly match will place Tile Id values here.
    *  @param[in] cells The data that indicates what IntGridValue is in each cell.
    *                   These are the values that a rule's pattern is compared against.
    *  @param[in] randomSeed Used when a rule uses random chance.
    *  @param[in] rulePriority The priority of the rule being applied.
    *                          Priority determines whether the tiles applied by the rule should
    *                          visually be on top of other tiles (that are placed by other rules) on the same cell.
    *                          Lower values have higher priority. Starts at 0 (highest priority).
    */
   void applyRule(TileGrid &tileGrid, const IntGrid &cells, const int randomSeed, const uint8_t rulePriority);

   /**
    *  @brief Unique identifier for this rule. Also contributes to the seed in pseudo-random number checks.
    *
    *  @see https://ldtk.io/json/#ldtk-AutoRuleDef;uid
    */
   uid_t uid;

   /**
    *  @brief Whether this rule active or not.
    *  Deactivated rules should be skipped over and not processed.
    *
    *  @see https://ldtk.io/json/#ldtk-AutoRuleDef;active
    */
   bool active;

   /**
    *  @brief When lesser than 1.0, this rule needs to pass a random check
    *  whether it is processed or not.
    *
    *  @see https://ldtk.io/json/#ldtk-AutoRuleDef;chance
    */
   float chance;

   /**
    *  @brief When set to true, that means whenever this rule matches a cell,
    *  it prevents other rules (the ones below, since rules are ordered by priority)
    *  from getting applied to that cell.
    *
    *  That includes rules from other rule groups.
    *  Effectively, this break-on-match applies to the entire layer.
    *
    *  @see https://ldtk.io/json/#ldtk-AutoRuleDef;breakOnMatch
    */
   bool breakOnMatch;

   /**
    *  @brief When set to true, this rule will also be used as a horizontally flipped version.
    *
    *  @see https://ldtk.io/json/#ldtk-AutoRuleDef;flipX
    */
   bool flipX;

   /**
    *  @brief When set to true, this rule will also be used as a vertically flipped version.
    *
    *  @see https://ldtk.io/json/#ldtk-AutoRuleDef;flipY
    */
   bool flipY;

   /**
    *  @brief Check for cells to match at every nth column only.
    *  Value of 1 is default, and effectively checks every column.
    *  Value of 2 means we only check every other column (2, 4, 6, 8, etc.).
    *  Value of 3 means we only check every 3rd column (3, 6, 9, 12, etc.).
    *  And so on.
    *
    *  @see https://ldtk.io/json/#ldtk-AutoRuleDef;xModulo
    */
   int xModulo;

   /**
    *  @brief At what column do we start when checking the xModulo.
    *  The editor doesn't allow values in here to be >= xModulo.
    *  Default value is 0 (immediately start at 1st column).
    *
    *  @see https://ldtk.io/json/#ldtk-AutoRuleDef;xOffset
    */
   int xModuloOffset;

   /**
    *  @brief Check for cells to match at every nth row only.
    *  Value of 1 is default, and effectively checks every row.
    *  Value of 2 means we only check every other row (2, 4, 6, 8, etc.).
    *  Value of 3 means we only check every 3rd row (3, 6, 9, 12, etc.).
    *  And so on.
    *
    *  @see https://ldtk.io/json/#ldtk-AutoRuleDef;yModulo
    */
   int yModulo;

   /**
    *  @brief At what row do we start when checking the yModulo.
    *  The editor doesn't allow values in here to be >= yModulo.
    *  Default value is 0 (immediately start at 1st row).
    *
    *  @see https://ldtk.io/json/#ldtk-AutoRuleDef;yOffset
    */
   int yModuloOffset;

   enum class CheckerMode
   {
      None,
      Horizontal,
      Vertical
   };

   /**
    *  @brief Offset every other cell to check for.
    *
    *  Values can be:
    *  CheckerMode::None
    *  CheckerMode::Horizontal
    *  CheckerMode::Vertical
    *
    *  When Checker Mode isn't None, we ignore xModuloOffset and/or yModuloOffset.
    *
    *  @see https://ldtk.io/json/#ldtk-AutoRuleDef;checker
    */
   CheckerMode checker;

   /**
    *  @brief When pattern checking for cells that are outside
    *  the boundaries of the level, use this IntGridValue.
    *
    *  A value of 0 means regard the outside of the level
    *  as empty cells (no IntGridValue).
    *
    *  A value of 1 means to regard the outside of the level
    *  as having been marked with IntGridValue of 1.
    *
    *  And so on.
    *
    *  Default value is -1, which means to abort trying
    *  to check a cell if anything in the pattern is
    *  outside the boundaries.
    *
    *  @see https://ldtk.io/json/#ldtk-AutoRuleDef;outOfBoundsValue
    */
   int verticalOutOfBoundsValue;

   /**
    *  @brief Experimental separate out-of-bounds value in the horizontal axis.
    *  Right now, this is always assigned the same value as verticalOutOfBoundsValue,
    *  so it will behave as before.
    */
   int horizontalOutOfBoundsValue;

   /**
    *  @brief 2d grid of values specifying what the surrounding
    *  cells need to be, for this rule to match.
    *
    *  @details This 2d grid is stored as a one-dimensional array.
    *  It's square-shaped (width is equal to height).
    *  The size of the 2d grid is specified in patternSize.
    *  But you can derive it based on the fact that the
    *  width and height is the same, so:
    *
    *  | pattern length | pattern size  |
    *  |----------------|---------------|
    *  | 1              | 1             |
    *  | 9              | 3 (as in 3x3) |
    *  | 25             | 5 (as in 5x5) |
    *  | 49             | 7 (as in 7x7) |
    *
    *  Values are IntGridValues.
    *  For example, if an IntGridValue of 1 represents the floor,
    *  then having a value of 1 somewhere in the pattern means the cell in that
    *  position needs to have been marked as a floor, for this rule to match.
    *
    *  When the value is negative, it means the opposite.
    *  Again with the floor example, a value of -1 means it requires that the
    *  cell in that position is not marked as a floor (but it's fine if it's marked
    *  as something else).
    *
    *  A value of 1,000,001 (one million one) is special, it means "Anything"
    *  (there must be an intgrid value in this cell, any kind will do).
    *  You can use the const RULE_PATTERN_ANYTHING for this.
    *
    *  Likewise with the negative, -1000001 means "Nothing"
    *  (there must not be any intgrid value in this cell at all,
    *  or you can think of it as "this cell should be an empty space").
    *  You can use the const RULE_PATTERN_NOTHING for this.
    *
    *  A value of 0 means we don't care about the cell at that position.
    *
    *  So for example, a 3x3 pattern might look like this:
    *
    *  0,  1,  0
    *  1, -1,  1
    *  0,  1,  0
    *
    *  Which can be read as follows:
    *
    *  Northwest: ignore
    *  North: needs to be 1
    *  Northeast: ignore
    *  West: needs to be 1
    *  Center (where tile would be placed on): can be any value except 1
    *  East: needs to be 1
    *  Southwest: ignore
    *  South: needs to be 1
    *  Southeast: ignore
    *
    *  @see https://ldtk.io/json/#ldtk-AutoRuleDef;pattern
    */
   std::vector<pattern_t> pattern;

   /**
    *  @brief Width and height of the pattern's grid.
    *  As of LDtk 1.2.5, values can be 1, 3, 5, or 7.
    *
    *  @see https://ldtk.io/json/#ldtk-AutoRuleDef;size
    */
   uint8_t patternSize;

   /**
    *  @brief Which tile(s) to draw for this pattern.
    *
    *  Each value is an index to a tile inside a tilesheet.
    *  Upper-left corner of the tilesheet has the index 0,
    *  the tile next to it is 1, and so on.
    *  The index ordering in the tilesheet goes from left-to-right,
    *  and top-to-bottom.
    *
    *  Exactly which tilesheet is used is determined by the tilesetDefUid
    *  of the layer that owns this rule.
    *
    *  @see https://ldtk.io/json/#ldtk-AutoRuleDef;tileIds
    */
   std::vector<tileid_t> tileIds;

   enum class TileMode
   {
      Single,
      Stamp
   };

   /**
    *  @brief Determines how the tiles are displayed.
    *
    *  Values can be:
    *  TileMode::Single
    *  TileMode::Stamp
    *
    *  Single: This rule will place only one tile into the cell that it matches.
    *          If there are many tiles specified in tileIds, then one is chosen at random.
    *
    *  Stamp: This rule will place all tiles specified in tileIds into the cell that it matches.
    *         Use stampPivotX and stampPivotY to know how exactly the tiles will be positioned, relative to the cell.
    *         The shape of how these tiles are placed in the cell is determined by their
    *         original shape as they were in the tilesheet.
    *         Example: If the tiles specified are all adjacent together and look like they combine into
    *         a 2x3 rectangle inside the tilesheet, then they will be placed into the level as a 2x3 rectangle as well.
    *
    *  @see https://ldtk.io/json/#ldtk-AutoRuleDef;tileMode
    */
   TileMode tileMode;

   /**
    *  @brief When tileMode is Stamp, this is the X-position adjustment on
    *  where tiles will be placed, relative to the matching cell.
    *
    *  This is a normalized range of 0.0 to 1.0.
    *  Though any arbitrary value between 0.0 to 1.0 would be valid,
    *  the editor only allows assigning the values 0.0, 0.5, and 1.0.
    *
    *  0.0: Tiles will be placed such that its left edge is on the cell.
    *  0.5: Tiles will be placed such that it will be centered on the cell.
    *  1.0: Tiles will be placed such that its right edge is on the cell.
    *
    *  @see https://ldtk.io/json/#ldtk-AutoRuleDef;pivotX
    */
   float stampPivotX;

   /**
    *  @brief When tileMode is Stamp, this is the Y-position adjustment on
    *  where tiles will be placed, relative to the matching cell.
    *
    *  This is a normalized range of 0.0 to 1.0.
    *  Though any arbitrary value between 0.0 to 1.0 would be valid,
    *  the editor only allows assigning the values 0.0, 0.5, and 1.0.
    *
    *  0.0: Tiles will be placed such that its top edge is on the cell.
    *  0.5: Tiles will be placed such that it will be centered on the cell.
    *  1.0: Tiles will be placed such that its bottom edge is on the cell.
    *
    *  @see https://ldtk.io/json/#ldtk-AutoRuleDef;pivotY
    */
   float stampPivotY;

   /**
    *  @brief Cached values of where a tile is positioned when the Rule uses TILE_MODE_STAMP.
    */
   struct Offset
   {
      /**
       *  @brief Offset to the x position. This is relative to the left edge of the tile stamp as a whole.
       */
      int16_t x;

      /**
       *  @brief Offset to the y position. This is relative to the top edge of the tile stamp as a whole.
       */
      int16_t y;

      /**
       *  @brief Flags for whether tile has offset x/y.
       *  Offsets occur when the tile stamp has even-numbered width and/or height,
       *  and is being positioned at the center of its cell (pivot set to 0.5).
       */
      uint8_t flags;

      bool hasAnyOffset()
      {
         return tile::hasOffsetLeft(flags) || tile::hasOffsetUp(flags);
      }
   };

   /**
    *  @brief Cached values of where each tile in tileIds is positioned when the Rule uses TILE_MODE_STAMP.
    *
    *  This is computed once, when the LdtkDefFile is loaded. If we ever implement rule editing in runtime,
    *  then this would also need to be recomputed every time the rule is edited.
    */
   std::vector<Offset> stampTileOffsets;

#if !defined(NDEBUG) && defined(LDTK_IMPORT_DEBUG_RULE) && LDTK_IMPORT_DEBUG_RULE > 0
   /**
    *  @brief Only used for debugging. Lists all cells that this Rule matched.
    */
   std::vector<DebugMatchCell> matchedCells;

   /**
    *  @brief Only used for debugging. Log of what was stored in the stamp cache calculation.
    */
   std::string stampDebugInfo;
#endif


   friend std::ostream &operator<<(std::ostream &os, const Rule &rule);

private:

   /**
    *  @brief Check if this Rule matches the given cell coordinates.
    *  @param[out] debugLog Only used for debugging. The Rule will log what happened in the matching process here.
    *  @param[in] cells The data that indicates what IntGridValue is in each cell.
    *                   These are the values that a rule's pattern is compared against.
    *  @param[in] cellX X-coordinate of the cell we're checking a match for.
    *  @param[in] cellY Y-coordinate of the cell we're checking a match for.
    *  @param[in] directionX Set to -1 if Rule needs to be checked as a horizontally flipped version. Set to 1 if not. Value should only ever be 1 or -1.
    *  @param[in] directionY Set to -1 if Rule needs to be checked as a vertically flipped version. Set to 1 if not. Value should only ever be 1 or -1.
    *  @param[in] randomSeed Used when a rule uses random chance.
    *  @return true if the cell with specified X and Y coordinates are a match for this Rule.
   */
   bool matchesCell(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
      std::ostream &debugLog,
#endif
      const IntGrid &cells, const int cellX, const int cellY, const int8_t directionX, const int8_t directionY, const int randomSeed);

   /**
    *  @brief Check if this Rule matches the given cell coordinates.
    *  It will also properly check modulo, checker, and the flipped versions of the Rule if needed.
    *
    *  @param[in] cells The data that indicates what IntGridValue is in each cell.
    *                   These are the values that a rule's pattern is compared against.
    *  @param[in] cellX X-coordinate of the cell we're checking a match for.
    *  @param[in] cellY Y-coordinate of the cell we're checking a match for.
    *  @param[in] randomSeed Used when a rule uses random chance.
    *  @return Bitflags indicating whether this Rule matched the cell with specified X and Y coordinates.
    *          The flag will also indicate if it was the horizontally and/or vertically flipped version
    *          of the Rule that matched, if ever.
    */
   int8_t passesRule(const IntGrid &cells, const int cellX, const int cellY, const int randomSeed);
};

inline std::ostream &operator<<(std::ostream &os, const Rule &rule)
{
   os << "Uid: " << rule.uid << std::endl;
   os << "Active: " << toYesNo(rule.active) << std::endl;
   os << "Chance: " << rule.chance << std::endl;
   os << "Break on match: " << toYesNo(rule.breakOnMatch) << std::endl;
   os << "Flip X: " << toYesNo(rule.flipX) << std::endl;
   os << "Flip Y: " << toYesNo(rule.flipY) << std::endl;
   os << "Modulo X: " << rule.xModulo << std::endl;
   os << "Modulo Y: " << rule.yModulo << std::endl;
   os << "Modulo X Offset: " << rule.xModuloOffset << std::endl;
   os << "Modulo Y Offset: " << rule.yModuloOffset << std::endl;
   switch (rule.checker)
   {
      case Rule::CheckerMode::None:
         os << "Checker: None" << std::endl;
      break;
      case Rule::CheckerMode::Horizontal:
         os << "Checker: Horizontal" << std::endl;
      break;
      case Rule::CheckerMode::Vertical:
         os << "Checker: Vertical" << std::endl;
      break;
   }
   os << "Out-of-bounds: " << rule.verticalOutOfBoundsValue << std::endl;
   switch(rule.tileMode)
   {
      case Rule::TileMode::Single:
         os << "Tile Mode: Single" << std::endl;
      break;
      case Rule::TileMode::Stamp:
         os << "Tile Mode: Stamp" << std::endl;
      break;
   }
   os << "Stamp Pivot X: " << rule.stampPivotX << std::endl;
   os << "Stamp Pivot Y: " << rule.stampPivotY << std::endl;

   os << "Pattern:" << std::endl;
   os << "  ";
   int8_t c = 0;
   for (size_t n = 0, len = rule.pattern.size(); n < len; ++n)
   {
      if (rule.pattern[n] == RULE_PATTERN_ANYTHING)
      {
         os << " *, ";
      }
      else if (rule.pattern[n] == RULE_PATTERN_NOTHING)
      {
         os << "-*, ";
      }
      else
      {
         os << std::setw(2) << rule.pattern[n] << ", ";
      }
      ++c;
      if (c == rule.patternSize)
      {
         if (n != len - 1)
         {
            os << std::endl;
            os << "  ";
            c = 0;
         }
         else
         {
            os << std::endl;
         }
      }
   }

   os << "TileId: [";
   for (size_t n = 0, len = rule.tileIds.size(); n < len; ++n)
   {
      os << rule.tileIds[n];
      if (n < len - 1)
      {
         os << ", ";
      }
   }
   os << "]" << std::endl;

#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0

   if (!rule.stampDebugInfo.empty())
   {
      os << rule.stampDebugInfo;
   }

   os << "Matched: " << rule.matchedCells.size() << std::endl;
   for (auto matched = rule.matchedCells.cbegin(), matchedEnd = rule.matchedCells.cend(); matched != matchedEnd; ++matched)
   {
      os << "(" << matched->x << ", " << matched->y << ") ";
      if (tile::hasOffsetLeft(matched->flags))
      {
         os << "OffsetLeft ";
      }
      if (tile::hasOffsetUp(matched->flags))
      {
         os << "OffsetUp ";
      }
      if (tile::isFlippedX(matched->flags))
      {
         os << "FlippedX ";
      }
      if (tile::isFlippedY(matched->flags))
      {
         os << "FlippedY ";
      }
      if (tile::isFinal(matched->flags))
      {
         os << "Final ";
      }
      os << matched->extra << std::endl;
   }
#endif
   return os;
}

} // namespace ldtkimport

#endif // LDTK_IMPORT_RULE_H
