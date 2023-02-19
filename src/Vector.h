#pragma once
#include <cmath>

namespace OpenGLRenderer {

	template <typename T, int S>
	class VectorHelper
	{
	public:
		VectorHelper(T d[S])
		{
			for (int i = 0; i < S; i++)
				data[i] = d[i];
		}
		VectorHelper()
		{
			for (int i = 0; i < S; i++)
				data[i] = T();
		}
		VectorHelper(const VectorHelper<T, S>& vec)
		{
			for (int i = 0; i < S; i++)
				data[i] = vec.data[i];
		}
		VectorHelper<T, S>& operator =(const VectorHelper<T, S>& vec)
		{
			for (int i = 0; i < S; i++)
				data[i] = vec.data[i];
			return *this;
		}

		VectorHelper<T, S>& operator +(const VectorHelper<T, S>& vec) const
		{
			VectorHelper<T, S> retVec(this);
			for (int i = 0; i < S; i++)
				retVec.data[i] += vec.data[i];
			return retVec;
		}
		VectorHelper<T, S>& operator -(const VectorHelper<T, S>& vec) const
		{
			VectorHelper<T, S> retVec(this);
			for (int i = 0; i < S; i++)
				retVec.data[i] -= vec.data[i];
			return retVec;
		}

		VectorHelper<T, S>& operator =(VectorHelper<T, S>& vec)
		{
			for (int i = 0; i < S; i++)
				data[i] = vec.data[i];
			return *this;
		}

		VectorHelper<T, S>& operator +(VectorHelper<T, S>& vec) const
		{
			VectorHelper<T, S> retVec(this);
			for (int i = 0; i < S; i++)
				retVec.data[i] += vec.data[i];
			return retVec;
		}
		VectorHelper<T, S>& operator -(VectorHelper<T, S>& vec) const
		{
			VectorHelper<T, S> retVec(this);
			for (int i = 0; i < S; i++)
				retVec.data[i] -= vec.data[i];
			return retVec;
		}

		VectorHelper<T, S>& operator *(T scalar) const
		{
			VectorHelper<T, S> retVec(this);
			for (int i = 0; i < S; i++)
				retVec.data[i] *= scalar;
			return retVec;
		}
		VectorHelper<T, S>& operator /(T scalar) const
		{
			VectorHelper<T, S> retVec(this);
			for (int i = 0; i < S; i++)
				retVec.data[i] /= scalar;
			return retVec;
		}

	protected:
		T data[S];
	};

	template<typename T, int S>
	class Vector : public VectorHelper<T, S>
	{
	public:
		using VectorHelper<T, S>::VectorHelper;
		using VectorHelper<T, S>::operator*;
		using VectorHelper<T, S>::operator/;
		using VectorHelper<T, S>::operator+;
		using VectorHelper<T, S>::operator-;
		using VectorHelper<T, S>::operator=;
	};

	template<typename T>
	class Vec2T : public VectorHelper<T, 2>
	{
	public:
		Vec2T(T px, T py)
		{
			data[0] = px;
			data[1] = py;
		}
		Vec2T<T>& operator =(const Vec2T<T>& vec)
		{
			for (int i = 0; i < 2; i++)
				data[i] = vec.data[i];
			x = data[0];
			y = data[1];
			return *this;
		}
		void RotateAroundCenter(float rotation, const Vec2T<T>& centerOfRotation)
		{
			float angle = std::atan2(-centerOfRotation.y, -centerOfRotation.x);
			double distFromCent = sqrt(centerOfRotation.x * centerOfRotation.x + centerOfRotation.y * centerOfRotation.y);
			data[0] += centerOfRotation.x + distFromCent * std::cos(angle + rotation * 0.0174532925);
			data[1] += centerOfRotation.y + distFromCent * std::sin(angle + rotation * 0.0174532925);
		}

		using VectorHelper<T, 2>::VectorHelper;
		using VectorHelper<T, 2>::operator*;
		using VectorHelper<T, 2>::operator/;
		using VectorHelper<T, 2>::operator+;
		using VectorHelper<T, 2>::operator-;
		using VectorHelper<T, 2>::operator=;
		using VectorHelper<T, 2>::data;

		T& x = data[0];
		T& y = data[1];
	};
	template<typename T>
	class Vec3T : public VectorHelper<T, 3>
	{
	public:
		Vec3T(T px, T py, T pz)
		{
			data[0] = px;
			data[1] = py;
			data[2] = pz;
		}

		Vec3T<T>& operator =(const Vec3T<T>& vec)
		{
			for (int i = 0; i < 3; i++)
				data[i] = vec.data[i];
			x = data[0];
			y = data[1];
			z = data[2];
			return *this;
		}

		void RotateAroundCenter(float rotation, const Vec2T<T>& centerOfRotation)
		{
			centerOfRotation.x -= data[0];
			centerOfRotation.y -= data[1];
			float angle = std::atan2(-centerOfRotation.y, -centerOfRotation.x);
			double distFromCent = sqrt(centerOfRotation.x * centerOfRotation.x + centerOfRotation.y * centerOfRotation.y);
			data[0] += centerOfRotation.x + distFromCent * std::cos(angle + rotation * 0.0174532925);
			data[1] += centerOfRotation.y + distFromCent * std::sin(angle + rotation * 0.0174532925);
		}

		using VectorHelper<T, 3>::VectorHelper;
		using VectorHelper<T, 3>::operator*;
		using VectorHelper<T, 3>::operator/;
		using VectorHelper<T, 3>::operator+;
		using VectorHelper<T, 3>::operator-;
		using VectorHelper<T, 3>::operator=;
		using VectorHelper<T, 3>::data;

		T& x = data[0];
		T& y = data[1];
		T& z = data[2];
	};
	template<typename T>
	class Vec4T : public VectorHelper<T, 4>
	{
	public:
		Vec4T(T px, T py, T pz, T pw)
		{
			data[0] = px;
			data[1] = py;
			data[2] = pz;
			data[3] = pw;
		}

		Vec4T<T>& operator =(const Vec4T<T>& vec)
		{
			for (int i = 0; i < 4; i++)
				data[i] = vec.data[i];
			x = data[0];
			y = data[1];
			z = data[2];
			w = data[3];
			return *this;
		}

		using VectorHelper<T, 4>::VectorHelper;
		using VectorHelper<T, 4>::operator*;
		using VectorHelper<T, 4>::operator/;
		using VectorHelper<T, 4>::operator+;
		using VectorHelper<T, 4>::operator-;
		using VectorHelper<T, 4>::operator=;
		using VectorHelper<T, 4>::data;

		T& x = data[0];
		T& y = data[1];
		T& z = data[2];
		T& w = data[3];
	};

	using Vec2 = Vec2T<float>;
	using Vec3 = Vec3T<float>;
	using Vec4 = Vec4T<float>;
	using Vec2i = Vec2T<int>;
	using Vec3i = Vec3T<int>;
	using Vec4i = Vec4T<int>;
}