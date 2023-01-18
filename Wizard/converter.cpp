#include "converter.h"

using namespace std;

wstring result::concatenate() {
	wstring ws1(binary.begin(), binary.end());
	wstring ws2(hexa.begin(), hexa.end());
	wstring ws = L"binary is: " + ws1 + L"\r\n" + L"hexadecimal is : " + ws2 + L"\r\n" + L"decimal is : " + to_wstring(decimal);
	return ws;
}

long pow(int x, int y) {

	long result = 1;

	for (int i = y; i > 0; i--) {

		result *= x;

	}

	return result;

}



long hextodec(char hex) {

	switch (hex) {

	case '0': return 0;

	case '1': return 1;

	case '2': return 2;

	case '3': return 3;

	case '4': return 4;

	case '5': return 5;

	case '6': return 6;

	case '7': return 7;

	case '8': return 8;

	case '9': return 9;

	case 'A': case 'a': return 10;

	case 'B': case 'b': return 11;

	case 'C': case 'c': return 12;

	case 'D': case 'd': return 13;

	case 'E': case 'e': return 14;

	case 'F': case 'f': return 15;

	}

	return -1;

}


//hextobin not used
string hextobin(char hex) { //least sig bit changes every increment of 1 decimal value, 2nd least sig fig changes every increment of 2 decimal value
	
	string result = "";

	for (int i = 16; i >= 2; i /= 2) {
		long l = hextodec(hex);
		if (l == -1) {} //1 indicates invalid hex value, handle invalid input
		result += (l % i == 0) ? '0' : '1';

	}

	return result;

}



string dectobin(long d) {

	int a = 0;

	while (d > pow(2, a)) a++; //allocate space for bin string

	string result(a, ' ');

	for (int i = 1; i < a + 1; i++) {

		result[a - i] = ((d / pow(2, i - 1) % 2 == 0)) ? '0' : '1';

	}



	return result;



}



string bintohex(string s) {

	string result = "";

	long d = 0;

	long long int size = s.size();

	int rsize = size % 4; //get remainder of size when divided by 4 

	for (int i = rsize; i > 0; i--) {
		d += (pow(2, rsize - i)) * ((int)s[i - 1] - 48);

	}

	if (d < 10) {

		result += char(d + 48);

	}

	else if (d >= 10) {

		result += char(d + 55);

	}

	if (size < 4) return result;

	d = 0;

	for (int i = rsize; i < size; i++) {

		d += pow(2, 3 - ((i - rsize) % 4)) * ((int)s[i] - 48);

		if ((i + 1 - rsize) % 4 == 0) {

			if (d < 10) {

				result += char(d + 48);

			}

			else if (d >= 10) {

				result += char(d + 55);

			}

			d = 0;

		}

	}

	return result;

}

void isnum(result r, const string& s) {
	for (auto c : s) {
		if (isdigit(c) == false) {
			r.invInput = true;
			break;
		}
	}
}


result engine(string s, string type) {
		long decimal = 0;

		result res;

		long result = 0;

		string hex = "";

		string bin = "";

		long long int size = s.size();

		if (type == "bin") {
			if (size > 28) res.invInput = true;

			for (int i = 0; i < size; i++) { //loop thru every num in bin

				if (s[i] == '0') continue;

				else if (s[i] == '1') decimal += pow(2, size - i - 1);

				else {
					res.invInput = true;
					break;
				}

			}

			if (res.invInput != true) { //only proceed if input is valid
				hex = bintohex(s);
				res.decimal = decimal; res.hexa = hex; res.binary = s;
			}

		}

		else if (type == "hex") {

			if (size > 7) res.invInput = true;

			for (int i = 0; i < size; i++) { //loop thru every num in hex
				long l = hextodec(s[i]);
				if (l == -1) { 
					res.invInput = true;
					break;

				}
				decimal += (pow(16, size - i - 1)) * l;

			}
			if (res.invInput != true) {
				bin = dectobin(decimal);

				res.decimal = decimal; res.hexa = s; res.binary = bin;
			}

		}

		else if (type == "dec") {

			isnum(res , s);
			if (res.invInput != true) {

				int dec = stoi(s);
				if (dec > 268435455) {
					res.invInput = true;
					return res;
				}
				bin = dectobin(dec);

				hex = bintohex(bin);

				res.decimal = stoi(s); res.hexa = hex; res.binary = bin;
			}

		}
		return res;
	}

	
