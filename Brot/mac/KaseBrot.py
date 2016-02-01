import sys, os

# Kazu's temporary hack! 
#from ROOT import gSystem
#gSystem.Load('libLArLite_DataFormat')

#import matplotlib
#matplotlib.use('Qt4Agg')
# Kazu's temporary hack! 

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)
    
from ROOT import gSystem
from ROOT import larlite as fmwk
from ROOT import ertool
from singleE_config import GetERSelectionInstance

# Create ana_processor instance
my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)

use_reco = True if sys.argv[1] == 'reco' else False

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
#my_proc.set_ana_output_file("ertool_hist.root")

# Create ERTool algorithm (empty base class for this example)
#my_algo = ertool.AlgoBase()

# Create ERTool analysis (empty base class for this example)
my_ana = ertool.ERAnaIstgut()

# Create larlite interfce analysis unit for ERTool
my_anaunit = GetERSelectionInstance()

my_anaunit._mgr.ClearCfgFile()
my_anaunit._mgr.AddCfgFile(os.environ['LARLITE_USERDEVDIR']+'/SelectionTool/ERTool/dat/ertool_default%s.cfg'%('_reco' if use_reco else ''))

if use_reco:
    my_anaunit.SetShowerProducer(False,'showerrecofuzzy')
    my_anaunit.SetTrackProducer(False,'stitchkalmanhitcc')

#my_anaunit.SetVtxProducer(True,"generator");

#my_anaunit._mgr._mc_for_ana = True
#my_anaunit._mgr._profile_mode = True

# Implement manager
#my_anaunit._mgr.AddAlgo(my_algo)
my_anaunit._mgr.AddAna(my_ana)
#my_ana._mode =True # True = Select. False = Fill mode
my_proc.add_process(my_anaunit)

# run!
my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

#my_algo.StoreParams()
sys.exit(0)

