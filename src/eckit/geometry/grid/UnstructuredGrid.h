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

#include "eckit/geometry/Grid.h"


namespace eckit::geometry::grid {


class UnstructuredGrid : public Grid {
public:
    // -- Exceptions
    // None

    // -- Constructors

    explicit UnstructuredGrid(std::vector<Point>&&);

    // -- Destructor

    ~UnstructuredGrid() override;

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

    std::vector<Point> points_;

    // -- Methods
    // None

    // -- Overridden methods

    iterator cbegin() const override;
    iterator cend() const override;

    bool isPeriodicWestEast() const override { return true; }
    bool includesNorthPole() const override { return true; }
    bool includesSouthPole() const override { return true; }

    size_t size() const override;

    // -- Class members
    // None

    // -- Class methods
    // None

    // -- Friends
    // None
};


}  // namespace eckit::geometry::grid
