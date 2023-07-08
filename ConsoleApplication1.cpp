#include <iostream>
#include <bitset>
using namespace std;

//loginiai elementai
bool NAND(bool a, bool b)
{
	if (a == 1 && b == 1) return 0;
	else return 1;
}
bool NOT(bool a)
{
	return NAND(a, a);
}
bool AND(bool a, bool b)
{
	return NAND(NAND(a, b), NAND(a, b));
}
bool OR(bool a, bool b)
{
	return NAND(NAND(a, a), NAND(b, b));
}
bool NOR(bool a, bool b)
{
	return NAND(NAND(NAND(a, a), NAND(b, b)), NAND(NAND(a, a), NAND(b, b)));
}
bool XOR(bool a, bool b)
{
	return NAND(NAND(a, NAND(a, b)), NAND(b, NAND(a, b)));
}

//---aritmetika---//

//sudetis
bool Half_adder(bool a, bool b, bool &c_in)
{
	bool sum = XOR(a, b);
	c_in = AND(a, b);

	return sum;
}
bool Full_adder(bool a, bool b, bool c_in, bool &c_out)
{
	bool carry1 = 0, carry2 = 0;
	bool sum = Half_adder(Half_adder(a, b, carry1), c_in, carry2);
	c_out = OR(carry1, carry2);
	
	return sum;
}
bitset<8> Adder(bitset<8> &a, bitset<8> &b, bool &c_in, bool &c_out)
{
	bool ENA=1, ENB = 1;
	bool carry = c_in;
	bitset<8> result;

	for (int i = 0; i < 8; i++) {
		result[i] = Full_adder(a[i], b[i], carry, c_out);
		carry = c_out;
	}
	cout << "Rezultatas " << result << endl;
	cout << "Carry out " << c_out << endl;
	cout << "ENA " << ENA << endl;
	cout << "ENB " << ENB << endl;
	if (c_in >0) cout << "Overflow" << endl;
	return result;
}
//atimtis
bitset<8> PliusVienas(bitset<8> a)
{
	bitset<8> add = a;

	for (int i = 0; i < 8; i++) {
		bitset<1> temp = NOT(a[i]);
		add[i] = temp[0];
		if (temp == 1) break;
	}
	return add;
}
void Atimtis(bitset<8> a, bitset<8> b, bool &c_in, bool &c_out)
{
	bool ENA = 1, ENB = 1;
	bitset<8> A = a, B = b;
	bitset<8> result;
	bitset<1> temp;
	for (int i = 0; i < 8; i++) {
		temp = NOT(B[i]);
		B[i] = temp[0];
	}
	B = PliusVienas(B);
	result = Adder(A, B, c_in, c_out);
}
//A<<1
void Postumis(bitset<8>& a, bool& c_in, bool& c_out)
{
	bool ENA = 1, ENB = 0;
	c_out = a[0];
	a <<= 1;
	a[7] = c_in;

	cout << "Rezultatas " << a << endl;
	cout << "Carry out " << c_out << endl;
	cout << "ENA " << ENA << endl;
	cout << "ENB " << ENB << endl;
}
//A-1
void Minus_vienas(bitset<8> &a, bool &c_out)
{
	bool ENA = 1, ENB = 0;
	bool borrow = 1;
	for (int i = 0; i < 8; i++) {
		bool bit = a[i];
		a[i] = XOR(bit, borrow);
		borrow = AND(NOT(bit), borrow);
	}
	c_out = NOT(borrow);

	cout << "Rezultatas " << a << endl;
	cout << "Carry out " << c_out << endl;
	cout << "ENA " << ENA << endl;
	cout << "ENB " << ENB << endl;
}
//daugyba
void Daugyba(bitset<8> a, bitset<8> b, bool& c_in, bool& c_out)
{
	bitset<8> result;
	bool carry = 0;
	bool ENA = 1, ENB = 1;

	for (int i = 0; i < 8; i++) {
		if (b[i]) {
			for (int j = i; j < 8; j++) {
				result[j] = Full_adder(result[j], a[j - i], carry, c_out);
				carry = c_out;
			}
		}
	}
	cout << "Rezultatas " << result << endl;
	cout << "Carry out " << carry << endl;
	cout << "ENA " << ENA << endl;
	cout << "ENB " << ENB << endl;
	if (c_in >0) cout << "Overflow" << endl;
}
//B!=0
bool Nott(bitset<8> b)
{
	if (b.any() != 0) return true;
	else return false;
}
//A==B
bool Lygu(bitset<8> a, bitset<8> b)
{
	bitset<8> A, B;
	for (int i = 0; i < 8; i++) {
		A[i] = XOR(a[i], a[i]);
		B[i] = XOR(b[i], b[i]);
	}
	bitset<8> result;
	for (int i = 0; i < 8; i++) {
		result[i] = NOR(A[i], B[i]);
	}
	int k = 0;
	for (int i = 0; i < 8; i++) {
		if (result[i] == 1) k++;
	}
	bool output;
	if (k == 8 || k == 0) output = true;
	else output = false;

	return output;
}
//Dekoderis
bitset<8> Decoder(bool state1, bool state2, bool state3)
{
	bitset<8> result(0b00000000);

	result[0] = AND(NOR(NOT(state1), NOT(state2)), state3);
	result[1] = AND(NOR(NOT(state1), NOT(state2)), NOT(state3));
	result[2] = AND(NOR(NOT(state1), state2), state3);
	result[3] = AND(NOR(NOT(state1), state2), NOT(state3));
	result[4] = AND(NOR(state1, NOT(state2)), state3);
	result[5] = AND(NOR(state1, NOT(state2)), NOT(state3));
	result[6] = AND(NOR(state1, state2), state3);
	result[7] = AND(NOR(state1, state2), NOT(state3));
	return result;
}
void Decoder_full(bitset<8> a, bitset<8> b, bool& c_in, bool& c_out, bool state1, bool state2, bool state3)
{
	//cout << "A==B " << Lygu(a, b) << endl;
	switch (Decoder(state1, state2, state3).to_ulong()) {
	case 128:
		Atimtis(a, b, c_in, c_out);
		break;
	case 64:
		Adder(a, b, c_in, c_out);
		break;
	case 32:
		Daugyba(a, b, c_in, c_out);
		break;
	case 16:
		Postumis(a, c_in, c_out);
		break;
	case 8:
		Minus_vienas(a, c_out);
		break;
	case 4:
		cout << b << endl;
		cout << "F: " << Nott(b);
		break;
	default:
		cout << "Nezinoma operacija\n";
		break;
	}
}

int main()
{
	bitset<8> a(0b01101110), b(0b01100010);

	bool state1 = 0, state2 = 0, state3 = 0;
	bool c_in, c_out;
	Decoder_full(a, b, c_in, c_out, state1, state2, state3);
	return 0;
}