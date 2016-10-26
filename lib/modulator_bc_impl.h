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

#ifndef INCLUDED_DVBS2_MODULATOR_BC_IMPL_H
#define INCLUDED_DVBS2_MODULATOR_BC_IMPL_H

#include <dvbs2/modulator_bc.h>

namespace gr {
  namespace dvbs2 {

    class modulator_bc_impl : public modulator_bc
    {
     private:
      gr_complex m_bpsk[2][2];
      gr_complex m_qpsk[4];
      gr_complex m_8psk[8];
      gr_complex m_8apsk[8][2];
      gr_complex m_16apsk[16][12];
      gr_complex m_8_8apsk[16][3];
      gr_complex m_32apsk[32][5];
      gr_complex m_4_12_16apsk[32][3];
      gr_complex m_4_8_4_16apsk[32][3];
      void get_items(dvbs2_framesize_t, dvbs2_code_rate_t, dvbs2_constellation_t, int *, int *);

     public:
      modulator_bc_impl();
      ~modulator_bc_impl();

      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items);
    };

  } // namespace dvbs2
} // namespace gr

#endif /* INCLUDED_DVBS2_MODULATOR_BC_IMPL_H */

