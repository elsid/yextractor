#include <type_traits>
#include <tuple>

#include <yamail/yextractor.hpp>

namespace {

using namespace yamail::yextractor;
using namespace yamail::yextractor::detail;

DEFINE_PARAMETER(A, std::string, "a")
DEFINE_PARAMETER(B, std::string, "b")
DEFINE_PARAMETER(C, std::string, "c")
DEFINE_PARAMETER(D, std::string, "d")

static_assert(
    std::is_same<
        std::tuple<>,
        decltype(flat(Every<>()))
    >::value, "flat(Every<>()) doesn't returns std::tuple<>");


static_assert(
    std::is_same<
        std::tuple<A>,
        decltype(flat(Required<A>()))
    >::value, "flat(Required<A>()) doesn't returns std::tuple<A>");

static_assert(
    std::is_same<
        std::tuple<A, B>,
        decltype(flat(Every<Required<A>, Required<B>>()))
    >::value, "flat(Every<Required<A>, Required<B>>()) "
              "doesn't returns std::tuple<A, B>");


static_assert(
    std::is_same<
        std::tuple<A, B>,
        decltype(flat(Every<A, B>()))
    >::value, "flat(Every<A, B>()) doesn't returns std::tuple<A, B>");

static_assert(
    std::is_same<
        std::tuple<A, B>,
        decltype(flat(Any<A, B>()))
    >::value, "flat(Any<A, B>()) doesn't returns std::tuple<A, B>");


static_assert(
    std::is_same<
        std::tuple<A, B>,
        decltype(flat(Every<A, B>()))
    >::value, "flat(Every<A, B>()) doesn't returns std::tuple<A, B>");

static_assert(
    std::is_same<
        std::tuple<A, B>,
        decltype(flat(Any<A, B>()))
    >::value, "flat(Any<A, B>()) doesn't returns std::tuple<A, B>");


static_assert(
    std::is_same<
        std::tuple<A, B, C, D>,
        decltype(flat(Every<Any<A, B>, Any<C, D>>()))
    >::value, "flat(Every<Any<A, B>, Any<C, D>>()) "
              "doesn't returns std::tuple<A, B, C, D>");

static_assert(
    std::is_same<
        std::tuple<A, B, C, D>,
        decltype(flat(Every<Any<A, B>, Every<C, D>>()))
    >::value, "flat(Every<Any<A, B>, Every<C, D>>()) "
              "doesn't returns std::tuple<A, B, C, D>");

static_assert(
    std::is_same<
        std::tuple<A, B, C, D>,
        decltype(flat(Every<Every<A, B>, Any<C, D>>()))
    >::value, "flat(Every<EveryA, B>, Any<C, D>>()) "
              "doesn't returns std::tuple<A, B, C, D>");

static_assert(
    std::is_same<
        std::tuple<A, B, C, D>,
        decltype(flat(Every<Every<A, B>, Every<C, D>>()))
    >::value, "flat(Every<Every<A, B>, Every<C, D>>()) "
              "doesn't returns std::tuple<A, B, C, D>");


static_assert(
    std::is_same<
        std::tuple<A, B, C>,
        decltype(flat(Every<Any<A, Any<B, C>>>()))
    >::value, "flat(Every<Any<A, Any<B, C>>>()) "
              "doesn't returns std::tuple<A, B, C>");

static_assert(
    std::is_same<
        std::tuple<A, B, C>,
        decltype(flat(Every<Any<A, Every<B, C>>>()))
    >::value, "flat(Every<Any<A, Every<B, C>>>()) "
              "doesn't returns std::tuple<A, B, C>");

static_assert(
    std::is_same<
        std::tuple<A, B, C>,
        decltype(flat(Every<Every<A, Any<B, C>>>()))
    >::value, "flat(Every<Every<A, Any<B, C>>>()) "
              "doesn't returns std::tuple<A, B, C>");

static_assert(
    std::is_same<
        std::tuple<A, B, C>,
        decltype(flat(Every<Every<A, Every<B, C>>>()))
    >::value, "flat(Every<Every<A, Every<B, C>>>()) "
              "doesn't returns std::tuple<A, B, C>");

} // namespace
