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


#ifndef INCLUDED_DVBS2_INTERLEAVER_BB_H
#define INCLUDED_DVBS2_INTERLEAVER_BB_H

#include <dvbs2/api.h>
#include <dvbs2/dvbs2_config.h>
#include <gnuradio/block.h>

namespace gr {
  namespace dvbs2 {

    /*!
     * \brief Bit interleaves DVB-S2 FEC baseband frames.
     * \ingroup dvbs2
     *
     * Input: Normal or short FEC baseband frames with appended LPDC (LDPCFEC).
     * Output: Bit interleaved baseband frames.
     */
    class DVBS2_API interleaver_bb : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<interleaver_bb> sptr;

      /*!
       * \brief Create a DVB-S2 bit interleaver.
       */
      static sptr make();
    };

  } // namespace dvbs2
} // namespace gr

#endif /* INCLUDED_DVBS2_INTERLEAVER_BB_H */

