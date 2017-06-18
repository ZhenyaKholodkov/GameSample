
#ifndef _GEASINGS_H
#define _GEASINGS_H
class GEasings         // !< realization of GEasings
{
public:
	~GEasings() {};

	typedef enum EasingType    //!< type of Easing
	{
		NONE,
		QUAD_IN_EASING,
		QUAD_OUT_EASING,
		QUAD_INOUT_EASING,
		BACK_IN_EASING,
		BACK_OUT_EASING,
		BACK_INOUT_EASING,
		SINE_IN_EASING,
		SINE_OUT_EASING,
		SINE_INOUT_EASING,
		CUBIC_IN_EASING,
		CUBIC_OUT_EASING,
		CUBIC_INOUT_EASING,
		QUART_IN_EASING,
		QUART_OUT_EASING,
		QUART_INOUT_EASING,
		QUINT_IN_EASING,
		QUINT_OUT_EASING,
		QUINT_INOUT_EASING,
		EXPO_IN_EASING,
		EXPO_OUT_EASING,
		EXPO_INOUT_EASING,
		ELASTIC_IN_EASING,
		ELASTIC_OUT_EASING,
		ELASTIC_INOUT_EASING,
		BOUNCE_IN_EASING,
		BOUNCE_OUT_EASING,
		BOUNCE_INOUT_EASING,
		CIRC_IN_EASING,
		CIRC_OUT_EASING,
		CIRC_INOUT_EASING,
	}EasingType;

	static float calculateValueWithEasing(int easingType, float time, float begin, float change, float duration); //!< function that call another function in the order of the easing type

private:
	GEasings() {};
	//* <!NONE EAING!>*//
	static float calculateValueWithNoneEasing(float time, float begin, float change, float duration);

	//* <! Easing - Quade !> *//
	static float calculateValueWithQuadeIn(float time, float begin, float change, float duration);
	static float calculateValueWithQuadeOut(float time, float begin, float change, float duration);
	static float calculateValueWithQuadeInOut(float time, float begin, float change, float duration);

	//* <! Easing - Back !> *//
	static float calculateValueWithBackIn(float time, float begin, float change, float duration);
	static float calculateValueWithBackOut(float time, float begin, float change, float duration);
	static float calculateValueWithBackInOut(float time, float begin, float change, float duration);

	//* <! Easing - Sine !> *//
	static float calculateValueWithSineIn(float time, float begin, float change, float duration);
	static float calculateValueWithSineOut(float time, float begin, float change, float duration);
	static float calculateValueWithSineInOut(float time, float begin, float change, float duration);

	//* <! Easing - Cubic !> *//
	static float calculateValueWithCubicIn(float time, float begin, float change, float duration);
	static float calculateValueWithCubicOut(float time, float begin, float change, float duration);
	static float calculateValueWithCubicInOut(float time, float begin, float change, float duration);

	//* <! Easing - Quart !> *//
	static float calculateValueWithQuartIn(float time, float begin, float change, float duration);
	static float calculateValueWithQuartOut(float time, float begin, float change, float duration);
	static float calculateValueWithQuartInOut(float time, float begin, float change, float duration);

	//* <! Easing - Quint !> *//
	static float calculateValueWithQuintIn(float time, float begin, float change, float duration);
	static float calculateValueWithQuintOut(float time, float begin, float change, float duration);
	static float calculateValueWithQuintInOut(float time, float begin, float change, float duration);

	//* <! Easing - Expo !> *//
	static float calculateValueWithExpoIn(float time, float begin, float change, float duration);
	static float calculateValueWithExpoOut(float time, float begin, float change, float duration);
	static float calculateValueWithExpoInOut(float time, float begin, float change, float duration);

	//* <! Easing - Elastic !> *//
	static float calculateValueWithElasticIn(float time, float begin, float change, float duration);
	static float calculateValueWithElasticOut(float time, float begin, float change, float duration);
	static float calculateValueWithElasticInOut(float time, float begin, float change, float duration);

	//* <! Easing - Bounce !> *//
	static float calculateValueWithBounceIn(float time, float begin, float change, float duration);
	static float calculateValueWithBounceOut(float time, float begin, float change, float duration);
	static float calculateValueWithBounceInOut(float time, float begin, float change, float duration);

	//* <! Easing - Circ !> *//
	static float calculateValueWithCircIn(float time, float begin, float change, float duration);
	static float calculateValueWithCircOut(float time, float begin, float change, float duration);
	static float calculateValueWithCircInOut(float time, float begin, float change, float duration);
};
#endif