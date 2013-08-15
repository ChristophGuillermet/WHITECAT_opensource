TARGET := cvtest
VER := 
BINTYPE := APP
SRC_ROOT := ../../tests/cv/src
INC_ROOT := ../../tests/cv/src
CXCORE_INC := ../../cxcore/include
CV_INC := ../../cv/include
CVAUX_INC := ../../cvaux/include
HIGHGUI_INC := ../../otherlibs/highgui
CXTS_INC := ../../tests/cxts
SRC_DIRS := . ../../../cxcore/include ../../../cv/include ../../../cvaux/include

CXXFLAGS := -D"CVAPI_EXPORTS" -I"$(INC_ROOT)" -I"$(CXCORE_INC)" -I"$(CXTS_INC)" \
   -I"$(CV_INC)" -I"$(CVAUX_INC)" -I"$(HIGHGUI_INC)"

INCS := cvtest.h $(CXTS_INC)/cxts.h $(HIGHGUI_INC)/highgui.h \
   cxcore.h cxcore.hpp cxerror.h cxmisc.h cxtypes.h cvver.h \
   cvaux.h cvaux.hpp cvmat.hpp cvvidsurv.hpp \
   cv.h cv.hpp cvcompat.h cvtypes.h

LIBS := -lcxcore$(DBG) -lcxts$(DBG) -lcv$(DBG) -lcvaux$(DBG) -lhighgui$(DBG)

include ../../_make/make_module_gnu.mak
