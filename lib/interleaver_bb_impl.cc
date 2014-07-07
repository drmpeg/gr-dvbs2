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
    interleaver_bb::make(dvbs2_constellation_t constellation, dvbs2_code_rate_t rate)
    {
      return gnuradio::get_initial_sptr
        (new interleaver_bb_impl(constellation, rate));
    }

    /*
     * The private constructor
     */
    interleaver_bb_impl::interleaver_bb_impl(dvbs2_constellation_t constellation, dvbs2_code_rate_t rate)
      : gr::block("interleaver_bb",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {
        signal_constellation = constellation;
        code_rate = rate;
        switch (constellation)
        {
            case gr::dvbs2::MOD_QPSK:
                mod = 2;
                set_output_multiple(FRAME_SIZE_NORMAL / mod);
                packed_items = FRAME_SIZE_NORMAL / mod;
                break;
            case gr::dvbs2::MOD_8PSK:
                mod = 3;
                set_output_multiple(FRAME_SIZE_NORMAL / mod);
                packed_items = FRAME_SIZE_NORMAL / mod;
                break;
            case gr::dvbs2::MOD_16APSK:
                mod = 4;
                set_output_multiple(FRAME_SIZE_NORMAL / mod);
                packed_items = FRAME_SIZE_NORMAL / mod;
                break;
            case gr::dvbs2::MOD_32APSK:
                mod = 5;
                set_output_multiple(FRAME_SIZE_NORMAL / mod);
                packed_items = FRAME_SIZE_NORMAL / mod;
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
        ninput_items_required[0] = noutput_items * mod;
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
        int frame_size = FRAME_SIZE_NORMAL;
        int rows;

        if (signal_constellation == gr::dvbs2::MOD_QPSK)
        {
            for (int i = 0; i < noutput_items; i += packed_items)
            {
                rows = frame_size / 2;
                for (int j = 0; j < rows; j++)
                {
                    out[produced] = in[consumed++] << 1;
                    out[produced++] |= in[consumed++];
                }
            }
        }

        if (signal_constellation == gr::dvbs2::MOD_8PSK)
        {
            for (int i = 0; i < noutput_items; i += packed_items)
            {
                if (code_rate == gr::dvbs2::C3_5)
                {
                    rows = frame_size / 3;
                    const unsigned char *c1, *c2, *c3;
                    c1 = &in[consumed + (rows * 2)];
                    c2 = &in[consumed + rows];
                    c3 = &in[consumed + 0];
                    for (int j = 0; j < rows; j++)
                    {
                        out[produced++] = (c1[j]<<2) | (c2[j]<<1) | (c3[j]);
                        consumed += 3;
                    }
                }
                else
                {
                    rows = frame_size / 3;
                    const unsigned char  *c1, *c2, *c3;
                    c1 = &in[consumed + 0];
                    c2 = &in[consumed + rows];
                    c3 = &in[consumed + (rows * 2)];
                    for (int j = 0; j < rows; j++)
                    {
                        out[produced++] = (c1[j]<<2) | (c2[j]<<1) | (c3[j]);
                        consumed += 3;
                    }
                }
            }
        }

        if (signal_constellation == gr::dvbs2::MOD_16APSK)
        {
            for (int i = 0; i < noutput_items; i += packed_items)
            {
                rows = frame_size / 4;
                const unsigned char *c1, *c2, *c3, *c4;
                c1 = &in[consumed + 0];
                c2 = &in[consumed + rows];
                c3 = &in[consumed + (rows * 2)];
                c4 = &in[consumed + (rows * 3)];
                for (int j = 0; j < rows; j++)
                {
                    out[produced++] = (c1[j]<<3) | (c2[j]<<2) | (c3[j]<<1) | (c4[j]);
                    consumed += 4;
                }
            }
        }

        if (signal_constellation == gr::dvbs2::MOD_32APSK)
        {
            for (int i = 0; i < noutput_items; i += packed_items)
            {
                rows = frame_size / 5;
                const unsigned char *c1, *c2, *c3, *c4, *c5;
                c1 = &in[consumed + 0];
                c2 = &in[consumed + rows];
                c3 = &in[consumed + (rows * 2)];
                c4 = &in[consumed + (rows * 3)];
                c5 = &in[consumed + (rows * 4)];
                for (int j = 0; j < rows; j++)
                {
                    out[produced++] = (c1[j]<<4) | (c2[j]<<3) | (c3[j]<<2) | (c4[j]<<1) | c5[j];
                    consumed += 5;
                }
            }
        }

        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (consumed);

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace dvbs2 */
} /* namespace gr */

