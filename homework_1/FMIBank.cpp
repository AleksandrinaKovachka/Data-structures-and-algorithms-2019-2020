#include "pch.h"
#include "FMIBank.h"

FMIBank::FMIBank()
{
	this->addWalletFromFile();

	this->addTransactionFromFile();

	this->addOrderFromFile();

}

void FMIBank::add_wallet(double fiatMoney, const char* name)
{
	Wallet tempWallet;
	strcpy_s(tempWallet.owner, strlen(name) + 1, name);
	tempWallet.id = this->generateID();
	tempWallet.fiatMoney = fiatMoney;
	this->arrWallets.push(tempWallet);

	//транзакция със системния портфейл
	double firstFmiCoins = fiatMoney / 375.0;
	this->transfer(4294967295, tempWallet.id, firstFmiCoins);

}

void FMIBank::transfer(unsigned senderId, unsigned receiverId, double fmiCoins)
{
	if (this->haveWallet(senderId) && this->haveWallet(receiverId))
	{
		if (this->calculateFmiCoins(senderId) >= fmiCoins )
		{
			Transaction tempTransaction;
			tempTransaction.time = this->timeOfTransaction();
			tempTransaction.senderId = senderId;
			tempTransaction.receiverId = receiverId;
			tempTransaction.fmiCoins = fmiCoins;
			arrTransactions.push(tempTransaction);
		}
		else
		{
			std::cout << "Sender do not have enough fmiCoins to transfer\n";
		}
	}
	else if (this->haveWallet(receiverId) && senderId == systemWallet)
	{
		Transaction tempTransaction;
		tempTransaction.time = this->timeOfTransaction();
		tempTransaction.senderId = senderId;
		tempTransaction.receiverId = receiverId;
		tempTransaction.fmiCoins = fmiCoins;
		arrTransactions.push(tempTransaction);
	}
	else
	{
		std::cout << "The sender id and receiver id is not correct\n";
	}
	
}

void FMIBank::make_order(Order::Type type, double fmiCoins, unsigned walletId)
{
	if (this->haveWallet(walletId))
	{
		if (type == Order::Type::SELL)
		{
			this->makeSell(fmiCoins, walletId);
		}
		else if (type == Order::Type::BUY)
		{
			this->makeBuy(fmiCoins, walletId);
		}
	}
	else
	{
		std::cout << "The id is not correct\n";
	}
	
}

void FMIBank::wallet_info(unsigned walletId)
{
	if (this->haveWallet(walletId))
	{
		std::cout << "Name of owner: " << getNameById(walletId) << std::endl;
		std::cout << "Money of owner: " << getFiatMoneyById(walletId) << std::endl;
		std::cout << "FmiCoins of owner: " << calculateFmiCoins(walletId);
	}
}

void FMIBank::quit()
{
	fileWallet.open("wallets.dat", std::ios::binary | std::ios::out | std::ios::trunc);
	if (fileWallet.is_open() && this->arrWallets.sizeOfArray() != 0)
	{
		for (unsigned i = 0; i < arrWallets.sizeOfArray(); ++i)
		{
			fileWallet.write((const char*)&arrWallets[i], sizeof(Wallet));
		}
		fileWallet.close();
	}
	else
	{
		return;
	}
	
	fileTransaction.open("transactions.dat", std::ios::binary | std::ios::out | std::ios::trunc);
	if (fileTransaction.is_open() && this->arrTransactions.sizeOfArray() != 0)
	{
		for (unsigned i = 0; i < arrTransactions.sizeOfArray(); ++i)
		{
			fileTransaction.write((const char*)&arrTransactions[i], sizeof(Transaction));
		}
		fileTransaction.close();
	}
	else
	{
		return;
	}


	fileOrder.open("orders.dat", std::ios::binary | std::ios::out | std::ios::trunc); // | std::ios::app);
	if (fileOrder.is_open() && this->arrOrders.sizeOfArray() != 0)
	{
		for (unsigned i = 0; i < arrOrders.sizeOfArray(); ++i)
		{
			fileOrder.write((const char*)&arrOrders[i], sizeof(Order));
		}
		fileOrder.close();
	}
	else
	{
		return;
	}
	
}

void FMIBank::addWalletFromFile()
{
	fileWallet.open("wallets.dat", std::ios::binary | std::ios::in);
	if (!fileWallet)
	{
		return;
	}
	unsigned sizeOfFileWallet = this->sizeOfFileWallet();
	if (sizeOfFileWallet != 0)
	{
		for (unsigned i = 0; i < sizeOfFileWallet; ++i)
		{
			Wallet tempWallet;
			fileWallet.read((char*)&tempWallet, sizeof(Wallet));
			arrWallets.push(tempWallet);
		}
	}

	fileWallet.close();
}

unsigned FMIBank::sizeOfFileWallet()
{
	fileWallet.seekp(0, std::ios::end);
	std::streampos sizeOfFile = fileWallet.tellp();
	fileWallet.seekp(0, std::ios::beg);
	std::streampos size = sizeOfFile / sizeof(Wallet);

	return (unsigned)size;
}

unsigned FMIBank::generateID()
{
	unsigned generateId = arrWallets.sizeOfArray() + 1;
	if (generateId != systemWallet)
	{
		return generateId;
	}
	std::cout << "You can not have another wallet - the mamory is full\n";
}

void FMIBank::addTransactionFromFile()
{
	fileTransaction.open("transactions.dat", std::ios::binary | std::ios::in);
	if (!fileTransaction)
	{
		return;
	}
	unsigned sizeOfFileTransaction = this->sizeOfFileTransaction();
	if (sizeOfFileTransaction != 0)
	{
		for (unsigned i = 0; i < sizeOfFileTransaction; ++i)
		{
			Transaction tempTransaction;
			fileTransaction.read((char*)&tempTransaction, sizeof(Transaction));
			arrTransactions.push(tempTransaction);
		}
	}

	fileTransaction.close();
}

unsigned FMIBank::sizeOfFileTransaction()
{
	fileTransaction.seekp(0, std::ios::end);
	std::streampos sizeOfFile = fileTransaction.tellp();
	fileTransaction.seekp(0, std::ios::beg);
	std::streampos size = sizeOfFile / sizeof(Transaction);

	return (unsigned)size;
}

long long FMIBank::timeOfTransaction()
{
	time_t seconds;
	seconds = time(NULL);

	return (long long)seconds;
}

void FMIBank::addOrderFromFile()
{
	fileOrder.open("orders.dat", std::ios::binary | std::ios::in);
	if (!fileOrder)
	{
		return;
	}
	unsigned sizeOfFileOrder = this->sizeOfFileOrder();
	if (sizeOfFileOrder != 0)
	{
		for (unsigned i = 0; i < sizeOfFileOrder; ++i)
		{
			Order tempOrder;
			fileOrder.read((char*)&tempOrder, sizeof(Order));
			arrOrders.push(tempOrder);
		}
	}

	fileOrder.close();
}

unsigned FMIBank::sizeOfFileOrder()
{
	fileOrder.seekp(0, std::ios::end);
	std::streampos sizeOfFile = fileOrder.tellp();
	fileOrder.seekp(0, std::ios::beg);
	std::streampos size = sizeOfFile / sizeof(Order);

	return (unsigned)size;
}

void FMIBank::makeSell(double fmiCoins, unsigned walletId)
{
	double fmiCoinsToSell = this->calculateFmiCoins(walletId);
	if (fmiCoinsToSell < fmiCoins)
	{
		std::cout << "You can not sell " << fmiCoins << " coins because you have only " << fmiCoinsToSell << "coins\n";
	}
	else
	{
		//създава се името на текстовия файл
		long long time = this->timeOfTransaction();
		const char* nameTextFile = { this->creatNameByIdAndTime(walletId, time) };

		//брояч на FMICoins, който ще добавяме в края на текстовия файл
		double countFMICoins = 0;

		//булева променлива, която показва кога сме продали всичките FMICoins
		bool sellAll = false;

		for (unsigned i = 0; i < arrOrders.sizeOfArray(); ++i)
		{
			if (arrOrders[i].type == Order::Type::BUY)
			{
				if (fmiCoins < arrOrders[i].fmiCoins)
				{
					makeTransaction(walletId, arrOrders[i].walletId, fmiCoins);
					creatTextFile(nameTextFile, walletId, arrOrders[i].walletId, fmiCoins);
					sellAll = true;
					countFMICoins += fmiCoins;
					arrOrders[i].fmiCoins = arrOrders[i].fmiCoins - fmiCoins;
					break;
				}
				else if (fmiCoins > arrOrders[i].fmiCoins)
				{
					makeTransaction(walletId, arrOrders[i].walletId, fmiCoins);
					creatTextFile(nameTextFile, walletId, arrOrders[i].walletId, fmiCoins);
					countFMICoins += arrOrders[i].fmiCoins;
					fmiCoins -= arrOrders[i].fmiCoins;
					arrOrders.removeElement(i);
					if (i != 0) //проверка ако не сме били на първия елемент на масива с поръчки
					{
						--i;
					}
				}
				else
				{
					makeTransaction(walletId, arrOrders[i].walletId, fmiCoins);
					creatTextFile(nameTextFile, walletId, arrOrders[i].walletId, fmiCoins);
					sellAll = true;
					countFMICoins += fmiCoins;
					arrOrders.removeElement(i);
					break;
				}
			}
		}

		if (countFMICoins != 0) //стойността на добавените в текстовия файл койни
		{
			this->pushNumberOfTransactionAndCoinsInFile(nameTextFile, countFMICoins);
		}

		if (sellAll == false) // ако има останали койни за продаване се добавят къв масива с нареждания
		{
			Order tempOrder;
			tempOrder.type = Order::Type::SELL;
			tempOrder.walletId = walletId;
			tempOrder.fmiCoins = fmiCoins;
			this->arrOrders.push(tempOrder);
		}
	}
}

void FMIBank::makeBuy(double fmiCoins, unsigned walletId)
{
	double fiatMoneyNeedToBuy = fmiCoins * 375;
	double currentFiatMoney = this->calculateFiatMoneyForBuy(walletId);
	if (fiatMoneyNeedToBuy > currentFiatMoney)
	{
		std::cout << "You have " << currentFiatMoney << " leva and can not buy " << fmiCoins << "coins\n";
	}
	else
	{
		long long time = this->timeOfTransaction();
		const char* nameTextFile = { this->creatNameByIdAndTime(walletId, time) };

		double countFMICoins = 0;
		
		bool canBuy = false;
		for (unsigned i = 0; i < arrOrders.sizeOfArray(); ++i)
		{
			if (arrOrders[i].type == Order::Type::SELL)
			{
				if (arrOrders[i].fmiCoins > fmiCoins)
				{
					makeTransaction(arrOrders[i].walletId, walletId, fmiCoins);
					creatTextFile(nameTextFile, arrOrders[i].walletId, walletId, fmiCoins);
					countFMICoins += fmiCoins;
					canBuy = true;
					arrOrders[i].fmiCoins = arrOrders[i].fmiCoins - fmiCoins;
					break;
				}
				else if(arrOrders[i].type < fmiCoins)
				{
					makeTransaction(arrOrders[i].walletId, walletId, fmiCoins);
					creatTextFile(nameTextFile, arrOrders[i].walletId, walletId, fmiCoins);
					countFMICoins += arrOrders[i].fmiCoins;
					fmiCoins -= arrOrders[i].fmiCoins;
					arrOrders.removeElement(i);
					if (i != 0)
					{
						--i;
					}
				}
				else
				{
					makeTransaction(arrOrders[i].walletId, walletId, fmiCoins);
					creatTextFile(nameTextFile, arrOrders[i].walletId, walletId, fmiCoins);
					countFMICoins += fmiCoins;
					canBuy = true;
					arrOrders.removeElement(i);
					break;
				}
			}
		}

		if (countFMICoins != 0)
		{
			this->pushNumberOfTransactionAndCoinsInFile(nameTextFile, countFMICoins);
		}
		
		if (canBuy == false)
		{
			Order tempOrder;
			tempOrder.type = Order::Type::BUY;
			tempOrder.walletId = walletId;
			tempOrder.fmiCoins = fmiCoins;
			this->arrOrders.push(tempOrder);
		}

	}

}

void FMIBank::makeTransaction(unsigned senderId, unsigned receiverId, double fmiCoins)
{
	this->transfer(senderId, receiverId, fmiCoins);
	double fiatMoneyByCoins = fmiCoins * 375; //парите, които ще се променят в портмонетата на клиентите
	for (unsigned i = 0; i < arrWallets.sizeOfArray(); ++i)
	{
		if (arrWallets[i].id == senderId)
		{
			arrWallets[i].fiatMoney = arrWallets[i].fiatMoney + fiatMoneyByCoins;
		}
		if (arrWallets[i].id == receiverId)
		{
			arrWallets[i].fiatMoney = arrWallets[i].fiatMoney - fiatMoneyByCoins;
		}
	}
}

double FMIBank::calculateFmiCoins(unsigned walletId)
{
	double currentFmiCoins = 0;
	for (unsigned i = 0; i < arrTransactions.sizeOfArray(); ++i)
	{
		if (this->arrTransactions[i].receiverId == walletId)
		{
			currentFmiCoins += this->arrTransactions[i].fmiCoins;
			continue;
		}
		if (this->arrTransactions[i].senderId == walletId)
		{
			currentFmiCoins -= this->arrTransactions[i].fmiCoins;
		}
	}

	for (unsigned i = 0; i < arrOrders.sizeOfArray(); ++i)
	{
		if (this->arrOrders[i].walletId == walletId && this->arrOrders[i].type == Order::Type::SELL)
		{
			currentFmiCoins -= arrOrders[i].fmiCoins;
		}
	}

	return currentFmiCoins;
}

double FMIBank::calculateFiatMoneyForBuy(unsigned walletId)
{
	double currentFiatMoney = 0;
	for (unsigned i = 0; i < arrWallets.sizeOfArray(); ++i)
	{
		if (arrWallets[i].id == walletId)
		{
			currentFiatMoney += arrWallets[i].fiatMoney;
		}
	}

	for (unsigned i = 0; i < arrOrders.sizeOfArray(); ++i)
	{
		if (arrOrders[i].walletId == walletId)
		{
			currentFiatMoney -= (arrOrders[i].fmiCoins * 375);
		}
	}

	return currentFiatMoney;
}

bool FMIBank::haveWallet(unsigned walletId) const
{
	for (unsigned i = 0; i < arrWallets.sizeOfArray(); ++i)
	{
		if (arrWallets[i].id == walletId)
		{
			return true;
		}
	}

	return false;
}

const char* FMIBank::creatNameByIdAndTime(unsigned walletId, long long time)
{
	unsigned walletIdCopy = walletId, countForWallet = 0, countForTime = 0;
	long long timeCopy = time;
	while (walletIdCopy != 0)
	{
		walletIdCopy /= 10;
		++countForWallet;
	}

	while (timeCopy != 0)
	{
		timeCopy /= 10;
		++countForTime;
	}

	int sizeOfArr = countForWallet + countForTime + 6;
	char nameOfTextFile[64];
	if (countForWallet == 1) //ако id е едноцифрено не трябва да се изпълнява долния цикъл
	{
		nameOfTextFile[0] = walletId + '0';
	}
	else
	{
		for (unsigned i = countForWallet - 1; i >= 0; --i)
		{
			nameOfTextFile[i] = walletId % 10 + '0';
			walletId /= 10;
		}
	}
	
	nameOfTextFile[countForWallet] = ' ';
	for (unsigned i = countForWallet + countForTime; i > countForWallet; --i)
	{
		nameOfTextFile[i] = time % 10 + '0';
		time /= 10;
	}

	nameOfTextFile[sizeOfArr - 5] = '.';
	nameOfTextFile[sizeOfArr - 4] = 't';
	nameOfTextFile[sizeOfArr - 3] = 'x';
	nameOfTextFile[sizeOfArr - 2] = 't';
	nameOfTextFile[sizeOfArr - 1] = '\0';

	return nameOfTextFile;
}

void FMIBank::creatTextFile(const char* nameOfFile, unsigned senderId, unsigned receiverId, double fmiCoins)
{
	std::ofstream textFile(nameOfFile);
	if (textFile.is_open())
	{
		textFile << this->getNameById(senderId) << " " << this->getNameById(receiverId) << " " << fmiCoins << "\n";
		textFile.close();
	}
	else
	{
		return;
	}
}

void FMIBank::pushNumberOfTransactionAndCoinsInFile(const char* nameOfFile, double fmiCoins)
{
	unsigned numberOfLine = 0;
	std::string line;
	std::fstream textFile(nameOfFile);

	if (textFile.is_open())
	{
		while (std::getline(textFile, line))
		{
			++numberOfLine;
		}

		textFile << numberOfLine << " lines  and " << fmiCoins * 375 << "lv." << '\n';
		textFile.close();
	}
	else
	{
		return;
	}
}

const char* FMIBank::getNameById(unsigned walletId) const
{
	for (unsigned i = 0; i < this->arrWallets.sizeOfArray(); ++i)
	{
		if (this->arrWallets[i].id == walletId)
		{
			return this->arrWallets[i].owner;
		}
	}
}

double FMIBank::getFiatMoneyById(unsigned walletId) const
{
	for (unsigned i = 0; i < this->arrWallets.sizeOfArray(); ++i)
	{
		if (this->arrWallets[i].id == walletId)
		{
			return this->arrWallets[i].fiatMoney;
		}
	}
}