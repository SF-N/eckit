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

#include <iosfwd>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "eckit/geometry/Area.h"
#include "eckit/geometry/Configurator.h"
#include "eckit/geometry/Increments.h"
#include "eckit/geometry/Iterator.h"
#include "eckit/geometry/Ordering.h"
#include "eckit/geometry/Point.h"
#include "eckit/geometry/Renumber.h"
#include "eckit/geometry/area/BoundingBox.h"
#include "eckit/memory/Builder.h"
#include "eckit/memory/Factory.h"


namespace eckit {
class Configuration;
}


namespace eckit::geometry {


class Grid {
public:
    // -- Types

    using builder_t = BuilderT1<Grid>;
    using ARG1      = const Configuration&;

    struct Iterator final : std::unique_ptr<geometry::Iterator> {
        explicit Iterator(geometry::Iterator* it) :
            unique_ptr(it) { ASSERT(unique_ptr::operator bool()); }
        using diff_t = unique_ptr::element_type::diff_t;

        Iterator(const Iterator&) = delete;
        Iterator(Iterator&&)      = delete;

        ~Iterator() = default;

        void operator=(const Iterator&) = delete;
        void operator=(Iterator&&)      = delete;

        bool operator==(const Iterator& other) const { return get()->operator==(*(other.get())); }
        bool operator!=(const Iterator& other) const { return get()->operator!=(*(other.get())); }

        bool operator++() { return get()->operator++(); }
        bool operator+=(diff_t d) { return get()->operator+=(d); }

        bool operator--() { return get()->operator--(); }
        bool operator-=(diff_t d) { return get()->operator-=(d); }

        explicit operator bool() const { return get()->operator bool(); }
        Point operator*() const { return get()->operator*(); }

        size_t index() const { return get()->index(); }
    };

    using iterator = Iterator;

    // -- Exceptions
    // None

    // -- Constructors

    explicit Grid(const Configuration&);

    Grid(const Grid&) = delete;
    Grid(Grid&&)      = delete;

    // -- Destructor

    virtual ~Grid() = default;

    // -- Convertors
    // None

    // -- Operators
    // None

    Grid& operator=(const Grid&) = delete;
    Grid& operator=(Grid&&)      = delete;

    // -- Methods

    iterator begin() const { return cbegin(); }
    iterator end() const { return cend(); }

    virtual iterator cbegin() const = 0;
    virtual iterator cend() const   = 0;

    virtual const area::BoundingBox& boundingBox() const;
    void boundingBox(const area::BoundingBox&);

    virtual size_t size() const;

    virtual bool includesNorthPole() const;
    virtual bool includesSouthPole() const;
    virtual bool isPeriodicWestEast() const;

    virtual std::vector<Point> to_points() const;
    virtual std::pair<std::vector<double>, std::vector<double>> to_latlon() const;

    virtual Ordering order() const;
    virtual Renumber reorder(const PointLonLat&) const;

    // -- Overridden methods
    // None

    // -- Class members
    // None

    // -- Class methods

    static std::string className() { return "grid"; }

protected:
    // -- Constructors

    explicit Grid(const area::BoundingBox&);

    // -- Methods

    area::BoundingBox bbox() const { return bbox_; }
    void bbox(const area::BoundingBox& bbox) { bbox_ = bbox; }

    // -- Overridden methods
    // None

    // -- Class members
    // None

    // -- Class methods
    // None

private:
    // -- Members

    area::BoundingBox bbox_;

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


using GridFactoryType       = Factory<Grid>;
using GridConfigurationName = Configurator<ConfigurationGeneratorT1<const std::string&>>;
using GridConfigurationUID  = Configurator<ConfigurationGeneratorT0>;


template <typename T>
using GridRegisterType = ConcreteBuilderT1<Grid, T>;

template <typename T>
using GridRegisterUID = ConcreteConfigurationGeneratorT0<T>;

template <typename T>
using GridRegisterName = ConcreteConfigurationGeneratorT1<T, const std::string&>;


struct GridFactory {
    // This is 'const' as Grid should always be immutable
    static const Grid* build(const Configuration& config) { return instance().build_(config); }
    static void list(std::ostream& out) { return instance().list_(out); }

private:
    static GridFactory& instance();

    // This is 'const' as Grid should always be immutable
    const Grid* build_(const Configuration&) const;
    void list_(std::ostream&) const;

    mutable Mutex mutex_;
};


}  // namespace eckit::geometry
