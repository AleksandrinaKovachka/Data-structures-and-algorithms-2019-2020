// homework_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string.h>
#include "DynamicArray.h"
#include "FMIBank.h"

int main()
{	
	FMIBank bank;
	char input[28];
	do
	{
		std::cout << "Input comand: ";
		std::cin >> input;
		if (strcmp(input, "quit") == 0)
		{
			std::cout << "The program save the information and turn off!\n";

			bank.quit();
			break;
		}
		else if (strcmp(input, "add-wallet") == 0)
		{
			char name[256];
			double fiatMoney;
			std::cout << "Input name: ";
			std::cin.ignore();
			std::cin.getline(name, 255);
			std::cout << "Input fiat money: ";
			std::cin >> fiatMoney;

			bank.add_wallet(fiatMoney, name);
		}
		else if (strcmp(input, "make-order") == 0)
		{
			char type[5];
			double fmiCoins;
			unsigned walletId;
			Order::Type typeOrder;

			do
			{
				std::cout << "Input the type of order: ";
				std::cin >> type;
			} while (strcmp(type, "SELL") != 0 && strcmp(type, "BUY") != 0);

			if (strcmp(type, "SELL") == 0)
			{
				typeOrder = Order::Type::SELL;
			}
			else
			{
				typeOrder = Order::Type::BUY;
			}
			std::cout << "Input fmi coins: ";
			std::cin >> fmiCoins;
			std::cout << "Input wallet id: ";
			std::cin >> walletId;

			bank.make_order(typeOrder, fmiCoins, walletId);
		}
		else if (strcmp(input, "transfer") == 0)
		{
			unsigned senderId, receiverId;
			double fmiCoins;
			std::cout << "Input the sender id: ";
			std::cin >> senderId;
			std::cout << "Input the receiver id: ";
			std::cin >> receiverId;
			std::cout << "Input fmi coins: ";
			std::cin >> fmiCoins;

			bank.transfer(senderId, receiverId, fmiCoins);
		}
		else if (strcmp(input, "wallet-info") == 0)
		{
			unsigned walletId;
			std::cout << "Input wallet id: ";
			std::cin >> walletId;

			bank.wallet_info(walletId);
		}
		else if (strcmp(input, "attract-investors") == 0)
		{
			std::cout << "This function do not exist\n";
		}
		else
		{
			std::cout << "Invalid comand\n";
		}
	} while (true);

	return 0;
}
