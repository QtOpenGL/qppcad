#include <qppcad/sflow/sflow_base_nodes.hpp>

using namespace qpp;
using namespace qpp::cad;

sf_int_prop_node_t::sf_int_prop_node_t() : sflow_node_t () {

  m_node_name = "int";

  m_out_types = {
    {sflow_parameter_e::sfpar_int, 0, ""}
  };

  m_ipl_types = {
    {sflow_parameter_e::sfpar_int, "value", true}
  };

}

bool sf_int_prop_node_t::execute_ex() {

  auto out0 = get_pars_as<sflow_parameter_int_t>(0, m_outs);
  auto ipl0 = get_pars_as<sflow_parameter_int_t>(0, m_ipl);

  out0->m_value = ipl0->m_value;

  return true;

}

bool sf_int_prop_node_t::is_single_node() {
  return true;
}

sf_int_final_node_t::sf_int_final_node_t() : sflow_node_t () {

  m_node_name = "show int";

  m_inp_types = {
    {sflow_parameter_e::sfpar_int, 0, ""}
  };

  m_ipl_types = {
    {sflow_parameter_e::sfpar_int, "value", false}
  };

}

bool sf_int_final_node_t::execute_ex() {

  auto inp0 = get_pars_as<sflow_parameter_int_t>(0, m_inps);
  auto ipl0 = get_pars_as<sflow_parameter_int_t>(0, m_ipl);

  if (inp0 && ipl0) {
      ipl0->m_value = inp0->m_value;
      fmt::print(std::cout, "NODE_MESSAGE {}\n", ipl0->m_value);
      return true;
    } else {
      return false;
    }

  return true;

}

bool sf_int_final_node_t::is_single_node() {
  return true;
}

sf_int_p_const_node_t::sf_int_p_const_node_t() : sflow_node_t () {

  m_node_name = "int + const";

  m_out_types = {
   {sflow_parameter_e::sfpar_int, 0, "a"}
  };

  m_inp_types = {
    {sflow_parameter_e::sfpar_int, 0, "b"}
  };

  m_ipl_types = {
    {sflow_parameter_e::sfpar_int, "const", true}
  };

}

bool sf_int_p_const_node_t::execute_ex() {

  auto out0 = get_pars_as<sflow_parameter_int_t>(0, m_outs);
  auto inp0 = get_pars_as<sflow_parameter_int_t>(0, m_inps);
  auto ipl0 = get_pars_as<sflow_parameter_int_t>(0, m_ipl);

  if (out0 && inp0 && ipl0) {
      out0->m_value = inp0->m_value + ipl0->m_value;
    } else {
      return false;
    }

  return true;

}

sf_int_sum_i_node_t::sf_int_sum_i_node_t() {

  m_node_name = "int + int";

  m_out_types = {
   {sflow_parameter_e::sfpar_int, 0, "dst"}
  };

  m_inp_types = {
    {sflow_parameter_e::sfpar_int, 0, "src1"},
    {sflow_parameter_e::sfpar_int, 0, "src2"}
  };

}

bool sf_int_sum_i_node_t::execute_ex() {

  auto out0 = get_pars_as<sflow_parameter_int_t>(0, m_outs);
  auto inp0 = get_pars_as<sflow_parameter_int_t>(0, m_inps);
  auto inp1 = get_pars_as<sflow_parameter_int_t>(1, m_inps);

  if (out0 && inp0 && inp1) {
      out0->m_value = inp0->m_value + inp1->m_value;
    } else {
      return false;
    }

  return true;

}

sf_float_prop_node_t::sf_float_prop_node_t() {

  m_node_name = "float";

  m_out_types = {
    {sflow_parameter_e::sfpar_float, 0, "dst"}
  };

  m_ipl_types = {
    {sflow_parameter_e::sfpar_float, "value", true}
  };

}

bool sf_float_prop_node_t::execute_ex() {

  auto out0 = get_pars_as<sflow_parameter_float_t>(0, m_outs);
  auto ipl0 = get_pars_as<sflow_parameter_float_t>(0, m_ipl);

  out0->m_value = ipl0->m_value;

  return true;

}

bool sf_float_prop_node_t::is_single_node() {
  return true;
}

sf_float_p_const_node_t::sf_float_p_const_node_t() {

  m_node_name = "float + const";

  m_out_types = {
    {sflow_parameter_e::sfpar_float, 0, "dst"}
  };

  m_inp_types = {
   {sflow_parameter_e::sfpar_float, 0, "src"}
  };

}

bool sf_float_p_const_node_t::execute_ex() {

  auto out0 = get_pars_as<sflow_parameter_float_t>(0, m_outs);
  auto inp0 = get_pars_as<sflow_parameter_float_t>(0, m_inps);
  auto ipl0 = get_pars_as<sflow_parameter_float_t>(0, m_ipl);

  if (out0 && inp0 && ipl0) {
      out0->m_value = inp0->m_value + ipl0->m_value;
    } else {
      return false;
    }

  return true;

}

sf_int_patb_const_node_t::sf_int_patb_const_node_t() {

  m_node_name = "(int + a) * b";

  m_out_types = {
   {sflow_parameter_e::sfpar_int, 0, "src"}
  };

  m_inp_types = {
    {sflow_parameter_e::sfpar_int, 0, "dst"}
  };

  m_ipl_types = {
    {sflow_parameter_e::sfpar_int, "a", true},
    {sflow_parameter_e::sfpar_int, "b", true}
  };

}

bool sf_int_patb_const_node_t::execute_ex() {

  auto out0 = get_pars_as<sflow_parameter_int_t>(0, m_outs);
  auto inp0 = get_pars_as<sflow_parameter_int_t>(0, m_inps);
  auto ipl0 = get_pars_as<sflow_parameter_int_t>(0, m_ipl);
  auto ipl1 = get_pars_as<sflow_parameter_int_t>(1, m_ipl);

  if (out0 && inp0 && ipl0 && ipl1) {
      out0->m_value = (inp0->m_value + ipl0->m_value) * ipl1->m_value;
    } else {
      return false;
    }

  return true;

}

sf_float_final_node_t::sf_float_final_node_t() {

  m_node_name = "show float";

  m_inp_types = {
    {sflow_parameter_e::sfpar_float, 0, ""}
  };

  m_ipl_types = {
    {sflow_parameter_e::sfpar_float, "value", false}
  };

}

bool sf_float_final_node_t::execute_ex() {

  auto inp0   = get_pars_as<sflow_parameter_float_t>(0, m_inps);
  auto inp_p0 = get_pars_as<sflow_parameter_float_t>(0, m_ipl);

  if (inp0 && inp_p0) {
      inp_p0->m_value = inp0->m_value;
      return true;
    } else {
      return false;
    }

  return true;

}

bool sf_float_final_node_t::is_single_node() {
  return true;
}
