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
#include "modulator_bc_impl.h"

namespace gr {
  namespace dvbs2 {

    modulator_bc::sptr
    modulator_bc::make(dvbs2_constellation_t constellation, dvbs2_code_rate_t rate)
    {
      return gnuradio::get_initial_sptr
        (new modulator_bc_impl(constellation, rate));
    }

    /*
     * The private constructor
     */
    modulator_bc_impl::modulator_bc_impl(dvbs2_constellation_t constellation, dvbs2_code_rate_t rate)
      : gr::block("modulator_bc",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
        double r1, r2, r3;
        double m = 1.0;
        r1 = m;
        switch (constellation)
        {
            case gr::dvbs2::MOD_QPSK:
                m_qpsk[0].real() = (r1 * cos(M_PI / 4.0));
                m_qpsk[0].imag() = (r1 * sin(M_PI / 4.0));
                m_qpsk[1].real() = (r1 * cos(7 * M_PI / 4.0));
                m_qpsk[1].imag() = (r1 * sin(7 * M_PI / 4.0));
                m_qpsk[2].real() = (r1 * cos(3 * M_PI / 4.0));
                m_qpsk[2].imag() = (r1 * sin(3 * M_PI / 4.0));
                m_qpsk[3].real() = (r1 * cos(5 * M_PI / 4.0));
                m_qpsk[3].imag() = (r1 * sin(5 * M_PI / 4.0));
                break;
            case gr::dvbs2::MOD_8PSK:
                m_8psk[0].real() = (r1 * cos(M_PI / 4.0));
                m_8psk[0].imag() = (r1 * sin(M_PI / 4.0));
                m_8psk[1].real() = (r1 * cos(0.0));
                m_8psk[1].imag() = (r1 * sin(0.0));
                m_8psk[2].real() = (r1 * cos(4 * M_PI / 4.0));
                m_8psk[2].imag() = (r1 * sin(4 * M_PI / 4.0));
                m_8psk[3].real() = (r1 * cos(5 * M_PI / 4.0));
                m_8psk[3].imag() = (r1 * sin(5 * M_PI / 4.0));
                m_8psk[4].real() = (r1 * cos(2 * M_PI / 4.0));
                m_8psk[4].imag() = (r1 * sin(2 * M_PI / 4.0));
                m_8psk[5].real() = (r1 * cos(7 * M_PI / 4.0));
                m_8psk[5].imag() = (r1 * sin(7 * M_PI / 4.0));
                m_8psk[6].real() = (r1 * cos(3 * M_PI / 4.0));
                m_8psk[6].imag() = (r1 * sin(3 * M_PI / 4.0));
                m_8psk[7].real() = (r1 * cos(6 * M_PI / 4.0));
                m_8psk[7].imag() = (r1 * sin(6 * M_PI / 4.0));
                break;
            case gr::dvbs2::MOD_16APSK:
                r2 = m;
                switch(rate)
                {
                    case gr::dvbs2::C2_3:
                        r1 = r2 / 3.15;
                        break;
                    case gr::dvbs2::C3_4:
                        r1 = r2 / 2.85;
                        break;
                    case gr::dvbs2::C4_5:
                        r1 = r2 / 2.75;
                        break;
                    case gr::dvbs2::C5_6:
                        r1 = r2 / 2.70;
                        break;
                    case gr::dvbs2::C8_9:
                        r1 = r2 / 2.60;
                        break;
                    case gr::dvbs2::C9_10:
                        r1 = r2 / 2.57;
                        break;
                    default:
                        r1 = 0;
                        break;
                }
                m_16apsk[0].real()  = (r2 * cos(M_PI / 4.0));
                m_16apsk[0].imag()  = (r2 * sin(M_PI / 4.0));
                m_16apsk[1].real()  = (r2 * cos(-M_PI / 4.0));
                m_16apsk[1].imag()  = (r2 * sin(-M_PI / 4.0));
                m_16apsk[2].real()  = (r2 * cos(3 * M_PI / 4.0));
                m_16apsk[2].imag()  = (r2 * sin(3 * M_PI / 4.0));
                m_16apsk[3].real()  = (r2 * cos(-3 * M_PI / 4.0));
                m_16apsk[3].imag()  = (r2 * sin(-3 * M_PI / 4.0));
                m_16apsk[4].real()  = (r2 * cos(M_PI / 12.0));
                m_16apsk[4].imag()  = (r2 * sin(M_PI / 12.0));
                m_16apsk[5].real()  = (r2 * cos(-M_PI / 12.0));
                m_16apsk[5].imag()  = (r2 * sin(-M_PI / 12.0));
                m_16apsk[6].real()  = (r2 * cos(11 * M_PI / 12.0));
                m_16apsk[6].imag()  = (r2 * sin(11 * M_PI / 12.0));
                m_16apsk[7].real()  = (r2 * cos(-11 * M_PI / 12.0));
                m_16apsk[7].imag()  = (r2 * sin(-11 * M_PI / 12.0));
                m_16apsk[8].real()  = (r2 * cos(5 * M_PI / 12.0));
                m_16apsk[8].imag()  = (r2 * sin(5 * M_PI / 12.0));
                m_16apsk[9].real()  = (r2 * cos(-5 * M_PI / 12.0));
                m_16apsk[9].imag()  = (r2 * sin(-5 * M_PI / 12.0));
                m_16apsk[10].real() = (r2 * cos(7 * M_PI / 12.0));
                m_16apsk[10].imag() = (r2 * sin(7 * M_PI / 12.0));
                m_16apsk[11].real() = (r2 * cos(-7 * M_PI / 12.0));
                m_16apsk[11].imag() = (r2 * sin(-7 * M_PI / 12.0));
                m_16apsk[12].real() = (r1 * cos(M_PI / 4.0));
                m_16apsk[12].imag() = (r1 * sin(M_PI / 4.0));
                m_16apsk[13].real() = (r1 * cos(-M_PI / 4.0));
                m_16apsk[13].imag() = (r1 * sin(-M_PI / 4.0));
                m_16apsk[14].real() = (r1 * cos(3 * M_PI / 4.0));
                m_16apsk[14].imag() = (r1 * sin(3 * M_PI / 4.0));
                m_16apsk[15].real() = (r1 * cos(-3 * M_PI / 4.0));
                m_16apsk[15].imag() = (r1 * sin(-3 * M_PI / 4.0));
                break;
            case gr::dvbs2::MOD_32APSK:
                r3 = m;
                switch(rate)
                {
                    case dvbs2::C3_4:
                        r1 = r3 / 5.27;
                        r2 = r1 * 2.84;
                        break;
                    case dvbs2::C4_5:
                        r1 = r3 / 4.87;
                        r2 = r1 * 2.72;
                        break;
                    case dvbs2::C5_6:
                        r1 = r3 / 4.64;
                        r2 = r1 * 2.64;
                        break;
                    case dvbs2::C8_9:
                        r1 = r3 / 4.33;
                        r2 = r1 * 2.54;
                        break;
                    case dvbs2::C9_10:
                        r1 = r3 / 4.30;
                        r2 = r1 * 2.53;
                        break;
                    default:
                        r1 = 0;
                        r2 = 0;
                        break;
                }
                m_32apsk[0].real()  = (r2 * cos(M_PI / 4.0));
                m_32apsk[0].imag()  = (r2 * sin(M_PI / 4.0));
                m_32apsk[1].real()  = (r2 * cos(5 * M_PI / 12.0));
                m_32apsk[1].imag()  = (r2 * sin(5 * M_PI / 12.0));
                m_32apsk[2].real()  = (r2 * cos(-M_PI / 4.0));
                m_32apsk[2].imag()  = (r2 * sin(-M_PI / 4.0));
                m_32apsk[3].real()  = (r2 * cos(-5 * M_PI / 12.0));
                m_32apsk[3].imag()  = (r2 * sin(-5 * M_PI / 12.0));
                m_32apsk[4].real()  = (r2 * cos(3 * M_PI / 4.0));
                m_32apsk[4].imag()  = (r2 * sin(3 * M_PI / 4.0));
                m_32apsk[5].real()  = (r2 * cos(7 * M_PI / 12.0));
                m_32apsk[5].imag()  = (r2 * sin(7 * M_PI / 12.0));
                m_32apsk[6].real()  = (r2 * cos(-3 * M_PI / 4.0));
                m_32apsk[6].imag()  = (r2 * sin(-3 * M_PI / 4.0));
                m_32apsk[7].real()  = (r2 * cos(-7 * M_PI / 12.0));
                m_32apsk[7].imag()  = (r2 * sin(-7 * M_PI / 12.0));
                m_32apsk[8].real()  = (r3 * cos(M_PI / 8.0));
                m_32apsk[8].imag()  = (r3 * sin(M_PI / 8.0));
                m_32apsk[9].real()  = (r3 * cos(3 * M_PI / 8.0));
                m_32apsk[9].imag()  = (r3 * sin(3 * M_PI / 8.0));
                m_32apsk[10].real() = (r3 * cos(-M_PI / 4.0));
                m_32apsk[10].imag() = (r3 * sin(-M_PI / 4.0));
                m_32apsk[11].real() = (r3 * cos(-M_PI / 2.0));
                m_32apsk[11].imag() = (r3 * sin(-M_PI / 2.0));
                m_32apsk[12].real() = (r3 * cos(3 * M_PI / 4.0));
                m_32apsk[12].imag() = (r3 * sin(3 * M_PI / 4.0));
                m_32apsk[13].real() = (r3 * cos(M_PI / 2.0));
                m_32apsk[13].imag() = (r3 * sin(M_PI / 2.0));
                m_32apsk[14].real() = (r3 * cos(-7 * M_PI / 8.0));
                m_32apsk[14].imag() = (r3 * sin(-7 * M_PI / 8.0));
                m_32apsk[15].real() = (r3 * cos(-5 * M_PI / 8.0));
                m_32apsk[15].imag() = (r3 * sin(-5 * M_PI / 8.0));
                m_32apsk[16].real() = (r2 * cos(M_PI / 12.0));
                m_32apsk[16].imag() = (r2 * sin(M_PI / 12.0));
                m_32apsk[17].real() = (r1 * cos(M_PI / 4.0));
                m_32apsk[17].imag() = (r1 * sin(M_PI / 4.0));
                m_32apsk[18].real() = (r2 * cos(-M_PI / 12.0));
                m_32apsk[18].imag() = (r2 * sin(-M_PI / 12.0));
                m_32apsk[19].real() = (r1 * cos(-M_PI / 4.0));
                m_32apsk[19].imag() = (r1 * sin(-M_PI / 4.0));
                m_32apsk[20].real() = (r2 * cos(11 * M_PI / 12.0));
                m_32apsk[20].imag() = (r2 * sin(11 * M_PI / 12.0));
                m_32apsk[21].real() = (r1 * cos(3 * M_PI / 4.0));
                m_32apsk[21].imag() = (r1 * sin(3 * M_PI / 4.0));
                m_32apsk[22].real() = (r2 * cos(-11 * M_PI / 12.0));
                m_32apsk[22].imag() = (r2 * sin(-11 * M_PI / 12.0));
                m_32apsk[23].real() = (r1 * cos(-3 * M_PI / 4.0));
                m_32apsk[23].imag() = (r1 * sin(-3 * M_PI / 4.0));
                m_32apsk[24].real() = (r3 * cos(0.0));
                m_32apsk[24].imag() = (r3 * sin(0.0));
                m_32apsk[25].real() = (r3 * cos(M_PI / 4.0));
                m_32apsk[25].imag() = (r3 * sin(M_PI / 4.0));
                m_32apsk[26].real() = (r3 * cos(-M_PI / 8.0));
                m_32apsk[26].imag() = (r3 * sin(-M_PI / 8.0));
                m_32apsk[27].real() = (r3 * cos(-3 * M_PI / 8.0));
                m_32apsk[27].imag() = (r3 * sin(-3 * M_PI / 8.0));
                m_32apsk[28].real() = (r3 * cos(7 * M_PI / 8.0));
                m_32apsk[28].imag() = (r3 * sin(7 * M_PI / 8.0));
                m_32apsk[29].real() = (r3 * cos(5 * M_PI / 8.0));
                m_32apsk[29].imag() = (r3 * sin(5 * M_PI / 8.0));
                m_32apsk[30].real() = (r3 * cos(M_PI));
                m_32apsk[30].imag() = (r3 * sin(M_PI));
                m_32apsk[31].real() = (r3 * cos(-3 * M_PI / 4.0));
                m_32apsk[31].imag() = (r3 * sin(-3 * M_PI / 4.0));
                break;
        }
        signal_constellation = constellation;
        set_output_multiple(2);
    }

    /*
     * Our virtual destructor.
     */
    modulator_bc_impl::~modulator_bc_impl()
    {
    }

    void
    modulator_bc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items;
    }

    int
    modulator_bc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const unsigned char *in = (const unsigned char *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];
        int index;

        switch (signal_constellation)
        {
            case gr::dvbs2::MOD_QPSK:
                for (int i = 0; i < noutput_items; i++)
                {
                    index = *in++;
                    *out++ = m_qpsk[index & 0x3];
                }
                break;
            case gr::dvbs2::MOD_8PSK:
                for (int i = 0; i < noutput_items; i++)
                {
                    index = *in++;
                    *out++ = m_8psk[index & 0x7];
                }
                break;
            case gr::dvbs2::MOD_16APSK:
                for (int i = 0; i < noutput_items; i++)
                {
                    index = *in++;
                    *out++ = m_16apsk[index & 0xf];
                }
                break;
            case gr::dvbs2::MOD_32APSK:
                for (int i = 0; i < noutput_items; i++)
                {
                    index = *in++;
                    *out++ = m_32apsk[index & 0x1f];
                }
                break;
        }

        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (noutput_items);

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace dvbs2 */
} /* namespace gr */

