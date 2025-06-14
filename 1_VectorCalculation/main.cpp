//=================================================
//
// 1. Vector Calculation
//	
// - SIMD�� Ȱ���Ϸ��� XMVECTOR �������� ��ȯ�ؾߵ�
// - XMVECTOR -> XMFLOATn : XMStoreFloatn �Լ� ���
// - XMFLOATn -> XMVECTOR : XMLoadFloatn �Լ� ���
// - ������ʹ� XMVECTORF32 ���
// 
// - XMVECTOR ���� �Լ�
// - XMVECTOR ���� �Լ�
// - float�� ���� �� �� ����
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

	// SSE2 �� �����ϴ��� Ȯ��
	if (!XMVerifyCPUSupport()) {
		cout << "DirectXMath�� �������� ����" << endl;
		return 0;
	}

	BasicCalculation();
	FloatingPointError();
}

void BasicCalculation() {

	cout << "\n===================== Basic Calculation =====================\n\n";

	// XMVECTOR ���� �Լ�
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

	// XMVECTOR ����
	XMVECTOR a = u + v;
	XMVECTOR b = u - v;
	XMVECTOR c = 10.0f * u;

	XMVECTOR L = XMVector3Length(u);		// |u| (x ����)
	XMVECTOR d = XMVector3Normalize(u);		// u / |u|

	XMVECTOR s = XMVector3Dot(u, v);		// ���� ����
	XMVECTOR e = XMVector3Cross(u, v);		// ���� ����

	XMVECTOR projW;		// ���� ����
	XMVECTOR perpW;		// ���� ����
	XMVector3ComponentsFromNormal(&projW, &perpW, w, n);

	bool equal = XMVector3Equal(projW + perpW, w) != 0;
	bool notEqual = XMVector3NotEqual(projW + perpW, w) != 0;

	XMVECTOR angleVec = XMVector3AngleBetweenVectors(projW, perpW);		// ������ ���� x���п� ����
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

	// �̷������δ� 1������ �ε��Ҽ��� Ư���� ��Ȯ�� ���� ��Ÿ���� ���� �� ����
	// ����, �ε��Ҽ����� �񱳴� == �� �ƴ϶� Epsilon �� ����ϰų� XMVector3NearEqual �Լ� ���
	cout << "LU : " << LU << endl;

	cout << (LU == 1.0f ? "LU == 1" : "LU != 1") << endl;

	float epsilon = 0.001f;
	cout << (abs(LU - 1.0f) < epsilon ? "LU := 1" : "LU != 1") << endl;

}