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
#include <string>

#include "eckit/geometry/Point.h"


namespace eckit {
class Configuration;
}


namespace eckit::geometry {


class Projection {
public:
    // -- Types

    using Type = std::string;

    // -- Exceptions
    // None

    // -- Constructors

    Projection() noexcept = default;

    Projection(const Projection&) = default;
    Projection(Projection&&)      = default;

    // -- Destructor

    virtual ~Projection() = default;

    // -- Convertors
    // None

    // -- Operators

    Projection& operator=(const Projection&) = default;
    Projection& operator=(Projection&&)      = default;

    // -- Methods

    virtual Point fwd(const Point&) const = 0;
    virtual Point inv(const Point&) const = 0;

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
    // None

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


struct ProjectionFactory {
    static Projection* build(const Configuration&);
    static Projection* build(const Projection::Type&, const Configuration&);
    static std::ostream& list(std::ostream&);

    ProjectionFactory(const ProjectionFactory&)            = delete;
    ProjectionFactory(ProjectionFactory&&)                 = delete;
    ProjectionFactory& operator=(const ProjectionFactory&) = delete;
    ProjectionFactory& operator=(ProjectionFactory&&)      = delete;

    virtual Projection* make(const Configuration&) = 0;

protected:
    explicit ProjectionFactory(const Projection::Type&);
    virtual ~ProjectionFactory();

private:
    const Projection::Type key_;
};


template <class T>
class ProjectionBuilder final : public ProjectionFactory {
    Projection* make(const Configuration& config) override { return new T(config); }

public:
    explicit ProjectionBuilder(const Projection::Type& key) :
        ProjectionFactory(key) {}
};


}  // namespace eckit::geometry
