# module for AliRootMini
# Author: Jan Musinsky
# Date:   30 Oct 2012

MODULE	= AliRootMini
MODDIR	= AliRootMini
FILES	= AliLog

LNKDEF	= $(MODDIR)/$(LNKFILE).$(HdrSuf)
SRCS	= $(patsubst %,$(MODDIR)/%.$(SrcSuf),$(FILES))
HDRS	= $(SRCS:.$(SrcSuf)=.$(HdrSuf))
DICT	= $(TMPDIR)/$(MODDIR)/$(DICTPREFIX)$(MODULE).$(SrcSuf)
DICTH	= $(DICT:.$(SrcSuf)=.$(HdrSuf))
DICTO	= $(DICT:.$(SrcSuf)=.$(ObjSuf))
OBJS	= $(patsubst %.$(SrcSuf),$(TMPDIR)/%.$(ObjSuf),$(SRCS))
MODLIB	= $(LIBDIR)/$(LIBPREFIX)$(MODULE).$(DllSuf)

ROOTMINICLEAN := $(OBJS) $(DICTO)
ifeq (distclean,$(findstring distclean,$(MAKECMDGOALS)))
ROOTMINICLEAN += $(DICT) $(DICTH) $(MODLIB)
endif

# used in the main Makefile
ALLLIBS	+= $(MODLIB)
ALLHDRS	+= $(HDRS)
ALLDIST	+= $(SRCS) $(HDRS) $(LNKDEF) $(MODDIR)/$(MODMAKEFILE)
ALLDEPEND += $(SRCS)

$(MODDIR):	$(MODLIB)

$(MODLIB):	$(OBJS) $(DICTO)

$(DICT):	$(HDRS) $(LNKDEF)

clean-$(MODDIR):
		@rm -fv $(ROOTMINICLEAN)

clean:		clean-$(MODDIR)

distclean-$(MODDIR):
		@rm -fv $(ROOTMINICLEAN)

distclean:	distclean-$(MODDIR)
