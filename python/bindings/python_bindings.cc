/*
 * Copyright 2020 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <pybind11/pybind11.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

namespace py = pybind11;

// Headers for binding functions
/**************************************/
/* The following comment block is used for
/* gr_modtool to insert function prototypes
/* Please do not delete
/**************************************/
// BINDING_FUNCTION_PROTOTYPES(
    void bind_bbheader_bb(py::module& m);
    void bind_bbheader_source(py::module& m);
    void bind_bbscrambler_bb(py::module& m);
    void bind_bch_bb(py::module& m);
    void bind_interleaver_bb(py::module& m);
    void bind_ldpc_bb(py::module& m);
    void bind_modulator_bc(py::module& m);
    void bind_physical_cc(py::module& m);
    void bind_dvbs2_config(py::module& m);
// ) END BINDING_FUNCTION_PROTOTYPES


// We need this hack because import_array() returns NULL
// for newer Python versions.
// This function is also necessary because it ensures access to the C API
// and removes a warning.
void* init_numpy()
{
    import_array();
    return NULL;
}

PYBIND11_MODULE(dvbs2_python, m)
{
    // Initialize the numpy C API
    // (otherwise we will see segmentation faults)
    init_numpy();

    // Allow access to base block methods
    py::module::import("gnuradio.gr");

    /**************************************/
    /* The following comment block is used for
    /* gr_modtool to insert binding function calls
    /* Please do not delete
    /**************************************/
    // BINDING_FUNCTION_CALLS(
    bind_bbheader_bb(m);
    bind_bbheader_source(m);
    bind_bbscrambler_bb(m);
    bind_bch_bb(m);
    bind_interleaver_bb(m);
    bind_ldpc_bb(m);
    bind_modulator_bc(m);
    bind_physical_cc(m);
    bind_dvbs2_config(m);
    // ) END BINDING_FUNCTION_CALLS
}
