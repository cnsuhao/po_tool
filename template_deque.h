#pragma once
#include "index_calculate.h"
namespace PO{
	namespace Tool {
		//deque_
		template<typename ...AT> struct deque_ {};
		template<typename T> struct is_deque_ :std::false_type {};
		template<typename ...AT> struct is_deque_<deque_<AT...>> :std::true_type {};

		//operator link
		template<typename T,typename ...AT> struct deque_link_ {
			static_assert(is_deque_<T>::value, " deque_link_ only receive deque_ ");
			typedef T type;
		};
		template<typename T, typename K, typename ...AT> struct deque_link_<T,K,AT...> {
			static_assert(is_deque_<T>::value && is_deque_<K>::value, " deque_link_ only receive deque_ ");
		};
		template<typename ...T, typename ...K, typename ...AT> struct deque_link_<deque_<T...>, deque_<K...>, AT...> {
			typedef typename deque_link_< deque_<T..., K...>, AT...  >::type type;
		};

		//operator size
		template<typename T> struct deque_size_ {
			static_assert(is_deque_<T>::value, " deque_size_ only receive deque_ ");
		};
		template<typename ...AT> struct deque_size_<deque_<AT...>> :std::integral_constant<size_t, sizeof...(AT)> {};

		//operator at
		namespace Assistant {
			template<size_t i, typename T, typename ...AT> struct deque_at_execute_ {
				typedef typename deque_at_execute_<i - 1, T, AT...>::type type;
			};
			template< typename T, typename ...AT> struct deque_at_execute_<0, T, AT...> {
				typedef T type;
			};
		}
		template<typename T,size_t i> struct deque_at_ {
			static_assert(is_deque_<T>::value, " deque_at_ only receive deque_ ");
		};
		template<typename ...T, size_t i> struct deque_at_<deque_<T...>,i> {
			static_assert(sizeof...(T) < i, " overflow ");
			typedef typename Assistant::deque_at_execute_<i, T...>::type type;
		};

		//operator push front
		template<typename T,typename date> struct deque_push_front_ {
			static_assert(is_deque_<T>::value, " deque_push_front_ only receive deque_ ");
		};
		template<typename ...AT, typename data > struct deque_push_front_<deque_<AT...>,data> {
			typedef deque_<data,AT...> type;
		};

		//operator push back
		template<typename T, typename date> struct deque_push_back_ {
			static_assert(is_deque_<T>::value, " deque_push_back_ only receive deque_ ");
		};
		template<typename ...AT, typename data > struct deque_push_back_<deque_<AT...>, data> {
			typedef deque_<AT...,data> type;
		};

		//operator pop front
		template<typename T> struct deque_pop_front_ {
			static_assert(is_deque_<T>::value, " deque_pop_front_ only receive deque_ ");
			static_assert(deque_size_<T>::value > 0, " deque_ is empty ");
		};
		template<typename T, typename ...AT> struct deque_pop_front_<deque_<T,AT...>> {
			typedef deque_<AT...> type;
		};

		//operator remove with
		namespace Assistant {
			template<size_t i, typename T, typename ...AT > struct deque_remove_with_execute {};
			template< typename K, typename ...AT > struct deque_remove_with_execute<0, K, AT...> {
				typedef deque_<AT...> type;
			};
			template<size_t i, typename K, typename T, typename ...AT >
			struct deque_remove_with_execute<i, K, T, AT...> {
				typedef typename deque_remove_with_execute<i - 1, K,  AT..., T>::type type;
			};
			template<size_t i, typename K, typename ...AT >
			struct deque_remove_with_execute<i, K,  K, AT...> {
				typedef typename deque_remove_with_execute<i - 1, K,  AT...>::type type;
			};
		}
		template<typename T,typename index> struct deque_remove_with_ {
			static_assert(is_deque_<T>::value, " deque_remove_with_ only receive deque_ ");
		};
		template<typename ...T,typename index> struct deque_remove_with_<deque_<T...>,index> {
			typedef typename Assistant::deque_remove_with_execute<sizeof...(T), index, T...>::type type;
		};

		template<typename T, template<typename...> class sta, typename ...at> class deque_remove_if_ {
			static_assert(is_deque_<T>::value, " deque_remove_with_ only receive deque_ ");
		};
		template<typename ...T, template<typename...> class sta, typename ...at> class deque_remove_if_<deque_<T...>,sta,at...> {
			struct mark {};
		public:
			typedef typename Assistant::deque_remove_with_execute< sizeof...(T),  mark, typename std::conditional< sta<T,at...>::value,mark,T>::type... >::type type;
		};

		//operator reverse

		//operator pick
		template<typename T, typename index > struct deque_pick_ {
			static_assert(is_deque_<T>::value, " deque_pick_ only receive deque_ ");
			static_assert(is_index_array_<index>::value, " deque_pick_ only receive index_array_ ");
		};
		template<typename T, size_t ...i> struct deque_pick_<T,index_array_<i...>> {
			static_assert(is_deque_<T>::value, " deque_pick_ only receive deque_ ");
			typedef deque_<  typename deque_at_<T, i>::type... > type;
		};

		//operator replace at
		template<typename T, typename data,size_t i> struct deque_replace_at_ {
			static_assert(is_deque_<T>::value, " deque_replace_at_ only receive deque_ ");
			static_assert( deque_size_<T>::value > i , " deque_replace_at_ only receive deque_ ");
			typedef typename deque_link_<
				typename deque_pick_< typename index_array_range_<0, i>::type, T >::type,
				deque_<data>,
				typename deque_pick_< typename index_array_range_<i + 1, deque_size_<T>::value>::type, T >::type
			>::type type;
		};

		//operator replace front
		template<typename T, typename data> struct deque_replace_front_ {
			static_assert(is_deque_<T>::value, " deque_replace_front_ only receive deque_ ");
			static_assert(deque_size_<T>::value > 0, " deque_replace_at_ only receive deque_ ");
		};
		template<typename K,typename ...T, typename data> struct deque_replace_front_<deque_<K,T...>,data> {
			typedef deque_<data, T...> type;
		};

		//operator replace with 
		template<typename T, typename index, typename data > struct deque_replace_of_ {
			static_assert(is_deque_<T>::value, " deque_replace_of_ only receive deque_ ");
		};
		template<typename ...T, typename index, typename data > struct deque_replace_of_<deque_<T...>,index,data > {
			typedef deque_< typename std::conditional<std::is_same<T, index>::value, data, T>::type... > type;
		};

		//operator replace if
		template<typename T, template<typename ...> class state ,typename ...at> struct deque_replace_if_ {
			static_assert(is_deque_<T>::value, " deque_replace_if_ only receive deque_ ");
		};
		template<typename ...T, template<typename ...> class state ,typename ...at> struct deque_replace_if_<deque_<T...>,state,at...> {
			typedef deque_< typename std::conditional<state<T>::value, typename state<T,at...>::type, T>::type... > type;
		};

		//operator locate first
		namespace Assistant {
			template<size_t i,typename index,typename ...AT> 
			struct deque_location_first_execute_ {
				typedef index_<i> type;
			};
			template<size_t i, typename index,typename T, typename ...AT>
			struct deque_location_first_execute_<i,index,T,AT...> {
				typedef typename deque_location_first_execute_<i + 1, index, AT...>::type type;
			};
			template<size_t i, typename index, typename ...AT>
			struct deque_location_first_execute_<i, index,index, AT...> {
				typedef index_<i> type;
			};
		}
		template<typename T, typename index > struct deque_location_first_ {
			static_assert(is_deque_<T>::value, " deque_locate_at_ only receive deque_ ");
		};
		template<typename ...T, typename index > 
		struct deque_location_first_<deque_<T...>, index> {
			typedef typename Assistant::deque_location_first_execute_<0, index, T...>::type type;
		};

		//operator locate all
		namespace Assistant {
			template<size_t i, typename index, typename target,typename ...AT>
			struct deque_location_all_execute_ {
				static_assert(is_index_array_<index>::value, " deque_location_all_execute_ only receive index_array_ ");
				typedef index type;
			};
			template<size_t i, size_t ...it, typename target,typename T, typename ...AT>
			struct deque_location_all_execute_<i, index_array_<it...>, target,T, AT...> {
				typedef typename deque_location_all_execute_<i + 1, index_array_<it...>, target, AT...>::type type;
			};
			template<size_t i, size_t ...it, typename target, typename ...AT>
			struct deque_location_all_execute_<i, index_array_<it...>, target, target, AT...> {
				typedef typename deque_location_all_execute_<i + 1, index_array_<it...,i>, target, AT...>::type type;
			};
		}
		template <typename T, typename index > struct deque_locate_all_ {
			static_assert(is_deque_<T>::value, " deque_locate_all_ only receive deque_ ");
		};
		template <typename ...T, typename index > struct deque_locate_all_<deque_<T...>,index> {
			typedef typename Assistant::deque_location_all_execute_<0, index_array_<>, index, T...>::type type;
		};

		//operator find first
		template<typename T, template <typename ... > class state ,typename ...at> class deque_find_first_ {
			static_assert(is_deque_<T>::value, " deque_find_first_ only receive deque_ ");
		};
		template<typename ...T, template <typename ... > class state ,typename ...at > class deque_find_first_<deque_<T...>,state,at...> {
			struct mark {};
		public:
			typedef typename Assistant::deque_location_first_execute_<0, mark,
				typename std::conditional<state<T,at...>::value, mark,T>::type...
			>::type type;
		};

		//operator find all
		template<typename T, template <typename ... > class state,typename ...at > class deque_find_all_ {
			static_assert(is_deque_<T>::value, " deque_find_all_ only receive deque_ ");
		};
		template<typename ...T, template <typename ... > class state ,typename ...at> class deque_find_all_<deque_<T...>, state,at...> {
			struct mark {};
		public:
			typedef typename Assistant::deque_location_all_execute_<0, mark,
				typename std::conditional<state<T,at...>::value, mark, T>::type...
			>::type type;
		};

		//operator erase at

		//operator pop back

		template<typename T> struct trans_to_deque_ {};
		template<typename ...T, template<typename...> class tank> struct trans_to_deque_<tank<T...>> { typedef deque_<T...> type; };

	}
}