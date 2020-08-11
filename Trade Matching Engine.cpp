#include <iostream>
#include "Order.h"

void MatchOrders();
bool keepGoing = true;

int main()
{
	Order order;
	std::cout << "Enter your requests to buy or sell, one request per line. For exit enter 'q'. " << std::endl;

	// While 'q' isn't entered, continue inputting requests.
	while (keepGoing)
	{
		std::cin >> order;
	}

	// Matching buy and sell orders.
	MatchOrders();

	getchar();
	return 0;
}