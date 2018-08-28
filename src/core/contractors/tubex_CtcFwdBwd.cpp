/* ============================================================================
 *  tubex-lib - CtcFwdBwd class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_CtcFwdBwd.h"
#include "tubex_SlicingException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // todo: avoid redundant gate contractions
  
  CtcFwdBwd::CtcFwdBwd(const tubex::Function& f)
    : Ctc(), m_f(new tubex::Function(f))
  {

  }
  
  CtcFwdBwd::~CtcFwdBwd()
  {
    delete m_f;
  }

  void CtcFwdBwd::contract(TubeVector& x)
  {
    // todo: check x size
    ibex::CtcFwdBwd ctc_fwdbwd(m_f->ibex_function()); // todo: build the following in constructor? (bug)

    // todo: check if the following is needed
    //if(x.is_empty())
    //  return;

    Slice **v_x_slices = new Slice*[x.size()];
    for(int i = 0 ; i < x.size() ; i++)
      v_x_slices[i] = x[i].get_first_slice();

    IntervalVector envelope(x.size() + 1);
    IntervalVector ingate(x.size() + 1);

    while(v_x_slices[0] != NULL)
    {
      envelope[0] = v_x_slices[0]->domain();
      ingate[0] = v_x_slices[0]->domain().lb();

      for(int i = 0 ; i < x.size() ; i++)
      {
        envelope[i+1] = v_x_slices[i]->codomain();
        ingate[i+1] = v_x_slices[i]->input_gate();
      }

      ctc_fwdbwd.contract(envelope);
      ctc_fwdbwd.contract(ingate);

      for(int i = 0 ; i < x.size() ; i++)
      {
        v_x_slices[i]->set_envelope(envelope[i+1]);
        v_x_slices[i]->set_input_gate(ingate[i+1]);
      }
      
      for(int i = 0 ; i < x.size() ; i++)
        v_x_slices[i] = v_x_slices[i]->next_slice();
    }

    IntervalVector outgate(x.size() + 1);

    for(int i = 0 ; i < x.size() ; i++)
      v_x_slices[i] = x[i].get_last_slice();

    outgate[0] = v_x_slices[0]->domain().ub();
    for(int i = 0 ; i < x.size() ; i++)
      outgate[i+1] = v_x_slices[i]->output_gate();

    ctc_fwdbwd.contract(outgate);

    for(int i = 0 ; i < x.size() ; i++)
      v_x_slices[i]->set_output_gate(outgate[i+1]);
  }
}