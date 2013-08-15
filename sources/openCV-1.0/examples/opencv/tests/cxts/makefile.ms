TARGET = cxts
BINTYPE = DLL
VER = 001
ROOT = ..\..
PCH = _cxts.h
PCH_STARTER = precomp

OBJS = $(OBJPATH)/cxts.obj $(OBJPATH)/cxts_arrtest.obj $(OBJPATH)/cxts_math.obj

INCS = ./cxts.h ./_cxts.h \
    $(ROOT)/cxcore/include/cxcore.h $(ROOT)/cxcore/include/cxcore.hpp \
    $(ROOT)/cxcore/include/cxerror.h $(ROOT)/cxcore/include/cxmisc.h \
    $(ROOT)/cxcore/include/cxtypes.h $(ROOT)/cxcore/include/cvver.h

CXXFLAGS_PROJ = /I"." /I"$(ROOT)/cxcore/include" /EHa
LIBS_PROJ = $(LIBPATH)"$(ROOT)/lib" cxcore$(DBG)$(OUT_P_SUFFIX).lib

!include $(ROOT)/_make/make_module_$(MS).mak
