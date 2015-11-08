#pragma once
#include "magic_macro.h"
#include <utility>
#include <type_traits> 
namespace PO {
	namespace Tool {

		template<typename T> struct pure_type {
			typedef std::remove_const_t<std::remove_cv_t<std::remove_extent_t<std::remove_pointer<std::remove_reference<std::remove_volatile<T>>>>>> type;
		};
		template<typename T> using pure_type_t = typename pure_type<T>::type;

		template<typename T> struct pure_input_type {
			typedef std::remove_const_t<std::remove_reference_t<T>> type;
		};
		template<size_t i> struct pure_input_type<const char(&) [i]> {
			typedef const char* type;
		};
		template<typename T> using pure_input_type_t = typename pure_input_type<T>::type;

		template<typename T, T t> struct const_value { static constexpr T value = t; };

		template<typename T> struct itself { typedef T type; };

		template<bool, template<typename ...> class t1 = itself, template<typename...> class t2 = itself > struct bind_conditional {
			template<typename ...AT> using bind = t1<AT...>;
		};
		template<template<typename ...> class t1, template<typename ...> class t2 > struct bind_conditional<false, t1, t2> {
			template<typename ...AT> using bind = t2<AT...>;
		};

		template<bool, typename T, template<typename> class role> struct mapping_if {
			typedef typename role<T>::type type;
		};

		template< typename T, template<typename> class role> struct mapping_if< false, T, role> {
			typedef typename T type;
		};

		template<typename T> struct get_type {
			typedef typename T::type type;
		};

		template<typename T> struct get_deep_type {
			typedef typename T::type::type type;
		};

		template<template<typename...> class tank, typename ...AT> struct instant_type {
			typedef typename tank<AT...>::type type;
		};

		template<template<typename...> class intruct, template<typename...> class tank, typename ...L> struct depute_instant {
			typedef tank< typename intruct<L>::type ... >type;
		};

		template<template<typename...> class intruct, template<typename...> class tank, typename ...L> struct depute_instant_type {
			typedef typename tank< typename intruct<L>::type ... >::type type;
		};

		template<template<typename...> class tank, typename ...AT> struct instant {
			typedef typename tank<AT...> type;
		};

		template<typename T, template<typename...> class K>  struct extract {
		};

		template<typename ...T,template<typename ...> class tank, template<typename...> class K>  struct extract<tank<T...>,K> {
			typedef K<T...> type;
		};

		/*template<template<typename...> class tank, typename ...AT> struct extract {
			typedef typename tank<AT...> type;
		};*/

		template<template<typename...> class intruct, template<typename...> class tank, typename ...AT> struct depute_extract {
			typedef typename tank<typename intruct<AT>::type ...> type;
		};

		template< bool def , bool ...other > struct value_and : public std::conditional<def,value_and<other...>,std::false_type>::type{};
		template< bool def> struct value_and<def> : public std::conditional<def, std::true_type, std::false_type>::type{};
		
		template< bool def, bool ...other > struct value_or : public std::conditional<def, std::true_type, value_or<other...>>::type {};
		template< bool def> struct value_or<def> : public std::conditional<def, std::true_type, std::false_type>::type{};

		template<typename T, typename ...AT> struct is_one_of:public std::false_type {};

		template<typename T, typename I,typename ...AT> 
		struct is_one_of<T,I,AT...>:public std::conditional< std::is_same<T, I>::value,std::true_type, is_one_of<T, AT...> >::type {};

		template<typename ...AT> struct is_repeat : public std::false_type {};
		template<typename T, typename ...AT> 
		struct is_repeat <T, AT...>:public std::conditional< is_one_of<T,AT...>::value , std::true_type, is_repeat < AT...>  >::type{};

		template<typename...> struct template_void {};
		template<typename T, typename K, typename U> struct type_conditional {
			typedef typename std::conditional<T::value, K, U>::type type;
		};

		template<typename T, typename K> struct link {};
		template<template<typename...> class tank, typename ...AK, typename ...AK2> struct link<tank<AK...>, tank<AK2...>> {
			typedef tank<AK..., AK2...> type;
		};

		template<typename T> struct is_type_template :std::false_type {};
		template<typename ...T, template <typename ...> class tank> struct is_type_template<tank<T...>> :std::true_type {};

		template <typename ...AT> struct type_set {};

		template <typename T> struct is_type_set :std::false_type {};
		template <typename ...T, template <typename...> class tank> struct is_type_set<tank<T...>> :std::true_type{};


		template < template<typename ...> class tank, typename ...at > struct bind_template_{
			template<typename ...aT> using front = tank< aT..., at...  >;
			template<typename ...ay> using back = tank< at..., ay... >;
		};

	}


}