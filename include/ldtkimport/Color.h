#ifndef LDTK_IMPORT_COLOR_H
#define LDTK_IMPORT_COLOR_H

#include <cstdint>


namespace ldtkimport
{

/**
 *  @brief RGB stored in 8-bit integers.
 */
struct Color8
{
   uint8_t r, g, b;
};

/**
 *  @brief RGB stored in floats. 0.0 to 1.0 range.
 */
struct Colorf
{
   float r, g, b;
};

} // namespace ldtkimport

#endif // LDTK_IMPORT_COLOR_H
