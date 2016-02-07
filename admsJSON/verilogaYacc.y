/***
This is a derivative work of ADMS.  Original code is copyight the original authors.
Modifications Copyright Luther T. Cat

ADMS
Copyright (C) 2015  Luther T. Cat
Original code copyright the original authors.

This file is part of ADMS.

ADMS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 2 of the License.

ADMS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ADMS.  If not, see <http://www.gnu.org/licenses/>.
***/

/* File automatically created by ./mkgrammar.pl */

%{
#define YYDEBUG 1
#include "adms_yaccval.h"
#include "admsVeriloga.h"
#include "adms_message.h"
#include "adms_admsmain.h"
#include "adms_math.h"
#include "adms_discipline.h"
#include "adms_lexval.h"
#include "adms_nature.h"
#include "adms_module.h"
#include "adms_node.h"
#include "adms_analogfunction.h"
#include "adms_variable.h"
#include "adms_range.h"
#include "adms_source.h"
#include "adms_analog.h"
#include "adms_attribute.h"
#include "adms_instance.h"
#include "adms_block.h"

#include <cassert>
#include <list>
/*TODO: put all of these into a global context class to control initialization*/
#if 0
inline static void   Y (p_yaccval myyaccval,p_adms myusrdata) {myyaccval->_usrdata=myusrdata;}
inline static p_adms YY(p_yaccval myyaccval)                  {return myyaccval->_usrdata;}
#endif

struct my_va_global {
  my_va_global() :
    gDiscipline(nullptr),
    gModule(nullptr),
    gAnalogfunction(nullptr),
    gInstanceModule(nullptr),
    gGND(nullptr),
    gSource(nullptr),
    gLexval(nullptr),
    gContribution(nullptr),
    gVariableType(admse_real),
    uid(0)
  {}

std::string gNatureAccess;
p_number    gNatureAbsTol;
std::string gNatureUnits;
std::string gNatureidt;
std::string gNatureddt;
p_discipline gDiscipline;
p_module gModule;
p_analogfunction gAnalogfunction;
p_module gInstanceModule;
p_node gGND;
p_source gSource;
p_lexval gLexval;
p_contribution gContribution;
admse gVariableType;
admse gNodeDirection;
variableprototype_list gVariableDeclarationList;
instanceparameter_list gInstanceVariableList;
node_list              gTerminalList;
std::list<std::string> gBranchAliasList;
range_list             gRangeList;
node_list              gNodeList;
attribute_list         gAttributeList;
attribute_list         gGlobalAttributeList;
p_adms_list            gBlockList;
variableprototype_list gBlockVariableList;
p_branchalias          gBranchAlias;
int uid;
} mc;

//#define NEWVARIABLE(l) p_variableprototype myvariableprototype=adms_variableprototype_new(mc.gModule,l,mc.gBlockList.back());
p_variableprototype new_variable(p_lexval l)
{
  return adms_variableprototype_new(mc.gModule,l,mc.gBlockList.back());
}

namespace {
template <typename T>
void extend_list_by_move(std::list<T> &to, std::list<T> &from)
{
  to.splice(to.end(), from);
}

template <typename T>
void extend_list_by_copy(std::list<T> &to, std::list<T> &from)
{
  to.insert(to.end(), from.begin(), from.end());
}

}

static void adms_veriloga_message_fatal_continue(const p_lexval mytoken)
{
  adms_message_fatal_continue("[%s:%i:%i]: at '%s':\n",mytoken->_f.c_str(),mytoken->_l,mytoken->_c,mytoken->_string.c_str());
}
static void adms_veriloga_message_fatal (const char* message,const p_lexval mytoken)
{
  adms_veriloga_message_fatal_continue(mytoken);
  adms_message_fatal(message);
}

static p_variable variable_recursive_lookup_by_id (p_adms myadms,p_lexval mylexval)
{
  if(myadms==mc.gModule)
  {
    p_variable myvariable=nullptr;
    p_variableprototype myvariableprototype;
    if((myvariableprototype=adms_module_list_variable_lookup_by_id(mc.gModule,mc.gModule,mylexval,mc.gModule)))
    {
      myvariable=adms_variable_new(myvariableprototype);
      myvariableprototype->_instance.push_back(myvariable);
    }
    return myvariable;
  }
  else if(myadms==mc.gAnalogfunction)
  {
    p_variable myvariable=nullptr;
    p_variableprototype myvariableprototype;
    if((myvariableprototype=adms_analogfunction_list_variable_lookup_by_id(mc.gAnalogfunction,mc.gModule,mylexval,mc.gAnalogfunction)))
    {
      myvariable=adms_variable_new(myvariableprototype);
      myvariableprototype->_instance.push_back(myvariable);
    }
    return myvariable;
  }
  else
  {
    p_block b = dynamic_cast<p_block>(myadms);
    assert(b);
    
    for (auto l : b->_variable)
    {
      if (b->_lexval->_string == mylexval->_string)
      {
        p_variableprototype myvariableprototype=l;
        p_variable myvariable=adms_variable_new(myvariableprototype);
        myvariableprototype->_instance.push_back(myvariable);
        return myvariable;
      }
    }
    return variable_recursive_lookup_by_id(b->_block,mylexval);
  }
}
static p_nature lookup_nature(const std::string &myname)
{
  p_nature ret = nullptr;
  nature_list &list=root()->_nature;

  auto it = std::find_if(list.begin(), list.end(),
    [&](p_nature n)->bool {return n->_name == myname;});

  if (it != list.end())
  {
    ret = *it;
  }
  return ret;
}

%}

%left PREC_IF_THEN
%left tk_else

%union
{
  p_lexval _lexval;
  p_yaccval _yaccval;
}

%token <_lexval> tk_beginattribute
%token <_lexval> tk_else
%token <_lexval> tk_module
%token <_lexval> tk_or
%token <_lexval> tk_inf
%token <_lexval> tk_from
%token <_lexval> tk_analog
%token <_lexval> tk_number
%token <_lexval> tk_aliasparameter
%token <_lexval> tk_anytext
%token <_lexval> tk_default
%token <_lexval> tk_case
%token <_lexval> tk_end
%token <_lexval> tk_domain
%token <_lexval> tk_integer
%token <_lexval> tk_parameter
%token <_lexval> tk_input
%token <_lexval> tk_output
%token <_lexval> tk_endfunction
%token <_lexval> tk_op_shl
%token <_lexval> tk_op_shr
%token <_lexval> tk_function
%token <_lexval> tk_aliasparam
%token <_lexval> tk_string
%token <_lexval> tk_real
%token <_lexval> tk_anystring
%token <_lexval> tk_begin
%token <_lexval> tk_endcase
%token <_lexval> tk_if
%token <_lexval> tk_inout
%token <_lexval> tk_exclude
%token <_lexval> tk_ident
%token <_lexval> tk_ground
%token <_lexval> tk_endnature
%token <_lexval> tk_discipline
%token <_lexval> tk_branch
%token <_lexval> tk_flow
%token <_lexval> tk_enddiscipline
%token <_lexval> tk_dollar_ident
%token <_lexval> tk_nature
%token <_lexval> tk_bitwise_equr
%token <_lexval> tk_and
%token <_lexval> tk_potential
%token <_lexval> tk_for
%token <_lexval> tk_while
%token <_lexval> tk_char
%token <_lexval> tk_endmodule
%token <_lexval> tk_endattribute

%type <_yaccval> R_admsParse
%type <_yaccval> R_l__admsParse
%type <_yaccval> R_s__admsParse
%type <_yaccval> R_discipline_member
%type <_yaccval> R_discipline_name
%type <_yaccval> R_l__discipline_assignment
%type <_yaccval> R_s__discipline_assignment
%type <_yaccval> R_discipline__naturename
%type <_yaccval> R_nature_member
%type <_yaccval> R_l__nature_assignment
%type <_yaccval> R_s__nature_assignment
%type <_yaccval> R_d__attribute__0
%type <_yaccval> R_d__attribute
%type <_yaccval> R_l__attribute
%type <_yaccval> R_s__attribute
%type <_yaccval> R_d__module
%type <_yaccval> R_modulebody
%type <_yaccval> R_netlist
%type <_yaccval> R_l__instance
%type <_yaccval> R_d__terminal
%type <_yaccval> R_l__terminal__0
%type <_yaccval> R_l__terminal
%type <_yaccval> R_s__terminal
%type <_yaccval> R_l__declaration
%type <_yaccval> R_s__declaration__withattribute
%type <_yaccval> R_d__attribute__global
%type <_yaccval> R_s__declaration
%type <_yaccval> R_d__node
%type <_yaccval> R_node__type
%type <_yaccval> R_l__terminalnode
%type <_yaccval> R_l__node
%type <_yaccval> R_s__terminalnode
%type <_yaccval> R_s__node
%type <_yaccval> R_d__branch
%type <_yaccval> R_l__branchalias
%type <_yaccval> R_s__branchalias
%type <_yaccval> R_s__branch
%type <_yaccval> R_d__analogfunction
%type <_yaccval> R_d__analogfunction__proto
%type <_yaccval> R_d__analogfunction__name
%type <_yaccval> R_l__analogfunction__declaration
%type <_yaccval> R_s__analogfunction__declaration
%type <_yaccval> R_l__analogfunction__input__variable
%type <_yaccval> R_l__analogfunction__output__variable
%type <_yaccval> R_l__analogfunction__inout__variable
%type <_yaccval> R_l__analogfunction__integer__variable
%type <_yaccval> R_l__analogfunction__real__variable
%type <_yaccval> R_variable__type
%type <_yaccval> R_d__variable__end
%type <_yaccval> R_l__parameter
%type <_yaccval> R_l__variable
%type <_yaccval> R_d__aliasparameter
%type <_yaccval> R_d__aliasparameter__token
%type <_yaccval> R_s__parameter
%type <_yaccval> R_s__variable
%type <_yaccval> R_s__parameter__name
%type <_yaccval> R_s__variable__name
%type <_yaccval> R_s__parameter__range
%type <_yaccval> R_l__interval
%type <_yaccval> R_s__interval
%type <_yaccval> R_d__interval
%type <_yaccval> R_interval__inf
%type <_yaccval> R_interval__sup
%type <_yaccval> R_analog
%type <_yaccval> R_analogcode
%type <_yaccval> R_l__expression
%type <_yaccval> R_analogcode__atomic
%type <_yaccval> R_analogcode__block
%type <_yaccval> R_analogcode__block__atevent
%type <_yaccval> R_l__analysis
%type <_yaccval> R_s__analysis
%type <_yaccval> R_d__block
%type <_yaccval> R_d__block__begin
%type <_yaccval> R_l__blockitem
%type <_yaccval> R_d__blockvariable
%type <_yaccval> R_l__blockvariable
%type <_yaccval> R_s__blockvariable
%type <_yaccval> R_d__contribution
%type <_yaccval> R_contribution
%type <_yaccval> R_source
%type <_yaccval> R_d__while
%type <_yaccval> R_d__for
%type <_yaccval> R_d__case
%type <_yaccval> R_l__case__item
%type <_yaccval> R_s__case__item
%type <_yaccval> R_s__instance
%type <_yaccval> R_instance__module__name
%type <_yaccval> R_l__instance__parameter
%type <_yaccval> R_s__instance__parameter
%type <_yaccval> R_s__assignment
%type <_yaccval> R_d__conditional
%type <_yaccval> R_s__expression
%type <_yaccval> R_l__enode
%type <_yaccval> R_s__function_expression
%type <_yaccval> R_expression
%type <_yaccval> R_e__conditional
%type <_yaccval> R_e__bitwise_equ
%type <_yaccval> R_e__bitwise_xor
%type <_yaccval> R_e__bitwise_or
%type <_yaccval> R_e__bitwise_and
%type <_yaccval> R_e__logical_or
%type <_yaccval> R_e__logical_and
%type <_yaccval> R_e__comp_equ
%type <_yaccval> R_e__comp
%type <_yaccval> R_e__bitwise_shift
%type <_yaccval> R_e__arithm_add
%type <_yaccval> R_e__arithm_mult
%type <_yaccval> R_e__unary
%type <_yaccval> R_e__atomic

%%
R_admsParse
        : R_l__admsParse
          {
          }
        ;
R_l__admsParse
        : R_s__admsParse
          {
          }
        | R_l__admsParse R_s__admsParse
          {
          }
        ;
R_s__admsParse
        : R_d__module
          {
          }
        | R_discipline_member
          {
          }
        | R_nature_member
          {
          }
        ;
R_discipline_member
        : tk_discipline R_discipline_name R_l__discipline_assignment tk_enddiscipline
          {
            adms_admsmain_list_discipline_prepend_once_or_abort(root(),mc.gDiscipline);
            mc.gDiscipline=nullptr;
          }
        ;
R_discipline_name
        : tk_ident
          {
            const std::string &mylexval1=$1->_string;
            mc.gDiscipline=adms_discipline_new(mylexval1.c_str());
          }
        ;
R_l__discipline_assignment
        : R_s__discipline_assignment
          {
          }
        | R_l__discipline_assignment R_s__discipline_assignment
          {
          }
        ;
R_s__discipline_assignment
        : tk_potential R_discipline__naturename ';'
          {
            mc.gDiscipline->_potential=$2->get_p<p_nature>();
          }
        | tk_flow R_discipline__naturename ';'
          {
            mc.gDiscipline->_flow=$2->get_p<p_nature>();
          }
        | tk_domain tk_ident ';'
          {
            std::string mylexval2=$2->_string;
            if(mylexval2 == "discrete")
              mc.gDiscipline->_domain=admse_discrete;
            else if(mylexval2 == "continuous")
              mc.gDiscipline->_domain=admse_continuous;
            else
             adms_veriloga_message_fatal("domain: bad value given - should be either 'discrete' or 'continuous'\n",$2);
          }
        ;
R_discipline__naturename
        : tk_ident
          {
            std::string mylexval1=$1->_string.c_str();
            p_nature mynature=lookup_nature(mylexval1);
            if(!mynature)
              adms_veriloga_message_fatal("can't find nature definition\n",$1);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(mynature);
          }
        ;
R_nature_member
        : tk_nature tk_ident R_l__nature_assignment tk_endnature
          {
            std::string mylexval2=$2->_string.c_str();
            p_nature mynature=nullptr;
            if(!mc.gNatureAccess.empty()) 
            {
              mynature=adms_admsmain_list_nature_prepend_by_id_once_or_abort(root(),mc.gNatureAccess);
            }
            else
             adms_veriloga_message_fatal("attribute 'access' in nature definition not found\n",$2);

            mynature->_name = mylexval2;
            mynature->_idt_name=mc.gNatureidt;
            mynature->_ddt_name=mc.gNatureddt;
            mynature->_units=mc.gNatureUnits;
            mynature->_abstol=mc.gNatureAbsTol;

            mc.gNatureAccess.clear();
            mc.gNatureAbsTol=nullptr;
            mc.gNatureUnits.clear();
            mc.gNatureidt.clear();
            mc.gNatureddt.clear();
          }
        ;
R_l__nature_assignment
        : R_s__nature_assignment
          {
          }
        | R_l__nature_assignment R_s__nature_assignment
          {
          }
        ;
R_s__nature_assignment
        : tk_ident '=' tk_number ';'
          {
            if($1->_string == "abstol")
            {
              if(mc.gNatureAbsTol)
                adms_veriloga_message_fatal("nature attribute defined more than once\n",$1);
              mc.gNatureAbsTol=adms_number_new($3);
            }
            else
             adms_veriloga_message_fatal("unknown nature attribute\n",$1);
          }
        | tk_ident '=' tk_number tk_ident ';'
          {
            std::string mylexval4=$4->_string;
            if($1->_string == "abstol")
            {
              if(mc.gNatureAbsTol)
                adms_veriloga_message_fatal("nature attribute defined more than once\n",$1);
              mc.gNatureAbsTol=adms_number_new($3);
            }
            else
             adms_veriloga_message_fatal("unknown nature attribute\n",$1);

            if (!is_valid_scaling_unit(mylexval4))
            {
              adms_veriloga_message_fatal("can not convert symbol to valid unit\n",$4);
            }
            mc.gNatureAbsTol->_scalingunit=mylexval4;
          }
        | tk_ident '=' tk_anystring ';'
          {
            std::string mylexval3=$3->_string;
            if($1->_string == "units")
            {
              if (!mc.gNatureUnits.empty())
              {
                adms_veriloga_message_fatal("nature attribute defined more than once\n",$1);
              }
              mc.gNatureUnits=mylexval3;
            }
            else
             adms_veriloga_message_fatal("unknown nature attribute\n",$1);
          }
        | tk_ident '=' tk_ident ';'
          {
            std::string mylexval3=$3->_string;
            if($1->_string == "access")
            {
              if (!mc.gNatureAccess.empty())
              {
                adms_veriloga_message_fatal("nature attribute defined more than once\n",$1);
              }
              mc.gNatureAccess=mylexval3;
            }
            else if($1->_string == "idt_nature")
            {
              if(!mc.gNatureidt.empty())
              {
                adms_veriloga_message_fatal("idt_nature attribute defined more than once\n",$1);
              }
              mc.gNatureidt=mylexval3;
            }
            else if($1->_string == "ddt_nature")
            {
              if(!mc.gNatureddt.empty())
              {
                adms_veriloga_message_fatal("ddt_nature attribute defined more than once\n",$1);
              }
              mc.gNatureddt=mylexval3;
            }
            else
             adms_veriloga_message_fatal("unknown nature attribute\n",$1);
          }
        ;
R_d__attribute__0
        : 
          {
          }
        | R_d__attribute
          {
          }
        ;
R_d__attribute
        : tk_beginattribute R_l__attribute tk_endattribute
          {
          }
        | tk_beginattribute tk_anytext
          {
            std::string mylexval2=$2->_string;
            p_attribute myattribute=adms_attribute_new("ibm", mylexval2);
            mc.gAttributeList.push_back(myattribute);
          }
        | tk_beginattribute tk_endattribute
          {
          }
        ;
R_l__attribute
        : R_s__attribute
          {
          }
        | R_l__attribute R_s__attribute
          {
          }
        ;
R_s__attribute
        : tk_ident '=' tk_anystring
          {
            std::string mylexval1=$1->_string;
            std::string mylexval3=$3->_string;
            p_attribute myattribute=adms_attribute_new(mylexval1, mylexval3);
            mc.gAttributeList.push_back(myattribute);
          }
        ;
R_d__module
        : R_d__attribute__0 tk_module tk_ident
          {
            std::string mylexval3=$3->_string;
            p_nodealias mynodealias;

            mc.gModule=adms_admsmain_list_module_prepend_by_id_once_or_abort(root(),mylexval3); 

            mc.gBlockList.push_back(mc.gModule);
            mynodealias=adms_module_list_nodealias_prepend_by_id_once_or_abort(mc.gModule,mc.gModule,"0"); 
            mc.gGND=adms_module_list_node_prepend_by_id_once_or_abort(mc.gModule,mc.gModule,"GND"); 
            mynodealias->_node=mc.gGND;
            mc.gGND->_location=admse_ground;
            extend_list_by_move(mc.gModule->_attribute, mc.gAttributeList);
          }
         R_d__terminal R_modulebody tk_endmodule
          {
            mc.gBlockList.pop_back();
          }
        ;
R_modulebody
        : 
          {
          }
        | R_l__declaration
          {
          }
        | R_netlist
          {
          }
        | R_l__declaration R_netlist
          {
          }
        ;
R_netlist
        : R_analog
          {
          }
        | R_l__instance
          {
          }
        | R_l__instance R_analog
          {
          }
        | R_analog R_l__instance
          {
          }
        | R_l__instance R_analog R_l__instance
          {
          }
        ;
R_l__instance
        : R_s__instance
          {
          }
        | R_l__instance R_s__instance
          {
          }
        ;
R_d__terminal
        : '(' R_l__terminal__0 ')' R_d__attribute__0 ';'
          {
            extend_list_by_move(mc.gModule->_attribute, mc.gAttributeList);
          }
        ;
R_l__terminal__0
        : 
          {
          }
        | R_l__terminal
          {
          }
        ;
R_l__terminal
        : R_s__terminal
          {
          }
        | R_l__terminal ',' R_s__terminal
          {
          }
        ;
R_s__terminal
        : tk_ident
          {
            std::string mylexval1=$1->_string;
            p_nodealias mynodealias=adms_module_list_nodealias_prepend_by_id_once_or_abort(mc.gModule,mc.gModule,mylexval1); 
            p_node mynode=adms_module_list_node_prepend_by_id_once_or_abort(mc.gModule,mc.gModule,mylexval1); 
            mynodealias->_node=mynode;
            mynode->_location=admse_external;
          }
        ;
R_l__declaration
        : R_s__declaration__withattribute
          {
          }
        | R_l__declaration R_s__declaration__withattribute
          {
          }
        ;
R_s__declaration__withattribute
        : R_s__declaration
          {
          }
        | R_d__attribute__global R_s__declaration
          {
            mc.gGlobalAttributeList.clear();
          }
        ;
R_d__attribute__global
        : R_d__attribute
          {
            extend_list_by_move(mc.gGlobalAttributeList, mc.gAttributeList);
          }
        ;
R_s__declaration
        : R_d__node
          {
          }
        | R_d__branch
          {
          }
        | tk_parameter R_variable__type R_l__parameter R_d__variable__end
          {
          }
        | tk_parameter R_l__parameter R_d__variable__end
          {
          }
        | R_variable__type R_l__variable R_d__variable__end
          {
          }
        | R_d__aliasparameter
          {
          }
        | R_d__analogfunction
          {
          }
        | ';'
          {
          }
        ;
R_d__node
        : R_node__type R_l__terminalnode ';'
          {
            for (auto l : mc.gTerminalList)
            {
              l->_direction=mc.gNodeDirection;
            }
            mc.gTerminalList.clear();
          }
        | tk_ground R_l__node ';'
          {
            for (auto l : mc.gNodeList)
            {
              l->_location=admse_ground;
            }
            mc.gNodeList.clear();
          }
        | tk_ident R_l__node ';'
          {
            std::string mylexval1=$1->_string;
            std::string mydisciplinename=mylexval1;
            p_discipline mydiscipline=adms_admsmain_list_discipline_lookup_by_id(root(), mydisciplinename);
            for (auto l : mc.gNodeList)
            {
              l->_discipline=mydiscipline;
            }
            mc.gNodeList.clear();
          }
        ;
R_node__type
        : tk_input
          {
            mc.gNodeDirection=admse_input;
          }
        | tk_output
          {
            mc.gNodeDirection=admse_output;
          }
        | tk_inout
          {
            mc.gNodeDirection=admse_inout;
          }
        ;
R_l__terminalnode
        : R_s__terminalnode
          {
          }
        | R_l__terminalnode ',' R_s__terminalnode
          {
          }
        ;
R_l__node
        : R_s__node
          {
          }
        | R_l__node ',' R_s__node
          {
          }
        ;
R_s__terminalnode
        : tk_ident R_d__attribute__0
          {
            std::string mylexval1=$1->_string;
            p_node mynode=adms_module_list_node_lookup_by_id(mc.gModule,mc.gModule,mylexval1);
            if(!mynode)
             adms_veriloga_message_fatal("terminal not found\n",$1);
            if(mynode->_location!=admse_external)
             adms_veriloga_message_fatal("node not a terminal\n",$1);
            mc.gTerminalList.push_back(mynode);

            extend_list_by_move(mynode->_attribute, mc.gAttributeList);
            extend_list_by_copy(mynode->_attribute, mc.gGlobalAttributeList);
          }
        ;
R_s__node
        : tk_ident R_d__attribute__0
          {
            std::string mylexval1=$1->_string;
            p_node mynode=adms_module_list_node_prepend_by_id_once_or_ignore(mc.gModule,mc.gModule,mylexval1);
            mc.gNodeList.push_back(mynode);
            extend_list_by_move(mynode->_attribute, mc.gAttributeList);
            extend_list_by_copy(mynode->_attribute, mc.gGlobalAttributeList);
          }
        ;
R_d__branch
        : tk_branch R_s__branch ';'
          {
          }
        ;
R_l__branchalias
        : R_s__branchalias
          {
          }
        | R_l__branchalias ',' R_s__branchalias
          {
          }
        ;
R_s__branchalias
        : tk_ident
          {
            std::string mylexval1=$1->_string;
            mc.gBranchAliasList.push_back(mylexval1);
          }
        ;
R_s__branch
        : '(' tk_ident ',' tk_ident ')' R_l__branchalias
          {
            std::string mylexval2=$2->_string;
            std::string mylexval4=$4->_string;
            p_branch mybranch; 
            p_node pnode=adms_module_list_node_lookup_by_id(mc.gModule,mc.gModule,mylexval2);
            p_node nnode=adms_module_list_node_lookup_by_id(mc.gModule,mc.gModule,mylexval4);
            mybranch=adms_module_list_branch_prepend_by_id_once_or_ignore(mc.gModule,mc.gModule,pnode,nnode); 
            if(!pnode)
             adms_veriloga_message_fatal("node never declared\n",$2);
            if(!nnode)
             adms_veriloga_message_fatal("node never declared\n",$4);

            for(const std::string &aliasname : mc.gBranchAliasList)
            {
              p_branchalias mybranchalias; 
              mybranchalias=adms_module_list_branchalias_prepend_by_id_once_or_abort(mc.gModule,mc.gModule,aliasname); 
              if(mybranchalias) mybranchalias->_branch=mybranch;
            }
            mc.gBranchAliasList.clear();
            extend_list_by_copy(mybranch->_attribute, mc.gGlobalAttributeList);
          }
        | '(' tk_ident ')' R_l__branchalias
          {
            std::string mylexval2=$2->_string;
            p_branch mybranch;
            p_node pnode=adms_module_list_node_lookup_by_id(mc.gModule,mc.gModule,mylexval2);
            if(!pnode)
             adms_veriloga_message_fatal("node never declared\n",$2);
            mybranch=adms_module_list_branch_prepend_by_id_once_or_ignore(mc.gModule,mc.gModule,pnode,mc.gGND); 
            for(const std::string &aliasname : mc.gBranchAliasList)
            {
              p_branchalias mybranchalias; 
              mybranchalias=adms_module_list_branchalias_prepend_by_id_once_or_abort(mc.gModule,mc.gModule,aliasname); 
              if (mybranchalias)
              {
                mybranchalias->_branch=mybranch;
              }
            }
            mc.gBranchAliasList.clear();
            extend_list_by_copy(mybranch->_attribute, mc.gGlobalAttributeList);
          }
        ;
R_d__analogfunction
        : R_d__analogfunction__proto R_l__analogfunction__declaration R_analogcode__block tk_endfunction
          {
            mc.gBlockList.pop_back();
            mc.gAnalogfunction->_tree=$3->get_p<p_adms>();
            mc.gAnalogfunction=nullptr;
          }
        ;
R_d__analogfunction__proto
        : tk_analog tk_function R_d__analogfunction__name ';'
          {
            p_variableprototype myvariableprototype=new_variable(mc.gAnalogfunction->_lexval);
            adms_analogfunction_list_variable_prepend_once_or_abort(mc.gAnalogfunction,myvariableprototype); 
            myvariableprototype->_output=admse_yes;
          }
        | tk_analog tk_function tk_integer R_d__analogfunction__name ';'
          {
            p_variableprototype myvariableprototype=new_variable(mc.gAnalogfunction->_lexval);
            adms_analogfunction_list_variable_prepend_once_or_abort(mc.gAnalogfunction,myvariableprototype); 
            myvariableprototype->_output=admse_yes;
            myvariableprototype->_type=admse_integer;
            mc.gAnalogfunction->_type=admse_integer; 
          }
        | tk_analog tk_function tk_real R_d__analogfunction__name ';'
          {
            p_variableprototype myvariableprototype=new_variable(mc.gAnalogfunction->_lexval);
            adms_analogfunction_list_variable_prepend_once_or_abort(mc.gAnalogfunction,myvariableprototype); 
            myvariableprototype->_output=admse_yes;
          }
        ;
R_d__analogfunction__name
        : tk_ident
          {
            mc.gAnalogfunction=adms_analogfunction_new(mc.gModule,$1);
            mc.gBlockList.push_back(mc.gAnalogfunction);
            adms_module_list_analogfunction_prepend_once_or_abort(mc.gModule, mc.gAnalogfunction); 
            extend_list_by_copy(mc.gAnalogfunction->_attribute, mc.gGlobalAttributeList);
          }
        ;
R_l__analogfunction__declaration
        : R_s__analogfunction__declaration
          {
          }
        | R_l__analogfunction__declaration R_s__analogfunction__declaration
          {
          }
        ;
R_s__analogfunction__declaration
        : tk_input R_l__analogfunction__input__variable ';'
          {
          }
        | tk_output R_l__analogfunction__output__variable ';'
          {
          }
        | tk_inout R_l__analogfunction__inout__variable ';'
          {
          }
        | tk_integer R_l__analogfunction__integer__variable ';'
          {
          }
        | tk_real R_l__analogfunction__real__variable ';'
          {
          }
        ;
R_l__analogfunction__input__variable
        : tk_ident
          {
            p_variableprototype myvariableprototype=new_variable($1);
            adms_analogfunction_list_variable_prepend_once_or_abort(mc.gAnalogfunction,myvariableprototype); 
            myvariableprototype->_input=admse_yes;
            myvariableprototype->_parametertype=admse_analogfunction;
          }
        | R_l__analogfunction__input__variable ',' tk_ident
          {
            p_variableprototype myvariableprototype=new_variable($3);
            adms_analogfunction_list_variable_prepend_once_or_abort(mc.gAnalogfunction,myvariableprototype); 
            myvariableprototype->_input=admse_yes;
            myvariableprototype->_parametertype=admse_analogfunction;
          }
        ;
R_l__analogfunction__output__variable
        : tk_ident
          {
            p_variableprototype myvariableprototype=new_variable($1);
            adms_analogfunction_list_variable_prepend_once_or_abort(mc.gAnalogfunction,myvariableprototype); 
            myvariableprototype->_output=admse_yes;
            myvariableprototype->_parametertype=admse_analogfunction;
          }
        | R_l__analogfunction__output__variable ',' tk_ident
          {
            p_variableprototype myvariableprototype=new_variable($3);
            adms_analogfunction_list_variable_prepend_once_or_abort(mc.gAnalogfunction,myvariableprototype); 
            myvariableprototype->_output=admse_yes;
            myvariableprototype->_parametertype=admse_analogfunction;
          }
        ;
R_l__analogfunction__inout__variable
        : tk_ident
          {
            p_variableprototype myvariableprototype=new_variable($1);
            adms_analogfunction_list_variable_prepend_once_or_abort(mc.gAnalogfunction,myvariableprototype); 
            myvariableprototype->_input=admse_yes;
            myvariableprototype->_output=admse_yes;
            myvariableprototype->_parametertype=admse_analogfunction;
          }
        | R_l__analogfunction__inout__variable ',' tk_ident
          {
            p_variableprototype myvariableprototype=new_variable($3);
            adms_analogfunction_list_variable_prepend_once_or_abort(mc.gAnalogfunction,myvariableprototype); 
            myvariableprototype->_input=admse_yes;
            myvariableprototype->_output=admse_yes;
            myvariableprototype->_parametertype=admse_analogfunction;
          }
        ;
R_l__analogfunction__integer__variable
        : tk_ident
          {
            p_variableprototype myvariableprototype=adms_analogfunction_list_variable_lookup_by_id(mc.gAnalogfunction,mc.gModule,$1,mc.gAnalogfunction);
            if(myvariableprototype)
              myvariableprototype->_type=admse_integer;
            else
            {
              p_variableprototype myvariableprototype=new_variable($1);
              adms_analogfunction_list_variable_prepend_once_or_abort(mc.gAnalogfunction,myvariableprototype); 
              myvariableprototype->_type=admse_integer;
            }
          }
        | R_l__analogfunction__integer__variable ',' tk_ident
          {
            p_variableprototype myvariableprototype=adms_analogfunction_list_variable_lookup_by_id(mc.gAnalogfunction,mc.gModule,$3,mc.gAnalogfunction);
            if(myvariableprototype)
              myvariableprototype->_type=admse_integer;
            else
            {
              p_variableprototype myvariableprototype=new_variable($3);
              adms_analogfunction_list_variable_prepend_once_or_abort(mc.gAnalogfunction,myvariableprototype); 
              myvariableprototype->_type=admse_integer;
            }
          }
        ;
R_l__analogfunction__real__variable
        : tk_ident
          {
            p_variableprototype myvariableprototype=adms_analogfunction_list_variable_lookup_by_id(mc.gAnalogfunction,mc.gModule,$1,mc.gAnalogfunction);
            if(myvariableprototype)
              myvariableprototype->_type=admse_real;
            else
            {
              p_variableprototype myvariableprototype=new_variable($1);
              adms_analogfunction_list_variable_prepend_once_or_abort(mc.gAnalogfunction,myvariableprototype); 
              myvariableprototype->_type=admse_real;
            }
          }
        | R_l__analogfunction__real__variable ',' tk_ident
          {
            p_variableprototype myvariableprototype=adms_analogfunction_list_variable_lookup_by_id(mc.gAnalogfunction,mc.gModule,$3,mc.gAnalogfunction);
            if(myvariableprototype)
              myvariableprototype->_type=admse_real;
            else
            {
              p_variableprototype myvariableprototype=new_variable($3);
              adms_analogfunction_list_variable_prepend_once_or_abort(mc.gAnalogfunction,myvariableprototype); 
              myvariableprototype->_type=admse_real;
            }
          }
        ;
R_variable__type
        : tk_integer R_d__attribute__0
          {
            mc.gVariableType=admse_integer;
            extend_list_by_move(mc.gGlobalAttributeList, mc.gAttributeList);
          }
        | tk_real R_d__attribute__0
          {
            mc.gVariableType=admse_real;
            extend_list_by_move(mc.gGlobalAttributeList, mc.gAttributeList);
          }
        | tk_string R_d__attribute__0
          {
            mc.gVariableType=admse_string;
            extend_list_by_move(mc.gGlobalAttributeList, mc.gAttributeList);
          }
        ;
R_d__variable__end
        : ';'
          {
            //// Very, very confusing.
            for (auto v : mc.gVariableDeclarationList)
            {
              v->_type=mc.gVariableType;
            }
            mc.gVariableDeclarationList.clear();
          }
        ;
R_l__parameter
        : R_s__parameter
          {
          }
        | R_l__parameter ',' R_s__parameter
          {
          }
        ;
R_l__variable
        : R_s__variable
          {
          }
        | R_l__variable ',' R_s__variable
          {
          }
        ;
R_d__aliasparameter
        : R_d__aliasparameter__token tk_ident '=' tk_ident R_d__attribute__0 ';'
          {
            std::string mylexval2=$2->_string;
            p_variableprototype myvariableprototype=adms_module_list_variable_lookup_by_id(mc.gModule,mc.gModule,$4,mc.gModule);
            if(!myvariableprototype)
             adms_veriloga_message_fatal("variable never declared\n",$4);
            adms_variableprototype_list_alias_prepend_once_or_abort(myvariableprototype,mylexval2);

            extend_list_by_move(myvariableprototype->_attribute, mc.gAttributeList);
            extend_list_by_copy(myvariableprototype->_attribute, mc.gGlobalAttributeList);
          }
        ;
R_d__aliasparameter__token
        : tk_aliasparameter
          {
          }
        | tk_aliasparam
          {
          }
        ;
R_s__parameter
        : R_s__parameter__name R_d__attribute__0
          {
            for (auto v : mc.gVariableDeclarationList)
            {
              extend_list_by_move(v->_attribute, mc.gAttributeList);
              extend_list_by_copy(v->_attribute, mc.gGlobalAttributeList);
            }
          }
        ;
R_s__variable
        : R_s__variable__name R_d__attribute__0
          {
            extend_list_by_move(mc.gVariableDeclarationList.back()->_attribute, mc.gAttributeList);
            extend_list_by_copy(mc.gVariableDeclarationList.back()->_attribute, mc.gGlobalAttributeList);
          }
        ;
R_s__parameter__name
        : R_s__variable__name '=' R_s__expression R_s__parameter__range
          {
            mc.gVariableDeclarationList.back()->_input=admse_yes;
            mc.gVariableDeclarationList.back()->_default=($3->get_p<p_expression>());
            extend_list_by_move(mc.gVariableDeclarationList.back()->_range, mc.gRangeList);
          }
        | R_s__variable__name '=' '{' R_l__expression '}' R_s__parameter__range
          {
            mc.gVariableDeclarationList.back()->_input=admse_yes;
            mc.gVariableDeclarationList.back()->_default=dynamic_cast<p_expression>($4->_usrlist.back());
            extend_list_by_copy(mc.gVariableDeclarationList.back()->_arraydefault, $4->_usrlist);
            extend_list_by_move(mc.gVariableDeclarationList.back()->_range, mc.gRangeList);
          }
        ;
R_s__variable__name
        : tk_ident
          {
            std::string mylexval1=$1->_string;
            p_variableprototype myvariableprototype=new_variable($1);
            if(adms_module_list_node_lookup_by_id(mc.gModule,mc.gModule,mylexval1))
             adms_veriloga_message_fatal("variable already defined as node\n",$1);
            adms_module_list_variable_prepend_once_or_abort(mc.gModule,myvariableprototype); 
            mc.gVariableDeclarationList.push_back(myvariableprototype);
          }
        | tk_ident '[' tk_number ':' tk_number ']'
          {
            std::string mylexval1=$1->_string;
            p_variableprototype myvariableprototype=new_variable($1);
            if(adms_module_list_node_lookup_by_id(mc.gModule,mc.gModule,mylexval1))
             adms_veriloga_message_fatal("variable already defined as node\n",$1);
            adms_module_list_variable_prepend_once_or_abort(mc.gModule,myvariableprototype); 
            mc.gVariableDeclarationList.push_back(myvariableprototype);
            myvariableprototype->_sizetype=admse_array;
            myvariableprototype->_minsize=adms_number_new($3);
            myvariableprototype->_maxsize=adms_number_new($5);
          }
        ;
R_s__parameter__range
        : 
          {
          }
        | R_l__interval
          {
          }
        ;
R_l__interval
        : R_s__interval
          {
          }
        | R_l__interval R_s__interval
          {
          }
        ;
R_s__interval
        : tk_from R_d__interval
          {
            if(($2->get_p<p_range>())->_infboundtype==admse_range_bound_value)
              ($2->get_p<p_range>())->_type=admse_include_value;
            else
              ($2->get_p<p_range>())->_type=admse_include;
            mc.gRangeList.push_back($2->get_p<p_range>());
          }
        | tk_exclude R_d__interval
          {
            if(($2->get_p<p_range>())->_infboundtype==admse_range_bound_value)
              ($2->get_p<p_range>())->_type=admse_exclude_value;
            else
              ($2->get_p<p_range>())->_type=admse_exclude;
            mc.gRangeList.push_back($2->get_p<p_range>());
          }
        ;
R_d__interval
        : '(' R_interval__inf ':' R_interval__sup ')'
          {
            p_range myrange=adms_module_list_range_prepend_by_id_once_or_abort(mc.gModule,mc.gModule,$2->get_p<p_expression>(),$4->get_p<p_expression>()); 
            $$=adms_yaccval_new("unknown source file");
            myrange->_infboundtype=admse_range_bound_exclude;
            myrange->_supboundtype=admse_range_bound_exclude;
            $$->set_p(myrange);
          }
        | '(' R_interval__inf ':' R_interval__sup ']'
          {
            p_range myrange=adms_module_list_range_prepend_by_id_once_or_abort(mc.gModule,mc.gModule,$2->get_p<p_expression>(),$4->get_p<p_expression>()); 
            $$=adms_yaccval_new("unknown source file");
            myrange->_infboundtype=admse_range_bound_exclude;
            myrange->_supboundtype=admse_range_bound_include;
            $$->set_p(myrange);
          }
        | '[' R_interval__inf ':' R_interval__sup ')'
          {
            p_range myrange=adms_module_list_range_prepend_by_id_once_or_abort(mc.gModule,mc.gModule,$2->get_p<p_expression>(),$4->get_p<p_expression>()); 
            $$=adms_yaccval_new("unknown source file");
            myrange->_infboundtype=admse_range_bound_include;
            myrange->_supboundtype=admse_range_bound_exclude;
            $$->set_p(myrange);
          }
        | '[' R_interval__inf ':' R_interval__sup ']'
          {
            p_range myrange=adms_module_list_range_prepend_by_id_once_or_abort(mc.gModule,mc.gModule,$2->get_p<p_expression>(),$4->get_p<p_expression>()); 
            $$=adms_yaccval_new("unknown source file");
            myrange->_infboundtype=admse_range_bound_include;
            myrange->_supboundtype=admse_range_bound_include;
            $$->set_p(myrange);
          }
        | R_s__expression
          {
            p_range myrange=adms_module_list_range_prepend_by_id_once_or_abort(mc.gModule,mc.gModule,$1->get_p<p_expression>(),$1->get_p<p_expression>()); 
            $$=adms_yaccval_new("unknown source file");
            myrange->_infboundtype=admse_range_bound_value;
            myrange->_supboundtype=admse_range_bound_value;
            $$->set_p(myrange);
          }
        ;
R_interval__inf
        : R_s__expression
          {
            $$=$1;
          }
        | '-' tk_inf
          {
            p_number mynumber=adms_number_new($2); 
            p_expression myexpression=adms_expression_new(mc.gModule,mynumber); 
            mynumber->_lexval->_string="-inf";
            mc.gModule->_expression.push_back(myexpression); 
            $$=adms_yaccval_new("unknown source file");
            myexpression->_infinity=admse_minus;
            myexpression->_hasspecialnumber="YES";
            $$->set_p(myexpression);
          }
        ;
R_interval__sup
        : R_s__expression
          {
            $$=$1;
          }
        | tk_inf
          {
            p_number mynumber=adms_number_new($1); 
            p_expression myexpression=adms_expression_new(mc.gModule,mynumber); 
            mynumber->_lexval->_string="+inf";
            mc.gModule->_expression.push_back(myexpression); 
            $$=adms_yaccval_new("unknown source file");
            myexpression->_infinity=admse_plus;
            myexpression->_hasspecialnumber="YES";
            $$->set_p(myexpression);
          }
        | '+' tk_inf
          {
            p_number mynumber=adms_number_new($2); 
            p_expression myexpression=adms_expression_new(mc.gModule,mynumber); 
            mynumber->_lexval->_string="+inf";
            mc.gModule->_expression.push_back(myexpression); 
            $$=adms_yaccval_new("unknown source file");
            myexpression->_infinity=admse_plus;
            myexpression->_hasspecialnumber="YES";
            $$->set_p(myexpression);
          }
        ;
R_analog
        : tk_analog R_analogcode
          {
            mc.gModule->_analog=adms_analog_new($2->get_p<p_adms>());
          }
        ;
R_analogcode
        : R_analogcode__atomic
          {
            $$=$1;
          }
        | R_analogcode__block
          {
            $$=$1;
          }
        ;
R_l__expression
        : R_s__expression
          {
            $$=adms_yaccval_new("unknown source file");
            $$->_usrlist.push_back($1->get_p<p_adms>());
          }
        | R_l__expression ',' R_s__expression
          {
            $$=$1;
            $$->_usrlist.push_back($3->get_p<p_adms>());
          }
        ;
R_analogcode__atomic
        : R_d__attribute__0 R_d__blockvariable
          {
            for (auto lv : $2->get_p<p_blockvariable>()->_variable)
            {
              extend_list_by_copy(lv->_attribute, mc.gAttributeList);
            }
            mc.gAttributeList.clear();
            $$=$2;
          }
        | R_d__contribution
          {
            $$=$1;
          }
        | R_s__assignment ';'
          {
            $$=$1;
          }
        | R_d__conditional
          {
            $$=$1;
          }
        | R_d__while
          {
            $$=$1;
          }
        | R_d__case
          {
            $$=$1;
          }
        | R_d__for
          {
            $$=$1;
          }
        | tk_dollar_ident '(' R_l__expression ')' ';'
          {
            p_function myfunction=adms_function_new($1,mc.uid++);
            auto myArgs= $3->_usrlist;
            p_callfunction mycallfunction=adms_callfunction_new(mc.gModule,myfunction);
            mc.gModule->_callfunction.push_back(mycallfunction);
            $$=adms_yaccval_new("unknown source file");
            myfunction->_arguments=myArgs;
            $$->set_p(mycallfunction);
          }
        | tk_dollar_ident '(' ')' ';'
          {
            p_function myfunction=adms_function_new($1,mc.uid++);
            p_callfunction mycallfunction=adms_callfunction_new(mc.gModule,myfunction);
            mc.gModule->_callfunction.push_back(mycallfunction);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(mycallfunction);
          }
        | tk_dollar_ident ';'
          {
            p_function myfunction=adms_function_new($1,mc.uid++);
            p_callfunction mycallfunction=adms_callfunction_new(mc.gModule,myfunction);
            mc.gModule->_callfunction.push_back(mycallfunction);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(mycallfunction);
          }
        | ';'
          {
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(adms_nilled_new(mc.gModule));
          }
        ;
R_analogcode__block
        : R_d__block
          {
            $$=$1;
          }
        | R_analogcode__block__atevent R_d__block
          {
            $$=$2;
            ($$->get_p<p_block>())->_lexval=$1->get_p<p_lexval>();
          }
        ;
R_analogcode__block__atevent
        : '@' '(' tk_ident '(' R_l__analysis ')' ')'
          {
            adms_veriloga_message_fatal("@ control not supported\n",$3);
          }
        | '@' tk_ident
          {
            std::string mylexval2=$2->_string;
            const std::string &mypartitionning=mylexval2;
            if(  mypartitionning != "initial_model"
              && mypartitionning != "initial_instance"
              && mypartitionning != "noise"
              && mypartitionning != "initial_step"
              && mypartitionning != "final_step")
              adms_veriloga_message_fatal(" @() control not supported\n",$2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p($2);
          }
        | '@' '(' tk_ident ')'
          {
            std::string mylexval3=$3->_string;
            const std::string &mypartitionning=mylexval3;
            if(  mypartitionning != "initial_model"
              && mypartitionning != "initial_instance"
              && mypartitionning != "noise"
              && mypartitionning != "initial_step"
              && mypartitionning != "final_step")
              adms_veriloga_message_fatal(" @() control not supported\n",$3);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p($3);
          }
        ;
R_l__analysis
        : R_s__analysis
          {
          }
        | R_l__analysis ',' R_s__analysis
          {
          }
        ;
R_s__analysis
        : tk_anystring
          {
          }
        ;
R_d__block
        : R_d__block__begin tk_end
          {
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(mc.gBlockList.back());
            mc.gBlockList.pop_back();
          }
        | R_d__block__begin ':' tk_ident tk_end
          {
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(mc.gBlockList.back());
            mc.gBlockList.pop_back();
            ($$->get_p<p_block>())->_lexval->_string=$3->_string;
          }
        | R_d__block__begin R_l__blockitem tk_end
          {
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(mc.gBlockList.back());
            mc.gBlockList.pop_back();
          }
        | R_d__block__begin ':' tk_ident R_l__blockitem tk_end
          {
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(mc.gBlockList.back());
            mc.gBlockList.pop_back();
            ($$->get_p<p_block>())->_lexval->_string=$3->_string;
          }
        ;
R_d__block__begin
        : R_d__attribute__0 tk_begin
          {
            p_block myblock=adms_block_new(mc.gModule,$2, ((!mc.gBlockList.empty())?(mc.gBlockList.back()):nullptr), p_adms_list());
            $$=adms_yaccval_new("unknown source file");
            myblock->_lexval->_string="";
            mc.gBlockList.push_back(myblock);
            extend_list_by_move(myblock->_attribute, mc.gAttributeList);
            mc.gModule->_block.push_back(dynamic_cast<p_block>(mc.gBlockList.back()));
          }
        ;
R_l__blockitem
        : R_analogcode
          {
            dynamic_cast<p_block>(mc.gBlockList.back())->_item.push_back($1->get_p<p_adms>());
          }
        | R_l__blockitem R_analogcode
          {
            dynamic_cast<p_block>(mc.gBlockList.back())->_item.push_back($2->get_p<p_adms>());
          }
        ;
R_d__blockvariable
        : tk_integer R_l__blockvariable ';'
          {
            p_blockvariable myblockvariable=adms_blockvariable_new(dynamic_cast<p_block>(mc.gBlockList.back())); 
            mc.gModule->_blockvariable.push_back(myblockvariable); 
            for(auto l : mc.gBlockVariableList)
            {
              dynamic_cast<p_variableprototype>(l)->_type=admse_integer;
            }
            $$=adms_yaccval_new("unknown source file");
            myblockvariable->_variable=mc.gBlockVariableList;
            mc.gBlockVariableList.clear();
            $$->set_p(myblockvariable);
          }
        | tk_real R_l__blockvariable ';'
          {
            p_blockvariable myblockvariable=adms_blockvariable_new(dynamic_cast<p_block>(mc.gBlockList.back())); 
            mc.gModule->_blockvariable.push_back(myblockvariable); 
            for (auto l : mc.gBlockVariableList)
            {
              l->_type=admse_real;
            }
            $$=adms_yaccval_new("unknown source file");
            myblockvariable->_variable=mc.gBlockVariableList;
            mc.gBlockVariableList.clear();
            $$->set_p(myblockvariable);
          }
        | tk_string R_l__blockvariable ';'
          {
            p_blockvariable myblockvariable=adms_blockvariable_new(dynamic_cast<p_block>(mc.gBlockList.back())); 
            mc.gModule->_blockvariable.push_back(myblockvariable); 
            for (auto l : mc.gBlockVariableList)
            {
              l->_type=admse_string;
            }
            $$=adms_yaccval_new("unknown source file");
            myblockvariable->_variable=mc.gBlockVariableList;
            mc.gBlockVariableList.clear();
            $$->set_p(myblockvariable);
          }
        ;
R_l__blockvariable
        : R_s__blockvariable
          {
          }
        | R_l__blockvariable ',' R_s__blockvariable
          {
          }
        ;
R_s__blockvariable
        : tk_ident
          {
            p_variableprototype myvariableprototype=new_variable($1);
            adms_block_list_variable_prepend_once_or_abort(dynamic_cast<p_block>(mc.gBlockList.back()),myvariableprototype); 
            mc.gBlockVariableList.push_back(myvariableprototype);
          }
        | tk_ident '[' tk_number ':' tk_number ']'
          {
            p_variableprototype myvariableprototype=new_variable($1);
            adms_block_list_variable_prepend_once_or_abort(dynamic_cast<p_block>(mc.gBlockList.back()),myvariableprototype); 
            mc.gVariableDeclarationList.push_back(myvariableprototype);
            myvariableprototype->_sizetype=admse_array;
            myvariableprototype->_minsize=adms_number_new($3);
            myvariableprototype->_maxsize=adms_number_new($5);
          }
        ;
R_d__contribution
        : R_contribution R_d__attribute__0 ';'
          {
            extend_list_by_move(mc.gContribution->_attribute, mc.gAttributeList);
            mc.gContribution=nullptr;
          }
        ;
R_contribution
        : R_source '<' '+' R_s__expression
          {
            p_source mysource=$1->get_p<p_source>();
            $$=adms_yaccval_new("unknown source file");
            mc.gContribution=adms_contribution_new(mc.gModule,mysource,$4->get_p<p_expression>(),mc.gLexval);
            mc.gModule->_contribution.push_back(mc.gContribution);
            $$->set_p(mc.gContribution);
            mc.gContribution->_branchalias=mc.gBranchAlias;
            mc.gBranchAlias=nullptr;
          }
        ;
R_source
        : tk_ident '(' tk_ident ',' tk_ident ')'
          {
            std::string mylexval1=$1->_string;
            std::string mylexval3=$3->_string;
            std::string mylexval5=$5->_string;
            p_node Pnode=adms_module_list_node_lookup_by_id(mc.gModule,mc.gModule,mylexval3);
            p_node Nnode=adms_module_list_node_lookup_by_id(mc.gModule,mc.gModule,mylexval5);
            std::string natureID=mylexval1;
            p_nature mynature=adms_admsmain_list_nature_lookup_by_id(root(),natureID);
            p_branch mybranch=adms_module_list_branch_prepend_by_id_once_or_ignore(mc.gModule,mc.gModule,Pnode,Nnode);
            $$=adms_yaccval_new("unknown source file");
            if(!mynature)
             adms_message_fatal("[source:error] there is no nature with access %s, missing discipline.h file?\n",natureID.c_str());
            mc.gSource=adms_module_list_source_prepend_by_id_once_or_ignore(mc.gModule,mc.gModule,mybranch,mynature);
            mc.gLexval=$1;
            $$->set_p(mc.gSource);
          }
        | tk_ident '(' tk_ident ')'
          {
            std::string mylexval1=$1->_string;
            std::string mylexval3=$3->_string;
            std::string natureID=mylexval1;
            p_nature mynature=adms_admsmain_list_nature_lookup_by_id(root(),natureID);
            p_branchalias branchalias=adms_module_list_branchalias_lookup_by_id(mc.gModule,mc.gModule,mylexval3);
            p_node pnode=adms_module_list_node_lookup_by_id(mc.gModule,mc.gModule,mylexval3);
            p_branch mybranch=nullptr;
            $$=adms_yaccval_new("unknown source file");
            if(!mynature)
             adms_message_fatal("[source:error] there is no nature with access %s, please, include discipline.h file\n",natureID.c_str());
            if(pnode)
              mybranch=adms_module_list_branch_prepend_by_id_once_or_ignore(mc.gModule,mc.gModule,pnode,mc.gGND);
            else if(branchalias)
              mybranch=branchalias->_branch;
            else
              adms_veriloga_message_fatal("undefined branch or node\n",$1);
            mc.gSource=adms_module_list_source_prepend_by_id_once_or_ignore(mc.gModule,mc.gModule,mybranch,mynature);
            mc.gLexval=$1;
            mc.gBranchAlias=branchalias;
            $$->set_p(mc.gSource);
          }
        ;
R_d__while
        : tk_while '(' R_s__expression ')' R_analogcode
          {
            p_whileloop mywhileloop=adms_whileloop_new(mc.gModule,$3->get_p<p_expression>(),$5->get_p<p_adms>());
            mc.gModule->_whileloop.push_back(mywhileloop);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(mywhileloop);
          }
        ;
R_d__for
        : tk_for '(' R_s__assignment ';' R_s__expression ';' R_s__assignment ')' R_analogcode
          {
            p_forloop myforloop=adms_forloop_new(mc.gModule,$3->get_p<p_assignment>(),$5->get_p<p_expression>(),$7->get_p<p_assignment>(),$9->get_p<p_adms>());
            mc.gModule->_forloop.push_back(myforloop);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myforloop);
          }
        ;
R_d__case
        : tk_case '(' R_s__expression ')' R_l__case__item tk_endcase
          {
            p_case mycase=adms_case_new(mc.gModule,$3->get_p<p_expression>());
            mc.gModule->_case.push_back(mycase);
            $$=adms_yaccval_new("unknown source file");
            mycase->_caseitem.push_back($5);
            $$->set_p(mycase);
          }
        ;
R_l__case__item
        : R_s__case__item
          {
            $$=adms_yaccval_new("unknown source file");
            $$->_usrlist.push_back($1->get_p<p_adms>());
          }
        | R_l__case__item R_s__case__item
          {
            $$=$1;
            $$->_usrlist.push_back($2->get_p<p_adms>());
          }
        ;
R_s__case__item
        : R_l__enode ':' R_analogcode
          {
            $$=adms_yaccval_new("unknown source file");
            p_caseitem mycaseitem=adms_caseitem_new($3->get_p<p_adms>());
            mycaseitem->_condition=$1->_usrlist;
            $$->set_p(mycaseitem);
          }
        | tk_default ':' R_analogcode
          {
            p_caseitem mycaseitem=adms_caseitem_new($3->get_p<p_adms>());
            $$=adms_yaccval_new("unknown source file");
            mycaseitem->_defaultcase=admse_yes;
            $$->set_p(mycaseitem);
          }
        | tk_default R_analogcode
          {
            p_caseitem mycaseitem=adms_caseitem_new($2->get_p<p_adms>());
            $$=adms_yaccval_new("unknown source file");
            mycaseitem->_defaultcase=admse_yes;
            $$->set_p(mycaseitem);
          }
        ;
R_s__instance
        : R_instance__module__name '#' '(' R_l__instance__parameter ')' tk_ident '(' R_l__node ')' ';'
          {
            std::string mylexval6=$6->_string;
            //p_instance myinstance;
            p_instance myinstance=adms_module_list_instance_prepend_by_id_once_or_abort(mc.gModule,mc.gModule,mc.gInstanceModule,mylexval6);
            //// TODO: make sure the GND is ignored!!!!
            auto l2 = mc.gInstanceModule->_node.begin();
            ++l2; /* GND ignored */

            for (auto l1 : mc.gNodeList)
            {
              adms_instance_list_terminal_prepend_once_or_abort(myinstance,adms_instancenode_new(l1, *l2));
              ++l2;
            }
            for (auto l1 : mc.gInstanceVariableList)
            {
              adms_instance_list_parameterset_prepend_once_or_abort(myinstance,l1);
            }
            mc.gNodeList.clear();
            mc.gInstanceVariableList.clear();
          }
        ;
R_instance__module__name
        : tk_ident
          {
            std::string mylexval1=$1->_string;
            mc.gInstanceModule=adms_admsmain_list_module_lookup_by_id(root(),mylexval1);
            if(!mc.gInstanceModule)
              adms_message_fatal("module '%s' not found\n",mylexval1.c_str());
          }
        ;
R_l__instance__parameter
        : R_s__instance__parameter
          {
          }
        | R_l__instance__parameter ',' R_s__instance__parameter
          {
          }
        ;
R_s__instance__parameter
        : '.' tk_ident '(' R_s__expression ')'
          {
            std::string mylexval2=$2->_string;
            p_variableprototype myvariableprototype=adms_module_list_variable_lookup_by_id(mc.gInstanceModule,mc.gInstanceModule,$2,mc.gInstanceModule);
            if(myvariableprototype)
            {
              p_instanceparameter myinstanceparameter;
              myinstanceparameter=adms_instanceparameter_new(myvariableprototype);
              mc.gInstanceVariableList.push_back(myinstanceparameter);
              myinstanceparameter->_value=($4->get_p<p_expression>());
            }
            else
            {
              adms_veriloga_message_fatal_continue($2);
              adms_message_fatal("[%s.%s.%s]: undefined variable (instance declaration)",
                mc.gModule->get_uid().c_str(),mc.gInstanceModule->get_uid().c_str(),mylexval2.c_str());
            }
          }
        ;
R_s__assignment
        : tk_ident '=' R_s__expression
          {
            p_assignment myassignment;
            p_variable myvariable=variable_recursive_lookup_by_id(mc.gBlockList.back(),$1);
            p_variableprototype myvariableprototype;
            if(!myvariable)
              adms_veriloga_message_fatal("undefined variable\n",$1);
            myvariableprototype=myvariable->_prototype;
            myassignment=adms_assignment_new(mc.gModule,myvariable,$3->get_p<p_expression>(),$1);
            mc.gModule->_assignment.push_back(myassignment);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myassignment);
            myvariableprototype->_vcount++;
            myvariableprototype->_vlast=myassignment;
          }
        | R_d__attribute tk_ident '=' R_s__expression
          {
            p_assignment myassignment;
            p_variable myvariable=variable_recursive_lookup_by_id(mc.gBlockList.back(),$2);
            p_variableprototype myvariableprototype;
            if(!myvariable)
              adms_veriloga_message_fatal("undefined variable\n",$2);
            myvariableprototype=myvariable->_prototype;
            myassignment=adms_assignment_new(mc.gModule,myvariable,$4->get_p<p_expression>(),$2);
            mc.gModule->_assignment.push_back(myassignment);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myassignment);
            extend_list_by_move(myassignment->_attribute, mc.gAttributeList);
            myvariableprototype->_vcount++;
            myvariableprototype->_vlast=myassignment;
          }
        | tk_ident '[' R_expression ']' '=' R_s__expression
          {
            p_assignment myassignment;
            p_array myarray;
            p_variable myvariable=variable_recursive_lookup_by_id(mc.gBlockList.back(),$1);
            p_variableprototype myvariableprototype;
            if(!myvariable)
              adms_veriloga_message_fatal("undefined variable\n",$1);
            myvariableprototype=myvariable->_prototype;
            myarray=adms_array_new(myvariable,$3->get_p<p_adms>());
            myassignment=adms_assignment_new(mc.gModule,myarray,$6->get_p<p_expression>(),$1);
            mc.gModule->_assignment.push_back(myassignment);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myassignment);
            myvariableprototype->_vcount++;
            myvariableprototype->_vlast=myassignment;
          }
        | R_d__attribute tk_ident '[' R_expression ']' '=' R_s__expression
          {
            p_assignment myassignment;
            p_array myarray;
            p_variable myvariable=variable_recursive_lookup_by_id(mc.gBlockList.back(),$2);
            p_variableprototype myvariableprototype;
            if(!myvariable)
              adms_veriloga_message_fatal("undefined variable\n",$2);
            myvariableprototype=myvariable->_prototype;
            myarray=adms_array_new(myvariable,$4->get_p<p_adms>());
            myassignment=adms_assignment_new(mc.gModule,myarray,$7->get_p<p_expression>(),$2);
            mc.gModule->_assignment.push_back(myassignment);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myassignment);
            extend_list_by_move(myassignment->_attribute, mc.gAttributeList);
            myvariableprototype->_vcount++;
            myvariableprototype->_vlast=myassignment;
          }
        ;
R_d__conditional
        : tk_if '(' R_s__expression ')' R_analogcode
          {
            p_expression myexpression=$3->get_p<p_expression>();
            p_adms mythen=$5->get_p<p_adms>();
            p_conditional myconditional=adms_conditional_new(mc.gModule,myexpression,mythen,nullptr);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myconditional);
          } %prec PREC_IF_THEN
        | tk_if '(' R_s__expression ')' R_analogcode tk_else R_analogcode
          {
            p_expression myexpression=$3->get_p<p_expression>();
            p_adms mythen=$5->get_p<p_adms>();
            p_adms myelse=$7->get_p<p_adms>();
            p_conditional myconditional=adms_conditional_new(mc.gModule,myexpression,mythen,myelse);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myconditional);
          }
        ;
R_s__expression
        : R_expression
          {
            p_expression myexpression=adms_expression_new(mc.gModule,$1->get_p<p_adms>()); 
            mc.gModule->_expression.push_back(myexpression); 
            $$=$1;
            $$->set_p(myexpression);
          }
        ;
R_l__enode
        : R_s__function_expression
          {
            $$=adms_yaccval_new("unknown source file");
            $$->_usrlist.push_back($1->get_p<p_adms>());
          }
        | R_l__enode ',' R_s__function_expression
          {
            $$=$1;
            $$->_usrlist.push_back($3->get_p<p_adms>());
          }
        ;
R_s__function_expression
        : R_expression
          {
            $$=$1;
          }
        ;
R_expression
        : R_e__conditional
          {
            $$=$1;
          }
        ;
R_e__conditional
        : R_e__bitwise_equ
          {
            $$=$1;
          }
        | R_e__bitwise_equ '?' R_e__bitwise_equ ':' R_e__bitwise_equ
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$3->get_p<p_adms>();
            p_adms m3=$5->get_p<p_adms>();
            p_mapply_ternary myop=adms_mapply_ternary_new("conditional",m1,m2,m3);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        ;
R_e__bitwise_equ
        : R_e__bitwise_xor
          {
            $$=$1;
          }
        | R_e__bitwise_equ tk_bitwise_equr R_e__bitwise_xor
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$3->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("bw_equr",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        | R_e__bitwise_equ '~' '^' R_e__bitwise_xor
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$4->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("bw_equl",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        ;
R_e__bitwise_xor
        : R_e__bitwise_or
          {
            $$=$1;
          }
        | R_e__bitwise_xor '^' R_e__bitwise_or
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$3->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("bw_xor",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        ;
R_e__bitwise_or
        : R_e__bitwise_and
          {
            $$=$1;
          }
        | R_e__bitwise_or '|' R_e__bitwise_and
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$3->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("bw_or",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        ;
R_e__bitwise_and
        : R_e__logical_or
          {
            $$=$1;
          }
        | R_e__bitwise_and '&' R_e__logical_or
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$3->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("bw_and",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        ;
R_e__logical_or
        : R_e__logical_and
          {
            $$=$1;
          }
        | R_e__logical_or tk_or R_e__logical_and
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$3->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("or",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        ;
R_e__logical_and
        : R_e__comp_equ
          {
            $$=$1;
          }
        | R_e__logical_and tk_and R_e__comp_equ
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$3->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("and",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        ;
R_e__comp_equ
        : R_e__comp
          {
            $$=$1;
          }
        | R_e__comp_equ '=' '=' R_e__comp
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$4->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("equ",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        | R_e__comp_equ '!' '=' R_e__comp
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$4->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("notequ",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        ;
R_e__comp
        : R_e__bitwise_shift
          {
            $$=$1;
          }
        | R_e__comp '<' R_e__bitwise_shift
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$3->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("lt",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        | R_e__comp '<' '=' R_e__bitwise_shift
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$4->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("lt_equ",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        | R_e__comp '>' R_e__bitwise_shift
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$3->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("gt",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        | R_e__comp '>' '=' R_e__bitwise_shift
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$4->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("gt_equ",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        ;
R_e__bitwise_shift
        : R_e__arithm_add
          {
            $$=$1;
          }
        | R_e__bitwise_shift tk_op_shr R_e__arithm_add
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$3->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("shiftr",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        | R_e__bitwise_shift tk_op_shl R_e__arithm_add
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$3->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("shiftl",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        ;
R_e__arithm_add
        : R_e__arithm_mult
          {
            $$=$1;
          }
        | R_e__arithm_add '+' R_e__arithm_mult
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$3->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("addp",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        | R_e__arithm_add '-' R_e__arithm_mult
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$3->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("addm",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        ;
R_e__arithm_mult
        : R_e__unary
          {
            $$=$1;
          }
        | R_e__arithm_mult '*' R_e__unary
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$3->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("multtime",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        | R_e__arithm_mult '/' R_e__unary
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$3->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("multdiv",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        | R_e__arithm_mult '%' R_e__unary
          {
            p_adms m1=$1->get_p<p_adms>();
            p_adms m2=$3->get_p<p_adms>();
            p_mapply_binary myop=adms_mapply_binary_new("multmod",m1,m2);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myop);
          }
        ;
R_e__unary
        : R_e__atomic
          {
            $$=$1;
          }
        | '+' R_e__atomic
          {
            p_adms m=$2->get_p<p_adms>();
            p_mapply_unary mymathapply=adms_mapply_unary_new("plus",m);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(mymathapply);
          }
        | '-' R_e__atomic
          {
            p_adms m=$2->get_p<p_adms>();
            p_mapply_unary mymathapply=adms_mapply_unary_new("minus",m);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(mymathapply);
          }
        | '!' R_e__atomic
          {
            p_adms m=$2->get_p<p_adms>();
            p_mapply_unary mymathapply=adms_mapply_unary_new("not",m);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(mymathapply);
          }
        | '~' R_e__atomic
          {
            p_adms m=$2->get_p<p_adms>();
            p_mapply_unary mymathapply=adms_mapply_unary_new("bw_not",m);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(mymathapply);
          }
        ;
R_e__atomic
        : tk_integer
          {
            p_number mynumber=adms_number_new($1);
            $$=adms_yaccval_new("unknown source file");
            mynumber->_cast="i";
            $$->set_p(mynumber);
          }
        | tk_integer tk_ident
          {
            std::string mylexval2=$2->_string;
            p_number mynumber=adms_number_new($1);
            $$=adms_yaccval_new("unknown source file");
            if (!is_valid_scaling_unit(mylexval2))
            {
              adms_veriloga_message_fatal("can not convert symbol to valid unit\n",$2);
            }
            mynumber->_scalingunit=mylexval2;
            mynumber->_cast="i";
            $$->set_p(mynumber);
          }
        | tk_number
          {
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(adms_number_new($1));
          }
        | tk_number tk_ident
          {
            std::string mylexval2=$2->_string;
            p_number mynumber=adms_number_new($1);
            $$=adms_yaccval_new("unknown source file");
            if (!is_valid_scaling_unit(mylexval2))
            {
              adms_veriloga_message_fatal(" can not convert symbol to valid unit\n",$2);
            }
            mynumber->_scalingunit=mylexval2;
            $$->set_p(mynumber);
          }
        | tk_char
          {
            adms_veriloga_message_fatal("%s: character are not handled\n",$1);
          }
        | tk_anystring
          {
            std::string mylexval1=$1->_string;
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(adms_string_new(mylexval1));
          }
        | tk_ident
          {
            std::string mylexval1=$1->_string;
            $$=adms_yaccval_new("unknown source file");
            assert(!mc.gBlockList.empty());
            if(mc.gAnalogfunction)
            {
              p_variable myvariable=variable_recursive_lookup_by_id(mc.gBlockList.back(),$1);
              if(myvariable)
                $$->set_p(myvariable);
            }
            else
            {
              p_variable myvariable=variable_recursive_lookup_by_id(mc.gBlockList.back(),$1);
              if(myvariable)
              {
                $$->set_p(myvariable);
              }
              else
              {
                p_branchalias mybranchalias=adms_module_list_branchalias_lookup_by_id(mc.gModule,mc.gModule,mylexval1);
                p_node mynode=adms_module_list_node_lookup_by_id(mc.gModule,mc.gModule,mylexval1);
                if(mynode) $$->set_p(mynode);
                if(mybranchalias)
                  $$->set_p(mybranchalias->_branch);
              }
            }
            if(!$$->get_p<p_adms>())
            {
              adms_veriloga_message_fatal("identifier never declared\n",$1);
            }
          }
        | tk_dollar_ident
          {
            p_function myfunction=adms_function_new($1,mc.uid++);
            $$=adms_yaccval_new("unknown source file");
            $$->set_p(myfunction);
          }
        | tk_ident '[' R_expression ']'
          {
            $$=adms_yaccval_new("unknown source file");
            p_variable myvariable=variable_recursive_lookup_by_id(mc.gBlockList.back(),$1);
            if(!myvariable)
               adms_veriloga_message_fatal("undefined array variable\n",$1);
            $$->set_p(adms_array_new(myvariable,$3->get_p<p_adms>()));
          }
        | tk_dollar_ident '(' R_l__enode ')'
          {
            p_function myfunction=adms_function_new($1,mc.uid++);
            p_adms_list myArgs=$3->_usrlist;
            $$=adms_yaccval_new("unknown source file");
            myfunction->_arguments=myArgs;
            $$->set_p(myfunction);
          }
        | tk_ident '(' R_l__enode ')'
          {
            std::string mylexval1=$1->_string;
            std::string myfunctionname=mylexval1;
            p_adms_list myArgs=$3->_usrlist;
            int narg=myArgs.size();
            p_probe myprobe=nullptr;
            p_nature mynature=adms_admsmain_list_nature_lookup_by_id(root(),myfunctionname);
            $$=adms_yaccval_new("unknown source file");
            if(mynature && narg==1)
            {
              p_adms mychild0=myArgs.back();
              if(mychild0->_datatypename==admse_node)
              {
                p_branch mybranch=adms_module_list_branch_prepend_by_id_once_or_ignore(mc.gModule,mc.gModule,dynamic_cast<p_node>(mychild0),mc.gGND);
                myprobe=adms_module_list_probe_prepend_by_id_once_or_ignore(mc.gModule,mc.gModule,mybranch,mynature);
              }
              else if(mychild0->_datatypename==admse_branch)
              {
                myprobe=adms_module_list_probe_prepend_by_id_once_or_ignore(mc.gModule,mc.gModule,dynamic_cast<p_branch>(mychild0),mynature);
              }
              else
                adms_veriloga_message_fatal("bad argument (expecting node or branch)\n",$1);
            }
            else if(mynature && narg==2)
            {
              p_adms mychild0=myArgs.front();
              p_adms mychild1=myArgs.back();
              p_branch mybranch;
              if(mychild0->_datatypename!=admse_node)
                adms_veriloga_message_fatal("second argument of probe is not a node\n",$1);
              if(mychild1->_datatypename!=admse_node)
                adms_veriloga_message_fatal("first argument of probe is not a node\n",$1);
              mybranch=adms_module_list_branch_prepend_by_id_once_or_ignore(mc.gModule,mc.gModule,dynamic_cast<p_node>(mychild1),(dynamic_cast<p_node>(mychild0)));
              myprobe=adms_module_list_probe_prepend_by_id_once_or_ignore(mc.gModule,mc.gModule,mybranch,mynature);
            }
            if(myprobe)
              $$->set_p(myprobe);
            else
            {
              p_function myfunction=adms_function_new($1,mc.uid++);
              ////for(l=mc.gModule->_analogfunction;l&&(myfunction->_definition==nullptr);l=l->next)
              //// TODO make sure this makes sense
              //// should this be in reverse?
              for(auto myanalogfunction : mc.gModule->_analogfunction)
              {
                if($1->_string == myanalogfunction->_lexval->_string)
                {
                  myfunction->_definition=myanalogfunction;
                  break;
                }
              }
              myfunction->_arguments=myArgs;
              $$->set_p(myfunction);
            }
          }
        | '(' R_expression ')'
          {
            $$=$2;
          }
        ;
%%
void adms_veriloga_setint_yydebug(const int val)
{
  yydebug=val;
}
