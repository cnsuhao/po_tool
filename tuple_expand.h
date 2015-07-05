#ifndef PO_TOOL_TUPLE_EXPAND_H_INCLUDED
#define PO_TOOL_TUPLE_EXPAND_H_INCLUDED
#include <tuple>
#include "tuple_expand_assistant.h"

namespace PO
{
	namespace Tool
	{
		template<typename tuple_t, typename func_t>
		decltype(auto) tuple_unpack(tuple_t&& tt, func_t&& ft)
		{
			using namespace std;
			return Assistant::tuple_unpack(
				make_index_sequence<
					tuple_size<
						MAGIC_TYPENAME remove_reference<tuple_t>::type
								>::value
									>(), std::forward<tuple_t>(tt), std::forward<func_t>(ft));
		}

		template<std::size_t s, typename tuple_t>
		decltype(auto) tuple_cut_off(tuple_t&& tt)
		{
			using namespace std;
			return Assistant::tuple_unpack(
						make_index_sequence<(
								tuple_size<
										MAGIC_TYPENAME remove_reference<tuple_t>::type
											>::value > s ?
								s : tuple_size<MAGIC_TYPENAME remove_reference<tuple_t>::type>::value
								)>(), std::forward<tuple_t>(tt), [](auto&& ... ai) {return make_tuple(std::forward<auto>(ai)...); });
		}

		template<std::size_t s, typename tuple_t>
		decltype(auto) tuple_cut_off_back(tuple_t&& tt)
		{
			using namespace std;
			return	Assistant::tuple_cut_off_back<(
					tuple_size< MAGIC_TYPENAME remove_reference<tuple_t>::type>::value > s ?
					s : tuple_size< MAGIC_TYPENAME remove_reference<tuple_t>::type>::value
													)>
													(
														make_index_sequence<(
																tuple_size< MAGIC_TYPENAME remove_reference<tuple_t>::type>::value > s ?
																s : tuple_size< MAGIC_TYPENAME remove_reference<tuple_t>::type>::value
																			)>(), std::forward<tuple_t>(tt)
													);
		}

		template<std::size_t s,typename tuple_t,typename T,typename ...AT>
		decltype(auto) tuple_insert(tuple_t&& tt, T&& t,AT&&... at)
		{
			constexpr std::size_t ts = std::tuple_size<std::remove_reference<tuple_t>::type>::value;
			return Assistant::tuple_insert<s>(
					type_index_range<0,( s > ts? ts : s ) >::type(),
					type_index_range<(s > ts ? ts : s), ts >::type(),
					std::forward<tuple_t>(tt),
					std::forward<T>(t),
					std::forward<AT>(at)...
				);
		}

		template<size_t ...t1,typename T,size_t ...t2,typename P>
		decltype(auto) bind_tuple(std::index_sequence<t1...>, T&& t, std::index_sequence<t2...>, P&& p) {
			return std::make_tuple(  std::get<t1>(std::forward<T>(t))..., std::get<t2>(std::forward<P>(P))...);
		}

		template<typename T>
		struct tuple_constructor :public T
		{

			template<typename tuple_t>
			tuple_constructor(tuple_t&& st)
				:tuple_constructor(
					std::make_index_sequence<
						std::tuple_size< typename std::remove_reference<tuple_t>::type >::value
											>(), 
					std::forward<tuple_t>(st)){}

			operator T& () { return *this; }

			T& get_original(){return *this;}

		private:

			template<std::size_t ...ind, typename Tt>
			tuple_constructor(std::index_sequence<ind...>, Tt& st)
				: T(std::get<ind>(st)...) {}
		};
	}
}


#endif // TUPLE_EXPAND_H_INCLUDED
