//=================================================
//
// 1. Vector Algebra
//	
// - 
// - 
// - 
// 
//=================================================

#include <windows.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

ostream& XM_CALLCONV operator<< (ostream& os, FXMVECTOR v) {
	XMFLOAT3 dest;
	XMStoreFloat3(&dest, v);

	os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ")";
	return os;
}

void BasicCalculation();
void FloatingPointError();

int main() {

	cout.setf(ios_base::boolalpha);
	
	// SSE2 를 지원하는지 확인
	if (!XMVerifyCPUSupport()) {
		cout << "DirectXMath를 지원하지 않음" << endl;
		return 0;
	}
	
	// BasicCalculation();
	FloatingPointError();
}

void BasicCalculation() {

	// XMVECTOR 설정 함수
	XMVECTOR p = XMVectorZero();
	XMVECTOR q = XMVectorSplatOne();
	XMVECTOR u = XMVectorSet(1.0f, 2.0f, 3.0f, 4.0f);
	XMVECTOR v = XMVectorReplicate(-2.0);
	XMVECTOR w = XMVectorSplatZ(u);
	XMVECTOR n = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	// XMVECTOR 연산
	XMVECTOR a = u + v;
	XMVECTOR b = u - v;
	XMVECTOR c = 10.0f * u;

	XMVECTOR L = XMVector3Length(u);		// |u|
	XMVECTOR d = XMVector3Normalize(u);		// u / |u|

	XMVECTOR s = XMVector3Dot(u, v);		// 내적 연산
	XMVECTOR e = XMVector3Cross(u, v);		// 외적 연산

	XMVECTOR projW;		// 투영 벡터
	XMVECTOR perpW;		// 수직 벡터
	XMVector3ComponentsFromNormal(&projW, &perpW, w, n);

	bool equal = XMVector3Equal(projW + perpW, w) != 0;
	bool notEqual = XMVector3NotEqual(projW + perpW, w) != 0;

	XMVECTOR angleVec = XMVector3AngleBetweenVectors(projW, perpW);		// 사이의 각을 x성분에 저장
	float angleRadians = XMVectorGetX(angleVec);
	float angleDegrees = XMConvertToDegrees(angleRadians);
}

void FloatingPointError() {

	XMVECTOR u = XMVectorSet(1.0f, .10f, 1.0f, 0.0f);
	XMVECTOR n = XMVector3Normalize(u);

	float LU = XMVectorGetX(XMVector3Length(n));

	cout << setprecision(8);
	cout << LU << endl;
	if (LU == 1.0f)
		cout << "길이 1" << endl;
	else
		cout << "길이 1 아님" << endl;

	float powLU = powf(LU, 1.0e6f);
	cout << "LU^(10^6) = " << powLU << endl;
}