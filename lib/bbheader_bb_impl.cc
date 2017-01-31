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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "bbheader_bb_impl.h"

namespace gr {
  namespace dvbs2 {

    bbheader_bb::sptr
    bbheader_bb::make(int nstreams, dvbs2_framesize_t framesize1, dvbs2_code_rate_t rate1, dvbs2_constellation_t constellation1, dvbs2_pilots_t pilots1, int goldcode1, dvbs2_framesize_t framesize2, dvbs2_code_rate_t rate2, dvbs2_constellation_t constellation2, dvbs2_pilots_t pilots2, int goldcode2, dvbs2_framesize_t framesize3, dvbs2_code_rate_t rate3, dvbs2_constellation_t constellation3, dvbs2_pilots_t pilots3, int goldcode3, dvbs2_framesize_t framesize4, dvbs2_code_rate_t rate4, dvbs2_constellation_t constellation4, dvbs2_pilots_t pilots4, int goldcode4, dvbs2_rolloff_factor_t rolloff)
    {
      return gnuradio::get_initial_sptr
        (new bbheader_bb_impl(nstreams, framesize1, rate1, constellation1, pilots1, goldcode1, framesize2, rate2, constellation2, pilots2, goldcode2, framesize3, rate3, constellation3, pilots3, goldcode3, framesize4, rate4, constellation4, pilots4, goldcode4, rolloff));
    }

    /*
     * The private constructor
     */
    bbheader_bb_impl::bbheader_bb_impl(int nstreams, dvbs2_framesize_t framesize1, dvbs2_code_rate_t rate1, dvbs2_constellation_t constellation1, dvbs2_pilots_t pilots1, int goldcode1, dvbs2_framesize_t framesize2, dvbs2_code_rate_t rate2, dvbs2_constellation_t constellation2, dvbs2_pilots_t pilots2, int goldcode2, dvbs2_framesize_t framesize3, dvbs2_code_rate_t rate3, dvbs2_constellation_t constellation3, dvbs2_pilots_t pilots3, int goldcode3, dvbs2_framesize_t framesize4, dvbs2_code_rate_t rate4, dvbs2_constellation_t constellation4, dvbs2_pilots_t pilots4, int goldcode4, dvbs2_rolloff_factor_t rolloff)
      : gr::block("bbheader_bb",
              gr::io_signature::make(nstreams, nstreams, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {
      num_streams = nstreams;
      stream = 0;
      for (int i = 0; i < nstreams; i++) {
        count[i] = 0;
        crc[i] = 0x0;
        dvbs2x[i] = FALSE;
        alternate[i] = TRUE;
        nibble[i] = TRUE;
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
        f->ts_gs   = TS_GS_TRANSPORT;
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
        f->upl     = 188 * 8;
        f->dfl     = kbch[i] - 80;
        f->sync    = 0x47;
        if (rolloff & 0x4) {
          dvbs2x[i] = TRUE;
        }
        f->ro      = rolloff & 0x3;
      }

      build_crc8_table();
      set_output_multiple(FRAME_SIZE_NORMAL);
    }

    /*
     * Our virtual destructor.
     */
    bbheader_bb_impl::~bbheader_bb_impl()
    {
    }

    void
    bbheader_bb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = ((noutput_items - 80) / 8);
    }

#define CRC_POLY 0xAB
// Reversed
#define CRC_POLYR 0xD5

    void
    bbheader_bb_impl::build_crc8_table(void)
    {
      int r,crc;

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
    bbheader_bb_impl::add_crc8_bits(unsigned char *in, int length)
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
    bbheader_bb_impl::add_bbheader(unsigned char *out, int count, bool nibble, int isi)
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
      temp = h->dfl;
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
    bbheader_bb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const unsigned char *in;
      unsigned char *out = (unsigned char *) output_items[0];
      int consumed[NUM_STREAMS] = {0, 0, 0, 0};
      int produced = 0;
      int i, offset = 0;
      unsigned char b;

      i = stream;
      in = (const unsigned char *) input_items[i];
      while (kbch[i] + produced <= (unsigned int)noutput_items) {
        const uint64_t tagoffset = this->nitems_written(0);
        const uint64_t tagmodcod = (uint64_t(gold_code[0]) << 32) | (uint64_t(pilot_mode[i]) << 24) | (uint64_t(signal_constellation[i]) << 16) | (uint64_t(code_rate[i]) << 8) | (uint64_t(frame_size[i]) << 1) | uint64_t(0);
        pmt::pmt_t key = pmt::string_to_symbol("modcod");
        pmt::pmt_t value = pmt::from_long(tagmodcod);
        this->add_item_tag(0, tagoffset, key, value);
        gold_code[0]++;    /* VCM gold code not supported for now, use it for stream tag debugging instead. */
        if (frame_size[i] != FECFRAME_MEDIUM) {
          add_bbheader(&out[offset], count[i], nibble[i], i);
          offset = offset + 80;
          for (int j = 0; j < (int)((kbch[i] - 80) / 8); j++) {
            if (count[i] == 0) {
              if (*in != 0x47) {
                GR_LOG_WARN(d_logger, boost::format("Transport Stream %1% sync error!") \
                            % i);
              }
              in++;
              b = crc[i];
              crc[i] = 0;
            }
            else {
              b = *in++;
              crc[i] = crc_tab[b ^ crc[i]];
            }
            count[i] = (count[i] + 1) % 188;
            consumed[i]++;
            for (int n = 7; n >= 0; n--) {
              out[offset++] = b & (1 << n) ? 1 : 0;
            }
          }
        }
        else {
          add_bbheader(&out[offset], count[i], nibble[i], i);
          offset = offset + 80;
          for (int j = 0; j < (int)((kbch[i] - 80) / 4); j++) {
            if (nibble[i] == TRUE) {
              if (count[i] == 0) {
                if (*in != 0x47) {
                  GR_LOG_WARN(d_logger, boost::format("Transport Stream %1% sync error!") \
                              % i);
                }
                in++;
                b = crc[i];
                crc[i] = 0;
              }
              else {
                b = *in++;
                crc[i] = crc_tab[b ^ crc[i]];
              }
              bsave[i] = b;
              count[i] = (count[i] + 1) % 188;
              consumed[i]++;
              for (int n = 7; n >= 4; n--) {
                out[offset++] = b & (1 << n) ? 1 : 0;
              }
              nibble[i] = FALSE;
            }
            else {
              for (int n = 3; n >= 0; n--) {
                out[offset++] = bsave[i] & (1 << n) ? 1 : 0;
              }
              nibble[i] = TRUE;
            }
          }
        }
        produced += kbch[i];
        produce(0, kbch[i]);
      }
      stream++;
      if (stream == num_streams) {
        stream = 0;
      }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume (i, consumed[i]);

      // Tell runtime system how many output items we produced.
      return WORK_CALLED_PRODUCE;
    }

  } /* namespace dvbs2 */
} /* namespace gr */

