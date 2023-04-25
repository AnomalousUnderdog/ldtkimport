#ifndef LDTK_IMPORT_LDTK_DEF_FILE_H
#define LDTK_IMPORT_LDTK_DEF_FILE_H

#include <string>
#include <vector>
#include <ostream>

#include "ldtkimport/MiscUtility.h"
#include "ldtkimport/Color.h"
#include "ldtkimport/IntGridValue.h"
#include "ldtkimport/IntGrid.h"
#include "ldtkimport/Layer.h"
#include "ldtkimport/RuleGroup.h"
#include "ldtkimport/TileSet.h"
#include "ldtkimport/Level.h"


namespace ldtkimport
{

/**
 *  @brief Main class that holds together the definitions part of an LDtk file.
 *
 *  @see https://ldtk.io/json/#ldtk-DefinitionsJson
 */
class LdtkDefFile
{
public:

   LdtkDefFile() :
      m_filename(),
      m_projectUniqueId(),
      m_fileVersion(),
      m_bgColor(),
      m_bgColor8(),
      m_bgColorf(),
      m_layers(),
      m_tilesets()
   {
   }

   /**
    *  @brief Populate this LdtkDefFile with values coming from the passed json text (ldtk files are actually just json).
    *
    *  @param[in] ldtkText Actual json text content of the Ldtk file.
    *  @param[in] textLength Length of json text passed.
    *  @param[in] loadDeactivatedContent Whether or not to load deactivated RuleGroups and Rules inside
    *                                    the file. Some level designers have tests/experiments that they
    *                                    keep deactivated, so you might not want to bother with those.
    *  @param[in] filename Filename of the Ldtk file to load. Not strictly needed,
    *                      only used for debugging/informational purposes.
    */
   void loadFromText(const char *ldtkText, size_t textLength, bool loadDeactivatedContent, const char *filename);

   /**
    *  @brief Populate this LdtkDefFile with values coming from the ldtk file specified.
    *
    *  @param[in] ldtkFile Path and filename to the ldtk file you want to be loaded.
    *  @param[in] loadDeactivatedContent Whether or not to load deactivated RuleGroups and Rules inside
    *                                    the file. Some level designers have tests/experiments that they
    *                                    keep deactivated, so you might not want to bother with those.
    */
   void loadFromFile(const char *ldtkFile, bool loadDeactivatedContent);

   /**
    *  @brief Prints the contents of the LdtkDefFile to the out stream.
    *  Use std::cout to print it immediately, or a std::ostringstream if you want it as a string.
    */
   friend std::ostream &operator<<(std::ostream &os, const LdtkDefFile &ldtkFile);

   /**
    *  @brief Prints the contents of a particular Rule to the out stream.
    *  Use std::cout to print it immediately, or a std::ostringstream if you want it as a string.
    */
   void debugPrintRule(std::ostream &outStream, int ruleUid);

   /**
    *  @brief Populate a level's TileGrids by letting this LdtkDefFile run its Rules through it.
    *
    *  @param[out] level Where output of rule matching process is placed onto.
    *  @param[in] randomizeSeed Set to true to give a new random seed to each layer,
    *                           creating a new variation for the randomized parts.
    */
   void generate(Level &level, bool randomizeSeed = false);

   /**
    *  @brief Populate a layer of a level's TileGrids by letting this LdtkDefFile run its Rules through it.
    *
    *  @param[out] level Where output of rule matching process is placed onto.
    *  @param[in] layerIdx Which layer's rules to run.
    *  @param[in] randomizeSeed Set to true to give a new random seed to each layer,
    *                           creating a new variation for the randomized parts.
    */
   void generate(Level &level, size_t layerIdx, bool randomizeSeed = false);

   /**
    *  @brief Find a TileSet with the given unique id.
    *  @param[in] tilesetDefUid Unique id of the TileSet to get.
    *  @param[out] result Where the found TileSet will be in, if any. Use the return value to check.
    *  @return true means the Layer was found, false if not.
    */
   bool getTileset(int tilesetDefUid, TileSet *&result);

   /**
    *  @brief Find a TileSet with the given unique id (const version).
    *  @param[in] tilesetDefUid Unique id of the TileSet to get.
    *  @param[out] result Where the found TileSet will be in, if any. Use the return value to check.
    *  @return true means the Layer was found, false if not.
    */
   bool getTileset(int tilesetDefUid, const TileSet *&result) const;

   // ---------------------------------------------------------------------
   // Note: These next set of functions are used for debugging, to
   // browse through the rules that the LdtkDefFile has.
   // By going through each Layer, each RuleGroup, each Rule,
   // and each TileId used by the Rule.

   size_t getLayerCount()
   {
      return m_layers.size();
   }

   const Layer &getLayer(int layerIdx)
   {
      return m_layers[layerIdx];
   }

   size_t getRuleGroupCount(int layerIdx)
   {
      if (layerIdx < 0 || layerIdx >= m_layers.size())
      {
         return 0;
      }
      return m_layers[layerIdx].ruleGroups.size();
   }

   size_t getRuleCount(int layerIdx, int ruleGroupIdx)
   {
      if (layerIdx < 0 || layerIdx >= m_layers.size())
      {
         return 0;
      }
      if (ruleGroupIdx < 0 || ruleGroupIdx >= m_layers[layerIdx].ruleGroups.size())
      {
         return 0;
      }
      return m_layers[layerIdx].ruleGroups[ruleGroupIdx].rules.size();
   }

   size_t getRuleTileIdCount(int layerIdx, int ruleGroupIdx, int ruleIdx)
   {
      if (layerIdx < 0 || layerIdx >= m_layers.size())
      {
         return 0;
      }
      if (ruleGroupIdx < 0 || ruleGroupIdx >= m_layers[layerIdx].ruleGroups.size())
      {
         return 0;
      }
      if (ruleIdx < 0 || ruleIdx >= m_layers[layerIdx].ruleGroups[ruleGroupIdx].rules.size())
      {
         return 0;
      }

      return m_layers[layerIdx].ruleGroups[ruleGroupIdx].rules[ruleIdx].tileIds.size();
   }

   const char *getLayerName(int layerIdx)
   {
      if (layerIdx < 0 || layerIdx >= m_layers.size())
      {
         return "";
      }

      return m_layers[layerIdx].name.c_str();
   }

   const char *getRuleGroupName(int layerIdx, int ruleGroupIdx)
   {
      if (layerIdx < 0 || layerIdx >= m_layers.size())
      {
         return "";
      }
      if (ruleGroupIdx < 0 || ruleGroupIdx >= m_layers[layerIdx].ruleGroups.size())
      {
         return "";
      }

      return m_layers[layerIdx].ruleGroups[ruleGroupIdx].name.c_str();
   }

   std::pair<bool, uid_t> getRuleUid(int layerIdx, int ruleGroupIdx, int ruleIdx)
   {
      if (layerIdx < 0 || layerIdx >= m_layers.size())
      {
         return std::make_pair(false, 0);
      }
      if (ruleGroupIdx < 0 || ruleGroupIdx >= m_layers[layerIdx].ruleGroups.size())
      {
         return std::make_pair(false, 0);
      }
      if (ruleIdx < 0 || ruleIdx >= m_layers[layerIdx].ruleGroups[ruleGroupIdx].rules.size())
      {
         return std::make_pair(false, 0);
      }

      uid_t ruleUid = m_layers[layerIdx].ruleGroups[ruleGroupIdx].rules[ruleIdx].uid;

      return std::make_pair(true, ruleUid);
   }

   std::pair<bool, tileid_t> getRuleTileId(int layerIdx, int ruleGroupIdx, int ruleIdx, int tileIdIdx)
   {
      if (layerIdx < 0 || layerIdx >= m_layers.size())
      {
         return std::make_pair(false, 0);
      }
      if (ruleGroupIdx < 0 || ruleGroupIdx >= m_layers[layerIdx].ruleGroups.size())
      {
         return std::make_pair(false, 0);
      }
      if (ruleIdx < 0 || ruleIdx >= m_layers[layerIdx].ruleGroups[ruleGroupIdx].rules.size())
      {
         return std::make_pair(false, 0);
      }
      if (tileIdIdx < 0 || tileIdIdx >= m_layers[layerIdx].ruleGroups[ruleGroupIdx].rules[ruleIdx].tileIds.size())
      {
         return std::make_pair(false, 0);
      }

      tileid_t tileId = m_layers[layerIdx].ruleGroups[ruleGroupIdx].rules[ruleIdx].tileIds[tileIdIdx];

      return std::make_pair(true, tileId);
   }

   // ---------------------------------------------------------------------

   const Color8 &getBgColor8() const
   {
      return m_bgColor8;
   }

   const Colorf &getBgColorf() const
   {
      return m_bgColorf;
   }

private:
   // ---------------------------------------------------------------------

   using layers_t = std::vector<Layer>;
   using tilesets_t = std::vector<TileSet>;

   /**
    *  @brief Filename of the LdtkDefFile that was loaded.
    *  Not strictly needed, only for debugging/informational purposes.
    */
   std::string m_filename;

   /**
    *  @brief https://ldtk.io/json/#ldtk-ProjectJson;iid
    */
   std::string m_projectUniqueId;

   /**
    *  @brief https://ldtk.io/json/#ldtk-ProjectJson;jsonVersion
    */
   std::string m_fileVersion;

   /**
    *  @brief Normally, Background Color resides in the LdtkDefFile's "Level" section.
    *  But since we dynamically generate our own levels, we don't bother storing levels from the file.
    *  The problem is that level designers usually assign the Bg Color from the level,
    *  so we store the first level's Bg Color here.
    */
   std::string m_bgColor;

   /**
    *  @brief Color8 (8-bit integers) representation of the Bg Color.
    *  @see m_bgColor
    */
   Color8 m_bgColor8;

   /**
    *  @brief Colorf (float with values 0.0 to 1.0) representation of the Bg Color.
    *  @see m_bgColor
    */
   Colorf m_bgColorf;

   /**
    *  @brief List of all layers in the file.
    *  The order here is the z-order when drawn (first layer should be visually on top).
    *
    *  @see https://ldtk.io/json/#ldtk-DefinitionsJson;layers
    */
   layers_t m_layers;

   /**
    *  @brief Info on the images used for the tiles.
    *
    *  @see https://ldtk.io/json/#ldtk-DefinitionsJson;tilesets
    */
   tilesets_t m_tilesets;

   /**
    *  @brief Assigns the random seed property to a layer definition.
    *
    *  Normally, the random seed is stored in the level's layer instances.
    *  But since we dynamically generate levels, we don't bother with
    *  layer instances.
    *
    *  So loadFromFile() will still parse through layer instances,
    *  and we use this function to store the random seed for the layer.
    *
    *  @param[in] layerDefUid Unique id of the Layer to edit.
    *  @param[in] newInitialSeed Random seed value for the Layer.
    */
   void setLayerInitialSeed(int layerDefUid, int newInitialSeed);

   /**
    *  @brief Find a Layer with the given unique id.
    *  @param[in] layerDefUid Unique id of the Layer to get.
    *  @param[out] result Where the found Layer will be in, if any. Use the return value to check.
    *  @return true means the Layer was found, false if not.
    */
   bool getLayer(int layerDefUid, Layer *&result);

   /**
    *  @brief This computes certain values that will be cached, so that
    *  they wouldn't have to be computed over and over every time you generate a new level.
    *  Particularly, this caches the offsets for each tile to be used in a tile stamp.
    *
    *  This is automatically called in loadFromText.
    *
    *  @param[in] preProcessDeactivatedContent Whether or not we also compute cache for deactivated
    *  RuleGroups and Rules. Some level designers have tests/experiments that they keep
    *  deactivated, so you might not want to bother with those.
    */
   void preProcess(bool preProcessDeactivatedContent = false);

public:

   // ---------------------------------------------------------------------
   // Functions to iterate through layers

   using layer_iterator = layers_t::iterator;
   using const_layer_iterator = layers_t::const_iterator;
   using const_reverse_layer_iterator = layers_t::const_reverse_iterator;

   layer_iterator layerBegin()
   {
      return m_layers.begin();
   }
   layer_iterator layerEnd()
   {
      return m_layers.end();
   }

   const_layer_iterator layerBegin() const
   {
      return m_layers.begin();
   }
   const_layer_iterator layerEnd() const
   {
      return m_layers.end();
   }

   const_layer_iterator layerCBegin() const
   {
      return m_layers.cbegin();
   }
   const_layer_iterator layerCEnd() const
   {
      return m_layers.cend();
   }

   const_reverse_layer_iterator layerCRBegin() const
   {
      return m_layers.crbegin();
   }
   const_reverse_layer_iterator layerCREnd() const
   {
      return m_layers.crend();
   }

   // ---------------------------------------------------------------------
   // Functions to iterate through tilesets

   using tileset_iterator = tilesets_t::iterator;
   using const_tileset_iterator = tilesets_t::const_iterator;

   tileset_iterator tilesetBegin()
   {
      return m_tilesets.begin();
   }
   tileset_iterator tilesetEnd()
   {
      return m_tilesets.end();
   }

   const_tileset_iterator tilesetBegin() const
   {
      return m_tilesets.begin();
   }
   const_tileset_iterator tilesetEnd() const
   {
      return m_tilesets.end();
   }

   const_tileset_iterator tilesetCBegin() const
   {
      return m_tilesets.cbegin();
   }
   const_tileset_iterator tilesetCEnd() const
   {
      return m_tilesets.cend();
   }

   // ---------------------------------------------------------------------
};

inline std::ostream &operator<<(std::ostream &os, const LdtkDefFile &ldtkFile)
{
   os << "LDtk file: " << ldtkFile.m_filename << std::endl;
   os << "Unique Id: " << ldtkFile.m_projectUniqueId << std::endl;
   os << "File version: " << ldtkFile.m_fileVersion << std::endl;
   os << "BG color: " << ldtkFile.m_bgColor << std::endl;
   os << "BG color 8: " << +(ldtkFile.m_bgColor8.r) << ", " << +(ldtkFile.m_bgColor8.g) << ", " << +(ldtkFile.m_bgColor8.b) << std::endl;
   os << "BG color f: " << ldtkFile.m_bgColorf.r << ", " << ldtkFile.m_bgColorf.g << ", " << ldtkFile.m_bgColorf.b << std::endl;
   os << "m_layers.capacity() " << ldtkFile.m_layers.capacity() << std::endl;
   os << "m_layers.size() " << ldtkFile.m_layers.size() << std::endl;
   os << "m_tilesets.capacity() " << ldtkFile.m_tilesets.capacity() << std::endl;
   os << "m_tilesets.size() " << ldtkFile.m_tilesets.size() << std::endl;

   for (size_t layerIdx = 0, layerLen = ldtkFile.m_layers.size(); layerIdx < layerLen; ++layerIdx)
   {
      const Layer &layer = ldtkFile.m_layers[layerIdx];

      os << "Layer " << layerIdx << ": (" << layer.uid << ") \"" << layer.name << "\"" << std::endl;
      os << "  cellPixelSize: " << layer.cellPixelSize << std::endl;
      os << "  randomSeed: " << layer.randomSeed << std::endl;

      const ldtkimport::TileSet *tileset = nullptr;
      if (ldtkFile.getTileset(layer.tilesetDefUid, tileset))
      {
         os << "  tilesetDefUid: " << tileset->name << " (" << layer.tilesetDefUid << ")" << std::endl;
      }
      else
      {
         os << "  tilesetDefUid: " << layer.tilesetDefUid << std::endl;
      }

      for (size_t intGridIdx = 0, intGridLen = layer.intGridValues.size(); intGridIdx < intGridLen; ++intGridIdx)
      {
         os << "  IntGridValue: (" << layer.intGridValues[intGridIdx].id << ") " << layer.intGridValues[intGridIdx].name << std::endl;
      }

#ifdef LDTK_IMPORT_INCLUDE_RULES_IN_DEF_FILE_OSTREAM
      for (size_t ruleGroupIdx = 0, ruleGroupLen = layer.ruleGroups.size(); ruleGroupIdx < ruleGroupLen; ++ruleGroupIdx)
      {
         const RuleGroup &ruleGroup = layer.ruleGroups[ruleGroupIdx];
         if (!ruleGroup.active)
         {
            continue;
         }

         os << "  Rule Group " << ruleGroupIdx << ": \"" << ruleGroup.name << "\"" << std::endl;
         for (size_t ruleIdx = 0, ruleLen = ruleGroup.rules.size(); ruleIdx < ruleLen; ++ruleIdx)
         {
            const Rule &rule = ruleGroup.rules[ruleIdx];
            if (!rule.active)
            {
               continue;
            }

            os << rule << std::endl;
         }
      }
#endif
   }

   for (size_t tilesetIdx = 0, tilesetLen = ldtkFile.m_tilesets.size(); tilesetIdx < tilesetLen; ++tilesetIdx)
   {
      auto &tileset = ldtkFile.m_tilesets[tilesetIdx];
      os << "Tileset " << tilesetIdx << ": (" << tileset.uid << ") \"" << tileset.name << "\"" << std::endl;
      os << "  Image: " << tileset.imagePath << std::endl;
      os << "  Image Size: " << tileset.imageWidth << "x" << tileset.imageHeight << std::endl;
      os << "  tileSize: " << tileset.tileSize << std::endl;
      os << "  margin: " << tileset.margin << std::endl;
      os << "  spacing: " << tileset.spacing << std::endl;
   }

   return os;
}

} // namespace ldtkimport

#endif // LDTK_IMPORT_LDTK_DEF_FILE_H
