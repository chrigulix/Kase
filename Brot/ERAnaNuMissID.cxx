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
	
	bool FoundBITE = false;
	
	for(auto const& MCParticle : mc_graph.GetParticleArray())
	{
	  if(MCParticle.RecoID() == ChildRecoID)
	  {
	    Particle MCAncestor = mc_graph.GetParticle(MCParticle.Ancestor());
	    std::cout << "MC Ancestor ID: " << MCParticle.Ancestor() << " Reco ID: " << MCAncestor.RecoID() << " PDG Code: " << MCAncestor.PdgCode() << " Vertex: " << MCAncestor.Vertex() << std::endl;
	    std::cout << "MC Particle ID: " << MCParticle.ID() << " Reco ID: " << MCParticle.RecoID() << " PDG Code: " << MCParticle.PdgCode() << " Vertex: " << MCParticle.Vertex() << std::endl;
	    if( !(abs(MCAncestor.PdgCode()) == 12 || abs(MCAncestor.PdgCode()) == 14 || abs(MCAncestor.PdgCode()) == 16)
	        && !DetectorBox.Contain(MCAncestor.Vertex()) && Cryostat.Contain(MCAncestor.Vertex())
	        && (mc_data.Shower(MCParticle)._time < 0 || mc_data.Shower(MCParticle)._time > 1600) )
	    {
	      BITEandCosmicEnergy.push_back(data.Shower(ChildRecoID)._energy);
	      std::cout << "Bite And Cosmic: " << BITEandCosmicEnergy.back() << " " << mc_data.Shower(ChildRecoID)._energy << " " << particle.Energy() << std::endl;
	      FoundBITE = true;
	    }
	    else if( (abs(MCAncestor.PdgCode()) == 12 || abs(MCAncestor.PdgCode()) == 14 || abs(MCAncestor.PdgCode()) == 16)
		   && !DetectorBox.Contain(MCAncestor.Vertex()) && Cryostat.Contain(MCAncestor.Vertex())
		   && (mc_data.Shower(MCParticle)._time >= 0 || mc_data.Shower(MCParticle)._time <= 1600) )
	    {
	      BITEandDaugterEnergy.push_back(data.Shower(ChildRecoID)._energy);
	      std::cout << "Bite And Daughter: " << BITEandDaugterEnergy.back() << " " << particle.Energy() << std::endl;
	      FoundBITE = true;
	    }
	    else if(abs(MCAncestor.PdgCode()) == 12 && DetectorBox.Contain(MCAncestor.Vertex()))
	    {
	      SignalEnergy.push_back(data.Shower(ChildRecoID)._energy);
	      FoundBITE = true;
	      std::cout << "We have a signal! " << SignalEnergy.back() << " " << particle.Energy() << std::endl;
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
	      std::cout << "Nu And Cosmic: " << NuandCosmicEnergy.back() << " " << particle.Energy() << std::endl;
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
      if(abs(mc_particle.PdgCode()) == 12 && abs(mc_particle.PdgCode()) == 14 && abs(mc_particle.PdgCode()) == 16)
      {
	std::cout << mc_particle.Print() << std::endl;
	
	
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
    
    fout->Close();
    
    std::cout << FancyPOTCalculator(0,0) << std::endl;
  }
  
double ERAnaNuMissID::FancyPOTCalculator(unsigned int FileNumberStart, unsigned int FileNumberEnd)
{
  double TotPOT = 0;
  
  double FilePOT;
  
  for(unsigned int file_no = FileNumberStart; file_no <= FileNumberEnd; file_no++)
  {
    std::string FileName = "/home/crohr/uBData/scan_prodgenie_bnb_nu_cosmic_uboone_mcc7_detsim_v1/larlite_mcinfo_";
    std::stringstream FileNumber;
    FileNumber << std::setfill('0') << std::setw(4) << file_no;
    FileName += FileNumber.str() + ".root";
    
    TFile* InputFile = new TFile(FileName.c_str(),"READ");
    InputFile->cd();
    
    TTree *POTTree = (TTree*)InputFile->Get("potsummary_generator_tree");
    TBranch* TheBranch = POTTree->GetBranch('potsummary_generator_branch/totgoodpot');
    TheBranch->SetAddress(&FilePOT);
//     POTTree->SetBranchAddress("potsummary_generator_branch/totgoodpot",&FilePOT);
    
    for(unsigned int entry_no = 0; entry_no < POTTree->GetEntries(); entry_no++)
    {
      POTTree->GetEntry(entry_no);
      std::cout <<  FilePOT << std::endl;
      TotPOT += FilePOT;
    } 
  }
  return TotPOT;
}

  
}

#endif
