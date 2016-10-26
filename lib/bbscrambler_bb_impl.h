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

#ifndef INCLUDED_DVBS2_BBSCRAMBLER_BB_IMPL_H
#define INCLUDED_DVBS2_BBSCRAMBLER_BB_IMPL_H

#include <dvbs2/bbscrambler_bb.h>

namespace gr {
  namespace dvbs2 {

    class bbscrambler_bb_impl : public bbscrambler_bb
    {
     private:
      unsigned char bb_randomise[FRAME_SIZE_NORMAL];
      void get_kbch(dvbs2_framesize_t, dvbs2_code_rate_t, unsigned int *);
      void init_bb_randomiser(void);

     public:
      bbscrambler_bb_impl();
      ~bbscrambler_bb_impl();

      int work(int noutput_items,
               gr_vector_const_void_star &input_items,
               gr_vector_void_star &output_items);
    };

  } // namespace dvbs2
} // namespace gr

#endif /* INCLUDED_DVBS2_BBSCRAMBLER_BB_IMPL_H */

