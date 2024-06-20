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


#include "eckit/geo/projection/PROJ.h"

#include <proj.h>

#include <map>

#include "eckit/exception/Exceptions.h"
#include "eckit/geo/Figure.h"
#include "eckit/geo/spec/Custom.h"


namespace eckit::geo::projection {


static ProjectionBuilder<PROJ> PROJECTION("proj");


namespace {


constexpr auto CTX               = PJ_DEFAULT_CTX;
static const std::string DEFAULT = "EPSG:4326";  // wgs84, latitude/longitude coordinate system


struct pj_t : std::unique_ptr<PJ, decltype(&proj_destroy)> {
    explicit pj_t(element_type* ptr) : unique_ptr(ptr, &proj_destroy) {}
};


struct ctx_t : std::unique_ptr<PJ_CONTEXT, decltype(&proj_context_destroy)> {
    explicit ctx_t(element_type* ptr) : unique_ptr(ptr, &proj_context_destroy) {}
};


struct Convert {
    Convert()          = default;
    virtual ~Convert() = default;

    Convert(const Convert&)        = delete;
    Convert(Convert&&)             = delete;
    void operator=(const Convert&) = delete;
    void operator=(Convert&&)      = delete;

    virtual PJ_COORD to_coord(const Point&) const = 0;
    virtual Point to_point(const PJ_COORD&) const = 0;
};


struct LonLat final : Convert {
    PJ_COORD to_coord(const Point& p) const final {
        const auto& q = std::get<PointLonLat>(p);
        return proj_coord(q.lon, q.lat, 0, 0);
    }

    Point to_point(const PJ_COORD& c) const final { return PointLonLat::make(c.enu.e, c.enu.n, lon_minimum_); }

    explicit LonLat(double lon_minimum) : lon_minimum_(lon_minimum) {}
    const double lon_minimum_;
};


struct XY final : Convert {
    PJ_COORD to_coord(const Point& p) const final {
        const auto& q = std::get<Point2>(p);
        return proj_coord(q.X, q.Y, 0, 0);
    }

    Point to_point(const PJ_COORD& c) const final { return Point2{c.xy.x, c.xy.y}; }
};


struct XYZ final : Convert {
    PJ_COORD to_coord(const Point& p) const final {
        const auto& q = std::get<Point3>(p);
        return proj_coord(q.X, q.Y, q.Z, 0);
    }

    Point to_point(const PJ_COORD& c) const final { return Point3{c.xy.x, c.xy.y, c.xyz.z}; }
};


}  // namespace


struct PROJ::Implementation {
    Implementation(PJ* pj_ptr, PJ_CONTEXT* pjc_ptr, Convert* source_ptr, Convert* target_ptr) :
        proj_(pj_ptr), ctx_(pjc_ptr), source_(source_ptr), target_(target_ptr) {
        ASSERT(proj_);
        ASSERT(source_);
        ASSERT(target_);
    }

    inline Point fwd(const Point& p) const {
        return target_->to_point(proj_trans(proj_.get(), PJ_FWD, source_->to_coord(p)));
    }

    inline Point inv(const Point& p) const {
        return source_->to_point(proj_trans(proj_.get(), PJ_INV, target_->to_coord(p)));
    }

private:
    const pj_t proj_;
    const ctx_t ctx_;
    const std::unique_ptr<Convert> source_;
    const std::unique_ptr<Convert> target_;
};


PROJ::PROJ(const std::string& source, const std::string& target, double lon_minimum) :
    source_(source), target_(target) {
    ASSERT(!source.empty());

    auto make_convert = [lon_minimum](const std::string& string) -> Convert* {
        pj_t identity(proj_create_crs_to_crs(CTX, string.c_str(), string.c_str(), nullptr));
        pj_t crs(proj_get_target_crs(CTX, identity.get()));
        pj_t cs(proj_crs_get_coordinate_system(CTX, crs.get()));
        ASSERT(cs);

        auto type = proj_cs_get_type(CTX, cs.get());
        auto dim  = proj_cs_get_axis_count(CTX, cs.get());

        return type == PJ_CS_TYPE_CARTESIAN && dim == 3   ? static_cast<Convert*>(new XYZ)
               : type == PJ_CS_TYPE_CARTESIAN && dim == 2 ? static_cast<Convert*>(new XY)
               : type == PJ_CS_TYPE_ELLIPSOIDAL           ? static_cast<Convert*>(new LonLat(lon_minimum))
               : type == PJ_CS_TYPE_SPHERICAL             ? static_cast<Convert*>(new LonLat(lon_minimum))
                                                          : NOTIMP;
    };

    // projection, normalised
    auto ctx = PJ_DEFAULT_CTX;

    implementation_ = std::make_unique<Implementation>(
        proj_normalize_for_visualization(ctx, proj_create_crs_to_crs(ctx, source_.c_str(), target_.c_str(), nullptr)),
        ctx, make_convert(source_), make_convert(target_));
    ASSERT(implementation_);
}


PROJ::PROJ(const Spec& spec) :
    PROJ(spec.get_string("source", spec.get_string("proj", DEFAULT)),  // default to WGS 84
         spec.get_string("target", DEFAULT),                           // ...
         spec.get_double("lon_minimum", 0)) {}


Figure* PROJ::make_figure() const {
    pj_t identity(proj_create_crs_to_crs(CTX, target_.c_str(), target_.c_str(), nullptr));

    pj_t crs(proj_get_target_crs(CTX, identity.get()));
    pj_t ellipsoid(proj_get_ellipsoid(CTX, crs.get()));
    ASSERT(ellipsoid);

    double a = 0;
    double b = 0;
    ASSERT(proj_ellipsoid_get_parameters(CTX, ellipsoid.get(), &a, &b, nullptr, nullptr));
    ASSERT(0 < b && b <= a);

    return FigureFactory::build(spec::Custom{{{"a", a}, {"b", b}}});
}


Point PROJ::fwd(const Point& p) const {
    return implementation_->fwd(p);
}


Point PROJ::inv(const Point& q) const {
    return implementation_->inv(q);
}


void PROJ::fill_proj(std::string& str, const spec::Custom& custom) {
    struct rename_type : std::map<std::string, std::string> {
        using map::map;
        const std::string& operator()(const std::string& key) const {
            const auto it = find(key);
            return it != end() ? it->second : key;
        }
    };

    static const rename_type RENAME_KEYS{
        {"projection", "proj"},
        {"figure", "ellps"},
        {"r", "R"},
    };

    static const rename_type RENAME_VALUES{
        {"mercator", "merc"},
        {"grs80", "GRS80"},
        {"wgs84", "WGS84"},
    };

    for (const auto& [k, v] : custom.container()) {
        if (const auto& key = RENAME_KEYS(k); !key.empty()) {
            const auto value = RENAME_VALUES(to_string(v));
            str += " +" + key + "=" + value;
        }
    }
}


void PROJ::fill_spec(spec::Custom& custom) const {
    custom.set("projection", "proj");
    if (source_ != DEFAULT) {
        custom.set("source", source_);
    }
    if (target_ != DEFAULT) {
        custom.set("target", target_);
    }
}


}  // namespace eckit::geo::projection
