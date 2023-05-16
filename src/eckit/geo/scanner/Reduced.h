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

#include "eckit/geo/BoundingBox.h"
#include "eckit/geo/Point.h"
#include "eckit/geo/Scanner.h"
#include "eckit/geo/util.h"
#include "eckit/types/Fraction.h"


namespace eckit::geo::scanner {


class Reduced final : public Scanner {
public:
    // -- Types

    using pl_type = util::pl_type;

    // -- Exceptions
    // None

    // -- Constructors

    Reduced(const std::vector<double>& latitudes,
            pl_type&& pl,
            const BoundingBox&,
            size_t N,
            size_t Nj,
            size_t k);

    // -- Destructor
    // None

    // -- Convertors
    // None

    // -- Operators
    // None

    // -- Methods
    // None

    // -- Overridden methods
    // None

    // -- Class members
    // None

    // -- Class methods
    // None

private:
    // -- Members

    const std::vector<double>& latitudes_;
    const pl_type pl_;
    const BoundingBox& bbox_;
    const size_t N_;
    size_t Ni_;
    size_t Nj_;
    Fraction lon_;
    double lat_;
    Fraction inc_;
    size_t i_;
    size_t j_;
    size_t k_;
    size_t count_;
    bool first_;

    Point p_;

    // -- Methods

    void print(std::ostream&) const;
    size_t index() const;
    size_t resetToRow(size_t);

    // -- Overridden methods

    bool operator++() override;
    size_t size() const override;

    // -- Class members
    // None

    // -- Class methods
    // None

    // -- Friends
    // None
};


}  // namespace eckit::geo::scanner
