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

#include "adms_slist.h"
#include <cstdlib>

p_slist adms_slist_last (p_slist l)
{
  if(l)
  {
    while(l->next)
      l=l->next;
  }
  return l;
}
p_slist adms_slist_new (p_adms d)
{
  p_slist newl=NULL;
  adms_slist_push(&newl,d);
  return newl;
}
p_slist adms_slist_copy (p_slist l)
{
  p_slist copiedl=NULL;
  while(l)
  {
    adms_slist_push(&copiedl,l->data);
    l=l->next;
  }
  return adms_slist_reverse(copiedl);
}
void adms_slist_push (p_slist* l,p_adms data)
{
  p_slist n= new s_slist;
  n->next=*l;
  n->data=data;
  *l=n;
}
p_adms adms_slist_pull (p_slist* l)
{
  if(*l)
  {
    p_slist n=*l;
    p_adms data=n->data;
    *l=(*l)->next;
    free(n);
    return data;
  }
  return NULL;
}
void adms_slist_concat (p_slist* l1,p_slist l2)
{
  if(l2)
  {
    if(*l1)
      adms_slist_last(*l1)->next=l2;
    else
      *l1=l2;
  }
}
unsigned int adms_slist_length (p_slist l)
{
  unsigned int length=0;
  while(l)
  {
    length++;
    l=l->next;
  }
  return length;
}
p_slist adms_slist_nth (p_slist l,unsigned int  n)
{
  while (n-->0 && l)
    l=l->next;
  return l;
}
p_adms adms_slist_nth_data (p_slist l,unsigned int n)
{
  while (n-->0 && l)
    l=l->next;
  return l ? l->data : ((p_adms)0);
}
#if 0
p_slist adms_slist_find (p_slist l,p_kadms data)
{
  while(l)
  {
    if(l->data==data)
      break;
    l=l->next;
  }
  return l;
}
int adms_slist_index (p_slist l, p_kadms data)
{
  int i=0;
  while(l)
  {
    if(l->data==data)
      return i;
    i++;
    l=l->next;
  }
  return -1;
}
#endif
p_slist adms_slist_reverse (p_slist l)
{
  p_slist p=NULL;
  while(l)
  {
    p_slist n=l->next;
    l->next=p;
    p=l;
    l=n;
  }
  return p;
}
void adms_slist_inreverse (p_slist* l)
{
  *l=adms_slist_reverse(*l);
}
void adms_slist_free (p_slist l)
{
  while(l)
  {
    p_slist freed=l;
    l=l->next;
    free(freed);
  }
}
