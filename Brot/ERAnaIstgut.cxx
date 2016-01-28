#ifndef ERTOOL_ERANAISTGUT_CXX
#define ERTOOL_ERANAISTGUT_CXX

#include "ERAnaIstgut.h"

namespace ertool {

  ERAnaIstgut::ERAnaIstgut(const std::string& name) : AnaBase(name)
  {}

  void ERAnaIstgut::Reset()
  {}

  void ERAnaIstgut::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaIstgut::ProcessBegin()
  {}

  bool ERAnaIstgut::Analyze(const EventData &data, const ParticleGraph &ps)
  { return true; }

  void ERAnaIstgut::ProcessEnd(TFile* fout)
  {}

}

#endif
