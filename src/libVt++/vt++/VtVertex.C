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
// Description: Vertex class
//
// changes:
// 21 Aug 2000 (TG) creation
// 25 Aug 2000 (TG) remove_worst added
// 04 Sep 2000 (TG) Modified argument of VtRemoveTrack()
// 04 Sep 2000 (TG) fixed memory leak in push_back()
// 05 Sep 2000 (TG) remove_worst calls VtRemoveTrack() for track removal
// 05 Sep 2000 (TG) findVertexVt() checks first if >= 2 tracks on vertex
// 05 Sep 2000 (TG) erase() member function added
// 13 Sep 2000 (TG) VtMass() dummy added
// 14 Sep 2000 (TG) bug fix in calculating the track-vertex chi2 distance
// 02 Okt 2000 (TG) added chi2l() member function
// 04 Okt 2000 (TG) modified use_momentum() member function
// 11 Okt 2000 (TG) kalman->init() called in findVertexVt()
// 18 Okt 2000 (TG) code for MassConstr() added, bug fixes in bigcov()
// 23 Okt 2000 (TG) finished VtMass() code
// 24 Okt 2000 (TG) added invalid() code, modified bigcov()
// 25 Okt 2000 (TG) small modification in calc_mother(), calc_mother_cov()
// 25 Okt 2000 (TG) mass() modified: now also permanent track rest-mass is handled
// 26 Okt 2000 (TG) #ifndef NO_ARTE added (for integration into CLUE)
// 14 Nov 2000 (TG) added constructor code for CLUE (RecoSegment, ArtePointer<RecoTrack>)
// 20 Nov 2000 (TG) added using namespace CLUE
// 24 Nov 2000 (TG) removed using namespace CLUE
// 25 Nov 2000 (TG) added RecoTrack constructor, added createRecoTrack()
// 15 Dec 2000 (TG) small improvement in createRecoVertex()
// 27 Dec 2000 (TG) end() optimization
// 02 Jan 2001 (TG) end() bug fixes 
// 02 Feb 2001 (TG) added global ClMember objects
// 11 Feb 2001 (TG) modified createRecoVertex(), added #include RecoTrack
// 20 Feb 2001 (TG) pmaxfrac() added, ClMember pmaxfrac added
// 24 Feb 2001 (TG) improved operator=(), bug fix in copy constructor
// 27 Feb 2001 (TG) improved setting of v_valid and v_Mvalid
// 28 Feb 2001 (TG) calc_mother: computation dependent on v_use_kalman
// 06 Mar 2001 (TG) v_angdist added
// 09 Mar 2001 (TG) bug fix in clearMassConstr()
// 12 Mar 2001 (TG) findVertexVt(): assure at least 2 iteration steps
// 17 Apr 2001 (TG) bug fix in calc_mother_cov(), jac(i,4)
// 09 Jul 2001 (TG) added clear();
// 10 Jul 2001 (TG) bug fix in clear();
// 04 Sep 2001 (TG) added push_back(TrackIf), added VertexIf inheritance
// 17 Okt 2001 (TG) added/improved #defines for ARTE
// 29 Okt 2001 (TG) bug fix in findVertex3D()
//
// *****************************************************************************
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "vt++/VtVertex.hh"
#include "vt++/VtTrack.hh"
#include "vt++/VtKalman.hh"
#include "vt++/VtRelation.hh"
#include "vt++/VtNegMatrix.hh"
#include "vt++/VtMassC.hh"

#if defined USE_ROOT
//#include "clue/cluearte.hh"
//#include "geometry/Point3D.hh" // for RecoSegment.hh
//#include "clue/RecoTrack.hh"
//#include "clue/RecoVertex.hh"
#include "smatrix/SVector.hh"
//#include "interfaces/TrackIf.hh"
#endif // USE_ROOT


ClassImp(VERTEX::Vertex);

extern "C" {
  // from CERNLIB (libmathlib.a)
  float prob_(float* chi, int* ndf);
}
// CERNLIB interface function
static float CERN_prob(float X, int N) { return prob_(&X, &N); }

using namespace MATRIX;

namespace VERTEX {

  static const VtSymMatrix CINV0(3,1.e-5);
  static const VtVector    kal_xv0(3);

  //==============================================================================
  // Constructors
  //==============================================================================
  Vertex::Vertex() :
    Track       (),
    v_use_kalman(false),
    v_valid     (false),
    v_Mvalid    (false),
    v_mother    (false),
    v_angdist   (false),
    v_angle     (0.),
    v_dist      (0.),
    v_bk13      (0.),
    v_chi2      (0.),
    kal_xv      (VtVector(3)),
    kal_xvs     (VtVector(3)),
    v_CS        (VtSymMatrix(3,1.e5)),
    v_CINV      (CINV0),
    v_covn      (0),
    tracks      (),
    xmass       ()
  {}

  Vertex::Vertex(Track& t1, Track& t2):
    Track       (),
    v_use_kalman(false),
    v_valid     (false),
    v_Mvalid    (false),
    v_mother    (false),
    v_angdist   (false),
    v_angle     (0.),
    v_dist      (0.),
    v_bk13      (0.),
    v_chi2      (0.),
    kal_xv      (VtVector(3)),
    kal_xvs     (VtVector(3)),
    v_CS        (VtSymMatrix(3,1.e5)),
    v_CINV      (CINV0),
    v_covn      (0),
    tracks      (),
    xmass       ()
 {
    createRelation(t1, *this);
    createRelation(t2, *this);
  }

  Vertex::Vertex(const vector<Track*>& v):
    Track       (),
    v_use_kalman(false),
    v_valid     (false),
    v_Mvalid    (false),
    v_mother    (false),
    v_angdist   (false),
    v_angle     (0.),
    v_dist      (0.),
    v_bk13      (0.),
    v_chi2      (0.),
    kal_xv      (VtVector(3)),
    kal_xvs     (VtVector(3)),
    v_CS        (VtSymMatrix(3,1.e5)),
    v_CINV      (CINV0),
    v_covn      (0),
    tracks      (),
    xmass       ()
  {
    const vector<Track*>::const_iterator end = v.end();
    for(vector<Track*>::const_iterator it = v.begin(); it != end; ++it) {
      createRelation(**it, *this);
    }
  }

  Vertex::Vertex(const Vertex& rhs):
    Track       (rhs),
    v_use_kalman(rhs.v_use_kalman),
    v_valid     (rhs.v_valid),
    v_Mvalid    (rhs.v_Mvalid),
    v_mother    (rhs.v_mother),
    v_angdist   (rhs.v_angdist),
    v_angle     (rhs.v_angle),
    v_dist      (rhs.v_dist),
    v_bk13      (rhs.v_bk13),
    v_chi2      (rhs.v_chi2),
    kal_xv      (rhs.kal_xv),
    kal_xvs     (rhs.kal_xvs),
    v_CS        (rhs.v_CS),
    v_CINV      (v_CS.dsinv()),
    v_covn      (0)
    //    tracks  (rhs.tracks),
    //    xmass   (rhs.xmass)
  {
    // do not copy relations
  }

  Vertex::~Vertex() {
    // delete the mass constraints
    clearMassConstr();
    // delete tracks owned by vertex
    const Track_cit end = tracks.end();
    for(Track_cit it = tracks.begin(); it != end; ++it) {
      delete *it;
    }
    // delete big covariance matrix
    if(v_covn != 0) delete v_covn;
  }
  


  
  //==============================================================================
  // clear(), resets completly Vertex object
  //==============================================================================
  void Vertex::clear() {
    RelationList::clear();
    invalid();
    v_mother = v_use_kalman = false;
    // delete the mass constraints
    clearMassConstr();
    // delete tracks owned by vertex
    const Track_cit end = tracks.end();
    for(Track_cit it = tracks.begin(); it != end; ++it) {
      delete *it;
    }
    tracks.clear();
    // delete big covariance matrix
    if(v_covn != 0) { 
      delete v_covn; 
      v_covn = 0;
    }
  }

  //==============================================================================
  // invalid() actions to be taken if vertex becomes invalid
  //==============================================================================
  void Vertex::invalid() {
    v_valid = v_Mvalid = false;

    if(v_covn != 0) {
      delete v_covn;
      v_covn = 0;
    }

    return;
  }


  //==============================================================================
  // VertexIf access methods
  //==============================================================================
  float Vertex::vx()      const { return kal_xvs[0];  }
  float Vertex::vy()      const { return kal_xvs[1];  }
  float Vertex::vz()      const { return kal_xvs[2];  }
  float Vertex::chi2()    const { return v_chi2; }
  float Vertex::prob()    const { return CERN_prob(chi2(), ndf()); }
  unsigned short int Vertex::ndf() const
    { return ((v_Mvalid == true) ? 2*ntracks()-3 + xmass.size() : 2*ntracks()-3); }
  unsigned short int Vertex::ntracks() const { return size(); }

#if defined USE_ROOT
//  const TrackIf* Vertex::track(unsigned int i) const { return 0; }
//  const TrackIf*& Vertex::track(unsigned int i) 
//    { static const TrackIf* ptr=0; return ptr; }

  SVector<double,3> Vertex::vpos() const {
    return SVector<double,3>(kal_xvs[0], kal_xvs[1], kal_xvs[2]);
  }
  const SMatrix<double,3>& Vertex::VCOV() const {
    static SMatrix<double,3> vcov_;
    vcov_ = SMatrix<double,3>(v_CS.array());
    return vcov_;
  }
#endif

  float Vertex::vtx_cov_x()   const { return v_CS(0,0); }
  float Vertex::vtx_cov_y()   const { return v_CS(1,1); }
  float Vertex::vtx_cov_z()   const { return v_CS(2,2); }

  //==============================================================================
  // push_back
  //==============================================================================
  void Vertex::push_back(Track& track) {
    createRelation(track, *this);
    invalid();
  }

  //==============================================================================
  // use_momentum: set for all tracks whether momentum should be used
  //==============================================================================
  void Vertex::use_momentum(const bool use) {
    const RelationList::iterator last = end();
    for(RelationList::iterator tit = begin(); tit != last; ++tit) {
      tit->kalman.use_momentum(use);
    }
    return;
  }

  //==============================================================================
  // remove: remove a relation
  //==============================================================================
  void Vertex::remove(Relation* r){
    RelationList::remove(r);
    invalid();
  }

  //==============================================================================
  // erase: erase a relation
  //==============================================================================
  const iterator Vertex::erase(const iterator& pos) {
    invalid();
    return RelationList::erase(pos);
  }

  //==============================================================================
  // MassConstr: include all tracks in single mass constraint
  //==============================================================================
  void Vertex::MassConstr(double m) {
    MassC* massc = new MassC(m);
    const RelationList::iterator last = end();
    for(RelationList::iterator it = begin(); it != last; ++it) {
      massc->push_back(*it);
    }

    xmass.push_back(massc);
    return;
  }

  //==============================================================================
  // addMassConstr: add a mass constraint
  //==============================================================================
  MassC& Vertex::addMassConstr(double m) {
    xmass.push_back(new MassC(m));
    return *xmass.back();
  }

  //==============================================================================
  // clearMassConstr: clear all mass constraints
  //==============================================================================
  void Vertex::clearMassConstr() {
    // delete mass constraints owned by vertex
    const MassC_cit end = xmass.end();
    for(MassC_cit it = xmass.begin(); it != end; ++it) {
      delete *it;
    }
    xmass.clear();
    return;
  }

  //==============================================================================
  // bigcov: return general covariance matrix
  //==============================================================================
  const VtSymMatrix& Vertex::bigcov() {

    // build up big cov. matrix (in Vt: covn)
    // covn consists of vertex + track parameter cov. matrices:
    //  vtx  tr1  tr2  tr3  ...
    //
    //  CS | ES | ES | ES | ...   CC = cor(q_k,q_l) = F_k*C_n*F_l^T
    //  ---+----+----+----+          = -ES_k * F_l
    //  ES | DS | CC | CC | ...
    //  ---+----+----+----+ 
    //  ES | CC | DS | CC | ...
    //  ---+----+----+----+
    //  ES | CC | CC | DS | ...
    //  ...

    if(v_covn == 0)
      v_covn = new VtSymMatrix(bigdim());

    VtSymMatrix& covn = *v_covn;

    //    VtSymMatrix covn(bigdim());

    covn.place_at(v_CS,0,0);

    unsigned int ipos = 3;
    const iterator last = end();
    for(iterator it = begin(); it != last; ++it) {
      const Kalman& kalman = it->kalman;

      covn.place_at(kalman.ES(), ipos, 0);
      covn.place_at(kalman.DS(), ipos, ipos);

      // fill cor(q_k^n,q_l^n) = F_k*C_n*F_l^T = -E_k^n * F_l^T
      unsigned int jpos = 6;
      iterator tmp=it;
      for(iterator it2=++tmp; it2 != last; ++it2) {
	const Kalman& kalman2 = it2->kalman;
	
	covn.place_at(-kalman.ES()*kalman2.F().T(), ipos,jpos);
	jpos+=3;
      }

      ipos+=3;
    }

    return covn;
  }

  //==============================================================================
  // calc_mother: calculate mother particle properties
  //==============================================================================
  bool Vertex::calc_mother(bool use_kalman) {
    //    if(v_valid == false) return v_mother = false;

    // add up track momenta
    t_p[5] = 0;
#if defined USE_ROOT
    SVector<double,3> psum;
#else
    VtVector psum(0,0,0);
#endif

    const iterator last = end();
    if(use_kalman == true) {
      for(iterator it = begin(); it != last; ++it) {
	const Kalman& t = it->kalman;
	t_p[5] += t.p();
	psum   += t.pvec();
      }

      if(fabs(psum[2])>1e-20) {
	t_p[3] = psum[0]/psum[2]; // tx = pxsum/pzsum
	t_p[4] = psum[1]/psum[2]; // ty = pysum/pzsum
      } else {
	return v_mother = false;
      }

      // x,y at z = 0
      t_p[0] = vx() - vz() * tx();
      t_p[1] = vy() - vz() * ty();
      t_p[2] = 0.;

    } else {
      double x, y, z;
      VtEstimateVertexMath(x,y,z);
      for(iterator it = begin(); it != last; ++it) {
	const Track& t = it->track;
	t_p[5] += t.p();
	psum   += t.pvec();
      }

      if(fabs(psum[2])>1e-20) {
	t_p[3] = psum[0]/psum[2]; // tx = pxsum/pzsum
	t_p[4] = psum[1]/psum[2]; // ty = pysum/pzsum
      } else {
	return v_mother = false;
      }

      // x,y at z = 0
      t_p[0] = x - z * tx();
      t_p[1] = y - z * ty();
      t_p[2] = 0.;
    }

    //    t_p[5] = sqrt(sqr(pxsum) + sqr(pysum) + sqr(pzsum));

    // what is the difference betwenn sum of track.p() and
    // calculation sqrt(sqr(pxsum)+sqr(pysum)+sqr(pzsum))?????
    //      cout << " reco-p: " << sqrt(sqr(pxsum) + sqr(pysum) + sqr(pzsum))
    //    	 << " orig reco p: " << p() << endl;

    return v_mother = true;
  }

  //==============================================================================
  // calc_mother: calculate mother particle covariance Matrix
  //==============================================================================
  bool Vertex::calc_mother_cov() {
    // no track parameters computed
    if(v_mother == false || v_valid == false) return false;
    // get big covariance matrix
    const VtSymMatrix& covn = (v_Mvalid==false)? bigcov() : *v_covn;

    // fill jacobian
    VtMatrix    jac(bigdim(),5);

    // unit vectors
    double epx = ex();
    double epy = ey();
    double epz = ez();

    // vertex part:
    jac(0,0) = jac(1,1) = 1.;
    jac(2,0) = -tx();
    jac(2,1) = -ty();

    double fact3 = 1./pz();
    //    double fact3 = 1./(p() * epz);

    unsigned int i = 2;
    const iterator last = end();
    for(iterator it = begin(); it != last; ++it) {
      const Kalman& t = it->kalman;
      
      // unit vectors along track:
      double ex = t.ex();
      double ey = t.ey();
      double ez = t.ez();

      double fact2 = t.pz();
      double fact  = fact2 * fact3;

      ++i;
      jac(i,2) = fact * (1. - sqr(ex) + tx() * ex * ez);
      jac(i,3) = fact * ex * (ty()*ez - ey);
      jac(i,0) = -vz() * jac(i,2);
      jac(i,1) = -vz() * jac(i,3);

      jac(i,4) = fact2 * ( epx*(1.-sqr(ex)) - epy*ex*ey - epz*ex*ez );
      
      ++i;
      jac(i,2) = fact*ey*(tx()*ez-ex);
      jac(i,3) = fact*(1. - sqr(ey) + ty()*ey*ez);
      jac(i,0) = -vz() * jac(i,2);
      jac(i,1) = -vz() * jac(i,3);

      jac(i,4) = fact2 * ( epy*(1.-sqr(ey)) - epx*ex*ey - epz*ey*ez );

      ++i;
      jac(i,2) = fact3 * (ex - tx()*ez);
      jac(i,3) = fact3 * (ey - ty()*ez);
      jac(i,0) = -vz() * jac(i,2);
      jac(i,1) = -vz() * jac(i,3);
      
      jac(i,4) = epx*ex + epy*ey + epz*ez;
    }

#ifdef VtDEBUG
    cout << " jac: " << jac << endl;
#endif

    // compute Track covariance matrix:
    t_V = covn.product(jac);
    // compute inverse
    t_G = t_V;
    t_G.VtDsinv();
    t_GM.copy(t_V.dsinv(4)); // inverse without momentum

    return true;
  }

  //==============================================================================
  // VtMass: mass constrained vertex fit (in Vt: vtmass.F)
  //==============================================================================
  const bool Vertex::VtMass() {
    v_Mvalid = false;

    // should be assured:
    use_momentum(true);
    if(valid() == false && findVertexVt() == false) return false;

    // check kinematic boundaries
    const MassC_cit xend = xmass.end();
    for(MassC_cit it = xmass.begin(); it != xend; ++it) {
      if((*it)->check_kinematic() == false)
	return false;
    }

    // fill general state vectors (in Vt: alp(1,...), alpc(1,...))
    VtVector alpha (bigdim());
    VtVector alphac(bigdim());

    // initialize general state vector alphac (in Vt: alpc)
    alphac.place_at(xvs(),0);

    unsigned int i=3;
    const iterator last = end();
    for(iterator it = begin(); it != last; ++it,i+=3) {
      it->kalman.alpc_init();
      alphac.place_at(it->kalman.alpc(),i);
    }

#ifdef VtDEBUG
    cout << " alphac0: " << alphac << endl;
#endif

    double chi2old = -100.;
    double dchi2 = 0.;
    double mc_chi2 = 0.;
    const unsigned int maxiter = 100 * nMassConstr();

    // declare some matrices which are needed outside do-loop
    VtSymMatrix gaVga(nMassConstr());
    VtMatrix    Vgam(bigdim(),nMassConstr());

    // iteration
    unsigned int iter = 0;
    do {

      if(iter > 0) {
	// redo filter and smoother
	kal_xvs.copy(alphac,0);
	unsigned int i=3;
	for(iterator it = begin(); it != last; ++it,i+=3) {
	  Kalman& kalm = it->kalman;
	  kalm.qvs_nc().copy(alphac,i);
	  kalm.alpc_init();
	}
	if((v_valid = VtFit()) == false)
	  return false;
      }

      // fill general state vector alpha (in Vt: alp)
      alpha.place_at(xvs(),0);

      i=3;
      for(iterator it = begin(); it != last; ++it,i+=3) {
	Kalman& kalm = it->kalman;

	kalm.alp_init();
	kalm.calc_ealpc(); // n in Vt (eq. 40)
	alpha.place_at(kalm.alp(),i);
      }

#ifdef VtDEBUG
      cout << " alpha: " << alpha << endl;
#endif

      // get general covariance matrix v_covn (in Vt: Valp)
      const VtSymMatrix& Valp = bigcov();

#ifdef VtDEBUG
      cout << "Valp: " << Valp << endl;
#endif

      // build up matrix gam (in Vt: Gamma^T, eq. 34, 44, 45, 46)
      VtMatrix gam(bigdim(),nMassConstr());

      // vector \vect{g}(\vect{\alpha}^(0))
      VtVector galp(nMassConstr());

      unsigned int j=0;
      for(MassC_cit mit = xmass.begin(); mit != xend; ++mit,++j) {
	MassC* mC = *mit;
	mC->calc_Sl();
	galp[j] = mC->galp();

	i=3;
	for(iterator it = begin(); it != last; ++it,i+=3) {
	  Relation& r = *it;
	  if(find(r.massc.begin(),r.massc.end(),mC) != r.massc.end()) {
	    gam.place_at(mC->gam(r),i,j);
	  }
	}
      } // end of for(MassC_cit)

#ifdef VtDEBUG
      cout << " galp: " << galp << endl;
      cout << " gam: " << gam << endl;
#endif

      // calc (Gamma * V_alpha * Gamma^T)^-1 (in Vt: eq. 36,37,39)
      gaVga = Valp.product(gam);
      if(gaVga.VtDsinv() == false)
	return false;
      
#ifdef VtDEBUG
      cout << " gaVga^-1: " << gaVga << endl;
#endif

      // calc g(alpha) + Gamma * (alpha - alpha_c)
      VtVector gnum = galp + gam.T() * (alpha - alphac);

      // calc \vect{\mu} (in Vt: xmu, eq. 36)
      VtVector xmu = gaVga * gnum;

#ifdef VtDEBUG
      cout << " xmu: " << xmu << endl;
#endif

      // calc V_alpha*Gamma^T
      VtMatrix Vgam = Valp * gam;

      // calc alpha_c (in Vt: eq. 37)
      alphac = alpha - Vgam * xmu;
      
#ifdef VtDEBUG
      cout << " alphac: " << alphac << endl;
#endif

      // calc chi^2 (in Vt: chi2m, eq. 39)
      // this is an incremental chi2!
      // (chi2 due to mass constraint, to be added to
      // chi2 of unconstraint vertex fit!)
      mc_chi2 = gaVga.product(gnum);

      double chi  = mc_chi2 + chi2n();
      dchi2 = fabs(chi - chi2old);
      chi2old = chi;

#ifdef VtDEBUG
      cout << "chi2m: " << setprecision(7) << mc_chi2 
	   << " chi: " << chi2n()
	   << " fabs(chi2m + chi2() - chi2old: " << dchi2
	   << endl;
#endif
    
    } while(++iter < maxiter && dchi2 > 0.00001);

#ifdef VtDEBUG
    cout << " iterations: " << iter << endl;
#endif
    if(iter >= maxiter) return false;

    VtSymMatrix& cov = *v_covn;

    // v_covn = Valp => v_covn = Valp - gaVga.product(Vgam.T())
    // calc v_covn (in Vt: cov(alpha_c), eq. 38)
    cov -= gaVga.product(Vgam.T());

#ifdef VtDEBUG
    cout << " v_covn: " << cov << endl;
#endif

    // get results
    v_CS.copy(cov);
    // add mass constraint chi2 term to vertex fit term
    v_chi2 += mc_chi2;

    kal_xvs.copy(alphac,0);
    i=3;
    for(iterator it = begin(); it != last; ++it,i+=3) {
      it->kalman.qvs_nc().copy(alphac,i);
    }
    
    return v_Mvalid = true;
  } // end of VtMass()




  //==============================================================================
  // remove_last
  //==============================================================================
  const bool Vertex::remove_last() {
    return VtRemoveTrack(*back());
  }

  //==============================================================================
  // remove_worst
  //==============================================================================
  void Vertex::remove_worst() {
    double prchi2 = 0.;
    double maxchi2 = 0.;
    Relation* worst = 0;

    // a sorted list would be better here
    const iterator last = end();
    for(iterator it=begin(); it!=last; ++it) {
      const Kalman& k = it->kalman;
      double dchi2 = k.chi2() - prchi2;
      prchi2 = k.chi2();

      if(dchi2 > maxchi2){
	maxchi2 = dchi2;
	worst = &(*it);
      }
    }
    
    if(worst != 0) VtRemoveTrack(*worst);
    //    if(worst != 0) remove(worst);
    return;
  }

  //==============================================================================
  // distance: Chi2 distance to space point
  //==============================================================================
  double Vertex::distance(double xx, double yy, double zz) const {
    VtVector dx(xx-vx(),yy-vy(),zz-vz());
    return back()->kalman.CINV().product(dx);
  }


  //==============================================================================
  // distance: Chi2 distance to other vertex
  //==============================================================================
  double Vertex::distance(const Vertex& rhs) const {
    const VtSymMatrix& cov1 = back()->kalman.CINV();
    const VtSymMatrix& cov2 = rhs.back()->kalman.CINV();
    VtSymMatrix Cmat = cov1+cov2;

    if(Cmat.VtDsinv() == false) return 1000.;

    const VtVector x1  = cov1 * kal_xvs;
    const VtVector x2  = cov2 * rhs.kal_xvs;
    const VtVector xav = Cmat * (x1+x2);
    const VtVector dx1 = xav - kal_xvs;
    const VtVector dx2 = xav - rhs.kal_xvs;

    // get a chi2 value
    double sum1 = cov1.product(dx1);
    double sum2 = cov2.product(dx2);
    return (sum1 + sum2);
  }

  //==============================================================================
  // distance: Chi2 distance to track
  //==============================================================================
  double Vertex::distance(Track& t) const {
    Vertex tmp(*this);
    tmp.push_back(t);
    tmp.use_momentum(false);
    tmp.back()->kalman.init();
    tmp.VtFilter();
    return tmp.chi2n();
  }

  //==============================================================================
  // chi2n
  //==============================================================================
  double Vertex::chi2n() const { 
    return back()->kalman.chi2();
  }

  //==============================================================================
  // chi2l
  //==============================================================================
  double Vertex::chi2l() const { 
    const_reverse_iterator it1;
    const_reverse_iterator it2 = it1 = rbegin();
    ++it2;
    return it1->kalman.chi2() - it2->kalman.chi2();
  }

  //==============================================================================
  // print
  //==============================================================================
  std::ostream& Vertex::print(std::ostream& os) const {
    os.setf(ios::right,ios::adjustfield);
    os.setf(ios::fixed,ios::floatfield);
    if(v_mother == true) {
      os << " Mother Track: ";
      Track::print(os);
      os << endl;
    }
//       if (1) return os;
    os << " Vertex:"
       << " vx: "       << setw(6) << setprecision(3) << vx()
       << " vy: "       << setw(6) << setprecision(3) << vy()
       << " vz: "       << setw(7) << setprecision(3) << vz()
//       if (1) return os;
       << " chi2: "     << setw(6) << setprecision(2) << chi2()
       << " prob: "     << setw(6) << setprecision(2) << prob()
       << " angle: "    << setw(5) << setprecision(3) << angle() 
       << " dist: "     << setw(6) << setprecision(4) << dist()
       << " mass: "     << setw(6) << setprecision(4) << mass(v_use_kalman)
       << " no of tracks: " << ntracks() 
       << endl;

    if(xmass.size() > 0) {
      os << "Mass constraints: " << endl;
      const MassC_cit end = xmass.end();
      for(MassC_cit it = xmass.begin(); it != end; ++it) {
	os << "   " << **it << endl;
      }
    }

#ifdef VtDEBUG
    os << " Relations: ";

    RelationList::print(os);
#endif

    os << "Vertex cov. matrix: " << CS();

#ifdef VtDEBUG
    if(v_covn != 0) {
      cout << " bigcov: " << *v_covn << endl;
    }
#endif

    return os;
  }

  //==============================================================================
  // invariant mass functions
  //==============================================================================
  double Vertex::massCC(const bool use) const {
    return mass(vector<double>(ntracks(),0), use, true);
  }

  double Vertex::mass(const bool use) const {
    return mass(vector<double>(ntracks(),0), use, false);
  }
  
  double Vertex::mass(double m1) const {
    return mass(vector<double>(ntracks(),m1*m1));
  }

  double Vertex::mass(double m1, double m2) const {
    if(ntracks() != 2) return -1;
    vector<double> tmp(2);
    tmp[0] = m1*m1, tmp[1] = m2*m2;
    return mass(tmp);
  }

  double Vertex::mass(double m1, double m2, double m3) const {
    if(ntracks() != 3) return -1;
    vector<double> tmp(3);
    tmp[0] = m1*m1, tmp[1] = m2*m2, tmp[2] = m3*m3;
    return mass(tmp);
  }

  double Vertex::mass(double m1, double m2, double m3, double m4) const {
    if(ntracks() != 4) return -1;
    vector<double> tmp(4);
    tmp[0] = m1*m1, tmp[1] = m2*m2, tmp[2] = m3*m3, tmp[4] = m4*m4;
    return mass(tmp);
  }

  //==============================================================================
  // calc invariant mass
  //==============================================================================
  double Vertex::mass(const Vector_d& m02, bool use, bool CC) const {

    double esum  = 0.;
#if defined USE_ROOT
    SVector<double,3> psum;
#else
    VtVector psum(0,0,0);
#endif

    unsigned int i=0;
    const const_iterator last = end();
    for(RelationList::const_iterator it = begin(); it != last; ++it) {
      const Track& t = it->track;
      double p = 0.;
      if(v_use_kalman == false && use == false) {
  	p     = t.p();
	psum += t.pvec();
      } else {
  	const Kalman& k = it->kalman;
  	p     = k.p();
	psum += k.pvec();
      }

      // in case no rest-mass given, use permanent rest-mass
      double mm = m02[i++];

      // handle track rest masses
      if(mm == 0.) {
	double rm = t.rm();
	if(CC == true) rm = t.rmCC(); // use conjugated mass
	mm = rm * rm;
      }
      esum  += sqrt(p*p + mm);
    }
    //   cout << "unllllock mutex" << endl;
    //    pthread_mutex_unlock(&VTmutex);

#if defined USE_ROOT
    double mass2 = esum*esum - mag2(psum);
#else
    double mass2 = esum*esum - psum(0)*psum(0) - psum(1)*psum(1) -
      psum(2)*psum(2);
#endif
    if(mass2 < 0) return 0.;
    else return sqrt(mass2);
  }

  //==============================================================================
  // find 3D vertex of a set of tracks and return the rms distance from the vertex
  //==============================================================================
  const bool Vertex::findVertex3D(void) {

    if(size() < 2) return false;

    double c[3], v[3], tt[9];
    for(int i=0; i<9; i++) tt[i] = 0.;
    for(int i=0; i<3; i++) c[i]  = v[i] = 0.;

    double t[3], r[3], dt;
    const RelationList::iterator last = end();
    for(RelationList::iterator pos = begin(); pos != last; ++pos) {
      const Track& track = pos->track;

      t[0] = track.tx();  r[0] = track.x();
      t[1] = track.ty();  r[1] = track.y();
      t[2] = 1;           r[2] = track.z();
      
      double tsq = sqr(t[0]) + sqr(t[1]) + sqr(t[2]); 
      
      for(int i=0; i<3; i++) {
	for(int j=0; j<3; j++) {
	  dt = t[i]*t[j]/tsq; if(i==j) dt -= 1; 
	  c[i]      += dt*r[j];
	  tt[3*i+j] += dt;
	}
      }
    }
    
    int ifail = 0, ndim = 3;
    double work[3];
    dinv_(&ndim, tt, &ndim, work, &ifail);
    
    if(ifail) {
      cout << "findVertex: matrix inversion failed" << endl;
      return false;
    }
    
    // get the vertex position
    for(int i=0; i<3; i++) {
      for(int j=0; j<3; j++) v[i] += tt[3*i+j]*c[j];
    }


    // fill vertex parameters
    kal_xvs[0]  = v[0];      // x-vertex
    kal_xvs[1]  = v[1];      // y-vertex
    kal_xvs[2]  = v[2];      // z-vertex

    // require that vertex-z is smaller than smallest hit-z of rsegs
    // to avoid vertexing the downstream end of rseg's
    //    for(RelationList::iterator it = begin(); it != last; ++it) {
    //      if(vz() > it->track.z() ) return false;
    //    }

    v_valid = true;
    return true;
  }


  //=============================================================================
  // RMSdist: compute rms distance of tracks to vertex & rms opening angle
  //=============================================================================
  double Vertex::rmsDistAngle(void) const {

    // and the rms distance to the vertex
    v_dist = 0.;
    double tsq, rsq, rt, t[3], r[3];
    double sumtx = 0., sumty = 0.;
    double sumtx2 = 0., sumty2 = 0.;
    const RelationList::const_iterator last = end();
    for(RelationList::const_iterator pos = begin(); pos != last; ++pos) {
      const Track& track = pos->track;

      t[0] = track.tx();  r[0] = track.x() - kal_xvs[0];
      t[1] = track.ty();  r[1] = track.y() - kal_xvs[1];
      t[2] = 1;           r[2] = track.z() - kal_xvs[2];

      sumtx += t[0]; sumtx2 += sqr(t[0]);
      sumty += t[1]; sumty2 += sqr(t[1]);

      tsq = sqr(t[0]) + sqr(t[1]) + sqr(t[2]);
      rsq = sqr(r[0]) + sqr(r[1]) + sqr(r[2]);
      rt  = r[0]*t[0] + r[1]*t[1] + r[2]*t[2];

      v_dist += rsq - sqr(rt)/tsq;
    } 

    unsigned int v_nrseg = size();
    v_angle = sqrt(sumtx2*v_nrseg - sqr(sumtx) + sumty2*v_nrseg -
		   sqr(sumty))/static_cast<double>(v_nrseg); 
    
    v_dist /= static_cast<double>(v_nrseg-1);
    v_dist  = sqrt(v_dist);
    v_angdist = true;
    return v_dist;
  }

  //=============================================================================
  // find 2D vertex of a set of tracks and return the rms distance from the vertex
  //=============================================================================
  const bool Vertex::findVertex2D(void) {  

    unsigned int v_nrseg = size();
    if(v_nrseg < 2) return false;
    
    double sumtx=0, sumty=0, sumrx=0, sumry=0, sumrt=0, sumtt=0;

    double tx, ty, rx, ry;
    const RelationList::iterator last = end();
    for(RelationList::iterator pos = begin(); pos != last; ++pos) {
      const Track& track = pos->track;

      // slopes and position at z=0
      tx = track.tx();  rx = track.x() - tx*track.z();
      ty = track.ty();  ry = track.y() - ty*track.z();
      
      // update cumulants
      sumtx += tx;   sumrx += rx;
      sumty += ty;   sumry += ry;
      sumrt += rx*tx   + ry*ty;
      sumtt += sqr(tx) + sqr(ty);
    }

#ifdef VtDEBUG
    cout << " sumtx: " << sumtx << " sumty: " << sumty
	 << " sumrx: " << sumrx << " sumry: " << sumry
	 << " sumrt: " << sumrt << " sumtt: " << sumtt
	 << endl;
#endif

    // get vertex z-position

    double z = -(v_nrseg*sumrt - sumrx*sumtx - sumry*sumty) / 
                (v_nrseg*sumtt - sqr<double>(sumtx) - sqr<double>(sumty));
  
    // and the transverse coordinates
    double x = (sumrx + z*sumtx)/static_cast<double>(v_nrseg);
    double y = (sumry + z*sumty)/static_cast<double>(v_nrseg);

    // fill vertex parameters
    kal_xvs[0]  = x;         // x-vertex
    kal_xvs[1]  = y;         // y-vertex
    kal_xvs[2]  = z;         // z-vertex

    v_valid = true;
    return true;
  }

  //=============================================================================
  // VtFit: fit the vertex
  //=============================================================================
  const bool Vertex::VtFit() {
    if(VtFilter() == false) return false;
    else {
      VtSmoothX();
      VtSmoothQ();
    }
    return true;
  }

  //=============================================================================
  // findVertexVt: find Vertex with Vt package algorithm
  //=============================================================================
  const bool Vertex::findVertexVt() {  
    v_valid = v_Mvalid = false;
    if(ntracks() < 2) { return false; }
    
    // 1st step: find approximate Vertex from average of all given tracks.
    // Initialize Kalman-filter
    VtEstimateVertex();
    
    // 2nd step: initialize kalman filter
    v_CS   = VtSymMatrix(3,1.e5);
    v_CINV = CINV0;
    const RelationList::iterator last = end();
    for(RelationList::iterator tit = begin(); tit != last; ++tit) {
      tit->kalman.init();
    }

#ifdef VtDEBUG
    cout << "-------------------------------" << endl;
    cout << "Estimated Vertex: (" << vx() << "," << vy() << "," << vz() << ")" << endl;
    cout << "Number of Tracks: " << size();
    copy(begin(), end(), ostream_iterator<Relation>(cout," "));
    cout << endl;
#endif

    double prChi2 = 0.;
    const Kalman& trackn = back()->kalman; // last element

    // 3rd step: iterate the kalman filter
    unsigned int iter = 0;
    do {

      if(VtFit() == false) return false;

      double chi2 = trackn.chi2();

#ifdef VtDEBUG
      cout << "----> chi2n: " << chi2 << " prChi2: " << prChi2 << " fabs(chi2-prChi2): " 
	   << fabs(chi2 - prChi2) << endl;
#endif

      if(fabs(chi2 - prChi2) > 1.) {
	prChi2 = chi2;
      } else
	if(iter > 0)
	  break;
    } while(++iter < 20);

#ifdef VtDEBUG
    cout << "-------------------------------" << endl;
    cout << "No of iterations: " << iter << endl;
    cout << *this;
#endif

    if(iter<20) { //  && chi2()<10.
      v_valid = true;
    } else {
      //      cout << "-----> VERTEX FIT FAILED!! <--------" << endl;
    }
    return v_valid;
  }

  //=============================================================================
  // VtEstimateVertex (former VTAVER function)
  //=============================================================================
  const bool Vertex::VtEstimateVertex() {
    return VtEstimateVertexMath(kal_xvs[0], kal_xvs[1], kal_xvs[2]);
  }

  //=============================================================================
  // VtEstimateVertex (former VTAVER function)
  //=============================================================================
  const bool Vertex::VtEstimateVertexMath(double& x, double& y, double& z) {

    double tx_sum  = 0.;
    double x_sum   = 0.;
    double xw_sum  = 0.;
    double xtx_sum = 0.;
    double tx2_sum = 0.;
    double ty_sum  = 0.;
    double y_sum   = 0.;
    double yw_sum  = 0.;
    double yty_sum = 0.;
    double ty2_sum = 0.;

    // fill cumulants
    const RelationList::iterator last = end();
    for(RelationList::iterator tit = begin(); tit != last; ++tit) {
      const Track& track = tit->track;

      double x        = track.x();
      double y        = track.y();
      double tx       = track.tx();
      double ty       = track.ty();
      double xweight  = 1./track.V().x();
      double yweight  = 1./track.V().y();
      double xweight2 = xweight*xweight;
      double yweight2 = yweight*yweight;

      tx_sum  += tx * xweight;
      x_sum   += x  * xweight;
      xw_sum  += xweight;
      xtx_sum += x  * tx * xweight2;
      tx2_sum += tx * tx * xweight2;

      ty_sum  += ty * yweight;
      y_sum   += y  * yweight;
      yw_sum  += yweight;
      yty_sum += y  * ty * yweight2;
      ty2_sum += ty * ty * yweight2;

    } // for track

    double det = -tx2_sum - ty2_sum + tx_sum*tx_sum/xw_sum + ty_sum*ty_sum/yw_sum;

#ifdef VtDEBUG    
    cout << " txsum: " << tx_sum << " xsum: " << x_sum << " xwsum: " << xw_sum
	 << " xtxsum: " << xtx_sum << " tx2sum: " << tx2_sum << endl;
    cout << " tysum: " << ty_sum << " ysum: " << y_sum << " ywsum: " << yw_sum
	 << " ytysum: " << yty_sum << " ty2sum: " << ty2_sum << endl;
    cout << " det: " << det << endl;
#endif

    if(det == 0.) {
#ifdef VtDEBUG
      cout << "VtVertex::VtEstimateVertex error: determinant == 0.!" << endl;
#endif
      return false;
    }

//      kal_xvs[2] = ( xtx_sum + yty_sum - tx_sum*x_sum/xw_sum - ty_sum*y_sum/yw_sum ) / det;
//      kal_xvs[0] = ( x_sum + tx_sum * vz() ) / xw_sum;
//      kal_xvs[1] = ( y_sum + ty_sum * vz() ) / yw_sum;

    z = ( xtx_sum + yty_sum - tx_sum*x_sum/xw_sum - ty_sum*y_sum/yw_sum ) / det;
    x = ( x_sum + tx_sum * z ) / xw_sum;
    y = ( y_sum + ty_sum * z ) / yw_sum;

    v_valid = true;
    return true;
  }

  //=============================================================================
  // VtFilter (in Vt: vtfilt) 
  //=============================================================================
  const bool Vertex::VtFilter() {
    
    // initialize by vertex position & cov. matrix
    //    const VtVector*    prkal_xv = &kal_xv0;
    const VtVector*    prkal_xv = &kal_xvs;
    const VtSymMatrix* prCINV   = &v_CINV;
    double             prChi2   = 0.;
    v_bk13 = vz();

    // cout << "bk13: " << v_bk13 << endl;

    const RelationList::iterator last = end();
    for(RelationList::iterator tit = begin(); tit != last; ++tit) {
      Kalman& kalman = tit->kalman;

      //      cout << "----> FILTER TRACK " << track;
#ifdef VtDEBUG
      cout << endl;
#endif

      // compute contributions of track k
      if(kalman.filter(v_bk13, *prCINV, *prkal_xv) == false) {
	return false; // forget this vertex
      }

      kalman.filter_chi2(v_bk13, prChi2, *prCINV, *prkal_xv);
    
      //      cout << "Track-Kalman chi2: " << kalman.chi2() << endl;

      // store pointers to previous (k-1) values
      prkal_xv = &kalman.xv();
      prCINV   = &kalman.CINV();
      prChi2   = kalman.chi2();
    } // for tracks

    return true;
  }

  //=============================================================================
  // VtInverseFilter (in Vt: vtdis): get smoothed residuals for tracks
  //=============================================================================
  const bool Vertex::VtInverseFilter() const {
    
    // get last track (in Vt: x_n)
    const Kalman& trackn = back()->kalman;

    const VtSymMatrix* prCINV = &v_CINV;

    const RelationList::const_iterator last = end();
    for(RelationList::const_iterator tit = begin(); tit != last; ++tit) {
      Kalman& kalman = tit->kalman;
      
      if(kalman.inverse_filter(v_bk13, trackn.CINV(), *prCINV,
			       trackn.xv()) == false) {
	return false;
      }
      prCINV   = &kalman.CINV();
    }

    return true;
  }

  //=============================================================================
  // VtInverseFilter (in Vt: vtkill): remove track from vertex
  //=============================================================================
  const bool Vertex::VtRemoveTrack(Relation& trackk) {
    if(ntracks() <= 2 || valid() == false) return false; // exclude weird cases

    if(VtInverseFilter() == false) return false;

    // get last track (in Vt: x_n)
    const Kalman& trackn = back()->kalman;

    // compute chi2_n -> chi2_n - chi2_k,S (eq. 22)
    double dchi2 = trackn.chi2() - trackk.kalman.chi2s();
    
    // new smoothed vertex position
    kal_xvs = trackk.kalman.xnk();

    const RelationList::iterator last = end();
    for(RelationList::iterator tit = begin(); tit != last; ++tit) {
      // calculate qvs
      tit->kalman.calc_qvs(kal_xvs);
    }

    remove(&trackk);

    v_chi2 = back()->kalman.set_chi2(dchi2);

    // refit the vertex
    return v_valid = VtFit();
  }

  //=============================================================================
  // VtSmoothX (in Vt: vtsmo)  smooth fit parameters, update xvs
  //=============================================================================
  void Vertex::VtSmoothX() {
    // get last track (in Vt: x_n)
    const Kalman& kaln = back()->kalman;
    
#ifdef VtDEBUG
    cout << "--> VtSmooth called!" << endl;
#endif

    // update vertex position saved in xvs!!
    kal_xvs = kaln.xv(); 

#ifdef VtDEBUG
    cout << "kal_xvs: "    << kal_xvs  << endl;
    cout << "track->C(): " << kaln.C() << endl;
#endif

    // save smoothed covariance matrix of vertex
    v_CS = kaln.C();

    // save chi2
    v_chi2 = kaln.chi2();

#ifdef VtDEBUG
    cout << "v_CS: " << v_CS << endl;
#endif

    return;
  }

  //=============================================================================
  // VtSmooth (in Vt: vtsmo)  smooth track parameters, update qvs
  //=============================================================================
  void Vertex::VtSmoothQ() {
   
#ifdef VtDEBUG
    cout << "---> VtSmoothQ called! <---" << endl;
#endif

    const Kalman& trackn = back()->kalman;
    const RelationList::iterator last = end();
    for(RelationList::iterator tit = begin(); tit != last; ++tit) {
      tit->kalman.smooth(v_bk13, kal_xvs, trackn.C());
    }
    
    return;
  }


  //=============================================================================
  // operator =
  //=============================================================================
  const Vertex& Vertex::operator=(const Vertex& rhs) {
    if( this == &rhs ) { return *this; }
    
    Track::operator=(rhs);

    v_use_kalman   = rhs.v_use_kalman;
    v_valid        = rhs.v_valid;
    v_Mvalid       = rhs.v_Mvalid;
    v_mother       = rhs.v_mother;
    v_angdist      = rhs.v_angdist;
    v_angle        = rhs.v_angle;
    v_dist         = rhs.v_dist;
    v_bk13         = rhs.v_bk13;
    v_chi2         = rhs.v_chi2;

    kal_xv         = rhs.kal_xv;
    kal_xvs        = rhs.kal_xvs;
    v_CS           = rhs.v_CS;

    // possible if tracks & xmass contain SmartPointers to the objects
    //      tracks         = rhs.tracks;
    //      xmass          = rhs.xmass;

    return *this;
  }


  //=============================================================================
  // operator ==
  //=============================================================================
  const bool Vertex::operator==(const Vertex& rhs) const {
    return RelationList::operator==(rhs);
  }


  //=============================================================================
  // simple distance between vertices
  //=============================================================================
  const double Vertex::operator-(const Vertex& rhs) const {
    return sqrt(sqr(vx()-rhs.vx()) + sqr(vy()-rhs.vy()) + sqr(vz()-rhs.vz()));
  }

  //=============================================================================
  // maximum momentum fraction of tracks
  //=============================================================================
  double Vertex::pmaxfrac() const {
    double maxfrac = 0.;

    const RelationList::const_iterator last = end();
    for(RelationList::const_iterator tit = begin(); tit != last; ++tit) {
      double p1 = fabs(tit->track.p());

      RelationList::const_iterator it=tit;
      for(++it; it != last; ++it) {
	double p2 = fabs(it->track.p());
	double frac = max(p1/p2, p2/p1);
	if( frac > maxfrac ) maxfrac = frac;
      }
    }

    return maxfrac;
  }
} // End of namespace
