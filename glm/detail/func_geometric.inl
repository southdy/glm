/// @ref core
/// @file glm/detail/func_geometric.inl

#include "func_exponential.hpp"
#include "func_common.hpp"
#include "type_vec2.hpp"
#include "type_vec4.hpp"
#include "type_float.hpp"

namespace glm{
namespace detail
{
	template <template <int, typename, precision> class vecType, int D, typename T, precision P, bool Aligned>
	struct compute_length
	{
		GLM_FUNC_QUALIFIER static T call(vecType<D, T, P> const & v)
		{
			return sqrt(dot(v, v));
		}
	};

	template <template <int, typename, precision> class vecType, int D, typename T, precision P, bool Aligned>
	struct compute_distance
	{
		GLM_FUNC_QUALIFIER static T call(vecType<D, T, P> const & p0, vecType<D, T, P> const & p1)
		{
			return length(p1 - p0);
		}
	};

	template <typename V, typename T, bool Aligned>
	struct compute_dot{};

	template <typename T, precision P, bool Aligned>
	struct compute_dot<vec<1, T, P>, T, Aligned>
	{
		GLM_FUNC_QUALIFIER static T call(vec<1, T, P> const & a, vec<1, T, P> const & b)
		{
			return a.x * b.x;
		}
	};

	template <typename T, precision P, bool Aligned>
	struct compute_dot<vec<2, T, P>, T, Aligned>
	{
		GLM_FUNC_QUALIFIER static T call(vec<2, T, P> const & a, vec<2, T, P> const & b)
		{
			vec<2, T, P> tmp(a * b);
			return tmp.x + tmp.y;
		}
	};

	template <typename T, precision P, bool Aligned>
	struct compute_dot<vec<3, T, P>, T, Aligned>
	{
		GLM_FUNC_QUALIFIER static T call(vec<3, T, P> const & a, vec<3, T, P> const & b)
		{
			tvec3<T, P> tmp(a * b);
			return tmp.x + tmp.y + tmp.z;
		}
	};

	template <typename T, precision P, bool Aligned>
	struct compute_dot<vec<4, T, P>, T, Aligned>
	{
		GLM_FUNC_QUALIFIER static T call(vec<4, T, P> const & a, vec<4, T, P> const & b)
		{
			vec<4, T, P> tmp(a * b);
			return (tmp.x + tmp.y) + (tmp.z + tmp.w);
		}
	};

	template <typename T, precision P, bool Aligned>
	struct compute_cross
	{
		GLM_FUNC_QUALIFIER static tvec3<T, P> call(tvec3<T, P> const & x, tvec3<T, P> const & y)
		{
			GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'cross' accepts only floating-point inputs");

			return tvec3<T, P>(
				x.y * y.z - y.y * x.z,
				x.z * y.x - y.z * x.x,
				x.x * y.y - y.x * x.y);
		}
	};

	template <int D, typename T, precision P, template <int, typename, precision> class vecType, bool Aligned>
	struct compute_normalize
	{
		GLM_FUNC_QUALIFIER static vecType<D, T, P> call(vecType<D, T, P> const & v)
		{
			GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'normalize' accepts only floating-point inputs");

			return v * inversesqrt(dot(v, v));
		}
	};

	template <int D, typename T, precision P, template <int, typename, precision> class vecType, bool Aligned>
	struct compute_faceforward
	{
		GLM_FUNC_QUALIFIER static vecType<D, T, P> call(vecType<D, T, P> const & N, vecType<D, T, P> const & I, vecType<D, T, P> const & Nref)
		{
			GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'normalize' accepts only floating-point inputs");

			return dot(Nref, I) < static_cast<T>(0) ? N : -N;
		}
	};

	template <int D, typename T, precision P, template <int, typename, precision> class vecType, bool Aligned>
	struct compute_reflect
	{
		GLM_FUNC_QUALIFIER static vecType<D, T, P> call(vecType<D, T, P> const & I, vecType<D, T, P> const & N)
		{
			return I - N * dot(N, I) * static_cast<T>(2);
		}
	};

	template <int D, typename T, precision P, template <int, typename, precision> class vecType, bool Aligned>
	struct compute_refract
	{
		GLM_FUNC_QUALIFIER static vecType<D, T, P> call(vecType<D, T, P> const & I, vecType<D, T, P> const & N, T eta)
		{
			T const dotValue(dot(N, I));
			T const k(static_cast<T>(1) - eta * eta * (static_cast<T>(1) - dotValue * dotValue));
			return (eta * I - (eta * dotValue + std::sqrt(k)) * N) * static_cast<T>(k >= static_cast<T>(0));
		}
	};
}//namespace detail

	// length
	template <typename genType>
	GLM_FUNC_QUALIFIER genType length(genType x)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genType>::is_iec559, "'length' accepts only floating-point inputs");

		return abs(x);
	}

	template <int D, typename T, precision P, template <int, typename, precision> class vecType>
	GLM_FUNC_QUALIFIER T length(vecType<D, T, P> const & v)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'length' accepts only floating-point inputs");

		return detail::compute_length<vecType, D, T, P, detail::is_aligned<P>::value>::call(v);
	}

	// distance
	template <typename genType>
	GLM_FUNC_QUALIFIER genType distance(genType const & p0, genType const & p1)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genType>::is_iec559, "'distance' accepts only floating-point inputs");

		return length(p1 - p0);
	}

	template <int D, typename T, precision P, template <int, typename, precision> class vecType>
	GLM_FUNC_QUALIFIER T distance(vecType<D, T, P> const & p0, vecType<D, T, P> const & p1)
	{
		return detail::compute_distance<vecType, D, T, P, detail::is_aligned<P>::value>::call(p0, p1);
	}

	// dot
	template <typename T>
	GLM_FUNC_QUALIFIER T dot(T x, T y)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'dot' accepts only floating-point inputs");
		return x * y;
	}

	template <int D, typename T, precision P>
	GLM_FUNC_QUALIFIER T dot(vec<D, T, P> const & x, vec<D, T, P> const & y)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'dot' accepts only floating-point inputs");
		return detail::compute_dot<vec<D, T, P>, T, detail::is_aligned<P>::value>::call(x, y);
	}

	template <typename T, precision P>
	GLM_FUNC_QUALIFIER T dot(tquat<T, P> const & x, tquat<T, P> const & y)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'dot' accepts only floating-point inputs");
		return detail::compute_dot<tquat<T, P>, T, detail::is_aligned<P>::value>::call(x, y);
	}

	// cross
	template <typename T, precision P>
	GLM_FUNC_QUALIFIER tvec3<T, P> cross(tvec3<T, P> const & x, tvec3<T, P> const & y)
	{
		return detail::compute_cross<T, P, detail::is_aligned<P>::value>::call(x, y);
	}

	// normalize
	template <typename genType>
	GLM_FUNC_QUALIFIER genType normalize(genType const & x)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genType>::is_iec559, "'normalize' accepts only floating-point inputs");

		return x < genType(0) ? genType(-1) : genType(1);
	}

	template <int D, typename T, precision P, template <int, typename, precision> class vecType>
	GLM_FUNC_QUALIFIER vecType<D, T, P> normalize(vecType<D, T, P> const & x)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'normalize' accepts only floating-point inputs");

		return detail::compute_normalize<D, T, P, vecType, detail::is_aligned<P>::value>::call(x);
	}

	// faceforward
	template <typename genType>
	GLM_FUNC_QUALIFIER genType faceforward(genType const & N, genType const & I, genType const & Nref)
	{
		return dot(Nref, I) < static_cast<genType>(0) ? N : -N;
	}

	template <int D, typename T, precision P, template <int, typename, precision> class vecType>
	GLM_FUNC_QUALIFIER vecType<D, T, P> faceforward(vecType<D, T, P> const & N, vecType<D, T, P> const & I, vecType<D, T, P> const & Nref)
	{
		return detail::compute_faceforward<D, T, P, vecType, detail::is_aligned<P>::value>::call(N, I, Nref);
	}

	// reflect
	template <typename genType>
	GLM_FUNC_QUALIFIER genType reflect(genType const & I, genType const & N)
	{
		return I - N * dot(N, I) * genType(2);
	}

	template <int D, typename T, precision P, template <int, typename, precision> class vecType>
	GLM_FUNC_QUALIFIER vecType<D, T, P> reflect(vecType<D, T, P> const & I, vecType<D, T, P> const & N)
	{
		return detail::compute_reflect<D, T, P, vecType, detail::is_aligned<P>::value>::call(I, N);
	}

	// refract
	template <typename genType>
	GLM_FUNC_QUALIFIER genType refract(genType const & I, genType const & N, genType eta)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genType>::is_iec559, "'refract' accepts only floating-point inputs");
		genType const dotValue(dot(N, I));
		genType const k(static_cast<genType>(1) - eta * eta * (static_cast<genType>(1) - dotValue * dotValue));
		return (eta * I - (eta * dotValue + sqrt(k)) * N) * static_cast<genType>(k >= static_cast<genType>(0));
	}

	template <int D, typename T, precision P, template <int, typename, precision> class vecType>
	GLM_FUNC_QUALIFIER vecType<D, T, P> refract(vecType<D, T, P> const & I, vecType<D, T, P> const & N, T eta)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'refract' accepts only floating-point inputs");
		return detail::compute_refract<D, T, P, vecType, detail::is_aligned<P>::value>::call(I, N, eta);
	}
}//namespace glm

#if GLM_ARCH != GLM_ARCH_PURE && GLM_HAS_UNRESTRICTED_UNIONS
#	include "func_geometric_simd.inl"
#endif
