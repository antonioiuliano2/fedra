#ifndef __VTRELATIONLIST_HH
#define __VTRELATIONLIST_HH
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
// 04 Sep 2000 (TG) RelationList::erase(), RelationList::unbook() added
// 04 Sep 2000 (TG) RelationIterator, ConstRelationIterator friend of RelationList
// 04 Sep 2000 (TG) RelationList not a friend of it´s iterators any more
// 02 Okt 2000 (TG) ReverseRelationIterator, ConstReverseRelationIterator added
// 02 Okt 2000 (TG) rbegin(), rend() member functions added
// 27 Okt 2000 (TG) #ifndef __CINT__ added
// 01 Nov 2000 (TG) operator!=() added for iterators
// 09 Mar 2001 (TG) changed size() return value to unsigned int,
//                  added RelationList::clear()
//
// *****************************************************************************

#include <iosfwd>
#include <list>

namespace VERTEX {
  class Relation;
  class RelationIterator;
  class ConstRelationIterator;
  class ReverseRelationIterator;
  class ConstReverseRelationIterator;
  class Track;
  class Vertex;

  bool createRelation(Track& t, Vertex& v);
  
  typedef RelationIterator             iterator;
  typedef ConstRelationIterator        const_iterator;
  typedef ReverseRelationIterator      reverse_iterator;
  typedef ConstReverseRelationIterator const_reverse_iterator;

  typedef std::list<Relation*>                          Relation_v;
  typedef std::list<Relation*>::iterator                Relation_it;
  typedef std::list<Relation*>::const_iterator          Relation_cit;
  typedef std::list<Relation*>::reverse_iterator        Relation_rit;
  typedef std::list<Relation*>::const_reverse_iterator  Relation_rcit;

  //============================================================================
  // Class RelationIterator
  //============================================================================
  class RelationIterator {
  private:
    friend class RelationList;
    Relation_it   iter;

  public:
    typedef RelationIterator self;
#ifndef __CINT__
    typedef Relation_it::iterator_category iterator_category;
#endif
    typedef Relation_it::value_type        value_type;
    typedef Relation_it::pointer           pointer;
    typedef Relation_it::reference         reference;
    typedef Relation_it::size_type         size_type;
    typedef Relation_it::difference_type   difference_type;

    RelationIterator();
    RelationIterator(const Relation_it& it):iter(it) {}

    self&     operator++();
    self&     operator--();
    Relation& operator*()  const;
    Relation* operator->() const;

    inline int operator==(const RelationIterator& rhs) const {
      return iter == rhs.iter;
    }
    inline int operator!=(const RelationIterator& rhs) const {
      return iter != rhs.iter;
    }
  }; // class RelationIterator
  

  //============================================================================
  // Class ConstRelationIterator
  //============================================================================
  class ConstRelationIterator {
  private:
    friend class RelationList;
    Relation_cit   iter;

  public:
    typedef ConstRelationIterator self;
#ifndef __CINT__
    typedef Relation_cit::iterator_category iterator_category;
#endif
    typedef Relation_cit::value_type        value_type;
    typedef Relation_cit::pointer           pointer;
    typedef Relation_cit::reference         reference;
    typedef Relation_cit::size_type         size_type;
    typedef Relation_cit::difference_type   difference_type;


    ConstRelationIterator();
    ConstRelationIterator(const Relation_cit& it):iter(it) {}

    self&     operator++();
    self&     operator--();
    const Relation& operator*()  const;
    const Relation* const operator->() const;

    inline int operator==(const ConstRelationIterator& rhs) const {
      return iter == rhs.iter;
    }
    inline int operator!=(const ConstRelationIterator& rhs) const {
      return iter != rhs.iter;
    }
  }; // class ConstRelationIterator


  //============================================================================
  // Class ReverseRelationIterator
  //============================================================================
  class ReverseRelationIterator {
  private:
    friend class RelationList;
    Relation_rit   iter;

  public:
    typedef ReverseRelationIterator self;
#ifndef __CINT__
    typedef Relation_rit::iterator_category iterator_category;
#endif
    typedef Relation_rit::value_type        value_type;
    typedef Relation_rit::pointer           pointer;
    typedef Relation_rit::reference         reference;
    typedef Relation_rit::difference_type   difference_type;

    ReverseRelationIterator();
    ReverseRelationIterator(const Relation_rit& it):iter(it) {}

    self&     operator++();
    self&     operator--();
    Relation& operator*()  const;
    Relation* operator->() const;

    inline int operator==(const ReverseRelationIterator& rhs) const {
      return iter == rhs.iter;
    }
    inline int operator!=(const ReverseRelationIterator& rhs) const {
      return iter != rhs.iter;
    }
  }; // class ReverseRelationIterator
  

  //============================================================================
  // Class ConstReverseRelationIterator
  //============================================================================
  class ConstReverseRelationIterator {
  private:
    friend class RelationList;
    Relation_rcit   iter;

  public:
    typedef ConstReverseRelationIterator self;
#ifndef __CINT__
    typedef Relation_rcit::iterator_category iterator_category;
#endif
    typedef Relation_rcit::value_type        value_type;
    typedef Relation_rcit::pointer           pointer;
    typedef Relation_rcit::reference         reference;
    typedef Relation_rcit::difference_type   difference_type;


    ConstReverseRelationIterator();
    ConstReverseRelationIterator(const Relation_rcit& it):iter(it) {}

    self&     operator++();
    self&     operator--();
    const Relation& operator*()  const;
    const Relation* const operator->() const;

    inline int operator==(const ConstReverseRelationIterator& rhs) const {
      return iter == rhs.iter;
    }
    inline int operator!=(const ConstReverseRelationIterator& rhs) const {
      return iter != rhs.iter;
    }
  }; // class ConstReverseRelationIterator


  /** Relation container class */
  //============================================================================
  // Class RelationList - class for Vertex-Track-Kalman relation container
  //============================================================================
  class RelationList {
  public:
    typedef RelationIterator             iterator;
    typedef ConstRelationIterator        const_iterator;
    typedef ReverseRelationIterator      reverse_iterator;
    typedef ConstReverseRelationIterator const_reverse_iterator;

    /** @name --- Constructors --- */
    ///
    RelationList();
    ///
    virtual ~RelationList();

    /** @name --- Access functions --- */
    /// clear all relations
    virtual void clear();
    /// add a relation
    inline void push__back(Relation* rel) { rellist.push_back(rel); }
    /// return last relation
    Relation* back() const { return rellist.back(); }
    /// return no of relations
    inline unsigned int  size() const { return rellist.size(); }
    /// Erase an element
    virtual const iterator erase(const iterator& pos);

    /** @name --- Iterator methods --- */
    ///
    inline               iterator begin()        { return iterator(rellist.begin()); }
    ///
    inline               iterator end()          { return iterator(rellist.end()); }
    ///
    inline         const_iterator begin()  const { return const_iterator(rellist.begin()); }
    ///
    inline         const_iterator end()    const { return const_iterator(rellist.end()); }
    ///
    inline       reverse_iterator rbegin()       { return reverse_iterator(rellist.rbegin()); }
    ///
    inline       reverse_iterator rend()         { return reverse_iterator(rellist.rend()); }
    ///
    inline const_reverse_iterator rbegin() const { return const_reverse_iterator(rellist.rbegin()); }
    ///
    inline const_reverse_iterator rend()   const { return const_reverse_iterator(rellist.rend()); }

    /// direct access
    Relation *const operator[](unsigned int i) const;

    /** @name --- Expert functions --- */
    ///
    void print(std::ostream& os) const;
    /// get rid of relation pointer
    inline  void unbook(Relation* const rel) { rellist.remove(rel); }
    /// remove a relation
    virtual void remove(Relation* const rel);

    const RelationList& operator=(const RelationList& rhs);

    inline bool operator==(const RelationList& rhs) const {
      return rellist == rhs.rellist;
    }

  private:
    Relation_v   rellist;
  }; // class RelationList

  //==============================================================================
  // operator<<
  //==============================================================================
  inline std::ostream& operator<<(std::ostream& s, const RelationList& a) {
    a.print(s);
    return s;
  }

} // end of namespace VERTEX
#endif
