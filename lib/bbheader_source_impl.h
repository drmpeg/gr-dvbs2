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

#ifndef INCLUDED_DVBS2_BBHEADER_SOURCE_IMPL_H
#define INCLUDED_DVBS2_BBHEADER_SOURCE_IMPL_H

#include <dvbs2/bbheader_source.h>
#include <pcap.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <linux/if_tun.h>
#include <boost/asio.hpp>

#define START_INDICATOR_SIZE 1
#define END_INDICATOR_SIZE 1
#define LABEL_TYPE_INDICATOR_SIZE 2
#define GSE_LENGTH_SIZE 12

#define HEADER_SIZE ((START_INDICATOR_SIZE + END_INDICATOR_SIZE + LABEL_TYPE_INDICATOR_SIZE + GSE_LENGTH_SIZE) / 8)
#define FRAG_ID_SIZE 1
#define TOTAL_LENGTH_SIZE 2
#define MAX_GSE_LENGTH 4096

#define NUM_STREAMS 4

typedef struct{
    int ts_gs;
    int sis_mis;
    int ccm_acm;
    int issyi;
    int npd;
    int ro;
    int isi;
    int upl;
    int dfl;
    int sync;
    int syncd;
}BBHeader;

typedef struct{
   BBHeader bb_header;
}FrameFormat;

namespace gr {
  namespace dvbs2 {

    class bbheader_source_impl : public bbheader_source
    {
     private:
      int num_streams;
      int stream;
      unsigned int kbch[NUM_STREAMS];
      unsigned int count[NUM_STREAMS];
      unsigned char crc[NUM_STREAMS];
      unsigned int frame_size[NUM_STREAMS];
      unsigned int code_rate[NUM_STREAMS];
      unsigned int signal_constellation[NUM_STREAMS];
      unsigned int pilot_mode[NUM_STREAMS];
      unsigned int gold_code[NUM_STREAMS];
      int ping_reply_mode;
      int ipaddr_spoof_mode;
      bool dvbs2x[NUM_STREAMS];
      bool alternate[NUM_STREAMS];
      bool nibble[NUM_STREAMS];
      FrameFormat m_format[1][NUM_STREAMS];
      unsigned char crc_tab[256];
      unsigned int crc32_table[256];
      pcap_t* descr[NUM_STREAMS];
      int fd[NUM_STREAMS];
      unsigned char *packet_ptr[NUM_STREAMS];
      bool packet_fragmented[NUM_STREAMS];
      int packet_length[NUM_STREAMS];
      bool last_packet_valid[NUM_STREAMS];
      const unsigned char *packet[NUM_STREAMS];
      unsigned char frag_id_pool;
      unsigned char frag_id[NUM_STREAMS];
      int crc32_partial[NUM_STREAMS];
      unsigned char src_addr[sizeof(in_addr)];
      unsigned char dst_addr[sizeof(in_addr)];
      void add_bbheader(unsigned char *, int, int, bool, int);
      void build_crc8_table(void);
      int add_crc8_bits(unsigned char *, int);
      void crc32_init(void);
      int crc32_calc(unsigned char *, int, int);
      int checksum(unsigned short *, int, int);
      inline void ping_reply(int);
      inline void ipaddr_spoof(int);
      inline void dump_packet(unsigned char *);

      unsigned char udp_packet[FRAME_SIZE_NORMAL / 8];
      unsigned char frame_number;
      bool d_connected;
      gr::thread::mutex d_mutex;
      boost::asio::ip::udp::socket *d_socket;
      boost::asio::ip::udp::endpoint d_endpoint;
      boost::asio::io_service d_io_service;
      void connect(const std::string &host, int port);
      void disconnect();

     public:
      bbheader_source_impl(int nstreams, dvbs2_framesize_t framesize1, dvbs2_code_rate_t rate1, dvbs2_constellation_t constellation1, dvbs2_pilots_t pilots1, int goldcode1, char *mac_address1, dvbs2_framesize_t framesize2, dvbs2_code_rate_t rate2, dvbs2_constellation_t constellation2, dvbs2_pilots_t pilots2, int goldcode2, char *mac_address2, dvbs2_framesize_t framesize3, dvbs2_code_rate_t rate3, dvbs2_constellation_t constellation3, dvbs2_pilots_t pilots3, int goldcode3, char *mac_address3, dvbs2_framesize_t framesize4, dvbs2_code_rate_t rate4, dvbs2_constellation_t constellation4, dvbs2_pilots_t pilots4, int goldcode4, char *mac_address4, dvbs2_rolloff_factor_t rolloff, dvbs2_ping_reply_t ping_reply, dvbs2_ipaddr_spoof_t ipaddr_spoof, char *dst_address);
      ~bbheader_source_impl();

      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace dvbs2
} // namespace gr

#endif /* INCLUDED_DVBS2_BBHEADER_SOURCE_IMPL_H */

