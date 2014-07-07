/* -*- c++ -*- */

#define DVBS2_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "dvbs2_swig_doc.i"

%{
#include "dvbs2/dvbs2_config.h"
#include "dvbs2/bbheader_bb.h"
#include "dvbs2/bbscrambler_bb.h"
#include "dvbs2/bch_bb.h"
#include "dvbs2/ldpc_bb.h"
#include "dvbs2/modulator_bc.h"
#include "dvbs2/physical_cc.h"
#include "dvbs2/interleaver_bb.h"
%}


%include "dvbs2/dvbs2_config.h"
%include "dvbs2/bbheader_bb.h"
GR_SWIG_BLOCK_MAGIC2(dvbs2, bbheader_bb);
%include "dvbs2/bbscrambler_bb.h"
GR_SWIG_BLOCK_MAGIC2(dvbs2, bbscrambler_bb);
%include "dvbs2/bch_bb.h"
GR_SWIG_BLOCK_MAGIC2(dvbs2, bch_bb);
%include "dvbs2/ldpc_bb.h"
GR_SWIG_BLOCK_MAGIC2(dvbs2, ldpc_bb);
%include "dvbs2/modulator_bc.h"
GR_SWIG_BLOCK_MAGIC2(dvbs2, modulator_bc);
%include "dvbs2/physical_cc.h"
GR_SWIG_BLOCK_MAGIC2(dvbs2, physical_cc);
%include "dvbs2/interleaver_bb.h"
GR_SWIG_BLOCK_MAGIC2(dvbs2, interleaver_bb);
