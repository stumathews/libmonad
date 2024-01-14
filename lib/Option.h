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
			Option<T2> Map(std::function<Option<T2>(T)> transform)
			{
				std::function<Option<T2>(T)> fn = [=](T t) -> Option<T2> { return transform(t); };
				std::function<Either<None, T2>(T)> fn2  = [=](T t ) -> Either<None,T2> { return ToEither<T2>(fn(t)); };

				Either<None, T2> either = value.Map(fn2);
				return ToOption(either);
			}

			template <typename T2>
			Option<T2> Bind(std::function<Either<None,Option<T2>>(T)> transform)
			{
				std::function<Either<None, T2>(T)> fn2 = [=](T t)
				{
					return ToEither(
							transform(t).Match(
								[&](None n){ return Option<T2>();},
								[&](Option<T2> t2) { return t2;}));
				};
				
				return ToOption(value.Bind(fn2));
			}
			
			T Match(std::function<T()> ifNone, std::function<T(T)> ifSome )
			{
				return value.Match(
					[=](None n){return ifNone();},
					[=](T t){return ifSome(t);});
			}

			void MatchVoid(const std::function<void(None)>& ifNone, std::function<void(T)> ifSome )
			{
				return value.MatchVoid(
					[=](None n){ ifNone(n); },
					[=](T t){ ifSome(t); });
			}

			template <typename T2>
			Option<T2> ToOption(Either<None, T2> either)
			{
				Option<T2> option = None();
				
				either.MatchVoid(
					[&](None n){ option = None(); }, 
					[&](T2 t2){ option = t2;});
				return option;
			}

			template <typename T2>
			Either<None, T2> ToEither(Option<T2> option)
			{
				Either<None,T2> either = None();

				option.MatchVoid(
					[&](None n){ either = n;},
					[&](T2 t) { either = t;});
				return either;
			}			
			
		};

		template <typename T>
		static Option<T> ToOption(T thing)
		{
			return Option<T>(thing);
		}		
		
}
