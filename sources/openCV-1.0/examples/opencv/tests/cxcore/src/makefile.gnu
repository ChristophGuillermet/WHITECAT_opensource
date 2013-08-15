TARGET := cxcoretest
VER := 
BINTYPE := APP
SRC_ROOT := ../../tests/cxcore/src
INC_ROOT := ../../tests/cxcore/src
CXCORE_INC := ../../cxcore/include
CXTS_INC := ../../tests/cxts
SRC_DIRS := . ../../../cxcore/include

CXXFLAGS := -D"CVAPI_EXPORTS" -I"$(INC_ROOT)" -I"$(CXCORE_INC)" -I"$(CXTS_INC)"

INCS := cxcoretest.h $(CXTS_INC)/cxts.h \
   cxcore.h cxcore.hpp cxerror.h cxmisc.h cxtypes.h cvver.h

LIBS := -lcxcore$(DBG) -lcxts$(DBG)

include ../../_make/make_module_gnu.mak
