#ifndef PO_TOOL_TEMPLATE_EXPRESSION_H_INCLUDED
#define PO_TOOL_TEMPLATE_EXPRESSION_H_INCLUDED
#include "magic_macro.h"
#include "type_tool.h"
namespace PO
{
	namespace Tool
	{

#define TE_TYPE_OPERATOR_2( ope , name ) \
		template<typename T,typename K> \
		struct template_expression_operator_##name \
		{ \
			template<typename ...AT> \
			auto operator() (AT&&... at) \
				{return T()(std::forward<AT>(at)...) ope K()(std::forward<AT>(at)...);} \
		}; \
        template<bool,typename T,typename T2,typename T3>\
        struct finnal_type_2_##name\
        {\
            typedef typename T:: template type_##name<T2,T3> type;\
        };\
        template<typename T,typename T2,typename T3>\
        struct finnal_type_2_##name<false,T,T2,T3>\
        {\
            typedef template_expression_operator_##name<T2,T3> type;\
        };\
        template<typename T,typename T2,typename T3> \
        struct type_detector_of_2_##name \
        { \
            template<typename P> \
            struct detector{}; \
            template<typename P>  static true_t fun( detector<typename  P::template type_##name<T2,T3> >* );\
            template<typename P>  static false_t fun(...); \
			typedef decltype( fun<T>(nullptr) )   target; \
            typedef typename finnal_type_2_##name< \
						target::value,T,T2,T3 \
						>::type type;\
        };

#define TE_TYPE_OPERATOR( ope , name ) \
		template<typename T> \
		struct template_expression_operator_##name \
		{ \
			template<typename ...AT> \
			auto operator() (AT&&... at) \
				{return ope T()(std::forward<AT>(at)...);} \
		}; \
		template<bool,typename T,typename T2>\
        struct finnal_type_##name\
        {\
            typedef typename T:: template type_##name<T2> type;\
        };\
        template<typename T,typename T2>\
        struct finnal_type_##name<false,T,T2>\
        {\
            typedef template_expression_operator_##name<T2> type;\
        };\
        template<typename T,typename T2> \
        struct type_detector_of_##name \
        { \
            template<typename P> \
            struct detector{}; \
            template<typename P>  static true_t fun( detector<typename P::template type_##name<T2> >* );\
            template<typename P>  static false_t fun(...); \
			typedef  decltype(fun<T>(nullptr)) target; \
            typedef typename finnal_type_##name< target::value,T,T2>::type type;\
        };\

		TE_TYPE_OPERATOR_2(&&, and)
		TE_TYPE_OPERATOR_2(|| , or )
		TE_TYPE_OPERATOR(!, not)
		TE_TYPE_OPERATOR_2(^, dif)


#undef TE_TYPE_OPERATOR_2
#undef TE_TYPE_OPERATOR


		struct template_expression_default_type
		{
			typedef template_expression_default_type this_type;

#define TE_TYPE_OPERATOR_2(ope,name) \
			auto operator ope (this_type) { return this_type(); } \
				template<typename T,typename K> using type_##name = template_expression_operator_##name<T,K>;
#define TE_TYPE_OPERATOR(ope,name) \
			auto operator ope () { return this_type(); } \
				template<typename T> using type_##name = template_expression_operator_##name<T>;

			TE_TYPE_OPERATOR_2(&&,and)
			TE_TYPE_OPERATOR_2(||,or)
			TE_TYPE_OPERATOR(!,not)
			TE_TYPE_OPERATOR_2(^, dif)

#undef TE_TYPE_OPERATOR_2
#undef TE_TYPE_OPERATOR

		};

		template<typename statement, typename Ide = template_expression_default_type>
		struct template_expression
		{
		    typedef Ide statement_type;
#define TE_TYPE_OPERATOR_2(ope_s,name) \
			template<typename state,typename Ide2> \
			auto operator ope_s ( template_expression<state,Ide2> p) \
			{ \
				return template_expression< \
                            typename type_detector_of_2_##name<Ide,template_expression,template_expression<state,Ide2>>::type\
                        ,decltype(Ide() ope_s Ide2())>();\
			}

#define TE_TYPE_OPERATOR(ope_s,name) \
			auto operator ope_s ( ) \
			{ \
				return template_expression< \
				typename type_detector_of_##name<Ide,template_expression>::type,\
				decltype( ope_s Ide())>();\
			}

#define TEMPLATE_EXPRESSION_OPERATOR_CALCULATE(ope_s) \
			template<typename ...T> \
			auto operator ope_s ( T&&... t) \
			{ \
				return statement().operator ope_s (std::forward<T>(t)...);\
			}

			TE_TYPE_OPERATOR_2(&&, and)
			TE_TYPE_OPERATOR_2(|| , or )
			TE_TYPE_OPERATOR(!, not)
			TE_TYPE_OPERATOR_2(^, dif)
			TEMPLATE_EXPRESSION_OPERATOR_CALCULATE(())

#undef TE_TYPE_OPERATOR_2
#undef TE_TYPE_OPERATOR
#undef TEMPLATE_EXPRESSION_OPERATOR_CALCULATE
		};
	}
}


#endif // TEMPLATE_EXPRESSION_H_INCLUDED
