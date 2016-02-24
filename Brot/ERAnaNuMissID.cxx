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
  {}

  bool ERAnaNuMissID::Analyze(const EventData &data, const ParticleGraph &ps)
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
  
  bool ERAnaNuMissID::MCChecker(Particle& ParticleToCheck, const EventData& MCData, const ParticleGraph& MCGraph, const int PDGCode)
  {
    RecoID_t ChildRecoID;
    RecoType_t ChildRecoType;
    
    for(const auto & Child : ParticleToCheck.Children())
    {
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
	if(MCGraph.GetParticle(MCParticle.Ancestor().PdgCode()) == PDGCode)
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
