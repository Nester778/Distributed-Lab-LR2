#include <iostream>
#include <deque>
#include <string>
#include <cmath>
using namespace std;

class BigNum
{
private:
	deque<unsigned> number;
	bool sign = true;
public:
	int BASE = 10;
	BigNum() {}
	BigNum(int n)
	{
		while (n > 0)
		{
			number.push_back(n % BASE);
			n /= BASE;
		}
	}
	BigNum(string n)
	{
		for (int i = 0; i < n.size(); i++) {
			number.push_front(n[i] - '0');
		}
	}

	BigNum operator+(const BigNum& y) const
	{
		BigNum result;
		int carrier = 0;
		for (auto i = 0u; i < max(y.number.size(), number.size()); ++i)
		{
			auto xx = i >= number.size() ? 0 : number[i];
			auto yy = i >= y.number.size() ? 0 : y.number[i];
			uint64_t r = 0ULL + xx + yy + carrier;
			result.number.push_back(r % BASE);
			carrier = r / BASE;
		}
		if (carrier != 0)
			result.number.push_back(carrier);
		return result;
	}

	BigNum operator-(const BigNum& y) const
	{
		BigNum result;

		int carrier = 0;

		for (auto i = 0u; i < max(y.number.size(), number.size()); ++i)
		{
			auto xx = i >= number.size() ? 0 : number[i];
			auto yy = i >= y.number.size() ? 0 : y.number[i];
			if (0LL + xx >= 0LL + yy + carrier)
			{
				result.number.push_back(xx - yy - carrier);
				carrier = 0;
			}
			else
			{
				result.number.push_back(BASE + xx - yy - carrier);
				carrier = 1;
			}
		}
		for (int i = result.number.size() - 1; i >= 0; i--) {
			if (result.number[i] > 0)
				break;
			if (result.number[i] == 0)
				result.number.pop_back();
		}
		if (result.number.size() == 0)
			result.number.push_back(0);

		if (result > y && result > *this)
		{
			BigNum r = y * BigNum("10");
			BigNum res = r - result;
			res.sign = true;
			return res;
		}
		return result;
	}

	BigNum operator*(const BigNum& y) const
	{
		BigNum result;
		BigNum result1;
		for (auto i = 0u; i < y.number.size(); ++i)
		{
			unsigned carrier = 0;
			BigNum k;
			for (auto j = 0u; j < i; ++j)
				k.number.push_back(0);
			for (auto j = 0u; j < number.size(); ++j)
			{
				auto xx = number[j];
				auto yy = y.number[i];
				uint64_t r = 1ULL * xx * yy + carrier;
				k.number.push_back(r % BASE);
				carrier = r / BASE;
			}
			if (carrier != 0)
				k.number.push_back(carrier);
			result = result + k;
		}
		return result;
	}

	BigNum operator/(const BigNum& y) const
	{
		BigNum result;
		BigNum l;
		for (auto i = 0u; i < number.size(); ++i)
		{
			l.number.push_front(number[number.size() - i - 1]);
			auto div = findDiv(l, y);
			result.number.push_front(div);
			l = l - y * BigNum(div);
		}
		return result;
	}

	unsigned findDiv(const BigNum& dividend, const BigNum& divisor) const
	{
		unsigned first = 0;
		unsigned last = BASE;

		unsigned it;
		int64_t count, step;
		count = last - first;
		while (count > 0)
		{
			it = first;
			step = count / 2;
			it += step;
			if (!(dividend < BigNum(it) * divisor))
			{
				first = ++it;
				count -= step + 1;
			}
			else
				count = step;
		}
		return first - 1;
	}

	bool operator<(const BigNum& y) const
	{
		for (int i = max(number.size(), y.number.size()) - 1; i >= 0; --i)
		{
			auto xx = i >= static_cast<int>(number.size()) ? 0 : number[i];
			auto yy = i >= static_cast<int>(y.number.size()) ? 0 : y.number[i];
			if (xx != yy)
				return xx < yy;
		}
		return false;
	}

	bool operator>(const BigNum& y) const
	{
		for (int i = max(number.size(), y.number.size()) - 1; i >= 0; --i)
		{
			auto xx = i >= static_cast<int>(number.size()) ? 0 : number[i];
			auto yy = i >= static_cast<int>(y.number.size()) ? 0 : y.number[i];
			if (xx != yy)
				return xx > yy;
		}
		return false;
	}

	bool operator>=(const BigNum& y) const
	{
		if (number.size() == y.number.size()) {
			for (int i = max(number.size(), y.number.size()) - 1; i >= 0; --i)
			{
				auto xx = i >= static_cast<int>(number.size()) ? 0 : number[i];
				auto yy = i >= static_cast<int>(y.number.size()) ? 0 : y.number[i];
				if (xx != yy)
					return xx > yy;
				if (xx == yy && i == 0)
					return true;
			}
		}
		else {
			if (number.size() > y.number.size())
				return true;
			if (number.size() < y.number.size())
				return false;
		}
		return false;
	}

	void setHex(string hex)
	{
		string n = hexToDec(hex);
		for (int i = 0; i < n.size(); i++) {
			number.push_front(n[i] - '0');
		}
	}

	bool getSign() {
		return this->sign;
	}

	string getDec() {
		string str = "";
		for (int x = this->number.size() - 1; x >= 0; x--) {
			str += to_string(this->number[x]);
		}
		return str;
	}

	string getHex() {
		return decToHex(*this);
	}

	string getBin() {
		return decToBin(*this);
	}

	string hexToDec(string hex)
	{
		string res;
		BigNum n1("0");
		reverse(hex.begin(), hex.end());
		for (int i = hex.size() - 1; i >= 0; i--)
		{
			int a;
			switch (hex[i])
			{
			case 'a':
				a = 10;
				break;

			case 'b':
				a = 11;
				break;

			case 'c':
				a = 12;
				break;

			case 'd':
				a = 13;
				break;

			case 'e':
				a = 14;
				break;

			case 'f':
				a = 15;
				break;

			default:
				a = hex[i] - '0';
				break;
			}
			BigNum a2(to_string(a));
			BigNum h("16");
			BigNum po("1");
			for (int j = 0; j < i; j++)
				po = po * h;

			BigNum b = a2 * po;
			string str = "";
			for (int x = b.number.size() - 1; x >= 0; x--)
				str += to_string(b.number[x]);
			BigNum n2(str);
			n1 = n1 + n2;
		}
		for (int x = n1.number.size() - 1; x >= 0; x--)
			res += to_string(n1.number[x]);
		return res;
	}

	string decToHex(BigNum dec) {
		string res = "";
		while (dec > 0) {
			auto z = dec / BigNum(16);
			auto g = z * BigNum(16);
			auto h = dec - g;
			dec = z;
			string str = "";
			for (int x = h.number.size() - 1; x >= 0; x--)
				str += to_string(h.number[x]);

			switch (stoi(str))
			{
			case 10:
				res = "a" + res;
				break;

			case 11:
				res = "b" + res;
				break;

			case 12:
				res = "c" + res;
				break;

			case 13:
				res = "d" + res;
				break;

			case 14:
				res = "e" + res;
				break;

			case 15:
				res = "f" + res;
				break;

			default:
				res = str + res;
				break;
			}
		}
		return res;
	}

	string decToBin(BigNum dec) {
		string res = "";
		while (dec > 0) {
			auto z = dec / BigNum(2);
			auto g = z * BigNum(2);
			auto h = dec - g;
			dec = z;
			res = to_string(h.number[0]) + res;
		}
		return res;
	}

	string binToDec(string hex)
	{
		string res;
		BigNum n1("0");
		reverse(hex.begin(), hex.end());
		for (int i = hex.size() - 1; i >= 0; i--)
		{
			string fg = "";
			BigNum a2(fg + hex[i]);
			BigNum h("2");
			BigNum po("1");
			for (int j = 0; j < i; j++)
				po = po * h;

			BigNum b = a2 * po;
			string str = "";
			for (int x = b.number.size() - 1; x >= 0; x--)
				str += to_string(b.number[x]);
			BigNum n2(str);
			n1 = n1 + n2;
		}
		for (int x = n1.number.size() - 1; x >= 0; x--)
			res += to_string(n1.number[x]);
		return res;
	}

	string XOR(string y)
	{
		string res = "";
		string xx = this->getBin();
		string yy = decToBin(hexToDec(y));

		while (xx.size() < yy.size()) {
			xx = "0" + xx;
		}
		while (xx.size() > yy.size()) {
			yy = "0" + yy;
		}
		if (xx.size() == yy.size())
		{
			for (int i = 0; i < xx.size(); i++)
			{
				if (xx[i] == yy[i])
					res += "0";
				else
					res += "1";
			}
		}
		return decToHex(binToDec(res));
	}

	string INV() {
		string res = "";
		string xx = this->getBin();
		for (int i = 0; i < xx.size(); i++)
		{
			if (xx[i] == '1')
				res += "0";
			else
				res += "1";
		}

		return decToHex(binToDec(res));
	}

	string OR(string y)
	{
		string res = "";
		string xx = this->getBin();
		string yy = decToBin(hexToDec(y));

		while (xx.size() < yy.size()) {
			xx = "0" + xx;
		}
		while (xx.size() > yy.size()) {
			yy = "0" + yy;
		}
		if (xx.size() == yy.size())
		{
			for (int i = 0; i < xx.size(); i++)
			{
				if (xx[i] == '0' && yy[i] == '0')
					res += "0";
				else
					res += "1";
			}
		}
		return decToHex(binToDec(res));
	}

	string AND(string y)
	{
		string res = "";
		string xx = this->getBin();
		string yy = decToBin(hexToDec(y));

		while (xx.size() < yy.size()) {
			xx = "0" + xx;
		}
		while (xx.size() > yy.size()) {
			yy = "0" + yy;
		}
		if (xx.size() == yy.size())
		{
			for (int i = 0; i < xx.size(); i++)
			{
				if (xx[i] == '1' && yy[i] == '1')
					res += "1";
				else
					res += "0";
			}
		}
		return decToHex(binToDec(res));
	}

	string shiftR(int shift) {
		string res = "";
		string xx = this->getBin();
		for (int i = 0; i < shift; i++)
			xx = "0" + xx;
			
		res = xx.substr(0, xx.size() - shift);
		return res;
	}

	string shiftL(int shift) {
		string res = "";
		string xx = this->getBin();
		for (int i = 0; i < shift; i++)
			xx = xx + "0";
	
		res = xx.substr(shift, xx.size());
		return res;
	}
};

int main()
{
	BigNum n;
	n.setHex("51bf608414ad5726a3c1bec098f77b1b54ffb2787f8d528a74c1d7fde6470ea4");

	cout << "N bin = "<< n.getBin() << endl;
	cout << "N dec = " << n.getDec() << endl;
	cout << "N hex = " << n.getHex() << endl;
	
	cout << "--------------------------------------------------------------------------" << endl;

	BigNum nXOR, n2XOR;
	nXOR.setHex("51bf608414ad5726a3c1bec098f77b1b54ffb2787f8d528a74c1d7fde6470ea4");
	n2XOR.setHex("403db8ad88a3932a0b7e8189aed9eeffb8121dfac05c3512fdb396dd73f6331c");

	cout << "hex1 = 51bf608414ad5726a3c1bec098f77b1b54ffb2787f8d528a74c1d7fde6470ea4" << endl;
	cout << "XOR" << endl;
	cout << "hex2 = 403db8ad88a3932a0b7e8189aed9eeffb8121dfac05c3512fdb396dd73f6331c" << endl << endl;
	cout << "result = " << nXOR.XOR(n2XOR.getHex())<<endl;
	cout << "--------------------------------------------------------------------------" << endl;

	BigNum nINV;
	nINV.setHex("51bf608414ad5726a3c1bec098f77b1b54ffb2787f8d528a74c1d7fde6470ea4");

	cout << "hex1 = 51bf608414ad5726a3c1bec098f77b1b54ffb2787f8d528a74c1d7fde6470ea4" << endl;
	cout << "INV" << endl << endl;
	cout << "result = " << nINV.INV() << endl;
	cout << "--------------------------------------------------------------------------" << endl;

	BigNum nOR, n2OR;
	nOR.setHex("51bf608414ad5726a3c1bec098f77b1b54ffb2787f8d528a74c1d7fde6470ea4");
	n2OR.setHex("403db8ad88a3932a0b7e8189aed9eeffb8121dfac05c3512fdb396dd73f6331c");

	cout << "hex1 = 51bf608414ad5726a3c1bec098f77b1b54ffb2787f8d528a74c1d7fde6470ea4" << endl;
	cout << "OR" << endl;
	cout << "hex2 = 403db8ad88a3932a0b7e8189aed9eeffb8121dfac05c3512fdb396dd73f6331c" << endl << endl;
	cout << "result = " << nOR.OR(n2OR.getHex()) << endl;
	cout << "--------------------------------------------------------------------------" << endl;

	BigNum nAND, n2AND;
	nAND.setHex("51bf608414ad5726a3c1bec098f77b1b54ffb2787f8d528a74c1d7fde6470ea4");
	n2AND.setHex("403db8ad88a3932a0b7e8189aed9eeffb8121dfac05c3512fdb396dd73f6331c");

	cout << "hex1 = 51bf608414ad5726a3c1bec098f77b1b54ffb2787f8d528a74c1d7fde6470ea4" << endl;
	cout << "AND" << endl;
	cout << "hex2 = 403db8ad88a3932a0b7e8189aed9eeffb8121dfac05c3512fdb396dd73f6331c" << endl << endl;
	cout << "result = " << nAND.AND(n2AND.getHex()) << endl;
	cout << "--------------------------------------------------------------------------" << endl;

	BigNum nShr;
	nShr.setHex("51bf608414ad5726a3c1bec098f77b1b54ffb2787f8d528a74c1d7fde6470ea4");

	cout << "hex1 = 51bf608414ad5726a3c1bec098f77b1b54ffb2787f8d528a74c1d7fde6470ea4" << endl;
	cout << "shiftR" << endl << endl;
	
	cout << "result = " << nShr.shiftR(12) << endl;
	cout << "--------------------------------------------------------------------------" << endl;

	BigNum nShl;
	nShl.setHex("51bf608414ad5726a3c1bec098f77b1b54ffb2787f8d528a74c1d7fde6470ea4");

	cout << "hex1 = 51bf608414ad5726a3c1bec098f77b1b54ffb2787f8d528a74c1d7fde6470ea4" << endl;
	cout << "shiftL" << endl << endl;

	cout << "result = " << nShl.shiftL(12) << endl;
	cout << "--------------------------------------------------------------------------" << endl;


	BigNum nADD, n2ADD, n3ADD;
	nADD.setHex("36f028580bb02cc8272a9a020f4200e346e276ae664e45ee80745574e2f5ab80");
	n2ADD.setHex("70983d692f648185febe6d6fa607630ae68649f7e6fc45b94680096c06e4fadb");
	n3ADD = nADD + n2ADD;

	cout << "hex1 = 36f028580bb02cc8272a9a020f4200e346e276ae664e45ee80745574e2f5ab80" << endl;
	cout << "ADD" << endl;
	cout << "hex2 = 70983d692f648185febe6d6fa607630ae68649f7e6fc45b94680096c06e4fadb" << endl << endl;
	cout << "result = " << n3ADD.getHex() << endl;
	cout << "--------------------------------------------------------------------------" << endl;

	BigNum nSUB, n2SUB, n3SUB;
	nSUB.setHex("33ced2c76b26cae94e162c4c0d2c0ff7c13094b0185a3c122e732d5ba77efebc");
	n2SUB.setHex("22e962951cb6cd2ce279ab0e2095825c141d48ef3ca9dabf253e38760b57fe03");
	n3SUB = nSUB - n2SUB;

	cout << "hex1 = 33ced2c76b26cae94e162c4c0d2c0ff7c13094b0185a3c122e732d5ba77efebc" << endl;
	cout << "SUB" << endl;
	cout << "hex2 = 22e962951cb6cd2ce279ab0e2095825c141d48ef3ca9dabf253e38760b57fe03" << endl << endl;
	cout << "result = " << n3SUB.getHex() << endl;
	cout << "--------------------------------------------------------------------------" << endl;

	BigNum nMUL, n2MUL, n3MUL;
	nMUL.setHex("7d7deab2affa38154326e96d350deee1");
	n2MUL.setHex("97f92a75b3faf8939e8e98b96476fd22");
	n3MUL = nMUL * n2MUL;

	cout << "hex1 = 7d7deab2affa38154326e96d350deee1" << endl;
	cout << "MUL" << endl;
	cout << "hex2 = 97f92a75b3faf8939e8e98b96476fd22" << endl << endl;
	cout << "result = " << n3MUL.getHex() << endl;
	cout << "--------------------------------------------------------------------------" << endl;

	BigNum nDIV, n2DIV, n3DIV;
	nDIV.setHex("c6f3b40b6c000");
	n2DIV.setHex("186a0");
	n3DIV = nDIV / n2DIV;

	cout << "hex1 = c6f3b40b6c000" << endl;
	cout << "DIV" << endl;
	cout << "hex2 = 186a0" << endl << endl;
	cout << "result = " << n3DIV.getHex() << endl;
	cout << "--------------------------------------------------------------------------" << endl;

	return 0;
}

