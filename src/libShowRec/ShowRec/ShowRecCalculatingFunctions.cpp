////////////////////////////////////////////////////////////////////////////

EdbVertex* CalcVertex(TObjArray *segments) {

    // Exactly same implementation as in EdbEDAUtil.C
    // but I do not want to link in hardcoded all eda libraries.
    // calc vertex point with given segments. just topological calculation.
    // VTA is currently not set.
    // in case of Single-stop. make a vertex 650 micron upstream ob base.
    double xx,yy,zz,Det,Ax,Ay,Az,Bx,By,Bz,Cx,Cy,Cz,Dx,Dy,Dz;
    Ax=0.;    Ay=0.;    Az=0.;
    Bx=0.;    By=0.;    Bz=0.;
    Cx=0.;    Cy=0.;    Cz=0.;
    Dx=0.;    Dy=0.;    Dz=0.;

    if (segments->GetEntries()==1) {
        // in case of Single-stop. make a vertex 650 micron upstream ob base.
        EdbSegP *s = new EdbSegP(*((EdbSegP *) segments->At(0)));
        s->PropagateTo(s->Z()-650);
        xx=s->X();
        yy=s->Y();
        zz=s->Z();
        delete s;
    }
    else {
        for (int i=0; i<segments->GetEntries(); i++) {
            EdbSegP *s = (EdbSegP *) segments->At(i);
            double ax = s->TX();
            double ay = s->TY();
            double az = 1.0;
            double x  = s->X();
            double y  = s->Y();
            double z  = s->Z();
            double a = ax*ax+ay*ay+az*az;
            double c = -ax*x-ay*y-az*z;
            double b = (ax*ax+ay*ay);
            //  double w = 1.0/a/a; // weight for small angle tracks.
            double w = 1.0; // no weight

            Ax+=2.0*w/b*( a*(ay*ay+az*az) );
            Bx+=2.0*w/b*( -a*ax*ay );
            Cx+=2.0*w/b*( -a*ax*az );
            Dx+=2.0*w/b*( -(a*x+c*ax)*(ax*ax-a)-(a*y+c*ay)*ax*ay-(a*z+c*az)*az*ax );

            Ay+=2.0*w/b*( -a*ay*ax );
            By+=2.0*w/b*( a*(az*az+ax*ax) );
            Cy+=2.0*w/b*( -a*ay*az );
            Dy+=2.0*w/b*( -(a*y+c*ay)*(ay*ay-a)-(a*z+c*az)*ay*az-(a*x+c*ax)*ax*ay );

            Az+=2.0*w/b*( -a*az*ax );
            Bz+=2.0*w/b*( -a*az*ay );
            Cz+=2.0*w/b*( a*b );
            Dz+=2.0*w/b*( -(a*z+c*az)*(az*az-a)-(a*x+c*ax)*az*ax-(a*y+c*ay)*ay*az );

        }

        Det=fabs( Ax*(By*Cz-Cy*Bz)-Bx*(Ay*Cz-Cy*Az)+Cx*(Ay*Bz-By*Az) );
        xx=( (By*Cz-Cy*Bz)*Dx-(Bx*Cz-Cx*Bz)*Dy+(Bx*Cy-Cx*By)*Dz)/Det;
        yy=(-(Ay*Cz-Cy*Az)*Dx+(Ax*Cz-Cx*Az)*Dy-(Ax*Cy-Cx*Ay)*Dz)/Det;
        zz=( (Ay*Bz-By*Az)*Dx-(Ax*Bz-Bx*Az)*Dy+(Ax*By-Bx*Ay)*Dz)/Det;
    }

    EdbVertex *v = new EdbVertex();
    v->SetXYZ(xx,yy,zz);

    return v;
}

//______________________________________________________________________________

Bool_t IsSameSegment(EdbSegP* seg1,EdbSegP* seg2) {
    // This function checks on absolute values rather
    // on adress of segments.
    // This might be safer when dealing with copies
    // of SegP objects in memory.
    if (TMath::Abs(seg1->X()-seg2->X())<0.01) {
        if (TMath::Abs(seg1->Y()-seg2->Y())<0.01) {
            if (TMath::Abs(seg1->TY()-seg2->TY())<0.01) {
                if (TMath::Abs(seg1->TX()-seg2->TX())<0.01) {
                    return kTRUE;
                }
            }
        }
    }
    return kFALSE;
}

//______________________________________________________________________________

Double_t CalcIP(EdbSegP* s, EdbVertex* v) {
    // calculate IP for the selected tracks wrt the given vertex.
    // if the vertex is not given, use the selected vertex.
    if (NULL==v) {
        printf("select a vertex!\n");
        return 0;
    }
    if (gEDBDEBUGLEVEL>3) printf(" /// Calc IP w.r.t. Vertex %d %8.1lf %8.1lf %8.1lf (red vertex) ///\n",v->ID(), v->X(),v->Y(),v->Z() );
    double r = CalcIP(s,v->X(),v->Y(),v->Z());
    return r;
}

//______________________________________________________________________________

Double_t CalcIP(EdbSegP *s, double x, double y, double z) {
    // Calculate IP between a given segment and a given x,y,z.
    // return the IP value.
    double ax = s->TX();
    double ay = s->TY();
    double bx = s->X()-ax*s->Z();
    double by = s->Y()-ay*s->Z();
    double a;
    double r;
    double xx,yy,zz;
    a = (ax*(x-bx)+ay*(y-by)+1.*(z-0.))/(ax*ax+ay*ay+1.);
    xx = bx +ax*a;
    yy = by +ay*a;
    zz = 0. +1.*a;
    r = sqrt((xx-x)*(xx-x)+(yy-y)*(yy-y)+(zz-z)*(zz-z));
    return r;
}

//______________________________________________________________________________

void  FillShowerAxis() {
    cout << "IMPLEMENTED IN ... TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree ... CHECK THERE..."<<endl;
    return;
}


//______________________________________________________________________________

Double_t GetMinimumDist(EdbSegP* seg1,EdbSegP* seg2) {
    // calculate minimum distance of 2 lines.
    // use the data of (the selected object)->X(), Y(), Z(), TX(), TY().
    // means, if the selected object == segment, use the data of the segment. or it == track, the use the fitted data.
    // original code from Tomoko Ariga
    // double calc_dmin(EdbSegP *seg1, EdbSegP *seg2, double *dminz = NULL){

    double x1,y1,z1,ax1,ay1;
    double x2,y2,z2,ax2,ay2;
    double s1,s2,s1bunsi,s1bunbo,s2bunsi,s2bunbo;
    double p1x,p1y,p1z,p2x,p2y,p2z,p1p2;

    if (seg1->ID()==seg2->ID()&&seg1->PID()==seg2->PID()) return 0.0;

    x1 = seg1->X();
    y1 = seg1->Y();
    z1 = seg1->Z();
    ax1= seg1->TX();
    ay1= seg1->TY();

    x2 = seg2->X();
    y2 = seg2->Y();
    z2 = seg2->Z();
    ax2= seg2->TX();
    ay2= seg2->TY();

    s1bunsi=(ax2*ax2+ay2*ay2+1)*(ax1*(x2-x1)+ay1*(y2-y1)+z2-z1) - (ax1*ax2+ay1*ay2+1)*(ax2*(x2-x1)+ay2*(y2-y1)+z2-z1);
    s1bunbo=(ax1*ax1+ay1*ay1+1)*(ax2*ax2+ay2*ay2+1) - (ax1*ax2+ay1*ay2+1)*(ax1*ax2+ay1*ay2+1);
    s2bunsi=(ax1*ax2+ay1*ay2+1)*(ax1*(x2-x1)+ay1*(y2-y1)+z2-z1) - (ax1*ax1+ay1*ay1+1)*(ax2*(x2-x1)+ay2*(y2-y1)+z2-z1);
    s2bunbo=(ax1*ax1+ay1*ay1+1)*(ax2*ax2+ay2*ay2+1) - (ax1*ax2+ay1*ay2+1)*(ax1*ax2+ay1*ay2+1);
    s1=s1bunsi/s1bunbo;
    s2=s2bunsi/s2bunbo;
    p1x=x1+s1*ax1;
    p1y=y1+s1*ay1;
    p1z=z1+s1*1;
    p2x=x2+s2*ax2;
    p2y=y2+s2*ay2;
    p2z=z2+s2*1;
    p1p2=sqrt( (p1x-p2x)*(p1x-p2x)+(p1y-p2y)*(p1y-p2y)+(p1z-p2z)*(p1z-p2z) );

    return p1p2;
}
