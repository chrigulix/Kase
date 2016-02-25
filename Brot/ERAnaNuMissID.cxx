#ifndef ERTOOL_ERANANUMISSID_CXX
#define ERTOOL_ERANANUMISSID_CXX

#include "ERAnaNuMissID.h"

namespace ertool {

  ERAnaNuMissID::ERAnaNuMissID(const std::string& name) : AnaBase(name)
  {}

  void ERAnaNuMissID::Reset()
  {}

  void ERAnaNuMissID::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaNuMissID::ProcessBegin()
  {
    _inTPC = 0;
    _outTPC = 0;
    
    auto DetGeometry = ::larutil::Geometry::GetME(); 
    
    DetectorBox = geoalgo::AABox(0.0,-DetGeometry->DetHalfHeight(),0.0,
                                 2*DetGeometry->DetHalfWidth(),DetGeometry->DetHalfHeight(),DetGeometry->DetLength());
    
    // Distance of Cryostat wall to TPC on the cylinder axis
    double D = (DetGeometry->CryostatLength() - DetGeometry->DetLength())/2.0;
    
    Cryostat = geoalgo::Cylinder(DetGeometry->DetHalfWidth(), 0.0, -D,
                                 DetGeometry->DetHalfWidth(), 0.0, DetGeometry->CryostatLength() - D,
                                 DetGeometry->CryostatHalfHeight());
  }

  bool ERAnaNuMissID::Analyze(const EventData &data, const ParticleGraph &graph)
  {   
     // Get MC particle set
    auto const& mc_graph = MCParticleGraph();
    // Get the MC data
    auto const& mc_data = MCEventData();
    
    // Check if we found a neutrino during reconstruction
//     std::cout << "---------------- Event: " << data.Event_ID() << " ----------------" << std::endl;
    for (auto const & particle : graph.GetParticleArray()) 
    {
      // only the ancestor (base particle) has the tpc - flash matched information
      auto Ancestor = graph.GetParticle(particle.Ancestor());
      
      if (particle.PdgCode() == 12 && Ancestor.ProcessType() != kCosmic) 
      {
// 	RecoID_t MCParentRecoID = GetMCRecoID(particle,graph,mc_graph,12);
	
	RecoID_t ChildRecoID = std::numeric_limits<RecoID_t>::max();
	
	for(auto const & ChildNodeID : particle.Children())
	{
	  Particle Child = graph.GetParticle(ChildNodeID);
	  if( (/*Child.RecoType() ==  RecoType_t::kTrack ||*/ Child.RecoType() ==  RecoType_t::kShower)
	      && (abs(Child.PdgCode()) == 11 || abs(Child.PdgCode()) == 22) )
	  {
	    ChildRecoID = Child.RecoID();
	    break;
	  }
	}
	
	// TODO needs to be changed
	if(ChildRecoID == std::numeric_limits<RecoID_t>::max())
	{
//       return ChildRecoID;
	}
	
	bool FoundBITE = false;
	
	for(auto const& MCParticle : mc_graph.GetParticleArray())
	{
	  if(MCParticle.RecoID() == ChildRecoID)
	  {
	    Particle MCAncestor = mc_graph.GetParticle(MCParticle.Ancestor());
	    if( MCAncestor.RecoID() == std::numeric_limits<RecoID_t>::max()
	        && !DetectorBox.Contain(MCAncestor.Vertex()) && Cryostat.Contain(MCAncestor.Vertex())
	        && (mc_data.Shower(MCParticle)._time < 0 || mc_data.Shower(MCParticle)._time > 1600) )
	    {
	      BITEandCosmicEnergy.push_back(data.Shower(ChildRecoID)._energy);
	      std::cout << BITEandCosmicEnergy.back() << " " << mc_data.Shower(ChildRecoID)._energy << FoundBITE << std::endl;
	      FoundBITE = true;
	    }
	    else if( (MCAncestor.PdgCode() == 12 || MCAncestor.PdgCode() == 14 || MCAncestor.PdgCode() == 16)
		   && !DetectorBox.Contain(MCAncestor.Vertex()) && Cryostat.Contain(MCAncestor.Vertex())
		   && (mc_data.Shower(MCParticle)._time >= 0 || mc_data.Shower(MCParticle)._time <= 1600) )
	    {
	      BITEandDaugterEnergy.push_back(data.Shower(ChildRecoID)._energy);
	      std::cout << BITEandDaugterEnergy.back() << " " << FoundBITE << std::endl;
	      FoundBITE = true;
	    }
	    else if(MCAncestor.PdgCode() == 12 && DetectorBox.Contain(MCAncestor.Vertex()))
	    {
	      SignalEnergy.push_back(data.Shower(ChildRecoID)._energy);
	      std::cout << "We have a signal! " << SignalEnergy.back() << std::endl;
	    }
	  }
	} // loop over mc graph particles
	
	std::cout << "Found BITE? " << FoundBITE << std::endl;
	
	if(!FoundBITE)
	{
	  for(auto const& MCParticle : mc_graph.GetParticleArray())
	  {
	    if( (MCParticle.PdgCode() == 12 || MCParticle.PdgCode() == 14 || MCParticle.PdgCode() == 16) 
	      && DetectorBox.Contain(MCParticle.Vertex())
	      && (mc_data.Shower(ChildRecoID)._time < 0 || mc_data.Shower(ChildRecoID)._time > 1600) )
	    {
	      NuandCosmicEnergy.push_back(data.Shower(ChildRecoID)._energy);
	      std::cout << NuandCosmicEnergy.back() << std::endl;
	    }
	  }
	}
	
	
// 	std::cout << "MC GRAPH ------------------------------------------------------------" << std::endl;
// 	std::cout << mc_graph.Diagram() << std::endl;
// 	std::cout << "GRAPH ------------------------------------------------------------" << std::endl;
// 	std::cout << graph.Diagram() << std::endl;
// 	std::cout << "Found Nu: " << particle.ID() << " " << MCParentRecoID << std::endl;
// 	exit(1);
      }
    }
    
    for (auto const & mc_particle : mc_graph.GetParticleArray()) 
    {
      if(mc_particle.PdgCode() == 12)
      {
// 	bool IsNeutrino = MCChecker(mc_particle,mc_data,mc_graph,data,graph,12);
	
// 	std::cout << "MC GRAPH ------------------------------------------------------------" << std::endl;
// 	std::cout << mc_graph.Diagram() << std::endl;
// 	std::cout << "GRAPH ------------------------------------------------------------" << std::endl;
// 	std::cout << graph.Diagram() << std::endl;
// 	std::cout << "Not Found Nu: " << mc_particle.ID() << " " << IsNeutrino << std::endl;
// 	exit(1);
      }
    }
    
    return true;
  }

  void ERAnaNuMissID::ProcessEnd(TFile* fout)
  {
    for(auto const& energy : BITEandCosmicEnergy)
    {
      BITECosmics->Fill(energy);
    }
    for(auto const& energy : BITEandDaugterEnergy)
    {
      BITEDaugter->Fill(energy);
    }
    for(auto const& energy : NuandCosmicEnergy)
    {
      NuAndCosmic->Fill(energy);
    }
    BITECosmics->SetFillColor(kRed);
    BITEDaugter->SetFillColor(kBlue);
    NuAndCosmic->SetFillColor(kGreen);
    
    hs->Add(BITECosmics);
    hs->Add(BITEDaugter);
    hs->Add(NuAndCosmic);
    
    fout = new TFile("Bite.root","RECREATE");
    fout->cd();
    hs->Write();
  }
  
  RecoID_t ERAnaNuMissID::GetMCRecoID(const Particle& ParticleToCheck, const ParticleGraph& Graph, const ParticleGraph& MCGraph, const int PDGCode)
  {
    // Initialize the reco ID of the child particle
    RecoID_t ChildRecoID = std::numeric_limits<RecoID_t>::max();
    
    for(auto const & ChildNodeID : ParticleToCheck.Children())
    {
      Particle Child = Graph.GetParticle(ChildNodeID);
      if(Child.RecoType() ==  RecoType_t::kTrack || Child.RecoType() ==  RecoType_t::kShower)
      {
	ChildRecoID = Child.RecoID();
	break;
      }
    }
    // TODO needs to be changed
    if(ChildRecoID == std::numeric_limits<RecoID_t>::max())
    {
      return ChildRecoID;
    }
    
    for(auto const& MCParticle : MCGraph.GetParticleArray())
    {
//       std::cout << "RecoIDs: " << MCParticle.RecoID() << " " << ChildRecoID << std::endl;
      if(MCParticle.RecoID() == ChildRecoID)
      {
	return MCGraph.GetParticle(MCParticle.Ancestor()).RecoID();
      }
    } // loop over mc graph particles
    
    // The code shouldn't run to this point. Either there is a neutrion in the event or not!
    throw ERException(Form("There is something wrong, go and strangle Christoph and Matthias!"));
  } // MCChecker
  
}

#endif
