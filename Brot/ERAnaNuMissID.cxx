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
    // Check if we found a neutrino during reconstruction
		std::cout << "---------------- Event: " << data.Event_ID() << " ----------------" << std::endl;
		for (auto const & particle : graph.GetParticleArray()) {
			if (particle.RecoType() != RecoType_t::kTrack) {
				if (particle.PdgCode() == 12) {
					std::cout << "Neurino: " << particle.ID() << std::endl;
				}
				
			}
			
		}
    
    return true; }

  void ERAnaNuMissID::ProcessEnd(TFile* fout)
  {}
  
  bool ERAnaNuMissID::MCChecker(Particle& ParticleToCheck, const EventData& Data, const ParticleGraph& Graph, const EventData& MCData, const ParticleGraph& MCGraph, const int PDGCode)
  {
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
	  return true;
	}
	else
	{
	  return false;
	}
      }
    }
    throw ERException(Form("There is something wrong, go and strangle Christoph and Matthias!"));
  }
  
}

#endif
