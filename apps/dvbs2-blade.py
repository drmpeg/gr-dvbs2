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
    frame_size = dvbs2.FECFRAME_NORMAL
    constellation = dvbs2.MOD_16APSK
    code_rate = dvbs2.C9_10
    pilots = dvbs2.PILOTS_ON
    rolloff = dvbs2.RO_0_20
    gold_code = 0
    rrc_taps = 50
    center_freq = 1280000000
    txvga1_gain = -10
    txvga2_gain = 15
    bandwidth = 6000000

    if constellation == dvbs2.MOD_QPSK:
        if (   code_rate == dvbs2.C1_4
            or code_rate == dvbs2.C1_3
            or code_rate == dvbs2.C2_5
            or code_rate == dvbs2.C1_2
            or code_rate == dvbs2.C3_5
            or code_rate == dvbs2.C2_3
            or code_rate == dvbs2.C3_4
            or code_rate == dvbs2.C4_5
            or code_rate == dvbs2.C5_6
            or code_rate == dvbs2.C8_9
            or code_rate == dvbs2.C9_10
            or code_rate == dvbs2.C13_45
            or code_rate == dvbs2.C9_20
            or code_rate == dvbs2.C11_20
            or code_rate == dvbs2.C11_45
            or code_rate == dvbs2.C4_15
            or code_rate == dvbs2.C14_45
            or code_rate == dvbs2.C7_15
            or code_rate == dvbs2.C8_15
            or code_rate == dvbs2.C32_45):
            pass
        else:
            sys.stderr.write("Invalid code rate for QPSK\n");
            sys.exit(1)

    if constellation == dvbs2.MOD_8PSK:
        if (   code_rate == dvbs2.C3_5
            or code_rate == dvbs2.C2_3
            or code_rate == dvbs2.C3_4
            or code_rate == dvbs2.C5_6
            or code_rate == dvbs2.C8_9
            or code_rate == dvbs2.C9_10
            or code_rate == dvbs2.C23_36
            or code_rate == dvbs2.C25_36
            or code_rate == dvbs2.C13_18
            or code_rate == dvbs2.C7_15
            or code_rate == dvbs2.C8_15
            or code_rate == dvbs2.C26_45
            or code_rate == dvbs2.C32_45):
            pass
        else:
            sys.stderr.write("Invalid code rate for 8PSK\n");
            sys.exit(1)

    if constellation == dvbs2.MOD_8APSK:
        if (   code_rate == dvbs2.C100_180
            or code_rate == dvbs2.C104_180):
            pass
        else:
            sys.stderr.write("Invalid code rate for 8APSK\n");
            sys.exit(1)

    if constellation == dvbs2.MOD_16APSK:
        if (   code_rate == dvbs2.C2_3
            or code_rate == dvbs2.C3_4
            or code_rate == dvbs2.C4_5
            or code_rate == dvbs2.C5_6
            or code_rate == dvbs2.C8_9
            or code_rate == dvbs2.C9_10
            or code_rate == dvbs2.C26_45
            or code_rate == dvbs2.C3_5
            or code_rate == dvbs2.C28_45
            or code_rate == dvbs2.C23_36
            or code_rate == dvbs2.C25_36
            or code_rate == dvbs2.C13_18
            or code_rate == dvbs2.C140_180
            or code_rate == dvbs2.C154_180
            or code_rate == dvbs2.C7_15
            or code_rate == dvbs2.C8_15
            or code_rate == dvbs2.C26_45
            or code_rate == dvbs2.C3_5
            or code_rate == dvbs2.C32_45):
            pass
        else:
            sys.stderr.write("Invalid code rate for 16APSK\n");
            sys.exit(1)

    if constellation == dvbs2.MOD_8_8APSK:
        if (   code_rate == dvbs2.C90_180
            or code_rate == dvbs2.C96_180
            or code_rate == dvbs2.C100_180
            or code_rate == dvbs2.C18_30
            or code_rate == dvbs2.C20_30):
            pass
        else:
            sys.stderr.write("Invalid code rate for 8+8APSK\n");
            sys.exit(1)

    if constellation == dvbs2.MOD_32APSK:
        if (   code_rate == dvbs2.C3_4
            or code_rate == dvbs2.C4_5
            or code_rate == dvbs2.C5_6
            or code_rate == dvbs2.C8_9
            or code_rate == dvbs2.C9_10):
            pass
        else:
            sys.stderr.write("Invalid code rate for 32APSK\n");
            sys.exit(1)

    if constellation == dvbs2.MOD_4_12_16APSK:
        if (   code_rate == dvbs2.C2_3
            or code_rate == dvbs2.C32_45):
            pass
        else:
            sys.stderr.write("Invalid code rate for 4+12+16rbAPSK\n");
            sys.exit(1)

    if constellation == dvbs2.MOD_4_8_4_16APSK:
        if (   code_rate == dvbs2.C128_180
            or code_rate == dvbs2.C132_180
            or code_rate == dvbs2.C140_180):
            pass
        else:
            sys.stderr.write("Invalid code rate for 4+8+4+16APSK\n");
            sys.exit(1)

    if constellation == dvbs2.MOD_64APSK:
        if (   code_rate == dvbs2.C128_180):
            pass
        else:
            sys.stderr.write("Invalid code rate for 64APSK\n");
            sys.exit(1)

    if constellation == dvbs2.MOD_4_12_20_28APSK:
        if (   code_rate == dvbs2.C132_180):
            pass
        else:
            sys.stderr.write("Invalid code rate for 4+12+20+28APSK\n");
            sys.exit(1)

    if constellation == dvbs2.MOD_8_16_20_20APSK:
        if (   code_rate == dvbs2.C7_9
            or code_rate == dvbs2.C4_5
            or code_rate == dvbs2.C5_6):
            pass
        else:
            sys.stderr.write("Invalid code rate for 8+16+20+20APSK\n");
            sys.exit(1)

    if constellation == dvbs2.MOD_128APSK:
        if (   code_rate == dvbs2.C135_180
            or code_rate == dvbs2.C140_180):
            pass
        else:
            sys.stderr.write("Invalid code rate for 128APSK\n");
            sys.exit(1)

    if constellation == dvbs2.MOD_256APSK:
        if (   code_rate == dvbs2.C20_30
            or code_rate == dvbs2.C22_30
            or code_rate == dvbs2.C116_180
            or code_rate == dvbs2.C124_180
            or code_rate == dvbs2.C128_180
            or code_rate == dvbs2.C135_180):
            pass
        else:
            sys.stderr.write("Invalid code rate for 256APSK\n");
            sys.exit(1)

    if rolloff == dvbs2.RO_0_05:
        rrc_rolloff = 0.05
    elif rolloff == dvbs2.RO_0_10:
        rrc_rolloff = 0.10
    elif rolloff == dvbs2.RO_0_15:
        rrc_rolloff = 0.15
    elif rolloff == dvbs2.RO_0_20:
        rrc_rolloff = 0.20
    elif rolloff == dvbs2.RO_0_25:
        rrc_rolloff = 0.25
    elif rolloff == dvbs2.RO_0_35:
        rrc_rolloff = 0.35

    if (   code_rate == dvbs2.C3_5
        or code_rate == dvbs2.C4_5
        or code_rate == dvbs2.C5_6
        or code_rate == dvbs2.C7_9
        or code_rate == dvbs2.C13_18
        or code_rate == dvbs2.C23_36
        or code_rate == dvbs2.C25_36
        or code_rate == dvbs2.C26_45
        or code_rate == dvbs2.C28_45
        or code_rate == dvbs2.C90_180
        or code_rate == dvbs2.C96_180
        or code_rate == dvbs2.C100_180
        or code_rate == dvbs2.C116_180
        or code_rate == dvbs2.C124_180
        or code_rate == dvbs2.C128_180
        or code_rate == dvbs2.C135_180
        or code_rate == dvbs2.C140_180):
        code_rate_interleaver = code_rate
    else:
        code_rate_interleaver = dvbs2.C_OTHER

    if (   code_rate == dvbs2.C2_3
        or code_rate == dvbs2.C3_4
        or code_rate == dvbs2.C4_5
        or code_rate == dvbs2.C5_6
        or code_rate == dvbs2.C8_9
        or code_rate == dvbs2.C9_10
        or code_rate == dvbs2.C90_180
        or code_rate == dvbs2.C96_180
        or code_rate == dvbs2.C100_180
        or code_rate == dvbs2.C104_180
        or code_rate == dvbs2.C26_45
        or code_rate == dvbs2.C18_30
        or code_rate == dvbs2.C28_45
        or code_rate == dvbs2.C23_36
        or code_rate == dvbs2.C116_180
        or code_rate == dvbs2.C20_30
        or code_rate == dvbs2.C124_180
        or code_rate == dvbs2.C25_36
        or code_rate == dvbs2.C128_180
        or code_rate == dvbs2.C13_18
        or code_rate == dvbs2.C132_180
        or code_rate == dvbs2.C22_30
        or code_rate == dvbs2.C135_180
        or code_rate == dvbs2.C140_180
        or code_rate == dvbs2.C7_9
        or code_rate == dvbs2.C154_180):
        code_rate_modulator = code_rate
    else:
        code_rate_modulator = dvbs2.C_OTHER

    if constellation == dvbs2.MOD_128APSK:
        constellation_ldpc = constellation
    else:
        constellation_ldpc = dvbs2.MOD_OTHER

    tb = gr.top_block()

    src = blocks.file_source(gr.sizeof_char, infile, True)

    dvbs2_bbheader = dvbs2.bbheader_bb(code_rate, rolloff, frame_size)
    dvbs2_bbscrambler = dvbs2.bbscrambler_bb(code_rate, frame_size)
    dvbs2_bch = dvbs2.bch_bb(code_rate, frame_size)
    dvbs2_ldpc = dvbs2.ldpc_bb(code_rate, frame_size, constellation)
    dvbs2_interleaver = dvbs2.interleaver_bb(constellation, code_rate_interleaver, frame_size)
    dvbs2_modulator = dvbs2.modulator_bc(constellation, code_rate_modulator, frame_size)
    dvbs2_physical = dvbs2.physical_cc(constellation, code_rate, pilots, frame_size, gold_code)

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

