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

#ifndef INCLUDED_DVBS2_BCH_BB_IMPL_H
#define INCLUDED_DVBS2_BCH_BB_IMPL_H

#include <dvbs2/bch_bb.h>

namespace gr {
  namespace dvbs2 {

    class bch_bb_impl : public bch_bb
    {
     private:
      unsigned int m_poly_n_8[4];
      unsigned int m_poly_n_10[5];
      unsigned int m_poly_n_12[6];
      unsigned int m_poly_s_12[6];
      unsigned int m_poly_m_12[6];
      void get_kbch_nbch(dvbs2_framesize_t, dvbs2_code_rate_t, unsigned int *, unsigned int *, unsigned int *);
      int poly_mult(const int*, int, const int*, int, int*);
      void poly_pack(const int*, unsigned int*, int);
      void poly_reverse(int*, int*, int);
      inline void reg_4_shift(unsigned int*);
      inline void reg_5_shift(unsigned int*);
      inline void reg_6_shift(unsigned int*);
      void bch_poly_build_tables(void);

     public:
      bch_bb_impl();
      ~bch_bb_impl();

      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items);
    };

  } // namespace dvbs2
} // namespace gr

#endif /* INCLUDED_DVBS2_BCH_BB_IMPL_H */

