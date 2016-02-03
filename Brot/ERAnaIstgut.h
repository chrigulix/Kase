/**
 * \file ERAnaIstgut.h
 *
 * \ingroup Brot
 * 
 * \brief Class def header for a class ERAnaIstgut
 *
 * @author crohr
 */

/** \addtogroup Brot

    @{*/

#ifndef ERTOOL_ERANAISTGUT_H
#define ERTOOL_ERANAISTGUT_H

#include "ERTool/Base/AnaBase.h"
#include "ERTool/Base/Flash.h"
#include "LArUtil/Geometry.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoCylinder.h"

#include "TH1.h"

class TH1F;
namespace ertool {

  /**
     \class ERAnaIstgut
     User custom Analysis class made by kazuhiro
   */
  class ERAnaIstgut : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaIstgut(const std::string& name="ERAnaIstgut");

    /// Default destructor
    virtual ~ERAnaIstgut(){}

    /// Reset function
    virtual void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Analyze(const EventData &data, const ParticleGraph &ps);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout);
    
  private:
    
    // Number of in TPC neutrinos
    unsigned int _inTPC;
    // Number of out of TPC neutrinos
    unsigned int _outTPC;
    
    // Detector Box object
    geoalgo::AABox DetectorBox;
    geoalgo::Cylinder Cryostat;
    
    std::vector<float> InTPCEnergy;
    std::vector<float> InCryostatEnergy;
    std::vector<float> OutsideEnergy;
  };
}
#endif

/** @} */ // end of doxygen group 
