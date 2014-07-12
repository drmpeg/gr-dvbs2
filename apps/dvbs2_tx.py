#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Dvbs2 Tx
# Generated: Sat Jul 12 16:06:55 2014
##################################################

from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio import wxgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.wxgui import fftsink2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import dvbs2
import osmosdr
import wx

class dvbs2_tx(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Dvbs2 Tx")

        ##################################################
        # Variables
        ##################################################
        self.symbol_rate = symbol_rate = 5000000
        self.taps = taps = 50
        self.samp_rate = samp_rate = symbol_rate * 2
        self.rolloff = rolloff = 0.2

        ##################################################
        # Blocks
        ##################################################
        self.wxgui_fftsink2_0 = fftsink2.fft_sink_c(
        	self.GetWin(),
        	baseband_freq=1280000000,
        	y_per_div=10,
        	y_divs=10,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate,
        	fft_size=1024,
        	fft_rate=15,
        	average=True,
        	avg_alpha=0.13333,
        	title="FFT Plot",
        	peak_hold=False,
        )
        self.Add(self.wxgui_fftsink2_0.win)
        self.osmosdr_sink_0 = osmosdr.sink( args="numchan=" + str(1) + " " + "bladerf=0,buffers=128,buflen=32768" )
        self.osmosdr_sink_0.set_sample_rate(samp_rate)
        self.osmosdr_sink_0.set_center_freq(1280e6, 0)
        self.osmosdr_sink_0.set_freq_corr(0, 0)
        self.osmosdr_sink_0.set_gain(15, 0)
        self.osmosdr_sink_0.set_if_gain(0, 0)
        self.osmosdr_sink_0.set_bb_gain(-10, 0)
        self.osmosdr_sink_0.set_antenna("", 0)
        self.osmosdr_sink_0.set_bandwidth(6000000, 0)
          
        self.fft_filter_xxx_0 = filter.fft_filter_ccc(1, (firdes.root_raised_cosine(1, samp_rate, samp_rate/2, rolloff, taps)), 1)
        self.fft_filter_xxx_0.declare_sample_delay(0)
        self.dvbs2_physical_cc_0 = dvbs2.physical_cc(dvbs2.MOD_16APSK, dvbs2.C9_10, dvbs2.PILOTS_ON)
        self.dvbs2_modulator_bc_0 = dvbs2.modulator_bc(dvbs2.MOD_16APSK, dvbs2.C9_10)
        self.dvbs2_ldpc_bb_0 = dvbs2.ldpc_bb(dvbs2.C9_10)
        self.dvbs2_interleaver_bb_0 = dvbs2.interleaver_bb(dvbs2.MOD_16APSK, dvbs2.C_OTHER)
        self.dvbs2_bch_bb_0 = dvbs2.bch_bb(dvbs2.C9_10)
        self.dvbs2_bbscrambler_bb_0 = dvbs2.bbscrambler_bb(dvbs2.C9_10)
        self.dvbs2_bbheader_bb_0 = dvbs2.bbheader_bb(dvbs2.C9_10,dvbs2.RO_0_20)
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_char*1, "/run/shm/adv16apsk910.ts", True)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_file_source_0, 0), (self.dvbs2_bbheader_bb_0, 0))
        self.connect((self.dvbs2_bbheader_bb_0, 0), (self.dvbs2_bbscrambler_bb_0, 0))
        self.connect((self.dvbs2_bbscrambler_bb_0, 0), (self.dvbs2_bch_bb_0, 0))
        self.connect((self.dvbs2_bch_bb_0, 0), (self.dvbs2_ldpc_bb_0, 0))
        self.connect((self.dvbs2_ldpc_bb_0, 0), (self.dvbs2_interleaver_bb_0, 0))
        self.connect((self.dvbs2_interleaver_bb_0, 0), (self.dvbs2_modulator_bc_0, 0))
        self.connect((self.dvbs2_modulator_bc_0, 0), (self.dvbs2_physical_cc_0, 0))
        self.connect((self.dvbs2_physical_cc_0, 0), (self.fft_filter_xxx_0, 0))
        self.connect((self.fft_filter_xxx_0, 0), (self.osmosdr_sink_0, 0))
        self.connect((self.fft_filter_xxx_0, 0), (self.wxgui_fftsink2_0, 0))


# QT sink close method reimplementation

    def get_symbol_rate(self):
        return self.symbol_rate

    def set_symbol_rate(self, symbol_rate):
        self.symbol_rate = symbol_rate
        self.set_samp_rate(self.symbol_rate * 2)

    def get_taps(self):
        return self.taps

    def set_taps(self, taps):
        self.taps = taps
        self.fft_filter_xxx_0.set_taps((firdes.root_raised_cosine(1, self.samp_rate, self.samp_rate/2, self.rolloff, self.taps)))

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.fft_filter_xxx_0.set_taps((firdes.root_raised_cosine(1, self.samp_rate, self.samp_rate/2, self.rolloff, self.taps)))
        self.osmosdr_sink_0.set_sample_rate(self.samp_rate)
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate)

    def get_rolloff(self):
        return self.rolloff

    def set_rolloff(self, rolloff):
        self.rolloff = rolloff
        self.fft_filter_xxx_0.set_taps((firdes.root_raised_cosine(1, self.samp_rate, self.samp_rate/2, self.rolloff, self.taps)))

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    tb = dvbs2_tx()
    tb.Start(True)
    tb.Wait()

