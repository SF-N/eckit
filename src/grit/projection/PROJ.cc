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


#include "grit/projection/PROJ.h"

#include "grit/Parametrisation.h"
#include "grit/exception.h"
#include "grit/types.h"


namespace grit::projection {


PROJ::PROJ(const std::string& source, const std::string& target, double(lon_minimum)) :
    proj_(nullptr), ctx_(PJ_DEFAULT_CTX), source_(source), target_(target) {
    ASSERT(!source.empty());

    // projection, normalised
    pj_t p(proj_create_crs_to_crs(ctx_.get(), source_.c_str(), target_.c_str(), nullptr));
    ASSERT(p);

    proj_.reset(proj_normalize_for_visualization(ctx_.get(), p.get()));
    ASSERT(proj_);

    struct LatLon final : Convert {
        PJ_COORD convert(const Point& p) const final {
            const auto& q = std::get<PointLatLon>(p);
            return proj_coord(q.lon, q.lat, 0, 0);
        }

        Point convert(const PJ_COORD& c) const final { return PointLatLon::make(c.enu.n, c.enu.e, lon_minimum_); }

        explicit LatLon(double lon_minimum) : lon_minimum_(lon_minimum) {}
        const double lon_minimum_;
    };

    struct XY final : Convert {
        PJ_COORD convert(const Point& p) const final {
            const auto& q = std::get<PointXY>(p);
            return proj_coord(q.x, q.y, 0, 0);
        }

        Point convert(const PJ_COORD& c) const final { return PointXY{c.xy.x, c.xy.y}; }
    };

    struct XYZ final : Convert {
        PJ_COORD convert(const Point& p) const final {
            const auto& q = std::get<PointXYZ>(p);
            return proj_coord(q.x, q.y, q.z, 0);
        }

        Point convert(const PJ_COORD& c) const final { return PointXYZ{c.xy.x, c.xy.y, c.xyz.z}; }
    };

    auto convert_ptr = [lon_minimum](const std::string& string) -> Convert* {
        constexpr auto ctx = PJ_DEFAULT_CTX;

        pj_t identity(proj_create_crs_to_crs(ctx, string.c_str(), string.c_str(), nullptr));
        pj_t crs(proj_get_target_crs(ctx, identity.get()));
        pj_t cs(proj_crs_get_coordinate_system(ctx, crs.get()));
        ASSERT(cs);

        auto type = proj_cs_get_type(ctx, cs.get());
        auto dim  = proj_cs_get_axis_count(ctx, cs.get());

        return type == PJ_CS_TYPE_CARTESIAN && dim == 3   ? static_cast<Convert*>(new XYZ)
               : type == PJ_CS_TYPE_CARTESIAN && dim == 2 ? static_cast<Convert*>(new XY)
               : type == PJ_CS_TYPE_ELLIPSOIDAL           ? static_cast<Convert*>(new LatLon(lon_minimum))
               : type == PJ_CS_TYPE_SPHERICAL             ? static_cast<Convert*>(new LatLon(lon_minimum))
                                                          : NOTIMP;
    };

    from_.reset(convert_ptr(source_));
    ASSERT(from_);

    to_.reset(convert_ptr(target_));
    ASSERT(to_);
}


PROJ::PROJ(const Parametrisation& param) :
    PROJ(param.has("source") ? param.get_string("source") : "EPSG:4326",  // default to WGS 84
         param.has("target") ? param.get_string("target") : "EPSG:4326",  // ...
         param.has("lon_minimum") ? param.get_double("lon_minimum") : 0) {}


std::string PROJ::ellipsoid(const std::string& string) {
    constexpr auto ctx = PJ_DEFAULT_CTX;

    pj_t identity(proj_create_crs_to_crs(ctx, string.c_str(), string.c_str(), nullptr));
    pj_t crs(proj_get_target_crs(ctx, identity.get()));
    pj_t ellipsoid(proj_get_ellipsoid(ctx, crs.get()));
    ASSERT(ellipsoid);

    double a = 0;
    double b = 0;
    ASSERT(proj_ellipsoid_get_parameters(ctx, ellipsoid.get(), &a, &b, nullptr, nullptr));
    ASSERT(0 < b && b <= a);

    return b < a ? "+a=" + std::to_string(a) + " +b=" + std::to_string(b) : "+R=" + std::to_string(a);
}


Point PROJ::fwd(const Point& p) const {
    return to_->convert(proj_trans(proj_.get(), PJ_FWD, from_->convert(p)));
}


Point PROJ::inv(const Point& q) const {
    return from_->convert(proj_trans(proj_.get(), PJ_INV, to_->convert(q)));
}


}  // namespace grit::projection
