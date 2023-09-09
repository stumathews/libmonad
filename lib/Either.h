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

		/**
		 * \brief Transforms a right type value
		 * \tparam T type to transform to
		 * \param transform transformation function that transforms either right type from T to L
		 * \return Either if right type as R
		 */
		template <typename T>
		Either<L, T> Map(std::function<T(R)> transform);

		/**
		 * \brief Transforms a right type value
		 * \tparam T type to transform to
		 * \param transform transformation function that transforms either right type from T to L
		 * \return Either if right type as R
		 */
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
		
		/**
		 * \brief What right value to return
		 * \param ifLeft the right value to return if either contains left value
		 * \param ifRight the right value to return if either contains the right value
		 * \return right value
		 */
		R Match(std::function<R(L)> ifLeft, std::function<R(R)> ifRight);

		/**
		 * \brief what right value to return if either contains a left value
		 * \param ifLeft what right value to return if either contains a left value
		 * \return right value
		 */
		R IfLeft(std::function<R(L)> ifLeft);

		/**
		 * \brief Determine of either contains the left type value
		 * \return true if either contains left value
		 */
		bool IsLeft() const;

		/**
		 * \brief Determines of the either contains the right type value
		 * \return true if the either contains a right value 
		 */
		bool IsRight() const;

		/**
		 * \brief Determines of either is initialized or not
		 * \return true either not initialized - no value assigned to either
		 */
		bool IsBottom() const;

	private:
		void CheckIfInitialized() const;
		L leftValue;
		R rightValue;
		
		bool isLeft{};
		bool isBottom;
	};

	template <typename L, typename R>
	Either<L, R>::Either(L left)
	{
		isLeft = true;
		leftValue = left;
		isBottom = false;
	}

	template <typename L, typename R>
	Either<L, R>::Either(R right)
	{
		isLeft = false;
		rightValue = right;
		isBottom = false;		
	}

	template <typename L, typename R>
	Either<L, R>::Either()
	{
		isLeft = false;
		isBottom = true;
	}

	template <typename L, typename R>
	template <typename T>
	Either<L, T> Either<L, R>::Map(std::function<T(R)> transform)
	{
		CheckIfInitialized();
		if(isLeft)
		{
			return leftValue;
		}
		auto result = Either<L, T>(transform(rightValue));
		return result;
	}

	template <typename L, typename R>
	template <typename T>
	Either<L, T> Either<L, R>::Bind(std::function<Either<L,T>(R)> transform)
	{
		CheckIfInitialized();
		if(isLeft)
		{
			return leftValue;
		}
		return transform(rightValue);
	}	

	template <typename L, typename R>
	void Either<L, R>::CheckIfInitialized() const
	{
		if(isBottom) { throw std::exception("Either is not initialized. Assign it a value");}
	}

	template <typename L, typename R>
	R Either<L, R>::Match(std::function<R(L)> ifLeft, std::function<R(R)> ifRight )
	{
		CheckIfInitialized();
		return isLeft ? ifLeft(leftValue) : ifRight(rightValue);
	}

	template <typename L, typename R>
	L Either<L, R>::Match(std::function<L(L)> ifLeft, std::function<L(R)> ifRight)
	{
		CheckIfInitialized();
		return isLeft ? ifLeft(leftValue) : ifRight(rightValue);
	}

	template <typename L, typename R>
	R Either<L, R>::IfLeft(std::function<R(L)> ifLeft )
	{
		CheckIfInitialized();
		return isLeft ? ifLeft(leftValue) : rightValue;
	}

	template <typename L, typename R>
	L Either<L, R>::IfRight(std::function<L(R)> ifRight)
	{
		CheckIfInitialized();
		return isLeft ? leftValue : ifRight(rightValue);
	}

	template <typename L, typename R>
	bool Either<L, R>::IsLeft() const
	{
		return !isBottom && isLeft;
	}

	template <typename L, typename R>
	bool Either<L, R>::IsRight() const
	{
		return !isBottom && !isLeft;
	}

	template <typename L, typename R>
	bool Either<L, R>::IsBottom() const
	{
		return isBottom;
	}
}

