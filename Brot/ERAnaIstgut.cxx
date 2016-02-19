#ifndef ERTOOL_ERANAISTGUT_CXX
#define ERTOOL_ERANAISTGUT_CXX

#include <fstream>

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
                                 DetGeometry->DetHalfWidth(), 0.0, DetGeometry->CryostatLength() - D,
                                 DetGeometry->CryostatHalfHeight());
                                                
  }

  bool ERAnaIstgut::Analyze(const EventData &data, const ParticleGraph &graph)
  {
     
    // ========================================================================
    // ============================== DATA PART ===============================
    // ========================================================================
    _NumberOfEvents++;
    auto const & EventNumber = data.Event_ID();
    
//     std::cout << mc_graph.Diagram() << std::endl;
    
    // particlegraph.GetParticle( particle.Ancestor() ).FlashID()
    // Loop over particle set
    
//    std::cout << "---------------- Event: " << data.Event_ID() << " ----------------" << std::endl;
    
    // set of flashes that are tagged as primary
    std::set< FlashID_t > TaggedAncestors;
    
    for(auto const & particle : graph.GetParticleArray())
    { 
      // only the ancestor (base particle) has the tpc - flash matched information
      auto Ancestor = graph.GetParticle(particle.Ancestor());
      
      // all non matched base particles get the kCosmic tag 
      if(Ancestor.ProcessType() != kCosmic)
      {
        // Make sure we only count base particles once.
	if((TaggedAncestors.empty()) || (TaggedAncestors.find(data.Flash(Ancestor).FlashID()) == TaggedAncestors.end()))
	{
	  _NumberOfPrimaryFlashes++;
          TaggedAncestors.insert(data.Flash(Ancestor).FlashID());
	}
        
//	 std::cout << "part reco ID:  " << particle.RecoID() << "\n" 
//		    << "ancestor flash id:  " <<  data.Flash(graph.GetParticle(particle.Ancestor())).FlashID() << "\n "
//		    << "part flash time:  " << data.Flash(graph.GetParticle(particle.Ancestor()))._t << "\n " 
//		    << std::endl;
	
	
	if(particle.RecoType() == RecoType_t::kTrack)
	{
// 	  try{

// 	  auto const flash = data.Flash( graph.GetParticle(particle.Ancestor()).FlashID() );
	  auto const time = data.Track(particle.RecoID())._time; 
 	 
// 	  std::cout << "time:  " << particle.RecoID() << " " << time << " " << flash._t << std::endl;
// 	  } catch (ERException &e ) {}
// 	auto const flash = data.Flash(particle.RecoID());
      
// 	auto const time = data.Track(particle.RecoID())._time;
//       auto const mcflash = mc_data.Flash(particle);
      
	  
	}
      }
    }
    
    
    // ========================================================================
    // =============================== MC PART ================================
    // ========================================================================
    // Get MC particle set
    auto const& mc_graph = MCParticleGraph();
    // Get the MC data
    auto const& mc_data = MCEventData();
    
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
	if(particle.PdgCode() == 14)
	{
	  // Check if the particle is originating in the TPC        
	  if(DetectorBox.Contain(particle.Vertex()))
	  {
// 	    std::cout << "Is inside TPC" << particle.Vertex() << std::endl;
	    InTPCEnergy.push_back(particle.Energy());
	  }
	  // Check if the particle is originating in the Cryostat
	  else if(Cryostat.Contain (particle.Vertex ()))
	  {
            // loop over actual data events here to see what our analysis did
            for (auto const & interesting : graph.GetParticleArray())
            {
                    //std::cout << "interesting (in cryo)" << std::endl;
                    //std::cout << "pdg: " << interesting.PdgCode() << std::endl;
                    //std::cout << "reco obj: " << interesting.RecoType() << std::endl;
                    
                    // was this tagged as a kCosmic during the analysis --> we ignore this event
                    if ( !(interesting.RecoType() == kCosmic))
                    {
                      std::cout << "whoops " << data.Flash(interesting.Ancestor()).FlashID() << std::endl;
                      std::cout << "whoops " << data.Flash(interesting.Ancestor())._t << std::endl;  
                    } 
            }
            
	    InCryostatEnergy.push_back(particle.Energy());
	    
	    XVertex.push_back(particle.Vertex().at(0));
	    YVertex.push_back(particle.Vertex().at(1));
	    ZVertex.push_back(particle.Vertex().at(2));
	  }
	  else 
	  {
	    OutsideEnergy.push_back(particle.Energy());
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
    
    std::cout << "Total number of Events: " << _NumberOfEvents << std::endl;
    std::cout << "Number of recognized nu events: " << _NumberOfPrimaryFlashes << std::endl;
    
    fout = new TFile("Kasebrot.root","RECREATE");
    fout->cd();
    TH1F* hInTPC = new TH1F("In TPC","In TPC",40,0,4000);
    TH1F* hInCryo = new TH1F("In Cryo","In Cryo",40,0,4000);
    TH1F* hOut = new TH1F("Outside","Outside",40,0,4000);
    
    TH1F* hX = new TH1F("x position","x position",356,-50,306);
    TH1F* hY = new TH1F("y position","y position",333,-166.5,166.5);
    TH1F* hZ = new TH1F("z position","z position",1235,-100,1135);
    
    for(auto & event : InTPCEnergy)
    {
//       std::cout << event << std::endl;
      hInTPC->Fill(std::move(event));
    }
    
    for(auto & event : InCryostatEnergy)
    {
      hInCryo->Fill(std::move(event));
    }
    
    for(auto & event : OutsideEnergy)
    {
      hOut->Fill(std::move(event));
    }
    
    for(auto & event : XVertex)
    {
      hX->Fill(std::move(event));
    }
    
    for(auto & event : YVertex)
    {
      hY->Fill(std::move(event));
    }
    
    for(auto & event : ZVertex)
    {
      hZ->Fill(std::move(event));
    }
    
    hInTPC->Write();
    hInCryo->Write();
    hOut->Write();
    
    hX->Write();
    hY->Write();
    hZ->Write();
    
    fout->Close();
  }

}

#endif
