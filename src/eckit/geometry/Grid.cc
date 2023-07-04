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


#include "eckit/geometry/Grid.h"

#include <map>
#include <memory>
#include <sstream>

#include "eckit/exception/Exceptions.h"
#include "eckit/geometry/Domain.h"
#include "eckit/geometry/Iterator.h"
#include "eckit/geometry/detail/UnstructuredGrid.h"
#include "eckit/log/Log.h"
#include "eckit/thread/AutoLock.h"
#include "eckit/thread/Mutex.h"


namespace eckit::geometry {


Grid::Grid(const Configuration& config) :
    bbox_(config) {}


Grid::Grid(const BoundingBox& bbox) :
    bbox_(bbox) {}


const BoundingBox& Grid::boundingBox() const {
    return bbox_;
}


static pthread_once_t __once;
static Mutex* __mutex                         = nullptr;
static std::map<std::string, GridFactory*>* m = nullptr;
static void __init() {
    __mutex = new Mutex;
    m       = new std::map<std::string, GridFactory*>();
}


GridFactory::GridFactory(const std::string& name) :
    name_(name) {
    pthread_once(&__once, __init);
    AutoLock<Mutex> lock(*__mutex);

    if (m->find(name) != m->end()) {
        throw SeriousBug("GridFactory: duplicate '" + name + "'");
    }

    (*m)[name] = this;
}


GridFactory::~GridFactory() {
    AutoLock<Mutex> lock(*__mutex);

    m->erase(name_);
}


const Grid* GridFactory::build(const Configuration& config) {
    pthread_once(&__once, __init);
    AutoLock<Mutex> lock(*__mutex);

    std::string name;
    if (!config.get("gridType", name)) {
        throw SeriousBug("GridFactory: cannot get 'gridType'");
    }

    if (auto j = m->find(name); j != m->end()) {
        return j->second->make(config);
    }

    list(Log::error() << "GridFactory: unknown '" << name << "', choices are: ");
    throw SeriousBug("GridFactory: unknown '" + name + "'");
}


const Grid* GridFactory::build_from_uid(const Grid::UID& uid) {
    NOTIMP;
}


const Grid* GridFactory::build_from_name(const Grid::Name&, const Grid::Area& area, const Grid::Rotation& rotation) {
    NOTIMP;
}


const Grid* GridFactory::build_from_name(const Grid::Name&, const Grid::Rotation& rotation) {
    NOTIMP;
}


const Grid* GridFactory::build_from_increments(const Increments&, const Grid::Area& area, const Grid::Rotation& rotation) {
    NOTIMP;
}


const Grid* GridFactory::build_from_increments(const Increments&, const Grid::Rotation& rotation) {
    NOTIMP;
}


void GridFactory::list(std::ostream& out) {
    pthread_once(&__once, __init);
    AutoLock<Mutex> lock(*__mutex);

    const char* sep = "";
    for (const auto& j : *m) {
        out << sep << j.first;
        sep = ", ";
    }
}


}  // namespace eckit::geometry
