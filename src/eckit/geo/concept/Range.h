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

#include <cstddef>


namespace eckit::geo::concept {


    class Range {
    public:
        // -- Types
        // None

        // -- Exceptions
        // None

        // -- Constructors

        Range(size_t N, double a, double b, double inc, double ref);

        Range(size_t N, double a, double b, double inc) :
            Range(N, a, b, inc, a) {}

        // -- Destructor

        virtual ~Range() = default;

        // -- Convertors
        // None

        // -- Operators
        // None

        // -- Methods

        size_t size() const { return N_; }

        // -- Overridden methods
        // None

        // -- Class members
        // None

        // -- Class methods
        // None

    protected:
        // -- Members
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

        const size_t N_;

        // -- Methods
        // None

        // -- Overridden methods
        // None

        // -- Class members
        // None

        // -- Class methods
        // None

        // -- Friends
        // None
    };


}  // namespace eckit::geo::concept
