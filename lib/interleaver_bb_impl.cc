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
#include "interleaver_bb_impl.h"

namespace gr {
  namespace dvbs2 {

    interleaver_bb::sptr
    interleaver_bb::make(dvbs2_constellation_t constellation, dvbs2_code_rate_t rate, dvbs2_framesize_t framesize)
    {
      return gnuradio::get_initial_sptr
        (new interleaver_bb_impl(constellation, rate, framesize));
    }

    /*
     * The private constructor
     */
    interleaver_bb_impl::interleaver_bb_impl(dvbs2_constellation_t constellation, dvbs2_code_rate_t rate, dvbs2_framesize_t framesize)
      : gr::block("interleaver_bb",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {
        int rows;
        signal_constellation = constellation;
        code_rate = rate;
        if (framesize == gr::dvbs2::FECFRAME_NORMAL)
        {
            frame_size = FRAME_SIZE_NORMAL;
        }
        else
        {
            frame_size = FRAME_SIZE_SHORT;
        }
        switch (constellation)
        {
            case gr::dvbs2::MOD_QPSK:
                mod = 2;
                rows = frame_size / mod;
                set_output_multiple(rows);
                packed_items = rows;
                break;
            case gr::dvbs2::MOD_8PSK:
                mod = 3;
                rows = frame_size / mod;
                if (rate == gr::dvbs2::C3_5)
                {
                    rowaddr0 = rows * 2;
                    rowaddr1 = rows;
                    rowaddr2 = 0;
                }
                else if (rate == gr::dvbs2::C25_36 || rate == gr::dvbs2::C13_18 || rate == gr::dvbs2::C7_15 || rate == gr::dvbs2::C8_15)    /* 102 */
                {
                    rowaddr0 = rows;
                    rowaddr1 = 0;
                    rowaddr2 = rows * 2;
                }
                else
                {
                    rowaddr0 = 0;
                    rowaddr1 = rows;
                    rowaddr2 = rows * 2;
                }
                set_output_multiple(rows);
                packed_items = rows;
                break;
            case gr::dvbs2::MOD_8APSK:
                mod = 3;
                rows = frame_size / mod;
                rowaddr0 = 0;
                rowaddr1 = rows;
                rowaddr2 = rows * 2;
                set_output_multiple(rows);
                packed_items = rows;
                break;
            case gr::dvbs2::MOD_16APSK:
                mod = 4;
                rows = frame_size / mod;
                if (rate == gr::dvbs2::C26_45)    /* 3201 */
                {
                    if (frame_size == FRAME_SIZE_NORMAL)
                    {
                        rowaddr0 = rows * 3;
                        rowaddr1 = rows * 2;
                        rowaddr2 = 0;
                        rowaddr3 = rows;
                    }
                    else    /* 2130 */
                    {
                        rowaddr0 = rows * 2;
                        rowaddr1 = rows;
                        rowaddr2 = rows * 3;
                        rowaddr3 = 0;
                    }
                }
                else if (rate == gr::dvbs2::C3_5)    /* 3210 */
                {
                    if (frame_size == FRAME_SIZE_NORMAL)
                    {
                        rowaddr0 = rows * 3;
                        rowaddr1 = rows * 2;
                        rowaddr2 = rows;
                        rowaddr3 = 0;
                    }
                    else    /* 3201 */
                    {
                        rowaddr0 = rows * 3;
                        rowaddr1 = rows * 2;
                        rowaddr2 = 0;
                        rowaddr3 = rows;
                    }
                }
                else if (rate == gr::dvbs2::C28_45)    /*3012 */
                {
                    rowaddr0 = rows * 3;
                    rowaddr1 = 0;
                    rowaddr2 = rows;
                    rowaddr3 = rows * 2;
                }
                else if (rate == gr::dvbs2::C23_36 || rate == gr::dvbs2::C13_18)    /* 3021 */
                {
                    rowaddr0 = rows * 3;
                    rowaddr1 = 0;
                    rowaddr2 = rows * 2;
                    rowaddr3 = rows;
                }
                else if (rate == gr::dvbs2::C25_36)    /* 2310 */
                {
                    rowaddr0 = rows * 2;
                    rowaddr1 = rows * 3;
                    rowaddr2 = rows;
                    rowaddr3 = 0;
                }
                else if (rate == gr::dvbs2::C7_15 || rate == gr::dvbs2::C8_15)    /* 2103 */
                {
                    rowaddr0 = rows * 2;
                    rowaddr1 = rows;
                    rowaddr2 = 0;
                    rowaddr3 = rows * 3;
                }
                else
                {
                    rowaddr0 = 0;
                    rowaddr1 = rows;
                    rowaddr2 = rows * 2;
                    rowaddr3 = rows * 3;
                }
                set_output_multiple(rows);
                packed_items = rows;
                break;
            case gr::dvbs2::MOD_8_8APSK:
                mod = 4;
                rows = frame_size / mod;
                if (rate == gr::dvbs2::C90_180)    /* 3210 */
                {
                    rowaddr0 = rows * 3;
                    rowaddr1 = rows * 2;
                    rowaddr2 = rows;
                    rowaddr3 = 0;
                }
                else if (rate == gr::dvbs2::C96_180)    /* 2310 */
                {
                    rowaddr0 = rows * 2;
                    rowaddr1 = rows * 3;
                    rowaddr2 = rows;
                    rowaddr3 = 0;
                }
                else if (rate == gr::dvbs2::C100_180)    /* 2301 */
                {
                    rowaddr0 = rows * 2;
                    rowaddr1 = rows * 3;
                    rowaddr2 = 0;
                    rowaddr3 = rows;
                }
                else
                {
                    rowaddr0 = 0;
                    rowaddr1 = rows;
                    rowaddr2 = rows * 2;
                    rowaddr3 = rows * 3;
                }
                set_output_multiple(rows);
                packed_items = rows;
                break;
            case gr::dvbs2::MOD_32APSK:
                mod = 5;
                rows = frame_size / mod;
                rowaddr0 = 0;
                rowaddr1 = rows;
                rowaddr2 = rows * 2;
                rowaddr3 = rows * 3;
                rowaddr4 = rows * 4;
                set_output_multiple(rows);
                packed_items = rows;
                break;
            case gr::dvbs2::MOD_4_12_16APSK:
                mod = 5;
                rows = frame_size / mod;
                if (frame_size == FRAME_SIZE_NORMAL)    /* 21430 */
                {
                    rowaddr0 = rows * 2;
                    rowaddr1 = rows;
                    rowaddr2 = rows * 4;
                    rowaddr3 = rows * 3;
                    rowaddr4 = 0;
                }
                else
                {
                    if (rate == gr::dvbs2::C2_3)    /* 41230 */
                    {
                        rowaddr0 = rows * 4;
                        rowaddr1 = rows;
                        rowaddr2 = rows * 2;
                        rowaddr3 = rows * 3;
                        rowaddr4 = 0;
                    }
                    else if (rate == gr::dvbs2::C32_45)    /* 10423 */
                    {
                        rowaddr0 = rows;
                        rowaddr1 = 0;
                        rowaddr2 = rows * 4;
                        rowaddr3 = rows * 2;
                        rowaddr4 = rows * 3;
                    }
                }
                set_output_multiple(rows);
                packed_items = rows;
                break;
            case gr::dvbs2::MOD_4_8_4_16APSK:
                mod = 5;
                rows = frame_size / mod;
                if (rate == gr::dvbs2::C140_180)    /* 40213 */
                {
                    rowaddr0 = rows * 4;
                    rowaddr1 = 0;
                    rowaddr2 = rows * 2;
                    rowaddr3 = rows;
                    rowaddr4 = rows * 3;
                }
                else
                {
                    rowaddr0 = rows * 4;
                    rowaddr1 = 0;
                    rowaddr2 = rows * 3;
                    rowaddr3 = rows;
                    rowaddr4 = rows * 2;
                }
                set_output_multiple(rows);
                packed_items = rows;
                break;
            case gr::dvbs2::MOD_64APSK:
                mod = 6;
                rows = frame_size / mod;
                rowaddr0 = rows * 3;
                rowaddr1 = 0;
                rowaddr2 = rows * 5;
                rowaddr3 = rows * 2;
                rowaddr4 = rows;
                rowaddr5 = rows * 4;
                set_output_multiple(rows);
                packed_items = rows;
                break;
            case gr::dvbs2::MOD_8_16_20_20APSK:
                mod = 6;
                rows = frame_size / mod;
                if (rate == gr::dvbs2::C7_9)    /* 201543 */
                {
                    rowaddr0 = rows * 2;
                    rowaddr1 = 0;
                    rowaddr2 = rows;
                    rowaddr3 = rows * 5;
                    rowaddr4 = rows * 4;
                    rowaddr5 = rows * 3;
                }
                else if (rate == gr::dvbs2::C4_5)    /* 124053 */
                {
                    rowaddr0 = rows;
                    rowaddr1 = rows * 2;
                    rowaddr2 = rows * 4;
                    rowaddr3 = 0;
                    rowaddr4 = rows * 5;
                    rowaddr5 = rows * 3;
                }
                else if (rate == gr::dvbs2::C5_6)    /* 421053 */
                {
                    rowaddr0 = rows * 4;
                    rowaddr1 = rows * 2;
                    rowaddr2 = rows;
                    rowaddr3 = 0;
                    rowaddr4 = rows * 5;
                    rowaddr5 = rows * 3;
                }
                set_output_multiple(rows);
                packed_items = rows;
                break;
            case gr::dvbs2::MOD_4_12_20_28APSK:
                mod = 6;
                rows = frame_size / mod;
                rowaddr0 = rows * 5;
                rowaddr1 = rows * 2;
                rowaddr2 = 0;
                rowaddr3 = rows;
                rowaddr4 = rows * 4;
                rowaddr5 = rows * 3;
                set_output_multiple(rows);
                packed_items = rows;
                break;
            case gr::dvbs2::MOD_128APSK:
                mod = 7;
                rows = (frame_size + 6) / mod;
                if (rate == gr::dvbs2::C135_180)    /* 4250316 */
                {
                    rowaddr0 = rows * 4;
                    rowaddr1 = rows * 2;
                    rowaddr2 = rows * 5;
                    rowaddr3 = 0;
                    rowaddr4 = rows * 3;
                    rowaddr5 = rows;
                    rowaddr6 = rows * 6;
                }
                else if (rate == gr::dvbs2::C140_180)    /* 4130256 */
                {
                    rowaddr0 = rows * 4;
                    rowaddr1 = rows;
                    rowaddr2 = rows * 3;
                    rowaddr3 = 0;
                    rowaddr4 = rows * 2;
                    rowaddr5 = rows * 5;
                    rowaddr6 = rows * 6;
                }
                set_output_multiple(rows + 12);
                packed_items = rows + 12;
                break;
            case gr::dvbs2::MOD_256APSK:
                mod = 8;
                rows = frame_size / mod;
                if (rate == gr::dvbs2::C116_180)    /* 40372156 */
                {
                    rowaddr0 = rows * 4;
                    rowaddr1 = 0;
                    rowaddr2 = rows * 3;
                    rowaddr3 = rows * 7;
                    rowaddr4 = rows * 2;
                    rowaddr5 = rows;
                    rowaddr6 = rows * 5;
                    rowaddr7 = rows * 6;
                }
                else if (rate == gr::dvbs2::C124_180)    /* 46320571 */
                {
                    rowaddr0 = rows * 4;
                    rowaddr1 = rows * 6;
                    rowaddr2 = rows * 3;
                    rowaddr3 = rows * 2;
                    rowaddr4 = 0;
                    rowaddr5 = rows * 5;
                    rowaddr6 = rows * 7;
                    rowaddr7 = rows;
                }
                else if (rate == gr::dvbs2::C128_180)    /* 75642301 */
                {
                    rowaddr0 = rows * 7;
                    rowaddr1 = rows * 5;
                    rowaddr2 = rows * 6;
                    rowaddr3 = rows * 4;
                    rowaddr4 = rows * 2;
                    rowaddr5 = rows * 3;
                    rowaddr6 = 0;
                    rowaddr7 = rows;
                }
                else if (rate == gr::dvbs2::C135_180)    /* 50743612 */
                {
                    rowaddr0 = rows * 5;
                    rowaddr1 = 0;
                    rowaddr2 = rows * 7;
                    rowaddr3 = rows * 4;
                    rowaddr4 = rows * 3;
                    rowaddr5 = rows * 6;
                    rowaddr6 = rows;
                    rowaddr7 = rows * 2;
                }
                else
                {
                    rowaddr0 = 0;
                    rowaddr1 = rows;
                    rowaddr2 = rows * 2;
                    rowaddr3 = rows * 3;
                    rowaddr4 = rows * 4;
                    rowaddr5 = rows * 5;
                    rowaddr6 = rows * 6;
                    rowaddr7 = rows * 7;
                }
                set_output_multiple(rows);
                packed_items = rows;
                break;
            default:
                mod = 2;
                rows = frame_size / mod;
                set_output_multiple(rows);
                packed_items = rows;
                break;
        }
    }

    /*
     * Our virtual destructor.
     */
    interleaver_bb_impl::~interleaver_bb_impl()
    {
    }

    void
    interleaver_bb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        if (signal_constellation == gr::dvbs2::MOD_128APSK)
        {
            ninput_items_required[0] = ((noutput_items / 9270) * 9258) * mod;
        }
        else
        {
            ninput_items_required[0] = noutput_items * mod;
        }
    }

    int
    interleaver_bb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const unsigned char *in = (const unsigned char *) input_items[0];
        unsigned char *out = (unsigned char *) output_items[0];
        int consumed = 0;
        int produced = 0;
        int rows;

        switch (signal_constellation)
        {
            case gr::dvbs2::MOD_QPSK:
                for (int i = 0; i < noutput_items; i += packed_items)
                {
                    rows = frame_size / 2;
                    for (int j = 0; j < rows; j++)
                    {
                        out[produced] = in[consumed++] << 1;
                        out[produced++] |= in[consumed++];
                    }
                }
                break;
            case gr::dvbs2::MOD_8PSK:
            case gr::dvbs2::MOD_8APSK:
                for (int i = 0; i < noutput_items; i += packed_items)
                {
                    rows = frame_size / 3;
                    const unsigned char  *c1, *c2, *c3;
                    c1 = &in[consumed + rowaddr0];
                    c2 = &in[consumed + rowaddr1];
                    c3 = &in[consumed + rowaddr2];
                    for (int j = 0; j < rows; j++)
                    {
                        out[produced++] = (c1[j]<<2) | (c2[j]<<1) | (c3[j]);
                        consumed += 3;
                    }
                }
                break;
            case gr::dvbs2::MOD_16APSK:
            case gr::dvbs2::MOD_8_8APSK:
                for (int i = 0; i < noutput_items; i += packed_items)
                {
                    rows = frame_size / 4;
                    const unsigned char *c1, *c2, *c3, *c4;
                    c1 = &in[consumed + rowaddr0];
                    c2 = &in[consumed + rowaddr1];
                    c3 = &in[consumed + rowaddr2];
                    c4 = &in[consumed + rowaddr3];
                    for (int j = 0; j < rows; j++)
                    {
                        out[produced++] = (c1[j]<<3) | (c2[j]<<2) | (c3[j]<<1) | (c4[j]);
                        consumed += 4;
                    }
                }
                break;
            case gr::dvbs2::MOD_32APSK:
            case gr::dvbs2::MOD_4_12_16APSK:
            case gr::dvbs2::MOD_4_8_4_16APSK:
                for (int i = 0; i < noutput_items; i += packed_items)
                {
                    rows = frame_size / 5;
                    const unsigned char *c1, *c2, *c3, *c4, *c5;
                    c1 = &in[consumed + rowaddr0];
                    c2 = &in[consumed + rowaddr1];
                    c3 = &in[consumed + rowaddr2];
                    c4 = &in[consumed + rowaddr3];
                    c5 = &in[consumed + rowaddr4];
                    for (int j = 0; j < rows; j++)
                    {
                        out[produced++] = (c1[j]<<4) | (c2[j]<<3) | (c3[j]<<2) | (c4[j]<<1) | c5[j];
                        consumed += 5;
                    }
                }
                break;
            case gr::dvbs2::MOD_64APSK:
            case gr::dvbs2::MOD_8_16_20_20APSK:
            case gr::dvbs2::MOD_4_12_20_28APSK:
                for (int i = 0; i < noutput_items; i += packed_items)
                {
                    rows = frame_size / 6;
                    const unsigned char *c1, *c2, *c3, *c4, *c5, *c6;
                    c1 = &in[consumed + rowaddr0];
                    c2 = &in[consumed + rowaddr1];
                    c3 = &in[consumed + rowaddr2];
                    c4 = &in[consumed + rowaddr3];
                    c5 = &in[consumed + rowaddr4];
                    c6 = &in[consumed + rowaddr5];
                    for (int j = 0; j < rows; j++)
                    {
                        out[produced++] = (c1[j]<<5) | (c2[j]<<4) | (c3[j]<<3) | (c4[j]<<2) | (c5[j]<<1) | c6[j];
                        consumed += 6;
                    }
                }
                break;
            case gr::dvbs2::MOD_128APSK:
                for (int i = 0; i < noutput_items; i += packed_items)
                {
                    rows = (frame_size + 6) / 7;
                    const unsigned char *c1, *c2, *c3, *c4, *c5, *c6, *c7;
                    c1 = &in[consumed + rowaddr0];
                    c2 = &in[consumed + rowaddr1];
                    c3 = &in[consumed + rowaddr2];
                    c4 = &in[consumed + rowaddr3];
                    c5 = &in[consumed + rowaddr4];
                    c6 = &in[consumed + rowaddr5];
                    c7 = &in[consumed + rowaddr6];
                    for (int j = 0; j < rows; j++)
                    {
                        out[produced++] = (c1[j]<<6) | (c2[j]<<5) | (c3[j]<<4) | (c4[j]<<3) | (c5[j]<<2) | (c6[j]<<1) | c7[j];
                        consumed += 7;
                    }
                    for (int j = 0; j < 12; j++)
                    {
                        out[produced++] = 0x7f;
                    }
                }
                break;
            case gr::dvbs2::MOD_256APSK:
                for (int i = 0; i < noutput_items; i += packed_items)
                {
                    rows = frame_size / 8;
                    const unsigned char *c1, *c2, *c3, *c4, *c5, *c6, *c7, *c8;
                    c1 = &in[consumed + rowaddr0];
                    c2 = &in[consumed + rowaddr1];
                    c3 = &in[consumed + rowaddr2];
                    c4 = &in[consumed + rowaddr3];
                    c5 = &in[consumed + rowaddr4];
                    c6 = &in[consumed + rowaddr5];
                    c7 = &in[consumed + rowaddr6];
                    c8 = &in[consumed + rowaddr7];
                    for (int j = 0; j < rows; j++)
                    {
                        out[produced++] = (c1[j]<<7) | (c2[j]<<6) | (c3[j]<<5) | (c4[j]<<4) | (c5[j]<<3) | (c6[j]<<2) | (c7[j]<<1) | c8[j];
                        consumed += 8;
                    }
                }
                break;
            default:
                for (int i = 0; i < noutput_items; i += packed_items)
                {
                    rows = frame_size / 2;
                    for (int j = 0; j < rows; j++)
                    {
                        out[produced] = in[consumed++] << 1;
                        out[produced++] |= in[consumed++];
                    }
                }
                break;
        }

        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (consumed);

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace dvbs2 */
} /* namespace gr */

