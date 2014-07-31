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


#ifndef INCLUDED_DVBS2_BBSCRAMBLER_BB_H
#define INCLUDED_DVBS2_BBSCRAMBLER_BB_H

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
    class DVBS2_API bbscrambler_bb : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<bbscrambler_bb> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of dvbs2::bbscrambler_bb.
       *
       * To avoid accidental use of raw pointers, dvbs2::bbscrambler_bb's
       * constructor is in a private implementation
       * class. dvbs2::bbscrambler_bb::make is the public interface for
       * creating new instances.
       */
      static sptr make(dvbs2_code_rate_t rate, dvbs2_framesize_t framesize);
    };

  } // namespace dvbs2
} // namespace gr

#endif /* INCLUDED_DVBS2_BBSCRAMBLER_BB_H */

