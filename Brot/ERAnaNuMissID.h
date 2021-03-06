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
#include <TH1.h>
#include <THStack.h>
#include <TBranchElement.h>

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

    
  private:
    
    /// Number of in TPC neutrinos
    unsigned int _inTPC;
    /// Number of out of TPC neutrinos
    unsigned int _outTPC;
    
    /// Detector Box object
    geoalgo::AABox DetectorBox;
    geoalgo::Cylinder Cryostat;
    
    std::vector<float> BITEandCosmicEnergy;
    std::vector<float> BITEandDaugterEnergy;
    std::vector<float> NuandCosmicEnergy;
    std::vector<float> Unclassified;
    std::vector<float> SignalEnergy;
    
    THStack *hs = new THStack("hs","Miss ID");
    
    TH1F* BITECosmics = new TH1F("Bite And Cosmics","Bite And Cosmics",50,50,1000);
    TH1F* BITEDaugter = new TH1F("Bite And Daughter","Bite And Daughter",50,50,1000);
    TH1F* NuAndCosmic = new TH1F("Nu And Cosmic","Nu And Cosmic",50,50,1000);
    TH1F* Unclass = new TH1F("Nu And Cosmic","Nu And Cosmic",50,50,1000);
    
    double FancyPOTCalculator(unsigned int FileNumberStart, unsigned int FileNumberEnd);

  };
}
#endif

/** @} */ // end of doxygen group 
