//=================================================
//
// 1. Vector Calculation
//	
// - SIMD를 활용하려면 XMVECTOR 형식으로 변환해야됨
// - XMVECTOR -> XMFLOATn : XMStoreFloatn 함수 사용
// - XMFLOATn -> XMVECTOR : XMLoadFloatn 함수 사용
// - 상수벡터는 XMVECTORF32 사용
// 
// - XMVECTOR 설정 함수
// - XMVECTOR 연산 함수
// - float의 오차 및 비교 연산
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

	BasicCalculation();
	FloatingPointError();
}

void BasicCalculation() {

	cout << "\n===================== Basic Calculation =====================\n\n";

	// XMVECTOR 설정 함수
	XMVECTOR p = XMVectorZero();
	XMVECTOR q = XMVectorSplatOne();
	XMVECTOR u = XMVectorSet(1.0f, 2.0f, 3.0f, 4.0f);
	XMVECTOR v = XMVectorReplicate(-2.0);
	XMVECTOR w = XMVectorSplatZ(u);
	XMVECTOR n = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	cout << "p = " << p << endl;
	cout << "q = " << q << endl;
	cout << "u = " << u << endl;
	cout << "v = " << v << endl;
	cout << "w = " << w << endl;
	cout << "n = " << n << endl;

	// XMVECTOR 연산
	XMVECTOR a = u + v;
	XMVECTOR b = u - v;
	XMVECTOR c = 10.0f * u;

	XMVECTOR L = XMVector3Length(u);		// |u| (x 성분)
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

	cout << left << endl;
	cout << setw(20) << "a = u + v" << " : " << a << endl;
	cout << setw(20) << "b = u - v" << " : " << b << endl;
	cout << setw(20) << "c = 10.0f + u" << " : " << c << endl;
	cout << setw(20) << "d = u / |u|" << " : " << d << endl;
	cout << setw(20) << "e = u x v" << " : " << e << endl;
	cout << setw(20) << "L = |u|" << " : " << L << endl;
	cout << setw(20) << "s = u.v" << " : " << s << endl;
	cout << setw(20) << "projW" << " : " << projW << endl;
	cout << setw(20) << "perpW" << " : " << perpW << endl;
	cout << setw(20) << "angle" << " : " << angleDegrees << endl;

}

void FloatingPointError() {

	cout << "\n===================== Floating Point Error =====================\n\n";

	XMVECTOR u = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	XMVECTOR n = XMVector3Normalize(u);

	float LU = XMVectorGetX(XMVector3Length(n));

	cout << setprecision(8);

	// 이론적으로는 1이지만 부동소수점 특성상 정확한 수를 나타내지 못할 수 있음
	// 따라서, 부동소수점의 비교는 == 가 아니라 Epsilon 을 사용하거나 XMVector3NearEqual 함수 사용
	cout << "LU : " << LU << endl;

	cout << (LU == 1.0f ? "LU == 1" : "LU != 1") << endl;

	float epsilon = 0.001f;
	cout << (abs(LU - 1.0f) < epsilon ? "LU := 1" : "LU != 1") << endl;

}