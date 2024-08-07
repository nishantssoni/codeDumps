#include <iostream>
#include <math.h>
using namespace std;

int get_len(int x)
{
	int counter = 0;
	while (x)
	{
		x /= 10;
		++counter;
	}
	return counter;
}

int arm(int x)
{
	int pwr = get_len(x);
	int rem, sum = 0;
	int check = x;

	while (x)
	{
		rem = x % 10;
		x /= 10;
		sum += pow(rem, pwr);
	}

	if (check == sum)
		return 1;

	return 0;
}

int main()
{
	int l, h;

	cout << "enter a value of [L , H] seperated by space :: ";
	cin >> l >> h;

	for (int i = l; i <= h; ++i)
	{

		if (arm(i))
		{
			cout << i << endl;
		}
	}

	return 0;
}
