#ifndef ERTOOL_ERANAISTGUT_CXX
#define ERTOOL_ERANAISTGUT_CXX

#include "ERAnaIstgut.h"

namespace ertool {

  ERAnaIstgut::ERAnaIstgut(const std::string& name) : AnaBase(name)
  {
    
  }

  void ERAnaIstgut::Reset()
  {
    
  }

  void ERAnaIstgut::AcceptPSet(const ::fcllite::PSet& cfg)
  {
    
  }

  void ERAnaIstgut::ProcessBegin()
  {
    _inTPC = 0;
    _outTPC =0;
  }

  bool ERAnaIstgut::Analyze(const EventData &data, const ParticleGraph &ps)
  {
    std::cout << "starting loop: " << ps.GetNumParticles() << std::endl;
    std::cout << "array: "  << std::endl;
        
    // Get MC particle set
    auto const& mc_graph = MCParticleGraph();
    // Get the MC data
    auto const& mc_data = MCEventData();
    
    for(auto const & particle : mc_graph.GetParticleArray())
    {
        
        std::cout << "out1 " << particle.PdgCode() <<  " ";

        
        if ( particle.RecoType() == RecoType_t::kShower )
        {
            auto const shower = mc_data.Shower(particle);
            std::cout << "Shower " << shower.Start() << std::endl;
        }
        
        if (particle.RecoType() == RecoType_t::kTrack)
        {
            auto const track = mc_data.Track(particle);
            std::cout << "Track " << track.front() << std::endl;
        }
        
    }
    return true; 
  }

  void ERAnaIstgut::ProcessEnd(TFile* fout)
  {
   
  }

}

#endif
