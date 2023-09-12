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
			Option<T2> Map(std::function<T2(T)> transform)
			{
				std::function<T2(T)> fn = [=](T t) -> T2 { return transform(t);};
				Either<None, T2> either = value.Map(fn);
				
				return ToOption(either);
			}
				
			template <typename T2>
			Option<T2> Bind(std::function<Option<T2>(T)> transform)
			{
				Option<T2> option = None();

				value.MatchVoid( [&](None n)
				{
					option = None();
				}, [&](T t)
				{
					option = transform(t);
				});
								
				return option;
			}
			
			T Match(std::function<T()> ifNone, std::function<T(T)> ifSome )
			{
				return value.Match([=](None n){return ifNone();}, [=](T t){return ifSome(t);});
			}
		};
		
}
