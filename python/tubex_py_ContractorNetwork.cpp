
/** 
 *  \file
 *  ContractorNetwork binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_ContractorNetwork.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "tubex_py_ContractorNetwork_docs.h"
#include "tubex_py_Domain_docs.h"

namespace py = pybind11;
using namespace pybind11::literals;
using py::class_;
using py::init;

using namespace tubex;
using ibex::Vector;
using ibex::Interval;
using ibex::IntervalVector;

std::string to_string(const Domain& x)
{
  std::ostringstream str;
  str << x;
  return str.str();
}

Domain* create_domain_from_vector_as_list(py::list lst)
{
  std::cout << "create_domain_from_vector_as_list " << lst.size() << std::endl;
  if(lst.size() < 1)
    throw std::invalid_argument("Size of the input list is 0");
  
  Vector v(lst.size());
  for(size_t i = 0 ; i < lst.size() ; i++)
    v[i] = lst[i].cast<double>();
  return new Domain(v);
}


class pyContractorNetwork : public ContractorNetwork {
public:
  // pyCtc(int v): Ctc(v){}
  /* Inherit the constructors */
  using ContractorNetwork::ContractorNetwork;

  /* Trampoline (need one for each virtual function) */
  void add_list(ibex::Ctc &,py::list lst)
  {
    std::vector<Domain> v_domains(lst.size());
    for(size_t i = 0 ; i < lst.size() ; i++)
      v_domains[i] = lst[i].cast<Domain>();

    for(const auto& dom : v_domains)
      std::cout << "dom: " << dom << std::endl;

    std::cout << "pyContractorNetwork add" << std::endl;
  };
};

void export_ContractorNetwork(py::module& m){

    py::class_<Domain> domain(m, "Domain", DOCS_DOMAIN);
    domain
      .def(py::init<const Domain &>(),DOCS_DOMAIN_DOMAIN_DOMAIN, "ad"_a.noconvert(),
        py::return_value_policy::reference_internal,
        py::keep_alive<1,2>())
      .def(py::init<double &>(),DOCS_DOMAIN_DOMAIN_DOUBLE, "d"_a.noconvert(),
        py::return_value_policy::reference_internal,
        py::keep_alive<1,2>())
      .def(py::init<ibex::Interval &>(),DOCS_DOMAIN_DOMAIN_INTERVAL, "i"_a.noconvert(),
        py::return_value_policy::reference_internal,
        py::keep_alive<1,2>())
      .def(py::init<ibex::Interval &,double &>(),
        DOCS_DOMAIN_DOMAIN_INTERVAL_DOUBLE, "i"_a, "extern_d"_a.noconvert(),
        py::return_value_policy::reference_internal,
        py::keep_alive<1,2>())
      .def(py::init<ibex::Interval &,ibex::Interval &>(),
        DOCS_DOMAIN_DOMAIN_INTERVAL_INTERVAL, "i"_a, "extern_i"_a.noconvert(),
        py::return_value_policy::reference_internal,
        py::keep_alive<1,2>())
      .def(py::init<const ibex::Interval &>(),DOCS_DOMAIN_DOMAIN_INTERVAL1, "i"_a.noconvert(),
        py::return_value_policy::reference_internal,
        py::keep_alive<1,2>())
      .def(py::init<ibex::Vector &>(),DOCS_DOMAIN_DOMAIN_VECTOR, "v"_a.noconvert(),
        py::return_value_policy::reference_internal,
        py::keep_alive<1,2>())
      .def(py::init(&create_domain_from_vector_as_list), "list"_a.noconvert(),
        py::return_value_policy::reference_internal,
        py::keep_alive<1,2>())
      .def(py::init<ibex::IntervalVector &>(),
        DOCS_DOMAIN_DOMAIN_INTERVALVECTOR, "iv"_a.noconvert(),
        py::return_value_policy::reference_internal,
        py::keep_alive<1,2>())
      .def(py::init<const ibex::IntervalVector &>(),
        DOCS_DOMAIN_DOMAIN_INTERVALVECTOR1, "iv"_a.noconvert(),
        py::return_value_policy::reference_internal,
        py::keep_alive<1,2>())
      .def(py::init<Slice &>(),DOCS_DOMAIN_DOMAIN_SLICE, "s"_a.noconvert(),
        py::return_value_policy::reference_internal,
        py::keep_alive<1,2>())
      .def(py::init<Tube &>(),DOCS_DOMAIN_DOMAIN_TUBE, "t"_a.noconvert(),
        py::return_value_policy::reference_internal,
        py::keep_alive<1,2>())
      .def(py::init<const Tube &>(),DOCS_DOMAIN_DOMAIN_TUBE1, "t"_a.noconvert(),
        py::return_value_policy::reference_internal,
        py::keep_alive<1,2>())
      .def(py::init<TubeVector &>(),DOCS_DOMAIN_DOMAIN_TUBEVECTOR, "tv"_a.noconvert(),
        py::return_value_policy::reference_internal,
        py::keep_alive<1,2>())
      .def(py::init<const TubeVector &>(),DOCS_DOMAIN_DOMAIN_TUBEVECTOR1, "tv"_a.noconvert(),
        py::return_value_policy::reference_internal,
        py::keep_alive<1,2>())
      .def("__repr__", &to_string)
    ;



    py::class_<pyContractorNetwork> cn(m, "ContractorNetwork", DOCS_CONTRACTORNETWORK);
    cn
      .def(py::init<>(),DOCS_CONTRACTORNETWORK_CONTRACTORNETWORK)
      .def("nb_ctc", &ContractorNetwork::nb_ctc,DOCS_CONTRACTORNETWORK_NB_CTC)
      .def("nb_dom", &ContractorNetwork::nb_dom,DOCS_CONTRACTORNETWORK_NB_DOM)
      .def("emptiness", &ContractorNetwork::emptiness,
          DOCS_CONTRACTORNETWORK_EMPTINESS)
      .def("create_dom", (ibex::Interval & (ContractorNetwork::*)(const ibex::Interval &) )&ContractorNetwork::create_dom,
          DOCS_CONTRACTORNETWORK_CREATE_DOM_INTERVAL, "i"_a,
          py::return_value_policy::reference_internal,
          py::keep_alive<1,0>())
      .def("create_dom", (ibex::IntervalVector & (ContractorNetwork::*)(const ibex::IntervalVector &) )&ContractorNetwork::create_dom,
          DOCS_CONTRACTORNETWORK_CREATE_DOM_INTERVALVECTOR, "iv"_a,
          py::return_value_policy::reference_internal,
          py::keep_alive<1,0>())
      .def("create_dom", (Tube & (ContractorNetwork::*)(const Tube &) )&ContractorNetwork::create_dom,
          DOCS_CONTRACTORNETWORK_CREATE_DOM_TUBE, "t"_a,
          py::return_value_policy::reference_internal,
          py::keep_alive<1,0>())
      .def("create_dom", (TubeVector & (ContractorNetwork::*)(const TubeVector &) )&ContractorNetwork::create_dom,
          DOCS_CONTRACTORNETWORK_CREATE_DOM_TUBEVECTOR, "tv"_a,
          py::return_value_policy::reference_internal,
          py::keep_alive<1,0>())
      .def("subvector", (ibex::IntervalVector & (ContractorNetwork::*)(ibex::Vector &,int,int) )&ContractorNetwork::subvector,
          DOCS_CONTRACTORNETWORK_SUBVECTOR_VECTOR_INT_INT, "v"_a, "start_index"_a, "end_index"_a,
          py::return_value_policy::reference_internal,
          py::keep_alive<1,0>())
      .def("subvector", (ibex::IntervalVector & (ContractorNetwork::*)(ibex::IntervalVector &,int,int) )&ContractorNetwork::subvector,
          DOCS_CONTRACTORNETWORK_SUBVECTOR_INTERVALVECTOR_INT_INT, "iv"_a, "start_index"_a, "end_index"_a,
          py::return_value_policy::reference_internal,
          py::keep_alive<1,0>())

      .def("add", (void (ContractorNetwork::*)(ibex::Ctc &,py::list) )&pyContractorNetwork::add_list,
          DOCS_CONTRACTORNETWORK_ADD_CTC_VECTOR_DOMAIN_, "static_ctc"_a, "v_domains"_a)

      
      //.def("add", (void (ContractorNetwork::*)(ibex::Ctc &,const std::vector<Domain> &) )&ContractorNetwork::add,
      //    DOCS_CONTRACTORNETWORK_ADD_CTC_VECTOR_DOMAIN_, "static_ctc"_a, "v_domains"_a,
      //    py::keep_alive<1,2>(),py::keep_alive<1,3>())
      //.def("add", (void (ContractorNetwork::*)(DynCtc &,const std::vector<Domain> &) )&ContractorNetwork::add,
      //    DOCS_CONTRACTORNETWORK_ADD_DYNCTC_VECTOR_DOMAIN_, "dyn_ctc"_a, "v_domains"_a,
      //    py::keep_alive<1,2>(),py::keep_alive<1,3>())
      .def("add_data", (void (ContractorNetwork::*)(Tube &,double,const ibex::Interval &) )&ContractorNetwork::add_data,
          DOCS_CONTRACTORNETWORK_ADD_DATA_TUBE_DOUBLE_INTERVAL, "x"_a, "t"_a, "y"_a)
      .def("add_data", (void (ContractorNetwork::*)(TubeVector &,double,const ibex::IntervalVector &) )&ContractorNetwork::add_data,
          DOCS_CONTRACTORNETWORK_ADD_DATA_TUBEVECTOR_DOUBLE_INTERVALVECTOR, "x"_a, "t"_a, "y"_a)
      .def("contract", &ContractorNetwork::contract,
          DOCS_CONTRACTORNETWORK_CONTRACT_BOOL, "verbose"_a=false)
      .def("contract_during", &ContractorNetwork::contract_during,
          DOCS_CONTRACTORNETWORK_CONTRACT_DURING_DOUBLE_BOOL, "dt"_a, "verbose"_a=false)
      .def("set_fixedpoint_ratio", &ContractorNetwork::set_fixedpoint_ratio,
          DOCS_CONTRACTORNETWORK_SET_FIXEDPOINT_RATIO_FLOAT, "r"_a)
      .def("trigger_all_contractors", &ContractorNetwork::trigger_all_contractors,
          DOCS_CONTRACTORNETWORK_TRIGGER_ALL_CONTRACTORS)
      .def("nb_ctc_in_stack", &ContractorNetwork::nb_ctc_in_stack,
          DOCS_CONTRACTORNETWORK_NB_CTC_IN_STACK)
      .def("set_name", (void (ContractorNetwork::*)(Domain,const std::string &) )&ContractorNetwork::set_name,
          DOCS_CONTRACTORNETWORK_SET_NAME_DOMAIN_STRING, "dom"_a, "name"_a)
      .def("set_name", (void (ContractorNetwork::*)(ibex::Ctc &,const std::string &) )&ContractorNetwork::set_name,
          DOCS_CONTRACTORNETWORK_SET_NAME_CTC_STRING, "ctc"_a, "name"_a)
      .def("set_name", (void (ContractorNetwork::*)(DynCtc &,const std::string &) )&ContractorNetwork::set_name,
          DOCS_CONTRACTORNETWORK_SET_NAME_DYNCTC_STRING, "ctc"_a, "name"_a)
      .def("print_dot_graph", &ContractorNetwork::print_dot_graph,
          DOCS_CONTRACTORNETWORK_PRINT_DOT_GRAPH_STRING_STRING, "cn_name"_a="cn", "layer_model"_a="fdp")
    

;
}