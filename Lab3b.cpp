#include <iostream>
#include <cstdint>
#include <cstring>

using namespace std;

struct Float {
	uint32_t sign;
	uint32_t exponent;
	uint32_t significand;
};

Float multiplyFloat(Float a, Float b) {
	if (a.significand == 0 || b.significand == 0) {
		Float result = { 0,0,0 };
		return result;
	}

	int32_t exp_sum = (int32_t)a.exponent - 127 + (int32_t)b.exponent - 127;
	int32_t result_exponent = 127 + exp_sum;

	uint64_t result_significand = (uint64_t)a.significand * (uint64_t)b.significand;

	while ((result_significand >> 23) == 0) {
		result_significand <<= 1;
		result_exponent--;
	}

	if (result_exponent < 0) {
		cout << "Resultado: Underflow" << endl;
		result_exponent = 0;
		result_significand = 0;
	}
	else if (result_exponent > 255) {
		cout << "Resultado Overflow" << endl;
		result_exponent = 255;
		result_significand = (1ULL << 23) - 1;
	}

	Float result;
	result.sign = 0;
	result.exponent = static_cast<uint32_t>(result_exponent);
	result.significand = static_cast<uint32_t>(result_significand & 0x7FFFFF);

	return result;
}

int main() {
	float input_a, input_b;

	cout << "Ingrese el primer valor en punto flotante: ";
	cin >> input_a;
	cout << "Ingrese el segundo valor en punto flotante: ";
	cin >> input_b;

	Float float_a = { 0, 127, *reinterpret_cast<uint32_t*>(&input_a)};
	Float float_b = { 0, 127, *reinterpret_cast<uint32_t*>(&input_b)};

	Float result = multiplyFloat(float_a, float_b);

	cout << "resultado: " << endl;
	cout << "signo: " << result.sign << endl;
	cout << "exponente: " << result.exponent << endl;
	cout << "significando: " << result.significand << endl;

	float result_native = input_a * input_b;
	cout << "Resultado nativo: " << result_native << endl;

	return 0;
}
