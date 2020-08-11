#include <iostream>
#include <vector>
#include "Order.h"

std::vector<Order> buy_orders;
std::vector<Order> sell_orders;
extern bool keepGoing;

std::istream& operator >> (std::istream& input, Order& order)
{	
	// If one of the inputs is 'q' then keepGoing triggers to false and inputting is stopping.
	input >> order.m_traderID;
	if (order.m_traderID == "q")
	{
		keepGoing = false;
		return input;
	}
	input >> order.m_side;
	if (order.m_side == 'q')
	{
		keepGoing = false;
		return input;
	}
	input >> order.m_quantity;
	order.m_tradeQuantity = order.m_quantity;
	if (order.m_quantity == 'q')
	{
		keepGoing = false;
		return input;
	}
	input >> order.m_price;
	order.m_tradePrice = order.m_price;
	if (order.m_price == 'q')
	{
		keepGoing = false;
		return input;
	} 
	bool requestIsCorrect = false;
	
	while (!requestIsCorrect)
	{
		// Counter which helps in understanding is request fully correct or no. If tha value isn't equal to 4
		// then one more check is necessary until value becomes equal to 4.
		int parametersThatAreCorrect = 4;

		// Check that all parameters are correct.
		if (order.m_traderID == "q" || order.m_side == 'q' || order.m_quantity == 'q' || order.m_price == 'q')
		{
			keepGoing = false;
			return input;
		}

		// If traderId isn't like T#, where # is number then order must be entered again with correct value.
		if (order.m_traderID[0] != 'T' || !isdigit(order.m_traderID[1]))
		{
			std::cout << "Please enter your order again with correct trader id: T*." << std::endl;
			input >> order.m_traderID >> order.m_side >> order.m_quantity >> order.m_price;
			--parametersThatAreCorrect;
		}

		if (order.m_side != 'B' && order.m_side != 'S')
		{
			std::cout << "Please enter your order again with  correct side: for buy order - B, for sell order - S." << std::endl;
			input >> order.m_traderID >> order.m_side >> order.m_quantity >> order.m_price;
			--parametersThatAreCorrect;
		}

		if (order.m_quantity < 1 || order.m_quantity > 20000)
		{
			std::cout << "Please enter your order again with correct order quantity: it must be a number and more than 1." << std::endl;
			input >> order.m_traderID >> order.m_side >> order.m_quantity >> order.m_price;
			--parametersThatAreCorrect;
		}

		if (order.m_price < 1 /*|| !isdigit(order.m_price)*/)
		{
			std::cout << "Please enter your order again with  correct order price: it must be a number and more than 1." << std::endl;
			input >> order.m_traderID >> order.m_side >> order.m_quantity >> order.m_price;
			--parametersThatAreCorrect;
		}

		// If all parameters are correct then thhis request is correct and we must get out from While.
		if (parametersThatAreCorrect == 4)
		{
			requestIsCorrect = true;
		}
		order.m_tradePrice = order.m_price;
		order.m_tradeQuantity = order.m_quantity;
	}

	// Insert order in appropriate vector.
	if (order.GetSide() == 'B')
	{
		buy_orders.push_back(order);
	}
	else
	{
		sell_orders.push_back(order);
	}
	return input;
}