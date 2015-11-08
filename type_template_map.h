#pragma once
#include "type_tool.h"
namespace PO {
	namespace Tool {
		namespace TTC {
			template<typename index, typename data, typename next> struct tree_node_;
			template<typename T> struct is_tree_node_:std::false_type{};
			template<typename index, typename data, typename next> struct is_tree_node_<tree_node_<index, data, next>> :is_tree_node_<next>{};
			template<typename index, typename data> struct is_tree_node_<tree_node_<index, data, void>> :std::true_type{};

			template<typename index, typename data, typename next> struct tree_node_ {

				static_assert(is_tree_node_<next>::value || std::is_same<next, void>::value, "tree_node_ only conect to tree_node_");

				template<typename i_index,typename i_data> struct insert {
					typedef tree_node_< index,
						typename std::conditional< std::is_same<i_index, index>::value, i_data, data >::type,
						typename std::conditional< std::is_same<i_index, index>::value, itself<next>, typename instant_type< next::template insert,i_index,i_data > >::type::type
					> type;
				};
				template<template<typename...> class tank, typename ...AT> struct extract {
					typedef typename next::template extract<tank, AT..., data>::type type;
				};

				template<template<typename...> class tank, typename ...AT> struct extract_index {
					typedef typename next::template extract_index<tank, AT..., index>::type type;
				};

				template<typename e_index> struct erase {
					typedef typename std::conditional < std::is_same<e_index, index>::value,
						next,
						tree_node_< index,data, 
							typename std::conditional<
								std::is_same<e_index, index>::value,
								itself<void>,
								instant_type< next::template erase,e_index>
							>::type::type
						>
					>::type type;
				};

				template<typename h_index> struct have :std::conditional< 
					std::is_same<h_index,index>::value,
					itself<std::true_type>,
					instant_type<next::have,h_index>
				>::type::type{};

				template<typename g_index> struct get {
					typedef typename std::conditional< std::is_same<g_index, index>::value,
						itself<data>,
						instant_type<next::template get,g_index>
					>::type::type type;
				};
			};

			template<typename index, typename data> struct tree_node_<index,data,void> {
				template<typename i_index, typename i_data> struct insert {
					typedef typename std::conditional<std::is_same<i_index, index>::value,
						tree_node_<index, i_data, void>,
						tree_node_<index, data, tree_node_<i_index, i_data, void>>
					>::type type;
				};

				template<template<typename...> class tank, typename ...AT> struct extract {
					typedef tank<AT...,data> type;
				};

				template<template<typename...> class tank, typename ...AT> struct extract_index {
					typedef tank<AT..., index> type;
				};

				template<typename e_index> struct erase {
					typedef typename std::conditional < std::is_same<e_index, index>::value,
						void,
						tree_node_
					>::type type;
				};
				template<typename h_index> struct have :std::conditional< std::is_same<h_index, index>::value, std::true_type, std::false_type >::type {};

				template<typename g_index> struct get {
					static_assert(std::is_same<g_index, index>::value, "do not have data with this key");
					typedef data type;
				};
			};



			template<typename T = void > struct map_ {
				static_assert(is_tree_node_<T>::value,"map_ only able for tree_node_");
				template<typename K,typename L> struct insert {
					typedef map_<typename T::template insert<K,L>::type> type;
				};
				template<typename K> struct erase {
					typedef map_<typename T::template erase<K>::type> type;
				};
				template<typename K> struct have :T::template have<K> {};
				template<typename K> struct get {
					typedef typename T::template get<K>::type type;
				};
				template<template<typename...> class tank> struct extract {
					typedef typename T::template extract<tank>::type type;
				};
				template<template<typename...> class tank> struct extract_index {
					typedef typename T::template extract_index<tank>::type type;
				};

				template<typename K, typename L> using insert_t = typename insert<K, L>::type;
				template<typename K> using erase_t = typename erase<K>::type;
				template<template<typename...> class tank> using extract_index_t = typename extract_index<tank>::type;
				template<typename K> using get_t = typename get<K>::type;
				template<template<typename...> class tank> using extract_t = typename extract<tank>::type;
			};

			template<> struct map_<void> {
				template<typename K, typename L> struct insert {
					typedef map_<tree_node_<K,L,void>> type;
				};
				template<typename K> struct erase {
					static_assert( 0 == 0,"not able to erase an empty map_");
					typedef map_<> type;
				};
				template<typename K> struct have :std::false_type {};
				template<typename K> struct get {
					static_assert(0 == 0, "not able to get from empty map_");
					typedef void type;
				};
				template<template<typename...> class tank> struct extract {
					typedef tank<> type;
				};
				template<template<typename...> class tank> struct extract_index {
					typedef tank<> type;
				};
				template<typename K, typename L> using insert_t = typename insert<K, L>::type;
				template<typename K> using erase_t = typename erase<K>::type;
				template<typename K> using get_t = typename get<K>::type;
				template<template<typename...> class tank> using extract_t = typename extract<tank>::type;
				template<template<typename...> class tank> using extract_index_t = typename extract_index<tank>::type;
			};

			template <typename T> struct is_map_ :std::false_type {};
			template <typename T> struct is_map_<map_<T>> :std::true_type {};

			namespace Assistant {
				template<typename T, size_t k, typename ...AT> struct make_order_map_assistant {
					typedef T type;
				};
				template<typename T, size_t k, typename L, typename ...AT> struct make_order_map_assistant<T,k,L,AT...> {
					typedef typename make_order_map_assistant< typename T::template insert<std::integral_constant<size_t,k>,L >::type ,k+1,AT...>::type type;
				};
			}

			template<typename ...T> struct make_order_map {
				typedef typename Assistant::make_order_map_assistant<map_<>, 0, T...>::type type;
			};
			template<typename ...T> using make_order_map_t = typename make_order_map<T...>::type;

			template<typename T> struct make_order_map_from_type_template {
				static_assert(is_type_template<T>::value, "make_order_map_from_type_template only recive type template");
			};
			template<typename ...T,template<typename ...> class tank> struct make_order_map_from_type_template<tank<T...>> {
				typedef typename Assistant::make_order_map_assistant<map_<>, 0, T...>::type type;
			};
			template<typename T> using make_order_map_from_type_template_t = typename make_order_map_from_type_template<T>::type;

			

		}
	}
}
