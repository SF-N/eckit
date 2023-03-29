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


#pragma once


#include <vector>

#include "grit/types.h"


namespace grit::geometry {


class GreatCircle {
public:
    /// Great circle given two points in geographic coordinates
    GreatCircle(const Point2&, const Point2&);

    /// Great circle latitude given longitude, see http://www.edwilliams.org/avform.htm#Int
    std::vector<double> latitude(double lon) const;

    /// Great circle longitude given latitude, see http://www.edwilliams.org/avform.htm#Par
    std::vector<double> longitude(double lat) const;

    bool crossesPoles() const { return crossesPoles_; }

private:
    const Point2 A_;
    const Point2 B_;

    bool crossesPoles_;
};


}  // namespace grit::geometry
