# module for AliRsnOutManager
# Author: Jan Musinsky
# Date:   12 Mar 2013

MODULE	= AliRsnOutManager
MODDIR	= AliRsnOutManager
FILES	= AliRsnOutManager AliRsnOutAxis AliRsnOutFragment AliRsnOutGroup \
          AliROMGraphErrors AliROMFragment AliROMGroup

LNKDEF	= $(MODDIR)/$(LNKFILE).$(HdrSuf)
SRCS	= $(patsubst %,$(MODDIR)/%.$(SrcSuf),$(FILES))
HDRS	= $(SRCS:.$(SrcSuf)=.$(HdrSuf))
DICT	= $(TMPDIR)/$(MODDIR)/$(DICTPREFIX)$(MODULE).$(SrcSuf)
DICTH	= $(DICT:.$(SrcSuf)=.$(HdrSuf))
DICTO	= $(DICT:.$(SrcSuf)=.$(ObjSuf))
OBJS	= $(patsubst %.$(SrcSuf),$(TMPDIR)/%.$(ObjSuf),$(SRCS))
MODLIB	= $(LIBDIR)/$(LIBPREFIX)$(MODULE).$(DllSuf)

RSNOUTMANAGERCLEAN := $(OBJS) $(DICTO)
ifeq (distclean,$(findstring distclean,$(MAKECMDGOALS)))
RSNOUTMANAGERCLEAN += $(DICT) $(DICTH) $(MODLIB)
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
		@rm -fv $(RSNOUTMANAGERCLEAN)

clean:		clean-$(MODDIR)

distclean-$(MODDIR):
		@rm -fv $(RSNOUTMANAGERCLEAN)

distclean:	distclean-$(MODDIR)
