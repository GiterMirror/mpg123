#############################################################################
#                                                                           #
#              Copyright 2000  by Microware Systems Corporation             #
#                            All Rights Reserved                            #
#                          Reproduced Under License                         #
#                                                                           #
#  This software is confidential property of Microware Systems Corporation, #
#  and is provided under license for internal development purposes only.    #
#  Reproduction, publication, distribution, or creation of derivative works #
#  in any form to any party other than the licensee is strictly prohibited, #
#  unless expressly authorized in writing by Microware Systems Corporation. #
#                                                                           #
#############################################################################
#                                                                           #
# Edition History:                                                          #
# #   Date    Comments                                                By    #
# -- -------- ------------------------------------------------------- ---   #
# 01 00/10/23 Created.                                                GbG   #
# 02 00/10/24 Added fp option for arm4.                               GbG   #
# 03 00/10/25 Added dependency on mpgmp3 libs.                        GbG   #
# 04 00/10/25 Changed to ICODE linking.                               GbG   #
#############################################################################
# Makefile to produce MP3 player.                                           #
#############################################################################
#                                                                           #

MWOS            =       ../../../../..
TRGTS           =       mpgplay

include $(MWOS)/MAKETMPL/make.com

RDIR            =       IDIR/$(TARGET)

SDIR            =       .
ODIR            =       $(MWOS_ODIR)/MAUIDEMO

DEFS            =       $(MWOS_DEFS) -v=../DEFS

MAKERS          =       ./makefile

CRELS           =       $(RDIR)/$(TRGTS).i

RELS            =       $(CRELS) 

if $(TARGET) == "arm4"
L_TPOPT         =       -tp=$(CPU)$(COPT_LCD)$(COPT_LD)$(COPT_LC),fp
L_FPULIB        =       -l=$(MWOS_DFTLIB)/fpl.l
else
L_TPOPT         =       -tp=$(CPU)$(COPT_LCD)$(COPT_LD)$(COPT_LC)
L_FPULIB        =
endif


LIBS            =       -ill=$(MWOS_ILIBDIR)/mfm.il \
                        -ill=$(MWOS_ILIBDIR)/mpglib.il \
						$(SYS_CLIBS) $(L_FPULIB) \
                        $(L_TPOPT)


SPEC_COPTS      =	

CEND            =       -eio=$(RDIR) 


COMPILE         =       $(CC) $(CPARMS) -r -t=7 $(CEND) $(DEFS) \
                        $(SPEC_COPTS) $(COPTS) $(COPT_CODEADDR)

SPEC_LOPTS      =       -olms

LINK            =       $(LINKER) -olg -olM=60k \
                        $(LPARMS) $(LIBS) $(SPEC_LOPTS) $(LOPTS)

notarget:	nulltrg
	$(COMMENT)

$(TRGTS):	_makdir $(ODIR)/$(TRGTS)
	$(COMMENT)

$(ODIR)/$(TRGTS):	$(RELS)
	$(CODO) $@
	-$(DEL) $@
	$(CODO) $(ODIR)/STB/$(TRGTS).stb
	-$(DEL) $@.stb
	$(CODO) $(ODIR)/STB/$(TRGTS).map
	-$(DEL) $@.map
	$(LINK) $(RELS) -fd=$@ \
	$(REDIR_OUT)$(ODIR)/STB/$(TRGTS).map

$(CRELS):	$(MAKERS) $(MWOS_DFTLIB)/mpglib.l $(MWOS_ILIBDIR)/mpglib.il 
	$(COMPILE) $(SDIR)/$*.c

$(RDIR)/$(TRGTS).i: $(SDIR)/$(TRGTS).c

_makdir: nulltrg
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)
	$(MAKDIR) $(ODIR)/STB
	$(COMMENT)

_clean:  nulltrg
	-$(DEL) $(RDIR)/*.i
	$(COMMENT)

_purge:  nulltrg
	$(CODO) $(ODIR)/$(TRGTS)
	-$(DEL) $(ODIR)/$(TRGTS)
	$(CODO) $(ODIR)/STB/$(TRGTS).map
	-$(DEL) $(ODIR)/STB/$(TRGTS).map
	$(CODO) $(ODIR)/STB/$(TRGTS).stb
	-$(DEL) $(ODIR)/STB/$(TRGTS).stb
	$(COMMENT)

nulltrg:  .
	$(COMMENT)

#                                                                           #
#############################################################################

