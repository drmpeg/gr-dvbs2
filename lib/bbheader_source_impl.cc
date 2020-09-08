/* -*- c++ -*- */
/* 
 * Copyright 2017,2020 Ron Economos.
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
#include "bbheader_source_impl.h"
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/format.hpp>

#define DEFAULT_IF "tap0"
#define FILTER "ether src "
#undef DEBUG

namespace gr {
  namespace dvbs2 {

    bbheader_source::sptr
    bbheader_source::make(int nstreams, dvbs2_framesize_t framesize1, dvbs2_code_rate_t rate1, dvbs2_constellation_t constellation1, dvbs2_pilots_t pilots1, int goldcode1, char *mac_address1, dvbs2_framesize_t framesize2, dvbs2_code_rate_t rate2, dvbs2_constellation_t constellation2, dvbs2_pilots_t pilots2, int goldcode2, char *mac_address2, dvbs2_framesize_t framesize3, dvbs2_code_rate_t rate3, dvbs2_constellation_t constellation3, dvbs2_pilots_t pilots3, int goldcode3, char *mac_address3, dvbs2_framesize_t framesize4, dvbs2_code_rate_t rate4, dvbs2_constellation_t constellation4, dvbs2_pilots_t pilots4, int goldcode4, char *mac_address4, dvbs2_rolloff_factor_t rolloff, dvbs2_ping_reply_t ping_reply, dvbs2_ipaddr_spoof_t ipaddr_spoof, char *dst_address)
    {
      return gnuradio::get_initial_sptr
        (new bbheader_source_impl(nstreams, framesize1, rate1, constellation1, pilots1, goldcode1, mac_address1, framesize2, rate2, constellation2, pilots2, goldcode2, mac_address2, framesize3, rate3, constellation3, pilots3, goldcode3, mac_address3, framesize4, rate4, constellation4, pilots4, goldcode4, mac_address4, rolloff, ping_reply, ipaddr_spoof, dst_address));
    }

    /*
     * The private constructor
     */
    bbheader_source_impl::bbheader_source_impl(int nstreams, dvbs2_framesize_t framesize1, dvbs2_code_rate_t rate1, dvbs2_constellation_t constellation1, dvbs2_pilots_t pilots1, int goldcode1, char *mac_address1, dvbs2_framesize_t framesize2, dvbs2_code_rate_t rate2, dvbs2_constellation_t constellation2, dvbs2_pilots_t pilots2, int goldcode2, char *mac_address2, dvbs2_framesize_t framesize3, dvbs2_code_rate_t rate3, dvbs2_constellation_t constellation3, dvbs2_pilots_t pilots3, int goldcode3, char *mac_address3, dvbs2_framesize_t framesize4, dvbs2_code_rate_t rate4, dvbs2_constellation_t constellation4, dvbs2_pilots_t pilots4, int goldcode4, char *mac_address4, dvbs2_rolloff_factor_t rolloff, dvbs2_ping_reply_t ping_reply, dvbs2_ipaddr_spoof_t ipaddr_spoof, char *dst_address)
      : gr::sync_block("bbheader_source",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(unsigned char))),
        frame_number(0),
        d_connected(false)
    {
      char errbuf[PCAP_ERRBUF_SIZE];
      char dev[IFNAMSIZ];
      char default_if[NUM_STREAMS][5] = {"tap0", "tap1", "tap2", "tap3"};
      char mac_address[NUM_STREAMS][20];
      struct bpf_program fp;
      bpf_u_int32 netp = 0;
      char filter[50];
      struct ifreq ifr;
      int err;
      const std::string &host = "10.0.2.2";
      int port = 1234;

      connect(host, port);

      ping_reply_mode = ping_reply;
      ipaddr_spoof_mode = ipaddr_spoof;
      inet_pton(AF_INET, dst_address, &dst_addr);
      frag_id_pool = 1;

      num_streams = nstreams;
      stream = 0;
      for (int i = 0; i < nstreams; i++) {
        count[i] = 0;
        crc[i] = 0x0;
        dvbs2x[i] = FALSE;
        alternate[i] = TRUE;
        nibble[i] = TRUE;
        packet_fragmented[i] = FALSE;
        last_packet_valid[i] = FALSE;
        descr[i] = NULL;
        fd[i] = 0;
      }

      frame_size[0] = framesize1;
      code_rate[0] = rate1;
      signal_constellation[0] =  constellation1;
      pilot_mode[0] = pilots1;
      if (goldcode1 < 0 || goldcode1 > 262141) {
        GR_LOG_WARN(d_logger, "Gold Code 1 must be between 0 and 262141 inclusive.");
        GR_LOG_WARN(d_logger, "Gold Code 1 set to 0.");
        goldcode1 = 0;
      }
      gold_code[0] = goldcode1;
      strcpy(&mac_address[0][0], mac_address1);

      frame_size[1] = framesize2;
      code_rate[1] = rate2;
      signal_constellation[1] =  constellation2;
      pilot_mode[1] = pilots2;
      if (goldcode2 < 0 || goldcode2 > 262141) {
        GR_LOG_WARN(d_logger, "Gold Code 2 must be between 0 and 262141 inclusive.");
        GR_LOG_WARN(d_logger, "Gold Code 2 set to 0.");
        goldcode2 = 0;
      }
      gold_code[1] = goldcode2;
      strcpy(&mac_address[1][0], mac_address2);

      frame_size[2] = framesize3;
      code_rate[2] = rate3;
      signal_constellation[2] =  constellation3;
      pilot_mode[2] = pilots3;
      if (goldcode3 < 0 || goldcode3 > 262141) {
        GR_LOG_WARN(d_logger, "Gold Code 3 must be between 0 and 262141 inclusive.");
        GR_LOG_WARN(d_logger, "Gold Code 3 set to 0.");
        goldcode3 = 0;
      }
      gold_code[2] = goldcode3;
      strcpy(&mac_address[2][0], mac_address3);

      frame_size[3] = framesize4;
      code_rate[3] = rate4;
      signal_constellation[3] =  constellation4;
      pilot_mode[3] = pilots4;
      if (goldcode4 < 0 || goldcode4 > 262141) {
        GR_LOG_WARN(d_logger, "Gold Code 4 must be between 0 and 262141 inclusive.");
        GR_LOG_WARN(d_logger, "Gold Code 4 set to 0.");
        goldcode4 = 0;
      }
      gold_code[3] = goldcode4;
      strcpy(&mac_address[3][0], mac_address4);

      for (int i = 0; i < nstreams; i++) {
        if (frame_size[i] == FECFRAME_NORMAL) {
          switch (code_rate[i]) {
            case C1_4:
              kbch[i] = 16008;
              break;
            case C1_3:
              kbch[i] = 21408;
              break;
            case C2_5:
              kbch[i] = 25728;
              break;
            case C1_2:
              kbch[i] = 32208;
              break;
            case C3_5:
              kbch[i] = 38688;
              break;
            case C2_3:
              kbch[i] = 43040;
              break;
            case C3_4:
              kbch[i] = 48408;
              break;
            case C4_5:
              kbch[i] = 51648;
              break;
            case C5_6:
              kbch[i] = 53840;
              break;
            case C8_9:
              kbch[i] = 57472;
              break;
            case C9_10:
              kbch[i] = 58192;
              break;
            case C2_9_VLSNR:
              kbch[i] = 14208;
              break;
            case C13_45:
              kbch[i] = 18528;
              break;
            case C9_20:
              kbch[i] = 28968;
              break;
            case C90_180:
              kbch[i] = 32208;
              break;
            case C96_180:
              kbch[i] = 34368;
              break;
            case C11_20:
              kbch[i] = 35448;
              break;
            case C100_180:
              kbch[i] = 35808;
              break;
            case C104_180:
              kbch[i] = 37248;
              break;
            case C26_45:
              kbch[i] = 37248;
              break;
            case C18_30:
              kbch[i] = 38688;
              break;
            case C28_45:
              kbch[i] = 40128;
              break;
            case C23_36:
              kbch[i] = 41208;
              break;
            case C116_180:
              kbch[i] = 41568;
              break;
            case C20_30:
              kbch[i] = 43008;
              break;
            case C124_180:
              kbch[i] = 44448;
              break;
            case C25_36:
              kbch[i] = 44808;
              break;
            case C128_180:
              kbch[i] = 45888;
              break;
            case C13_18:
              kbch[i] = 46608;
              break;
            case C132_180:
              kbch[i] = 47328;
              break;
            case C22_30:
              kbch[i] = 47328;
              break;
            case C135_180:
              kbch[i] = 48408;
              break;
            case C140_180:
              kbch[i] = 50208;
              break;
            case C7_9:
              kbch[i] = 50208;
              break;
            case C154_180:
              kbch[i] = 55248;
              break;
            default:
              kbch[i] = 0;
              break;
          }
        }
        else if (frame_size[i] == FECFRAME_SHORT) {
          switch (code_rate[i]) {
            case C1_4:
              kbch[i] = 3072;
              break;
            case C1_3:
              kbch[i] = 5232;
              break;
            case C2_5:
              kbch[i] = 6312;
              break;
            case C1_2:
              kbch[i] = 7032;
              break;
            case C3_5:
              kbch[i] = 9552;
              break;
            case C2_3:
              kbch[i] = 10632;
              break;
            case C3_4:
              kbch[i] = 11712;
              break;
            case C4_5:
              kbch[i] = 12432;
              break;
            case C5_6:
              kbch[i] = 13152;
              break;
            case C8_9:
              kbch[i] = 14232;
              break;
            case C11_45:
              kbch[i] = 3792;
              break;
            case C4_15:
              kbch[i] = 4152;
              break;
            case C14_45:
              kbch[i] = 4872;
              break;
            case C7_15:
              kbch[i] = 7392;
              break;
            case C8_15:
              kbch[i] = 8472;
              break;
            case C26_45:
              kbch[i] = 9192;
              break;
            case C32_45:
              kbch[i] = 11352;
              break;
            case C1_5_VLSNR_SF2:
              kbch[i] = 2512;
              break;
            case C11_45_VLSNR_SF2:
              kbch[i] = 3792;
              break;
            case C1_5_VLSNR:
              kbch[i] = 3072;
              break;
            case C4_15_VLSNR:
              kbch[i] = 4152;
              break;
            case C1_3_VLSNR:
              kbch[i] = 5232;
              break;
            default:
              kbch[i] = 0;
              break;
          }
        }
        else {
          switch (code_rate[i]) {
            case C1_5_MEDIUM:
              kbch[i] = 5660;
              break;
            case C11_45_MEDIUM:
              kbch[i] = 7740;
              break;
            case C1_3_MEDIUM:
              kbch[i] = 10620;
              break;
            default:
              kbch[i] = 0;
              break;
          }
        }
      }

      for (int i = 0; i < nstreams; i++) {
        BBHeader *f = &m_format[0][i].bb_header;
        f->ts_gs   = TS_GS_GENERIC_CONTINUOUS;
        if (nstreams > 1) {
          f->sis_mis = SIS_MIS_MULTIPLE;
          f->ccm_acm = ACM;
        }
        else {
          f->sis_mis = SIS_MIS_SINGLE;
          f->ccm_acm = CCM;
        }
        f->issyi   = ISSYI_NOT_ACTIVE;
        f->npd     = NPD_NOT_ACTIVE;
        f->upl     = 0;
        f->dfl     = kbch[i] - 80;
        f->sync    = 0;
        if (rolloff & 0x4) {
          dvbs2x[i] = TRUE;
        }
        f->ro      = rolloff & 0x3;
      }

      build_crc8_table();
      crc32_init();

      for (int i = 0; i < nstreams; i++) {
        if ((fd[i] = open("/dev/net/tun", O_RDWR)) == -1) {
          throw std::runtime_error("Error calling open()\n");
        }
        memset(&ifr, 0, sizeof(ifr));
        ifr.ifr_flags = IFF_TAP;
        strncpy(ifr.ifr_name, &default_if[i][0], IFNAMSIZ);

        if ((err = ioctl(fd[i], TUNSETIFF, (void *) &ifr)) == -1) {
          printf("err = %d\n", err);
          close(fd[i]);
          throw std::runtime_error("Error calling ioctl()\n");
        }

        strcpy(dev, &default_if[i][0]);
        descr[i] = pcap_create(dev, errbuf);
        if (descr[i] == NULL) {
          std::stringstream s;
          s << "Error calling pcap_create(): " << errbuf << std::endl;
          throw std::runtime_error(s.str());
        }
        if (pcap_set_promisc(descr[i], 0) != 0) {
          pcap_close(descr[i]);
          throw std::runtime_error("Error calling pcap_set_promisc()\n");
        }
        if (pcap_set_timeout(descr[i], -1) != 0) {
          pcap_close(descr[i]);
          throw std::runtime_error("Error calling pcap_set_timeout()\n");
        }
        if (pcap_set_snaplen(descr[i], 65536) != 0) {
          pcap_close(descr[i]);
          throw std::runtime_error("Error calling pcap_set_snaplen()\n");
        }
        if (pcap_set_buffer_size(descr[i], 1024 * 1024 * 16) != 0) {
          pcap_close(descr[i]);
          throw std::runtime_error("Error calling pcap_set_buffer_size()\n");
        }
        if (pcap_activate(descr[i]) != 0) {
          pcap_close(descr[i]);
          throw std::runtime_error("Error calling pcap_activate()\n");
        }
        strcpy(filter, FILTER);
        strcat(filter, &mac_address[i][0]);
        if (pcap_compile(descr[i], &fp, filter, 0, netp) == -1) {
          pcap_close(descr[i]);
          throw std::runtime_error("Error calling pcap_compile()\n");
        }
        if (pcap_setfilter(descr[i], &fp) == -1) {
          pcap_close(descr[i]);
          throw std::runtime_error("Error calling pcap_setfilter()\n");
        }
      }

      set_output_multiple(FRAME_SIZE_NORMAL);
    }

    /*
     * Our virtual destructor.
     */
    bbheader_source_impl::~bbheader_source_impl()
    {
      for (int i = 0; i < num_streams; i++) {
        if (fd[i]) {
          close(fd[i]);
        }
      }
      for (int i = 0; i < num_streams; i++) {
        if (descr[i]) {
          pcap_close(descr[i]);
        }
      }
      if (d_connected)
        disconnect();
    }

    void
    bbheader_source_impl::connect(const std::string &host, int port)
    {
      if (d_connected)
        disconnect();

      std::string s_port = (boost::format("%d")%port).str();
      if (host.size() > 0) {
        boost::asio::ip::udp::resolver resolver(d_io_service);
        boost::asio::ip::udp::resolver::query query(host, s_port,
                                                    boost::asio::ip::resolver_query_base::passive);
        d_endpoint = *resolver.resolve(query);

        d_socket = new boost::asio::ip::udp::socket(d_io_service);
        d_socket->open(d_endpoint.protocol());

        boost::asio::socket_base::reuse_address roption(true);
        d_socket->set_option(roption);

        d_connected = true;
      }
    }

    void
    bbheader_source_impl::disconnect()
    {
      if (!d_connected)
        return;

      gr::thread::scoped_lock guard(d_mutex);

      d_socket->close();
      delete d_socket;

      d_connected = false;
    }

#define CRC_POLY 0xAB
// Reversed
#define CRC_POLYR 0xD5

    void
    bbheader_source_impl::build_crc8_table(void)
    {
      int r, crc;

      for (int i = 0; i < 256; i++) {
        r = i;
        crc = 0;
        for (int j = 7; j >= 0; j--) {
          if ((r & (1 << j) ? 1 : 0) ^ ((crc & 0x80) ? 1 : 0)) {
            crc = (crc << 1) ^ CRC_POLYR;
          }
          else {
            crc <<= 1;
          }
        }
        crc_tab[i] = crc;
      }
    }

    /*
     * MSB is sent first
     *
     * The polynomial has been reversed
     */
    int
    bbheader_source_impl::add_crc8_bits(unsigned char *in, int length)
    {
      int crc = 0;
      int b;
      int i = 0;

      for (int n = 0; n < length; n++) {
        b = in[i++] ^ (crc & 0x01);
        crc >>= 1;
        if (b) {
          crc ^= CRC_POLY;
        }
      }

      for (int n = 0; n < 8; n++) {
        in[i++] = (crc & (1 << n)) ? 1 : 0;
      }
      return 8;// Length of CRC
    }

    void
    bbheader_source_impl::add_bbheader(unsigned char *out, int count, int padding, bool nibble, int isi)
    {
      int temp, m_frame_offset_bits;
      unsigned char *m_frame = out;
      BBHeader *h = &m_format[0][isi].bb_header;

      m_frame[0] = h->ts_gs >> 1;
      m_frame[1] = h->ts_gs & 1;
      m_frame[2] = h->sis_mis;
      m_frame[3] = h->ccm_acm;
      m_frame[4] = h->issyi & 1;
      m_frame[5] = h->npd & 1;
      if (dvbs2x[isi] == TRUE) {
        if (alternate[isi] == TRUE) {
          alternate[isi] = FALSE;
          m_frame[6] = 1;
          m_frame[7] = 1;
        }
        else {
          alternate[isi] = TRUE;
          m_frame[6] = h->ro >> 1;
          m_frame[7] = h->ro & 1;
        }
      }
      else {
        m_frame[6] = h->ro >> 1;
        m_frame[7] = h->ro & 1;
      }
      m_frame_offset_bits = 8;
      if (h->sis_mis == SIS_MIS_MULTIPLE) {
        temp = isi;
        for (int n = 7; n >= 0; n--) {
          m_frame[m_frame_offset_bits++] = temp & (1 << n) ? 1 : 0;
        }
      }
      else {
        for (int n = 7; n >= 0; n--) {
          m_frame[m_frame_offset_bits++] = 0;
        }
      }
      temp = h->upl;
      for (int n = 15; n >= 0; n--) {
        m_frame[m_frame_offset_bits++] = temp & (1 << n) ? 1 : 0;
      }
      temp = h->dfl - padding;
      for (int n = 15; n >= 0; n--) {
        m_frame[m_frame_offset_bits++] = temp & (1 << n) ? 1 : 0;
      }
      temp = h->sync;
      for (int n = 7; n >= 0; n--) {
        m_frame[m_frame_offset_bits++] = temp & (1 << n) ? 1 : 0;
      }
      // Calculate syncd, this should point to the MSB of the CRC
      temp = count;
      if (temp == 0) {
        temp = count;
      }
      else {
        temp = (188 - count) * 8;
      }
      if (nibble == FALSE) {
        temp += 4;
      }
      for (int n = 15; n >= 0; n--) {
        m_frame[m_frame_offset_bits++] = temp & (1 << n) ? 1 : 0;
      }
      // Add CRC to BB header, at end
      int len = BB_HEADER_LENGTH_BITS;
      m_frame_offset_bits += add_crc8_bits(m_frame, len);
    }

    int
    bbheader_source_impl::crc32_calc(unsigned char *buf, int size, int crc)
    {
      for (int i = 0; i < size; i++) {
        crc = (crc << 8) ^ crc32_table[((crc >> 24) ^ buf[i]) & 0xff];
      }
      return (crc);
    }

    void
    bbheader_source_impl::crc32_init(void)
    {
      unsigned int i, j, k;

      for (i = 0; i < 256; i++) {
        k = 0;
        for (j = (i << 24) | 0x800000; j != 0x80000000; j <<= 1) {
          k = (k << 1) ^ (((k ^ j) & 0x80000000) ? 0x04c11db7 : 0);
        }
        crc32_table[i] = k;
      }
    }

    int
    bbheader_source_impl::checksum(unsigned short *addr, int count, int sum)
    {
      while (count > 1) {
        sum += *addr++;
        count -= 2;
      }
      if (count > 0) {
        sum += *(unsigned char *)addr;
      }
      sum = (sum & 0xffff) + (sum >> 16);
      sum += (sum >> 16);
      return (~sum);
    }

    inline void
    bbheader_source_impl::ping_reply(int index)
    {
      unsigned short *csum_ptr;
      unsigned short header_length, total_length, type_code, fragment_offset;
      int csum;
      struct ip *ip_ptr;
      unsigned char *saddr_ptr, *daddr_ptr;
      unsigned char addr[sizeof(in_addr)];

      /* jam ping reply and calculate new checksum */
      ip_ptr = (struct ip*)(packet[index] + sizeof(struct ether_header));
      csum_ptr = (unsigned short *)ip_ptr;
      header_length = (*csum_ptr & 0xf) * 4;
      csum_ptr = &ip_ptr->ip_len;
      total_length = ((*csum_ptr & 0xff) << 8) | ((*csum_ptr & 0xff00) >> 8);
      csum_ptr = &ip_ptr->ip_off;
      fragment_offset = ((*csum_ptr & 0xff) << 8) | ((*csum_ptr & 0xff00) >> 8);

      csum_ptr = (unsigned short *)(packet[index] + sizeof(struct ether_header) + sizeof(struct ip));
      type_code = *csum_ptr;
      type_code = (type_code & 0xff00) | 0x0;
      if ((fragment_offset & 0x1fff) == 0) {
        *csum_ptr++ = type_code;
        *csum_ptr = 0x0000;
        csum_ptr = (unsigned short *)(packet[index] + sizeof(struct ether_header) + sizeof(struct ip));
        csum = checksum(csum_ptr, total_length - header_length, 0);
        csum_ptr++;
        *csum_ptr = csum;
      }

      /* swap IP adresses */
      saddr_ptr = (unsigned char *)&ip_ptr->ip_src;
      daddr_ptr = (unsigned char *)&ip_ptr->ip_dst;
      for (unsigned int i = 0; i < sizeof(in_addr); i++) {
        addr[i] = *daddr_ptr++;
      }
      daddr_ptr = (unsigned char *)&ip_ptr->ip_dst;
      for (unsigned int i = 0; i < sizeof(in_addr); i++) {
        *daddr_ptr++ = *saddr_ptr++;
      }
      saddr_ptr = (unsigned char *)&ip_ptr->ip_src;
      for (unsigned int i = 0; i < sizeof(in_addr); i++) {
        *saddr_ptr++ = addr[i];
      }
    }

    inline void
    bbheader_source_impl::ipaddr_spoof(int index)
    {
      unsigned short *csum_ptr;
      unsigned short header_length, fragment_offset;
      int csum;
      struct ip *ip_ptr;
      unsigned char *saddr_ptr, *daddr_ptr;

      ip_ptr = (struct ip*)(packet[index] + sizeof(struct ether_header));

      daddr_ptr = (unsigned char *)&ip_ptr->ip_dst;
      for (unsigned int i = 0; i < sizeof(in_addr); i++) {
        src_addr[i] = *daddr_ptr++;
      }

      saddr_ptr = (unsigned char *)&ip_ptr->ip_src;
      for (unsigned int i = 0; i < sizeof(in_addr); i++) {
        *saddr_ptr++ = src_addr[i];
      }

      daddr_ptr = (unsigned char *)&ip_ptr->ip_dst;
      for (unsigned int i = 0; i < sizeof(in_addr); i++) {
        *daddr_ptr++ = dst_addr[i];
      }

      csum_ptr = (unsigned short *)ip_ptr;
      header_length = (*csum_ptr & 0xf) * 4;
      csum_ptr = &ip_ptr->ip_off;
      fragment_offset = ((*csum_ptr & 0xff) << 8) | ((*csum_ptr & 0xff00) >> 8);

      if ((fragment_offset & 0x1fff) == 0) {
        csum_ptr = &ip_ptr->ip_sum;
        *csum_ptr = 0x0000;
        csum_ptr = (unsigned short *)ip_ptr;
        csum = checksum(csum_ptr, header_length, 0);
        csum_ptr = &ip_ptr->ip_sum;
        *csum_ptr = csum;

        csum_ptr = (unsigned short *)(packet[index] + sizeof(struct ether_header) + sizeof(struct ip) + 6);
        *csum_ptr = 0x0000;
      }
    }

    inline void
    bbheader_source_impl::dump_packet(unsigned char *packet)
    {
#ifdef DEBUG
      unsigned char pack;

      printf("\n");
      for (unsigned int i = 0; i < kbch / 8; i++) {
        if (i % 16 == 0) {
          printf("\n");
        }
        pack = 0;
        for (int n = 0; n < 8; n++) {
          pack |= *packet++ << (7 - n);
        }
        printf("0x%02x:", pack);
      }
      printf("\n");
#endif
    }

    int
    bbheader_source_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      unsigned char *out = (unsigned char *) output_items[0];
      int i, produced = 0;
      unsigned int offset = 0;
      unsigned int padding, bits, first_offset;
      struct pcap_pkthdr hdr;
      struct ether_header *eptr;
      unsigned char *ptr;
      unsigned char total_length[2];
      int length, crc32;
      unsigned int ether_addr_len, dummy;
      bool maxsize;
      bool gse = FALSE;

      gr::thread::scoped_lock guard(d_mutex);

      i = stream;
      while (kbch[i] + produced <= (unsigned int)noutput_items) {
        if (frame_size[i] != FECFRAME_MEDIUM) {
          padding = 0;
        }
        else {
          padding = 4;
        }
        ether_addr_len = ETHER_ADDR_LEN;
        add_bbheader(&out[offset], count[i], padding, TRUE, i);
        first_offset = offset;
        offset = offset + 80;
        while (1) {
          if (packet_fragmented[i] == FALSE) {
            if (last_packet_valid[i] == FALSE) {
              packet[i] = pcap_next(descr[i], &hdr);
            }
            if (packet[i] != NULL) {
              last_packet_valid[i] = FALSE;
              if (((hdr.len - sizeof(struct ether_header) + HEADER_SIZE + ETHER_TYPE_LEN + ether_addr_len) <= ((kbch[i] - (offset - first_offset) - padding) / 8)) && ((hdr.len - sizeof(struct ether_header) + ETHER_TYPE_LEN + ether_addr_len) < 4096)) {
                /* PDU start, no fragmentation */
                gse = TRUE;
                out[offset++] = 1;    /* Start_Indicator = 1 */
                out[offset++] = 1;    /* End_Indicator = 1 */
                if (ether_addr_len) {
                  bits = 0x0;           /* Label_Type_Indicator = 6 byte */
                }
                else {
                  bits = 0x3;           /* Label_Type_Indicator = re-use */
                }
                for (int n = 1; n >= 0; n--) {
                  out[offset++] = bits & (1 << n) ? 1 : 0;
                }
                bits = hdr.len - sizeof(struct ether_header) + ETHER_TYPE_LEN + ether_addr_len;    /* GSE_Length */
                for (int n = 11; n >= 0; n--) {
                  out[offset++] = bits & (1 << n) ? 1 : 0;
                }

                if (ping_reply_mode) {
                  ping_reply(i);
                }
                if (ipaddr_spoof_mode) {
                  ipaddr_spoof(i);
                }

                eptr = (struct ether_header *)packet[i];
                /* Protocol_Type */
                ptr = (unsigned char *)&eptr->ether_type;
                for (int j = 0; j < ETHER_TYPE_LEN; j++) {
                  bits = *ptr++;
                  for (int n = 7; n >= 0; n--) {
                    out[offset++] = bits & (1 << n) ? 1 : 0;
                  }
                }
                /* 6_Byte_Label */
                ptr = eptr->ether_dhost;
                for (unsigned int j = 0; j < ether_addr_len; j++) {
                  bits = *ptr++;
                  for (int n = 7; n >= 0; n--) {
                    out[offset++] = bits & (1 << n) ? 1 : 0;
                  }
                }
                ether_addr_len = ETHER_ADDR_LEN;    /* disable label re-use for now */
                /* GSE_data_byte */
                ptr = (unsigned char *)(packet[i] + sizeof(struct ether_header));
                for (unsigned int j = 0; j < hdr.len - sizeof(struct ether_header); j++) {
                  bits = *ptr++;
                  for (int n = 7; n >= 0; n--) {
                    out[offset++] = bits & (1 << n) ? 1 : 0;
                  }
                }
                if (offset == produced + (kbch[i]) - padding) {
                  break;
                }
                continue;
              }
              else {
                /* PDU start, fragmented */
                if (((kbch[i] - (offset - first_offset) - padding) / 8) >= (HEADER_SIZE + FRAG_ID_SIZE + TOTAL_LENGTH_SIZE + ETHER_TYPE_LEN + ether_addr_len)) {
                  if (((kbch[i] - (offset - first_offset) - padding) / 8) == (HEADER_SIZE + FRAG_ID_SIZE + TOTAL_LENGTH_SIZE + ETHER_TYPE_LEN + ether_addr_len)) {
                    printf("fail mode\n");
                  }
                  gse = TRUE;
                  out[offset++] = 1;    /* Start_Indicator = 1 */
                  out[offset++] = 0;    /* End_Indicator = 0 */
                  if (ether_addr_len) {
                    bits = 0x0;           /* Label_Type_Indicator = 6 byte */
                  }
                  else {
                    bits = 0x3;           /* Label_Type_Indicator = re-use */
                  }
                  for (int n = 1; n >= 0; n--) {
                    out[offset++] = bits & (1 << n) ? 1 : 0;
                  }
                  bits = (kbch[i] - ((offset + GSE_LENGTH_SIZE) - first_offset) - padding) / 8;    /* GSE_Length */
                  if (bits >= MAX_GSE_LENGTH) {
                    bits = MAX_GSE_LENGTH - 1;
                    maxsize = TRUE;
                  }
                  else {
                    maxsize = FALSE;
                  }
                  for (int n = 11; n >= 0; n--) {
                    out[offset++] = bits & (1 << n) ? 1 : 0;
                  }
                  frag_id[i] = frag_id_pool++;
                  bits = frag_id[i];    /* Frag_ID */
                  for (int n = 7; n >= 0; n--) {
                    out[offset++] = bits & (1 << n) ? 1 : 0;
                  }
                  bits = hdr.len - sizeof(struct ether_header) + ETHER_TYPE_LEN + ether_addr_len;    /* Total_Length */
                  total_length[0] = (bits >> 8) & 0xff;
                  total_length[1] = bits & 0xff;
                  crc32_partial[i] = crc32_calc(&total_length[0], 2, 0xffffffff);
                  for (int n = 15; n >= 0; n--) {
                    out[offset++] = bits & (1 << n) ? 1 : 0;
                  }

                  if (ping_reply_mode) {
                    ping_reply(i);
                  }
                  if (ipaddr_spoof_mode) {
                    ipaddr_spoof(i);
                  }

                  eptr = (struct ether_header *)packet[i];
                  /* Protocol_Type */
                  ptr = (unsigned char *)&eptr->ether_type;
                  crc32_partial[i] = crc32_calc(ptr, ETHER_TYPE_LEN, crc32_partial[i]);
                  for (int j = 0; j < ETHER_TYPE_LEN; j++) {
                    bits = *ptr++;
                    for (int n = 7; n >= 0; n--) {
                      out[offset++] = bits & (1 << n) ? 1 : 0;
                    }
                  }
                  /* 6_Byte_Label */
                  ptr = eptr->ether_dhost;
                  crc32_partial[i] = crc32_calc(ptr, ether_addr_len, crc32_partial[i]);
                  for (unsigned int j = 0; j < ether_addr_len; j++) {
                    bits = *ptr++;
                    for (int n = 7; n >= 0; n--) {
                      out[offset++] = bits & (1 << n) ? 1 : 0;
                    }
                  }
                  ether_addr_len = ETHER_ADDR_LEN;    /* disable label re-use for now */
                  /* GSE_data_byte */
                  ptr = (unsigned char *)(packet[i] + sizeof(struct ether_header));
                  if (maxsize == TRUE) {
                    length = MAX_GSE_LENGTH - 1 - FRAG_ID_SIZE - TOTAL_LENGTH_SIZE - ETHER_TYPE_LEN - ether_addr_len;
                  }
                  else {
                    length = (kbch[i] - (offset - first_offset) - padding) / 8;
                  }
                  crc32_partial[i] = crc32_calc(ptr, length, crc32_partial[i]);
                  packet_length[i] = hdr.len - sizeof(struct ether_header) - length;
                  for (int j = 0; j < length; j++) {
                    bits = *ptr++;
                    for (int n = 7; n >= 0; n--) {
                      out[offset++] = bits & (1 << n) ? 1 : 0;
                    }
                  }
                  packet_ptr[i] = ptr;
                  packet_fragmented[i] = TRUE;
                  if (offset == produced + (kbch[i]) - padding) {
                    break;
                  }
                }
                else {
                  last_packet_valid[i] = TRUE;
                }
              }
            }
          }
          if (packet_fragmented[i] == TRUE) {
            if (((packet_length[i] + HEADER_SIZE + FRAG_ID_SIZE + sizeof(crc32)) <= ((kbch[i] - (offset - first_offset) - padding) / 8)) && ((packet_length[i] + HEADER_SIZE + FRAG_ID_SIZE + sizeof(crc32)) < 4096)) {
              /* PDU end */
              gse = TRUE;
              out[offset++] = 0;    /* Start_Indicator = 0 */
              out[offset++] = 1;    /* End_Indicator = 1 */
              bits = 0x3;           /* Label_Type_Indicator = re-use */
              for (int n = 1; n >= 0; n--) {
                out[offset++] = bits & (1 << n) ? 1 : 0;
              }
              if (packet_length[i] != 0) {
                bits = FRAG_ID_SIZE + packet_length[i] + sizeof(crc32);    /* GSE_Length */
              }
              else {
                bits = FRAG_ID_SIZE + sizeof(crc32);    /* GSE_Length */
              }
              for (int n = 11; n >= 0; n--) {
                out[offset++] = bits & (1 << n) ? 1 : 0;
              }
              bits = frag_id[i];    /* Frag_ID */
              for (int n = 7; n >= 0; n--) {
                out[offset++] = bits & (1 << n) ? 1 : 0;
              }
              /* GSE_data_byte */
              ptr = packet_ptr[i];
              length = packet_length[i];
              if (length != 0) {
                crc32 = crc32_calc(ptr, length, crc32_partial[i]);
                for (int j = 0; j < length; j++) {
                  bits = *ptr++;
                  for (int n = 7; n >= 0; n--) {
                    out[offset++] = bits & (1 << n) ? 1 : 0;
                  }
                }
                bits = crc32;
                for (int n = 31; n >= 0; n--) {
                  out[offset++] = bits & (1 << n) ? 1 : 0;
                }
              }
              else {
                printf("crc32, length = 0\n");
                bits = crc32_partial[i];
                for (int n = 31; n >= 0; n--) {
                  out[offset++] = bits & (1 << n) ? 1 : 0;
                }
              }
              packet_fragmented[i] = FALSE;
              if (offset == produced + (kbch[i]) - padding) {
                break;
              }
            }
            else {
              /* PDU continuation */
              gse = TRUE;
              length = (kbch[i] - (offset - first_offset) - padding) / 8;
              if (packet_length[i] < (length - (HEADER_SIZE + FRAG_ID_SIZE))) {
                padding = ((length - (HEADER_SIZE + FRAG_ID_SIZE)) - packet_length[i]) * 8;
                add_bbheader(&out[first_offset], count[i], padding, TRUE, i);
                printf("padding = %d\n", padding);
              }
              out[offset++] = 0;    /* Start_Indicator = 0 */
              out[offset++] = 0;    /* End_Indicator = 0 */
              bits = 0x3;           /* Label_Type_Indicator = re-use */
              for (int n = 1; n >= 0; n--) {
                out[offset++] = bits & (1 << n) ? 1 : 0;
              }
              bits = (kbch[i] - (offset + GSE_LENGTH_SIZE - first_offset) - padding) / 8;    /* GSE_Length */
              if (bits >= MAX_GSE_LENGTH) {
                bits = MAX_GSE_LENGTH - 1;
                maxsize = TRUE;
              }
              else {
                maxsize = FALSE;
              }
              for (int n = 11; n >= 0; n--) {
                out[offset++] = bits & (1 << n) ? 1 : 0;
              }
              bits = frag_id[i];    /* Frag_ID */
              for (int n = 7; n >= 0; n--) {
                out[offset++] = bits & (1 << n) ? 1 : 0;
              }
              /* GSE_data_byte */
              ptr = packet_ptr[i];
              if (maxsize == TRUE) {
                length = MAX_GSE_LENGTH - 1 - FRAG_ID_SIZE;
              }
              else {
                length = (kbch[i] - (offset - first_offset) - padding) / 8;
              }
              packet_ptr[i] += length;
              packet_length[i] -= length;
              crc32_partial[i] = crc32_calc(ptr, length, crc32_partial[i]);
              for (int j = 0; j < length; j++) {
                bits = *ptr++;
                for (int n = 7; n >= 0; n--) {
                  out[offset++] = bits & (1 << n) ? 1 : 0;
                }
              }
              if (offset == produced + (kbch[i]) - padding) {
                break;
              }
            }
          }
          else {
            padding = kbch[i] - (offset - first_offset);
            add_bbheader(&out[first_offset], count[i], padding, TRUE, i);
            if (offset == produced + (kbch[i]) - padding) {
              break;
            }
          }
        }
        if (padding != 0) {
          memset(&out[offset], 0, padding);
          offset += padding;
        }
        if (gse == TRUE) {
          gse = FALSE;
          dummy = 0;
          dump_packet(&out[first_offset]);
          if (0) {
            unsigned char pack;
            unsigned char *packet_ptr = &out[first_offset];
            unsigned char *udp_packet_ptr = udp_packet;
            *udp_packet_ptr++ = 0xb8;
            *udp_packet_ptr++ = 0x21;
            *udp_packet_ptr++ = 248;
            *udp_packet_ptr++ = frame_number++;
            for (unsigned int n = 0; n < kbch[i] / 8; n++) {
              pack = 0;
              for (int n = 0; n < 8; n++) {
                pack |= *packet_ptr++ << (7 - n);
              }
              *udp_packet_ptr++ = pack;
            }
            try {
              d_socket->send_to(boost::asio::buffer((void*)(udp_packet), (kbch[i] / 8) + 4), d_endpoint);
            }
            catch(std::exception& e) {
              GR_LOG_ERROR(d_logger, boost::format("send error: %s") % e.what());
            }
          }
        }
        else {
          dummy = 1;
        }
        const uint64_t tagoffset = this->nitems_written(0);
        const uint64_t tagmodcod = (uint64_t(gold_code[i]) << 32) | (uint64_t(pilot_mode[i]) << 24) | (uint64_t(signal_constellation[i]) << 16) | (uint64_t(code_rate[i]) << 8) | (uint64_t(frame_size[i]) << 1) | uint64_t(dummy);
        pmt::pmt_t key = pmt::string_to_symbol("modcod");
        pmt::pmt_t value = pmt::from_uint64(tagmodcod);
        this->add_item_tag(0, tagoffset, key, value);
        produced += kbch[i];
        produce(0, kbch[i]);
      }
      stream++;
      if (stream == num_streams) {
        stream = 0;
      }

      // Tell runtime system how many output items we produced.
      return WORK_CALLED_PRODUCE;
    }

  } /* namespace dvbs2 */
} /* namespace gr */

