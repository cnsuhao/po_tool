#pragma once
#include <utility>
#include <type_traits>
namespace PO{
	namespace Tool {
		template <size_t i> using index_ = std::integral_constant<size_t, i>;
		
		template <typename T> struct is_index_ :std::false_type{};
		template <size_t i> struct is_index_<index_<i>> :std::true_type {};

		template <typename T, typename K> struct add_index_ {
			static_assert(is_index_<T>::value && is_index_<K>::value, "add_index_ only recive index_");
			typedef index_<T::value + K::value> type;
		};

		template <typename T, typename K> struct dec_index_ {
			static_assert(is_index_<T>::value && is_index_<K>::value, "dec_index_ only recive index_");
			typedef index_<T::value - K::value> type;
		};

		template <typename T, typename K> struct mod_index_ {
			static_assert(is_index_<T>::value && is_index_<K>::value, "mod_index_ only recive index_");
			typedef index_<T::value % K::value> type;
		};

		template <typename T, typename K> struct mul_index_ {
			static_assert(is_index_<T>::value && is_index_<K>::value, "mul_index_ only recive index_");
			typedef index_<T::value * K::value> type;
		};

		template <typename T, typename K> struct div_index_ {
			static_assert(is_index_<T>::value && is_index_<K>::value, "div_index_ only recive index_");
			typedef index_<T::value / K::value> type;
		};

		template <size_t ...i> using index_array_ = std::integer_sequence<size_t, i...>;

		template <typename T> struct is_index_array_ :std::false_type {};
		template <size_t ...i> struct is_index_array_<index_array_<i...>> :std::true_type {};

		template <size_t i, size_t k ,size_t ...oi> struct index_array_range_ {
			typedef typename index_array_range_ < i + (i<k ? 1 : -1), k, oi..., i>::type type;
		};
		template <size_t i, size_t ...oi> struct index_array_range_<i,i,oi...> {
			typedef index_array_<oi...> type;
		};

	}
}