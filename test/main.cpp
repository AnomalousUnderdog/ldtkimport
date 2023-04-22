#include <iostream>

#include <catch2/catch_session.hpp>

#include "ldtkimport/IntGrid.h"
#include "ldtkimport/LdtkDefFile.h"


int main(int argc, char *argv[])
{
   int result = Catch::Session().run(argc, argv);
   return result;
}
