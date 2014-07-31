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

#ifndef INCLUDED_DVBS2_PHYSICAL_CC_IMPL_H
#define INCLUDED_DVBS2_PHYSICAL_CC_IMPL_H

#include <dvbs2/physical_cc.h>

namespace gr {
  namespace dvbs2 {

    class physical_cc_impl : public physical_cc
    {
     private:
      int frame_size;
      int slots;
      int pilot_mode;
      int pilot_symbols;
      gr_complex m_bpsk[2][2];
      gr_complex m_pl[90];
      gr_complex m_zero[1];
      int m_cscram[FRAME_SIZE_NORMAL];
      void b_64_8_code(unsigned char, int *);
      void pl_header_encode(unsigned char, unsigned char, int *);
      int parity_chk(long, long);
      void build_symbol_scrambler_table(void);

      const static unsigned long g[7];
      const static int ph_scram_tab[64];
      const static int ph_sync_seq[26];

     public:
      physical_cc_impl(dvbs2_constellation_t constellation, dvbs2_code_rate_t rate, dvbs2_pilots_t pilots, dvbs2_framesize_t framesize);
      ~physical_cc_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace dvbs2
} // namespace gr

#endif /* INCLUDED_DVBS2_PHYSICAL_CC_IMPL_H */

