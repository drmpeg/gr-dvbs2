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


#ifndef INCLUDED_DVBS2_BBHEADER_BB_H
#define INCLUDED_DVBS2_BBHEADER_BB_H

#include <dvbs2/api.h>
#include <dvbs2/dvbs2_config.h>
#include <gnuradio/block.h>

namespace gr {
  namespace dvbs2 {

    /*!
     * \brief Formats MPEG-2 Transport Stream packets into FEC baseband frames
     * and adds a 10-byte header.
     * \ingroup dvbs2
     *
     * \details
     * Input: 188-byte MPEG-2 Transport Stream packets.
     * Output: Variable length FEC baseband frames (BBFRAME). The output frame
     *         length is based on the FEC rate.
     */
    class DVBS2_API bbheader_bb : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<bbheader_bb> sptr;

      /*!
       * \brief Create a baseband header formatter.
       *
       * \param framesize FEC frame size (normal, medium or short).
       * \param rate FEC code rate.
       * \param rolloff DVB-S2 root-raised-cosine filter roll-off.
       */
      static sptr make(int nstreams, dvbs2_framesize_t framesize1, dvbs2_code_rate_t rate1, dvbs2_constellation_t constellation1, dvbs2_pilots_t pilots1, int goldcode1, dvbs2_framesize_t framesize2, dvbs2_code_rate_t rate2, dvbs2_constellation_t constellation2, dvbs2_pilots_t pilots2, int goldcode2, dvbs2_framesize_t framesize3, dvbs2_code_rate_t rate3, dvbs2_constellation_t constellation3, dvbs2_pilots_t pilots3, int goldcode3, dvbs2_framesize_t framesize4, dvbs2_code_rate_t rate4, dvbs2_constellation_t constellation4, dvbs2_pilots_t pilots4, int goldcode4, dvbs2_rolloff_factor_t rolloff);
    };

  } // namespace dvbs2
} // namespace gr

#endif /* INCLUDED_DVBS2_BBHEADER_BB_H */

