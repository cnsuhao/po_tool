#pragma once
#include "type_tool.h"
#include "template_depute.h"
namespace PO{
    namespace Tool{

		template <typename T, typename P> struct is_subset {
			static_assert(is_depute_<T>::value && is_depute_<P>::value, "is_subset only receive depute_.");
		};

		template < typename ...AT, typename ...P >
		struct is_subset <is_depute_<AT...>, is_depute_<P...>> {
			static constexpr bool value = value_and<true, !is_one_of<AT, P...>::value...>::value;
		};

		template<typename T, typename K> struct is_same_set {
			static_assert(is_depute_<T>::value && is_depute_<K>::value, "is_same_set only recive depute_.");
			static constexpr bool value = is_subset<T, K>::value && is_subset<K, T>::value;
		};

		template<typename T, typename P> struct is_porper_subset
		{
			static_assert(is_depute_<T>::value && is_depute_<P>::value, "is_porper_subset only receive depute_.");
			static constexpr bool value = is_subset<T, P>::value && !is_subset<P, T>::value;
		};

		template<typename T,typename P> class union_set{
			static_assert(is_depute_<T>::value && is_depute_<P>::value ,"union_set only recive depute_");
		};
		template<typename ...T , typename ...K>
		class union_set<depute_<T...>, depute_<K...>> {
			struct mark {};
		public:
			typedef typename remove<mark,depute_, typename std::conditional<!is_one_of<T, K...>::value, T, mark>::type..., K...>::type type;
		};

		template<typename T, typename P> class union_set_index {
			static_assert(is_depute_<T>::value && is_depute_<P>::value, "union_set_index only recive type_set");
		};

		template<typename ...T,  typename ...K>
		class union_set_index<depute_<T...>, depute_<K...>> {
			struct mark {};
		public:
			typedef typename locate<mark, typename std::conditional<is_one_of<T, K...>::value, T, mark>::type..., K...>::type type;
		};

		//template<typename ...T, template 

		/*template<typename T,typename K> struct able_for_set_calculate { 
			static constexpr bool value = (is_integer_squence<T>::value &&is_integer_squence<K>::value )||( is_type_set<K>::value&&is_type_set<T>::value );
		};


		template<typename T, typename K>  class is_subset {
			static_assert(able_for_set_calculate<T, K>::value, "only integer or type set is enable");

			template<typename T> using translate = integer_translate_to_value_t<T,type_set>;
			typedef typename either_or_mapping< is_integer_squence<T>::value,translate>::template mapping<T>::type statement_1;
			typedef typename either_or_mapping< is_integer_squence<K>::value,translate>::template mapping<K>::type statement_2;
			template<typename ...AT> using temporary_mapping = bind_mapping<is_one_of, Tool::placeholder, AT...>;
			typedef typename extract<statement_2, temporary_mapping>::type mapping_type;
			template<typename T> using pre_mapping = typename mapping_type::template binding<T>;
			typedef typename filter<statement_1,  pre_mapping >::type filter_type;

		public:
			static constexpr bool value = std::is_same<filter_type, statement_1>::value;

		};
		
		
		template<typename T> struct type_set_index_translate {};
		template<std::size_t ...at> struct type_set_index_translate<std::index_sequence<at...>> {
			typedef type_set<value_t<std::size_t, at>...> type;
		};
		template<std::size_t ...at> struct type_set_index_translate<type_set<value_t<std::size_t, at>...>> {
			typedef std::index_sequence< at...> type;
		};

		template<typename T,typename P> struct is_same_set
		{
            static constexpr bool value = is_subset<T,P>::value && is_subset<P,T>::value;
		};

		template<typename T,typename P> struct is_porper_subset
		{
            static constexpr bool value = is_subset<T,P>::value && ! is_subset<P,T>::value;
		};

		

        template<typename T> class unrepeat_set{
			static_assert(is_integer_squence<T>::value  || is_type_set<T>::value, "only integer or type set is enable");

			template<typename T> using translate = integer_translate_to_value_t<T, type_set>;
			typedef typename either_or_mapping< is_integer_squence<T>::value, translate>::template mapping<T>::type statement_1;
			typedef typename all_filter<statement_1, value_not<is_one_of>::ope> filter_type;
			template<typename T> using pre_translate = value_t_translate_to_integer<T, std::integer_sequence>;

		public:
			typedef typename either_or_mapping<is_integer_squence<T>::value, pre_translate>::template mapping<typename filter_type::type>::type type;
			typedef typename filter_type::index index;
		};

        template<typename T,typename P> class union_set
        {
			static_assert(able_for_set_calculate<T, P>::value, "only integer or type set is enable");
			template<typename T> using translate = integer_translate_to_value_t<T, type_set>;
			typedef typename either_or_mapping< is_integer_squence<T>::value, translate>::template mapping<T>::type statement_1;
			typedef typename either_or_mapping< is_integer_squence<P>::value, translate>::template mapping<P>::type statement_2;

			template<typename ...AT> using mapping_template = bind_mapping<is_one_of, Tool::placeholder, AT...>;
			typedef typename extract<statement_1, mapping_template>::type pre_extract_type;
			typedef typename extract<statement_2, mapping_template>::type last_extract_type;

			typedef typename filter< statement_2, typename value_not<typename pre_extract_type::template binding>::template ope > last_filter_type;
			typedef typename filter< statement_1, typename value_not<typename last_extract_type::template binding>::template ope  > pre_filter_type;

			typedef typename link< statement_1, typename last_filter_type::type >::type pre_type_set;
			typedef typename link< typename pre_filter_type::type , statement_2 >::type last_type_set;

			template<typename T> using pre_translate = value_t_translate_to_integer<T, std::integer_sequence>;

		public:

			typedef typename either_or_mapping<is_integer_squence<T>::value, pre_translate>::template mapping<pre_type_set>::type pre_type;
			typedef typename either_or_mapping<is_integer_squence<T>::value, pre_translate>::template mapping<last_type_set>::type last_type;

			typedef typename last_filter_type::index last_index;
			typedef typename pre_filter_type::index pre_index;
        };

        template<typename T,typename P> class intersection_set{
			static_assert(able_for_set_calculate<T, P>::value, "only integer or type set is enable");
			template<typename T> using translate = integer_translate_to_value_t<T, type_set>;
			typedef typename either_or_mapping< is_integer_squence<T>::value, translate>::template mapping<T>::type statement_1;
			typedef typename either_or_mapping< is_integer_squence<P>::value, translate>::template mapping<P>::type statement_2;

			template<typename... AT> using mapping_template = bind_mapping<is_one_of, Tool::placeholder, AT...>;
			typedef typename extract<statement_1, mapping_template>::type pre_extract_type;
			typedef typename extract<statement_2, mapping_template>::type last_extract_type;

			typedef typename filter< statement_2, typename pre_extract_type::template binding> last_filter_type;
			typedef typename filter< statement_1, typename last_extract_type::template binding> pre_filter_type;

			template<typename T> using pre_translate = value_t_translate_to_integer<T, std::integer_sequence>;

		public:
			typedef typename either_or_mapping<is_integer_squence<T>::value, pre_translate>::template mapping<typename pre_filter_type::type>::type pre_type;
			typedef typename either_or_mapping<is_integer_squence<T>::value, pre_translate>::template mapping<typename last_filter_type::type>::type last_type;
			typedef typename last_filter_type::index last_index;
			typedef typename pre_filter_type::index pre_index;
		};
   

        template<typename T,typename P> class complementary_set{
			static_assert(able_for_set_calculate<T, P>::value, "only integer or type set is enable");
			template<typename T> using translate = integer_translate_to_value_t<T, type_set>;
			typedef typename either_or_mapping< is_integer_squence<T>::value, translate>::template mapping<T>::type statement_1;
			typedef typename either_or_mapping< is_integer_squence<P>::value, translate>::template mapping<P>::type statement_2;
			template<typename T> using pre_translate = value_t_translate_to_integer<T, std::integer_sequence>;

			template<typename... AT> using mapping_template = bind_mapping<is_one_of, Tool::placeholder, AT...>;
			typedef typename extract<statement_1, mapping_template>::type extract_type;
			typedef filter<statement_2, typename extract_type::template binding> filter_type;

		public:

			typedef typename filter_type::type type;
			typedef typename filter_type::index index;
		};*/

    }
}
