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
#include "interleaver_bb_impl.h"

namespace gr {
  namespace dvbs2 {

    interleaver_bb::sptr
    interleaver_bb::make()
    {
      return gnuradio::get_initial_sptr
        (new interleaver_bb_impl());
    }

    /*
     * The private constructor
     */
    interleaver_bb_impl::interleaver_bb_impl()
      : gr::block("interleaver_bb",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {
      set_tag_propagation_policy(TPP_DONT);
      set_output_multiple(FRAME_SIZE_NORMAL);
    }

    /*
     * Our virtual destructor.
     */
    interleaver_bb_impl::~interleaver_bb_impl()
    {
    }

    void
    interleaver_bb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items * 5;
    }

    void
    interleaver_bb_impl::get_rows(dvbs2_framesize_t framesize, dvbs2_code_rate_t rate, dvbs2_constellation_t constellation, int *frame_size, int *mod_order)
    {
      int mod, rows;

      if (framesize == FECFRAME_NORMAL) {
        *frame_size = FRAME_SIZE_NORMAL;
        if (rate == C2_9_VLSNR) {
          *frame_size = FRAME_SIZE_NORMAL - NORMAL_PUNCTURING;
        }
      }
      else if (framesize == FECFRAME_SHORT) {
        *frame_size = FRAME_SIZE_SHORT;
        if (rate == C1_5_VLSNR_SF2 || rate == C11_45_VLSNR_SF2) {
          *frame_size = FRAME_SIZE_SHORT - SHORT_PUNCTURING_SET1;
        }
        if (rate == C1_5_VLSNR || rate == C4_15_VLSNR || rate == C1_3_VLSNR) {
          *frame_size = FRAME_SIZE_SHORT - SHORT_PUNCTURING_SET2;
        }
      }
      else {
        *frame_size = FRAME_SIZE_MEDIUM - MEDIUM_PUNCTURING;
      }

      switch (constellation) {
        case MOD_BPSK:
          mod = 1;
          rows = *frame_size / mod;
          break;
        case MOD_BPSK_SF2:
          mod = 1;
          rows = *frame_size / mod;
          break;
        case MOD_QPSK:
          mod = 2;
          rows = *frame_size / mod;
          break;
        case MOD_8PSK:
          mod = 3;
          rows = *frame_size / mod;
          /* 210 */
          if (rate == C3_5) {
            rowaddr0 = rows * 2;
            rowaddr1 = rows;
            rowaddr2 = 0;
          }
          /* 102 */
          else if (rate == C25_36 || rate == C13_18 || rate == C7_15 || rate == C8_15 || rate == C26_45) {
            rowaddr0 = rows;
            rowaddr1 = 0;
            rowaddr2 = rows * 2;
          }
          /* 012 */
          else {
            rowaddr0 = 0;
            rowaddr1 = rows;
            rowaddr2 = rows * 2;
          }
          break;
        case MOD_8APSK:
          mod = 3;
          rows = *frame_size / mod;
          /* 012 */
          rowaddr0 = 0;
          rowaddr1 = rows;
          rowaddr2 = rows * 2;
          break;
        case MOD_16APSK:
          mod = 4;
          rows = *frame_size / mod;
          if (rate == C26_45) {
            /* 3201 */
            if (*frame_size == FRAME_SIZE_NORMAL) {
              rowaddr0 = rows * 3;
              rowaddr1 = rows * 2;
              rowaddr2 = 0;
              rowaddr3 = rows;
            }
            /* 2130 */
            else {
              rowaddr0 = rows * 2;
              rowaddr1 = rows;
              rowaddr2 = rows * 3;
              rowaddr3 = 0;
            }
          }
          else if (rate == C3_5) {
            /* 3210 */
            if (*frame_size == FRAME_SIZE_NORMAL) {
              rowaddr0 = rows * 3;
              rowaddr1 = rows * 2;
              rowaddr2 = rows;
              rowaddr3 = 0;
            }
            /* 3201 */
            else {
              rowaddr0 = rows * 3;
              rowaddr1 = rows * 2;
              rowaddr2 = 0;
              rowaddr3 = rows;
            }
          }
          /* 3012 */
          else if (rate == C28_45) {
            rowaddr0 = rows * 3;
            rowaddr1 = 0;
            rowaddr2 = rows;
            rowaddr3 = rows * 2;
          }
          /* 3021 */
          else if (rate == C23_36 || rate == C13_18) {
            rowaddr0 = rows * 3;
            rowaddr1 = 0;
            rowaddr2 = rows * 2;
            rowaddr3 = rows;
          }
          /* 2310 */
          else if (rate == C25_36) {
            rowaddr0 = rows * 2;
            rowaddr1 = rows * 3;
            rowaddr2 = rows;
            rowaddr3 = 0;
          }
          /* 2103 */
          else if (rate == C7_15 || rate == C8_15) {
            rowaddr0 = rows * 2;
            rowaddr1 = rows;
            rowaddr2 = 0;
            rowaddr3 = rows * 3;
          }
          /* 3210 */
          else if (rate == C140_180) {
            rowaddr0 = rows * 3;
            rowaddr1 = rows * 2;
            rowaddr2 = rows;
            rowaddr3 = 0;
          }
          /* 0321 */
          else if (rate == C154_180) {
            rowaddr0 = 0;
            rowaddr1 = rows * 3;
            rowaddr2 = rows * 2;
            rowaddr3 = rows;
          }
          /* 0123 */
          else {
            rowaddr0 = 0;
            rowaddr1 = rows;
            rowaddr2 = rows * 2;
            rowaddr3 = rows * 3;
          }
          break;
        case MOD_8_8APSK:
          mod = 4;
          rows = *frame_size / mod;
          /* 3210 */
          if (rate == C90_180) {
            rowaddr0 = rows * 3;
            rowaddr1 = rows * 2;
            rowaddr2 = rows;
            rowaddr3 = 0;
          }
          /* 2310 */
          else if (rate == C96_180) {
            rowaddr0 = rows * 2;
            rowaddr1 = rows * 3;
            rowaddr2 = rows;
            rowaddr3 = 0;
          }
          /* 2301 */
          else if (rate == C100_180) {
            rowaddr0 = rows * 2;
            rowaddr1 = rows * 3;
            rowaddr2 = 0;
            rowaddr3 = rows;
          }
          /* 0123 */
          else {
            rowaddr0 = 0;
            rowaddr1 = rows;
            rowaddr2 = rows * 2;
            rowaddr3 = rows * 3;
          }
          break;
        case MOD_32APSK:
          mod = 5;
          rows = *frame_size / mod;
          /* 01234 */
          rowaddr0 = 0;
          rowaddr1 = rows;
          rowaddr2 = rows * 2;
          rowaddr3 = rows * 3;
          rowaddr4 = rows * 4;
          break;
        case MOD_4_12_16APSK:
          mod = 5;
          rows = *frame_size / mod;
          /* 21430 */
          if (*frame_size == FRAME_SIZE_NORMAL) {
            rowaddr0 = rows * 2;
            rowaddr1 = rows;
            rowaddr2 = rows * 4;
            rowaddr3 = rows * 3;
            rowaddr4 = 0;
          }
          else {
            /* 41230 */
            if (rate == C2_3) {
              rowaddr0 = rows * 4;
              rowaddr1 = rows;
              rowaddr2 = rows * 2;
              rowaddr3 = rows * 3;
              rowaddr4 = 0;
            }
            /* 10423 */
            else if (rate == C32_45) {
              rowaddr0 = rows;
              rowaddr1 = 0;
              rowaddr2 = rows * 4;
              rowaddr3 = rows * 2;
              rowaddr4 = rows * 3;
            }
          }
          break;
        case MOD_4_8_4_16APSK:
          mod = 5;
          rows = *frame_size / mod;
          /* 40213 */
          if (rate == C140_180) {
            rowaddr0 = rows * 4;
            rowaddr1 = 0;
            rowaddr2 = rows * 2;
            rowaddr3 = rows;
            rowaddr4 = rows * 3;
          }
          /* 40312 */
          else {
            rowaddr0 = rows * 4;
            rowaddr1 = 0;
            rowaddr2 = rows * 3;
            rowaddr3 = rows;
            rowaddr4 = rows * 2;
          }
          break;
        default:
          mod = 2;
          rows = *frame_size / mod;
          break;
      }
      *mod_order = mod;
    }

    int
    interleaver_bb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];
      int consumed = 0;
      int produced = 0;
      int produced_per_iteration;
      dvbs2_framesize_t framesize;
      dvbs2_code_rate_t rate;
      dvbs2_constellation_t constellation;
      dvbs2_pilots_t pilots;
      unsigned int goldcode, dummy;
      int frame_size, mod_order, rows;
      const unsigned char *c1, *c2, *c3, *c4, *c5;

      std::vector<tag_t> tags;
      const uint64_t nread = this->nitems_read(0); //number of items read on port 0

      // Read all tags on the input buffer
      this->get_tags_in_range(tags, 0, nread, nread + noutput_items, pmt::string_to_symbol("modcod"));

      for (int i = 0; i < (int)tags.size(); i++) {
        dummy = (unsigned int)((pmt::to_long(tags[i].value)) & 0x1);
        framesize = (dvbs2_framesize_t)(((pmt::to_long(tags[i].value)) >> 1) & 0x7f);
        rate = (dvbs2_code_rate_t)(((pmt::to_long(tags[i].value)) >> 8) & 0xff);
        constellation = (dvbs2_constellation_t)(((pmt::to_long(tags[i].value)) >> 16) & 0xff);
        pilots = (dvbs2_pilots_t)(((pmt::to_long(tags[i].value)) >> 24) & 0xff);
        goldcode = (unsigned int)(((pmt::to_long(tags[i].value)) >> 32) & 0x3ffff);
        get_rows(framesize, rate, constellation, &frame_size, &mod_order);
        if ((produced + (frame_size / mod_order)) <= noutput_items) {
          produced_per_iteration = 0;
          const uint64_t tagoffset = this->nitems_written(0);
          const uint64_t tagmodcod = (uint64_t(goldcode) << 32) | (uint64_t(pilots) << 24) | (uint64_t(constellation) << 16) | (uint64_t(rate) << 8) | (uint64_t(framesize) << 1) | uint64_t(dummy);
          pmt::pmt_t key = pmt::string_to_symbol("modcod");
          pmt::pmt_t value = pmt::from_long(tagmodcod);
          this->add_item_tag(0, tagoffset, key, value);
          switch (constellation) {
            case MOD_BPSK:
              rows = frame_size;
              for (int j = 0; j < rows; j++) {
                out[produced++] = in[consumed++];
                produced_per_iteration++;
              }
              break;
            case MOD_BPSK_SF2:
              rows = frame_size;
              for (int j = 0; j < rows; j++) {
                out[produced++] = in[consumed];
                out[produced++] = in[consumed++];
                produced_per_iteration += 2;
              }
              break;
            case MOD_QPSK:
              rows = frame_size / 2;
              for (int j = 0; j < rows; j++) {
                out[produced] = in[consumed++] << 1;
                out[produced++] |= in[consumed++];
                produced_per_iteration++;
              }
              break;
            case MOD_8PSK:
            case MOD_8APSK:
              rows = frame_size / 3;
              c1 = &in[consumed + rowaddr0];
              c2 = &in[consumed + rowaddr1];
              c3 = &in[consumed + rowaddr2];
              for (int j = 0; j < rows; j++) {
                out[produced++] = (c1[j]<<2) | (c2[j]<<1) | (c3[j]);
                produced_per_iteration++;
                consumed += 3;
              }
              break;
            case MOD_16APSK:
            case MOD_8_8APSK:
              rows = frame_size / 4;
              c1 = &in[consumed + rowaddr0];
              c2 = &in[consumed + rowaddr1];
              c3 = &in[consumed + rowaddr2];
              c4 = &in[consumed + rowaddr3];
              for (int j = 0; j < rows; j++) {
                out[produced++] = (c1[j]<<3) | (c2[j]<<2) | (c3[j]<<1) | (c4[j]);
                produced_per_iteration++;
                consumed += 4;
              }
              break;
            case MOD_32APSK:
            case MOD_4_12_16APSK:
            case MOD_4_8_4_16APSK:
              rows = frame_size / 5;
              c1 = &in[consumed + rowaddr0];
              c2 = &in[consumed + rowaddr1];
              c3 = &in[consumed + rowaddr2];
              c4 = &in[consumed + rowaddr3];
              c5 = &in[consumed + rowaddr4];
              for (int j = 0; j < rows; j++) {
                out[produced++] = (c1[j]<<4) | (c2[j]<<3) | (c3[j]<<2) | (c4[j]<<1) | c5[j];
                produced_per_iteration++;
                consumed += 5;
              }
              break;
            default:
              rows = frame_size / 2;
              for (int j = 0; j < rows; j++) {
                out[produced] = in[consumed++] << 1;
                out[produced++] |= in[consumed++];
                produced_per_iteration++;
              }
              break;
          }
          produce(0, produced_per_iteration);
        }
        else {
          break;
        }
      }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (consumed);

      // Tell runtime system how many output items we produced.
      return WORK_CALLED_PRODUCE;
    }

  } /* namespace dvbs2 */
} /* namespace gr */

