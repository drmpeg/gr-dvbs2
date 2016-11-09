/* -*- c++ -*- */
/* 
 * Copyright 2014,2016 Ron Economos.
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
    modulator_bc::make()
    {
      return gnuradio::get_initial_sptr
        (new modulator_bc_impl());
    }

    /*
     * The private constructor
     */
    modulator_bc_impl::modulator_bc_impl()
      : gr::block("modulator_bc",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
      double r1, r2, r3, r4;
      double m = 1.0;
      r1 = m;
      m_bpsk[0][0] = gr_complex((r1 * cos(M_PI / 4.0)), (r1 * sin(M_PI / 4.0)));
      m_bpsk[0][1] = gr_complex((r1 * cos(5.0 * M_PI / 4.0)), (r1 * sin(5.0 * M_PI / 4.0)));
      m_bpsk[1][0] = gr_complex((r1 * cos(5.0 * M_PI / 4.0)), (r1 * sin(M_PI / 4.0)));
      m_bpsk[1][1] = gr_complex((r1 * cos(M_PI / 4.0)), (r1 * sin(5.0 * M_PI /4.0)));

      m_qpsk[0] = gr_complex((r1 * cos(M_PI / 4.0)), (r1 * sin(M_PI / 4.0)));
      m_qpsk[1] = gr_complex((r1 * cos(7 * M_PI / 4.0)), (r1 * sin(7 * M_PI / 4.0)));
      m_qpsk[2] = gr_complex((r1 * cos(3 * M_PI / 4.0)), (r1 * sin(3 * M_PI / 4.0)));
      m_qpsk[3] = gr_complex((r1 * cos(5 * M_PI / 4.0)), (r1 * sin(5 * M_PI / 4.0)));

      m_8psk[0] = gr_complex((r1 * cos(M_PI / 4.0)), (r1 * sin(M_PI / 4.0)));
      m_8psk[1] = gr_complex((r1 * cos(0.0)), (r1 * sin(0.0)));
      m_8psk[2] = gr_complex((r1 * cos(4 * M_PI / 4.0)), (r1 * sin(4 * M_PI / 4.0)));
      m_8psk[3] = gr_complex((r1 * cos(5 * M_PI / 4.0)), (r1 * sin(5 * M_PI / 4.0)));
      m_8psk[4] = gr_complex((r1 * cos(2 * M_PI / 4.0)), (r1 * sin(2 * M_PI / 4.0)));
      m_8psk[5] = gr_complex((r1 * cos(7 * M_PI / 4.0)), (r1 * sin(7 * M_PI / 4.0)));
      m_8psk[6] = gr_complex((r1 * cos(3 * M_PI / 4.0)), (r1 * sin(3 * M_PI / 4.0)));
      m_8psk[7] = gr_complex((r1 * cos(6 * M_PI / 4.0)), (r1 * sin(6 * M_PI / 4.0)));

      float m_8apsk_radius[2][2] = {{6.8, 5.32}, {8.0, 6.39}};
      r3 = m;
      for (int i = 0; i < 2; i++) {
        r1 = r3 / m_8apsk_radius[i][0];
        r2 = r1 * m_8apsk_radius[i][1];
        m_8apsk[0][i] = gr_complex((r1 * cos(M_PI)), (r1 * sin(M_PI)));
        m_8apsk[1][i] = gr_complex((r2 * cos(M_PI * 1.352)), (r2 * sin(M_PI * 1.352)));
        m_8apsk[2][i] = gr_complex((r2 * cos(M_PI * -1.352)), (r2 * sin(M_PI * -1.352)));
        m_8apsk[3][i] = gr_complex((r3 * cos(M_PI)), (r3 * sin(M_PI)));
        m_8apsk[4][i] = gr_complex((r1 * cos(0.0)), (r1 * sin(0.0)));
        m_8apsk[5][i] = gr_complex((r2 * cos(M_PI * -0.352)), (r2 * sin(M_PI * -0.352)));
        m_8apsk[6][i] = gr_complex((r2 * cos(M_PI * 0.352)), (r2 * sin(M_PI * 0.352)));
        m_8apsk[7][i] = gr_complex((r3 * cos(0.0)), (r3 * sin(0.0)));
      }

      float m_16apsk_radius[12][1] = {{3.70}, {3.60}, {3.50}, {3.32}, {3.20}, {3.15}, {3.10}, {2.85}, {2.75}, {2.70}, {2.60}, {2.57}};
      r2 = m;
      for (int i = 0; i < 12; i++) {
        r1 = r2 / m_16apsk_radius[i][0];
        m_16apsk[0][i] = gr_complex((r2 * cos(M_PI / 4.0)), (r2 * sin(M_PI / 4.0)));
        m_16apsk[1][i] = gr_complex((r2 * cos(-M_PI / 4.0)), (r2 * sin(-M_PI / 4.0)));
        m_16apsk[2][i] = gr_complex((r2 * cos(3 * M_PI / 4.0)), (r2 * sin(3 * M_PI / 4.0)));
        m_16apsk[3][i] = gr_complex((r2 * cos(-3 * M_PI / 4.0)), (r2 * sin(-3 * M_PI / 4.0)));
        m_16apsk[4][i] = gr_complex((r2 * cos(M_PI / 12.0)), (r2 * sin(M_PI / 12.0)));
        m_16apsk[5][i] = gr_complex((r2 * cos(-M_PI / 12.0)), (r2 * sin(-M_PI / 12.0)));
        m_16apsk[6][i] = gr_complex((r2 * cos(11 * M_PI / 12.0)), (r2 * sin(11 * M_PI / 12.0)));
        m_16apsk[7][i] = gr_complex((r2 * cos(-11 * M_PI / 12.0)), (r2 * sin(-11 * M_PI / 12.0)));
        m_16apsk[8][i] = gr_complex((r2 * cos(5 * M_PI / 12.0)), (r2 * sin(5 * M_PI / 12.0)));
        m_16apsk[9][i] = gr_complex((r2 * cos(-5 * M_PI / 12.0)), (r2 * sin(-5 * M_PI / 12.0)));
        m_16apsk[10][i] = gr_complex((r2 * cos(7 * M_PI / 12.0)), (r2 * sin(7 * M_PI / 12.0)));
        m_16apsk[11][i] = gr_complex((r2 * cos(-7 * M_PI / 12.0)), (r2 * sin(-7 * M_PI / 12.0)));
        m_16apsk[12][i] = gr_complex((r1 * cos(M_PI / 4.0)), (r1 * sin(M_PI / 4.0)));
        m_16apsk[13][i] = gr_complex((r1 * cos(-M_PI / 4.0)), (r1 * sin(-M_PI / 4.0)));
        m_16apsk[14][i] = gr_complex((r1 * cos(3 * M_PI / 4.0)), (r1 * sin(3 * M_PI / 4.0)));
        m_16apsk[15][i] = gr_complex((r1 * cos(-3 * M_PI / 4.0)), (r1 * sin(-3 * M_PI / 4.0)));
      }

      m_8_8apsk[0][0] = gr_complex(0.4718, 0.2606);
      m_8_8apsk[1][0] = gr_complex(0.2606, 0.4718);
      m_8_8apsk[2][0] = gr_complex(-0.4718, 0.2606);
      m_8_8apsk[3][0] = gr_complex(-0.2606, 0.4718);
      m_8_8apsk[4][0] = gr_complex(0.4718, -0.2606);
      m_8_8apsk[5][0] = gr_complex(0.2606, -0.4718);
      m_8_8apsk[6][0] = gr_complex(-0.4718, -0.2606);
      m_8_8apsk[7][0] = gr_complex(-0.2606, -0.4718);
      m_8_8apsk[8][0] = gr_complex(1.2088, 0.4984);
      m_8_8apsk[9][0] = gr_complex(0.4984, 1.2088);
      m_8_8apsk[10][0] = gr_complex(-1.2088, 0.4984);
      m_8_8apsk[11][0] = gr_complex(-0.4984, 1.2088);
      m_8_8apsk[12][0] = gr_complex(1.2088, -0.4984);
      m_8_8apsk[13][0] = gr_complex(0.4984, -1.2088);
      m_8_8apsk[14][0] = gr_complex(-1.2088, -0.4984);
      m_8_8apsk[15][0] = gr_complex(-0.4984, -1.2088);

      m_8_8apsk[0][1] = gr_complex(0.5061, 0.2474);
      m_8_8apsk[1][1] = gr_complex(0.2474, 0.5061);
      m_8_8apsk[2][1] = gr_complex(-0.5061, 0.2474);
      m_8_8apsk[3][1] = gr_complex(-0.2474, 0.5061);
      m_8_8apsk[4][1] = gr_complex(0.5061, -0.2474);
      m_8_8apsk[5][1] = gr_complex(0.2474, -0.5061);
      m_8_8apsk[6][1] = gr_complex(-0.5061, -0.2474);
      m_8_8apsk[7][1] = gr_complex(-0.2474, -0.5061);
      m_8_8apsk[8][1] = gr_complex(1.2007, 0.4909);
      m_8_8apsk[9][1] = gr_complex(0.4909, 1.2007);
      m_8_8apsk[10][1] = gr_complex(-1.2007, 0.4909);
      m_8_8apsk[11][1] = gr_complex(-0.4909, 1.2007);
      m_8_8apsk[12][1] = gr_complex(1.2007, -0.4909);
      m_8_8apsk[13][1] = gr_complex(0.4909, -1.2007);
      m_8_8apsk[14][1] = gr_complex(-1.2007, -0.4909);
      m_8_8apsk[15][1] = gr_complex(-0.4909, -1.2007);

      r2 = m;
      r1 = r2 / 2.19;
      m_8_8apsk[0][2] = gr_complex((r1 * cos(M_PI / 8.0)), (r1 * sin(M_PI / 8.0)));
      m_8_8apsk[1][2] = gr_complex((r1 * cos(3 * M_PI / 8.0)), (r1 * sin(3 * M_PI / 8.0)));
      m_8_8apsk[2][2] = gr_complex((r1 * cos(7 * M_PI / 8.0)), (r1 * sin(7 * M_PI / 8.0)));
      m_8_8apsk[3][2] = gr_complex((r1 * cos(5 * M_PI / 8.0)), (r1 * sin(5 * M_PI / 8.0)));
      m_8_8apsk[4][2] = gr_complex((r1 * cos(15 * M_PI / 8.0)), (r1 * sin(15 * M_PI / 8.0)));
      m_8_8apsk[5][2] = gr_complex((r1 * cos(13 * M_PI / 8.0)), (r1 * sin(13 * M_PI / 8.0)));
      m_8_8apsk[6][2] = gr_complex((r1 * cos(9 * M_PI / 8.0)), (r1 * sin(9 * M_PI / 8.0)));
      m_8_8apsk[7][2] = gr_complex((r1 * cos(11 * M_PI / 8.0)), (r1 * sin(11 * M_PI / 8.0)));
      m_8_8apsk[8][2] = gr_complex((r2 * cos(M_PI / 8.0)), (r2 * sin(M_PI / 8.0)));
      m_8_8apsk[9][2] = gr_complex((r2 * cos(3 * M_PI / 8.0)), (r2 * sin(3 * M_PI / 8.0)));
      m_8_8apsk[10][2] = gr_complex((r2 * cos(7 * M_PI / 8.0)), (r2 * sin(7 * M_PI / 8.0)));
      m_8_8apsk[11][2] = gr_complex((r2 * cos(5 * M_PI / 8.0)), (r2 * sin(5 * M_PI / 8.0)));
      m_8_8apsk[12][2] = gr_complex((r2 * cos(15 * M_PI / 8.0)), (r2 * sin(15 * M_PI / 8.0)));
      m_8_8apsk[13][2] = gr_complex((r2 * cos(13 * M_PI / 8.0)), (r2 * sin(13 * M_PI / 8.0)));
      m_8_8apsk[14][2] = gr_complex((r2 * cos(9 * M_PI / 8.0)), (r2 * sin(9 * M_PI / 8.0)));
      m_8_8apsk[15][2] = gr_complex((r2 * cos(11 * M_PI / 8.0)), (r2 * sin(11 * M_PI / 8.0)));


      float m_32apsk_radius[5][2] = {{5.27, 2.84}, {4.87, 2.72}, {4.64, 2.64}, {4.33, 2.54}, {4.30, 2.53}};
      r3 = m;
      for (int i = 0; i < 5; i++) {
        r1 = r3 / m_32apsk_radius[i][0];
        r2 = r1 * m_32apsk_radius[i][1];
        m_32apsk[0][i] = gr_complex((r2 * cos(M_PI / 4.0)), (r2 * sin(M_PI / 4.0)));
        m_32apsk[1][i] = gr_complex((r2 * cos(5 * M_PI / 12.0)), (r2 * sin(5 * M_PI / 12.0)));
        m_32apsk[2][i] = gr_complex((r2 * cos(-M_PI / 4.0)), (r2 * sin(-M_PI / 4.0)));
        m_32apsk[3][i] = gr_complex((r2 * cos(-5 * M_PI / 12.0)), (r2 * sin(-5 * M_PI / 12.0)));
        m_32apsk[4][i] = gr_complex((r2 * cos(3 * M_PI / 4.0)), (r2 * sin(3 * M_PI / 4.0)));
        m_32apsk[5][i] = gr_complex((r2 * cos(7 * M_PI / 12.0)), (r2 * sin(7 * M_PI / 12.0)));
        m_32apsk[6][i] = gr_complex((r2 * cos(-3 * M_PI / 4.0)), (r2 * sin(-3 * M_PI / 4.0)));
        m_32apsk[7][i] = gr_complex((r2 * cos(-7 * M_PI / 12.0)), (r2 * sin(-7 * M_PI / 12.0)));
        m_32apsk[8][i] = gr_complex((r3 * cos(M_PI / 8.0)), (r3 * sin(M_PI / 8.0)));
        m_32apsk[9][i] = gr_complex((r3 * cos(3 * M_PI / 8.0)), (r3 * sin(3 * M_PI / 8.0)));
        m_32apsk[10][i] = gr_complex((r3 * cos(-M_PI / 4.0)), (r3 * sin(-M_PI / 4.0)));
        m_32apsk[11][i] = gr_complex((r3 * cos(-M_PI / 2.0)), (r3 * sin(-M_PI / 2.0)));
        m_32apsk[12][i] = gr_complex((r3 * cos(3 * M_PI / 4.0)), (r3 * sin(3 * M_PI / 4.0)));
        m_32apsk[13][i] = gr_complex((r3 * cos(M_PI / 2.0)), (r3 * sin(M_PI / 2.0)));
        m_32apsk[14][i] = gr_complex((r3 * cos(-7 * M_PI / 8.0)), (r3 * sin(-7 * M_PI / 8.0)));
        m_32apsk[15][i] = gr_complex((r3 * cos(-5 * M_PI / 8.0)), (r3 * sin(-5 * M_PI / 8.0)));
        m_32apsk[16][i] = gr_complex((r2 * cos(M_PI / 12.0)), (r2 * sin(M_PI / 12.0)));
        m_32apsk[17][i] = gr_complex((r1 * cos(M_PI / 4.0)), (r1 * sin(M_PI / 4.0)));
        m_32apsk[18][i] = gr_complex((r2 * cos(-M_PI / 12.0)), (r2 * sin(-M_PI / 12.0)));
        m_32apsk[19][i] = gr_complex((r1 * cos(-M_PI / 4.0)), (r1 * sin(-M_PI / 4.0)));
        m_32apsk[20][i] = gr_complex((r2 * cos(11 * M_PI / 12.0)), (r2 * sin(11 * M_PI / 12.0)));
        m_32apsk[21][i] = gr_complex((r1 * cos(3 * M_PI / 4.0)), (r1 * sin(3 * M_PI / 4.0)));
        m_32apsk[22][i] = gr_complex((r2 * cos(-11 * M_PI / 12.0)), (r2 * sin(-11 * M_PI / 12.0)));
        m_32apsk[23][i] = gr_complex((r1 * cos(-3 * M_PI / 4.0)), (r1 * sin(-3 * M_PI / 4.0)));
        m_32apsk[24][i] = gr_complex((r3 * cos(0.0)), (r3 * sin(0.0)));
        m_32apsk[25][i] = gr_complex((r3 * cos(M_PI / 4.0)), (r3 * sin(M_PI / 4.0)));
        m_32apsk[26][i] = gr_complex((r3 * cos(-M_PI / 8.0)), (r3 * sin(-M_PI / 8.0)));
        m_32apsk[27][i] = gr_complex((r3 * cos(-3 * M_PI / 8.0)), (r3 * sin(-3 * M_PI / 8.0)));
        m_32apsk[28][i] = gr_complex((r3 * cos(7 * M_PI / 8.0)), (r3 * sin(7 * M_PI / 8.0)));
        m_32apsk[29][i] = gr_complex((r3 * cos(5 * M_PI / 8.0)), (r3 * sin(5 * M_PI / 8.0)));
        m_32apsk[30][i] = gr_complex((r3 * cos(M_PI)), (r3 * sin(M_PI)));
        m_32apsk[31][i] = gr_complex((r3 * cos(-3 * M_PI / 4.0)), (r3 * sin(-3 * M_PI / 4.0)));
      }

      float m_4_12_16apsk_radius[3][2] = {{5.55, 2.85}, {5.54, 2.84}, {5.26, 2.84}};
      r3 = m;
      for (int i = 0; i < 3; i++) {
        r1 = r3 / m_4_12_16apsk_radius[i][0];
        r2 = r1 * m_4_12_16apsk_radius[i][1];
        m_4_12_16apsk[0][i] = gr_complex((r3 * cos(11 * M_PI / 16.0)), (r3 * sin(11 * M_PI / 16.0)));
        m_4_12_16apsk[1][i] = gr_complex((r3 * cos(9 * M_PI / 16.0)), (r3 * sin(9 * M_PI / 16.0)));
        m_4_12_16apsk[2][i] = gr_complex((r3 * cos(5 * M_PI / 16.0)), (r3 * sin(5 * M_PI / 16.0)));
        m_4_12_16apsk[3][i] = gr_complex((r3 * cos(7 * M_PI / 16.0)), (r3 * sin(7 * M_PI / 16.0)));
        m_4_12_16apsk[4][i] = gr_complex((r2 * cos(3 * M_PI / 4.0)), (r2 * sin(3 * M_PI / 4.0)));
        m_4_12_16apsk[5][i] = gr_complex((r2 * cos(7 * M_PI / 12.0)), (r2 * sin(7 * M_PI / 12.0)));
        m_4_12_16apsk[6][i] = gr_complex((r2 * cos(M_PI / 4.0)), (r2 * sin(M_PI / 4.0)));
        m_4_12_16apsk[7][i] = gr_complex((r2 * cos(5 * M_PI / 12.0)), (r2 * sin(5 * M_PI / 12.0)));
        m_4_12_16apsk[8][i] = gr_complex((r3 * cos(13 * M_PI / 16.0)), (r3 * sin(13 * M_PI / 16.0)));
        m_4_12_16apsk[9][i] = gr_complex((r3 * cos(15 * M_PI / 16.0)), (r3 * sin(15 * M_PI / 16.0)));
        m_4_12_16apsk[10][i] = gr_complex((r3 * cos(3 * M_PI / 16.0)), (r3 * sin(3 * M_PI / 16.0)));
        m_4_12_16apsk[11][i] = gr_complex((r3 * cos(M_PI / 16.0)), (r3 * sin(M_PI / 16.0)));
        m_4_12_16apsk[12][i] = gr_complex((r2 * cos(11 * M_PI / 12.0)), (r2 * sin(11 * M_PI / 12.0)));
        m_4_12_16apsk[13][i] = gr_complex((r1 * cos(3 * M_PI / 4.0)), (r1 * sin(3 * M_PI / 4.0)));
        m_4_12_16apsk[14][i] = gr_complex((r2 * cos(M_PI / 12.0)), (r2 * sin(M_PI / 12.0)));
        m_4_12_16apsk[15][i] = gr_complex((r1 * cos(M_PI / 4.0)), (r1 * sin(M_PI / 4.0)));
        m_4_12_16apsk[16][i] = gr_complex((r3 * cos(21 * M_PI / 16.0)), (r3 * sin(21 * M_PI / 16.0)));
        m_4_12_16apsk[17][i] = gr_complex((r3 * cos(23 * M_PI / 16.0)), (r3 * sin(23 * M_PI / 16.0)));
        m_4_12_16apsk[18][i] = gr_complex((r3 * cos(27 * M_PI / 16.0)), (r3 * sin(27 * M_PI / 16.0)));
        m_4_12_16apsk[19][i] = gr_complex((r3 * cos(25 * M_PI / 16.0)), (r3 * sin(25 * M_PI / 16.0)));
        m_4_12_16apsk[20][i] = gr_complex((r2 * cos(5 * M_PI / 4.0)), (r2 * sin(5 * M_PI / 4.0)));
        m_4_12_16apsk[21][i] = gr_complex((r2 * cos(17 * M_PI / 12.0)), (r2 * sin(17 * M_PI / 12.0)));
        m_4_12_16apsk[22][i] = gr_complex((r2 * cos(7 * M_PI / 4.0)), (r2 * sin(7 * M_PI / 4.0)));
        m_4_12_16apsk[23][i] = gr_complex((r2 * cos(19 * M_PI / 12.0)), (r2 * sin(19 * M_PI / 12.0)));
        m_4_12_16apsk[24][i] = gr_complex((r3 * cos(19 * M_PI / 16.0)), (r3 * sin(19 * M_PI / 16.0)));
        m_4_12_16apsk[25][i] = gr_complex((r3 * cos(17 * M_PI / 16.0)), (r3 * sin(17 * M_PI / 16.0)));
        m_4_12_16apsk[26][i] = gr_complex((r3 * cos(29 * M_PI / 16.0)), (r3 * sin(29 * M_PI / 16.0)));
        m_4_12_16apsk[27][i] = gr_complex((r3 * cos(31 * M_PI / 16.0)), (r3 * sin(31 * M_PI / 16.0)));
        m_4_12_16apsk[28][i] = gr_complex((r2 * cos(13 * M_PI / 12.0)), (r2 * sin(13 * M_PI / 12.0)));
        m_4_12_16apsk[29][i] = gr_complex((r1 * cos(5 * M_PI / 4.0)), (r1 * sin(5 * M_PI / 4.0)));
        m_4_12_16apsk[30][i] = gr_complex((r2 * cos(23 * M_PI / 12.0)), (r2 * sin(23 * M_PI / 12.0)));
        m_4_12_16apsk[31][i] = gr_complex((r1 * cos(7 * M_PI / 4.0)), (r1 * sin(7 * M_PI / 4.0)));
      }

      float m_4_8_4_16apsk_radius[3][3] = {{5.60, 2.99, 2.60}, {5.60, 2.86, 2.60}, {5.60, 3.08, 2.60}};
      r4 = m;
      for (int i = 0; i < 3; i++) {
        r1 = r4 / m_4_8_4_16apsk_radius[i][0];
        r3 = r1 * m_4_8_4_16apsk_radius[i][1];
        r2 = r1 * m_4_8_4_16apsk_radius[i][2];
        m_4_8_4_16apsk[0][i] = gr_complex((r1 * cos(M_PI / 4.0)), (r1 * sin(M_PI / 4.0)));
        m_4_8_4_16apsk[1][i] = gr_complex((r4 * cos(7 * M_PI / 16.0)), (r4 * sin(7 * M_PI / 16.0)));
        m_4_8_4_16apsk[2][i] = gr_complex((r1 * cos(7 * M_PI / 4.0)), (r1 * sin(7 * M_PI / 4.0)));
        m_4_8_4_16apsk[3][i] = gr_complex((r4 * cos(25 * M_PI / 16.0)), (r4 * sin(25 * M_PI / 16.0)));
        m_4_8_4_16apsk[4][i] = gr_complex((r1 * cos(3 * M_PI / 4.0)), (r1 * sin(3 * M_PI / 4.0)));
        m_4_8_4_16apsk[5][i] = gr_complex((r4 * cos(9 * M_PI / 16.0)), (r4 * sin(9 * M_PI / 16.0)));
        m_4_8_4_16apsk[6][i] = gr_complex((r1 * cos(5 * M_PI / 4.0)), (r1 * sin(5 * M_PI / 4.0)));
        m_4_8_4_16apsk[7][i] = gr_complex((r4 * cos(23 * M_PI / 16.0)), (r4 * sin(23 * M_PI / 16.0)));
        m_4_8_4_16apsk[8][i] = gr_complex((r2 * cos(M_PI / 12.0)), (r2 * sin(M_PI / 12.0)));
        m_4_8_4_16apsk[9][i] = gr_complex((r4 * cos(M_PI / 16.0)), (r4 * sin(M_PI / 16.0)));
        m_4_8_4_16apsk[10][i] = gr_complex((r2 * cos(23 * M_PI / 12.0)), (r2 * sin(23 * M_PI / 12.0)));
        m_4_8_4_16apsk[11][i] = gr_complex((r4 * cos(31 * M_PI / 16.0)), (r4 * sin(31 * M_PI / 16.0)));
        m_4_8_4_16apsk[12][i] = gr_complex((r2 * cos(11 * M_PI / 12.0)), (r2 * sin(11 * M_PI / 12.0)));
        m_4_8_4_16apsk[13][i] = gr_complex((r4 * cos(15 * M_PI / 16.0)), (r4 * sin(15 * M_PI / 16.0)));
        m_4_8_4_16apsk[14][i] = gr_complex((r2 * cos(13 * M_PI / 12.0)), (r2 * sin(13 * M_PI / 12.0)));
        m_4_8_4_16apsk[15][i] = gr_complex((r4 * cos(17 * M_PI / 16.0)), (r4 * sin(17 * M_PI / 16.0)));
        m_4_8_4_16apsk[16][i] = gr_complex((r2 * cos(5 * M_PI / 12.0)), (r2 * sin(5 * M_PI / 12.0)));
        m_4_8_4_16apsk[17][i] = gr_complex((r4 * cos(5 * M_PI / 16.0)), (r4 * sin(5 * M_PI / 16.0)));
        m_4_8_4_16apsk[18][i] = gr_complex((r2 * cos(19 * M_PI / 12.0)), (r2 * sin(19 * M_PI / 12.0)));
        m_4_8_4_16apsk[19][i] = gr_complex((r4 * cos(27 * M_PI / 16.0)), (r4 * sin(27 * M_PI / 16.0)));
        m_4_8_4_16apsk[20][i] = gr_complex((r2 * cos(7 * M_PI / 12.0)), (r2 * sin(7 * M_PI / 12.0)));
        m_4_8_4_16apsk[21][i] = gr_complex((r4 * cos(11 * M_PI / 16.0)), (r4 * sin(11 * M_PI / 16.0)));
        m_4_8_4_16apsk[22][i] = gr_complex((r2 * cos(17 * M_PI / 12.0)), (r2 * sin(17 * M_PI / 12.0)));
        m_4_8_4_16apsk[23][i] = gr_complex((r4 * cos(21 * M_PI / 16.0)), (r4 * sin(21 * M_PI / 16.0)));
        m_4_8_4_16apsk[24][i] = gr_complex((r3 * cos(M_PI / 4.0)), (r3 * sin(M_PI / 4.0)));
        m_4_8_4_16apsk[25][i] = gr_complex((r4 * cos(3 * M_PI / 16.0)), (r4 * sin(3 * M_PI / 16.0)));
        m_4_8_4_16apsk[26][i] = gr_complex((r3 * cos(7 * M_PI / 4.0)), (r3 * sin(7 * M_PI / 4.0)));
        m_4_8_4_16apsk[27][i] = gr_complex((r4 * cos(29 * M_PI / 16.0)), (r4 * sin(29 * M_PI / 16.0)));
        m_4_8_4_16apsk[28][i] = gr_complex((r3 * cos(3 * M_PI / 4.0)), (r3 * sin(3 * M_PI / 4.0)));
        m_4_8_4_16apsk[29][i] = gr_complex((r4 * cos(13 * M_PI / 16.0)), (r4 * sin(13 * M_PI / 16.0)));
        m_4_8_4_16apsk[30][i] = gr_complex((r3 * cos(5 * M_PI / 4.0)), (r3 * sin(5 * M_PI / 4.0)));
        m_4_8_4_16apsk[31][i] = gr_complex((r4 * cos(19 * M_PI / 16.0)), (r4 * sin(19 * M_PI / 16.0)));
      }

      set_tag_propagation_policy(TPP_DONT);
      set_output_multiple(FRAME_SIZE_NORMAL);
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

    void
    modulator_bc_impl::get_items(dvbs2_framesize_t framesize, dvbs2_code_rate_t rate, dvbs2_constellation_t constellation, int *num_items, int *constellation_index)
    {
      switch (constellation) {
        case MOD_BPSK:
          *num_items = FRAME_SIZE_SHORT - SHORT_PUNCTURING_SET2;
          break;
        case MOD_BPSK_SF2:
          *num_items = FRAME_SIZE_SHORT - SHORT_PUNCTURING_SET1;
          break;
        case MOD_QPSK:
          if (rate == C2_9_VLSNR) {
            *num_items = (FRAME_SIZE_NORMAL - NORMAL_PUNCTURING) / 2;
          }
          else {
            if (framesize == FECFRAME_NORMAL) {
              *num_items = FRAME_SIZE_NORMAL / 2;
            }
            else {
              *num_items = FRAME_SIZE_SHORT / 2;
            }
          }
          break;
        case MOD_8PSK:
          if (framesize == FECFRAME_NORMAL) {
            *num_items = FRAME_SIZE_NORMAL / 3;
          }
          else {
            *num_items = FRAME_SIZE_SHORT / 3;
          }
          break;
        case MOD_8APSK:
          if (framesize == FECFRAME_NORMAL) {
            *num_items = FRAME_SIZE_NORMAL / 3;
          }
          else {
            *num_items = FRAME_SIZE_SHORT / 3;
          }
          switch(rate) {
            case C100_180:
              *constellation_index = 0;
              break;
            case C104_180:
              *constellation_index = 1;
              break;
            default:
              break;
          }
          break;
        case MOD_16APSK:
          if (framesize == FECFRAME_NORMAL) {
            *num_items = FRAME_SIZE_NORMAL / 4;
          }
          else {
            *num_items = FRAME_SIZE_SHORT / 4;
          }
          if (framesize == FECFRAME_NORMAL) {
            switch(rate) {
              case C2_3:
                *constellation_index = 5;
                break;
              case C3_4:
                *constellation_index = 7;
                break;
              case C4_5:
                *constellation_index = 8;
                break;
              case C5_6:
                *constellation_index = 9;
                break;
              case C8_9:
                *constellation_index = 10;
                break;
              case C9_10:
                *constellation_index = 11;
                break;
              case C26_45:
              case C3_5:
                *constellation_index = 0;
                break;
              case C28_45:
                *constellation_index = 2;
                break;
              case C23_36:
              case C25_36:
                *constellation_index = 6;
                break;
              case C13_18:
                *constellation_index = 7;
                break;
              case C140_180:
                *constellation_index = 1;
                break;
              case C154_180:
                *constellation_index = 4;
                break;
              default:
                break;
            }
          }
          else {
            switch(rate) {
              case C2_3:
                *constellation_index = 5;
                break;
              case C3_4:
                *constellation_index = 7;
                break;
              case C4_5:
                *constellation_index = 8;
                break;
              case C5_6:
                *constellation_index = 9;
                break;
              case C8_9:
                *constellation_index = 10;
                break;
              case C7_15:
                *constellation_index = 3;
                break;
              case C8_15:
                *constellation_index = 2;
                break;
              case C26_45:
              case C3_5:
                *constellation_index = 0;
                break;
              case C32_45:
                *constellation_index = 7;
                break;
              default:
                *constellation_index = 0;
                break;
            }
          }
          break;
        case MOD_8_8APSK:
          if (framesize == FECFRAME_NORMAL) {
            *num_items = FRAME_SIZE_NORMAL / 4;
          }
          else {
            *num_items = FRAME_SIZE_SHORT / 4;
          }
          if (rate == C18_30) {
            *constellation_index = 0;
          }
          else if (rate == C20_30) {
            *constellation_index = 1;
          }
          else {
            switch(rate) {
              case C90_180:
              case C96_180:
              case C100_180:
                *constellation_index = 2;
                break;
              default:
                break;
            }
          }
          break;
        case MOD_32APSK:
          if (framesize == FECFRAME_NORMAL) {
            *num_items = FRAME_SIZE_NORMAL / 5;
          }
          else {
            *num_items = FRAME_SIZE_SHORT / 5;
          }
          switch(rate) {
            case C3_4:
              *constellation_index = 0;
              break;
            case C4_5:
              *constellation_index = 1;
              break;
            case C5_6:
              *constellation_index = 2;
              break;
            case C8_9:
              *constellation_index = 3;
              break;
            case C9_10:
              *constellation_index = 4;
              break;
            default:
              *constellation_index = 0;
              break;
          }
          break;
        case MOD_4_12_16APSK:
          if (framesize == FECFRAME_NORMAL) {
            *num_items = FRAME_SIZE_NORMAL / 5;
          }
          else {
            *num_items = FRAME_SIZE_SHORT / 5;
          }
          if (framesize == FECFRAME_NORMAL) {
            switch(rate) {
              case C2_3:
                *constellation_index = 0;
                break;
              default:
                break;
            }
          }
          else {
            switch(rate) {
              case C2_3:
                *constellation_index = 1;
                break;
              case C32_45:
                *constellation_index = 2;
                break;
              default:
                break;
            }
          }
          break;
        case MOD_4_8_4_16APSK:
          if (framesize == FECFRAME_NORMAL) {
            *num_items = FRAME_SIZE_NORMAL / 5;
          }
          else {
            *num_items = FRAME_SIZE_SHORT / 5;
          }
          switch(rate) {
            case C128_180:
              *constellation_index = 0;
              break;
            case C132_180:
              *constellation_index = 1;
              break;
            case C140_180:
              *constellation_index = 2;
              break;
            default:
              break;
          }
          break;
        default:
          *num_items = 0;
          *constellation_index = 0;
          break;
      }
    }

    int
    modulator_bc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];
      int produced = 0;
      int index, num_items, constellation_index;
      dvbs2_framesize_t framesize;
      dvbs2_code_rate_t rate;
      dvbs2_constellation_t constellation;
      dvbs2_pilots_t pilots;
      unsigned int goldcode;
      static unsigned int check = 0;

      std::vector<tag_t> tags;
      const uint64_t nread = this->nitems_read(0); //number of items read on port 0

      // Read all tags on the input buffer
      this->get_tags_in_range(tags, 0, nread, nread + noutput_items, pmt::string_to_symbol("modcod"));

      for (int i = 0; i < (int)tags.size(); i++) {
        framesize = (dvbs2_framesize_t)((pmt::to_long(tags[i].value)) & 0xff);
        rate = (dvbs2_code_rate_t)(((pmt::to_long(tags[i].value)) >> 8) & 0xff);
        constellation = (dvbs2_constellation_t)(((pmt::to_long(tags[i].value)) >> 16) & 0xff);
        pilots = (dvbs2_pilots_t)(((pmt::to_long(tags[i].value)) >> 24) & 0xff);
        goldcode = (unsigned int)(((pmt::to_long(tags[i].value)) >> 32) & 0x3ffff);
        get_items(framesize, rate, constellation, &num_items, &constellation_index);
        if (produced <= noutput_items) {
          if (goldcode != check) {
            printf("modulator index = %d, %d\n", goldcode, check);
            check = goldcode + 1;
          }
          else {
            check++;
          }
          const uint64_t tagoffset = this->nitems_written(0);
          const uint64_t tagmodcod = (uint64_t(goldcode) << 32) | (uint64_t(pilots) << 24) | (uint64_t(constellation) << 16) | (uint64_t(rate) << 8) | uint64_t(framesize);
          pmt::pmt_t key = pmt::string_to_symbol("modcod");
          pmt::pmt_t value = pmt::from_long(tagmodcod);
          this->add_item_tag(0, tagoffset, key, value);

          switch (constellation) {
            case MOD_BPSK:
            case MOD_BPSK_SF2:
              for (int j = 0; j < num_items; j++) {
                index = *in++;
                *out++ = m_bpsk[i & 1][index & 0x1];
              }
              break;
            case MOD_QPSK:
              for (int j = 0; j < num_items; j++) {
                index = *in++;
                *out++ = m_qpsk[index & 0x3];
              }
              break;
            case MOD_8PSK:
              for (int j = 0; j < num_items; j++) {
                index = *in++;
                *out++ = m_8psk[index & 0x7];
              }
              break;
            case MOD_8APSK:
              for (int j = 0; j < num_items; j++) {
                index = *in++;
                *out++ = m_8apsk[index & 0x7][constellation_index];
              }
              break;
            case MOD_16APSK:
              for (int j = 0; j < num_items; j++) {
                index = *in++;
                *out++ = m_16apsk[index & 0xf][constellation_index];
              }
              break;
            case MOD_8_8APSK:
              for (int j = 0; j < num_items; j++) {
                index = *in++;
                *out++ = m_8_8apsk[index & 0xf][constellation_index];
              }
              break;
            case MOD_32APSK:
              for (int j = 0; j < num_items; j++) {
                index = *in++;
                *out++ = m_32apsk[index & 0x1f][constellation_index];
              }
              break;
            case MOD_4_12_16APSK:
              for (int j = 0; j < num_items; j++) {
                index = *in++;
                *out++ = m_4_12_16apsk[index & 0x1f][constellation_index];
              }
              break;
            case MOD_4_8_4_16APSK:
              for (int j = 0; j < num_items; j++) {
                index = *in++;
                *out++ = m_4_8_4_16apsk[index & 0x1f][constellation_index];
              }
              break;
            default:
              for (int j = 0; j < num_items; j++) {
                index = *in++;
                *out++ = m_qpsk[index & 0x3];
              }
              break;
          }
          produced += num_items;
          produce(0, num_items);
        }
        else {
          break;
        }
      }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (produced);

      // Tell runtime system how many output items we produced.
      return WORK_CALLED_PRODUCE;
    }

  } /* namespace dvbs2 */
} /* namespace gr */

