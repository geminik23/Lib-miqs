//#pragma once
//#include "miqs_basictype.h"
//
//namespace miqs
//{
//
//	template <typename _Func>
//	struct multiple_bind
//	{
//		multiple_bind(_Func m):function{ m } {}
//
//		sample_t operator()(sample_t in)
//		{
//			return function()*in;
//		}
//
//		_Func function;
//	};
//
//	template <typename _Func>
//	struct addtion_bind
//	{
//		addtion_bind(_Func m):function{ m } {}
//
//		sample_t operator()(sample_t in)
//		{
//			return function() + in;
//		}
//
//		_Func function;
//	};
//
//
//	template <typename Func>
//	multiple_bind<Func&> make_ref_multiple_bind(Func& func) { return multiple_bind<Func&>(func); }
//	template <typename Func>
//	multiple_bind<Func> make_multiple_bind(Func func) { return multiple_bind<Func>(func); }
//
//	template <typename Func>
//	addtion_bind<Func&> make_ref_addtion_bind(Func& func) { return addtion_bind<Func&>(func); }
//	template <typename Func>
//	addtion_bind<Func> make_addtion_bind(Func func) { return addtion_bind<Func>(func); }
//
//}