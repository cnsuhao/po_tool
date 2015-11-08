#pragma once
namespace PO {
	namespace Tool {
		namespace TTC {
			template<typename T> struct ope_type {
				typedef typename T::type type;
			};

			template<typename T, typename K> struct ope_have {
				typedef typename T::template have<K> type;
			};

			template<typename T, typename K, typename P> struct ope_insert {
				typedef typename T::template insert<K, P>::type type;
			};

			template<typename T, typename K> struct ope_get {
				typedef typename T::template get<K>::type type;
			};

			template<typename T, typename K> struct ope_erase {
				typedef typename T::template erase<K>::type type;
			};
		}
	}
}