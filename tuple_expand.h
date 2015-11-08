#ifndef PO_TOOL_TUPLE_EXPAND_H_INCLUDED
#define PO_TOOL_TUPLE_EXPAND_H_INCLUDED
#include <tuple>
#include "type_tool.h"
namespace PO
{
	namespace Tool
	{

		namespace Assistant
		{
			template<std::size_t ...tu_co, typename tuple_t, typename func_t>
			decltype(auto) tuple_unpack(std::index_sequence<tu_co...>, tuple_t&& tt, func_t&& fc)
			{
				return fc(std::get<tu_co>(std::forward<tuple_t>(tt))...);
			}

			template<std::size_t ...tu_co, typename tuple_t, typename func_t>
			decltype(auto) tuple_unpack_reverse(std::index_sequence<tu_co...>, tuple_t&& tt, func_t&& fc)
			{
				using namespace std;
				return fc(std::get<tuple_size< MAGIC_TYPENAME remove_reference<tuple_t>::type >::value - tu_co - 1>(tt)...);
			}

			template<std::size_t s, std::size_t ...tu_co, typename tuple_t>
			decltype(auto) tuple_cut_off_back(std::index_sequence<tu_co...>, tuple_t&& tt) {
				using namespace std;
				return make_tuple(get< tu_co + tuple_size< MAGIC_TYPENAME remove_reference<tuple_t>::type>::value - s >(tt)...);
			}

			template<std::size_t s, std::size_t ...p1, std::size_t ...p2, typename tuple_t, typename T, typename ...AT>
			auto tuple_insert(std::index_sequence<p1...>, std::index_sequence<p2...>, tuple_t&& tt, T&& t, AT&&... at) {
				return std::move(t(std::get<p1>(std::forward<tuple_t>(tt))..., std::forward<AT>(at)..., std::get<p2>(std::forward<tuple_t>(tt))...));
			}

			template<size_t s, size_t e> struct tuple_run_each_execute {
				template<typename tuple_t, typename func_t>
				void operator() ( tuple_t&& t, func_t&& ft ) {
					ft(get<s>(std::forward<tuple_t>(t)));
					tuple_run_each_execute<s + 1, e>()(std::forward<tuple_t>(t), std::forward<func_t>(ft));
				}
			};

			template<size_t s> struct tuple_run_each_execute<s,s> {
				template<typename tuple_t, typename func_t>
				void operator() (tuple_t&& t, func_t&& ft) {
					ft(std::get<s>(std::forward<tuple_t>(t)));
				}
			};

			template<size_t s> struct tuple_run_each_reverse_execute {
				template<typename tuple_t, typename func_t>
				void operator() (tuple_t&& t, func_t&& ft) {
					ft(get<s -1>(std::forward<tuple_t>(t)));
					tuple_run_each_reverse_execute<s - 1>()(std::forward<tuple_t>(t), std::forward<func_t>(ft));
				}
			};

			template<> struct tuple_run_each_reverse_execute<0> {
				template<typename tuple_t, typename func_t>
				void operator() (tuple_t&& t, func_t&& ft) {
					//ft(std::get<s>(std::forward<tuple_t>(t)));
				}
			};


		}


		template<typename tuple_t, typename func_t>
		decltype(auto) tuple_unpack(tuple_t&& tt, func_t&& ft)
		{
			using namespace std;
			return Assistant::tuple_unpack(
				make_index_sequence<
					tuple_size<
						typename remove_reference<tuple_t>::type
								>::value
									>(), std::forward<tuple_t>(tt), std::forward<func_t>(ft));
		}

		template<typename tuple_t,typename func_t>
		void tuple_run_each(tuple_t&& t, func_t&& ft) {
			tuple_run_each_execute<0, std::tuple_size<tuple_t>::value>()(std::forward<tuple_t>(t),std::forward<func_t>(ft));
		}

		template<typename tuple_t, typename func_t>
		void tuple_run_each_reverse(tuple_t&& t, func_t&& ft) {
			tuple_run_each_reverse_execute<std::tuple_size<tuple_t>::value>()(std::forward<tuple_t>(t), std::forward<func_t>(ft));
		}

		template<std::size_t s, typename tuple_t>
		decltype(auto) tuple_cut_off(tuple_t&& tt)
		{
			using namespace std;
			return Assistant::tuple_unpack(
						make_index_sequence<(
								tuple_size<
										typename remove_reference<tuple_t>::type
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
		auto tuple_insert(tuple_t&& tt, T&& t,AT&&... at)
		{
			constexpr std::size_t ts = std::tuple_size<std::remove_reference<tuple_t>::type>::value;
			return std::move(Assistant::tuple_insert<s>(
					type_index_range<0,( s > ts? ts : s ) >::type(),
					type_index_range<(s > ts ? ts : s), ts >::type(),
					std::forward<tuple_t>(tt),
					std::forward<T>(t),
					std::forward<AT>(at)...
				));
		}

		template<size_t ...t1,typename T,size_t ...t2,typename P>
		decltype(auto) bind_tuple(std::index_sequence<t1...>, T&& t, std::index_sequence<t2...>, P&& p) 
		{
			return std::make_tuple(  std::get<t1>(std::forward<T>(t))..., std::get<t2>(std::forward<P>(P))...);
		}

		template<size_t ...i,typename T, typename K, typename ope1>
		auto tuple_ope_order(std::index_sequence<i...>, T&& t, K&& k, ope1&& op1) 
		{
			return std::move(std::make_tuple(std::move(op1(std::get<i>(std::forward<T>(t)),std::get<i>(std::forward<K>(k)))...)));
		}

		template<typename T,typename K,typename ope1,typename = std::void_t<std::enable_if_t<std::tuple_size<T>::value == std::tuple_size<K>::value>>> 
		auto tuple_ope(T&& t,K&& k, ope1&& op1)
		{
			return std::move(tuple_ope_order(std::make_index_sequence<std::tuple_size<T>::value>(), std::forward<T>(t), std::forward<K>(k), std::forward<ope1>(op1)));
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
