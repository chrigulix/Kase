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
	bool IsNeutrino = MCChecker(particle,data,graph,mc_data,mc_graph,12);
	
// 	std::cout << "MC GRAPH ------------------------------------------------------------" << std::endl;
// 	std::cout << mc_graph.Diagram() << std::endl;
// 	std::cout << "GRAPH ------------------------------------------------------------" << std::endl;
// 	std::cout << graph.Diagram() << std::endl;
	std::cout << "Neutrino: " << particle.ID() << " " << IsNeutrino << std::endl;
// 	exit(1);
      }
    }
    
    for (auto const & mc_particle : mc_graph.GetParticleArray()) 
    {
      if(mc_particle.PdgCode() == 12)
      {
	bool IsNeutrino = MCChecker(mc_particle,data,graph,mc_data,mc_graph,12);
	
// 	std::cout << "MC GRAPH ------------------------------------------------------------" << std::endl;
// 	std::cout << mc_graph.Diagram() << std::endl;
// 	std::cout << "GRAPH ------------------------------------------------------------" << std::endl;
// 	std::cout << graph.Diagram() << std::endl;
	std::cout << "Neutrino: " << particle.ID() << " " << IsNeutrino << std::endl;
// 	exit(1);
      }
    }
    
    return true;
  }

  void ERAnaNuMissID::ProcessEnd(TFile* fout)
  {}
  
  bool ERAnaNuMissID::MCChecker(const Particle& ParticleToCheck, const EventData& Data, const ParticleGraph& Graph, const EventData& MCData, const ParticleGraph& MCGraph, const int PDGCode)
  {
    // Initialize the reco ID of the child particle
    RecoID_t ChildRecoID;
    
    for(auto const & ChildNodeID : ParticleToCheck.Children())
    {
      Particle Child = Graph.GetParticle(ChildNodeID);
      if(Child.RecoType() ==  RecoType_t::kTrack || Child.RecoType() ==  RecoType_t::kShower)
      {
	ChildRecoID = Child.RecoID();
	break;
      }
    }
    
    for(auto const& MCParticle : MCGraph.GetParticleArray())
    {
      if(MCParticle.RecoID() == ChildRecoID)
      {
	if(MCGraph.GetParticle(MCParticle.Ancestor()).PdgCode() == PDGCode)
	{
	  std::cout << "PDG code if not missID " << MCGraph.GetParticle(MCParticle.Ancestor()).PdgCode() << std::endl;
	  return true;
	}
	else
	{
	  std::cout << "PDG code if missID MCPrimary: " << MCGraph.GetParticle(MCParticle.Ancestor()).PdgCode() << " PDG code of child " << MCParticle.PdgCode() << std::endl;
	  return false;
	}
      }
    } // loop over mc graph particles
    
    // The code shouldn't run to this point. Either there is a neutrion in the event or not!
    throw ERException(Form("There is something wrong, go and strangle Christoph and Matthias!"));
  } // MCChecker
  
}

#endif
