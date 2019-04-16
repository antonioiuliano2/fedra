import fedrarootlogon
import ROOT

import sys #for the argument with the filename

#Example of a script to access FEDRA classes with PyROOT.
#Opening a tracks root file, building EdbTrackP objects and getting information
#usage: python -i filename 

verbose = False #put to True for the printout

#using fedra methods to build the EdbTrackP list from the file

dproc = ROOT.EdbDataProc()
gAli = dproc.PVR()

def buildtracks(filename):

 dproc.ReadTracksTree(gAli,filename,"nseg>1")
 gAli.FillCell(30,30,0.009,0.009)
 tracks = gAli.eTracks

 return tracks

#start of the main loop script

inputfilename = sys.argv[1]
#inputfilename = "/ship/CHARM2018/CH1-R6/b000001/b000001.0.0.0.trk.root"
tracks = buildtracks(inputfilename)

hxyz = ROOT.TH3F("hxyz","Positions of track segments",125,0,125,100,0,100,80,-80,0)
hnseg = ROOT.TH1I("hnseg","Number of segments per track",57,0,57)

microntomm = 1E-3

#accessing the list of tracks
for track in tracks:

 
 firstsegment = track.GetSegmentFirst()
 nseg = track.N()

 if verbose: print "My tracks has",nseg,"segments"
 hnseg.Fill(nseg)
 if verbose: print "My track starts at segment", track.GetSegmentFirst().Z()

 #start loop on segment indices
 for i in range(track.N()):
  #accessing single segment
  seg = track.GetSegment(i) 
  
  x = seg.X() *microntomm
  y = seg.Y() *microntomm
  z = seg.Z() *microntomm
  if verbose: print 'Position of %d segment: (%.3f mm,%.3f mm)'%(i,x,y)
  hxyz.Fill(x,y,z)
  
#drawing the results
c0 = ROOT.TCanvas()
hnseg.Draw()
hnseg.SetTitle("nseg")
c1 = ROOT.TCanvas()
hxyz.Draw()
hxyz.GetXaxis().SetTitle("x[mm]")
hxyz.GetYaxis().SetTitle("y[mm]")
hxyz.GetZaxis().SetTitle("z[mm]")
