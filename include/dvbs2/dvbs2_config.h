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


#ifndef INCLUDED_DVBS2_CONFIG_H
#define INCLUDED_DVBS2_CONFIG_H

#define BB_HEADER_LENGTH_BITS 72

// BB HEADER fields
#define TS_GS_TRANSPORT 3
#define TS_GS_GENERIC_PACKETIZED 0
#define TS_GS_GENERIC_CONTINUOUS 1
#define TS_GS_RESERVED 2

#define SIS_MIS_SINGLE 1
#define SIS_MIS_MULTIPLE 0

#define CCM 1
#define ACM 0

#define ISSYI_ACTIVE 1
#define ISSYI_NOT_ACTIVE 0

#define NPD_ACTIVE 1
#define NPD_NOT_ACTIVE 0

#define FRAME_SIZE_NORMAL 64800
#define FRAME_SIZE_SHORT  16200

// BCH Code
#define BCH_CODE_N8  0
#define BCH_CODE_N10 1
#define BCH_CODE_N12 2
#define BCH_CODE_S12 3

#define LDPC_ENCODE_TABLE_LENGTH (FRAME_SIZE_NORMAL * 10)

namespace gr {
  namespace dvbs2 {
    enum dvbs2_code_rate_t {
      C1_4 = 0,
      C1_3,
      C2_5,
      C1_2,
      C3_5,
      C2_3,
      C3_4,
      C4_5,
      C5_6,
      C8_9,
      C9_10,
      C_OTHER,
    };

    enum dvbs2_rolloff_factor_t {
      RO_0_35 = 0,
      RO_0_25,
      RO_0_20,
      RO_RESERVED,
    };

    enum dvbs2_constellation_t {
      MOD_QPSK = 0,
      MOD_8PSK,
      MOD_16APSK,
      MOD_32APSK,
    };

    enum dvbs2_pilots_t {
      PILOTS_OFF = 0,
      PILOTS_ON,
    };
  } // namespace dvbs2
} // namespace gr

typedef gr::dvbs2::dvbs2_code_rate_t dvbs2_code_rate_t;
typedef gr::dvbs2::dvbs2_rolloff_factor_t dvbs2_rolloff_factor_t;
typedef gr::dvbs2::dvbs2_constellation_t dvbs2_constellation_t;
typedef gr::dvbs2::dvbs2_pilots_t dvbs2_pilots_t;

#endif /* INCLUDED_DVBS2_CONFIG_H */

