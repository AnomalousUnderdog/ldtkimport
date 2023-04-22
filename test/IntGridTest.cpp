#include "ldtkimport/IntGrid.h"

#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

using Catch::Matchers::ContainsSubstring;
using namespace ldtkimport;


TEST_CASE("Int Grid width and height", "[Int Grid]")
{
   IntGrid grid5x5(5, 5, {
       1,  2,  3,  4,  5,
       6,  7,  8,  9, 10,
      11, 12, 13, 14, 15,
      16, 17, 18, 19, 20,
      21, 22, 23, 24, 25
      });

   REQUIRE(grid5x5.getWidth() == 5);
   REQUIRE(grid5x5.getHeight() == 5);

   IntGrid grid2x3(2, 3, {
      1,  2,
      3,  4,
      5,  6,
      });

   REQUIRE(grid2x3.getWidth() == 2);
   REQUIRE(grid2x3.getHeight() == 3);
}

TEST_CASE("Int Grid indexing is correct", "[Int Grid]")
{
   IntGrid grid5x5(5, 5, {
       1,  2,  3,  4,  5,
       6,  7,  8,  9, 10,
      11, 12, 13, 14, 15,
      16, 17, 18, 19, 20,
      21, 22, 23, 24, 25
      });

   SECTION("Getting cells by index")
   {
      // This is zero-based, like going through an array.
      REQUIRE(grid5x5(0) == 1);
      REQUIRE(grid5x5(24) == 25);
   }

   SECTION("Getting cells by x & y coordinates")
   {
      // Coordinates are also zero-based,
      // with the origin at the upper-left.
      //
      // Think of it as specifying an
      // offset from the upper-left cell.
      //
      // So (0, 0) gives you the upper-left cell itself.
      // (1, 0) gives the cell to its right.
      // (0, 1) gives the cell below it.
      //
      REQUIRE(grid5x5(0, 0) == 1);
      REQUIRE(grid5x5(4, 4) == 25);
   }

   SECTION("Negative x or y coordinate should throw std::out_of_range")
   {
      // Since the coordinates are zero-based,
      // anything negative is out of range.
      REQUIRE_THROWS_MATCHES(grid5x5(-1, 0), std::out_of_range, MessageMatches(ContainsSubstring("x index is negative")));
      REQUIRE_THROWS_MATCHES(grid5x5(0, -1), std::out_of_range, MessageMatches(ContainsSubstring("y index is negative")));
   }

   SECTION("Too large x or y coordinate should throw std::out_of_range")
   {
      // Since the coordinates are zero-based,
      // an x value equal to the width is actually trying to access beyond the width.
      // A y value equal to height is actually trying to access beyond the height.
      REQUIRE_THROWS_MATCHES(grid5x5(5, 0), std::out_of_range, MessageMatches(ContainsSubstring("x index is beyond width")));
      REQUIRE_THROWS_MATCHES(grid5x5(0, 5), std::out_of_range, MessageMatches(ContainsSubstring("y index is beyond height")));
   }
}
