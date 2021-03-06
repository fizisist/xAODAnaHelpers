#include "xAODRootAccess/Init.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "SampleHandler/DiskListLocal.h"
#include <TSystem.h>

#include "xAODAnaHelpers/BasicEventSelection.h"
#include "xAODAnaHelpers/JetCalibrator.h"
#include "xAODAnaHelpers/JetSelector.h"
#include "xAODAnaHelpers/BJetEfficiencyCorrector.h"
#include "xAODAnaHelpers/JetHistsAlgo.h"
#include "xAODAnaHelpers/MuonCalibrator.h"
#include "xAODAnaHelpers/MuonEfficiencyCorrector.h"
#include "xAODAnaHelpers/MuonSelector.h"
#include "xAODAnaHelpers/ElectronCalibrator.h"
#include "xAODAnaHelpers/ElectronEfficiencyCorrector.h"
#include "xAODAnaHelpers/ElectronSelector.h"
#include "xAODAnaHelpers/Writer.h"
#include "xAODAnaHelpers/OverlapRemover.h"
#include "xAODAnaHelpers/TreeAlgo.h"

#include "PATInterfaces/SystematicVariation.h"


int main( int argc, char* argv[] ) {

  // Take the submit directory from the input if provided:
  std::string submitDir = "submitDir";
  if( argc > 1 ) submitDir = argv[ 1 ];

  // Set up the job for xAOD access:
  xAOD::Init().ignore();

  // Construct the samples to run on:
  SH::SampleHandler sh;

  // default
  std::string datasetname;
  std::string filename;
  std::string dataPath;

  // usage:
  // test_multiAlgo  [optional] outdir dataPath/ datasetname filename
  if ( argc > 3 ) {
    dataPath = argv[ 2 ];
    datasetname = argv[3];

    SH::DiskListLocal list (dataPath);  // path to folder containing your datasets subfolders
    if( argc > 4 ){
      filename = argv[ 4 ];
      SH::scanDir (sh, list, filename); // running on a specific file
    } else {
      SH::scanDir (sh, list, "*.root*", datasetname ); // running on all files in a specific dataset
    }


  } else {
    // default
    //std::string filename = "mc14_13TeV.110351.PowhegPythia_P2012_ttbar_allhad.merge.AOD.e3232_s1982_s2008_r5787_r5853_skim.root";
    std::string filename = "mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.AOD.e3698_s2608_s2183_r6630_r6264.root";
    // std::string filename = "r20test_AOD.pool.root";
    // get the data path for xAODAnaHelpers/data
    std::string dataPath = gSystem->ExpandPathName("$ROOTCOREBIN/data");
    SH::DiskListLocal list (dataPath);
    SH::scanDir (sh, list, filename, "xAODAnaHelpers"); // specifying one particular sample
  }
  // Set the name of the input TTree. It's always "CollectionTree"
  // for xAOD files.
  sh.setMetaString( "nc_tree", "CollectionTree" );

  // Print what we found:
  sh.print();

  // Create an EventLoop job:
  EL::Job job;
  job.sampleHandler( sh );
  job.options()->setDouble(EL::Job::optRemoveSubmitDir, 1);

  // For Trigger
  job.options()->setString( EL::Job::optXaodAccessMode, EL::Job::optXaodAccessMode_branch );

  // Select max number of events
  //job.options()->setDouble (EL::Job::optMaxEvents, 1000);

  std::string localDataDir = "$ROOTCOREBIN/data/xAODAnaHelpers/";

  BasicEventSelection* baseEventSel             = new BasicEventSelection();
  baseEventSel->setName("baseEventSel")->setConfig(localDataDir+"baseEvent.config");

//  JET_GroupedNP_1__continuous
//  JET_GroupedNP_2__continuous
//  JET_GroupedNP_3__continuous
//  JET_RelativeNonClosure_MC12__continuous
//  JetCalibrator* jetCalib                       = new JetCalibrator(        "jetCalib_AntiKt4TopoEM",   localDataDir+"jetCalib_AntiKt4TopoEMCalib.config", "JET_GroupedNP_1", -1);

  JetCalibrator* jetCalib                       = new JetCalibrator();
  jetCalib->setName("jetCalib_AntiKt4TopoEM")->setConfig(localDataDir+"jetCalib_AntiKt4TopoEMCalib.config");

  MuonCalibrator* muonCalib                     = new MuonCalibrator();
  muonCalib->setName("muonCalib")->setConfig(localDataDir+"muonCalib.config");

  ElectronCalibrator* electronCalib             = new ElectronCalibrator();
  electronCalib->setName("electronCalib")->setConfig(localDataDir+"electronCalib.config");/*->setSysts("All");*/

  MuonEfficiencyCorrector*      muonEffCorr     = new MuonEfficiencyCorrector();
  muonEffCorr->setName("muonEfficiencyCorrector")->setConfig(localDataDir+"muonEffCorr.config");

  ElectronEfficiencyCorrector*  electronEffCorr = new ElectronEfficiencyCorrector();
  electronEffCorr->setName("electronEfficiencyCorrector")->setConfig(localDataDir+"electronEffCorr.config");/*->setSysts("All");*/

  MuonSelector* muonSelect_signal               = new MuonSelector();
  muonSelect_signal->setName("muonSelect_signal")->setConfig(localDataDir+"muonSelect_signal.config");

  ElectronSelector* electronSelect_signal       = new ElectronSelector();
  electronSelect_signal->setName("electronSelect_signal")->setConfig(localDataDir+"electronSelect_signal.config");

  JetSelector* jetSelect_signal                 = new JetSelector();
  jetSelect_signal->setName("jetSelect_signal")->setConfig(localDataDir+"jetSelect_signal.config");

  JetSelector* bjetSelect_signal                = new JetSelector();
  bjetSelect_signal->setName("bjetSelect_signal")->setConfig(localDataDir+"bjetSelect_signal.config");

  BJetEfficiencyCorrector* bjetEffCorr_btag     = new BJetEfficiencyCorrector();
  bjetEffCorr_btag->setName("bjetEffCor_btag")->setConfig(localDataDir+"bjetEffCorr.config");

  JetHistsAlgo* jetHistsAlgo_signal             = new JetHistsAlgo();
  jetHistsAlgo_signal->setName("jetHistsAlgo_signal")->setConfig(localDataDir+"jetHistsAlgo_signal.config");

  JetHistsAlgo* jetHistsAlgo_btag               = new JetHistsAlgo();
  jetHistsAlgo_btag->setName("jetHistsAlgo_btag")->setConfig(localDataDir+"jetHistsAlgo_btagged.config");

  JetSelector* jetSelect_truth                  = new JetSelector();
  jetSelect_truth->setName("jetSelect_truth")->setConfig(localDataDir+"jetSelect_truth.config");

  JetHistsAlgo* jetHistsAlgo_truth              = new JetHistsAlgo();
  jetHistsAlgo_truth->setName("jetHistsAlgo_truth")->setConfig(localDataDir+"jetHistsAlgo_truth.config");

  OverlapRemover* overlapRemoval                = new OverlapRemover();
  overlapRemoval->setName("OverlapRemovalTool")->setConfig(localDataDir+"overlapRemoval.config");

  JetHistsAlgo* jk_AntiKt10LC                   = new JetHistsAlgo();
  jk_AntiKt10LC->setName("AntiKt10/")->setConfig(localDataDir+"test_jetPlotExample.config");

  TreeAlgo* out_tree                            = new TreeAlgo();
  out_tree->setName("physics")->setConfig(localDataDir+"tree.config");

  // Attach algorithms
  job.algsAdd( baseEventSel );
  //job.algsAdd( jetCalib );
  //job.algsAdd( muonCalib );
  //job.algsAdd( muonEffCorr );
  //job.algsAdd( electronCalib );
  //job.algsAdd( electronEffCorr );
  //job.algsAdd( muonSelect_signal );
  //job.algsAdd( electronSelect_signal );
  //job.algsAdd( jetSelect_signal );
  //job.algsAdd( bjetSelect_signal );
  //job.algsAdd( bjetEffCorr_btag );
  //job.algsAdd( jetHistsAlgo_signal );
  //job.algsAdd( jetHistsAlgo_btag );
  //job.algsAdd( jetSelect_truth );
  //job.algsAdd( jetHistsAlgo_truth );
  //job.algsAdd( overlapRemoval );
  //job.algsAdd( jk_AntiKt10LC );
  job.algsAdd( out_tree );

  // Run the job using the local/direct driver:
  EL::DirectDriver driver;
  driver.submit( job, submitDir );

  return 0;
}


