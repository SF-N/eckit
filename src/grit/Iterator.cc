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


#include "grit/Iterator.h"

#include "grit/Figure.h"
#include "grit/Projection.h"
#include "grit/Scanner.h"
#include "grit/exception.h"


namespace grit {


Iterator::Iterator(Scanner* scanner, Figure* figure, Projection* projection) :
    scanner_(scanner), figure_(figure), projection_(projection) {
    ASSERT(scanner_);
    ASSERT(figure_);
    ASSERT(projection_);
}


bool Iterator::operator++() {
    return ++(*scanner_);
}


size_t Iterator::size() const {
    ASSERT(scanner_);
    return scanner_->size();
}


}  // namespace grit
