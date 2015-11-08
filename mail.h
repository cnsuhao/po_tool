#pragma once
#include <memory>
#include <set>
#include <functional>
#include <list>
#include <map>
#include "template_deque.h"
#include "type_tool.h"
namespace PO
{
	namespace Tool
	{


		namespace Assistant
		{
			template<int i, typename T, typename K, int ...oi> struct mail_type_allocator;

			template<int i, typename T, typename K, typename ...AT, typename ...AK, int ...oi>
			struct mail_type_allocator<i, deque_<T, AT...>, deque_<K, AK...>, oi...>
			{
				typedef typename mail_type_allocator<i + 1, deque_<AT...>, deque_<K, AK...>, oi...>::type type;
			};

			template<int i, typename T, typename ...AT, typename ...AK, int ...oi>
			struct mail_type_allocator<i, deque_<T, AT...>, deque_<T, AK...>, oi...>
			{
				static_assert(sizeof...(AT) >= sizeof...(AK), "parameter type dismatch.");
				typedef typename mail_type_allocator<i + 1, deque_<AT...>, deque_<AK...>, oi...,i>::type type;
			};

			template<int i, typename ...AK, int ...oi>
			struct mail_type_allocator<i, deque_<AK...>, deque_<>, oi...>
			{
				typedef typename std::integer_sequence<int,oi...> type;
			};

			template<int i> struct replace_placeholder;

#define REPLACE_PLACEHOLDER(i) template<> struct replace_placeholder<i> { auto operator() () { return std::placeholders::_##i; } };
				REPLACE_PLACEHOLDER(1)
				REPLACE_PLACEHOLDER(2)
				REPLACE_PLACEHOLDER(3)
				REPLACE_PLACEHOLDER(4)
				REPLACE_PLACEHOLDER(5)
				REPLACE_PLACEHOLDER(6)
				REPLACE_PLACEHOLDER(7)
				REPLACE_PLACEHOLDER(8)
				REPLACE_PLACEHOLDER(9)
				REPLACE_PLACEHOLDER(10)
				REPLACE_PLACEHOLDER(11)
				REPLACE_PLACEHOLDER(12)
				REPLACE_PLACEHOLDER(13)
				REPLACE_PLACEHOLDER(14)
				REPLACE_PLACEHOLDER(15)
				REPLACE_PLACEHOLDER(16)
				REPLACE_PLACEHOLDER(17)
				REPLACE_PLACEHOLDER(18)
				REPLACE_PLACEHOLDER(19)
				REPLACE_PLACEHOLDER(20)
#undef REPLACE_PLACEHOLDER


			template<typename T> class mail_function_allocator;
			template<typename T, typename ...AT> class mail_function_allocator<T(AT...)>
			{
				template<typename C, int ...i> auto bing_assistant(C&& c, std::integer_sequence<int,i...>)
				{
					return std::move(std::function<T(AT...)>(std::bind(c, replace_placeholder<i+1>()()...)));
				}
				template<typename C, typename K,int ...i> auto bing_assistant(C&& c, K* k, std::integer_sequence<int,i...>)
				{
					return std::move(std::function<T(AT...)>(std::bind(c,k, typename replace_placeholder<decltype(std::placeholders::_1), i + 1>::type()...)));
				}
			public:
				template<typename C> auto operator()(C&& t) { return std::move(std::function<T(AT...)>(std::forward<C>(t))); };
				template<typename C, typename ...AC> auto operator()(C(*func)(AC...))
				{
					static_assert(sizeof...(AT) >= sizeof...(AC), "unable to bind function");
					return std::move(bing_assistant(func, typename mail_type_allocator<0,deque_<AT...>, deque_<AC...>>::type()));
				}
				template<typename C, typename K,typename ...AC,typename P> auto operator()(C(K::*func)(AC...),P* t)
				{
					return std::move(bing_assistant(func,t, typename mail_type_allocator<0, deque_<AT...>, deque_<AC...>>::type()));
				}
			};
		}

		class mail_index
		{
			struct index {};
			template<typename T> friend class mail;
			std::shared_ptr<index> all_index;
		public:
			template<typename T> mail_index(T&& t) :all_index(std::forward<T>(t)) {}
			mail_index() {}
			void disconnect() { all_index.reset(); }
		};

		template<typename T>  class mail;
		template<typename ...AT> 
		class mail<void(AT...)>
		{
			std::list<std::pair<std::weak_ptr<mail_index::index>,std::function<void(AT...)>>> all_mail;
		public:
			template<typename ...PT> void operator()(PT&& ... pt) 
			{
				for (auto po = all_mail.begin(); po != all_mail.end();)
				{
					auto tem = po->first.lock();
					if (tem)
					{
						po->second(pt...);
						++po;
					}
					else {
						all_mail.erase(po++);
					}
				}
			}
			void claer() 
			{
				all_mail.clear();
			}
			template<typename T> mail_index bind(T&& t) 
			{ 
				std::shared_ptr<mail_index::index> tem = std::make_shared< mail_index::index >();
				auto temf = Assistant::mail_function_allocator<void(AT...)>()(t);
				all_mail.push_back(std::make_pair( tem, std::move(temf)));
				return std::move(mail_index(tem));
			}
			template<typename T,typename K,typename ...AP,typename P> mail_index bind(T (K::*func)(AP...),P* k)
			{
				std::shared_ptr<mail_index::index> tem = std::make_shared< mail_index::index >();
				auto temf = Assistant::mail_function_allocator<void(AT...)>()(func,k);
				all_mail.push_back(std::make_pair(tem, std::move(temf)));
				return std::move(mail_index(tem));
			}
		};









		




















		/*class mail_box_index {};

		template<typename T = void> class mail_box :public T
		{
			std::shared_ptr<mail_box_index> index;
			template<typename ...AT> friend class mail;
		public:
			template<typename ...AT>
			mail_box(AT&&... at) :T(std::forward<AT>(at)...), index(std::make_shared<mail_box_index>()) {}
		};

		template<> class mail_box<void>
		{
			std::shared_ptr<mail_box_index> index;
			template<typename ...AT> friend class mail;
		public:
			mail_box() :index(std::make_shared<mail_box_index>()) {}
		};

		template<typename ...AT>
		class mail
		{
			struct mail_function
			{
				std::weak_ptr<mail_box_index> index;
				virtual void execute(AT... at) = 0;
				mail_function(const std::shared_ptr<mail_box_index>& k) :index(k) {}
				operator bool() const { return !index.expired(); }
				virtual bool equal(const mail_function&) const = 0;
				bool operator == (const mail_function& mf) const { return mf.index.lock() == index.lock() && equal(mf); }
			};

			template<typename T>
			struct mail_function_execute :public mail_function
			{
				T& t;
				void (T::*ui)(AT...);
				bool equal(const mail_function& mf) const
				{
					try {
						const mail_function_execute& mfe = dynamic_cast<const mail_function_execute&>(mf);
						return ui == mfe.ui;
					}
					catch (...)
					{
						return false;
					}
				}
				virtual void execute(AT... at)
				{
					(t.*ui)(at...);
				}
				mail_function_execute(const std::shared_ptr<mail_box_index>& c, T& k, void (T::*u)(AT...))
					:mail_function(c), t(k), ui(u) {}
			};
			std::list<std::shared_ptr<mail_function>> mail_index;
			std::shared_ptr<mail_box_index> index;
		public:
			template<typename T, typename K>
			void connect(mail_box<T>& t, void (K::*func)(AT...))
			{
				static_assert(
					std::is_same<K, T>::value || std::is_base_of<K, T>::value,
					"the object is not able to call the function pointer."
					);
				mail_index.push_back(std::make_shared<mail_function_execute<K>>(t.mail_box<T>::index, t, func));
			}

			template<typename T, typename K>
			void connect(T& t, void (K::*func)(AT...))
			{
				static_assert(std::is_base_of<mail_box<void>, T>::value, "the object must derives from mail_box<> or mail_box<void>.");
				static_assert(
					std::is_same<K, T>::value || std::is_base_of<K, T>::value,
					"the object is not able to call the function pointer."
					);
				mail_index.push_back(std::make_shared<mail_function_execute<K>>(t.mail_box<void>::index, t, func));
			}

			void connect(mail& t)
			{
				mail_index.push_back(std::make_shared<mail_function_execute<mail>>(t.index, t, &mail::sent));
			}

			void sent(AT... at)
			{
				for (auto Poi = mail_index.begin(); Poi != mail_index.end();)
				{
					if (*(*Poi))
					{
						(*(Poi++))->execute(at...);
					}
					else {
						mail_index.erase(Poi++);
					}
				}
			}
			void duplicate_checking()
			{
				for (auto Poi = mail_index.begin(); Poi != mail_index.end(); ++Poi)
				{
					auto Poi2 = Poi;
					for (++Poi2; Poi2 != mail_index.end();)
					{
						if (**Poi == **Poi2)
							mail_index.erase(Poi2++);
						else
							++Poi2;
					}
				}
			}
			mail() :index(std::make_shared<mail_box_index>()) {}
		};*/
	}
}
