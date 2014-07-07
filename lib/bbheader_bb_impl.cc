/* -*- c++ -*- */
/* 
 * Copyright 2014 Ron Economos.
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
#include <stdio.h>

namespace gr {
  namespace dvbs2 {

    bbheader_bb::sptr
    bbheader_bb::make(dvbs2_code_rate_t rate, dvbs2_rolloff_factor_t rolloff)
    {
      return gnuradio::get_initial_sptr
        (new bbheader_bb_impl(rate, rolloff));
    }

    /*
     * The private constructor
     */
    bbheader_bb_impl::bbheader_bb_impl(dvbs2_code_rate_t rate, dvbs2_rolloff_factor_t rolloff)
      : gr::block("bbheader_bb",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {
        count = 0;
        crc = 0x47;
        BBHeader *f = &m_format[0].bb_header;
        switch (rate)
        {
            case gr::dvbs2::C1_4:
                kbch = 16008;
                break;
            case gr::dvbs2::C1_3:
                kbch = 21408;
                break;
            case gr::dvbs2::C2_5:
                kbch = 25728;
                break;
            case gr::dvbs2::C1_2:
                kbch = 32208;
                break;
            case gr::dvbs2::C3_5:
                kbch = 38688;
                break;
            case gr::dvbs2::C2_3:
                kbch = 43040;
                break;
            case gr::dvbs2::C3_4:
                kbch = 48408;
                break;
            case gr::dvbs2::C4_5:
                kbch = 51648;
                break;
            case gr::dvbs2::C5_6:
                kbch = 53840;
                break;
            case gr::dvbs2::C8_9:
                kbch = 57472;
                break;
            case gr::dvbs2::C9_10:
                kbch = 58192;
                break;
            default:
                kbch = 0;
                break;
        }

        f->ts_gs   = TS_GS_TRANSPORT;
        f->sis_mis = SIS_MIS_SINGLE;
        f->ccm_acm = CCM;
        f->issyi   = ISSYI_NOT_ACTIVE;
        f->npd     = NPD_NOT_ACTIVE;
        f->upl     = 188 * 8;
        f->dfl     = kbch - 80;
        f->sync    = 0x47;
        f->ro      = rolloff;

        build_crc8_table();
        set_output_multiple(kbch);
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

void bbheader_bb_impl::build_crc8_table(void)
{
    int r,crc;

    for (int i = 0; i < 256; i++)
    {
        r = i;
        crc = 0;
        for (int j = 7; j >= 0; j--)
        {
            if ((r & (1 << j) ? 1 : 0) ^ ((crc & 0x80) ? 1 : 0))
                crc = (crc << 1) ^ CRC_POLYR;
            else
                crc <<= 1;
        }
        crc_tab[i] = crc;
    }
}

//
// MSB is sent first
//
// The polynomial has been reversed
//
int bbheader_bb_impl::add_crc8_bits(unsigned char *in, int length)
{
    int crc = 0;
    int b;
    int i = 0;

    for (int n = 0; n < length; n++)
    {
        b = in[i++] ^ (crc & 0x01);
        crc >>= 1;
        if (b) crc ^= CRC_POLY;
    }

    for (int n = 0; n < 8; n++)
    {
        in[i++] = (crc & (1 << n)) ? 1 : 0;
    }
    return 8;// Length of CRC
}

void bbheader_bb_impl::add_bbheader(unsigned char *out, int count)
{
    int temp, m_frame_offset_bits;
    unsigned char *m_frame = out;
    BBHeader *h = &m_format[0].bb_header;

    m_frame[0] = h->ts_gs >> 1;
    m_frame[1] = h->ts_gs & 1;
    m_frame[2] = h->sis_mis;
    m_frame[3] = h->ccm_acm;
    m_frame[4] = h->issyi & 1;
    m_frame[5] = h->npd & 1;
    m_frame[6] = h->ro >> 1;
    m_frame[7] = h->ro & 1;
    m_frame_offset_bits = 8;
    if (h->sis_mis == SIS_MIS_MULTIPLE)
    {
        temp = h->isi;
        for (int n = 7; n >= 0; n--)
        {
            m_frame[m_frame_offset_bits++] = temp & (1 << n) ? 1 : 0;
        }
    }
    else
    {
        for (int n = 7; n >= 0 ; n--)
        {
            m_frame[m_frame_offset_bits++] = 0;
        }
    }
    temp = h->upl;
    for (int n = 15; n >= 0; n--)
    {
        m_frame[m_frame_offset_bits++] = temp & (1 << n) ? 1 : 0;
    }
    temp = h->dfl;
    for (int n = 15; n >= 0; n--)
    {
        m_frame[m_frame_offset_bits++] = temp & (1 << n) ? 1 : 0;
    }
    temp = h->sync;
    for (int n = 7; n >= 0; n--)
    {
        m_frame[m_frame_offset_bits++] = temp & (1 << n) ? 1 : 0;
    }
    // Calculate syncd, this should point to the MSB of the CRC
    temp = count;
    if (temp == 0)
        temp = count;
    else
        temp = (188 - count) * 8;
    for (int n = 15; n >= 0; n--)
    {
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
        const unsigned char *in = (const unsigned char *) input_items[0];
        unsigned char *out = (unsigned char *) output_items[0];
        int consumed = 0;
        int offset = 0;
        unsigned char b;


        for (int i = 0; i < noutput_items; i += kbch)
        {
            add_bbheader(&out[offset], count);
            offset = offset + 80;

            for (int j = 0; j < (int)((kbch - 80) / 8); j++)
            {
                if (count == 0)
                {
                    if (*in != 0x47)
                    {
                        printf("Transport Stream sync error!\n");
                    }
                    in++;
                    b = crc;
                    crc = 0;
                }
                else
                {
                    b = *in++;
                    crc = crc_tab[b ^ crc];
                }
                count = (count + 1) % 188;
                consumed++;
                for (int n = 7; n >= 0; n--)
                {
                    out[offset++] = b & (1 << n) ? 1 : 0;
                }
            }
        }

        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (consumed);

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace dvbs2 */
} /* namespace gr */

