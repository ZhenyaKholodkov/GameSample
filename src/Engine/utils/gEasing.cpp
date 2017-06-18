#include "gEasing.h"

#include "Types.h"
#include <math.h>

float GEasings::calculateValueWithEasing(int easingType, float time, float begin, float change, float duration)
{
	switch (easingType)
	{
	case NONE:
		return calculateValueWithNoneEasing(time, begin, change, duration);
		break;
	case QUAD_IN_EASING:
		return calculateValueWithQuadeIn(time, begin, change, duration);
		break;
	case QUAD_OUT_EASING:
		return calculateValueWithQuadeOut(time, begin, change, duration);
		break;
	case QUAD_INOUT_EASING:
		return calculateValueWithQuadeInOut(time, begin, change, duration);
		break;
	case BACK_IN_EASING:
		return calculateValueWithBackIn(time, begin, change, duration);
		break;
	case BACK_OUT_EASING:
		return calculateValueWithBackOut(time, begin, change, duration);
		break;
	case BACK_INOUT_EASING:
		return calculateValueWithBackInOut(time, begin, change, duration);
		break;
	case SINE_IN_EASING:
		return calculateValueWithSineIn(time, begin, change, duration);
		break;
	case SINE_OUT_EASING:
		return calculateValueWithSineOut(time, begin, change, duration);
		break;
	case SINE_INOUT_EASING:
		return calculateValueWithSineInOut(time, begin, change, duration);
		break;
	case CUBIC_IN_EASING:
		return calculateValueWithCubicIn(time, begin, change, duration);
		break;
	case CUBIC_OUT_EASING:
		return calculateValueWithCubicOut(time, begin, change, duration);
		break;
	case CUBIC_INOUT_EASING:
		return calculateValueWithCubicInOut(time, begin, change, duration);
		break;
	case QUART_IN_EASING:
		return calculateValueWithQuartIn(time, begin, change, duration);
		break;
	case QUART_OUT_EASING:
		return calculateValueWithQuartOut(time, begin, change, duration);
		break;
	case QUART_INOUT_EASING:
		return calculateValueWithQuartInOut(time, begin, change, duration);
		break;
	case QUINT_IN_EASING:
		return calculateValueWithQuintIn(time, begin, change, duration);
		break;
	case QUINT_OUT_EASING:
		return calculateValueWithQuintOut(time, begin, change, duration);
		break;
	case QUINT_INOUT_EASING:
		return calculateValueWithQuintInOut(time, begin, change, duration);
		break;
	case EXPO_IN_EASING:
		return calculateValueWithExpoIn(time, begin, change, duration);
		break;
	case EXPO_OUT_EASING:
		return calculateValueWithExpoOut(time, begin, change, duration);
		break;
	case EXPO_INOUT_EASING:
		return calculateValueWithExpoInOut(time, begin, change, duration);
		break;
	case ELASTIC_IN_EASING:
		return calculateValueWithElasticIn(time, begin, change, duration);
		break;
	case ELASTIC_OUT_EASING:
		return calculateValueWithElasticOut(time, begin, change, duration);
		break;
	case ELASTIC_INOUT_EASING:
		return calculateValueWithElasticInOut(time, begin, change, duration);
		break;
	case BOUNCE_IN_EASING:
		return calculateValueWithElasticIn(time, begin, change, duration);
		break;
	case BOUNCE_OUT_EASING:
		return calculateValueWithElasticOut(time, begin, change, duration);
		break;
	case BOUNCE_INOUT_EASING:
		return calculateValueWithElasticInOut(time, begin, change, duration);
		break;
	case CIRC_IN_EASING:
		return calculateValueWithCircIn(time, begin, change, duration);
		break;
	case CIRC_OUT_EASING:
		return calculateValueWithCircOut(time, begin, change, duration);
		break;
	case CIRC_INOUT_EASING:
		return calculateValueWithCircInOut(time, begin, change, duration);
		break;
	}

	return 0;
}
float GEasings::calculateValueWithNoneEasing(float time, float begin, float change, float duration)
{
	return change*(time /= duration) + begin;
}

//* <! Easing - Quade !> *//
float GEasings::calculateValueWithQuadeIn(float time, float begin, float change, float duration)
{
	return change*(time /= duration)*time + begin;
}

float GEasings::calculateValueWithQuadeOut(float time, float begin, float change, float duration)
{
	return -change*(time /= duration)*(time - 2) + begin;
}

float GEasings::calculateValueWithQuadeInOut(float time, float begin, float change, float duration)
{
	if ((time /= duration / 2) < 1)
	{
		return ((change / 2)*(time*time)) + begin;
	}
	else
	{
		return -change / 2 * (((time - 2)*(--time)) - 1) + begin;
	}
}

//* <! Easing - Back !> *//
float GEasings::calculateValueWithBackIn(float time, float begin, float change, float duration)
{
	float s = 1.70158f;
	float postFix = time /= duration;
	return change*(postFix)*time*((s + 1)*time - s) + begin;
}
float GEasings::calculateValueWithBackOut(float time, float begin, float change, float duration)
{
	float s = 1.70158f;
	return change*((time = time / duration - 1)*time*((s + 1)*time + s) + 1) + begin;
}
float GEasings::calculateValueWithBackInOut(float time, float begin, float change, float duration)
{
	float s = 1.70158f;
	if ((time /= duration / 2) < 1) return change / 2 * (time*time*(((s *= (1.525f)) + 1)*time - s)) + begin;
	float postFix = time -= 2;
	return change / 2 * ((postFix)*time*(((s *= (1.525f)) + 1)*time + s) + 2) + begin;
}


//* <! Easing - Sine !> *//
float GEasings::calculateValueWithSineIn(float time, float begin, float change, float duration)
{
	return (float)(-change * cos(time / duration * (PI / 2)) + change + begin);
}
float GEasings::calculateValueWithSineOut(float time, float begin, float change, float duration)
{
	return (float)(change * sin(time / duration * (PI / 2)) + begin);
}
float GEasings::calculateValueWithSineInOut(float time, float begin, float change, float duration)
{
	return (float)(-change / 2 * (cos(PI*time / duration) - 1) + begin);
}

//* <! Easing - Cubic !> *//
float GEasings::calculateValueWithCubicIn(float time, float begin, float change, float duration)
{
	return (float)(change*(time /= duration)*time*time + begin);
}
float GEasings::calculateValueWithCubicOut(float time, float begin, float change, float duration)
{
	return (float)(change*((time = time / duration - 1)*time*time + 1) + begin);
}
float GEasings::calculateValueWithCubicInOut(float time, float begin, float change, float duration)
{
	if ((time /= duration / 2) < 1) {
		return (float)(change / 2 * time*time*time + begin);
	}
	return (float)(change / 2 * ((time -= 2)*time*time + 2) + begin);
}

//* <! Easing - Quart !> *//
float GEasings::calculateValueWithQuartIn(float time, float begin, float change, float duration)
{
	return (float)(change*(time /= duration)*time*time*time + begin);
}
float GEasings::calculateValueWithQuartOut(float time, float begin, float change, float duration)
{
	return (float)(-change * ((time = time / duration - 1)*time*time*time - 1) + begin);
}
float GEasings::calculateValueWithQuartInOut(float time, float begin, float change, float duration)
{
	if ((time /= duration / 2) < 1) {
		return (float)(change / 2 * time*time*time*time + begin);
	}
	return -change / 2 * ((time -= 2)*time*time*time - 2) + begin;
}


//* <! Easing - Quint !> *//
float GEasings::calculateValueWithQuintIn(float time, float begin, float change, float duration)
{
	return (float)(change*(time /= duration)*time*time*time*time + begin);
}
float GEasings::calculateValueWithQuintOut(float time, float begin, float change, float duration)
{
	return (float)(change*((time = time / duration - 1)*time*time*time*time + 1) + begin);
}
float GEasings::calculateValueWithQuintInOut(float time, float begin, float change, float duration) {
	if ((time /= duration / 2) < 1) {
		return (float)(change / 2 * time*time*time*time*time + begin);
	}
	return (float)(change / 2 * ((time -= 2)*time*time*time*time + 2) + begin);
}


//* <! Easing - Expo !> *//
float GEasings::calculateValueWithExpoIn(float time, float begin, float change, float duration)
{
	return (float)((time == 0) ? begin : change * pow(2, 10 * (time / duration - 1)) + begin);
}
float GEasings::calculateValueWithExpoOut(float time, float begin, float change, float duration)
{
	return (float)((time == duration) ? begin + change : change * (-pow(2, -10 * time / duration) + 1) + begin);
}
float GEasings::calculateValueWithExpoInOut(float time, float begin, float change, float duration)
{
	if (time == 0) {
		return begin;
	}
	else if (time == duration) {
		return begin + change;
	}
	else if ((time /= duration / 2) < 1) {
		return (float)(change / 2 * pow(2, 10 * (time - 1)) + begin);
	}
	else {
		return (float)(change / 2 * (-pow(2, -10 * --time) + 2) + begin);
	}
}

//* <! Easing - Elastic !> *//
float GEasings::calculateValueWithElasticIn(float time, float begin, float change, float duration)
{
	if (time == 0) {
		return begin;
	}
	else if ((time /= duration) == 1) {
		return begin + change;
	}

	float p = duration*.3f;
	float a = change;
	float s = p / 4;
	float postFix = (float)(a*pow(2, 10 * (time -= 1))); // this is a fix, again, with post-increment operators
	return (float)(-(postFix * sin((time*duration - s)*(2 * PI) / p)) + begin);
}

float GEasings::calculateValueWithElasticOut(float time, float begin, float change, float duration)
{
	if (time == 0) {
		return begin;
	}
	else if ((time /= duration) == 1) {
		return begin + change;
	}
	float p = duration*.3f;
	float a = change;
	float s = p / 4;
	return (float)((a*pow(2, -10 * time) * sin((time*duration - s)*(2 * PI) / p) + change + begin));
}

float GEasings::calculateValueWithElasticInOut(float time, float begin, float change, float duration)
{
	if (time == 0) {
		return begin;
	}
	else if ((time /= duration / 2) == 2) {
		return begin + change;
	}
	float p = duration*(.3f*1.5f);
	float a = change;
	float s = p / 4;

	if (time < 1) {
		float postFix = (float)(a*pow(2, 10 * (time -= 1))); // postIncrement is evil
		return (float)(-.5f*(postFix* sin((time*duration - s)*(2 * PI) / p)) + begin);
	}
	float postFix = (float)(a*pow(2, -10 * (time -= 1))); // postIncrement is evil
	return (float)(postFix * sin((time*duration - s)*(2 * PI) / p)*.5f + change + begin);
}


//* <! Easing - Bounce !> *//
float GEasings::calculateValueWithBounceIn(float time, float begin, float change, float duration)
{
	return change - calculateValueWithBounceOut(duration - time, 0, change, duration) + begin;
}
float GEasings::calculateValueWithBounceOut(float time, float begin, float change, float duration)
{
	if ((time /= duration) < (1 / 2.75f)) {
		return change*(7.5625f*time*time) + begin;
	}
	else if (time < (2 / 2.75f)) {
		float postFix = time -= (1.5f / 2.75f);
		return change*(7.5625f*(postFix)*time + .75f) + begin;
	}
	else if (time < (2.5 / 2.75)) {
		float postFix = time -= (2.25f / 2.75f);
		return change*(7.5625f*(postFix)*time + .9375f) + begin;
	}
	else {
		float postFix = time -= (2.625f / 2.75f);
		return change*(7.5625f*(postFix)*time + .984375f) + begin;
	}
}
float GEasings::calculateValueWithBounceInOut(float time, float begin, float change, float duration)
{
	if (time < duration / 2) {
		return calculateValueWithBounceIn(time * 2, 0, change, duration) * .5f + begin;
	}
	else {
		return calculateValueWithBounceOut(time * 2 - duration, 0, change, duration) * .5f + change*.5f + begin;
	}
}

//* <! Easing - Circ !> *//
float GEasings::calculateValueWithCircIn(float time, float begin, float change, float duration)
{
	return (float)(-change * (sqrt(1 - (time /= duration)*time) - 1) + begin);
}
float GEasings::calculateValueWithCircOut(float time, float begin, float change, float duration)
{
	return (float)(change * sqrt(1 - (time = time / duration - 1)*time) + begin);
}
float GEasings::calculateValueWithCircInOut(float time, float begin, float change, float duration)
{
	if ((time /= duration / 2) < 1) {
		return (float)(-change / 2 * (sqrt(1 - time*time) - 1) + begin);
	}
	else {
		return (float)(change / 2 * (sqrt(1 - time*(time -= 2)) + 1) + begin);
	}
}