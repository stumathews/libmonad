#pragma once
#include <functional>

namespace libmonad
{
	template <typename L, typename R>
	class Either
	{
	public:
		Either(L left);
		Either(R right);
		Either();

		template <typename T>
		Either<L, T> Map(std::function<T(R)> bind);

		template <typename T>
		Either<L, T> Bind(std::function<Either<L,T>(R)> map);

		L Match(std::function<L(L)> ifLeft, std::function<L(R)> ifRight);
		L IfRight(std::function<L(R)> ifRight);
		
		void CheckIfInitialized() const;
		R Match(std::function<R(L)> ifLeft, std::function<R(R)> ifRight);
		R IfLeft(std::function<R(L)> ifLeft);
	private:
		L left;
		R right;
		bool IsLeft() const;
		bool isLeft;
		bool isBottom;
	};

	template <typename L, typename R>
	Either<L, R>::Either(L left)
	{
		isLeft = true;
		this->left = left;
		this->isBottom = false;
	}

	template <typename L, typename R>
	Either<L, R>::Either(R right)
	{
		isLeft = false;
		this->right = right;
		this->isBottom = false;
	}

	template <typename L, typename R>
	Either<L, R>::Either()
	{
		isBottom = true;
	}

	template <typename L, typename R>
	template <typename T>
	Either<L, T> Either<L, R>::Map(std::function<T(R)> bind)
	{
		CheckIfInitialized();
		if(isLeft)
		{
			return this;
		}
		auto result = Either<L, T>(bind(right));
		return result;
	}

	template <typename L, typename R>
	template <typename T>
	Either<L, T> Either<L, R>::Bind(std::function<Either<L,T>(R)> map)
	{
		CheckIfInitialized();
		if(isLeft)
		{
			return this;
		}
		return map(right);
	}

	

	template <typename L, typename R>
	void Either<L, R>::CheckIfInitialized() const
	{
		if(isBottom) { throw "Either is not initialized";}
	}

	template <typename L, typename R>
	R Either<L, R>::Match(std::function<R(L)> ifLeft, std::function<R(R)> ifRight )
	{
		CheckIfInitialized();
		return isLeft ? ifLeft(left) : ifRight(right);
	}

	template <typename L, typename R>
	L Either<L, R>::Match(std::function<L(L)> ifLeft, std::function<L(R)> ifRight)
	{
		CheckIfInitialized();
		return isLeft ? ifLeft(left) : ifRight(right);
	}

	template <typename L, typename R>
	R Either<L, R>::IfLeft(std::function<R(L)> ifLeft )
	{
		CheckIfInitialized();
		return isLeft ? ifLeft(left) : right;
	}

	template <typename L, typename R>
	L Either<L, R>::IfRight(std::function<L(R)> ifRight)
	{
		CheckIfInitialized();
		return isLeft ? left : ifRight(right);
	}


	template <typename L, typename R>
	bool Either<L, R>::IsLeft() const
	{
		return isLeft;
	}
	}

