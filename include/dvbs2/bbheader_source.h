/* -*- c++ -*- */
/* 
 * Copyright 2017 Ron Economos.
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

#ifndef INCLUDED_DVBS2_BBHEADER_SOURCE_H
#define INCLUDED_DVBS2_BBHEADER_SOURCE_H

#include <dvbs2/api.h>
#include <dvbs2/dvbs2_config.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace dvbs2 {

    /*!
     * \brief <+description of block+>
     * \ingroup dvbs2
     *
     */
    class DVBS2_API bbheader_source : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<bbheader_source> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of dvbs2::bbheader_source.
       *
       * To avoid accidental use of raw pointers, dvbs2::bbheader_source's
       * constructor is in a private implementation
       * class. dvbs2::bbheader_source::make is the public interface for
       * creating new instances.
       */
      static sptr make(int nstreams, dvbs2_framesize_t framesize1, dvbs2_code_rate_t rate1, dvbs2_constellation_t constellation1, dvbs2_pilots_t pilots1, int goldcode1, char *mac_address1, dvbs2_framesize_t framesize2, dvbs2_code_rate_t rate2, dvbs2_constellation_t constellation2, dvbs2_pilots_t pilots2, int goldcode2, char *mac_address2, dvbs2_framesize_t framesize3, dvbs2_code_rate_t rate3, dvbs2_constellation_t constellation3, dvbs2_pilots_t pilots3, int goldcode3, char *mac_address3, dvbs2_framesize_t framesize4, dvbs2_code_rate_t rate4, dvbs2_constellation_t constellation4, dvbs2_pilots_t pilots4, int goldcode4, char *mac_address4, dvbs2_rolloff_factor_t rolloff, dvbs2_ping_reply_t ping_reply, dvbs2_ipaddr_spoof_t ipaddr_spoof, char *dst_address);
    };

  } // namespace dvbs2
} // namespace gr

#endif /* INCLUDED_DVBS2_BBHEADER_SOURCE_H */

