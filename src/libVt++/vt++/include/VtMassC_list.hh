#ifndef __VTMASSC_LIST_HH
#define __VTMASSC_LIST_HH
// *****************************************************************************
//
// source:
//
// type:      source code
//
// created:   20. Okt 2000
//
// author:    Thorsten Glebe
//            HERA-B Collaboration
//            Max-Planck-Institut fuer Kernphysik
//            Saupfercheckweg 1
//            69117 Heidelberg
//            Germany
//            E-mail: T.Glebe@mpi-hd.mpg.de
//
// Description: definition of MassC container
//
// changes:
// 20 Okt 2000 (TG) creation
//
// *****************************************************************************
#include <list>

namespace VERTEX {
  class MassC;

  typedef std::list<MassC*>                  MassC_v;
  typedef std::list<MassC*>::iterator        MassC_it;
  typedef std::list<MassC*>::const_iterator  MassC_cit;

} // end of namespace VERTEX
#endif
