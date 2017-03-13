#pragma once

#include <boost/hana/ext/boost/mpl/vector.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/not_equal.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/second.hpp>
#include <boost/hana/size.hpp>
#include <boost/hana/take_while.hpp>
#include <boost/hana/transform.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/zip.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/variant.hpp>

#include <algorithm>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace opossum {

namespace hana = boost::hana;

using ChunkID = uint32_t;
using ChunkOffset = uint32_t;
struct RowID {
  ChunkID chunk_id;
  ChunkOffset chunk_offset;

  bool operator<(const RowID &rhs) const {
    return std::tie(chunk_id, chunk_offset) < std::tie(rhs.chunk_id, rhs.chunk_offset);
  }
};
constexpr ChunkOffset INVALID_CHUNK_OFFSET = std::numeric_limits<ChunkOffset>::max();

using ColumnID = uint16_t;
using ValueID = uint32_t;  // Cannot be larger than ChunkOffset
using WorkerID = uint32_t;
using NodeID = uint32_t;
using TaskID = uint32_t;
using CpuID = uint32_t;

using CommitID = uint32_t;
using TransactionID = uint32_t;

using StringLength = uint16_t;     // The length of column value strings must fit in this type.
using ColumnNameLength = uint8_t;  // The length of column names must fit in this type.
using AttributeVectorWidth = uint8_t;

using PosList = std::vector<RowID>;

class ColumnName {
 public:
  explicit ColumnName(const std::string &name) : _name(name) {}

  operator std::string() const { return _name; }

 protected:
  const std::string _name;
};

constexpr NodeID INVALID_NODE_ID = std::numeric_limits<NodeID>::max();
constexpr TaskID INVALID_TASK_ID = std::numeric_limits<TaskID>::max();
constexpr CpuID INVALID_CPU_ID = std::numeric_limits<CpuID>::max();
constexpr WorkerID INVALID_WORKER_ID = std::numeric_limits<WorkerID>::max();

constexpr NodeID CURRENT_NODE_ID = std::numeric_limits<NodeID>::max() - 1;

// The Scheduler currently supports just these 2 priorities, subject to change.
enum class SchedulePriority {
  Normal = 1,  // Schedule task at the end of the queue
  High = 0     // Schedule task at the beginning of the queue
};

/**
 * Only the following three lines are needed wherever AllTypeVariant is used.
 * This could be one header file.
 * @{
 */

// This holds all possible data types.
static constexpr auto types = hana::make_tuple(hana::type_c<int32_t>, hana::type_c<int64_t>, hana::type_c<float>,
                                               hana::type_c<double>, hana::type_c<std::string>);

// Convert tuple to mpl vector
using TypesAsMplVector = decltype(hana::to<hana::ext::boost::mpl::vector_tag>(types));

// Create boost::variant from mpl vector
using AllTypeVariant = typename boost::make_variant_over<TypesAsMplVector>::type;

/** @} */

/**
 * AllParameterVariant holds either an AllTypeVariant or a ColumnName.
 * It should be used to generalize Opossum operator calls.
 */
static auto parameter_types = hana::make_tuple(hana::make_pair("AllTypeVariant", AllTypeVariant(123)),
                                               hana::make_pair("ColumnName", ColumnName("column_name")));

// convert tuple of all types to sequence by first extracting the prototypes only and then applying decltype_
static auto parameter_types_as_hana_sequence =
    hana::transform(hana::transform(parameter_types, hana::second), hana::decltype_);
// convert hana sequence to mpl vector
using ParameterTypesAsMplVector =
    decltype(hana::to<hana::ext::boost::mpl::vector_tag>(parameter_types_as_hana_sequence));
// create boost::variant from mpl vector
using AllParameterVariant = typename boost::make_variant_over<ParameterTypesAsMplVector>::type;

/**
 * This is everything needed for type_cast
 * @{
 */

namespace {

// Returns the index of type T in an Iterable
template <typename Sequence, typename T>
constexpr auto index_of(Sequence const &sequence, T const &element) {
  constexpr auto size = decltype(hana::size(hana::take_while(sequence, hana::not_equal.to(element)))){};
  return decltype(size)::value;
}

// Negates a type trait
template <bool Condition>
struct _neg : public std::true_type {};

template <>
struct _neg<true> : public std::false_type {};

template <typename Condition>
struct neg : public _neg<Condition::value> {};

// Wrapper that makes std::enable_if a bit more readable
template <typename Condition, typename Type = void>
using enable_if = typename std::enable_if<Condition::value, Type>::type;

}  // namespace

// Retrieves the value stored in an AllTypeVariant without conversion
template <typename T>
const T &get(const AllTypeVariant &value) {
  static_assert(hana::contains(types, hana::type_c<T>), "Type not in AllTypeVariant");
  return boost::get<T>(value);
}

// cast methods - from variant to specific type

// Template specialization for everything but integral types
template <typename T>
enable_if<neg<std::is_integral<T>>, T> type_cast(const AllTypeVariant &value) {
  if (value.which() == index_of(types, hana::type_c<T>)) return get<T>(value);

  return boost::lexical_cast<T>(value);
}

// Template specialization for integral types
template <typename T>
enable_if<std::is_integral<T>, T> type_cast(const AllTypeVariant &value) {
  if (value.which() == index_of(types, hana::type_c<T>)) return get<T>(value);

  try {
    return boost::lexical_cast<T>(value);
  } catch (...) {
    return boost::numeric_cast<T>(boost::lexical_cast<double>(value));
  }
}

std::string to_string(const AllTypeVariant &x);

/** @} */

/**
 * This is everything needed for make_*_by_column_type to work.
 * It needs to include the AllVariantType header and could also
 * be moved into a separate header.
 * @{
 */

namespace {

// Functor that converts tuples with size two into pairs
struct to_pair_t {
  template <typename Tuple>
  constexpr decltype(auto) operator()(Tuple &&tuple) const {
    return hana::make_pair(hana::at_c<0>(tuple), hana::at_c<1>(tuple));
  }
};

constexpr to_pair_t to_pair{};

}  // namespace

static constexpr auto type_strings = hana::make_tuple("int", "long", "float", "double", "string");

// “Zips” the types and type_strings tuples creating a tuple of string-type pairs
static constexpr auto column_types = hana::transform(hana::zip(type_strings, types), to_pair);

template <class base, template <typename...> class impl, class... TemplateArgs, typename... ConstructorArgs>
std::unique_ptr<base> make_unique_by_column_type(const std::string &type, ConstructorArgs &&... args) {
  std::unique_ptr<base> ret = nullptr;
  hana::for_each(column_types, [&](auto x) {
    if (std::string(hana::first(x)) == type) {
      // The + before hana::second - which returns a reference - converts its return value
      // into a value so that we can access ::type
      using column_type = typename decltype(+hana::second(x))::type;
      ret = std::make_unique<impl<column_type, TemplateArgs...>>(std::forward<ConstructorArgs>(args)...);
      return;
    }
  });
  if (IS_DEBUG && !ret) throw std::runtime_error("unknown type " + type);
  return ret;
}

/**
 * We need to pass parameter packs explicitly for GCC due to the following bug:
 * http://stackoverflow.com/questions/41769851/gcc-causes-segfault-for-lambda-captured-parameter-pack
 */
template <class base, template <typename...> class impl, class... TemplateArgs, typename... ConstructorArgs>
std::unique_ptr<base> make_unique_by_column_types(const std::string &type1, const std::string &type2,
                                                  ConstructorArgs &&... args) {
  std::unique_ptr<base> ret = nullptr;
  hana::for_each(column_types, [&ret, &type1, &type2, &args...](auto x) {
    if (std::string(hana::first(x)) == type1) {
      hana::for_each(column_types, [&ret, &type1, &type2, &args...](auto y) {
        if (std::string(hana::first(y)) == type2) {
          using column_type1 = typename decltype(+hana::second(x))::type;
          using column_type2 = typename decltype(+hana::second(y))::type;
          ret = std::make_unique<impl<column_type1, column_type2, TemplateArgs...>>(
              std::forward<ConstructorArgs>(args)...);
          return;
        }
      });
      return;
    }
  });
  if (IS_DEBUG && !ret) throw std::runtime_error("unknown type " + type1 + " or " + type2);
  return ret;
}

template <class base, template <typename...> class impl, class... TemplateArgs, class... ConstructorArgs>
std::shared_ptr<base> make_shared_by_column_type(const std::string &type, ConstructorArgs &&... args) {
  return make_unique_by_column_type<base, impl, TemplateArgs...>(type, std::forward<ConstructorArgs>(args)...);
}

/** @} */

}  // namespace opossum
