#
# Makefile for B0->kpipi0 fit framework and related libraries
#

# Targets to make
TARGET1 = fit_framework
OBJS1 = FitFramework.o FF_Opt.o FF_Util.o FF_Test.o FF_Fit_bkg_bbar.o FF_Fit_bkg_continuum.o FF_Fit_data.o FF_Fit_Efficiency.o FF_Fit_signal.o FF_Fit_toy.o FF_Fit_EvtGen.o FF_Fit_control_channel.o FF_Fit_gsim.o FF_Fit_systematics.o

TARGET2 = IntegralBatchJob
OBJS2 = IntegralBatchJob.o

TARGET5 = JoinBatchJobs
OBJS5 = JoinBatchJobs.o

TARGET3 = libComplexPDF.so
OBJS3 = libComplexPDF/libComplexPDF.o

LRCPDF = libRooComplexPDF
TARGET4 = libRooComplexPDF.so
OBJS4 = $(LRCPDF)/RooRelativisticSpinBreitWigner.o $(LRCPDF)/RooBreitWignerTwoBodyPHSP.o $(LRCPDF)/RooBreitWignerTwoBodyPHSPEvtGen.o $(LRCPDF)/RooThreeBodyPhaseSpaceProjection.o $(LRCPDF)/RooBifurStudentsTDistribution.o $(LRCPDF)/RooStudentsTDistribution.o $(LRCPDF)/RooFlatte.o $(LRCPDF)/RooLASS.o $(LRCPDF)/RooKPiMass.o $(LRCPDF)/RooHelicityAngles.o $(LRCPDF)/RooMassHelicityAngles.o $(LRCPDF)/RooVarianceGammaDistribution.o $(LRCPDF)/RooDalitzPhaseSpaceAmplitude.o $(LRCPDF)/RooDalitzCubicPolynomial.o $(LRCPDF)/RooDalitzCubicPolynomialSymmetric.o $(LRCPDF)/RooTwoBodyThresholdFunction.o $(LRCPDF)/RooComplexPDF.o $(LRCPDF)/RooMassHelicityAngles_TransformedBase.o $(LRCPDF)/RooMassHelicityAnglesCPV.o $(LRCPDF)/RooCutPdf.o $(LRCPDF)/RooChebychevCut.o $(LRCPDF)/RooCategoryPdf.o $(LRCPDF)/RooFlatteAndNonRes.o $(LRCPDF)/RooKKInterference.o $(LRCPDF)/RooKKInterferenceNorm.o $(LRCPDF)/RooKKInterferencePositive.o $(LRCPDF)/RooDalitzAnalysis.o $(LRCPDF)/RooThresholdFunction.o $(LRCPDF)/RooDalitzDelaunay.o $(LRCPDF)/RooDalitzAnalysisCPV.o $(LRCPDF)/RooDalitzGauss.o $(LRCPDF)/RooDalitzRelativisticSpinBreitWigner.o $(LRCPDF)/RooDalitzHist.o $(LRCPDF)/RooDalitzXsd.o $(LRCPDF)/RooDalitzNovosibirsk.o
HDRS4 = $(LRCPDF)/RooRelativisticSpinBreitWigner.h $(LRCPDF)/RooBreitWignerTwoBodyPHSP.h $(LRCPDF)/RooBreitWignerTwoBodyPHSPEvtGen.h $(LRCPDF)/RooThreeBodyPhaseSpaceProjection.h $(LRCPDF)/RooBifurStudentsTDistribution.h $(LRCPDF)/RooStudentsTDistribution.h $(LRCPDF)/RooFlatte.h $(LRCPDF)/RooLASS.h $(LRCPDF)/RooKPiMass.h $(LRCPDF)/RooHelicityAngles.h $(LRCPDF)/RooMassHelicityAngles.h $(LRCPDF)/RooVarianceGammaDistribution.h $(LRCPDF)/RooDalitzPhaseSpaceAmplitude.h $(LRCPDF)/RooDalitzCubicPolynomial.h $(LRCPDF)/RooDalitzCubicPolynomialSymmetric.h $(LRCPDF)/RooTwoBodyThresholdFunction.h $(LRCPDF)/RooMassHelicityAngles_TransformedBase.h $(LRCPDF)/RooMassHelicityAnglesCPV.h $(LRCPDF)/RooCutPdf.h $(LRCPDF)/RooChebychevCut.h $(LRCPDF)/RooCategoryPdf.h $(LRCPDF)/RooFlatteAndNonRes.h $(LRCPDF)/RooKKInterference.h $(LRCPDF)/RooKKInterferenceNorm.h $(LRCPDF)/RooKKInterferencePositive.h $(LRCPDF)/RooDalitzAnalysis.h $(LRCPDF)/RooThresholdFunction.h $(LRCPDF)/RooDalitzDelaunay.h $(LRCPDF)/RooDalitzAnalysisCPV.h $(LRCPDF)/RooDalitzGauss.h $(LRCPDF)/RooDalitzRelativisticSpinBreitWigner.h $(LRCPDF)/RooDalitzHist.h $(LRCPDF)/RooDalitzXsd.h $(LRCPDF)/RooDalitzNovosibirsk.h

# Compiler
CC  = gcc
CXX = g++
FC = gfortran

# Optimization and Debug flags
OPT = -O2 #-g -v

# Other compiler flags
CFLAGS   = $(OPT) -fPIC -Wall -Wextra -Wfatal-errors
CXXFLAGS = $(OPT) -fPIC -Wall -Wextra -Wfatal-errors -Wno-sign-compare -std=c++0x
SOFLAGS  = $(OPT) -shared -Wl,-export-dynamic
FFLAGS   = $(OPT) -fPIC -fno-second-underscore -fno-automatic

# ROOT flags and libs
ROOT_FLAGS_INC = $(shell root-config --cflags)
ROOT_LIB = $(shell root-config --libs)
ROOFIT_LIB = -lRooFitCore -lRooFit -lHtml -lMinuit -lFoam -lGui
# Link against libMathMore if it exists
ROOFIT_LIB += $(shell if [ -e $(ROOTSYS)/lib/libMathMore.so ]; then echo -lMathMore; fi)

# Includes
NEUROBAYES_INC   = $(NEUROBAYES)/include
BOOST_INC = /home/belle/petric/boost/include
INCS = -I. $(ROOT_FLAGS_INC) -I$(BOOST_INC)

# Libraries
NEUROBAYES_LIB = -lEG -lNeuroBayesExpertCPP -lNeuroBayesTeacherCPP -L$(NEUROBAYES)/lib -L$(NEUROBAYES)/lib64
BOOST_LIB = -lboost_program_options -lboost_serialization -lboost_filesystem -lboost_system -lboost_thread
BOOST_LIB_DIR = /home/belle/petric/boost/lib
LIBS = -L. $(ROOT_LIB) $(ROOFIT_LIB) -L$(BOOST_LIB_DIR) $(BOOST_LIB)

.SUFFIXES: .c .cc

# C++ source files
.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCS) -o $@ $<

# C source files
.c.o:
	$(CXX) -c $(CFLAGS) $(INCS) -o $@ $<

# ------------------------------------------------------------------------

default::
	$(MAKE) $(TARGET3)
	$(MAKE) $(TARGET4)
	$(MAKE) $(TARGET1)

all::
	$(MAKE) $(TARGET3)
	$(MAKE) $(TARGET4)
	$(MAKE) $(TARGET1)
	$(MAKE) $(TARGET2)
	$(MAKE) $(TARGET5)

$(TARGET1): $(OBJS1) $(TARGET4)
	$(CXX) -o $@ $(OBJS1) $(LIBS) -lRooComplexPDF -lComplexPDF

$(TARGET2): $(OBJS2)
	$(CXX) -o $@ $(OBJS2) $(LIBS) -lRooComplexPDF -lComplexPDF

$(TARGET5): $(OBJS5)
	$(CXX) -o $@ $(OBJS5) $(LIBS) -lRooComplexPDF -lComplexPDF

$(TARGET3): $(OBJS3)
	$(CXX) -o $@ $(OBJS3) $(SOFLAGS)
	
$(TARGET4): $(OBJS4) $(TARGET3)
	$(CXX) -o $@ $(OBJS4) $(LIBS) $(SOFLAGS) -lComplexPDF
	
$(LRCPDF)/RooComplexPDF.o: $(HDRS4)
	rm -f $(LRCPDF)/RooComplexPDF.cc $(LRCPDF)/RooComplexPDF.h $(LRCPDF)/RooComplexPDF.o
	rootcint $(LRCPDF)/RooComplexPDF.cc -c $(HDRS4)
	$(CXX) -c $(CXXFLAGS) $(INCS) -o $(LRCPDF)/RooComplexPDF.o $(LRCPDF)/RooComplexPDF.cc
	
clean::
	rm -f $(OBJS1) $(OBJS3) $(OBJS4)

distclean::
	rm -f $(OBJS1) $(OBJS3) $(OBJS4)
	rm -f $(TARGET1) $(TARGET3) $(TARGET4)
	rm -f $(LRCPDF)/RooComplexPDF.h $(LRCPDF)/RooComplexPDF.cc
