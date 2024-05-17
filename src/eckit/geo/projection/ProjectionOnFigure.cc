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


#include "eckit/geo/projection/ProjectionOnFigure.h"

#include "eckit/exception/Exceptions.h"
#include "eckit/geo/geometry/Earth.h"
#include "eckit/geo/spec/Custom.h"
#include "eckit/types/FloatCompare.h"


namespace eckit::geo::projection {


// FIXME refactor figures into a single hierarchy
struct Earth final : Figure {
    Earth() = default;

    double R() const override { return geometry::Earth::radius(); }
    double a() const override { return geometry::Earth::radius(); }
    double b() const override { return geometry::Earth::radius(); }
};


ProjectionOnFigure::ProjectionOnFigure(const Spec&) : ProjectionOnFigure() {}


ProjectionOnFigure::ProjectionOnFigure(Figure* figure_ptr) : figure_(figure_ptr != nullptr ? figure_ptr : new Earth) {
    ASSERT(figure_);
}


void ProjectionOnFigure::spec(spec::Custom& custom) const {
    // FIXME OO figure
    if (types::is_approximately_equal(figure_->a(), figure_->b())) {
        custom.set("R", figure_->R());
        return;
    }
    custom.set("a", figure_->a());
    custom.set("b", figure_->b());
}


}  // namespace eckit::geo::projection
