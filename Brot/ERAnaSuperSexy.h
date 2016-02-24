/**
 * \file ERAnaSuperSexy.h
 *
 * \ingroup Brot
 * 
 * \brief Class def header for a class ERAnaSuperSexy
 *
 * @author matthias
 */

/** \addtogroup Brot

    @{*/

#ifndef ERTOOL_ERANASUPERSEXY_H
#define ERTOOL_ERANASUPERSEXY_H

#include "ERTool/Base/AnaBase.h"
#include "ERTool/Base/Flash.h"
#include "LArUtil/Geometry.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoCylinder.h"

#include "TFile.h"
#include "TTree.h"

#include <fstream>

namespace ertool {

  /**
     \class ERAnaSuperSexy
     User custom Analysis class made by kazuhiro
   */
  class ERAnaSuperSexy : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaSuperSexy(const std::string& name="ERAnaSuperSexy");

    /// Default destructor
    virtual ~ERAnaSuperSexy(){}

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
    
    void PrepareTree();

  private:
    // Number of in TPC neutrinos
    unsigned int _inTPC;
    // Number of out of TPC neutrinos
    unsigned int _outTPC;
    
    // Number of Events
    unsigned long int _NumberOfEvents = 0;
    unsigned long int _NumberOfPrimaryFlashes = 0;
    
    // Detector Box object
    geoalgo::AABox DetectorBox;
    geoalgo::Cylinder Cryostat;
    
    bool _output = true;
    
    // root definitions
    std::string _root_out_file = "bite.root"; 
    TTree* _result;
    
    // output definitions
    std::vector<float> _out_x_pos;
    std::vector<float> _out_y_pos;
    std::vector<float> _out_z_pos;
    
  };
}
#endif

/** @} */ // end of doxygen group 
