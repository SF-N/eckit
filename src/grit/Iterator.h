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


#include <memory>


namespace grit {
struct Figure;
struct Scanner;
struct Transformation;
}  // namespace grit


namespace grit {


class Iterator {
public:
    // -- Types
    // None

    // -- Exceptions
    // None

    // -- Constructors

    Iterator(Scanner* scanner, Figure* figure, Transformation* transformation);

    Iterator(const Iterator&) = delete;
    Iterator(Iterator&&)      = delete;

    Iterator& operator=(const Iterator&) = delete;
    Iterator& operator=(Iterator&&)      = delete;

    // -- Destructor

    virtual ~Iterator() = delete;

    // -- Convertors
    // None

    // -- Operators

    bool operator++();

    // -- Methods
    // None

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

    std::unique_ptr<Scanner> scanner_;
    std::unique_ptr<Figure> figure_;
    std::unique_ptr<Transformation> transformation_;

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


}  // namespace grit
