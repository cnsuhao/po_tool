#pragma once
namespace PO {
	namespace Tool {
		namespace Assistant {

			template<typename index, typename data, typename next = void > struct _map_node_ {};
			template<typename T> struct is_map_node_ :std::false_type {};
			template<typename index, typename data, typename next> struct is_map_node_<_map_node_<index, data, next>> :is_map_node_<next> {};
			template<typename index, typename data> struct is_map_node_<_map_node_<index, data, void>> :std::true_type {};

			template<typename _node, typename index, typename date > struct _map_node_insert_ {
				static_assert(is_map_node_<_node>::value, " _map_node_insert only receive map_node_ ");
			};

			// operator insert
			template<typename index_,typename date_, typename next ,typename index, typename date > struct _map_node_insert_<_map_node_<index_,date_,next>,index,date> {
				typedef  _map_node_ < index_, date_, typename _map_node_insert_<next, index, date>::type > type;
			};
			template< typename date_, typename next, typename index, typename date > struct _map_node_insert_<_map_node_<index, date_, next>, index, date> {
				typedef  _map_node_ < index, date_, next > type;
			};
			template< typename index, typename date > struct _map_node_insert_<void, index, date> {
				typedef  _map_node_ < index, date, void > type;
			};

			// operator find
			template<typename _node, typename index> struct _map_node_find_ {
				static_assert( is_map_node_<_node>::value||std::is_same<_node,void>::value ," _map_node_find_ only receive _map_node_ " );
				static_assert(!std::is_same<_node, void>::value, " do not have date of this index ");
			};
			template<typename index_,typename date_,typename next_, typename index> struct _map_node_find_<_map_node_<index_,date_,next_ >,index > {
				typedef typename _map_node_find_<next_, index>::type type;
			};
			template< typename date_, typename next_, typename index> struct _map_node_find_<_map_node_<index, date_, next_ >, index > {
				typedef date_ type;
			};

			//operator find index
			template<typename _node, typename data> struct _map_node_find_index_ {
				static_assert(is_map_node_<_node>::value || std::is_same<_node, void>::value, " _map_node_find_ only receive _map_node_ ");
				static_assert(!std::is_same<_node, void>::value, " do not have index of this date ");
			};
			template<typename index_, typename date_, typename next_, typename date> struct _map_node_find_index_<_map_node_<index_, date_, next_ >, date > {
				typedef typename _map_node_find_index_<next_, date>::type type;
			};
			template< typename index_, typename next_, typename date> struct _map_node_find_index_<_map_node_<index_, date, next_ >, date > {
				typedef index_ type;
			};

			// operator have
			template<typename _node, typename index> struct _map_node_have_:std::false_type {
				static_assert(is_map_node_<_node>::value || std::is_same<_node, void>::value, " _map_node_have_ only receive _map_node_ ");
			};
			template<typename index_, typename date_, typename next_, typename index> struct _map_node_have_<_map_node_<index_, date_, next_ >, index >:_map_node_have_<next_,index> {};
			template<typename date_, typename next_, typename index> struct _map_node_have_<_map_node_<index, date_, next_ >, index > :std::true_type {};

			// operator erase
			template<typename _node, typename index> struct _map_node_erase_ {
				static_assert(is_map_node_<_node>::value || std::is_same<_node, void>::value, " _map_node_erase_ only receive _map_node_ ");
				typedef _node type;
			};
			template<typename index_, typename date_, typename next_, typename index> struct _map_node_erase_<_map_node_<index_, date_, next_ >, index > {
				typedef _map_node_< index_, date_, typename _map_node_erase_ < next_, index> ::type > type;
			};
			template< typename date_, typename next_, typename index> struct _map_node_erase_<_map_node_<index, date_, next_ >, index > {
				typedef next_ type;
			};

			// operator extract
			template < typename _node, template<typename ... > class tank, typename ...AT > struct _map_node_extract_ {
				static_assert(is_map_node_<_node>::value || std::is_same<_node, void>::value, " _map_node_extract_ only receive _map_node_ ");
				typedef tank<AT...> type;
			};
			template<typename index_, typename date_, typename next_, template<typename ... > class tank, typename ...AT > struct _map_node_extract_<_map_node_<index_, date_, next_ >, tank, AT... > {
				typedef typename _map_node_extract_< next_, tank, AT..., date_ >::type type;
			};
		}

		// make_pair_
		template<typename index, typename date > struct map_pair_ {};
		template<typename T> struct is_map_pair_ :std::false_type {};
		template<typename index, typename date, template <typename, typename> class pair_ > struct is_map_pair_<pair_<index, date>> :std::true_type {};


		//map_
		template<typename _node = void > struct map_ {
			static_assert(Assistant::is_map_node_<_node>::value || std::is_same<_node, void>::value, " map_ only receive _map_node_ ");
		};
		template<typename T> struct is_map_ :std::false_type{};
		template<typename _node > struct is_map_<map_<_node>> :std::true_type {};

		//operator insert
		template<typename map, typename index_, typename date = void > struct map_insert_ {
			static_assert(is_map_<map>::value, "make_map_execute_ only receive map_ ");
		};
		template<typename _node, typename index_, typename date_> struct map_insert_<map_<_node>, index_, date_> {
			typedef map_< typename Assistant::_map_node_insert_<_node, index_, date_ >::type > type;
		};
		template<typename _node, typename index_, typename date_,template<typename ,typename > class pair_> struct map_insert_<map_<_node>, pair_<index_, date_>,void > {
			typedef map_< typename Assistant::_map_node_insert_<_node, index_, date_ >::type > type;
		};

		//operator have
		template<typename map, typename index_> struct map_have_ {
			static_assert(is_map_<map>::value, "map_have_ only receive map_ ");
		};
		template<typename _node, typename index_> struct map_have_<map_<_node>, index_>:Assistant::_map_node_have_<_node,index_> {};

		//operator find
		template<typename map, typename index_> struct map_find_ {
			static_assert(is_map_<map>::value, "map_find_ only receive map_ ");
		};
		template<typename _node, typename index_> struct map_find_<map_<_node>, index_> {
			typedef typename Assistant::_map_node_find_<_node, index_>::type type;
		};

		//operator find
		template<typename map, typename data> struct map_find_index_ {
			static_assert(is_map_<map>::value, "map_find_ only receive map_ ");
		};
		template<typename _node, typename data> struct map_find_index_<map_<_node>, data> {
			typedef typename Assistant::_map_node_find_index_ <_node, data>::type type;
		};

		//operator erase
		template<typename map, typename index_> struct map_erase_ {
			static_assert(is_map_<map>::value, "map_erase_ only receive map_ ");
		};
		template<typename _node, typename index_> struct map_erase_<map_<_node>, index_> {
			typedef map_< typename Assistant::_map_node_erase_<_node, index_ >::type > type;
		};

		//operator extract
		template<typename map, template<typename...> class tank > struct map_extract_ {
			static_assert(is_map_<map>::value, "map_erase_ only receive map_ ");
		};
		template<typename _node, template<typename...> class tank > struct map_extract_<map_<_node>,tank> {
			typedef typename Assistant::_map_node_extract_<_node, tank>::type type;
		};


		// make_map_
		template<typename map, typename ...AT> struct make_map_execute_ {
			static_assert(is_map_<map>::value, "make_map_execute_ only receive map_ ");
			typedef map type;
		};
		template<typename _node,typename T, typename ...AT> struct make_map_execute_<map_<_node>, T, AT... > {
			static_assert(is_map_pair_<T>::value, " make_map_execute_ only recive map_pair_ ");
		};

		template<typename _node, typename i,typename d, template<typename,typename> class pair_,typename ...AT> struct make_map_execute_<map_<_node>,pair_<i,d>,AT... > {
			typedef typename make_map_execute_< map_< typename Assistant::_map_node_insert_<_node, i, d>::type  >, AT...>::type type;
		};
		template<typename ...AT> struct make_map_ {
			typedef typename make_map_execute_<map_<>, AT...>::type type;
		};
	}
}