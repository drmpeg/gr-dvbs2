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
     * \brief Formats IP packets into FEC baseband frames and adds a 10-byte
     * header.
     * \ingroup dvbs2
     *
     * \details
     * Input: IP packets (through PCAP).
     * Output: Variable length FEC baseband frames (BBFRAME). The output frame
     *         length is based on the FEC rate.
     */
    class DVBS2_API bbheader_source : virtual public gr::sync_block
    {
     public:
      typedef std::shared_ptr<bbheader_source> sptr;

      /*!
       * \brief Create a GSE baseband header formatter.
       *
       * \param nstreams number of streams (1-4).
       * \param framesize1 FEC frame size (normal, medium or short).
       * \param rate1 FEC code rate.
       * \param constellation1 DVB-S2 constellation.
       * \param pilots1 pilot symbols (on/off).
       * \param goldcode1 physical layer scrambler Gold code (0 to 262141 inclusive).
       * \param mac_address1 MAC address of IP interface.
       * \param framesize2 FEC frame size (normal, medium or short).
       * \param rate2 FEC code rate.
       * \param constellation2 DVB-S2 constellation.
       * \param pilots2 pilot symbols (on/off).
       * \param goldcode2 physical layer scrambler Gold code (0 to 262141 inclusive).
       * \param mac_address2 MAC address of IP interface.
       * \param framesize3 FEC frame size (normal, medium or short).
       * \param rate3 FEC code rate.
       * \param constellation3 DVB-S2 constellation.
       * \param pilots3 pilot symbols (on/off).
       * \param goldcode3 physical layer scrambler Gold code (0 to 262141 inclusive).
       * \param mac_address3 MAC address of IP interface.
       * \param framesize4 FEC frame size (normal, medium or short).
       * \param rate4 FEC code rate.
       * \param constellation4 DVB-S2 constellation.
       * \param pilots4 pilot symbols (on/off).
       * \param goldcode4 physical layer scrambler Gold code (0 to 262141 inclusive).
       * \param mac_address4 MAC address of IP interface.
       * \param rolloff DVB-S2 root-raised-cosine filter roll-off.
       * \param ping_reply ping reply mode (on/off).
       * \param ipaddr_spoof IP address spoofing (on/off).
       * \param dst_address destination address for IP spoofing.
       */
      static sptr make(int nstreams, dvbs2_framesize_t framesize1, dvbs2_code_rate_t rate1, dvbs2_constellation_t constellation1, dvbs2_pilots_t pilots1, int goldcode1, char *mac_address1, dvbs2_framesize_t framesize2, dvbs2_code_rate_t rate2, dvbs2_constellation_t constellation2, dvbs2_pilots_t pilots2, int goldcode2, char *mac_address2, dvbs2_framesize_t framesize3, dvbs2_code_rate_t rate3, dvbs2_constellation_t constellation3, dvbs2_pilots_t pilots3, int goldcode3, char *mac_address3, dvbs2_framesize_t framesize4, dvbs2_code_rate_t rate4, dvbs2_constellation_t constellation4, dvbs2_pilots_t pilots4, int goldcode4, char *mac_address4, dvbs2_rolloff_factor_t rolloff, dvbs2_ping_reply_t ping_reply, dvbs2_ipaddr_spoof_t ipaddr_spoof, char *dst_address);
    };

  } // namespace dvbs2
} // namespace gr

#endif /* INCLUDED_DVBS2_BBHEADER_SOURCE_H */

