#include<iostream>
#include<string>
#include"my_md5.h"
using namespace std;
unsigned int origin_A;
unsigned int origin_B;
unsigned int origin_C;
unsigned int origin_D;
unsigned int int_number;
using namespace std;
/*第一分组需要将上面四个链接变量复制到另外四个变量中：A到a，B到b，C到c，D到d。从第二分组开始的变量为上一分组的运算结果，
即A = a， B = b， C = c， D = d。主循环有四轮（MD4只有三轮），每轮循环都很相似。第一轮进行16次操作。每次操作对a、b、c和d
中的其中三个作一次非线性函数运算，然后将所得结果加上第四个变量，文本的一个子分组和一个常数。再将所得结果向左环移一个不定
的数，并加上a、b、c或d中之一。最后用该结果取代a、b、c或d中之一*/
void process_data(unsigned int *data) {
	unsigned int a = origin_A;
	unsigned int b = origin_B;
	unsigned int c = origin_C;
	unsigned int d = origin_D;
	unsigned int f;
	unsigned int g;
	for (unsigned int i = 0; i < 64; i++) {
		if (i < 16) {
			f = F_func(b, c, d);
			g = i;
		}
		else if (i < 32) {
			f = G_func(b, c, d);
			g = (5 * i + 1) % 16;
		}
		else if (i < 48) {
			f = H_func(b, c, d);
			g = (3 * i + 5) % 16;
		}
		else {
			f = I_func(b, c, d);
			g = (7 * i % 16);
		}
		unsigned int temp = d;                    //用一个变量空间来交换a,b,c,d的值
		d = c;
		c = b;
		b = b + shift_func(a + f + Ti[i / 16][i % 16] + data[g], shift_left[i / 16][i % 16]);
		a = temp;
	}
	origin_A += a;
	origin_B += b;
	origin_C += c;
	origin_D += d;
}
string dec_to_hex(int dec) {
	int temp;
	string s1;
	string s = "";
	for (unsigned int i = 0; i < 4; i++) {
		s1 = "";
		temp = (dec >> i * 8) % (1 << 8) & 0xff;
		for (unsigned int j = 0; j < 2; j++) {
			s1.insert(0, 1, hex_16[temp % 16]);
			temp = temp / 16;
		}
		s = s + s1;
	}
	return s;
}
/* 在MD5算法中，首先需要对信息进行填充，使其位长对512求余的结果等于448，
并且填充必须进行，即使其位长对512求余的结果等于448*/
unsigned int* padding(string s) {
	unsigned int group_num = (s.length() + 8) / 64 + 1;        //512bit, 64 byte per group
	int_number = group_num * 16;                  //4 byte per int
	unsigned int *int_array = new unsigned int[int_number];
	for (unsigned int i = 0; i < int_number; i++) {
		int_array[i] = 0;
	}
	for (unsigned int j = 0; j < s.length(); j++) {
		int_array[j / 4] = int_array[j / 4] | (s[j]) << ((j % 4) * 8);
	}
	int_array[s.length() / 4] = int_array[s.length() / 4] | (0x80) << ((s.length() % 4) * 8); //加上一个1和若干个0
	int_array[int_number - 2] = s.length() * 8;
	return int_array;
}
string MD5_result(string src) {
	origin_A = A;
	origin_B = B;
	origin_C = C;
	origin_D = D;
	unsigned int* padding_array = padding(src);
	for (unsigned int i = 0; i < int_number / 16; i = i + 16) {
		unsigned int tmp_integer[16];
		for (unsigned int j = 0; j < 16; j++) {
			tmp_integer[j] = padding_array[i * 16 + j];
		}
		process_data(tmp_integer);
	}
	string a_b_c_d = dec_to_hex(origin_A).append(dec_to_hex(origin_B)).append(dec_to_hex(origin_C)).append(dec_to_hex(origin_D));
	return a_b_c_d;
}
int main() {
	string example1 = "abc";
	string md5_result1 = MD5_result(example1);
	cout << "The origin input is :" << endl;
	cout << example1 << endl;
	cout << "The corresponding MD5 output is :" << endl;
	cout << md5_result1 << endl;
	string example2 = "abcdefghijklmnopqrstuvwxyz";
	string md5_result2 = MD5_result(example2);
	cout << "The origin input is :" << endl;
	cout << example2 << endl;
	cout << "The corresponding MD5 output is :" << endl;
	cout << md5_result2 << endl;
	string example3 = "8a683566bcc7801226b3d8b0cf35fd97";
	string md5_result3 = MD5_result(example3);
	cout << "The origin input is :" << endl;
	cout << example3 << endl;
	cout << "The corresponding MD5 output is :" << endl;
	cout << md5_result3 << endl;
	cout << "Now you can input your string and get corresponding MD5 result." << endl;
	while (1) {
		string ss;
		cin >> ss;
		string s = MD5_result(ss);
		cout << endl;
		cout << s << endl;
	}
}