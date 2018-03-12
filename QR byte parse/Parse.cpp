#include <iostream>
#include <string>
#include <algorithm>

using namespace std;


int convertToBinary(unsigned int n);
int main()
{
	int x = 0, remainder, reversedNumber = 0;
	string binary;
	int input[2000];
	int max = 0, a, b;

	for (int i = 0; i < 2000; i++)
	{

		cin >> hex >> x;

		input[i] = x / 16;
		i++;
		input[i] = x % 16;
		max = i;
		if (x == 0) break;
	}
	for (int i = 3; i < max; i++)
	{
		cout << (char)(input[i] * 16 + input[i + 1]);
		i++;



		//input[i] = convertToBinary(input[i]);
		//binary = to_string(input[i]);	
		//for (int j = 0; j < 4 - binary.size(); j++)
		//	cout << "0";
		//cout << binary;
		//if (!(i % 2))
		//	cout << " ";
	}
	cout << "\n\n";
	system("Pause");
}

int convertToBinary(unsigned int n) {

	int a, b;
	if (n == 0) return 0;

	return convertToBinary(n / 2) * 10 + n % 2;

}
