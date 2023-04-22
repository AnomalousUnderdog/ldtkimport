#include "ldtkimport/GridUtility.h"

#include <catch2/catch_test_macros.hpp>

using namespace ldtkimport;


TEST_CASE("Coordinates are converted to indices", "[Grid Utility]")
{
   REQUIRE(GridUtility::getIndex(3, 7, 10) == 73);
}

TEST_CASE("Indices are converted to coordinates", "[Grid Utility]")
{
   int x;
   int y;
   GridUtility::getCoordinates(73, 10, x, y);
   REQUIRE(x == 3); REQUIRE(y == 7);
}

TEST_CASE("Bounds check", "[Grid Utility]")
{
   WHEN("input is negative")
   {
      THEN("isWithinBounds should fail")
      {
         REQUIRE(GridUtility::isWithinBounds(-1, -1, 10, 10) == false);
         REQUIRE(GridUtility::isWithinBounds(0, -1, 10, 10) == false);
         REQUIRE(GridUtility::isWithinBounds(-1, 0, 10, 10) == false);
      }
   }

   WHEN("input is beyond width/height")
   {
      THEN("isWithinBounds should fail")
      {
         REQUIRE(GridUtility::isWithinBounds(10, 10, 10, 10) == false);
         REQUIRE(GridUtility::isWithinBounds(0, 10, 10, 10) == false);
         REQUIRE(GridUtility::isWithinBounds(10, 0, 10, 10) == false);
      }
   }

   WHEN("input is within bounds")
   {
      THEN("isWithinBounds should succeed")
      {
         REQUIRE(GridUtility::isWithinBounds(0, 0, 10, 10) == true);
         REQUIRE(GridUtility::isWithinBounds(9, 9, 10, 10) == true);
         REQUIRE(GridUtility::isWithinBounds(0, 9, 10, 10) == true);
         REQUIRE(GridUtility::isWithinBounds(9, 0, 10, 10) == true);
      }
   }
}
