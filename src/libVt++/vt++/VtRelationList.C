// *****************************************************************************
//
// source:
//
// type:      source code
//
// created:   21. Aug 2000
//
// author:    Thorsten Glebe
//            HERA-B Collaboration
//            Max-Planck-Institut fuer Kernphysik
//            Saupfercheckweg 1
//            69117 Heidelberg
//            Germany
//            E-mail: T.Glebe@mpi-hd.mpg.de
//
// Description: relation container class
//
// changes:
// 21 Aug 2000 (TG) creation
// 04 Sep 2000 (TG) reprogrammed remove(), added erase()
// 02 Okt 2000 (TG) added ReverseIterator, ConstReverseIterator code
// 27 Dec 2000 (TG) end() optimization
// 09 Jul 2001 (TG) added clear()
//
// *****************************************************************************
#include "Riostream.h"
#include "vt++/VtVertex.hh"
#include "vt++/VtTrack.hh"
#include "vt++/VtRelation.hh"
#include "vt++/VtRelationList.hh"

namespace VERTEX {


  //=============================================================================
  // createRelation
  //=============================================================================
  bool createRelation(Track& t, Vertex& v) {
    //    cout << " Create Relation called!! " << &t << " " << &v << endl;
    Relation* rel = new Relation(t,v);
    t.push__back(rel);
    v.push__back(rel);
    return true;
  }

  //=============================================================================
  // Constructors
  //=============================================================================
  RelationList::RelationList() {}

  RelationList::~RelationList() { clear(); }

  //=============================================================================
  // clear: remove all relations
  //=============================================================================
  void RelationList::clear() {
    if(rellist.empty()) return;
    
    const Relation_cit end = rellist.end();
    if(static_cast<RelationList*>(&rellist.back()->vertex) == this) {
      // Dtor called from ~Vertex
      //      cout << "delete in vertex! " << this << endl;
      for(Relation_cit it=rellist.begin(); it!=end; ++it) {
	Relation* rel = *it;
	//  	cout << " Delete Relation " << rel 
	//  	     << " " << rel->track << endl;
	rel->track.unbook(rel);  // unbook relation
	delete rel;              // destroy relation
      }
    } else {
      // Dtor called from ~Track
      //      cout << "delete in track! " << this << endl;
      for(Relation_cit it=rellist.begin(); it!=end; ++it) {
	Relation* rel = *it;
	//  	cout << " Delete Relation " << rel 
	//  	     << " " << rel->vertex << endl;
	rel->vertex.unbook(rel); // unbook relation
	delete rel;              // destroy relation
      }
    }
    rellist.clear();
  }

  //=============================================================================
  // remove
  //=============================================================================
  void RelationList::remove(Relation* const rel) {
    if(static_cast<RelationList*>(&rel->vertex) == this) {
      // called from Vertex
      rel->track.unbook(rel);
    } else {
      // called from Track
      rel->vertex.unbook(rel);
    }

    delete rel;
    rellist.remove(rel);
    return;
  }

  //=============================================================================
  // erase
  //=============================================================================
  const iterator RelationList::erase(const iterator& pos) {
    Relation* const rel = &(*pos);
    if(static_cast<RelationList*>(&rel->vertex) == this) {
      // called from Vertex
      rel->track.unbook(rel);
    } else {
      // called from Track
      rel->vertex.unbook(rel);
    }
    delete rel;
    return iterator(rellist.erase(pos.iter));
  }

  //=============================================================================
  // operator =
  //=============================================================================
  const RelationList& RelationList::operator=(const RelationList& rhs) {
    if( this == &rhs ) { return *this; }
    
    rellist = rhs.rellist;
    return *this;
  }

  //==============================================================================
  // print
  //==============================================================================
  void RelationList::print(std::ostream& os) const {
    const Relation_cit end = rellist.end();
    for(Relation_cit it=rellist.begin(); it!=end; ++it)
      cout << **it << " ";
    return;
  }

  //=============================================================================
  // operator[] easy but slow access to Relation members
  //=============================================================================
  Relation *const RelationList::operator[](unsigned int i) const {
    if(i>rellist.size()) return 0;

    Relation_cit it=rellist.begin();
    unsigned int j=0;
    while(j++<i) ++it;
    //    cout << "pos: " << j << endl;
    return *it;
  }

  //==============================================================================
  //
  // class RelationIterator member functions
  //
  //==============================================================================
  RelationIterator::RelationIterator() {}


  //==============================================================================
  // operator++
  //==============================================================================
  RelationIterator::self& RelationIterator::operator++() {
    ++iter;
    return *this;
  }

  //==============================================================================
  // operator--
  //==============================================================================
  RelationIterator::self& RelationIterator::operator--() {
    --iter;
    return *this;
  }

  //==============================================================================
  // operator*
  //==============================================================================
  Relation& RelationIterator::operator*() const { 
    return **iter;
  }

  //==============================================================================
  // operator->
  //==============================================================================
  Relation* RelationIterator::operator->() const { 
    return *iter;
  }

  //==============================================================================
  //
  // class ConstRelationIterator member functions
  //
  //==============================================================================
  ConstRelationIterator::ConstRelationIterator() {}


  //==============================================================================
  // operator++
  //==============================================================================
  ConstRelationIterator::self& ConstRelationIterator::operator++() {
    ++iter;
    return *this;
  }

  //==============================================================================
  // operator--
  //==============================================================================
  ConstRelationIterator::self& ConstRelationIterator::operator--() {
    --iter;
    return *this;
  }

  //==============================================================================
  // operator*
  //==============================================================================
  const Relation& ConstRelationIterator::operator*() const { 
    return **iter;
  }

  //==============================================================================
  // operator->
  //==============================================================================
  const Relation* const ConstRelationIterator::operator->() const { 
    return *iter;
  }

  //==============================================================================
  //
  // class ReverseRelationIterator member functions
  //
  //==============================================================================
  ReverseRelationIterator::ReverseRelationIterator() {}


  //==============================================================================
  // operator++
  //==============================================================================
  ReverseRelationIterator::self& ReverseRelationIterator::operator++() {
    ++iter;
    return *this;
  }

  //==============================================================================
  // operator--
  //==============================================================================
  ReverseRelationIterator::self& ReverseRelationIterator::operator--() {
    --iter;
    return *this;
  }

  //==============================================================================
  // operator*
  //==============================================================================
  Relation& ReverseRelationIterator::operator*() const { 
    return **iter;
  }

  //==============================================================================
  // operator->
  //==============================================================================
  Relation* ReverseRelationIterator::operator->() const { 
    return *iter;
  }

  //==============================================================================
  //
  // class ConstReverseRelationIterator member functions
  //
  //==============================================================================
  ConstReverseRelationIterator::ConstReverseRelationIterator() {}


  //==============================================================================
  // operator++
  //==============================================================================
  ConstReverseRelationIterator::self& ConstReverseRelationIterator::operator++() {
    ++iter;
    return *this;
  }

  //==============================================================================
  // operator--
  //==============================================================================
  ConstReverseRelationIterator::self& ConstReverseRelationIterator::operator--() {
    --iter;
    return *this;
  }

  //==============================================================================
  // operator*
  //==============================================================================
  const Relation& ConstReverseRelationIterator::operator*() const { 
    return **iter;
  }

  //==============================================================================
  // operator->
  //==============================================================================
  const Relation* const ConstReverseRelationIterator::operator->() const { 
    return *iter;
  }
} // end of namespace VERTEX
