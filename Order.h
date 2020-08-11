#pragma
#include <iostream>

class Order
{
public:
	Order(const std::string& id = "", char side = 'B', int quantity = 0, int price = 0):m_traderID(id),
		m_side(side), m_quantity(quantity), m_price(price), m_firstContractPrice(0), m_tradePrice(price),
		m_tradeQuantity(quantity) {}
	Order(const Order& order) = default;
	Order& operator =(const Order& order) = default;

	friend std::istream& operator >> (std::istream& input, Order& order); 

	std::string GetTraderID() const noexcept { return m_traderID; };
	char GetSide() const noexcept { return m_side; };
	int GetQuantity() const noexcept { return m_quantity; };
	int GetPrice() const noexcept { return m_price; };
	int GetFirstContractPrice() const noexcept { return m_firstContractPrice; };
	int GetTradeQuantity() const noexcept { return m_tradeQuantity; }
	int GetTradePrice() const noexcept { return m_tradePrice; }

	void SetQuantity(int qty) { m_quantity = qty; }
	void SetTradePrice(int tradePrice) { m_tradePrice = tradePrice; }
	void SetTradeQuantity(int tradeQty) { m_tradeQuantity = tradeQty; }
	void SetFirstContractPrice(int price) { m_firstContractPrice = price; }

private:
	std::string m_traderID;
	char m_side;
	int m_quantity;
	int m_price;
	int m_firstContractPrice;
	int m_tradePrice;
	int m_tradeQuantity;
};