#pragma once
#include <functional>
namespace PO {
	namespace Tool {

		namespace Assistant {
			template<typename T,typename P>
			struct iterator_tool_template_express {
				T t;P p;
				iterator_tool_template_express(const T& t1,const P& p2):t(t1),p(p2){}
				iterator_tool_template_express(const iterator_tool_template_express& itte):t(itte.t),p(itte.p){}
				template<typename K> bool is_avalible(K&& k) {
					return t.is_avalible(std::forward<K>(k)) && p.is_avalible(t(std::forward<K>(k)));
				}
				template<typename K> decltype(auto) operator()(K&& k) { return p(t(std::forward<K>(k))); }
			};

			struct iterator_tool_all_away_true {
				template<typename T> bool is_avalible(T&& t) { return true; }
				template<typename T> decltype(auto) operator()(T&& t) { return t; }
			};

			template<typename T>
			class iterator_tool_filter_function {
				std::function<bool(T)> func;
			public:
				bool is_avalible(T t) { return func(t); }
				decltype(auto) operator()(T t) { return t; }
				iterator_tool_filter_function(const std::function<bool(T)>& t) :func(t) {}
				iterator_tool_filter_function(const iterator_tool_filter_function& tc) :func(tc.func) {}
			};

			template<typename T>
			class iterator_tool_map_function {
				typedef typename std::remove_reference<T>::type return_type;
				std::function< return_type(T) > func;
			public:
				bool is_avalible(T t) { return true; }
				decltype(auto) operator()(T t) { return func(t); }
				iterator_tool_map_function(const std::function<return_type(T)>& t) :func(t) {}
				iterator_tool_map_function(const iterator_tool_map_function& tc) :func(tc.func) {}
			};
		}

		template<typename T,typename role>
		class tank_ref {

			T tank;
			role role_for_tank;

			typedef typename std::remove_reference<T>::type original_type;
			typedef typename std::remove_reference<decltype(*((original_type*)(nullptr))->begin())>::type store_type;

		public:

			class iterator {

			public:

				typedef typename original_type::iterator original_ite;
				

			private:
				original_ite current_iterator;
				original_ite end_iterator;
				role& role_for_tank;

			public:

				operator original_ite() { return current_iterator; }


				iterator& operator ++ () {
					while (current_iterator != end_iterator) {
						++current_iterator;
						if ( !(current_iterator != end_iterator) || role_for_tank.is_avalible(*current_iterator))
							break;
					}
					return *this;
				}

				iterator operator++ (int) {
					iterator tem(*this);
					++*this;
					return tem;
				}

				

				bool operator != (const iterator& it) const { return current_iterator != it.current_iterator; }
				decltype(auto) operator*() { return role_for_tank(*current_iterator); }

				iterator(const iterator& it) :current_iterator(it.current_iterator), end_iterator(it.end_iterator) ,role_for_tank(it.role_for_tank){}
				iterator(role& rft):role_for_tank(rft) {}

				iterator(const original_ite & cu, const original_ite & en2, role& rft) :current_iterator(cu), end_iterator(en2), role_for_tank(rft){}
			};

			typedef decltype(((iterator*)(nullptr))->operator*()) output_type;

		public:

			iterator begin() { 
				auto start(tank.begin());
				auto end(tank.end());
				while (start != end) {
					if (role_for_tank.is_avalible(*start))
						break;
					++start;
				}
				return iterator(start,end,role_for_tank);
			}

			iterator end() { 
				return iterator(tank.end(), tank.end(), role_for_tank);
			}

			tank_ref(T&& t2,const role& r) :tank(std::forward<T>(t2)), role_for_tank(r){}
			tank_ref(const tank_ref& t2) : tank_ref(t2.tank,t2.role_for_tank){}
			tank_ref(tank_ref&& t2) :tank_ref(std::move(t2.t),t2.role_for_tank) {}

			decltype(auto) filter(const std::function<bool(output_type)>& t) {
				Assistant::iterator_tool_filter_function<output_type> tem_pre(t);
				Assistant::iterator_tool_template_express<role, Assistant::iterator_tool_filter_function<output_type>> tem(role_for_tank, tem_pre);
				return tank_ref<
					T,
					Assistant::iterator_tool_template_express<role,Assistant::iterator_tool_filter_function<output_type>>
				>(tank, tem);
			}

			decltype(auto) map(const std::function<store_type(output_type)>& t) {
				Assistant::iterator_tool_map_function<output_type> tem2(t);
				Assistant::iterator_tool_template_express<role, Assistant::iterator_tool_map_function<output_type>> tem(role_for_tank, tem2);
				return tank_ref<
					decltype(*this),
					Assistant::iterator_tool_template_express<role, Assistant::iterator_tool_map_function<output_type>>
				>(*this, tem);
			}

			void for_each(std::function<output_type> func) {
				for (auto Po = begin();Po != end();++Po) {
					func(*Po);
				}
			}

			void for_each_iterator(std::function<typename original_type::iterator> func) {
				for (auto k = begin();k != end();) {
					func(k++);
				}
			}
		};

		template<typename T>
		decltype(auto) ref(T&& t) { return tank_ref<T, Assistant::iterator_tool_all_away_true>(std::forward<T>(t), Assistant::iterator_tool_all_away_true()); }

	}
}
