// ReSharper disable CppNonExplicitConvertingConstructor
#pragma once
#include <functional>

#include "Either.h"

namespace libmonad
{

		struct None {};
		
		template <typename T>
		class Option
		{		
			bool isNone {};
			Either<None,T> value {};			

		public:
			
			Option(T in): value(std::move(in)) { }
			Option(None n = {}){ value = n; }
						
			bool IsNone() const { return value.IsLeft(); }
			bool IsSome() const { return value.IsRight(); }

			template <typename T2>
			Option<T2> ToOption(Either<None, T2> either)
			{
				Option<T2> option = None();

				either.MatchVoid( [&](None n)
				                  {
					                  option = None();
				                  }, [&](T2 t2)
				                  {
					                  option = t2;
				                  });
				return option;
			}

			template <typename T2>
			Option<T2> Map(std::function<Option<T2>(T)> transform)
			{
				std::function<Option<T2>(T)> fn = [=](T t) -> Option<T2> { return transform(t); };
				Either<None, Option<T2>> either = value.Map(fn);

				Option<T2> option = either.Match([](None n){ return n;}, [](Option<T2> t2){ return t2;});
				
				return option;
			}
				
			template <typename T2>
			Option<T2> Bind(std::function<Either<None,Option<T2>>(T)> transform)
			{
				Option<T2> option = None();
				std::function<Either<None,Option<T2>>(T)> fn = [=](T t) { return transform(t); };
				Either<None, Option<T2>> either = value.Bind(fn);
				return either.Match([](None n){ return n;}, [](Option<T2> t2){ return t2;});				
			}
			
			T Match(std::function<T()> ifNone, std::function<T(T)> ifSome )
			{
				return value.Match([=](None n){return ifNone();}, [=](T t){return ifSome(t);});
			}
		};
		
}
