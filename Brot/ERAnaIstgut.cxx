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
    _outTPC = 0;
    
    auto DetGeometry = ::larutil::Geometry::GetME(); 
    DetectorBox = geoalgo::AABox(0.0,-DetGeometry->DetHalfHeight(),0.0,2*DetGeometry->DetHalfWidth(),DetGeometry->DetHalfHeight(),DetGeometry->DetLength());
  }

  bool ERAnaIstgut::Analyze(const EventData &data, const ParticleGraph &ps)
  {
    // Get the MC graph
    std::cout << "starting loop: " << ps.GetNumParticles() << std::endl;
    std::cout << "array: "  << std::endl;
        
    // Get MC particle set
    auto const& mc_graph = MCParticleGraph();
    // Get the MC data
    auto const& mc_data = MCEventData();
    
    // Loop over particle array
    for(auto const & particle : mc_graph.GetParticleArray())
    {
      std::cout << "out1 " << particle.PdgCode() <<  " ";
      if(particle.RecoType() == RecoType_t::kShower)
      {
	auto const shower = mc_data.Shower(particle);
	std::cout << "Shower " << shower.Start() << std::endl;
      }
      else if(particle.RecoType() == RecoType_t::kTrack)
      {
	auto const track = mc_data.Track(particle);
	std::cout << "Track " << track.front() << std::endl;
	if(DetectorBox.Contain(track.front()))
	{
	    std::cout << "Is inside" << std::endl;
	}
      }
      else if(particle.RecoType() == RecoType_t::kInvisible)
      {
	auto const invisible = mc_data.Track(particle);
	if(particle.PdgCode() == 14)
	{
	  // Check if the particle is originating in the TPC
	  if(DetectorBox.Contain(invisible.front()))
	  {
	    std::cout << "Is inside" << std::endl;
	  }
	}
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
