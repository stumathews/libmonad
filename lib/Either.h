#pragma once
#include <functional>

namespace libmonad
{
	/**
	 * \brief An Either can contain either Left type or a Right type
	 * \tparam L Left type 
	 * \tparam R Right type
	 */
	template <typename L, typename R>
	class Either
	{
	public:
		/**
		 * \brief Initialize either with left type value
		 * \param left value
		 */
		Either(L left);

		/**
		 * \brief Initialize either with right type value
		 * \param right value
		 */
		Either(R right);

		/**
		 * \brief Initialize either with no value
		 */
		Either();

		template <typename T>
		Either<L, T> Map(std::function<T(R)> transform);

		template <typename T>
		Either<L, T> Bind(std::function<Either<L,T>(R)> transform);

		/**
		 * \brief What left value to return 
		 * \param ifLeft what to return if either contains left value
		 * \param ifRight what to return if either contains right value
		 * \return left value
		 */
		L Match(std::function<L(L)> ifLeft, std::function<L(R)> ifRight);

		/**
		 * \brief what left value to return if either contains right value
		 * \param ifRight value to return if either contains right value
		 * \return left value
		 */
		L IfRight(std::function<L(R)> ifRight);
		
		void CheckIfInitialized() const;
		/**
		 * \brief What right value to return
		 * \param ifLeft the right value to return if either contains left value
		 * \param ifRight the right vlaue to return if either contains the right value
		 * \return right value
		 */
		R Match(std::function<R(L)> ifLeft, std::function<R(R)> ifRight);

		/**
		 * \brief what right value to return if either contains a left value
		 * \param ifLeft what right value to return if either contains a left value
		 * \return right value
		 */
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
	Either<L, T> Either<L, R>::Map(std::function<T(R)> transform)
	{
		CheckIfInitialized();
		if(isLeft)
		{
			return left;
		}
		auto result = Either<L, T>(transform(right));
		return result;
	}

	template <typename L, typename R>
	template <typename T>
	Either<L, T> Either<L, R>::Bind(std::function<Either<L,T>(R)> transform)
	{
		CheckIfInitialized();
		if(isLeft)
		{
			return left;
		}
		return transform(right);
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

