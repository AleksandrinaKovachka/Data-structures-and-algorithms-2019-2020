#pragma once
#include "DynamicArray.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

const unsigned systemWallet = 4294967295;

struct Wallet 
{
	char owner[256];
	unsigned id;
	double fiatMoney;
};

struct Transaction 
{
	long long time;
	unsigned senderId;
	unsigned receiverId;
	double fmiCoins;
};

struct Order 
{
	enum Type { SELL, BUY } type;
	unsigned walletId;
	double fmiCoins;
};


class FMIBank
{
private:
	DynamicArray<Wallet> arrWallets;
	DynamicArray<Transaction> arrTransactions;
	DynamicArray<Order> arrOrders;


	std::fstream fileWallet;
	std::fstream fileTransaction;
	std::fstream fileOrder;

	std::fstream textFileForIdAndTime;

public:
	FMIBank();

	void add_wallet(double fiatMoney, const char* name);
	void transfer(unsigned senderId, unsigned receiverId, double fmiCoins);
	void make_order(Order::Type type, double fmiCoins, unsigned walletId);
	void wallet_info(unsigned walletId);
	void quit();

private:
	void addWalletFromFile();
	unsigned sizeOfFileWallet();
	unsigned generateID();

	void addTransactionFromFile();
	unsigned sizeOfFileTransaction();
	long long timeOfTransaction();

	void addOrderFromFile();
	unsigned sizeOfFileOrder();

	void makeSell(double fmiCoins, unsigned walletId);
	void makeBuy(double fmiCoins, unsigned walletId);
	void makeTransaction(unsigned senderId, unsigned receiverId, double fmiCoins);

	double calculateFmiCoins(unsigned walletId);
	double calculateFiatMoneyForBuy(unsigned walletId);

	bool haveWallet(unsigned walletId) const;

	const char* creatNameByIdAndTime(unsigned walletId, long long time);
	void creatTextFile(const char* nameOfFile, unsigned senderId, unsigned receiverId, double fmiCoins);
	void pushNumberOfTransactionAndCoinsInFile(const char* nameOfFile, double fmiCoins);

	const char* getNameById(unsigned walletId) const;
	double getFiatMoneyById(unsigned walletId) const;
};


