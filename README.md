# yextractor

[![Build Status](https://travis-ci.org/elsid/yextractor.svg?branch=master)](https://travis-ci.org/elsid/yextractor)
[![Coverage Status](https://coveralls.io/repos/github/elsid/yextractor/badge.svg?branch=master)](https://coveralls.io/github/elsid/yextractor?branch=master)

Header only library provides value extraction from key-value container
(like std::map) with verification by extraction expression.

## Build

CMake based. Required only for tests and examples:
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j $(nproc)
ctest -V -j $(nproc)
examples/example
```

## Install

Use ```add_subdirectory``` in your CMake project or just copy ```include/``` directory.

## Usage

To use library perform following steps:
* [Adopt container type](#values-source)
* [Define values types](#extracting-values)
* [Define expression type](#extraction-expressions)
* [Create and use extractor](#values-extraction)
* [Handle errors](#vefification-errors)

Or just look at complete [example](#examples).

### Values source

Source collection must provides interface:
```c++
Iterator find(std::string) const;
Iterator end() const;
```

Iterator type must provides interface:
```c++
std::pair<std::string, std::string> operator ->() const;
```

### Extracting values

Client must define special value type called ```Parameter``` using one of macro:
* ```DEFINE_PARAMETER(type_name, value_type, "key")``` -- defines parameter with default parser type;
* ```DEFINE_PARAMETER_WITH_PARSER(type_name, value_type, "key", parser_type)``` -- defines parameter with custom parser type.

Details see [here](include/yamail/yextractor/parameter.hpp).

Example:
```c++
DEFINE_PARAMETER(SomeParameter, std::string, "some")
```

#### Parser type

Parsers for all arithmetic types and dummy parser to copy ```std::string``` defined in library.
Custom parser type must be defined for all extra types.
Parser converts value from ```std::string``` into target type.
Must provides interface:
```c++
yamail::yextractor::Errors operator ()(value_type& dst, const std::string& src) const;
```

```value_type``` -- target value type.

Object of parser type construct for each value by default constructor.

Example:
```c++
using Errors = yamail::yextractor::Errors;

struct ParamParser {
    Errors operator ()(std::string& dst, const std::string& src) const {
        if (!boost::starts_with(src, "prefix")) {
            return Errors("value doesn't not starts with 'prefix'");
        }
        dst = src;
        return Errors();
    }
};

DEFINE_PARAMETER_WITH_PARSER(AnotherParameter, std::string, "another", ParamParser)
```

### Extraction expressions

Allow to verify values combination extracted from container.

#### Expressions types

Following types ```Required```, ```Optional```, ```Any```, ```First```, ```Every``` are expressions.
Arguments of all types must be expressions or type ```Parameter```, details see [below](#expressions-combinations).
Verification of user type succeed if value found and parser returns no errors or if value not found and top type allows value absence.

##### Required

```c++
template <class Argument> Required
```

Defines required parameter.
Verification succeed if argument passed verification. Value absence is disallowed.

Example:
```c++
using GetSomeRequiredParameter = Required<SomeParameter>;
```

#### Optional

```c++
template <class Argument> Optional
```

Defines optional parameter.
Verification succeed if argument succeed verification. Value absence is allowed.

Example:
```c++
using GetSomeOptionalParameter = Optional<SomeParameter>;
```

#### Without

```c++
template <class Argument> Without
```

Defines forbidden parameter.
Verification succeed if argument is not present.

Example:
```c++
using GetSomeWithoutParameter = Without<SomeParameter>;
```

#### Any

```c++
template <class ... Arguments> Any
```

Defines sequence of expressions.
Verification succeed if one of arguments succeed verification.
Value absence is disallowed.
Fills all values successfully passed verification.

Example:
```c++
using GetAnyParameter = Any<SomeParameter, AnotherParameter>;
```

#### First

```c++
template <class ... Arguments> First
```

Defines sequence of expressions.
Verification succeed if one of arguments succeed verification.
Value absence is disallowed.
Fills first value successfully passed verification.

Example:
```c++
using GetFirstParameter = First<SomeParameter, AnotherParameter>;
```

#### Every

```c++
template <class ... Arguments> Every
```

Defines sequence of expressions.
Verification succeed if all of arguments succeed verification.
Value absence is disallowed.
Fills all values successfully passed verification.

Example:
```c++
using GetEveryParameter = Every<SomeParameter, AnotherParameter>;
```

#### Expressions combinations

```Required``` and ```Optional``` arguments must be only type of ```Parameter```
```Any```, ```First```, and ```Every``` arguments must be any of types:
```Parameter```, ```Required```, ```Optional```, ```Any```, ```First```, or ```Every```

Example:
```c++
using Example1 = Any<Every<A, B>, Every<C, D>>;
using Example2 = Every<Any<A, B>, Any<C, D>>;
using Example3 = Every<A, B, Optional<C>>;
```

### Values extraction

Use object of type ```Extractor```.
To get all values from expression use:
```c++
template <class T, class Source>
std::tuple</* unspecified */> get(const Source& source);
```

So parameters has duplicates in expression so in the result.
Values wrapped by ```detail::Value``` (like ```boost::optional```).
Result type defines by ```Expression<T>::Type```.

Initialize ```Parameter``` by copied value from ```std::tuple``` using constructor:
```c++
template <class ... Values>
Parameter(const std::tuple<Values ...>& values);
```

or by moved value from ```std::tuple``:
```c++
template <class ... Values>
Parameter(std::tuple<Values ...>& values);
```

Following method checks is value initialized:
```c++
bool Parameter::initialized() const;
```

Get constant reference to value by using:
```c++
const Parameter::Type& Parameter::get() const;
```

To move value from parameter use then ```Parameter``` becomes uninitialized:
```c++
Parameter::Type&& Parameter::take();
```

### Verification errors

If verification fails all errors available by method:
```c++
const Errors& Extractor::errors() const;
```

Result contains sequence of errors from branch leads to verification failure ordered from last to first error.

### Examples

See [here](examples/main.cpp).
