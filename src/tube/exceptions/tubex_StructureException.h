/* ============================================================================
 *  tubex-lib - StructureException class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef StructureException_HEADER
#define StructureException_HEADER

#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include "tubex_Subtube.h"
#include "tubex_Exception.h"

namespace tubex
{
  /**
   * \brief Structure error exception.
   *
   * Thrown when two tubes do not share
   * the same data structure.
   */
  class StructureException : public Exception
  {
    public:

      StructureException(const Subtube& x1, const Subtube& x2);
      static void check(const Subtube& x1, const Subtube& x2);
      // todo: equivalent constructor for Tube class
  };
}

#endif