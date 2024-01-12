/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 *
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */


#include "eckit/geo/Range.h"

#include "eckit/exception/Exceptions.h"
#include "eckit/geo/Spec.h"


namespace eckit::geo {


Range::Range(const Spec& spec) :
    Range(spec.get_unsigned("n", 0)) {}


Range::Range(size_t n) :
    n_(n) {
    ASSERT(n > 0);
}


}  // namespace eckit::geo
