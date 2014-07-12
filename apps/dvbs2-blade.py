#!/usr/bin/env /usr/bin/python

# Copyright 2014 Ron Economos (w6rz@comcast.com)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

from gnuradio import blocks
from gnuradio import filter
from gnuradio import gr
from gnuradio.filter import firdes
import dvbs2
import osmosdr
import sys

def main(args):
    nargs = len(args)
    if nargs == 1:
        infile = args[0]
        outfile = None
    elif nargs == 2:
        infile = args[0]
        outfile = args[1]
    else:
        sys.stderr.write("Usage: dvbs2-blade.py input_file [output_file]\n");
        sys.exit(1)

    symbol_rate = 5000000
    samp_rate = symbol_rate * 2
    constellation = dvbs2.MOD_16APSK
    code_rate = dvbs2.C9_10
    pilots = dvbs2.PILOTS_ON
    rolloff = dvbs2.RO_0_20
    rrc_taps = 50
    center_freq = 1280000000
    txvga1_gain = -10
    txvga2_gain = 15
    bandwidth = 12000000

    if constellation == dvbs2.MOD_8PSK:
        if (   code_rate == dvbs2.C1_4
            or code_rate == dvbs2.C1_3
            or code_rate == dvbs2.C2_5
            or code_rate == dvbs2.C1_2
            or code_rate == dvbs2.C4_5):
            sys.stderr.write("Invalid code rate for 8PSK\n");
            sys.exit(1)

    if constellation == dvbs2.MOD_16APSK:
        if (   code_rate == dvbs2.C1_4
            or code_rate == dvbs2.C1_3
            or code_rate == dvbs2.C2_5
            or code_rate == dvbs2.C1_2
            or code_rate == dvbs2.C3_5):
            sys.stderr.write("Invalid code rate for 16APSK\n");
            sys.exit(1)

    if constellation == dvbs2.MOD_32APSK:
        if (   code_rate == dvbs2.C1_4
            or code_rate == dvbs2.C1_3
            or code_rate == dvbs2.C2_5
            or code_rate == dvbs2.C1_2
            or code_rate == dvbs2.C3_5
            or code_rate == dvbs2.C2_3):
            sys.stderr.write("Invalid code rate for 32APSK\n");
            sys.exit(1)

    if rolloff == dvbs2.RO_0_20:
        rrc_rolloff = 0.2
    elif rolloff == dvbs2.RO_0_25:
        rrc_rolloff = 0.25
    elif rolloff == dvbs2.RO_0_35:
        rrc_rolloff = 0.35

    if code_rate == dvbs2.C3_5:
        code_rate_interleaver = code_rate
    else:
        code_rate_interleaver = dvbs2.C_OTHER

    if code_rate == dvbs2.C1_4:
        code_rate_modulator = dvbs2.C_OTHER
    elif code_rate == dvbs2.C1_3:
        code_rate_modulator = dvbs2.C_OTHER
    elif code_rate == dvbs2.C2_5:
        code_rate_modulator = dvbs2.C_OTHER
    elif code_rate == dvbs2.C3_5:
        code_rate_modulator = dvbs2.C_OTHER
    else:
        code_rate_modulator = code_rate

    tb = gr.top_block()

    src = blocks.file_source(gr.sizeof_char, infile, True)

    dvbs2_bbheader = dvbs2.bbheader_bb(code_rate, rolloff)
    dvbs2_bbscrambler = dvbs2.bbscrambler_bb(code_rate)
    dvbs2_bch = dvbs2.bch_bb(code_rate)
    dvbs2_ldpc = dvbs2.ldpc_bb(code_rate)
    dvbs2_interleaver = dvbs2.interleaver_bb(constellation, code_rate_interleaver)
    dvbs2_modulator = dvbs2.modulator_bc(constellation, code_rate_modulator)
    dvbs2_physical = dvbs2.physical_cc(constellation, code_rate, pilots)

    fft_filter = filter.fft_filter_ccc(1, (firdes.root_raised_cosine(1, samp_rate, samp_rate/2, rrc_rolloff, rrc_taps)), 1)
    fft_filter.declare_sample_delay(0)

    out = osmosdr.sink(args="bladerf=0,buffers=128,buflen=32768")
    out.set_sample_rate(samp_rate)
    out.set_center_freq(center_freq, 0)
    out.set_freq_corr(0, 0)
    out.set_gain(txvga2_gain, 0)
    out.set_bb_gain(txvga1_gain, 0)
    out.set_bandwidth(bandwidth, 0)

    tb.connect(src, dvbs2_bbheader)
    tb.connect(dvbs2_bbheader, dvbs2_bbscrambler)
    tb.connect(dvbs2_bbscrambler, dvbs2_bch)
    tb.connect(dvbs2_bch, dvbs2_ldpc)
    tb.connect(dvbs2_ldpc, dvbs2_interleaver)
    tb.connect(dvbs2_interleaver, dvbs2_modulator)
    tb.connect(dvbs2_modulator, dvbs2_physical)
    tb.connect(dvbs2_physical, fft_filter)
    tb.connect(fft_filter, out)

    if outfile:
        dst = blocks.file_sink(gr.sizeof_gr_complex, outfile)
        tb.connect(fft_filter, dst)

    tb.run()


if __name__ == '__main__':
    main(sys.argv[1:])

