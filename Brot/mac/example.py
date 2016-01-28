import sys
from ROOT import gSystem
gSystem.Load("libKase_Brot")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing Brot..."

sys.exit(0)

