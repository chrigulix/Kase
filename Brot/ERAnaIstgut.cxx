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
    for(auto const & particle : ps.GetPrimaryNodes(RecoType_t::kInvisible))
    {
      ps.
      std::cout << particle. << std::endl;
    }
    return true; 
  }

  void ERAnaIstgut::ProcessEnd(TFile* fout)
  {
    
  }

}

#endif
