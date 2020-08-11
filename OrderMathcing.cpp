#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include "Order.h"

extern std::vector<Order> buy_orders;
extern std::vector<Order> sell_orders;

void PrintTrades(std::multimap<std::string, Order>& requests)
{
	std::multimap<std::string, Order>::iterator it;
	for (it = requests.begin(); it != requests.end(); ++it)
	{
		// For the first print TraderID.
		std::cout << (*it).first;
		// If the order is buy then must be printed sign '+'. Otherwise '-'.
		if ((*it).second.GetSide() == 'B')
		{
			std::cout << '+';
		}
		else
		{
			std::cout << '-';
		}
		std::cout << (*it).second.GetTradeQuantity() << '@' << (*it).second.GetTradePrice() << " ";
	}
	std::cout << std::endl;
}

void MatchOrders()
{
	bool fullMatch = false;

	// Start matching if there are buy ans sell orders.
	if (buy_orders.size() > 0 && sell_orders.size() > 0)
	{
		std::multimap<std::string, Order>::iterator it;
		for (int i = 0; i < buy_orders.size(); ++i)
		{
			// Map of matched orders for further printing where the key is TraderId and the value is the order.
			std::multimap<std::string, Order> requestOrders;
			
			Order buyOrder(buy_orders[i]);
			Order sellOrder;

			for (int j = 0; j < sell_orders.size(); ++j)
			{
				fullMatch = false;
				sellOrder = sell_orders[j];

				// If the price of buy order is equal or more then sell's order price than start mathcing.
				if (buyOrder.GetPrice() >= sellOrder.GetPrice())
				{

					int minQty = std::min(buyOrder.GetQuantity(), sellOrder.GetQuantity());

					// Set match price.
					if (buyOrder.GetPrice() > sellOrder.GetPrice())
					{
						buyOrder.SetTradePrice(sellOrder.GetPrice());
					}

					// If buy order quantity than matched quantity, then we must change order's quantity and insert it in requestOrders. 
					if (buyOrder.GetQuantity() > minQty)
					{
						buyOrder.SetQuantity(buyOrder.GetQuantity() - minQty);
						buyOrder.SetTradeQuantity(minQty);
						
						// If buy order is also in requestOrders map(created on one aggressor execution) with the same trade price
						// then we just need to accumulate its trade quantity. Otherwise we must insert that order in map.
						it = requestOrders.find(buyOrder.GetTraderID());
						if (it != requestOrders.end())
						{
							if (it->second.GetTradePrice() == buyOrder.GetTradePrice())
							{
								it->second.SetTradeQuantity(it->second.GetTradeQuantity() + buyOrder.GetTradeQuantity());
							}
							else
							{
								requestOrders.insert(std::pair<std::string, Order>(buyOrder.GetTraderID(), buyOrder));
							}
						}
						else
						{
							requestOrders.insert(std::pair<std::string, Order>(buyOrder.GetTraderID(), buyOrder));
						}
					}

					// If buy orders quantity is equal to matched quantity then we must insert this order in requestOrders
					// and delete from buy_orders as full matching has happened.
					else
					{
						buyOrder.SetTradeQuantity(minQty);
						std::multimap<std::string, Order>::iterator it = requestOrders.find(buyOrder.GetTraderID());
						if (it != requestOrders.end())
						{
							if (it->second.GetTradePrice() == buyOrder.GetTradePrice())
							{
								it->second.SetTradeQuantity(it->second.GetTradeQuantity() + buyOrder.GetTradeQuantity());
							}
							else
							{
								requestOrders.insert(std::pair<std::string, Order>(buyOrder.GetTraderID(), buyOrder));
							}
						}
						else
						{
							requestOrders.insert(std::pair<std::string, Order>(buyOrder.GetTraderID(), buyOrder));
						}

						buy_orders.erase(buy_orders.begin() + i);
						--i;
						fullMatch = true;
					}

					// If sell order quantity is more than matched quantity, then we must reduce order's quantity(as it is not fully matched)
					// and insert it in requestOrders. 
					if (sellOrder.GetQuantity() > minQty)
					{
						sellOrder.SetQuantity(sellOrder.GetQuantity() - minQty);
						sellOrder.SetTradeQuantity(minQty);

						// If we also have the sell order for the same trader with matched price in requestOrders, then we must just accumulate inserted order's
						// matched quantity for further printing.
						it = requestOrders.find(sellOrder.GetTraderID());
						if (it != requestOrders.end())
						{
							if (it->second.GetTradePrice() == sellOrder.GetTradePrice())
							{
								it->second.SetTradeQuantity(it->second.GetTradeQuantity() + sellOrder.GetTradeQuantity());
							}
							else
							{
								// If the sell order is also in requestOrders map but its trade price isn't the same as matched price
								// then we must insert sell order of the same trader with its trade price.
								requestOrders.insert(std::pair<std::string, Order>(sellOrder.GetTraderID(), sellOrder));
							}
						}
						else
						{
							requestOrders.insert(std::pair<std::string, Order>(sellOrder.GetTraderID(), sellOrder));
						}
					}

					// If sell orders quantity is equal to matched quantity then we must insert this order in requestOrders
					// and delete from sell_orders as full mathcing has happened.
					else
					{
						sellOrder.SetTradeQuantity(minQty);

						std::multimap<std::string, Order>::iterator it = requestOrders.find(sellOrder.GetTraderID());
						if (it != requestOrders.end())
						{
							if (it->second.GetTradePrice() == sellOrder.GetTradePrice())
							{
								it->second.SetTradeQuantity(it->second.GetTradeQuantity() + sellOrder.GetTradeQuantity());
							}
							else
							{
								requestOrders.insert(std::pair<std::string, Order>(sellOrder.GetTraderID(), sellOrder));
							}
						}
						else
						{
							requestOrders.insert(std::pair<std::string, Order>(sellOrder.GetTraderID(), sellOrder));
						}

						sell_orders.erase(sell_orders.begin() + j);
					}

					// If mathcing for buy order is full then we must take next buy order from vector.
					if (fullMatch)
					{
						j = sell_orders.size();
					}
					else
					{
						--j;
					}
				}			
			}

			// Print all trades for one buy order created on one aggressor execution. 
			PrintTrades(requestOrders);
		}
	}
}


