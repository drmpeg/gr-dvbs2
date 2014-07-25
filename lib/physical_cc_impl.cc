/* -*- c++ -*- */
/* 
 * Copyright 2014 Ron Economos.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "physical_cc_impl.h"

namespace gr {
  namespace dvbs2 {

    physical_cc::sptr
    physical_cc::make(dvbs2_constellation_t constellation, dvbs2_code_rate_t rate, dvbs2_pilots_t pilots)
    {
      return gnuradio::get_initial_sptr
        (new physical_cc_impl(constellation, rate, pilots));
    }

    /*
     * The private constructor
     */
    physical_cc_impl::physical_cc_impl(dvbs2_constellation_t constellation, dvbs2_code_rate_t rate, dvbs2_pilots_t pilots)
      : gr::block("physical_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
        int type, modcod;
        double r0 = 1.0;

        modcod = 0;
        type = 0;

        pilot_mode = pilots;
        if (pilot_mode) type |= 1;

        m_bpsk[0][0].real() = (r0 * cos(M_PI / 4.0));
        m_bpsk[0][0].imag() = (r0 * sin(M_PI / 4.0));
        m_bpsk[0][1].real() = (r0 * cos(5.0 * M_PI / 4.0));
        m_bpsk[0][1].imag() = (r0 * sin(5.0 * M_PI / 4.0));
        m_bpsk[1][0].real() = (r0 * cos(5.0 * M_PI / 4.0));
        m_bpsk[1][0].imag() = (r0 * sin(M_PI / 4.0));
        m_bpsk[1][1].real() = (r0 * cos(M_PI / 4.0));
        m_bpsk[1][1].imag() = (r0 * sin(5.0 * M_PI /4.0));

        m_zero[0].real() = 0.0;    /* used for zero stuffing interpolation */
        m_zero[0].imag() = 0.0;

        // Mode and code rate
        if (constellation == gr::dvbs2::MOD_QPSK)
        {
            slots = (FRAME_SIZE_NORMAL / 2) / 90;
            pilot_symbols = (slots / 16) * 36;
            if (!(slots % 16)) pilot_symbols -= 36;
            switch (rate)
            {
                case gr::dvbs2::C1_4:
                    modcod = 1;
                    break;
                case gr::dvbs2::C1_3:
                    modcod = 2;
                    break;
                case gr::dvbs2::C2_5:
                    modcod = 3;
                    break;
                case gr::dvbs2::C1_2:
                    modcod = 4;
                    break;
                case gr::dvbs2::C3_5:
                    modcod = 5;
                    break;
                case gr::dvbs2::C2_3:
                    modcod = 6;
                    break;
                case gr::dvbs2::C3_4:
                    modcod = 7;
                    break;
                case gr::dvbs2::C4_5:
                    modcod = 8;
                    break;
                case gr::dvbs2::C5_6:
                    modcod = 9;
                    break;
                case gr::dvbs2::C8_9:
                    modcod = 10;
                    break;
                case gr::dvbs2::C9_10:
                    modcod = 11;
                    break;
                case gr::dvbs2::C13_45:
                    modcod = 132;
                    break;
                case gr::dvbs2::C9_20:
                    modcod = 134;
                    break;
                case gr::dvbs2::C11_20:
                    modcod = 136;
                    break;
                default:
                    modcod = 0;
                    break;
            }
        }

        if (constellation == gr::dvbs2::MOD_8PSK)
        {
            slots = (FRAME_SIZE_NORMAL / 3) / 90;
            pilot_symbols = (slots / 16) * 36;
            if (!(slots % 16)) pilot_symbols -= 36;
            switch (rate)
            {
                case gr::dvbs2::C3_5:
                    modcod = 12;
                    break;
                case gr::dvbs2::C2_3:
                    modcod = 13;
                    break;
                case gr::dvbs2::C3_4:
                    modcod = 14;
                    break;
                case gr::dvbs2::C5_6:
                    modcod = 15;
                    break;
                case gr::dvbs2::C8_9:
                    modcod = 16;
                    break;
                case gr::dvbs2::C9_10:
                    modcod = 17;
                    break;
                case gr::dvbs2::C23_36:
                    modcod = 142;
                    break;
                case gr::dvbs2::C25_36:
                    modcod = 144;
                    break;
                case gr::dvbs2::C13_18:
                    modcod = 146;
                    break;
                default:
                    modcod = 0;
                    break;
            }
        }

        if (constellation == gr::dvbs2::MOD_8APSK)
        {
            slots = (FRAME_SIZE_NORMAL / 3) / 90;
            pilot_symbols = (slots / 16) * 36;
            if (!(slots % 16)) pilot_symbols -= 36;
            switch (rate)
            {
                case gr::dvbs2::C100_180:
                    modcod = 138;
                    break;
                case gr::dvbs2::C104_180:
                    modcod = 140;
                    break;
                default:
                    modcod = 0;
                    break;
            }
        }

        if (constellation == gr::dvbs2::MOD_16APSK)
        {
            slots = (FRAME_SIZE_NORMAL / 4) / 90;
            pilot_symbols = (slots / 16) * 36;
            if (!(slots % 16)) pilot_symbols -= 36;
            switch (rate)
            {
                case gr::dvbs2::C2_3:
                    modcod = 18;
                    break;
                case gr::dvbs2::C3_4:
                    modcod = 19;
                    break;
                case gr::dvbs2::C4_5:
                    modcod = 20;
                    break;
                case gr::dvbs2::C5_6:
                    modcod = 21;
                    break;
                case gr::dvbs2::C8_9:
                    modcod = 22;
                    break;
                case gr::dvbs2::C9_10:
                    modcod = 23;
                    break;
                case gr::dvbs2::C26_45:
                    modcod = 154;
                    break;
                case gr::dvbs2::C3_5:
                    modcod = 156;
                    break;
                case gr::dvbs2::C28_45:
                    modcod = 160;
                    break;
                case gr::dvbs2::C23_36:
                    modcod = 162;
                    break;
                case gr::dvbs2::C25_36:
                    modcod = 166;
                    break;
                case gr::dvbs2::C13_18:
                    modcod = 168;
                    break;
                case gr::dvbs2::C140_180:
                    modcod = 170;
                    break;
                case gr::dvbs2::C154_180:
                    modcod = 172;
                    break;
                default:
                    modcod = 0;
                    break;
            }
        }

        if (constellation == gr::dvbs2::MOD_8_8APSK)
        {
            slots = (FRAME_SIZE_NORMAL / 4) / 90;
            pilot_symbols = (slots / 16) * 36;
            if (!(slots % 16)) pilot_symbols -= 36;
            switch (rate)
            {
                case gr::dvbs2::C90_180:
                    modcod = 148;
                    break;
                case gr::dvbs2::C96_180:
                    modcod = 150;
                    break;
                case gr::dvbs2::C100_180:
                    modcod = 152;
                    break;
                case gr::dvbs2::C18_30:
                    modcod = 158;
                    break;
                case gr::dvbs2::C20_30:
                    modcod = 164;
                    break;
                default:
                    modcod = 0;
                    break;
            }
        }

        if (constellation == gr::dvbs2::MOD_32APSK)
        {
            slots = (FRAME_SIZE_NORMAL / 5) / 90;
            pilot_symbols = (slots / 16) * 36;
            if (!(slots % 16)) pilot_symbols -= 36;
            switch (rate)
            {
                case gr::dvbs2::C3_4:
                    modcod = 24;
                    break;
                case gr::dvbs2::C4_5:
                    modcod = 25;
                    break;
                case gr::dvbs2::C5_6:
                    modcod = 26;
                    break;
                case gr::dvbs2::C8_9:
                    modcod = 27;
                    break;
                case gr::dvbs2::C9_10:
                    modcod = 28;
                    break;
                default:
                    modcod = 0;
                    break;
            }
        }

        if (constellation == gr::dvbs2::MOD_4_12_16APSK)
        {
            slots = (FRAME_SIZE_NORMAL / 5) / 90;
            pilot_symbols = (slots / 16) * 36;
            if (!(slots % 16)) pilot_symbols -= 36;
            switch (rate)
            {
                case gr::dvbs2::C2_3:
                    modcod = 174;
                    break;
                default:
                    modcod = 0;
                    break;
            }
        }

        if (constellation == gr::dvbs2::MOD_4_8_4_16APSK)
        {
            slots = (FRAME_SIZE_NORMAL / 5) / 90;
            pilot_symbols = (slots / 16) * 36;
            if (!(slots % 16)) pilot_symbols -= 36;
            switch (rate)
            {
                case gr::dvbs2::C128_180:
                    modcod = 178;
                    break;
                case gr::dvbs2::C132_180:
                    modcod = 180;
                    break;
                case gr::dvbs2::C140_180:
                    modcod = 182;
                    break;
                default:
                    modcod = 0;
                    break;
            }
        }

        if (constellation == gr::dvbs2::MOD_64APSK)
        {
            slots = (FRAME_SIZE_NORMAL / 6) / 90;
            pilot_symbols = (slots / 16) * 36;
            if (!(slots % 16)) pilot_symbols -= 36;
            switch (rate)
            {
                case gr::dvbs2::C128_180:
                    modcod = 184;
                    break;
                default:
                    modcod = 0;
                    break;
            }
        }

        if (constellation == gr::dvbs2::MOD_8_16_20_20APSK)
        {
            slots = (FRAME_SIZE_NORMAL / 6) / 90;
            pilot_symbols = (slots / 16) * 36;
            if (!(slots % 16)) pilot_symbols -= 36;
            switch (rate)
            {
                case gr::dvbs2::C7_9:
                    modcod = 190;
                    break;
                case gr::dvbs2::C4_5:
                    modcod = 194;
                    break;
                case gr::dvbs2::C5_6:
                    modcod = 198;
                    break;
                default:
                    modcod = 0;
                    break;
            }
        }

        if (constellation == gr::dvbs2::MOD_4_12_20_28APSK)
        {
            slots = (FRAME_SIZE_NORMAL / 6) / 90;
            pilot_symbols = (slots / 16) * 36;
            if (!(slots % 16)) pilot_symbols -= 36;
            switch (rate)
            {
                case gr::dvbs2::C132_180:
                    modcod = 186;
                    break;
                default:
                    modcod = 0;
                    break;
            }
        }

        if (constellation == gr::dvbs2::MOD_128APSK)
        {
            slots = (FRAME_SIZE_NORMAL / 7) / 90;
            pilot_symbols = (slots / 16) * 36;
            if (!(slots % 16)) pilot_symbols -= 36;
            switch (rate)
            {
                case gr::dvbs2::C135_180:
                    modcod = 200;
                    break;
                case gr::dvbs2::C140_180:
                    modcod = 202;
                    break;
                default:
                    modcod = 0;
                    break;
            }
        }

        if (constellation == gr::dvbs2::MOD_256APSK)
        {
            slots = (FRAME_SIZE_NORMAL / 8) / 90;
            pilot_symbols = (slots / 16) * 36;
            if (!(slots % 16)) pilot_symbols -= 36;
            switch (rate)
            {
                case gr::dvbs2::C116_180:
                    modcod = 204;
                    break;
                case gr::dvbs2::C20_30:
                    modcod = 206;
                    break;
                case gr::dvbs2::C124_180:
                    modcod = 208;
                    break;
                case gr::dvbs2::C128_180:
                    modcod = 210;
                    break;
                case gr::dvbs2::C22_30:
                    modcod = 212;
                    break;
                case gr::dvbs2::C135_180:
                    modcod = 214;
                    break;
                default:
                    modcod = 0;
                    break;
            }
        }

        // Now create the PL header.
        int b[90];
        // Add the sync sequence SOF
        for (int i = 0; i < 26; i++) b[i] = ph_sync_seq[i];
        // Add the mode and code
        pl_header_encode(modcod, type, &b[26]);

        // BPSK modulate and create the header
        for (int i = 0; i < 90; i++)
        {
            m_pl[i] =  m_bpsk[i & 1][b[i]];
        }
        build_symbol_scrambler_table();
        if (!pilot_mode) pilot_symbols = 0;
        set_output_multiple((((slots * 90) + 90) + pilot_symbols) * 2);
    }

    /*
     * Our virtual destructor.
     */
    physical_cc_impl::~physical_cc_impl()
    {
    }

    void
    physical_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = (noutput_items / ((((slots * 90) + 90) + pilot_symbols) * 2)) * (slots * 90);
    }

void physical_cc_impl::b_64_8_code(unsigned char in, int *out)
{
    unsigned long temp, bit;

    temp = 0;

    if (in & 0x80) temp ^= g[0];
    if (in & 0x40) temp ^= g[1];
    if (in & 0x20) temp ^= g[2];
    if (in & 0x10) temp ^= g[3];
    if (in & 0x08) temp ^= g[4];
    if (in & 0x04) temp ^= g[5];
    if (in & 0x02) temp ^= g[6];

    bit = 0x80000000;
    for (int m = 0; m < 32; m++)
    {
        out[(m * 2)] = (temp & bit) ? 1 : 0;
        out[(m * 2) + 1] = out[m * 2] ^ (in & 0x01);
        bit >>= 1;
    }
    // Randomise it
    for (int m = 0; m < 64; m++)
    {
        out[m] = out[m] ^ ph_scram_tab[m];
    }
}

void physical_cc_impl::pl_header_encode(unsigned char modcod, unsigned char type, int *out)
{
    unsigned char code;

    if (modcod & 0x80)
    {
        code = modcod | type;
    }
    else
    {
        code = (modcod << 2) | type;
    }
    // Add the modcod and type information and scramble it
    b_64_8_code (code, out);
}

int physical_cc_impl::parity_chk(long a, long b)
{
    int c = 0;
    a = a & b;
    for (int i = 0; i < 18; i++)
    {
        if(a & (1L << i)) c++;
    }
    return c & 1;
}

void physical_cc_impl::build_symbol_scrambler_table(void)
{
    long x, y;
    int xa, xb, xc, ya, yb, yc;
    int rn, zna, znb;

    // Initialisation
    x = 0x00001;
    y = 0x3FFFF;

    for (int i = 0; i < FRAME_SIZE_NORMAL; i++)
    {
        xa = parity_chk(x, 0x8050);
        xb = parity_chk(x, 0x0081);
        xc = x & 1;

        x >>= 1;
        if (xb) x |= 0x20000;

        ya = parity_chk(y, 0x04A1);
        yb = parity_chk(y, 0xFF60);
        yc = y & 1;

        y >>= 1;
        if (ya) y |= 0x20000;

        zna = xc ^ yc;
        znb = xa ^ yb;
        rn = (znb << 1) + zna;
        m_cscram[i] = rn;
    }
}

    int
    physical_cc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];
        int consumed = 0;
        int produced = 0;
        int slot_count = 0;
        int n;
        gr_complex tempin, tempout;

        for (int i = 0; i < noutput_items / 2; i += (((slots * 90) + 90) + pilot_symbols))
        {
            n = 0;
            for (int plh = 0; plh < 90; plh++)
            {
                out[produced++] = m_pl[plh];
                out[produced++] = m_zero[0];
            }
            for (int j = 0; j < slots; j++)
            {
                for (int k = 0; k < 90; k++)
                {
                    tempin = in[consumed++];
                    switch (m_cscram[n++])
                    {
                        case 0:
                            tempout.real() =  tempin.real();
                            tempout.imag() =  tempin.imag();
                            break;
                        case 1:
                            tempout.real() = -tempin.imag();
                            tempout.imag() =  tempin.real();
                            break;
                        case 2:
                            tempout.real() = -tempin.real();
                            tempout.imag() = -tempin.imag();
                            break;
                        case 3:
                            tempout.real() =  tempin.imag();
                            tempout.imag() = -tempin.real();
                            break;
                    }
                    out[produced++] = tempout;
                    out[produced++] = m_zero[0];
                }
                slot_count = (slot_count + 1) % 16;
                if ((slot_count == 0) && (j < slots - 1))
                {
                    if (pilot_mode)
                    {
                        // Add pilots if needed
                        for (int p = 0; p < 36; p++)
                        {
                            tempin = m_bpsk[0][0];
                            switch (m_cscram[n++])
                            {
                                case 0:
                                    tempout.real() =  tempin.real();
                                    tempout.imag() =  tempin.imag();
                                    break;
                                case 1:
                                    tempout.real() = -tempin.imag();
                                    tempout.imag() =  tempin.real();
                                    break;
                                case 2:
                                    tempout.real() = -tempin.real();
                                    tempout.imag() = -tempin.imag();
                                    break;
                                case 03:
                                    tempout.real() =  tempin.imag();
                                    tempout.imag() = -tempin.real();
                                    break;
                            }
                            out[produced++] = tempout;
                            out[produced++] = m_zero[0];
                        }
                    }
                }
            }
        }

        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (consumed);

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

    const unsigned long physical_cc_impl::g[7] =
    {
        0x90AC2DDD, 0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF, 0x0000FFFF, 0xFFFFFFFF
    };

    const int physical_cc_impl::ph_scram_tab[64] =
    {
        0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1,
        0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0
    };

    const int physical_cc_impl::ph_sync_seq[26] =
    {
        0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0
    };

  } /* namespace dvbs2 */
} /* namespace gr */

