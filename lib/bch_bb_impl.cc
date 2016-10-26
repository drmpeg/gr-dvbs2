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
#include "bch_bb_impl.h"

namespace gr {
  namespace dvbs2 {

    bch_bb::sptr
    bch_bb::make()
    {
      return gnuradio::get_initial_sptr
        (new bch_bb_impl());
    }

    /*
     * The private constructor
     */
    bch_bb_impl::bch_bb_impl()
      : gr::block("bch_bb",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {
      bch_poly_build_tables();
      set_tag_propagation_policy(TPP_DONT);
      set_output_multiple(FRAME_SIZE_NORMAL);
    }

    /*
     * Our virtual destructor.
     */
    bch_bb_impl::~bch_bb_impl()
    {
    }

    void
    bch_bb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items;
    }

    void
    bch_bb_impl::get_kbch_nbch(dvbs2_framesize_t framesize, dvbs2_code_rate_t rate, unsigned int *kbch, unsigned int *nbch, unsigned int *bch_code)
    {
      if (framesize == FECFRAME_NORMAL) {
        switch (rate) {
          case C1_4:
            *kbch = 16008;
            *nbch = 16200;
            *bch_code = BCH_CODE_N12;
            break;
          case C1_3:
            *kbch = 21408;
            *nbch = 21600;
            *bch_code = BCH_CODE_N12;
            break;
          case C2_5:
            *kbch = 25728;
            *nbch = 25920;
            *bch_code = BCH_CODE_N12;
            break;
          case C1_2:
            *kbch = 32208;
            *nbch = 32400;
            *bch_code = BCH_CODE_N12;
            break;
          case C3_5:
            *kbch = 38688;
            *nbch = 38880;
            *bch_code = BCH_CODE_N12;
            break;
          case C2_3:
            *kbch = 43040;
            *nbch = 43200;
            *bch_code = BCH_CODE_N10;
            break;
          case C3_4:
            *kbch = 48408;
            *nbch = 48600;
            *bch_code = BCH_CODE_N12;
            break;
          case C4_5:
            *kbch = 51648;
            *nbch = 51840;
            *bch_code = BCH_CODE_N12;
            break;
          case C5_6:
            *kbch = 53840;
            *nbch = 54000;
            *bch_code = BCH_CODE_N10;
            break;
          case C8_9:
            *kbch = 57472;
            *nbch = 57600;
            *bch_code = BCH_CODE_N8;
            break;
          case C9_10:
            *kbch = 58192;
            *nbch = 58320;
            *bch_code = BCH_CODE_N8;
            break;
          case C2_9_VLSNR:
            *kbch = 14208;
            *nbch = 14400;
            *bch_code = BCH_CODE_N12;
            break;
          case C13_45:
            *kbch = 18528;
            *nbch = 18720;
            *bch_code = BCH_CODE_N12;
            break;
          case C9_20:
            *kbch = 28968;
            *nbch = 29160;
            *bch_code = BCH_CODE_N12;
            break;
          case C90_180:
            *kbch = 32208;
            *nbch = 32400;
            *bch_code = BCH_CODE_N12;
            break;
          case C96_180:
            *kbch = 34368;
            *nbch = 34560;
            *bch_code = BCH_CODE_N12;
            break;
          case C11_20:
            *kbch = 35448;
            *nbch = 35640;
            *bch_code = BCH_CODE_N12;
            break;
          case C100_180:
            *kbch = 35808;
            *nbch = 36000;
            *bch_code = BCH_CODE_N12;
            break;
          case C104_180:
            *kbch = 37248;
            *nbch = 37440;
            *bch_code = BCH_CODE_N12;
            break;
          case C26_45:
            *kbch = 37248;
            *nbch = 37440;
            *bch_code = BCH_CODE_N12;
            break;
          case C18_30:
            *kbch = 38688;
            *nbch = 38880;
            *bch_code = BCH_CODE_N12;
            break;
          case C28_45:
            *kbch = 40128;
            *nbch = 40320;
            *bch_code = BCH_CODE_N12;
            break;
          case C23_36:
            *kbch = 41208;
            *nbch = 41400;
            *bch_code = BCH_CODE_N12;
            break;
          case C116_180:
            *kbch = 41568;
            *nbch = 41760;
            *bch_code = BCH_CODE_N12;
            break;
          case C20_30:
            *kbch = 43008;
            *nbch = 43200;
            *bch_code = BCH_CODE_N12;
            break;
          case C124_180:
            *kbch = 44448;
            *nbch = 44640;
            *bch_code = BCH_CODE_N12;
            break;
          case C25_36:
            *kbch = 44808;
            *nbch = 45000;
            *bch_code = BCH_CODE_N12;
            break;
          case C128_180:
            *kbch = 45888;
            *nbch = 46080;
            *bch_code = BCH_CODE_N12;
            break;
          case C13_18:
            *kbch = 46608;
            *nbch = 46800;
            *bch_code = BCH_CODE_N12;
            break;
          case C132_180:
            *kbch = 47328;
            *nbch = 47520;
            *bch_code = BCH_CODE_N12;
            break;
          case C22_30:
            *kbch = 47328;
            *nbch = 47520;
            *bch_code = BCH_CODE_N12;
            break;
          case C135_180:
            *kbch = 48408;
            *nbch = 48600;
            *bch_code = BCH_CODE_N12;
            break;
          case C140_180:
            *kbch = 50208;
            *nbch = 50400;
            *bch_code = BCH_CODE_N12;
            break;
          case C7_9:
            *kbch = 50208;
            *nbch = 50400;
            *bch_code = BCH_CODE_N12;
            break;
          case C154_180:
            *kbch = 55248;
            *nbch = 55440;
            *bch_code = BCH_CODE_N12;
            break;
          default:
            *kbch = 0;
            *nbch = 0;
            *bch_code = 0;
            break;
        }
      }
      else if (framesize == FECFRAME_SHORT) {
        switch (rate) {
          case C1_4:
            *kbch = 3072;
            *nbch = 3240;
            *bch_code = BCH_CODE_S12;
            break;
          case C1_3:
            *kbch = 5232;
            *nbch = 5400;
            *bch_code = BCH_CODE_S12;
            break;
          case C2_5:
            *kbch = 6312;
            *nbch = 6480;
            *bch_code = BCH_CODE_S12;
            break;
          case C1_2:
            *kbch = 7032;
            *nbch = 7200;
            *bch_code = BCH_CODE_S12;
            break;
          case C3_5:
            *kbch = 9552;
            *nbch = 9720;
            *bch_code = BCH_CODE_S12;
            break;
          case C2_3:
            *kbch = 10632;
            *nbch = 10800;
            *bch_code = BCH_CODE_S12;
            break;
          case C3_4:
            *kbch = 11712;
            *nbch = 11880;
            *bch_code = BCH_CODE_S12;
            break;
          case C4_5:
            *kbch = 12432;
            *nbch = 12600;
            *bch_code = BCH_CODE_S12;
            break;
          case C5_6:
            *kbch = 13152;
            *nbch = 13320;
            *bch_code = BCH_CODE_S12;
            break;
          case C8_9:
            *kbch = 14232;
            *nbch = 14400;
            *bch_code = BCH_CODE_S12;
            break;
          case C11_45:
            *kbch = 3792;
            *nbch = 3960;
            *bch_code = BCH_CODE_S12;
            break;
          case C4_15:
            *kbch = 4152;
            *nbch = 4320;
            *bch_code = BCH_CODE_S12;
            break;
          case C14_45:
            *kbch = 4872;
            *nbch = 5040;
            *bch_code = BCH_CODE_S12;
            break;
          case C7_15:
            *kbch = 7392;
            *nbch = 7560;
            *bch_code = BCH_CODE_S12;
            break;
          case C8_15:
            *kbch = 8472;
            *nbch = 8640;
            *bch_code = BCH_CODE_S12;
            break;
          case C26_45:
            *kbch = 9192;
            *nbch = 9360;
            *bch_code = BCH_CODE_S12;
            break;
          case C32_45:
            *kbch = 11352;
            *nbch = 11520;
            *bch_code = BCH_CODE_S12;
            break;
          case C1_5_VLSNR_SF2:
            *kbch = 2512;
            *nbch = 2680;
            *bch_code = BCH_CODE_S12;
            break;
          case C11_45_VLSNR_SF2:
            *kbch = 3792;
            *nbch = 3960;
            *bch_code = BCH_CODE_S12;
            break;
          case C1_5_VLSNR:
            *kbch = 3072;
            *nbch = 3240;
            *bch_code = BCH_CODE_S12;
            break;
          case C4_15_VLSNR:
            *kbch = 4152;
            *nbch = 4320;
            *bch_code = BCH_CODE_S12;
            break;
          case C1_3_VLSNR:
            *kbch = 5232;
            *nbch = 5400;
            *bch_code = BCH_CODE_S12;
            break;
          default:
            *kbch = 0;
            *nbch = 0;
            *bch_code = 0;
            break;
        }
      }
      else {
        switch (rate) {
          case C1_5_MEDIUM:
            *kbch = 5660;
            *nbch = 5840;
            *bch_code = BCH_CODE_M12;
            break;
          case C11_45_MEDIUM:
            *kbch = 7740;
            *nbch = 7920;
            *bch_code = BCH_CODE_M12;
            break;
          case C1_3_MEDIUM:
            *kbch = 10620;
            *nbch = 10800;
            *bch_code = BCH_CODE_M12;
            break;
          default:
            *kbch = 0;
            *nbch = 0;
            *bch_code = 0;
            break;
        }
      }
    }

    /*
     * Polynomial calculation routines
     * multiply polynomials
     */
    int
    bch_bb_impl::poly_mult(const int *ina, int lena, const int *inb, int lenb, int *out)
    {
      memset(out, 0, sizeof(int) * (lena + lenb));

      for (int i = 0; i < lena; i++) {
        for (int j = 0; j < lenb; j++) {
          if (ina[i] * inb[j] > 0 ) {
            out[i + j]++;    // count number of terms for this pwr of x
          }
        }
      }
      int max = 0;
      for (int i = 0; i < lena + lenb; i++) {
        out[i] = out[i] & 1;    // If even ignore the term
        if(out[i]) {
          max = i;
        }
      }
      // return the size of array to house the result.
      return max + 1;
    }

    /*
     * Pack the polynomial into a 32 bit array
     */
    void
    bch_bb_impl::poly_pack(const int *pin, unsigned int* pout, int len)
    {
      int lw = len / 32;
      int ptr = 0;
      unsigned int temp;
      if (len % 32) {
        lw++;
      }

      for (int i = 0; i < lw; i++) {
        temp = 0x80000000;
        pout[i] = 0;
        for (int j = 0; j < 32; j++) {
          if (pin[ptr++]) {
            pout[i] |= temp;
          }
          temp >>= 1;
        }
      }
    }

    void
    bch_bb_impl::poly_reverse(int *pin, int *pout, int len)
    {
      int c;
      c = len - 1;

      for (int i = 0; i < len; i++) {
        pout[c--] = pin[i];
      }
    }

    /*
     *Shift a 128 bit register
     */
    inline void
    bch_bb_impl::reg_4_shift(unsigned int *sr)
    {
      sr[3] = (sr[3] >> 1) | (sr[2] << 31);
      sr[2] = (sr[2] >> 1) | (sr[1] << 31);
      sr[1] = (sr[1] >> 1) | (sr[0] << 31);
      sr[0] = (sr[0] >> 1);
    }

    /*
     * Shift 160 bits
     */
    inline void
    bch_bb_impl::reg_5_shift(unsigned int *sr)
    {
      sr[4] = (sr[4] >> 1) | (sr[3] << 31);
      sr[3] = (sr[3] >> 1) | (sr[2] << 31);
      sr[2] = (sr[2] >> 1) | (sr[1] << 31);
      sr[1] = (sr[1] >> 1) | (sr[0] << 31);
      sr[0] = (sr[0] >> 1);
    }

    /*
     * Shift 192 bits
     */
    inline void
    bch_bb_impl::reg_6_shift(unsigned int *sr)
    {
      sr[5] = (sr[5] >> 1) | (sr[4] << 31);
      sr[4] = (sr[4] >> 1) | (sr[3] << 31);
      sr[3] = (sr[3] >> 1) | (sr[2] << 31);
      sr[2] = (sr[2] >> 1) | (sr[1] << 31);
      sr[1] = (sr[1] >> 1) | (sr[0] << 31);
      sr[0] = (sr[0] >> 1);
    }

    void
    bch_bb_impl::bch_poly_build_tables(void)
    {
      // Normal polynomials
      const int polyn01[]={1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,1};
      const int polyn02[]={1,1,0,0,1,1,1,0,1,0,0,0,0,0,0,0,1};
      const int polyn03[]={1,0,1,1,1,1,0,1,1,1,1,1,0,0,0,0,1};
      const int polyn04[]={1,0,1,0,1,0,1,0,0,1,0,1,1,0,1,0,1};
      const int polyn05[]={1,1,1,1,0,1,0,0,1,1,1,1,1,0,0,0,1};
      const int polyn06[]={1,0,1,0,1,1,0,1,1,1,1,0,1,1,1,1,1};
      const int polyn07[]={1,0,1,0,0,1,1,0,1,1,1,1,0,1,0,1,1};
      const int polyn08[]={1,1,1,0,0,1,1,0,1,1,0,0,1,1,1,0,1};
      const int polyn09[]={1,0,0,0,0,1,0,1,0,1,1,1,0,0,0,0,1};
      const int polyn10[]={1,1,1,0,0,1,0,1,1,0,1,0,1,1,1,0,1};
      const int polyn11[]={1,0,1,1,0,1,0,0,0,1,0,1,1,1,0,0,1};
      const int polyn12[]={1,1,0,0,0,1,1,1,0,1,0,1,1,0,0,0,1};

      // Medium polynomials
      const int polym01[]={1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,1};
      const int polym02[]={1,1,0,0,1,0,0,1,0,0,1,1,0,0,0,1};
      const int polym03[]={1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,1};
      const int polym04[]={1,0,1,1,0,1,1,0,1,0,1,1,0,0,0,1};
      const int polym05[]={1,1,1,0,1,0,1,1,0,0,1,0,1,0,0,1};
      const int polym06[]={1,0,0,0,1,0,1,1,0,0,0,0,1,1,0,1};
      const int polym07[]={1,0,1,0,1,1,0,1,0,0,0,1,1,0,1,1};
      const int polym08[]={1,0,1,0,1,0,1,0,1,1,0,1,0,0,1,1};
      const int polym09[]={1,1,1,0,1,1,0,1,0,1,0,1,1,1,0,1};
      const int polym10[]={1,1,1,1,1,0,0,1,0,0,1,1,1,1,0,1};
      const int polym11[]={1,1,1,0,1,0,0,0,0,1,0,1,0,0,0,1};
      const int polym12[]={1,0,1,0,1,0,0,0,1,0,1,1,0,1,1,1};

      // Short polynomials
      const int polys01[]={1,1,0,1,0,1,0,0,0,0,0,0,0,0,1};
      const int polys02[]={1,0,0,0,0,0,1,0,1,0,0,1,0,0,1};
      const int polys03[]={1,1,1,0,0,0,1,0,0,1,1,0,0,0,1};
      const int polys04[]={1,0,0,0,1,0,0,1,1,0,1,0,1,0,1};
      const int polys05[]={1,0,1,0,1,0,1,0,1,1,0,1,0,1,1};
      const int polys06[]={1,0,0,1,0,0,0,1,1,1,0,0,0,1,1};
      const int polys07[]={1,0,1,0,0,1,1,1,0,0,1,1,0,1,1};
      const int polys08[]={1,0,0,0,0,1,0,0,1,1,1,1,0,0,1};
      const int polys09[]={1,1,1,1,0,0,0,0,0,1,1,0,0,0,1};
      const int polys10[]={1,0,0,1,0,0,1,0,0,1,0,1,1,0,1};
      const int polys11[]={1,0,0,0,1,0,0,0,0,0,0,1,1,0,1};
      const int polys12[]={1,1,1,1,0,1,1,1,1,0,1,0,0,1,1};

      int len;
      int polyout[2][200];

      len = poly_mult(polyn01, 17, polyn02,    17,  polyout[0]);
      len = poly_mult(polyn03, 17, polyout[0], len, polyout[1]);
      len = poly_mult(polyn04, 17, polyout[1], len, polyout[0]);
      len = poly_mult(polyn05, 17, polyout[0], len, polyout[1]);
      len = poly_mult(polyn06, 17, polyout[1], len, polyout[0]);
      len = poly_mult(polyn07, 17, polyout[0], len, polyout[1]);
      len = poly_mult(polyn08, 17, polyout[1], len, polyout[0]);
      poly_pack(polyout[0], m_poly_n_8, 128);

      len = poly_mult(polyn09, 17, polyout[0], len, polyout[1]);
      len = poly_mult(polyn10, 17, polyout[1], len, polyout[0]);
      poly_pack(polyout[0], m_poly_n_10, 160);

      len = poly_mult(polyn11, 17, polyout[0], len, polyout[1]);
      len = poly_mult(polyn12, 17, polyout[1], len, polyout[0]);
      poly_pack(polyout[0], m_poly_n_12, 192);

      len = poly_mult(polys01, 15, polys02,    15,  polyout[0]);
      len = poly_mult(polys03, 15, polyout[0], len, polyout[1]);
      len = poly_mult(polys04, 15, polyout[1], len, polyout[0]);
      len = poly_mult(polys05, 15, polyout[0], len, polyout[1]);
      len = poly_mult(polys06, 15, polyout[1], len, polyout[0]);
      len = poly_mult(polys07, 15, polyout[0], len, polyout[1]);
      len = poly_mult(polys08, 15, polyout[1], len, polyout[0]);
      len = poly_mult(polys09, 15, polyout[0], len, polyout[1]);
      len = poly_mult(polys10, 15, polyout[1], len, polyout[0]);
      len = poly_mult(polys11, 15, polyout[0], len, polyout[1]);
      len = poly_mult(polys12, 15, polyout[1], len, polyout[0]);
      poly_pack(polyout[0], m_poly_s_12, 168);

      len = poly_mult(polym01, 16, polym02,    16,  polyout[0]);
      len = poly_mult(polym03, 16, polyout[0], len, polyout[1]);
      len = poly_mult(polym04, 16, polyout[1], len, polyout[0]);
      len = poly_mult(polym05, 16, polyout[0], len, polyout[1]);
      len = poly_mult(polym06, 16, polyout[1], len, polyout[0]);
      len = poly_mult(polym07, 16, polyout[0], len, polyout[1]);
      len = poly_mult(polym08, 16, polyout[1], len, polyout[0]);
      len = poly_mult(polym09, 16, polyout[0], len, polyout[1]);
      len = poly_mult(polym10, 16, polyout[1], len, polyout[0]);
      len = poly_mult(polym11, 16, polyout[0], len, polyout[1]);
      len = poly_mult(polym12, 16, polyout[1], len, polyout[0]);
      poly_pack(polyout[0], m_poly_m_12, 180);
    }

    int
    bch_bb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];
      unsigned char b, temp;
      unsigned int shift[6];
      int consumed = 0;
      int produced = 0;
      dvbs2_framesize_t framesize;
      dvbs2_code_rate_t rate;
      dvbs2_constellation_t constellation;
      dvbs2_pilots_t pilots;
      unsigned int goldcode;
      unsigned int kbch, nbch, bch_code;
      static unsigned int check = 0;

      std::vector<tag_t> tags;
      const uint64_t nread = this->nitems_read(0); //number of items read on port 0

      // Read all tags on the input buffer
      this->get_tags_in_range(tags, 0, nread, nread + noutput_items, pmt::string_to_symbol("modcod"));

      for (int i = 0; i < (int)tags.size(); i++) {
        framesize = (dvbs2_framesize_t)((pmt::to_long(tags[i].value)) & 0xff);
        rate = (dvbs2_code_rate_t)(((pmt::to_long(tags[i].value)) >> 8) & 0xff);
        constellation = (dvbs2_constellation_t)(((pmt::to_long(tags[i].value)) >> 16) & 0xff);
        pilots = (dvbs2_pilots_t)(((pmt::to_long(tags[i].value)) >> 24) & 0xff);
        goldcode = (unsigned int)(((pmt::to_long(tags[i].value)) >> 32) & 0x3ffff);
        get_kbch_nbch(framesize, rate, &kbch, &nbch, &bch_code);
        printf("tags = %d, noutput_items = %d, produced = %d, consumed = %d, value = %d, nread = %d\n", (unsigned int)tags.size(), noutput_items, produced, consumed, goldcode, (unsigned int)nread);
        if (nbch + produced <= (unsigned int)noutput_items) {
          if (goldcode != check) {
            printf("bch index = %d, %d\n", goldcode, check);
            check = goldcode + 1;
          }
          else {
            check++;
          }
          const uint64_t tagoffset = this->nitems_written(0);
          const uint64_t tagmodcod = (uint64_t(goldcode) << 32) | (uint64_t(pilots) << 24) | (uint64_t(constellation) << 16) | (uint64_t(rate) << 8) | uint64_t(framesize);
          pmt::pmt_t key = pmt::string_to_symbol("modcod");
          pmt::pmt_t value = pmt::from_long(tagmodcod);
          this->add_item_tag(0, tagoffset, key, value);
          switch (bch_code) {
            case BCH_CODE_N12:
              //Zero the shift register
              memset(shift, 0, sizeof(unsigned int) * 6);
              // MSB of the codeword first
              for (int j = 0; j < (int)kbch; j++) {
                temp = *in++;
                *out++ = temp;
                b = (temp ^ (shift[5] & 1));
                reg_6_shift(shift);
                if (b) {
                  shift[0] ^= m_poly_n_12[0];
                  shift[1] ^= m_poly_n_12[1];
                  shift[2] ^= m_poly_n_12[2];
                  shift[3] ^= m_poly_n_12[3];
                  shift[4] ^= m_poly_n_12[4];
                  shift[5] ^= m_poly_n_12[5];
                }
              }
              consumed += kbch;
              produced += kbch;
              // Now add the parity bits to the output
              for (int n = 0; n < 192; n++) {
                *out++ = (shift[5] & 1);
                reg_6_shift(shift);
              }
              produced += 192;
              break;
            case BCH_CODE_N10:
              //Zero the shift register
              memset(shift, 0, sizeof(unsigned int) * 5);
              // MSB of the codeword first
              for (int j = 0; j < (int)kbch; j++) {
                temp = *in++;
                *out++ = temp;
                b = (temp ^ (shift[4] & 1));
                reg_5_shift(shift);
                if (b) {
                  shift[0] ^= m_poly_n_10[0];
                  shift[1] ^= m_poly_n_10[1];
                  shift[2] ^= m_poly_n_10[2];
                  shift[3] ^= m_poly_n_10[3];
                  shift[4] ^= m_poly_n_10[4];
                }
              }
              consumed += kbch;
              produced += kbch;
              // Now add the parity bits to the output
              for( int n = 0; n < 160; n++ ) {
                *out++ = (shift[4] & 1);
                reg_5_shift(shift);
              }
              produced += 160;
              break;
            case BCH_CODE_N8:
              //Zero the shift register
              memset(shift, 0, sizeof(unsigned int) * 4);
              // MSB of the codeword first
              for (int j = 0; j < (int)kbch; j++) {
                temp = *in++;
                *out++ = temp;
                b = temp ^ (shift[3] & 1);
                reg_4_shift(shift);
                if (b) {
                  shift[0] ^= m_poly_n_8[0];
                  shift[1] ^= m_poly_n_8[1];
                  shift[2] ^= m_poly_n_8[2];
                  shift[3] ^= m_poly_n_8[3];
                }
              }
              consumed += kbch;
              produced += kbch;
              // Now add the parity bits to the output
              for (int n = 0; n < 128; n++) {
                *out++ = shift[3] & 1;
                reg_4_shift(shift);
              }
              produced += 128;
              break;
            case BCH_CODE_S12:
              //Zero the shift register
              memset(shift, 0, sizeof(unsigned int) * 6);
              // MSB of the codeword first
              for (int j = 0; j < (int)kbch; j++) {
                temp = *in++;
                *out++ = temp;
                b = (temp ^ ((shift[5] & 0x01000000) ? 1 : 0));
                reg_6_shift(shift);
                if (b) {
                  shift[0] ^= m_poly_s_12[0];
                  shift[1] ^= m_poly_s_12[1];
                  shift[2] ^= m_poly_s_12[2];
                  shift[3] ^= m_poly_s_12[3];
                  shift[4] ^= m_poly_s_12[4];
                  shift[5] ^= m_poly_s_12[5];
                }
              }
              consumed += kbch;
              produced += kbch;
              // Now add the parity bits to the output
              for (int n = 0; n < 168; n++) {
                *out++ = (shift[5] & 0x01000000) ? 1 : 0;
                reg_6_shift(shift);
              }
              produced += 168;
              break;
            case BCH_CODE_M12:
              //Zero the shift register
              memset(shift, 0, sizeof(unsigned int) * 6);
              // MSB of the codeword first
              for (int j = 0; j < (int)kbch; j++) {
                temp = *in++;
                *out++ = temp;
                b = (temp ^ ((shift[5] & 0x00001000) ? 1 : 0));
                reg_6_shift(shift);
                if (b) {
                  shift[0] ^= m_poly_m_12[0];
                  shift[1] ^= m_poly_m_12[1];
                  shift[2] ^= m_poly_m_12[2];
                  shift[3] ^= m_poly_m_12[3];
                  shift[4] ^= m_poly_m_12[4];
                  shift[5] ^= m_poly_m_12[5];
                }
              }
              consumed += kbch;
              produced += kbch;
              // Now add the parity bits to the output
              for (int n = 0; n < 180; n++) {
                *out++ = (shift[5] & 0x00001000) ? 1 : 0;
                reg_6_shift(shift);
              }
              produced += 180;
              break;
          }
        }
        else {
          break;
        }
      }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (consumed);

      // Tell runtime system how many output items we produced.
      return produced;
    }

  } /* namespace dvbs2 */
} /* namespace gr */

