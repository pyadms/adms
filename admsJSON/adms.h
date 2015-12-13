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


/* ------- code automatically created by ./mkelements.pl -------------- */

#ifndef ADMS_H
#define ADMS_H
#include <string>
#include <list>
#include <utility>
#include <functional>
#include <algorithm>
#include <iterator>

#ifdef _WIN32
#include <io.h>
#define F_OK    0       /* Test for existence.  */
#define YY_NO_UNISTD_H
#else
#include <unistd.h>
#endif
#include <sys/stat.h>
#define ADMS_PATH_SEPARATOR "/"
#define PACKAGE_NAME "adms"
#define PACKAGE_VERSION "2.3.0"
#define PACKAGE_BUGREPORT "lutherthecat@github"
#define ADMS_COMPILER "N/A" 
#define ADMS_OS "N/A" 


// update for get_admse_string
enum admse {
admse_simulator,
admse_admsmain,
admse_list,
admse_nature,
admse_discipline,
admse_module,
admse_analogfunction,
admse_nodealias,
admse_node,
admse_branchalias,
admse_branch,
admse_variableprototype,
admse_source,
admse_range,
admse_analog,

admse_string,
admse_mapply_unary,
admse_mapply_binary,
admse_mapply_ternary,
admse_number,
admse_function,
admse_variable,
admse_array,
admse_probe,
admse_expression,
admse_instance,
admse_instanceparameter,
admse_instancenode,
admse_nilled,
admse_assignment,
admse_contribution,
admse_conditional,
admse_forloop,
admse_whileloop,
admse_case,
admse_caseitem,
admse_blockvariable,
admse_block,
admse_callfunction,
admse_attribute,
admse_lexval,
admse_yaccval,
admse_input,
admse_output,
admse_yes,
admse_no,
admse_continuous,
admse_discrete,
admse_real,
admse_integer,
admse_inout,
admse_internal,
admse_ground,
admse_external,
admse_scalar,
admse_model,
admse_range_bound_include,
admse_range_bound_exclude,
admse_range_bound_value,
admse_include,
admse_include_value,
admse_exclude,
admse_exclude_value,
admse_plus,
admse_minus,
};

struct        s_adms;
typedef       s_adms *p_adms;
//typedef const s_adms   *p_kadms;
typedef std::list<p_adms> p_adms_list;

struct s_simulator;
typedef struct s_simulator *p_simulator;
struct s_admsmain;
typedef s_admsmain *p_admsmain;

struct s_list;
typedef s_list *p_list;

struct s_nature;
typedef s_nature *p_nature;
typedef std::list<p_nature> nature_list;

struct s_discipline;
typedef s_discipline *p_discipline;
typedef std::list<p_discipline> discipline_list;

struct  s_module;
typedef s_module *p_module;
typedef std::list<p_module> module_list;

struct s_analogfunction;
typedef s_analogfunction *p_analogfunction;
typedef std::list<p_analogfunction> analogfunction_list;

struct s_nodealias;
typedef s_nodealias *p_nodealias;
typedef std::list<p_nodealias> nodealias_list;

struct s_node;
typedef s_node *p_node;
typedef std::list<p_node> node_list;

struct s_branchalias;
typedef s_branchalias *p_branchalias;
typedef std::list<p_branchalias> branchalias_list;

struct s_branch;
typedef s_branch *p_branch;
typedef std::list<p_branch> branch_list;

struct s_variableprototype;
typedef s_variableprototype *p_variableprototype;
typedef std::list<p_variableprototype> variableprototype_list;

struct s_source;
typedef s_source *p_source;
typedef std::list<p_source> source_list;

struct s_range;
typedef s_range *p_range;
typedef std::list<p_range> range_list;

struct s_analog;
typedef s_analog *p_analog;

struct s_text;
typedef s_text *p_text;

struct s_path;
typedef s_path *p_path;

struct s_ptraverse;
typedef s_ptraverse *p_ptraverse;

struct s_pparse;
typedef s_pparse *p_pparse;

struct s_attribute;
typedef s_attribute *p_attribute;
typedef std::list<p_attribute> attribute_list;

struct s_lexval;
typedef s_lexval *p_lexval;

struct s_yaccval;
typedef s_yaccval *p_yaccval;

/* math */
struct s_math;
typedef s_math *p_math;

struct s_string;
typedef s_string *p_string;
typedef std::list<p_string> string_list;

struct s_mapply_unary;
typedef s_mapply_unary *p_mapply_unary;

struct s_mapply_binary;
typedef s_mapply_binary *p_mapply_binary;

struct s_mapply_ternary;
typedef s_mapply_ternary *p_mapply_ternary;

struct s_number;
typedef s_number *p_number;

struct s_function;
typedef s_function *p_function;

struct s_variable;
typedef s_variable *p_variable;
typedef std::list<p_variable> variable_list;

struct s_array;
typedef s_array *p_array;

struct s_probe;
typedef s_probe *p_probe;
typedef std::list<p_probe> probe_list;

struct s_expression;
typedef s_expression *p_expression;
typedef std::list<p_expression> expression_list;

struct s_instance;
typedef s_instance *p_instance;
typedef std::list<p_instance> instance_list;


struct s_instanceparameter;
typedef s_instanceparameter *p_instanceparameter;
typedef std::list<p_instanceparameter> instanceparameter_list;

struct s_instancenode;
typedef s_instancenode *p_instancenode;
typedef std::list<p_instancenode> instancenode_list;

struct s_nilled;
typedef s_nilled *p_nilled;

struct s_assignment;
typedef s_assignment *p_assignment;
typedef std::list<p_assignment> assignment_list;

struct s_contribution;
typedef s_contribution *p_contribution;
typedef std::list<p_contribution> contribution_list;

struct s_conditional;
typedef s_conditional *p_conditional;
typedef std::list<p_conditional> conditional_list;

struct s_forloop;
typedef s_forloop *p_forloop;
typedef std::list<p_forloop> forloop_list;

struct s_whileloop;
typedef s_whileloop *p_whileloop;
typedef std::list<p_whileloop> whileloop_list;

struct s_case;
typedef s_case *p_case;
typedef std::list<p_case> case_list;

struct s_caseitem;
typedef s_caseitem *p_caseitem;

struct s_blockvariable;
typedef s_blockvariable *p_blockvariable;
typedef std::list<p_blockvariable> blockvariable_list;

struct s_block;
typedef s_block *p_block;
typedef std::list<p_block> block_list;

struct s_callfunction;
typedef s_callfunction *p_callfunction;
typedef std::list<p_callfunction> callfunction_list;

int admsmain(std::string xcode,std::string xflag,std::string vacode);


/*-- Miscellaneous routines --*/
std::string adms_integertostring(int value);
int adms_file_isdirectory(std::string myfilename);
int adms_file_isregular(std::string myfilename);


typedef std::tuple<std::string, std::list<p_adms> > mytup1;
typedef std::tuple<std::string, std::string > mytup2;
typedef std::list <mytup1> references_list_t;
typedef std::list <mytup2> attributes_list_t;
typedef attributes_list_t strings_list_t;

/* ------- adms -------------- */
struct s_adms {
  s_adms(admse datatypename) : _datatypename(datatypename) {}
  virtual ~s_adms() {}

  std::string get_uid() {
    return this->get_uid_implemented();
  }

  void get_reference_list(references_list_t &rlist);
  void get_attribute_list(attributes_list_t &alist);
  void get_string_list(strings_list_t &slist);



  virtual std::string get_uid_implemented() = 0;
  virtual void get_reference_list_implemented(references_list_t &rlist) = 0;
  virtual void get_attribute_list_implemented(attributes_list_t &alist) = 0;
  virtual void get_string_list_implemented(strings_list_t &slist) = 0;

  admse _datatypename;
};






template <typename T>
T* myprepend_or_fail(std::list<T*> &mylist, std::function<bool(T*)> myfunc, std::function<void()> myfail, std::function<T*()> mycreate)
{
  T *ret = mylookup_by_id(mylist, myfunc);

  if (ret)
  {
    myfail();
  }
  else
  {
    ret = mycreate();
    mylist.push_back(ret);
  }
  
  return ret;
}

template <typename T>
T* myprepend_or_ignore(T *new_elem, std::list<T*> &mylist, std::function<bool(T*)> myfunc)
{
  T *ret = new_elem;
  auto it = std::find_if(mylist.begin(), mylist.end(), myfunc);
  if (it != mylist.end())
  {
    ret = *it;
  }
  else
  {
    mylist.push_back(new_elem);
  }
  return ret;
}

template <typename T>
T* mylookup_by_id(std::list<T*> &mylist, std::function<bool(T*)> myfunc)
{
  T *ret = nullptr;
  auto it = std::find_if(mylist.begin(), mylist.end(), myfunc);
  if (it != mylist.end())
  {
    ret = *it; 
  }
  return ret;
}

const double adms_dzero=0.0;
const double adms_NAN = std::numeric_limits<double>::quiet_NaN();

bool is_valid_scaling_unit(const std::string &);

std::string get_admse_string(admse x);

void push_back_attributes(attributes_list_t &, attribute_list &);

template <typename T>
void push_back_mytup1(references_list_t &rlist, std::string name, std::list<T> &list);

template <typename T>
std::list<p_adms> create_adms_list(T);

#endif /* adms_h */

