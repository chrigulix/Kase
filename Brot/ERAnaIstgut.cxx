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
    
    DetectorBox = geoalgo::AABox(0.0,-DetGeometry->DetHalfHeight(),0.0,
                                 2*DetGeometry->DetHalfWidth(),DetGeometry->DetHalfHeight(),DetGeometry->DetLength());
    
    // Distance of Cryostat wall to TPC on the cylinder axis
    double D = (DetGeometry->CryostatLength() - DetGeometry->DetLength())/2.0;
    
    Cryostat = geoalgo::Cylinder(DetGeometry->DetHalfWidth(), 0.0, -D,
                                 DetGeometry->DetHalfWidth(), 0.0, DetGeometry->CryostatLength (),
                                 DetGeometry->CryostatHalfHeight());
                                                
  }

  bool ERAnaIstgut::Analyze(const EventData &data, const ParticleGraph &graph)
  {
    // Get the MC graph
    std::cout << " --------- starting loop: --------------" << std::endl;
        
    // Get MC particle set
    auto const& mc_graph = MCParticleGraph();
    // Get the MC data
    auto const& mc_data = MCEventData();
    
    // Loop over particle set
    for(auto const & particle : graph.GetParticleArray())
    {
      if(particle.RecoType() == RecoType_t::kTrack)
      {
	auto const flash = data.Track(particle);
	//std::cout << "time:  " << flash._time << std::endl;
      }
    }
    
    // Loop over monte carlo particle set
    for(auto const & particle : mc_graph.GetParticleArray())
    {
        
//      std::cout << "out1 " << particle.PdgCode() << std::endl;
      if(particle.RecoType() == RecoType_t::kShower)
      {
	auto const shower = mc_data.Shower(particle);
//	std::cout << "Shower " << shower.Start() << std::endl;
      }
      else if(particle.RecoType() == RecoType_t::kTrack)
      {
//         auto const flash = mc_data.Flash(particle.FlashID ());
//         std::cout << "time:  " << flash._t << std::endl;
        
	auto const track = mc_data.Track(particle);
	//std::cout << "Track " << track.front() << std::endl;
      }
      else if(particle.RecoType() == RecoType_t::kInvisible)
      {
 	//auto const invisible = mc_data.Shower(particle);
	if(particle.PdgCode() == 11)
	{
	  // Check if the particle is originating in the TPC        
	  if(DetectorBox.Contain(particle.Vertex()))
	  {
	    std::cout << "Is inside TPC" << particle.Vertex() << std::endl;
	  }
	  else
	  {
	    std::cout << "Is outside TPC" << particle.Vertex() << std::endl;
            
            // Check if the particle is originating in the Cryostat 
            if(Cryostat.Contain (particle.Vertex ())){
                std::cout << "Is inside Cryostat " << particle.Vertex() << std::endl;
              }
            else {
                std::cout << "Is outside Cryostat " << particle.Vertex() << std::endl;
              }
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
