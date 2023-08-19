###########################################################################
## Makefile generated for component 'LSTMnetwork22esfand400'. 
## 
## Makefile     : LSTMnetwork22esfand400_rtw.mk
## Generated on : Sun Mar 13 15:03:34 2022
## Final product: .\LSTMnetwork22esfand400.lib
## Product type : static-library
## 
###########################################################################

###########################################################################
## MACROS
###########################################################################

# Macro Descriptions:
# PRODUCT_NAME            Name of the system to build
# MAKEFILE                Name of this makefile
# COMPILER_COMMAND_FILE   Compiler command listing model reference header paths
# CMD_FILE                Command file
# MODELLIB                Static library target

PRODUCT_NAME              = LSTMnetwork22esfand400
MAKEFILE                  = LSTMnetwork22esfand400_rtw.mk
MATLAB_ROOT               = D:\Programs\MATLAB21\Polyspace
MATLAB_BIN                = D:\Programs\MATLAB21\Polyspace\bin
MATLAB_ARCH_BIN           = $(MATLAB_BIN)\win64
START_DIR                 = E:\projects\New projects\myo armband\LSTM\codegen\lib\LSTMnetwork22esfand400
TGT_FCN_LIB               = ISO_C++11
SOLVER_OBJ                = 
CLASSIC_INTERFACE         = 0
MODEL_HAS_DYNAMICALLY_LOADED_SFCNS = 
RELATIVE_PATH_TO_ANCHOR   = .
COMPILER_COMMAND_FILE     = LSTMnetwork22esfand400_rtw_comp.rsp
CMD_FILE                  = LSTMnetwork22esfand400_rtw.rsp
C_STANDARD_OPTS           = 
CPP_STANDARD_OPTS         = 
NODEBUG                   = 1
MODELLIB                  = LSTMnetwork22esfand400.lib

###########################################################################
## TOOLCHAIN SPECIFICATIONS
###########################################################################

# Toolchain Name:          Microsoft Visual C++ 2017 v15.0 | nmake (64-bit Windows)
# Supported Version(s):    15.0
# ToolchainInfo Version:   2021a
# Specification Revision:  1.0
# 
#-------------------------------------------
# Macros assumed to be defined elsewhere
#-------------------------------------------

# NODEBUG
# cvarsdll
# cvarsmt
# conlibsmt
# ldebug
# conflags
# cflags

#-----------
# MACROS
#-----------

MW_EXTERNLIB_DIR    = $(MATLAB_ROOT)\extern\lib\win64\microsoft
MW_LIB_DIR          = $(MATLAB_ROOT)\lib\win64
CPU                 = AMD64
APPVER              = 5.02
CVARSFLAG           = $(cvarsmt)
CFLAGS_ADDITIONAL   = -D_CRT_SECURE_NO_WARNINGS
CPPFLAGS_ADDITIONAL = -EHs -D_CRT_SECURE_NO_WARNINGS /wd4251
LIBS_TOOLCHAIN      = $(conlibs)

TOOLCHAIN_SRCS = 
TOOLCHAIN_INCS = 
TOOLCHAIN_LIBS = 

#------------------------
# BUILD TOOL COMMANDS
#------------------------

# C Compiler: Microsoft Visual C Compiler
CC = cl

# Linker: Microsoft Visual C Linker
LD = link

# C++ Compiler: Microsoft Visual C++ Compiler
CPP = cl

# C++ Linker: Microsoft Visual C++ Linker
CPP_LD = link

# Archiver: Microsoft Visual C/C++ Archiver
AR = lib

# MEX Tool: MEX Tool
MEX_PATH = $(MATLAB_ARCH_BIN)
MEX = "$(MEX_PATH)\mex"

# Download: Download
DOWNLOAD =

# Execute: Execute
EXECUTE = $(PRODUCT)

# Builder: NMAKE Utility
MAKE = nmake


#-------------------------
# Directives/Utilities
#-------------------------

CDEBUG              = -Zi
C_OUTPUT_FLAG       = -Fo
LDDEBUG             = /DEBUG
OUTPUT_FLAG         = -out:
CPPDEBUG            = -Zi
CPP_OUTPUT_FLAG     = -Fo
CPPLDDEBUG          = /DEBUG
OUTPUT_FLAG         = -out:
ARDEBUG             =
STATICLIB_OUTPUT_FLAG = -out:
MEX_DEBUG           = -g
RM                  = @del
ECHO                = @echo
MV                  = @ren
RUN                 = @cmd /C

#--------------------------------------
# "Faster Runs" Build Configuration
#--------------------------------------

ARFLAGS              = /nologo
CFLAGS               = $(cflags) $(CVARSFLAG) $(CFLAGS_ADDITIONAL) \
                       /O2 /Oy-
CPPFLAGS             = /TP $(cflags) $(CVARSFLAG) $(CPPFLAGS_ADDITIONAL) \
                       /O2 /Oy-
CPP_LDFLAGS          = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN)
CPP_SHAREDLIB_LDFLAGS  = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN) \
                         -dll -def:$(DEF_FILE)
DOWNLOAD_FLAGS       =
EXECUTE_FLAGS        =
LDFLAGS              = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN)
MEX_CPPFLAGS         =
MEX_CPPLDFLAGS       =
MEX_CFLAGS           =
MEX_LDFLAGS          =
MAKE_FLAGS           = -f $(MAKEFILE)
SHAREDLIB_LDFLAGS    = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN) \
                       -dll -def:$(DEF_FILE)



###########################################################################
## OUTPUT INFO
###########################################################################

PRODUCT = .\LSTMnetwork22esfand400.lib
PRODUCT_TYPE = "static-library"
BUILD_TYPE = "Static Library"

###########################################################################
## INCLUDE PATHS
###########################################################################

INCLUDES_BUILDINFO = 

INCLUDES = $(INCLUDES_BUILDINFO)

###########################################################################
## DEFINES
###########################################################################

DEFINES_CUSTOM = 
DEFINES_STANDARD = -DMODEL=LSTMnetwork22esfand400

DEFINES = $(DEFINES_CUSTOM) $(DEFINES_STANDARD)

###########################################################################
## SOURCE FILES
###########################################################################

SRCS = $(START_DIR)\MWCNNLayer.cpp $(START_DIR)\MWFCLayer.cpp $(START_DIR)\MWOutputLayer.cpp $(START_DIR)\MWRNNLayer.cpp $(START_DIR)\MWSequenceInputLayer.cpp $(START_DIR)\MWTensorBase.cpp $(START_DIR)\MWFCLayerImpl.cpp $(START_DIR)\MWOutputLayerImpl.cpp $(START_DIR)\MWRNNLayerImpl.cpp $(START_DIR)\MWSequenceInputLayerImpl.cpp $(START_DIR)\MWCNNLayerImpl.cpp $(START_DIR)\MWTargetNetworkImpl.cpp $(START_DIR)\MWMkldnnUtils.cpp $(START_DIR)\MWCustomLayerForMKLDNN.cpp $(START_DIR)\LSTMnetwork22esfand400_data.cpp $(START_DIR)\LSTMnetwork22esfand400_initialize.cpp $(START_DIR)\LSTMnetwork22esfand400_terminate.cpp $(START_DIR)\LSTMnetwork22esfand400.cpp $(START_DIR)\DeepLearningNetwork.cpp $(START_DIR)\predict.cpp

ALL_SRCS = $(SRCS)

###########################################################################
## OBJECTS
###########################################################################

OBJS = MWCNNLayer.obj MWFCLayer.obj MWOutputLayer.obj MWRNNLayer.obj MWSequenceInputLayer.obj MWTensorBase.obj MWFCLayerImpl.obj MWOutputLayerImpl.obj MWRNNLayerImpl.obj MWSequenceInputLayerImpl.obj MWCNNLayerImpl.obj MWTargetNetworkImpl.obj MWMkldnnUtils.obj MWCustomLayerForMKLDNN.obj LSTMnetwork22esfand400_data.obj LSTMnetwork22esfand400_initialize.obj LSTMnetwork22esfand400_terminate.obj LSTMnetwork22esfand400.obj DeepLearningNetwork.obj predict.obj

ALL_OBJS = $(OBJS)

###########################################################################
## PREBUILT OBJECT FILES
###########################################################################

PREBUILT_OBJS = 

###########################################################################
## LIBRARIES
###########################################################################

LIBS = C:\PROGRA~1\mkl-dnn\lib\dnnl.lib

###########################################################################
## SYSTEM LIBRARIES
###########################################################################

SYSTEM_LIBS = 

###########################################################################
## ADDITIONAL TOOLCHAIN FLAGS
###########################################################################

#---------------
# C Compiler
#---------------

CFLAGS_ = /source-charset:utf-8
CFLAGS_BASIC = $(DEFINES) @$(COMPILER_COMMAND_FILE)

CFLAGS = $(CFLAGS) $(CFLAGS_) $(CFLAGS_BASIC)

#-----------------
# C++ Compiler
#-----------------

CPPFLAGS_ = /source-charset:utf-8
CPPFLAGS_BASIC = $(DEFINES) @$(COMPILER_COMMAND_FILE)

CPPFLAGS = $(CPPFLAGS) $(CPPFLAGS_) $(CPPFLAGS_BASIC)

###########################################################################
## INLINED COMMANDS
###########################################################################


!include $(MATLAB_ROOT)\rtw\c\tools\vcdefs.mak


###########################################################################
## PHONY TARGETS
###########################################################################

.PHONY : all build clean info prebuild download execute set_environment_variables


all : build
	@cmd /C "@echo ### Successfully generated all binary outputs."


build : set_environment_variables prebuild $(PRODUCT)


prebuild : 


download : $(PRODUCT)


execute : download


set_environment_variables : 
	@set INCLUDE=$(INCLUDES);$(INCLUDE)
	@set LIB=$(LIB)


###########################################################################
## FINAL TARGET
###########################################################################

#---------------------------------
# Create a static library         
#---------------------------------

$(PRODUCT) : $(OBJS) $(PREBUILT_OBJS)
	@cmd /C "@echo ### Creating static library "$(PRODUCT)" ..."
	$(AR) $(ARFLAGS) -out:$(PRODUCT) @$(CMD_FILE)
	@cmd /C "@echo ### Created: $(PRODUCT)"


###########################################################################
## INTERMEDIATE TARGETS
###########################################################################

#---------------------
# SOURCE-TO-OBJECT
#---------------------

.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(RELATIVE_PATH_TO_ANCHOR)}.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{$(RELATIVE_PATH_TO_ANCHOR)}.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(START_DIR)}.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{$(START_DIR)}.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{E:\projects\New projects\myo armband\LSTM}.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{E:\projects\New projects\myo armband\LSTM}.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


MWCNNLayer.obj : $(START_DIR)\MWCNNLayer.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\MWCNNLayer.cpp


MWFCLayer.obj : $(START_DIR)\MWFCLayer.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\MWFCLayer.cpp


MWOutputLayer.obj : $(START_DIR)\MWOutputLayer.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\MWOutputLayer.cpp


MWRNNLayer.obj : $(START_DIR)\MWRNNLayer.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\MWRNNLayer.cpp


MWSequenceInputLayer.obj : $(START_DIR)\MWSequenceInputLayer.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\MWSequenceInputLayer.cpp


MWTensorBase.obj : $(START_DIR)\MWTensorBase.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\MWTensorBase.cpp


MWFCLayerImpl.obj : $(START_DIR)\MWFCLayerImpl.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\MWFCLayerImpl.cpp


MWOutputLayerImpl.obj : $(START_DIR)\MWOutputLayerImpl.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\MWOutputLayerImpl.cpp


MWRNNLayerImpl.obj : $(START_DIR)\MWRNNLayerImpl.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\MWRNNLayerImpl.cpp


MWSequenceInputLayerImpl.obj : $(START_DIR)\MWSequenceInputLayerImpl.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\MWSequenceInputLayerImpl.cpp


MWCNNLayerImpl.obj : $(START_DIR)\MWCNNLayerImpl.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\MWCNNLayerImpl.cpp


MWTargetNetworkImpl.obj : $(START_DIR)\MWTargetNetworkImpl.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\MWTargetNetworkImpl.cpp


MWMkldnnUtils.obj : $(START_DIR)\MWMkldnnUtils.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\MWMkldnnUtils.cpp


MWCustomLayerForMKLDNN.obj : $(START_DIR)\MWCustomLayerForMKLDNN.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\MWCustomLayerForMKLDNN.cpp


LSTMnetwork22esfand400_data.obj : $(START_DIR)\LSTMnetwork22esfand400_data.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\LSTMnetwork22esfand400_data.cpp


LSTMnetwork22esfand400_initialize.obj : $(START_DIR)\LSTMnetwork22esfand400_initialize.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\LSTMnetwork22esfand400_initialize.cpp


LSTMnetwork22esfand400_terminate.obj : $(START_DIR)\LSTMnetwork22esfand400_terminate.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\LSTMnetwork22esfand400_terminate.cpp


LSTMnetwork22esfand400.obj : $(START_DIR)\LSTMnetwork22esfand400.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\LSTMnetwork22esfand400.cpp


DeepLearningNetwork.obj : $(START_DIR)\DeepLearningNetwork.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\DeepLearningNetwork.cpp


predict.obj : $(START_DIR)\predict.cpp
	$(CPP) $(CPPFLAGS) -Fo"$@" $(START_DIR)\predict.cpp


###########################################################################
## DEPENDENCIES
###########################################################################

$(ALL_OBJS) : rtw_proj.tmw $(COMPILER_COMMAND_FILE) $(MAKEFILE)


###########################################################################
## MISCELLANEOUS TARGETS
###########################################################################

info : 
	@cmd /C "@echo ### PRODUCT = $(PRODUCT)"
	@cmd /C "@echo ### PRODUCT_TYPE = $(PRODUCT_TYPE)"
	@cmd /C "@echo ### BUILD_TYPE = $(BUILD_TYPE)"
	@cmd /C "@echo ### INCLUDES = $(INCLUDES)"
	@cmd /C "@echo ### DEFINES = $(DEFINES)"
	@cmd /C "@echo ### ALL_SRCS = $(ALL_SRCS)"
	@cmd /C "@echo ### ALL_OBJS = $(ALL_OBJS)"
	@cmd /C "@echo ### LIBS = $(LIBS)"
	@cmd /C "@echo ### MODELREF_LIBS = $(MODELREF_LIBS)"
	@cmd /C "@echo ### SYSTEM_LIBS = $(SYSTEM_LIBS)"
	@cmd /C "@echo ### TOOLCHAIN_LIBS = $(TOOLCHAIN_LIBS)"
	@cmd /C "@echo ### CFLAGS = $(CFLAGS)"
	@cmd /C "@echo ### LDFLAGS = $(LDFLAGS)"
	@cmd /C "@echo ### SHAREDLIB_LDFLAGS = $(SHAREDLIB_LDFLAGS)"
	@cmd /C "@echo ### CPPFLAGS = $(CPPFLAGS)"
	@cmd /C "@echo ### CPP_LDFLAGS = $(CPP_LDFLAGS)"
	@cmd /C "@echo ### CPP_SHAREDLIB_LDFLAGS = $(CPP_SHAREDLIB_LDFLAGS)"
	@cmd /C "@echo ### ARFLAGS = $(ARFLAGS)"
	@cmd /C "@echo ### MEX_CFLAGS = $(MEX_CFLAGS)"
	@cmd /C "@echo ### MEX_CPPFLAGS = $(MEX_CPPFLAGS)"
	@cmd /C "@echo ### MEX_LDFLAGS = $(MEX_LDFLAGS)"
	@cmd /C "@echo ### MEX_CPPLDFLAGS = $(MEX_CPPLDFLAGS)"
	@cmd /C "@echo ### DOWNLOAD_FLAGS = $(DOWNLOAD_FLAGS)"
	@cmd /C "@echo ### EXECUTE_FLAGS = $(EXECUTE_FLAGS)"
	@cmd /C "@echo ### MAKE_FLAGS = $(MAKE_FLAGS)"


clean : 
	$(ECHO) "### Deleting all derived files..."
	@if exist $(PRODUCT) $(RM) $(PRODUCT)
	$(RM) $(ALL_OBJS)
	$(ECHO) "### Deleted all derived files."


