#include "Types.h"

void IGMatrix::setIdentity()
{
	m[0][0] = m[1][1] = m[2][2] = 1.0f;
	m[0][1] = m[0][2] = m[1][0] = m[1][2] = m[2][0] = m[2][1] = 0.0f;
	t.x = t.y = t.z = 0.0f;
}

IGMatrix IGMatrix::matMul(IGMatrix const & A, IGMatrix const & B)
{
	static IGMatrix res;

	res.m[0][0] = A.m[0][0] * B.m[0][0] + A.m[0][1] * B.m[1][0] + A.m[0][2] * B.m[2][0];
	res.m[0][1] = A.m[0][0] * B.m[0][1] + A.m[0][1] * B.m[1][1] + A.m[0][2] * B.m[2][1];
	res.m[0][2] = A.m[0][0] * B.m[0][2] + A.m[0][1] * B.m[1][2] + A.m[0][2] * B.m[2][2];

	res.m[1][0] = A.m[1][0] * B.m[0][0] + A.m[1][1] * B.m[1][0] + A.m[1][2] * B.m[2][0];
	res.m[1][1] = A.m[1][0] * B.m[0][1] + A.m[1][1] * B.m[1][1] + A.m[1][2] * B.m[2][1];
	res.m[1][2] = A.m[1][0] * B.m[0][2] + A.m[1][1] * B.m[1][2] + A.m[1][2] * B.m[2][2];

	res.m[2][0] = A.m[2][0] * B.m[0][0] + A.m[2][1] * B.m[1][0] + A.m[2][2] * B.m[2][0];
	res.m[2][1] = A.m[2][0] * B.m[0][1] + A.m[2][1] * B.m[1][1] + A.m[2][2] * B.m[2][1];
	res.m[2][2] = A.m[2][0] * B.m[0][2] + A.m[2][1] * B.m[1][2] + A.m[2][2] * B.m[2][2];


	res.t.x = ((B.m[0][0] * A.t.x) + (B.m[1][0] * A.t.y) + (B.m[2][0] * A.t.z)) + B.t.x;
	res.t.y = ((B.m[0][1] * A.t.x) + (B.m[1][1] * A.t.y) + (B.m[2][1] * A.t.z)) + B.t.y;
	res.t.z = ((B.m[0][2] * A.t.x) + (B.m[1][2] * A.t.y) + (B.m[2][2] * A.t.z)) + B.t.z;

	return res;
}

IGVector3 IGMatrix::matVecMul(IGMatrix const & A, IGVector3 const & V)
{
	IGVector3 res;
	res.x = A.m[0][0] * V.x + A.m[1][0] * V.y + A.m[2][0] * V.z;
	res.y = A.m[0][1] * V.x + A.m[1][1] * V.y + A.m[2][1] * V.z;
	res.z = A.m[0][2] * V.x + A.m[1][2] * V.y + A.m[2][2] * V.z;
	return res;
}

void  IGMatrix::SetRotX(float r/* = 0*/, bool resetTrans/* = true*/, bool setZeros/* = true*/)
{
	if (resetTrans)     t.x = t.y = t.z = 0;
	if (setZeros)       m[0][1] = m[0][2] = m[1][0] = m[2][0] = 0;
	m[0][0] = 1.0f;
	m[1][1] = m[2][2] = (float)cos(r);
	m[1][2] = -(m[2][1] = (float)sin(r));
}

void  IGMatrix::SetRotY(float r/* = 0*/, bool resetTrans/* = true*/, bool setZeros/* = true*/)
{
	if (resetTrans)     t.x = t.y = t.z = 0;
	if (setZeros)       m[0][1] = m[1][2] = m[1][0] = m[2][1] = 0;
	m[1][1] = 1.0f;
	m[0][0] = m[2][2] = (float)cos(r);
	m[2][0] = -(m[0][2] = (float)sin(r));
}

void  IGMatrix::SetRotZ(float r/* = 0*/, bool resetTrans/* = true*/, bool setZeros/* = true*/)
{
	if (resetTrans)     t.x = t.y = t.z = 0;
	if (setZeros)       m[0][2] = m[1][2] = m[2][0] = m[2][1] = 0;
	m[2][2] = 1.0f;
	m[0][0] = m[1][1] = (float)cos(r);
	m[0][1] = -(m[1][0] = (float)sin(r));
}
