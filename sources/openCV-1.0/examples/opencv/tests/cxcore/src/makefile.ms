TARGET = cxcoretest
BINTYPE = CONSOLE
VER =
ROOT = ..\..\..
PCH = cxcoretest.h
PCH_STARTER = precomp

OBJS = $(OBJPATH)/aarithm.obj $(OBJPATH)/adatastruct.obj $(OBJPATH)/adxt.obj \
    $(OBJPATH)/amath.obj $(OBJPATH)/cxcoretest_main.obj

INCS = cxcoretest.h ../../cxts/cxts.h \
    $(ROOT)/cxcore/include/cxcore.h $(ROOT)/cxcore/include/cxcore.hpp \
    $(ROOT)/cxcore/include/cxerror.h $(ROOT)/cxcore/include/cxmisc.h \
    $(ROOT)/cxcore/include/cxtypes.h $(ROOT)/cxcore/include/cvver.h

CXXFLAGS_PROJ = /I"." /I"../../cxts" /I"$(ROOT)/cxcore/include"
LIBS_PROJ = $(LIBPATH)"$(ROOT)/lib" cxcore$(DBG)$(OUT_P_SUFFIX).lib cxts$(DBG)$(OUT_P_SUFFIX).lib

!include $(ROOT)/_make/make_module_$(MS).mak
