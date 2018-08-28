/* ============================================================================
 *  tubex-lib - CtcEval class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include <list>
#include "tubex_CtcEval.h"
#include "tubex_CtcDeriv.h"
#include "tubex_SlicingException.h"
#include "tubex_EmptyException.h"
#include "tubex_DomainException.h"
#include "tubex_DimensionException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcEval::CtcEval() : Ctc()
  {

  }

  void CtcEval::enable_temporal_propagation(bool enable_propagation)
  {
    m_propagation_enabled = enable_propagation;
  }

  void CtcEval::contract(double t, Interval& z, Tube& y, Tube& w) const
  {
    SlicingException::check(y, w);

    if(z.is_empty() || y.is_empty())
    {
      z.set_empty();
      y.set_empty();
      return;
    }

    Tube y_first_slicing = y, w_first_slicing = w; // doto: instanciate this just in case

    z &= y.interpol(t, w);
    y.set(z, t);
    w.sample(t); // w is also sampled to stay compliant with y

    if(m_propagation_enabled)
    {
      CtcDeriv ctc_deriv;
      ctc_deriv.contract(y, w);
    }

    if(z.is_empty() || y.is_empty())
    {
      z.set_empty();
      y.set_empty();
      return;
    }

    if(m_preserve_slicing)
    {
      y_first_slicing.set_empty();
      y_first_slicing |= y;
      y = y_first_slicing;

      w_first_slicing.set_empty();
      w_first_slicing |= w;
      w = w_first_slicing;
    }
  }

  void CtcEval::contract(Interval& t, Interval& z, Tube& y, Tube& w) const
  {
    SlicingException::check(y, w);
    
    if(t.is_degenerated())
      return contract(t.lb(), z, y, w);

    if(t.is_empty() || z.is_empty() || y.is_empty())
    {
      t.set_empty();
      z.set_empty();
      y.set_empty();
      return;
    }

    Tube y_first_slicing(y), w_first_slicing(w);
    Interval t_ = t;
    Interval z_ = z;
    Tube y_ = y;

    t &= y.domain();
    t &= y.invert(z, w ,t);

    if(!t.is_empty())
    {
      if(t.is_degenerated())
        return contract(t.lb(), z, y, w);
      
      z &= y.interpol(t, w);

      if(!z.is_empty())
      {
        y.set(y.interpol(t.lb(), w), t.lb()); w.sample(t.lb());
        y.set(y.interpol(t.ub(), w), t.ub()); w.sample(t.ub());
          
        // Note: w is also sampled to stay compliant with y.
        // The same for future comparison:
        if(!m_preserve_slicing)
        {
          y_.sample(t.lb());
          y_.sample(t.ub());
        }

        CtcDeriv ctc_deriv;
        Interval front_gate(y.size());
        list<Interval> l_gates;
        Slice *slice_y;
        Slice *slice_w;

        // 1. Forward propagation

          slice_y = y.get_slice(t.lb());
          slice_w = w.get_slice(t.lb());

          front_gate = slice_y->input_gate() & z;
            // Mathematically, front_gate should not be empty at this point.
            // Due to numerical approximations, the computation of t by the invert method
            // provides a wider enclosure t'. The evaluation of y[t'.lb()] may not
            // intersect z and so the front_gate becomes empty.
            // An epsilon inflation could be used to overcome this problem. Or:
            if(front_gate.is_empty())
            {
              if(slice_y->input_gate().ub() < z.lb()) front_gate = z.lb();
              else front_gate = z.ub();
            }

          l_gates.push_front(front_gate);

          while(slice_y != NULL && slice_y->domain().lb() < t.ub())
          {
            // Forward propagation of the evaluation
            front_gate += slice_y->domain().diam() * slice_w->codomain(); // projection
            front_gate |= z; // evaluation
            front_gate &= slice_y->output_gate(); // contraction

            // Storing temporarily fwd propagation 
            l_gates.push_front(front_gate);

            // Iteration
            slice_y = slice_y->next_slice();
            slice_w = slice_w->next_slice();
          }

        // 2. Backward propagation

          slice_y = y.get_slice(previous_float(t.ub()));
          slice_w = w.get_slice(previous_float(t.ub()));

          front_gate = slice_y->output_gate() & z;
            // Overcoming numerical approximations, same remark as before:
            if(front_gate.is_empty())
            {
              if(slice_y->output_gate().ub() < z.lb()) front_gate = z.lb();
              else front_gate = z.ub();
            }

          slice_y->set_output_gate(l_gates.front() | front_gate);

          while(slice_y != NULL && slice_y->domain().lb() >= t.lb())
          {
            // Backward propagation of the evaluation
            front_gate -= slice_y->domain().diam() * slice_w->codomain(); // projection
            front_gate |= z; // evaluation
            front_gate &= slice_y->input_gate(); // contraction

            // Updating tube
            l_gates.pop_front();
            slice_y->set_input_gate(l_gates.front() | front_gate);
            ctc_deriv.contract_gates(*slice_y, *slice_w);

            // Iteration
            slice_y = slice_y->prev_slice();
            slice_w = slice_w->prev_slice();
          }

        // 3. Envelopes contraction

          if(m_propagation_enabled)
            ctc_deriv.contract(y, w);

        // 4. Evaluation contraction

          t &= y.invert(z, w ,t);
          if(!t.is_empty())
            z &= y.interpol(t, w);
      }
    }

    if(m_preserve_slicing)
    {
      y_first_slicing.set_empty();
      y_first_slicing |= y;
      y = y_first_slicing;

      w_first_slicing.set_empty();
      w_first_slicing |= w;
      w = w_first_slicing;
    }

    if(t.is_empty() || z.is_empty() || y.is_empty())
    {
      t.set_empty();
      z.set_empty();
      y.set_empty();
    }
  }

  void CtcEval::contract(const Interval& t, const Interval& z, Tube& y, const Tube& w) const
  {
    Interval _t(t), _z(z);
    Tube _w(w);
    contract(_t, _z, y, _w);
  }

  void CtcEval::contract(double t, ibex::IntervalVector& z, TubeVector& y, TubeVector& w) const
  {
    DimensionException::check(y, z);
    DimensionException::check(y, w);

    for(int i = 0 ; i < y.size() ; i++)
      contract(t, z[i], y[i], w[i]);
  }

  void CtcEval::contract(ibex::Interval& t, ibex::IntervalVector& z, TubeVector& y, TubeVector& w) const
  {
    DimensionException::check(y, z);
    DimensionException::check(y, w);

    Interval t_result = Interval::EMPTY_SET;

    for(int i = 0 ; i < y.size() ; i++)
    {
      Interval _t(t);
      contract(_t, z[i], y[i], w[i]);
      t_result |= _t;
    }

    t &= t_result;
  }

  void CtcEval::contract(const Interval& t, const IntervalVector& z, TubeVector& y, const TubeVector& w) const
  {
    TubeVector _w(w);
    Interval _t(t); IntervalVector _z(z);
    contract(_t, _z, y, _w);
  }
}