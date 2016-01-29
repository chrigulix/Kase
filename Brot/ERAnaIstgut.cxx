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
    
    auto DetGeometry = ::larutil::Geometry::GetME();
    
    DetectorBox = geoalgo::AABox(0.0,-DetGeometry->DetHalfHeight(),0.0,2*DetGeometry->DetHalfWidth(),DetGeometry->DetHalfHeight(),DetGeometry->DetLength());
  }

  bool ERAnaIstgut::Analyze(const EventData &data, const ParticleGraph &ps)
  {
    // Get the MC graph
    auto const& mc_graph = MCParticleGraph();
    // Get the MC data
    auto const& mc_data = MCEventData();
    
    
    
    for(auto const & particle :  mc_graph.GetParticleArray())
    {
      auto const & Track = mc_data.Track(particle.RecoID());
      std::cout << particle.PdgCode() << std::endl;
//       std::cout << "hello" << std::endl;
      
      if(particle.PdgCode() == 12)
      {
	// Check if the particle is originating in the TPC
// 	if(DetectorBox.Contain(Track.front()))
// 	{
// 	  std::cout << "Is inside" << std::endl;
// 	}
	
      }
    }
//     for(auto const & particle : ps.GetPrimaryNodes(RecoType_t::kInvisible))
//     {
//       auto const & neutrals = data.Track(ps.GetParticle(particle).RecoID());
      
//       std::cout << ps.GetParticle(particle).PdgCode() << std::endl;
//     }
    return true; 
  }

  void ERAnaIstgut::ProcessEnd(TFile* fout)
  {
    
  }

}

#endif
