#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedefs;
#pragma link C++ namespace VERTEX;
#pragma link C++ namespace MATRIX;

#pragma link C++ class VERTEX::Track;
#pragma link C++ class VERTEX::Relation;
#pragma link C++ class VERTEX::Kalman;
#pragma link C++ class VERTEX::MassC;
#pragma link C++ class VERTEX::RelationList;
#pragma link C++ class VERTEX::RelationIterator;
#pragma link C++ class VERTEX::ConstRelationIterator;
#pragma link C++ class VERTEX::ReverseRelationIterator;
#pragma link C++ class VERTEX::ConstReverseRelationIterator;
#pragma link C++ class VERTEX::Vertex;

#pragma link C++ class MATRIX::VtMatrix;
#pragma link C++ class MATRIX::VtNegMatrix;
#pragma link C++ class MATRIX::VtSqMatrix;
#pragma link C++ class MATRIX::VtSymMatrix;
#pragma link C++ class MATRIX::CMatrix;
#pragma link C++ class MATRIX::VtVector;

#endif
