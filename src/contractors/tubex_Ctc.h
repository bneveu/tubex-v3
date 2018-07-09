/* ============================================================================
 *  tubex-lib - Ctc class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_CTC_H__
#define __TUBEX_CTC_H__

#include "tubex_Tube.h"
#include "tubex_TubeVector.h"

namespace tubex
{
  /**
   * \brief Ctc class.
   */
  class Ctc
  {
    public:

      Ctc() {};
      virtual bool contract() {};
  };
}

#endif