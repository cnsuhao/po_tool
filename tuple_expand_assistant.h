#ifndef PO_TOOL_TUPLE_EXPAND_ASSISTANT_H_INCLUDED
#define PO_TOOL_TUPLE_EXPAND_ASSISTANT_H_INCLUDED
#include "type_set.h"
#include "magic_macro.h"
namespace PO
{
	namespace Tool
	{
		namespace Assistant
		{
			template<std::size_t ...tu_co,typename tuple_t,typename func_t>
			decltype(auto) tuple_unpack(std::index_sequence<tu_co...>, tuple_t&& tt, func_t&& fc)
			{
				return fc(std::get<tu_co>(std::forward<tuple_t>(tt))...);
			}

			template<std::size_t ...tu_co, typename tuple_t, typename func_t>
			decltype(auto) tuple_unpack_reverse(std::index_sequence<tu_co...>, tuple_t&& tt, func_t&& fc)
			{
				using namespace std;
				return fc(std::get<tuple_size< MAGIC_TYPENAME remove_reference<tuple_t>::type >::value-tu_co-1>(tt)...);
			}

			template<std::size_t s, std::size_t ...tu_co,typename tuple_t>
			decltype(auto) tuple_cut_off_back(std::index_sequence<tu_co...>,tuple_t&& tt){
				using namespace std;
				return make_tuple(get< tu_co + tuple_size< MAGIC_TYPENAME remove_reference<tuple_t>::type>::value - s >(tt)...);
			}

			template<std::size_t s, std::size_t ...p1, std::size_t ...p2,typename tuple_t, typename T, typename ...AT>
			decltype(auto) tuple_insert(std::index_sequence<p1...>, std::index_sequence<p2...>, tuple_t&& tt, T&& t, AT&&... at){
				return t(std::get<p1>(std::forward<tuple_t>(tt))..., std::forward<AT>(at)..., std::get<p2>(std::forward<tuple_t>(tt))...);
			}
		}
	}
}

#endif // TUPLE_EXPAND_ASSISTANT_H_INCLUDED
