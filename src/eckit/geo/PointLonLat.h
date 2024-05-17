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

#include <algorithm>
#include <array>
#include <ostream>


namespace eckit::geo {
class PointLonLatR;
}


namespace eckit::geo {


class PointLonLat final : protected std::array<double, 2> {
public:
    // -- Types

    using container_type = array;
    using container_type::value_type;

    // -- Constructors

    PointLonLat() : PointLonLat(0., 0.) {}

    PointLonLat(value_type lon, value_type lat) : container_type{lon, lat} {}

    PointLonLat(const PointLonLat& other) : container_type(other) {}

    PointLonLat(PointLonLat&& other) : container_type(other) {}

    // -- Destructor

    ~PointLonLat() = default;

    // -- Operators

    PointLonLat& operator=(const PointLonLat& other) {
        container_type::operator=(other);
        return *this;
    }

    PointLonLat& operator=(PointLonLat&& other) {
        container_type::operator=(other);
        return *this;
    }

    // -- Members

    const value_type& lon = container_type::operator[](0);
    const value_type& lat = container_type::operator[](1);

    // -- Methods

    static value_type normalise_angle_to_minimum(value_type, value_type minimum);

    static value_type normalise_angle_to_maximum(value_type, value_type maximum);

    static void assert_latitude_range(const PointLonLat&);

    [[nodiscard]] static PointLonLat make(value_type lon, value_type lat, value_type lon_minimum = EQUATOR,
                                          value_type eps = EPS);

    [[nodiscard]] static PointLonLat make_from(const PointLonLatR&);

    PointLonLat antipode() const { return make(lon, lat + GLOBE / 2.); }

    // -- Class members

    static constexpr double GLOBE        = 360.;
    static constexpr double GREENWICH    = 0.;
    static constexpr double ANTIMERIDIAN = -180.;
    static constexpr double EQUATOR      = 0.;
    static constexpr double NORTH_POLE   = 90.;
    static constexpr double SOUTH_POLE   = -90.;

    static constexpr value_type EPS = 1e-9;

    // -- Class methods

    static PointLonLat componentsMin(const PointLonLat& p, const PointLonLat& q) {
        return {std::min(p.lon, q.lon), std::min(p.lat, q.lat)};
    }

    static PointLonLat componentsMax(const PointLonLat& p, const PointLonLat& q) {
        return {std::max(p.lon, q.lon), std::max(p.lat, q.lat)};
    }

    // -- Friends

    friend std::ostream& operator<<(std::ostream& out, const PointLonLat& p) {
        return out << '{' << p.lon << ", " << p.lat << '}';
    }

    friend PointLonLat operator-(const PointLonLat& p, const PointLonLat& q) { return {p.lon - q.lon, p.lat - q.lat}; }
    friend PointLonLat operator+(const PointLonLat& p, const PointLonLat& q) { return {p.lon + q.lon, p.lat + q.lat}; }
    friend PointLonLat operator*(const PointLonLat& p, value_type d) { return {p.lon * d, p.lat * d}; }

    friend bool operator<(const PointLonLat& p, const PointLonLat& q) {
        return static_cast<const container_type&>(p) < static_cast<const container_type&>(q);
    }
};


bool points_equal(const PointLonLat&, const PointLonLat&, PointLonLat::value_type eps = PointLonLat::EPS);


}  // namespace eckit::geo
