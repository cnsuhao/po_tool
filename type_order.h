#pragma once
#include "type_list.h"
namespace PO {
	namespace Tool {
		template<typename T, size_t d2 =0, size_t d1 = 0> struct type_order {};
		
		template<typename T> struct is_type_order { static constexpr bool value = false; };
		template<typename T, size_t i, size_t k> struct is_type_order<type_order<T, i, k>> { static constexpr bool value = true; };

		namespace Assert {
			template<typename T> struct not_type_order {
				static_assert(is_type_order<T>::value,"it is not a element order");
			};
		}

		template<size_t i=0> class mapping_to_order {
			template<typename T> struct inside_mapping {
				typedef type_order<typename type_list_type<T, 0>::type, type_list_size<T>::value - 1, i> type;
			};
		public:
			template<typename T>  using pass =typename pipleline<reverse_element,ascension_follow,reverse_element,separate_pipleline<replace<inside_mapping>>>::template pass<T>;
		};

		
		template<typename T, typename P> struct order_change_type_to :public Assert::not_type_order<T> { };
		template<typename T, size_t i, size_t k, typename P> 
		struct order_change_type_to<type_order<T, i, k>,P> {
			typedef type_order<P, i, k> type;
		};
		template<typename T> struct type_order_type :public Assert::not_type_order<T> {};
		template<typename T, size_t i, size_t k> struct type_order_type<type_order<T, i, k>> { typedef T type; };
		/*template<typename T> struct type_order_position_1 :public Assert::not_type_order<T> {};
		template<typename T, size_t i, size_t k> struct type_order_position_1<type_order<T, i, k>> { static constexpr size_t value = i; };
		template<typename T> struct type_order_position_2 :public Assert::not_type_order<T> {};
		template<typename T, size_t i, size_t k> struct type_order_position_2<type_order<T, i, k>> { static constexpr size_t value = k; };
		/*template<typename I, template<typename...> pa > struct order_unpack_with : public Assert::not_type_order<I> {};
		template<typename I , size_t i,size_t p, template<typename...> pa > 
		struct order_unpack_with<type_order<I,i,p>,pa> {
			typedef typename order_pack<typename pa<I>::type,i,p>::type type;
		};*/


		/*template<typename ...T > struct pre_order_unpack_with {
			template<typename T> struct unpack:public Assert::not_type_order<T> {
				typedef typename order_change_type_to<T,typename role< typename type_order_type<T>::type>::type > type;
			};
		};*/



		

		



		/*template<template<typename ...> class role> struct unpack_type_list {
			template<typename T> using method = typename extract_to<T, role>::type;
		};

		template<size_t t = 0> struct ascension_to_type_order_method{
			template<typename T> struct tem_mapping {
				typedef type_order<type_list_type<T,0>, type_list_size<T>::value, t> type;
			};
			template<typename T> using pass_to = typename pass_method<
				reverse_type_list_method,
				ascension_with_follow_method,
				mapping_method<tem_mapping>,
				reverse_type_list_method
			>::template method<T>;
		public:
			template <typename T> using method = pass_to<T>;
		};
		template<typename T, size_t i = 0> using ascension_to_type_order = typename ascension_to_type_order_method<i>::template method<T>::type;*/


	}
}