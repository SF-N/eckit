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


#include "eckit/geo/range/RegularLatitude.h"

#include "eckit/exception/Exceptions.h"
#include "eckit/types/FloatCompare.h"
#include "eckit/types/Fraction.h"


namespace eckit::geo::range {


static constexpr auto DB = 1e-12;


RegularLatitude::RegularLatitude(size_t n, double _a, double _b, double _eps) :
    Regular(n, _a, _b, _eps) {
    ASSERT(-90 <= a() && a() <= 90);
    ASSERT(-90 <= b() && b() <= 90);
}


Range* RegularLatitude::crop(double crop_a, double crop_b) const {
    ASSERT((a() < b() && crop_a <= crop_b) || (a() > b() && crop_a >= crop_b) ||
           (types::is_approximately_equal(a(), b(), eps()) && types::is_approximately_equal(crop_a, crop_b, eps())));

    if (types::is_approximately_equal(crop_a, crop_b, eps())) {
        NOTIMP;  // FIXME
    }
    else if (a() < b()) {
        ASSERT(a() <= crop_a && crop_b <= b());  // FIXME do better

        auto inc(increment());
        auto d  = (a() / inc).decimalPart() * inc;
        auto _a = adjust(crop_a - d, inc, true) + d;
        auto _b = adjust(crop_b - d, inc, false) + d;

        auto nf = (_b - _a) / inc;
        ASSERT(nf.integer());

        auto n = static_cast<size_t>(nf.integralPart() + 1);
        ASSERT(0 < n && n <= size());

        return new RegularLatitude(n, _a, _b, eps());
    }
    else {
        NOTIMP;  // FIXME
    }

    NOTIMP;
}


}  // namespace eckit::geo::range
