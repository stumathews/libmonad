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
			T some {};			

		public:
			
			Option(T value): some(std::move(value)) { }
			
			Option(None none = {}): isNone(true) { }
			
			bool IsNone() const { return isNone; }

			bool IsSome() const { return !isNone; }

			template <typename T2>
			Option<T2> Map(std::function<T2(T)> transform) { return isNone ? Option<T2>() : transform(some); }
				
			template <typename T2>
			Option<T2> Bind(std::function<Option<T2>(T)> transform) { return isNone ? Option<T2>() : transform(some); }
			
			template <typename T2>
			T2 Match(std::function<T2()> ifNone, std::function<T2(T)> ifSome ) { return isNone ? ifNone() : ifSome(some); }
		};
}
