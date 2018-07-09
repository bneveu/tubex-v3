/* ============================================================================
 *  tubex-lib - TrajectoryVector class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_TRAJECTORYVECTOR_H__
#define __TUBEX_TRAJECTORYVECTOR_H__

#include <map>
#include "ibex_Vector.h"
#include "ibex_Interval.h"
#include "tubex_Function.h"
#include "tubex_DynamicalItem.h"

// todo:
// - arithmetic operators for trajectories?

namespace tubex
{
  class Function;

  class TrajectoryVector : public DynamicalItem
  {
    public:

      // Definition
      TrajectoryVector();
      TrajectoryVector(const ibex::Interval& domain, const tubex::Function& f);
      TrajectoryVector(const std::map<double,ibex::Vector>& m_map_values);
      TrajectoryVector(const TrajectoryVector& traj);
      ~TrajectoryVector();
      const TrajectoryVector& operator=(const TrajectoryVector& x);
      int dim() const;

      // Access values
      const std::map<double,ibex::Vector>& getMap() const;
      const tubex::Function* getFunction() const;
      const ibex::Interval domain() const;
      const ibex::IntervalVector codomain() const;
      const ibex::IntervalVector codomainBox() const;
      const ibex::Vector operator[](double t) const;
      const ibex::IntervalVector operator[](const ibex::Interval& t) const;

      // Tests
      bool notDefined() const;
      bool operator==(const TrajectoryVector& x) const;
      bool operator!=(const TrajectoryVector& x) const;

      // Setting values
      const ibex::Vector& set(double t, const ibex::Vector& y);
      void truncateDomain(const ibex::Interval& domain);
      void shiftDomain(double shift_ref);

      // String
      friend std::ostream& operator<<(std::ostream& str, const TrajectoryVector& x);
      const std::string className() const { return "TrajectoryVector"; };

    protected:

      /** Class variables **/

        ibex::Interval m_domain = ibex::Interval::EMPTY_SET;
        ibex::IntervalVector m_codomain = ibex::IntervalVector(1, ibex::Interval::EMPTY_SET);
        // A trajectory is defined either by a Function or a map of values
        tubex::Function *m_function = NULL;
        std::map<double,ibex::Vector> m_map_values;
  };
}

#endif