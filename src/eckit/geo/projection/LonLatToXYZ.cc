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


#include "eckit/geo/projection/LonLatToXYZ.h"

#include "eckit/geo/geometry/OblateSpheroid.h"
#include "eckit/geo/geometry/Sphere.h"
#include "eckit/geo/spec/Custom.h"
#include "eckit/types/FloatCompare.h"


namespace eckit::geo::projection {


static ProjectionBuilder<LonLatToXYZ> PROJECTION("ll_to_xyz");


LonLatToXYZ::LonLatToXYZ(double a, double b) {
    ASSERT_MSG(types::is_strictly_greater(b, 0.) && b <= a, "LonLatToXYZ requires 0 < b <= a");

    struct LonLatToSphereXYZ final : Implementation {
        using S = geometry::Sphere;
        const double R_;

        explicit LonLatToSphereXYZ(double R) : R_(R) {}
        Point3 operator()(const PointLonLat& p) const override { return S::convertSphericalToCartesian(R_, p, 0.); }
        PointLonLat operator()(const Point3& q) const override { return S::convertCartesianToSpherical(R_, q); }
        void spec(spec::Custom& custom) const override { custom.set("R", R_); }
    };

    struct LonLatToSpheroidXYZ final : Implementation {
        using S = geometry::OblateSpheroid;
        const double a_;
        const double b_;

        explicit LonLatToSpheroidXYZ(double a, double b) : a_(a), b_(b) {}
        Point3 operator()(const PointLonLat& p) const override { return S::convertSphericalToCartesian(a_, b_, p, 0.); }
        PointLonLat operator()(const Point3& q) const override { NOTIMP; }
        void spec(spec::Custom& custom) const override {
            custom.set("a", a_);
            custom.set("b", b_);
        }
    };

    impl_.reset(types::is_approximately_equal(a, b) ? static_cast<Implementation*>(new LonLatToSphereXYZ(a))
                                                    : new LonLatToSpheroidXYZ(a, b));
}


LonLatToXYZ::LonLatToXYZ(double R) : LonLatToXYZ(R, R) {}


LonLatToXYZ::LonLatToXYZ(const Spec& spec) :
    LonLatToXYZ(spec.get_double("a", spec.get_double("R", 1.)), spec.get_double("b", spec.get_double("R", 1.))) {}


void LonLatToXYZ::spec(spec::Custom& custom) const {
    impl_->spec(custom);
}


}  // namespace eckit::geo::projection
