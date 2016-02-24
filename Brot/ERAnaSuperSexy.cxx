#ifndef ERTOOL_ERANASUPERSEXY_CXX
#define ERTOOL_ERANASUPERSEXY_CXX

#include "ERAnaSuperSexy.h"

namespace ertool {

	ERAnaSuperSexy::ERAnaSuperSexy(const std::string& name) : AnaBase(name)
	{
	}

	void ERAnaSuperSexy::Reset()
	{
	}

	void ERAnaSuperSexy::AcceptPSet(const ::fcllite::PSet& cfg)
	{
	}

	void ERAnaSuperSexy::ProcessBegin()
	{
		_inTPC = 0;
		_outTPC = 0;
		
		PrepareTree();

		auto DetGeometry = ::larutil::Geometry::GetME();

		DetectorBox = geoalgo::AABox(0.0, -DetGeometry->DetHalfHeight(), 0.0,
			2 * DetGeometry->DetHalfWidth(), DetGeometry->DetHalfHeight(), DetGeometry->DetLength());

		// Distance of Cryostat wall to TPC on the cylinder axis
		double D = (DetGeometry->CryostatLength() - DetGeometry->DetLength()) / 2.0;

		Cryostat = geoalgo::Cylinder(DetGeometry->DetHalfWidth(), 0.0, -D,
			DetGeometry->DetHalfWidth(), 0.0, DetGeometry->CryostatLength() - D,
			DetGeometry->CryostatHalfHeight());
	}

	bool ERAnaSuperSexy::Analyze(const EventData &data, const ParticleGraph &graph)
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
		
		// was the neutrino we found actually a neutrino
		
		
		
		auto const& mc_graph = MCParticleGraph();
		// Get the MC data
		auto const& mc_data = MCEventData();
		
		

//		// Loop over monte carlo particle set
//		for (auto const & mcparticle : mc_graph.GetParticleArray()) {
//			
//			// look at neutrinos
//			if (mcparticle.PdgCode() == 13) {
//				// Check if the particle is originating in the TPC        
//				if (DetectorBox.Contain(mcparticle.Vertex())) {
//					//std::cout << "Is inside TPC" << particle.Vertex() << std::endl;
//				}// Check if the particle is originating in the Cryostat
//				else if (!Cryostat.Contain(mcparticle.Vertex())) {
//							std::cout << "---------------- Event: " << data.Event_ID() << " ----------------" << std::endl;
//
//					// loop over actual data events here to see what our analysis did
//					for (auto const & interesting : graph.GetParticleArray()) {
//						if (interesting.RecoType() == RecoType_t::kTrack) {
//							if (!data.Track(interesting).IsLonger(3)) continue;
//							_out_x_pos.push_back(data.Track(interesting).front().at(0));
//							_out_y_pos.push_back(data.Track(interesting).front().at(1));
//							_out_z_pos.push_back(data.Track(interesting).front().at(2));
//						}
//						if (interesting.RecoType() == RecoType_t::kShower) {
//							if (data.Shower(interesting).Length() < 3) continue; 
//							_out_x_pos.push_back(data.Shower(interesting).Start().at(0));
//							_out_y_pos.push_back(data.Shower(interesting).Start().at(1));
//							_out_z_pos.push_back(data.Shower(interesting).Start().at(2));
//						}
//					}
//					for (unsigned int i = 0; i < _out_x_pos.size(); i++){
//						std::cout << "entry " << i << " x: " << _out_x_pos[i] << " y: " << _out_y_pos[i] << " z: " << _out_z_pos[i] << std::endl;
//					}
//					
//				}
//				_result->Fill();
//			}
//
//		}
		
		return true;

	}

	void ERAnaSuperSexy::ProcessEnd(TFile * fout)
	{
		if (_output) {
			fout = new TFile(_root_out_file.c_str(),"RECREATE");
			fout->cd();
			
			_result->Write();
		}

		return;
	}
	
	void ERAnaSuperSexy::PrepareTree(){
		if (_output) { delete _result; }
		_result = new TTree(Form("%s", "result"), "Result Tree");
		_result->Branch("x-pos", &_out_x_pos, "x-pos");
		_result->Branch("y-pos", &_out_y_pos, "x-pos");
		_result->Branch("z-pos", &_out_z_pos, "x-pos");
		
		
		
	}

}

#endif
