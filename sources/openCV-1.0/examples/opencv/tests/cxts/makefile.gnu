TARGET := cxts
VER := 001
BINTYPE := DLL
SRC_ROOT := ../../tests/cxts
INC_ROOT := ../../tests/cxts
CXCORE_INC := ../../cxcore/include
SRC_DIRS := . ../../cxcore/include

CXXFLAGS := -D"CVAPI_EXPORTS" -I"$(INC_ROOT)" -I"$(CXCORE_INC)"

INCS := cxts.h _cxts.h \
   cxcore.h cxcore.hpp cxerror.h cxmisc.h cxtypes.h cvver.h

LIBS := -lcxcore$(DBG)

include ../../_make/make_module_gnu.mak
