/**
 * \file ERAnaNuMissID.h
 *
 * \ingroup Brot
 * 
 * \brief Class def header for a class ERAnaNuMissID
 *
 * @author crohr
 */

/** \addtogroup Brot

    @{*/

#ifndef ERTOOL_ERANANUMISSID_H
#define ERTOOL_ERANANUMISSID_H

#include "ERTool/Base/AnaBase.h"
#include "ERTool/Base/Flash.h"
#include "LArUtil/Geometry.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoCylinder.h"

namespace ertool {

  /**
     \class ERAnaNuMissID
     User custom Analysis class made by kazuhiro
   */
  class ERAnaNuMissID : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaNuMissID(const std::string& name="ERAnaNuMissID");

    /// Default destructor
    virtual ~ERAnaNuMissID(){}

    /// Reset function
    virtual void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Analyze(const EventData &data, const ParticleGraph &ps);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);
    
    /// Check if MCPDG corresponds to RecoPDG
    bool MCChecker(Particle& ParticleToCheck, const EventData& Data, const ParticleGraph& Graph, const EventData& MCData, const ParticleGraph& MCGraph, const int PDGCode);
    
  private:
    
    /// Number of in TPC neutrinos
    unsigned int _inTPC;
    /// Number of out of TPC neutrinos
    unsigned int _outTPC;
    
    /// Detector Box object
    geoalgo::AABox DetectorBox;
    geoalgo::Cylinder Cryostat;
    

  };
}
#endif

/** @} */ // end of doxygen group 
