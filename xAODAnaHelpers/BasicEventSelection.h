#ifndef xAODAnaHelpers_BasicEventSelection_H
#define xAODAnaHelpers_BasicEventSelection_H

// ROOT include(s):
#include "TH1D.h"

// rootcore includes
#include "GoodRunsLists/GoodRunsListSelectionTool.h"
#include "PileupReweighting/PileupReweightingTool.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

namespace TrigConf {
  class xAODConfigTool;
}

namespace Trig {
  class TrigDecisionTool;
}

class BasicEventSelection : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
  public:
    // variables read in through configuration file
    bool m_truthLevelOnly;  //!
    // GRL
    bool m_applyGRL;        //!
    std::string m_GRLxml;   //!
    //PU Reweighting
    bool m_doPUreweighting; //!
    std::string m_triggerSelection; //!

    // primary vertex
    std::string m_vertexContainerName; //!
    int m_PVNTrack;                //!

  private:
    GoodRunsListSelectionTool*   m_grl;       //!
    CP::PileupReweightingTool*   m_pileuptool; //!

    TrigConf::xAODConfigTool*    m_trigConfTool;  //!
    Trig::TrigDecisionTool*      m_trigDecTool;   //!

    int m_eventCounter;     //!

    // read from MetaData
    TH1D* m_histEventCount;  //!
    int m_MD_initialNevents; //!
    int m_MD_finalNevents;   //!
    float m_MD_initialSumW;    //!
    float m_MD_finalSumW;	     //!

    // cutflow
    TH1D* m_cutflowHist;    //!
    TH1D* m_cutflowHistW;   //!
    int m_cutflow_all;      //!
    int m_cutflow_grl;      //!
    int m_cutflow_lar;      //!
    int m_cutflow_tile;     //!
    int m_cutflow_core;     //!
    int m_cutflow_npv;      //!
    int m_cutflow_trigger;      //!

    // variables that don't get filled at submission time should be
    // protected from being send from the submission node to the worker
    // node (done by the //!)
  public:
    // Tree *myTree; //!
    // TH1 *myHist; //!
    //

    // this is a standard constructor
    BasicEventSelection ();

    // these are the functions inherited from Algorithm
    virtual EL::StatusCode setupJob (EL::Job& job);
    virtual EL::StatusCode fileExecute ();
    virtual EL::StatusCode histInitialize ();
    virtual EL::StatusCode changeInput (bool firstFile);
    virtual EL::StatusCode initialize ();
    virtual EL::StatusCode execute ();
    virtual EL::StatusCode postExecute ();
    virtual EL::StatusCode finalize ();
    virtual EL::StatusCode histFinalize ();

    // these are the functions not inherited from Algorithm
    virtual EL::StatusCode configure ();

    // this is needed to distribute the algorithm to the workers
    ClassDef(BasicEventSelection, 1);
};

#endif
