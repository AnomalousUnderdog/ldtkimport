#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "ldtkimport/LdtkDefFile.h"

using Catch::Matchers::ContainsSubstring;
using namespace ldtkimport;


TEST_CASE("Rule Test", "[Rule]")
{
   Level level;
   level.setIntGrid(5, 5, {
      0, 1, 0, 0, 0,
      1, 0, 1, 0, 0,
      0, 1, 1, 0, 0,
      0, 1, 0, 1, 0,
      0, 0, 1, 0, 0
      });

   LdtkDefFile def;

   def.addLayer(Layer());
   Layer &layer1 = *def.layerBegin();
   layer1.ruleGroups.push_back(RuleGroup());

   RuleGroup &ruleGroup1 = layer1.ruleGroups[0];
   ruleGroup1.rules.push_back(Rule());

   Rule &rule1 = ruleGroup1.rules[0];

   rule1.patternSize = 3;
   rule1.pattern = {
      0, 1, 0,
      1, 0, 1,
      0, 1, 0,
      };
   rule1.tileIds = { 1337 };

#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
   RulesLog rulesLog;
#endif

   for (int n = 0; n < 10; ++n)
   {
      def.runRules(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
         rulesLog,
#endif
         level);
   }

   REQUIRE(level.getTileGridCount() == 1);

   const TileGrid &tileGrid = level.getTileGridByIdx(0);

   REQUIRE(tileGrid.getTileIdDebugString() == R"(
[], [], [], [], []
[], [1337], [], [], []
[], [], [], [], []
[], [], [1337], [], []
[], [], [], [], []
)");

}

TEST_CASE("Tile Stamp", "[Rule]")
{
   Level level;
   level.setIntGrid(5, 5, {
      0, 0, 0, 0, 0,
      2, 2, 2, 2, 2,
      1, 1, 1, 1, 1,
      1, 1, 1, 1, 1,
      0, 1, 1, 0, 0
      });

   LdtkDefFile def;

   def.addLayer(Layer());
   Layer &layer1 = *def.layerBegin();
   layer1.ruleGroups.push_back(RuleGroup());
   layer1.tilesetDefUid = 3224;

   def.addTileset(TileSet());
   TileSet &tileSet = *def.tilesetBegin();
   tileSet.uid = 3224;
   tileSet.tileCountWidth = 3;
   tileSet.tileCountHeight = 3;

   RuleGroup &ruleGroup1 = layer1.ruleGroups[0];
   ruleGroup1.rules.push_back(Rule());

   Rule &rule1 = ruleGroup1.rules[0];

   rule1.patternSize = 3;
   rule1.pattern = {
      -1, -1, -1,
       1,  1,  1,
       0,  1,  0,
   };

   rule1.tileIds = { 0, 1, 2, 4, 7 };
   // will be a 2x3 stamp, looks like this:
   // 0 1 2
   //   4
   //   7

   rule1.tileMode = Rule::TileMode::Stamp;


#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
   RulesLog rulesLog;
#endif

   WHEN("Stamp is anchored at bottom-center")
   {
      rule1.stampPivotX = 0.5f;
      rule1.stampPivotY = 1.0f;
      def.preProcess(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
         rulesLog
#endif
      );

      def.runRules(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
         rulesLog,
#endif
         level);

      THEN("Results should be at bottom-center")
      {
         REQUIRE(level.getTileGridCount() == 1);
         const TileGrid &tileGrid = level.getTileGridByIdx(0);

         REQUIRE(tileGrid.getTileIdDebugString() == R"(
[0], [1, 0], [2, 1, 0], [2, 1], [2]
[], [4], [4], [4], []
[], [7], [7], [7], []
[], [], [], [], []
[], [], [], [], []
)");
      }
   }

   WHEN("Stamp is anchored at bottom-right")
   {
      rulesLog.clear();

      rule1.stampPivotX = 1.0f;
      rule1.stampPivotY = 1.0f;
      def.preProcess(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
         rulesLog
#endif
      );

      def.runRules(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
         rulesLog,
#endif
         level);

      THEN("Results should be at bottom-right")
      {
         REQUIRE(level.getTileGridCount() == 1);
         const TileGrid &tileGrid = level.getTileGridByIdx(0);

         REQUIRE(tileGrid.getTileIdDebugString() == R"(
[1, 0], [2, 1, 0], [2, 1], [2], []
[4], [4], [4], [], []
[7], [7], [7], [], []
[], [], [], [], []
[], [], [], [], []
)");
      }
   }

   WHEN("Stamp is anchored at bottom-left")
   {
      rulesLog.clear();

      rule1.stampPivotX = 0.0f;
      rule1.stampPivotY = 1.0f;
      def.preProcess(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
         rulesLog
#endif
      );

      def.runRules(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
         rulesLog,
#endif
         level);

      THEN("Results should be at bottom-left")
      {
         REQUIRE(level.getTileGridCount() == 1);
         const TileGrid &tileGrid = level.getTileGridByIdx(0);

         std::cout << "Bottom-left anchor:" << std::endl << rulesLog.rule[0] << std::endl;

         REQUIRE(tileGrid.getTileIdDebugString() == R"(
[], [0], [1, 0], [2, 1, 0], [2, 1]
[], [], [4], [4], [4]
[], [], [7], [7], [7]
[], [], [], [], []
[], [], [], [], []
)");
      }
   }
}

TEST_CASE("Rule with modulo", "[Rule]")
{
   Level level;
   level.setIntGrid(3, 3, {
      1, 1, 1,
      1, 1, 1,
      1, 1, 1,
      });

   LdtkDefFile def;

   def.addLayer(Layer());
   Layer &layer1 = *def.layerBegin();
   layer1.ruleGroups.push_back(RuleGroup());

   RuleGroup &ruleGroup1 = layer1.ruleGroups[0];
   ruleGroup1.rules.push_back(Rule());

   Rule &rule1 = ruleGroup1.rules[0];

   rule1.patternSize = 1;
   rule1.pattern = { 1 };
   rule1.tileIds = { 9 };

#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
   RulesLog rulesLog;
#endif

   WHEN("Modulo is 2,1")
   {
      rule1.xModulo = 2;
      rule1.yModulo = 1;

      THEN("DefFile should report as valid")
      {
         REQUIRE(def.isValid());
      }

      def.runRules(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
         rulesLog,
#endif
         level);

      REQUIRE(level.getTileGridCount() == 1);

      THEN("Result should skip horizontal")
      {
         REQUIRE(level.getTileGridCount() == 1);
         const TileGrid &tileGrid = level.getTileGridByIdx(0);

         REQUIRE(tileGrid.getTileIdDebugString() == R"(
[9], [], [9]
[9], [], [9]
[9], [], [9]
)");
      }
   }

   WHEN("Modulo is 1,2")
   {
      rule1.xModulo = 1;
      rule1.yModulo = 2;

      THEN("DefFile should report as valid")
      {
         REQUIRE(def.isValid());
      }

      def.runRules(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
         rulesLog,
#endif
         level);

      REQUIRE(level.getTileGridCount() == 1);

      THEN("Result should skip vertical")
      {
         REQUIRE(level.getTileGridCount() == 1);
         const TileGrid &tileGrid = level.getTileGridByIdx(0);

         REQUIRE(tileGrid.getTileIdDebugString() == R"(
[9], [9], [9]
[], [], []
[9], [9], [9]
)");
      }
   }

   WHEN("Modulo is 1,2 and CheckerMode is Vertical")
   {
      rule1.xModulo = 1;
      rule1.yModulo = 2;
      rule1.checker = Rule::CheckerMode::Vertical;

      THEN("DefFile should report as valid")
      {
         REQUIRE(def.isValid());
      }

      def.runRules(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
         rulesLog,
#endif
         level);

      THEN("Result should be checkered")
      {
         REQUIRE(level.getTileGridCount() == 1);
         const TileGrid &tileGrid = level.getTileGridByIdx(0);

         REQUIRE(tileGrid.getTileIdDebugString() == R"(
[9], [], [9]
[], [9], []
[9], [], [9]
)");
      }
   }

   WHEN("Modulo is 2,1 and CheckerMode is Horizontal")
   {
      rule1.xModulo = 2;
      rule1.yModulo = 1;
      rule1.checker = Rule::CheckerMode::Horizontal;

      THEN("DefFile should report as valid")
      {
         REQUIRE(def.isValid());
      }

      def.runRules(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
         rulesLog,
#endif
         level);

      THEN("Result should be checkered")
      {
         REQUIRE(level.getTileGridCount() == 1);
         const TileGrid &tileGrid = level.getTileGridByIdx(0);

         REQUIRE(tileGrid.getTileIdDebugString() == R"(
[9], [], [9]
[], [9], []
[9], [], [9]
)");
      }
   }

   WHEN("Modulo is 2,1 and CheckerMode is Vertical")
   {
      rule1.xModulo = 2;
      rule1.yModulo = 1;
      rule1.checker = Rule::CheckerMode::Vertical;

      THEN("DefFile should report as valid")
      {
         REQUIRE(def.isValid());
      }

      def.runRules(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
         rulesLog,
#endif
         level);

      REQUIRE(level.getTileGridCount() == 1);

      THEN("Result should not checker")
      {
         REQUIRE(level.getTileGridCount() == 1);
         const TileGrid &tileGrid = level.getTileGridByIdx(0);

         REQUIRE(tileGrid.getTileIdDebugString() == R"(
[9], [], [9]
[9], [], [9]
[9], [], [9]
)");
      }
   }

   WHEN("Modulo is 1,2 and CheckerMode is Horizontal")
   {
      rule1.xModulo = 1;
      rule1.yModulo = 2;
      rule1.checker = Rule::CheckerMode::Horizontal;

      THEN("DefFile should report as valid")
      {
         REQUIRE(def.isValid());
      }

      def.runRules(
#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
         rulesLog,
#endif
         level);

      REQUIRE(level.getTileGridCount() == 1);

      THEN("Result should not checker")
      {
         REQUIRE(level.getTileGridCount() == 1);
         const TileGrid &tileGrid = level.getTileGridByIdx(0);

         REQUIRE(tileGrid.getTileIdDebugString() == R"(
[9], [9], [9]
[], [], []
[9], [9], [9]
)");
      }
   }

   WHEN("Modulo is 0,1")
   {
      rule1.xModulo = 0;
      rule1.yModulo = 1;
      rule1.checker = Rule::CheckerMode::Vertical;

      THEN("DefFile should report as invalid")
      {
         REQUIRE(def.isValid() == false);

#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
         REQUIRE_THROWS_MATCHES(def.runRules(rulesLog, level), std::logic_error, MessageMatches(ContainsSubstring("divisor is zero")));
#else
         REQUIRE_THROWS_MATCHES(def.runRules(level), std::logic_error, MessageMatches(ContainsSubstring("divisor is zero")));
#endif

         REQUIRE(level.getTileGridCount() == 1);
         const TileGrid &tileGrid = level.getTileGridByIdx(0);

         REQUIRE(tileGrid.getTileIdDebugString() == R"(
[], [], []
[], [], []
[], [], []
)");
      }
   }

   WHEN("Modulo is 1,0")
   {
      rule1.xModulo = 1;
      rule1.yModulo = 0;

      THEN("DefFile should report as invalid")
      {
         REQUIRE(def.isValid() == false);

#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
         REQUIRE_THROWS_MATCHES(def.runRules(rulesLog, level), std::logic_error, MessageMatches(ContainsSubstring("divisor is zero")));
#else
         REQUIRE_THROWS_MATCHES(def.runRules(level), std::logic_error, MessageMatches(ContainsSubstring("divisor is zero")));
#endif

         REQUIRE(level.getTileGridCount() == 1);
         const TileGrid &tileGrid = level.getTileGridByIdx(0);

         REQUIRE(tileGrid.getTileIdDebugString() == R"(
[], [], []
[], [], []
[], [], []
)");
      }

   }

   WHEN("Modulo is 0,0")
   {
      rule1.xModulo = 0;
      rule1.yModulo = 0;

      THEN("DefFile should report as invalid")
      {
         REQUIRE(def.isValid() == false);

#if !defined(NDEBUG) && LDTK_IMPORT_DEBUG_RULE > 0
         REQUIRE_THROWS_MATCHES(def.runRules(rulesLog, level), std::logic_error, MessageMatches(ContainsSubstring("divisor is zero")));
#else
         REQUIRE_THROWS_MATCHES(def.runRules(level), std::logic_error, MessageMatches(ContainsSubstring("divisor is zero")));
#endif

         REQUIRE(level.getTileGridCount() == 1);
         const TileGrid &tileGrid = level.getTileGridByIdx(0);

         REQUIRE(tileGrid.getTileIdDebugString() == R"(
[], [], []
[], [], []
[], [], []
)");
      }

   }
}
