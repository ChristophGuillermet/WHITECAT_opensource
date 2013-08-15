TARGET = cvtest
BINTYPE = CONSOLE
VER =
ROOT = ..\..\..
PCH = cvtest.h
PCH_STARTER = cvtest

OBJS = $(OBJPATH)/aaccum.obj $(OBJPATH)/aadaptthresh.obj   $(OBJPATH)/aapproxpoly.obj \
$(OBJPATH)/acameracalibration.obj $(OBJPATH)/acamshift.obj  $(OBJPATH)/acanny.obj \
$(OBJPATH)/achesscorners.obj  $(OBJPATH)/acolor.obj $(OBJPATH)/acondens.obj \
$(OBJPATH)/acontourmoments.obj  $(OBJPATH)/acontours.obj  $(OBJPATH)/acontoursmatch.obj \
$(OBJPATH)/aconvhull.obj  $(OBJPATH)/acreatecontourtree.obj $(OBJPATH)/adistancetransform.obj \
$(OBJPATH)/adrawing_regress.obj $(OBJPATH)/aeigenobjects.obj  $(OBJPATH)/aemd.obj \
$(OBJPATH)/afilter.obj  $(OBJPATH)/afloodfill.obj $(OBJPATH)/afundam.obj \
$(OBJPATH)/ahistograms.obj  $(OBJPATH)/ahmmobs.obj  $(OBJPATH)/ahoughtransform.obj \
$(OBJPATH)/aimage.obj $(OBJPATH)/aimgwarp.obj   $(OBJPATH)/akalman.obj \
$(OBJPATH)/akmeans.obj  $(OBJPATH)/amatchcontourtrees.obj $(OBJPATH)/amoments.obj \
$(OBJPATH)/amotiontemplates.obj $(OBJPATH)/amotseg.obj  $(OBJPATH)/aoptflowhs.obj \
$(OBJPATH)/aoptflowlk.obj $(OBJPATH)/aoptflowpyrlk.obj  $(OBJPATH)/aposit.obj \
$(OBJPATH)/apyrsegmentation.obj $(OBJPATH)/asnakes.obj  $(OBJPATH)/asubdivisions.obj \
$(OBJPATH)/atemplmatch.obj $(OBJPATH)/athresh.obj \
$(OBJPATH)/tsysa.obj  

INCS = cvtest.h ../../cxts/cxts.h $(ROOT)/otherlibs/highgui/highgui.h \
  $(ROOT)/cv/include/cv.h $(ROOT)/cv/include/cv.hpp \
  $(ROOT)/cv/include/cvcompat.h $(ROOT)/cv/include/cvtypes.h \
  $(ROOT)/cvaux/include/cvaux.h $(ROOT)/cvaux/include/cvaux.hpp \
  $(ROOT)/cvaux/include/cvmat.hpp $(ROOT)/cvaux/include/cvvidsurv.hpp \
  $(ROOT)/cxcore/include/cxcore.h $(ROOT)/cxcore/include/cxcore.hpp \
  $(ROOT)/cxcore/include/cxerror.h $(ROOT)/cxcore/include/cxmisc.h \
  $(ROOT)/cxcore/include/cxtypes.h $(ROOT)/cxcore/include/cvver.h \

CXXFLAGS_PROJ = /I"." /I"../../cxts" /I"$(ROOT)/cxcore/include" \
  /I"$(ROOT)/cv/include" /I"$(ROOT)/cvaux/include" /I"$(ROOT)/otherlibs/highgui"

LIBS_PROJ = $(LIBPATH)"$(ROOT)/lib" cxcore$(DBG)$(OUT_P_SUFFIX).lib \
  cv$(DBG)$(OUT_P_SUFFIX).lib cvaux$(DBG)$(OUT_P_SUFFIX).lib \
  cxts$(DBG)$(OUT_P_SUFFIX).lib highgui$(DBG)$(OUT_P_SUFFIX).lib

!include $(ROOT)/_make/make_module_$(MS).mak
