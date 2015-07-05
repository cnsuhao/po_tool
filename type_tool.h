#pragma once
#include "magic_macro.h"
#include <utility>
namespace PO
{
	namespace Tool
	{
		template<typename T, T t> struct value_t { static constexpr T value = t ; };
		typedef value_t<bool, true> true_t;
		typedef value_t<bool, false> false_t;

		template<typename ...AT> struct type_set {};

		template<typename T> struct is_type_set { static constexpr bool value = false; };
		template<typename ...T, template<typename ...> class role> struct is_type_set<role<T...>> { static constexpr bool value = true; };

		template<typename T> struct is_integer_squence { static constexpr bool value = false; };
		template<typename T, T... TL1, template<typename K, K...> class ind> struct is_integer_squence<ind<T, TL1...>> { static constexpr bool value = true; };

		template<typename T, template<typename ...> class role = type_set> struct integer_translate_to_value_t {};
		template<typename T, T... TL, template<typename ...> class role > struct integer_translate_to_value_t<std::integer_sequence<T, TL...>, role> {
			typedef role<value_t<T, TL>...> type;
		};

		template<typename T, template<typename T, T...> class role = std::integer_sequence> struct value_t_translate_to_integer {};
		template<typename T, T... TL, template<typename ...> class tank, template<typename T, T...> class role >
		struct value_t_translate_to_integer<tank<value_t<T, TL>...>, role> {
			typedef role<T, TL...> type;
		};

		template<template<typename...> typename S1, template<typename...> typename S2> struct value_and {
			template<typename ...T> struct ope {
				static constexpr bool value = S1<T...>::value && S2<T...>::value;
			};
		};

		template<template<typename...> typename S1, template<typename...> typename S2> struct value_or {
			template<typename ...T> struct ope {
				static constexpr bool value = S1<T...>::value || S2<T...>::value;
			};
		};

		template<template<typename...> typename S1> struct value_not {
			template<typename ...T> struct ope {
				static constexpr bool value = ! S1<T...>::value;
			};
		};

		template<bool, typename S1=void, typename S2=void> struct either_or
		{
		    typedef S1 type;
            template<typename T,typename K>
            auto operator()(T&& t,K&& k){return t();}
        };

		template<typename S1, typename S2> struct either_or<false, S1, S2>
		{
		    typedef S2 type;
		    template<typename T,typename K>
            auto operator()(T&& t,K&& k){return k();}
        };

		template<typename T> struct set_size {};
		template<typename ...AK, template<typename ...> class tem> struct set_size<tem<AK...>> {
			static constexpr size_t size = sizeof...(AK);
		};

		template<typename T,T... TIL ,template<typename U,U... > class tem> struct set_size<tem<T,TIL...>> {
			static constexpr size_t size = sizeof...(TIL);
		};

		template<typename T,size_t s> struct set_get{};
		template<typename T, typename ...AK, template<typename ...> class role, size_t s> struct set_get<role<T, AK...>, s> {
			static_assert(s >= sizeof(AK...) + 1, "overflow");
			typedef typename set_get<role<AK...>,s-1>::type type;
		};
		template<typename T, typename ...AK, template<typename ...> class role> struct set_get<role<T, AK...>, 0> {
			typedef T type;
		};

		template<typename T, typename K> struct set_index_get {};
		template<typename ...AK, template<typename...> class tank, size_t ...i,template<typename T,T...> class ind> 
		struct set_index_get<tank<AK...>,ind<size_t,i...>>{
			typedef tank< set_get< tank<AK...>, i>...> type;
		};
		 
		template<typename T> struct is_empty { static constexpr bool value = (set_size<T>::size == 0); };

		template<typename T> struct itself{
			typedef T type;
		};

		template<bool, template<typename ...> class t1 = itself , template<typename...> class t2 = itself > struct either_or_mapping {
			template<typename ...AT> using mapping = t1<AT...>;
		};

		template<template<typename ...> class t1, template<typename ...> class t2 > struct either_or_mapping<false,t1,t2> {
			template<typename ...AT> using mapping = t2<AT...>;
		};

		template<bool, template<typename> typename filter, typename T> struct if_filter {
			typedef typename filter<T>::type type;
		};

		template<template<typename> typename filter, typename T> struct if_filter<false,filter,T> {
			typedef T type;
		};

		template<typename ...AT> struct link{};
		template<typename ...AP, typename ...AP2, template<typename ...> typename K, typename ...AT> struct link<K<AP...>,K<AP2...>,AT...> {
			typedef typename link< K<AP..., AP2...>, AT... >::type type;
		};
		template<typename ...AP, template<typename ...> typename K> struct link<K<AP...>> {
			typedef  K<AP...> type;
		};

		template<typename T,T ...TL1, T ...TL2,template<typename T,T ...> typename K, typename ...AT> struct link<K<T,TL1...>, K<T,TL2...>, AT...> {
			typedef typename link< K<T,TL1...,TL2...>, AT... >::type type;
		};

		template<typename T, T ...TL1, template<typename T, T ...> typename K> struct link<K<T,TL1...>> {
			typedef  K<T, TL1...> type;
		};

		template<typename ...AT> struct is_one_of { static constexpr bool value = false; };

		template<typename T, typename K, typename ...AT> struct is_one_of<T, K, AT...> {
			static constexpr bool value = ( std::is_same<T,K>::value || is_one_of<T, AT...>::value );
		};
		template<typename T> struct is_one_of<T>
		{
			static constexpr bool value = false;
		};

		template<typename ...AT> struct is_repeat { static constexpr bool value = false; };
		template<typename T, typename ...AT> struct is_repeat <T,AT...>
		{
			static constexpr bool value = is_one_of<T, AT...>::value ||
				is_repeat<AT...>::value;
		};

        template<typename T,template<typename ...> typename P > struct extract{};
        template< typename ...AT,template<typename ...> class P2 ,template<typename ...> class P> struct extract<P2<AT...>,P>
        {
            typedef P<AT...> type;
        };

		template<typename T, template<typename...> typename role,size_t t = 0> struct filter {};
		template<template<typename...> typename tem, template<typename...> typename role,size_t s> struct filter<tem<>,role,s> {
			typedef tem<> type;
			typedef std::index_sequence<> index;
		};
		template < typename T, typename ...AT, template<typename...> typename tem, template<typename> typename role, size_t s>
		struct filter<tem<T,AT...>, role,s> {
			typedef typename link< typename either_or<role<T>::value, tem<T>, tem<>>::type,typename filter<tem<AT...>,role,s+1>::type>::type type;
			typedef typename link< typename either_or<role<T>::value, std::index_sequence<s>, std::index_sequence<> >::type , typename filter<tem<AT...>, role, s + 1>::index  >::type index;
		};

		

		template<typename T, template<typename> class role> struct mapping {};
		template<typename ...AT, template<typename ...> class tem, template<typename> class role> struct mapping<tem<AT...>, role> {
			typedef tem< typename role<AT>::type... > type;
		};

		
		struct placeholder {};

		template<typename T> struct is_placeholder { static constexpr bool value = std::is_same<T, placeholder>::value; };

		template<size_t i> struct placeholder_count {};

		template<typename T, typename K> struct placeholeder_cather {
			typedef T type;
		};

		template<size_t i, typename T> struct placeholeder_cather<placeholder_count<i>,T> {
			typedef typename set_get<T, i>::type type;
		};

		template<typename T, size_t = 0> struct placeholder_count_constructor {};
		template<typename T, typename ...ot, size_t t>
		struct placeholder_count_constructor<type_set<T, ot...>, t> {
			typedef typename Tool::link<
				typename either_or<is_placeholder<T>::value, type_set<placeholder_count<t>>, type_set<T> >::type,
				typename placeholder_count_constructor< type_set<ot...>, (is_placeholder<T>::value ? t + 1 : t)>::type
			>::type type;
		};
		template<size_t t> struct placeholder_count_constructor<type_set<>, t> {
			typedef type_set<> type;
		};

		namespace Assistant {

			template<template <typename ...> class role, typename T > class bind_mapping_execute {};
			template<template <typename ...> class role, typename ...AK> class bind_mapping_execute<role,type_set<AK...>> {
				template<typename ...AT> using binding = role< typename placeholeder_cather<AK,type_set<AT...>>::type...>;
			};

		};

		template< template<typename ...> class map,typename ...AT > class bind_mapping {

			typedef typename placeholder_count_constructor<type_set<AT...>>::type output_type;
			typedef Assistant::bind_mapping_execute< map, output_type > execute_type;
		public:
			template<typename ...AK> using binding =  typename  execute_type:: template binding<AK...>;
		};

		template<std::size_t s, std::size_t e> struct index_range {
			typedef typename link < std::index_sequence<s>, typename index_range<s + (e > s ? 1 : -1), e >::type >::type type;
		};

		template<std::size_t e> struct index_range<e, e> {
			typedef std::index_sequence<> type;
		};

		template<std::size_t t> struct index {
			typedef typename index_range<0, t>::type type;
		};


		template< typename T, template<typename...> typename role, size_t t = 0 > struct all_filter {};
		template< typename T, typename ...AT,template<typename...> class tank, template<typename...> class role, size_t s> 
		struct all_filter< tank<T,AT...>, role, s> {
			typedef typename link< typename either_or<role<T,AT...>::value, tank<T>, tank<>>::type, typename all_filter<tank<AT...>, role, s + 1>::type>::type type;
			typedef typename link< typename either_or<role<T,AT...>::value, std::index_sequence<s>, std::index_sequence<> >::type, typename all_filter<tank<AT...>, role, s + 1>::index  >::type index;
		};

		template< template<typename...> class tank, template<typename...> class role, size_t t>
		struct all_filter< tank<>, role, t> {
			typedef  tank<> type;
			typedef  std::index_sequence<>  index;
		};



	}
}
