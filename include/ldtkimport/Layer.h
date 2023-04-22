#ifndef LDTK_IMPORT_LAYER_H
#define LDTK_IMPORT_LAYER_H

#include <cstdint>
#include <string>
#include <vector>

#include "ldtkimport/IntGridValue.h"
#include "ldtkimport/RuleGroup.h"
#include "ldtkimport/TileGrid.h"


namespace ldtkimport
{

/**
 *  @brief https://ldtk.io/json/#ldtk-LayerDefJson
 */
struct Layer
{
   Layer() :
      name(),
      uid(0),
      gridSize(0),
      tilesetDefUid(0),
      useAutoSourceLayerDefUid(false),
      autoSourceLayerDefUid(-1),
      randomSeed(0),
      intGridValues(),
      ruleGroups()
   {
   }

   /**
    *  @brief User-defined name of the layer.
    *
    *  @see https://ldtk.io/json/#ldtk-LayerDefJson;identifier
    */
   std::string name;

   /**
    *  @brief Automatically assigned unique-identifier for this layer definition.
    *  Used when other parts of the data need to refer to this layer.
    *
    *  @see https://ldtk.io/json/#ldtk-LayerDefJson;uid
    */
   uid_t uid;

   /**
    *  @brief How many pixels (in both width and height)
    *  each tile should be displayed.
    *
    *  @see https://ldtk.io/json/#ldtk-LayerDefJson;gridSize
    */
   dimensions_t gridSize;

   /**
    *  @brief Which tilesheet to use for this layer, referred to by their unique identifier.
    *  All rules under this layer will then use the specified tilesheet.
    *  Corresponds to LdtkDefFile::TileSet::uid.
    *
    *  @see https://ldtk.io/json/#ldtk-LayerDefJson;tilesetDefUid
    */
   uid_t tilesetDefUid;

   /**
    *  @brief Whether this layer uses its own intGridValues,
    *  or the intGridValues of another layer.
    *  If set to true, use autoSourceLayerDefUid to get the layer uid.
    */
   bool useAutoSourceLayerDefUid;

   /**
    *  @brief If this layer doesn't have its own intGridValues,
    *  then the intGridValues of the layer specified here is used, using the layer's uid.
    *
    *  @see https://ldtk.io/json/#ldtk-LayerDefJson;autoSourceLayerDefUid
    */
   uid_t autoSourceLayerDefUid;

   /**
    *  @brief Random seed assigned to the layer.
    *
    *  @see https://ldtk.io/json/#ldtk-LayerInstanceJson;seed
    */
   uint32_t randomSeed;

   /**
    *  @see https://ldtk.io/json/#ldtk-LayerDefJson;intGridValues
    */
   std::vector<IntGridValue> intGridValues;

   /**
    *  @brief All the rules in this layer, separated into groups.
    *
    *  @see https://ldtk.io/json/#ldtk-LayerDefJson;autoRuleGroups
    */
   std::vector<RuleGroup> ruleGroups;
};

} // namespace ldtkimport

#endif // LDTK_IMPORT_LAYER_H
