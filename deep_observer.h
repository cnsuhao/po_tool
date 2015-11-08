#ifndef PO_TOOL_DEEP_OBSERVER_H_INCLUDED
#define PO_TOOL_DEEP_OBSERVER_H_INCLUDED
#include <memory>
#include <map>
namespace PO
{
	namespace Tool
	{
		template<typename T, typename P>
		class resource_control_reference_count
		{
		public:
			struct ptr_packet
			{
				std::shared_ptr<P> data;
				P* operator->() { return data.get(); }
				P& operator*() { return *data; }
				const P* operator->() const { return data; }
				const P& operator*() const { return *data; }
				operator bool() const { return static_cast<bool>(data); }
				operator P& () { return *data; }
				operator const P& () const { return *data; }
				ptr_packet(std::shared_ptr<P>&& d) :data(std::move(d)) {}
				ptr_packet(const std::shared_ptr<P>& d) :data(d) {}
				ptr_packet(ptr_packet&& p) :data(std::move(p.data)) {}
				ptr_packet() = default;
				ptr_packet& operator= (std::shared_ptr<P>&& p) { data = std::move(p); return *this; }
				ptr_packet& operator= (const std::shared_ptr<P>& p) { data = p; return *this; }
				template<typename ...AK>
				ptr_packet(AK&&... ak) :data(std::make_shared<P>(std::forward<AK>(ak)...)) {}
				template<typename ...AK>
				std::shared_ptr<P> create(AK&&... ak) { return std::make_shared<P>(std::forward<AK>(ak)...); }
				void free() { data.reset(); }
			};

		private:
			std::map<T, std::weak_ptr<ptr_packet> > resource;

		public:

			class ptr
			{
				std::shared_ptr<ptr_packet> data;
			public:
				P& operator*() { return (*data).operator*(); }
				P* operator->() { return (*data).operator->(); }
				const P& operator*() const { return (*data).operator*(); }
				const P* operator->() const { return (*data).operator->(); }
				operator bool() const { return static_cast<bool>(data) && static_cast<bool>(*data); }
				ptr& operator = (ptr&& p) { data = std::move(p.data); return *this; }
				ptr& operator = (const ptr& p) { data = p.data; return *this; }
				ptr(std::shared_ptr<ptr_packet>&& d) :data(d) {}
				ptr(ptr&& p) = default;
				ptr(const ptr& p) = default;
				ptr() {}
				operator P& () { return *data; }
				operator const P& () const { return *data; }
			};

			typedef std::shared_ptr<P> resource_type;

			template<typename ...AK>
			ptr create_resource(const T& t, AK&&... k)
			{
				auto te = resource.find(t);
				if (te != resource.end())
				{
					if (te->second.expired())
					{
						std::shared_ptr<ptr_packet> tem2(std::make_shared<ptr_packet>(std::forward<AK>(k)...));
						te->second = tem2;
						return ptr(std::move(tem2));
					}
					else {
						return ptr(te->second.lock());
					}
				}
				else {
					std::shared_ptr<ptr_packet> tem2(std::make_shared<ptr_packet>(std::forward<AK>(k)...));
					std::weak_ptr<ptr_packet> tem3(tem2);
					resource.insert(std::make_pair(t, std::move(tem3)));
					return ptr(std::move(tem2));
				}
			}

			template<typename fun>
			ptr create_resource_if(const T& t, fun&& fu)
			{
				auto te = resource.find(t);
				if (te != resource.end())
				{
					if (te->second.expired())
					{
						std::shared_ptr<ptr_packet> tem2(std::make_shared<ptr_packet>(fu()));
						te->second = tem2;
						return ptr(std::move(tem2));
					}
					else {
						return ptr(te->second.lock());
					}
				}
				else {
					std::shared_ptr<ptr_packet> tem2(std::make_shared<ptr_packet>(fu()));
					std::weak_ptr<ptr_packet> tem3(tem2);
					resource.insert(std::make_pair(t, std::move(tem3)));
					return ptr(std::move(tem2));
				}
			}

			template<typename I>
			void for_each(I&& t)
			{
				for (auto i = resource.begin(); i != resource.end();)
				{
					if (i->second.expired())
					{
						auto u = i;
						i++;
						resource.erase(u);
					}
					else {
						t(*((*(i->second.lock())).data), i->first);
						++i;
					}
				}
			}

			template<typename I>
			void for_each_packet(I&& t)
			{
				for (auto i = resource.begin(); i != resource.end();)
				{
					if (i->second.expired())
					{
						auto u = i;
						i++;
						resource.erase(u);
					}
					else {
						t(*(i->second.lock()), i->first);
						++i;
					}
				}
			}
		};
	}
}
#endif //PO_TOOL_DEEP_OBSERVER_H_INCLUDED
