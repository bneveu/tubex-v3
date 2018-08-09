/* ============================================================================
 *  tubex-lib - TubeVector class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_TUBEVECTOR_H__
#define __TUBEX_TUBEVECTOR_H__

#include <map>
#include <vector>
#include "tubex_Fnc.h"
#include "tubex_TrajectoryVector.h"
#include "tubex_AbstractTube.h"
#include "tubex_TubeSerialization.h"

namespace tubex
{
  class Fnc;
  class Tube;
  class Trajectory;
  
  class TubeVector : public AbstractTube
  {
    public:

    /** Base: **/

      // Definition
      TubeVector(const ibex::Interval& domain, int n = 1);
      TubeVector(const ibex::Interval& domain, const ibex::IntervalVector& codomain);
      TubeVector(const ibex::Interval& domain, double timestep, int n = 1);
      TubeVector(const ibex::Interval& domain, double timestep, const ibex::IntervalVector& codomain);
      TubeVector(const ibex::Interval& domain, double timestep, const tubex::Fnc& f);
      TubeVector(const TubeVector& x);
      TubeVector(const TubeVector& x, const ibex::IntervalVector& codomain);
      TubeVector(const Tube& x);
      TubeVector(int n, const Tube& x);
      TubeVector(const TrajectoryVector& traj, double timestep = 0.);
      TubeVector(const TrajectoryVector& lb, const TrajectoryVector& ub, double timestep = 0.);
      TubeVector(const std::string& binary_file_name);
      TubeVector(const std::string& binary_file_name, TrajectoryVector& traj);
      TubeVector(const std::string& binary_file_name, std::vector<TrajectoryVector>& v_trajs);
      ~TubeVector();
      const TubeVector primitive() const;
      const TubeVector& operator=(const TubeVector& x);
      const ibex::Interval domain() const;
      int size() const;
      void resize(int n);
      const TubeVector subvector(int start_index, int end_index) const;
      void put(int start_index, const TubeVector& subvec);

      // Slices structure
      int nb_slices() const;
      int input2index(double t) const;
      void sample(double t);
      void sample(double t, const ibex::IntervalVector& gate);
      //static bool share_same_slicing(const TubeVector& x1, const TubeVector& x2);

      // Access values
      const ibex::IntervalVector codomain() const;
      double volume() const;
      Tube& operator[](int index);
      const Tube& operator[](int index) const;
      const ibex::IntervalVector operator()(int slice_id) const;
      const ibex::IntervalVector operator()(double t) const;
      const ibex::IntervalVector operator()(const ibex::Interval& t) const;
      const ibex::Interval invert(const ibex::IntervalVector& y, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::IntervalVector& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      const ibex::Interval invert(const ibex::IntervalVector& y, const TubeVector& v, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::IntervalVector& y, std::vector<ibex::Interval> &v_t, const TubeVector& v, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      const std::pair<ibex::IntervalVector,ibex::IntervalVector> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      const ibex::IntervalVector interpol(double t, const TubeVector& v) const;
      const ibex::IntervalVector interpol(const ibex::Interval& t, const TubeVector& v) const;
      const ibex::Vector max_thickness() const;
      const ibex::Vector max_gate_thickness() const;

      // Tests
      bool operator==(const TubeVector& x) const;
      bool operator!=(const TubeVector& x) const;
      bool is_subset(const TubeVector& x) const;
      bool is_strict_subset(const TubeVector& x) const;
      bool is_empty() const;
      bool encloses(const TrajectoryVector& x) const;

      // Setting values
      void set(const ibex::IntervalVector& y);
      void set(const ibex::IntervalVector& y, int slice_id);
      void set(const ibex::IntervalVector& y, double t);
      void set(const ibex::IntervalVector& y, const ibex::Interval& t);
      void set_empty();
      const TubeVector& inflate(double rad);
      const TubeVector& inflate(const TrajectoryVector& rad);

      // Bisection
      const std::pair<TubeVector,TubeVector> bisect(double t, float ratio = 0.55) const;

      // Operators
        // Assignments
        const TubeVector& operator+=(const TubeVector& x);
        const TubeVector& operator+=(const Tube& x);
        const TubeVector& operator+=(const TrajectoryVector& x);
        const TubeVector& operator+=(const Trajectory& x);
        const TubeVector& operator+=(const ibex::IntervalVector& x);
        const TubeVector& operator+=(const ibex::Interval& x);
        const TubeVector& operator-=(const TubeVector& x);
        const TubeVector& operator-=(const Tube& x);
        const TubeVector& operator-=(const TrajectoryVector& x);
        const TubeVector& operator-=(const Trajectory& x);
        const TubeVector& operator-=(const ibex::IntervalVector& x);
        const TubeVector& operator-=(const ibex::Interval& x);
        const TubeVector& operator|=(const TubeVector& x);
        const TubeVector& operator|=(const Tube& x);
        const TubeVector& operator|=(const TrajectoryVector& x);
        const TubeVector& operator|=(const Trajectory& x);
        const TubeVector& operator|=(const ibex::IntervalVector& x);
        const TubeVector& operator|=(const ibex::Interval& x);
        const TubeVector& operator&=(const TubeVector& x);
        const TubeVector& operator&=(const Tube& x);
        const TubeVector& operator&=(const TrajectoryVector& x);
        const TubeVector& operator&=(const Trajectory& x);
        const TubeVector& operator&=(const ibex::IntervalVector& x);
        const TubeVector& operator&=(const ibex::Interval& x);
        const TubeVector& operator*=(const Tube& x);
        const TubeVector& operator*=(const Trajectory& x);
        const TubeVector& operator*=(const ibex::Interval& x);
        // Note: operation TubeVector*=(Tube|Trajectory|Interval)Vector not defined
        // Note: operation TubeVector/= not defined, since IntervalVector not defined either
      // Other mathematical operators
        const TubeVector& operator|(const TubeVector& x);
        const TubeVector& operator|(const TrajectoryVector& x);
        const TubeVector& operator|(const ibex::IntervalVector& x);
        const TubeVector& operator&(const TubeVector& x);
        const TubeVector& operator&(const TrajectoryVector& x);
        const TubeVector& operator&(const ibex::IntervalVector& x);

      // String
      const std::string class_name() const { return "TubeVector"; };

    /** Integration: **/

      const ibex::IntervalVector integral(double t) const;
      const ibex::IntervalVector integral(const ibex::Interval& t) const;
      const ibex::IntervalVector integral(const ibex::Interval& t1, const ibex::Interval& t2) const;
      const std::pair<ibex::IntervalVector,ibex::IntervalVector> partial_integral(const ibex::Interval& t) const;
      const std::pair<ibex::IntervalVector,ibex::IntervalVector> partial_integral(const ibex::Interval& t1, const ibex::Interval& t2) const;

    /** Contractors: **/

      bool ctc_deriv(const TubeVector& v);
      bool ctc_eval(ibex::Interval& t, ibex::IntervalVector& z, TubeVector& w);

    /** Serialization: **/

      void serialize(const std::string& binary_file_name = "x.tube", int version_number = SERIALIZATION_VERSION) const;
      void serialize(const std::string& binary_file_name, const TrajectoryVector& traj, int version_number = SERIALIZATION_VERSION) const;
      void serialize(const std::string& binary_file_name, const std::vector<TrajectoryVector>& v_trajs, int version_number = SERIALIZATION_VERSION) const;

    protected:

      TubeVector(); // todo: remove this?
      const ibex::IntervalVector codomain_box() const;
      void deserialize(const std::string& binary_file_name, std::vector<TrajectoryVector>& v_trajs);

    /** Class variables **/

      int m_n;
      Tube *m_v_tubes = NULL;

      friend void deserialize_tubevector(std::ifstream& bin_file, TubeVector& tube);
  };
}

#endif