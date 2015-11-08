#ifndef PO_TOOL_ADAPTER_H_INCLUDE
#define PO_TOOL_ADAPTER_H_INCLUDE
namespace PO
{
	namespace Tool
	{
		template<typename ...AT>
		struct adapter
		{
			adapter(){}
			template<typename ...AC>
				adapter(adapter<AC...>& ad) {}
			template<typename ...AC>
				adapter(adapter<AC...>&& ad) {}
		};

		template<typename T,typename ...AT>
		struct adapter<T,AT...>:public adapter<AT...>
		{
			typedef typename std::remove_reference<T>::type type;
			type& reference;
			operator type& () { return reference; }
			adapter(T&& t, AT&&... at) :reference(t), adapter<AT...>(at...) {}
			template<typename K, typename ...AC>
			adapter(adapter<K, AC...>& ad) : reference(ad), adapter<AT...>(ad){}
			template<typename K, typename ...AC>
			adapter(adapter<K, AC...>&& ad) : reference(ad), adapter<AT...>(ad) {}
		};

		template<typename ...AT>
		auto get_adapter(AT&& ...at){return adapter<AT...>(std::forward<AT>(at)...);}
	}
}
#endif
