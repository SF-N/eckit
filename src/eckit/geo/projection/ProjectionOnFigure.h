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

#include <memory>

#include "eckit/geo/Figure.h"
#include "eckit/geo/Projection.h"


namespace eckit::geo::projection {


class ProjectionOnFigure : public Projection {
protected:
    // -- Constructors

    explicit ProjectionOnFigure(const Spec&);
    explicit ProjectionOnFigure(Figure* = nullptr);

    // -- Methods

    const Figure& figure() const { return *figure_; }

    // -- Overridden methods

    void spec(spec::Custom&) const override;

private:
    // -- Members

    const std::unique_ptr<Figure> figure_;
};


}  // namespace eckit::geo::projection
