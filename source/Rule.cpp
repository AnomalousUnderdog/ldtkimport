#include "ldtkimport/Rule.h"

#include <string>
#include <vector>
#include <iostream>

#include "ldtkimport/AssertUtility.h"
#include "ldtkimport/GridUtility.h"
#include "ldtkimport/LdtkDefFile.h"
#include "ldtkimport/IntGrid.h"


namespace ldtkimport
{

const int16_t CHANCE_MAX = 100;

bool Rule::matchesCell(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
   std::ostream &debugLog,
#endif
   const IntGrid &cells, const int cellX, const int cellY, const int8_t directionX, const int8_t directionY, const int randomSeed) const
{
   // based on https://github.com/deepnight/ldtk/blob/08b91171913fe816c6ad8a09630c586ad63e174b/src/electron.renderer/data/def/AutoLayerRuleDef.hx#L248

   // Note: Rules with chance <= 0 have already been filtered out so there's no need to check it again here

   if (chance < 1.0f)
   {
      int16_t chance100 = static_cast<int16_t>(chance * 100);
      if (GridUtility::getRandomIndex(randomSeed + uid, cellX, cellY, CHANCE_MAX) >= chance100)
      {
         return false;
      }
   }

   /// @todo check perlin noise data here

   // radius serves as an offset so that when px = 0 (in the for loop below),
   // we start with checking the cell that is to the left of the cell we're trying to match
   uint8_t radius = patternSize / 2;

   for (uint8_t py = 0; py < patternSize; ++py)
   {
      for (uint8_t px = 0; px < patternSize; ++px)
      {
         // Pattern sizes are small enough that they will fit inside 8-bit ints.
         // The largest pattern's max idx value would be 48 (for a 7x7 pattern).
         uint8_t patternIdx = px + (py * patternSize);

         auto patternValue = pattern[patternIdx];
         if (patternValue == 0)
         {
            // pattern doesn't care about this cell, skip it
            continue;
         }

         // Translate the pattern coordinates into coordinates for the IntGrid.
         // Note: When checking for the flipped version of the pattern,
         // we don't actually flip the pattern, instead we flip the way
         // we look at the IntGrid. That's what the directionX/Y parameters are for.
         int checkX = cellX + ((px - radius) * directionX);
         int checkY = cellY + ((py - radius) * directionY);

#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
         debugLog << "checking cell: (" << checkX << ", " << checkY << ") ";
#endif

         intgridvalue_t intGridValue;
         bool withinHorizontal = cells.isWithinHorizontalBounds(checkX);
         bool withinVertical = cells.isWithinVerticalBounds(checkY);
         if (withinHorizontal && withinVertical)
         {
            intGridValue = cells(checkX, checkY);
         }
         else
         {
            if (!withinHorizontal && withinVertical)
            {
               // IntGrid coordinates are outside boundaries,
               // but horizontally only (to the left or right)
               if (horizontalOutOfBoundsValue == -1)
               {
                  // this means we don't care about this cell,
                  // since one of the pattern checks fall outside the grid boundaries
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
                  debugLog << "horizontal out-of-bounds require we ignore this cell";
#endif
                  return false;
               }

               intGridValue = horizontalOutOfBoundsValue;
            }
            else
            {
               // IntGrid coordinates are outside boundaries diagonally, or
               // outside boundaries but vertically only (above or below)
               if (verticalOutOfBoundsValue == -1)
               {
                  // this means we don't care about this cell,
                  // since one of the pattern checks fall outside the grid boundaries
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
                  debugLog << "vertical out-of-bounds require we ignore this cell";
#endif
                  return false;
               }

               intGridValue = verticalOutOfBoundsValue;
            }
         }

#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
         debugLog << "IntGridValue: " << intGridValue << " ";
#endif

         if (patternValue == RULE_PATTERN_ANYTHING && intGridValue == 0)
         {
            // we require anything to be in the cell, but the cell is empty
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
            debugLog << "(failed) pattern required any value here";
#endif
            return false;
         }
         else if (patternValue == RULE_PATTERN_NOTHING && intGridValue != 0)
         {
            // we require the cell to be empty but something is in there
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
            debugLog << "(failed) pattern required no value here";
#endif
            return false;
         }
         else if (patternValue > 0 && patternValue != RULE_PATTERN_ANYTHING && intGridValue != patternValue)
         {
            // we require the cell to have a specific IntGridValue, but the cell doesn't have that specific one
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
            debugLog << "(failed) pattern required " << patternValue;
#endif
            return false;
         }
         else if (intGridValue == -patternValue) // safe to assume patternValue < 0
         {
            // (a negative pattern value represents "any value is fine here as long as it's not that specific one")
            // we require the cell to NOT have a specific IntGridValue, but the cell has it
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
            debugLog << "(failed) pattern required: not " << -patternValue;
#endif
            return false;
         }
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
         else
         {
            debugLog << "(passed)";
         }
         debugLog << std::endl;
#endif
      }
   }

   // passed all checks
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
   debugLog << "passed";
#endif
   return true;
}

// -----------------------------------------------------------------------------------------------------

namespace RuleResult
{

/**
 *  @brief Return value given by passesRule to indicate that the Rule did not match.
 */
static const int8_t Fail = -1;

/**
 *  @brief Return value given by passesRule to indicate that the non-flipped version of the Rule matched.
 */
static const int8_t Success = 0;

}

// -----------------------------------------------------------------------------------------------------

int8_t Rule::passesRule(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
   RuleLog &ruleLog,
#endif
   const IntGrid &cells, const int cellX, const int cellY, const int randomSeed) const
{
   // based on https://github.com/deepnight/ldtk/blob/08b91171913fe816c6ad8a09630c586ad63e174b/src/electron.renderer/data/inst/LayerInstance.hx#L720

   // modulo acts as a filter
   //

   ASSERT_THROW(xModulo != 0 && yModulo != 0, std::logic_error,
      "Modulo to be used as divisor is zero. xModulo: " << xModulo << " yModulo: " << yModulo);

   if (checker != CheckerMode::Vertical && ((cellY - yModuloOffset) % yModulo) != 0)
   {
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
      ruleLog.matchedCells.push_back(DebugMatchCell{ cellX, cellY, 0, std::string("Skipped due to Y Modulo") });
#endif
      return RuleResult::Fail;
   }

   if (checker == CheckerMode::Vertical && ((cellY + ((cellX / xModulo) % 2)) % yModulo) != 0)
   {
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
      ruleLog.matchedCells.push_back(DebugMatchCell{ cellX, cellY, 0, std::string("Skipped due to Checker Y Modulo") });
#endif
      return RuleResult::Fail;
   }

   if (checker != CheckerMode::Horizontal && ((cellX - xModuloOffset) % xModulo) != 0)
   {
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
      ruleLog.matchedCells.push_back(DebugMatchCell{ cellX, cellY, 0, std::string("Skipped due to X Modulo") });
#endif
      return RuleResult::Fail;
   }

   if (checker == CheckerMode::Horizontal && ((cellX + ((cellY / yModulo) % 2)) % xModulo) != 0)
   {
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
      ruleLog.matchedCells.push_back(DebugMatchCell{ cellX, cellY, 0, std::string("Skipped due to Checker X Modulo") });
#endif
      return RuleResult::Fail;
   }

#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
   std::stringstream debugInfo;
#endif

   // now check the rule, we do additional checks if the rule applies flipped versions
   //
   if (matchesCell(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
      debugInfo,
#endif
      cells, cellX, cellY, 1, 1, randomSeed))
   {
      return RuleResult::Success;
   }

   if (flipX && flipY && matchesCell(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
      debugInfo,
#endif
      cells, cellX, cellY, -1, -1, randomSeed))
   {
      return TileFlags::FlippedX | TileFlags::FlippedY;
   }

   if (flipX && matchesCell(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
      debugInfo,
#endif
      cells, cellX, cellY, -1, 1, randomSeed))
   {
      return TileFlags::FlippedX;
   }

   if (flipY && matchesCell(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
      debugInfo,
#endif
      cells, cellX, cellY, 1, -1, randomSeed))
   {
      return TileFlags::FlippedY;
   }

   // no version of the rule pattern matched
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
   ruleLog.matchedCells.push_back(DebugMatchCell{ cellX, cellY, 0, debugInfo.str() });
#endif
   return RuleResult::Fail;
}

// -----------------------------------------------------------------------------------------------------

void Rule::applyRule(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
   RuleLog &ruleLog, RulesLog::RulesInGrid_t &tileGridLog,
#endif
   TileGrid &tileGrid, const IntGrid &cells, const int randomSeed, const dimensions_t cellPixelSize, const uint8_t rulePriority, const uint8_t runSettings) const
{
   if (tileIds.size() == 0)
   {
      // no tile to apply
      return;
   }

#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
   ruleLog.matchedCells.clear();
#endif

   for (int cellY = 0; cellY < cells.getHeight(); ++cellY)
   {
      uint8_t breakOnMatchFlag = breakOnMatch ? TileFlags::Final : TileFlags::NoFlags;

      for (int cellX = 0; cellX < cells.getWidth(); ++cellX)
      {
         if (!tileGrid.canStillPlaceTiles(cellX, cellY))
         {
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
            ruleLog.matchedCells.push_back(DebugMatchCell{ cellX, cellY, 0, "skipping. cell already finalized." });
#endif
            continue;
         }

         // return value can either be a RuleResult,
         // or one of the Flipped values in TileFlags
         int8_t ruleMatchResult = passesRule(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 1
            ruleLog,
#endif
            cells, cellX, cellY, randomSeed);

         if (ruleMatchResult == RuleResult::Fail)
         {
            continue;
         }

#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
         ruleLog.matchedCells.push_back(DebugMatchCell{ cellX, cellY, 0, "success" });
#endif

         // -----------------------------------------------------
         // Compute position offsets

         int cellXOffset = 0;
         int8_t excessPixelPosXOffset = 0;
         int16_t finalXOffset = GridUtility::getRandomIndex(randomSeed + uid, cellX, cellY, randomPosXOffsetMin, randomPosXOffsetMax) + posXOffset;
         if (finalXOffset != 0)
         {
            // convert pixel values to cell values, add that to the locationX,
            // and any excess value will be stored in excessPixelPosXOffset
            cellXOffset = finalXOffset / cellPixelSize;
            excessPixelPosXOffset = finalXOffset % cellPixelSize;
         }

         int cellYOffset = 0;
         int8_t excessPixelPosYOffset = 0;
         int16_t finalYOffset = GridUtility::getRandomIndex(randomSeed + uid + 1, cellX, cellY, randomPosYOffsetMin, randomPosYOffsetMax) + posYOffset;
         if (finalYOffset != 0)
         {
            // convert pixel values to cell values, add that to the locationY,
            // and any excess value will be stored in excessPixelPosYOffset
            cellYOffset = finalYOffset / cellPixelSize;
            excessPixelPosYOffset = finalYOffset % cellPixelSize;
         }

         // -----------------------------------------------------

         switch (tileMode)
         {
            case TileMode::Single:
            {
               int locationX = cellX + cellXOffset;
               int locationY = cellY + cellYOffset;

               if (locationX < 0 || locationX >= cells.getWidth() || locationY < 0 || locationY >= cells.getHeight())
               {
                  // Tile went over the map (probably due to offsets), skip it.
                  // This is fine, since that tile is effectively at off-screen area.
                  break;
               }

               // -----------------------------------------------------

               // choose one tile at random
               tileid_t tileId;
               if (tileIds.size() > 1)
               {
                  tileId = tileIds[GridUtility::getRandomIndex(randomSeed + uid, cellX, cellY, tileIds.size())];
               }
               else
               {
                  tileId = tileIds[0];
               }

               uint8_t flags = static_cast<uint8_t>(ruleMatchResult) | breakOnMatchFlag;

#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
               tileGridLog[GridUtility::getIndex(locationX, locationY, cells.getWidth())].push_back(uid);
#endif

               tileGrid.putTile(tileId, locationX, locationY, excessPixelPosXOffset, excessPixelPosYOffset, opacity, flags, rulePriority);
               break;
            }
            case TileMode::Stamp:
            {
               ASSERT(stampTileOffsets.size() == tileIds.size(),
                  "For Rule " << uid << ", stampTileOffsets size should match tileIds size at this point. stampTileOffsets.size(): " << stampTileOffsets.size() << " tileIds.size(): " << tileIds.size());

               // go through each tile in the stamp
               for (size_t tileIdx = 0, tileLen = tileIds.size(); tileIdx < tileLen; ++tileIdx)
               {
                  const Rule::Offset &offset = stampTileOffsets[tileIdx];

                  int locationX = cellX + cellXOffset + (offset.x * (TileFlags::isFlippedX(ruleMatchResult) ? -1 : 1));
                  int locationY = cellY + cellYOffset + (offset.y * (TileFlags::isFlippedY(ruleMatchResult) ? -1 : 1));

                  if (locationX < 0 || locationX >= cells.getWidth() || locationY < 0 || locationY >= cells.getHeight())
                  {
                     // Tile of stamp went over the map, skip it.
                     // It's ok if part of the stamp is cut-off,
                     // since that part is effectively at off-screen area.
                     continue;
                  }

                  // -----------------------------------------------------

                  uint8_t flags;
                  bool giveBreakOnMatch;
                  if (RunSettings::hasFasterStampBreakOnMatch(runSettings))
                  {
                     // at this point, the offsets don't have any right or down offset so we only specifically check for left or up
                     giveBreakOnMatch = (offset.x == 0 && offset.y == 0) || !offset.hasEitherLeftOrUpOffset();
                  }
                  else
                  {
                     giveBreakOnMatch = (offset.x == 0 && offset.y == 0) && !offset.hasEitherLeftOrUpOffset();
                  }
                  if (giveBreakOnMatch)
                  {
                     /// @todo to properly implement breakOnMatch for tiles that are not exactly on the matched cell,
                     /// we'll need to check if there are no more transparent areas left in the cell
                     flags = static_cast<uint8_t>(ruleMatchResult) | offset.flags | breakOnMatchFlag;
                  }
                  else
                  {
                     // do not finalize for cells that aren't the current one
                     flags = static_cast<uint8_t>(ruleMatchResult) | offset.flags;
                  }

                  // If we have left offset, check if (locationX-1, locationY) has a higher priority rule placed on it.
                  // If so, we need to move the tile there and switch the left offset to a right offset.
                  // Visually, it will be in the same position, it's just that we need to do this to properly
                  // enforce z-order, so that a higher priority rule shows up on top of this rule.
                  if (TileFlags::hasOffsetLeft(flags) && locationX > 0 && tileGrid.getHighestPriority(locationX - 1, locationY) < rulePriority)
                  {
                     --locationX;
                     flags &= ~TileFlags::LeftOffset;
                     flags |= TileFlags::RightOffset;
                  }

                  // Do the same in the Y-axis.
                  if (TileFlags::hasOffsetUp(flags) && locationY > 0 && tileGrid.getHighestPriority(locationX, locationY - 1) < rulePriority)
                  {
                     --locationY;
                     flags &= ~TileFlags::UpOffset;
                     flags |= TileFlags::DownOffset;
                  }

#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
                  tileGridLog[GridUtility::getIndex(locationX, locationY, cells.getWidth())].push_back(uid);
#endif

                  tileGrid.putTile(tileIds[tileIdx], locationX, locationY, excessPixelPosXOffset, excessPixelPosYOffset, opacity, flags, rulePriority);
               } // for tileId
               break;
            }
            default:
            {
               ASSERT_THROW(false, std::runtime_error, "For Rule " << uid << ", unknown tileMode property. ");
               break;
            }
         } // switch tileMode
      } // for cellX
   } // for cellY
}

} // namespace ldtkimport
