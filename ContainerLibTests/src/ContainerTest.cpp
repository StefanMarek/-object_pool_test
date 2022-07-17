/*****************************************************************************/
/*!
	@author			Stefan Marek
	@description   
*/
/*****************************************************************************/

#include "ContainerTest.h"

#include <random>

void init_clustered_data_f(CELL::OPOOL::TPool<CELL::SPOOL::string_t>::Type& new_clustered_data,
	CELL::OPOOL::TPool<std::string>::Type& new_clustered_data_std)
{
	CELL::OPOOL::TPool<CELL::SPOOL::string_t>::Type new_clustered_data1;
	CELL::OPOOL::TPool<std::string>::Type new_clustered_data_std1;
	calcClusteredData<CELL::SPOOL::string_t>(new_clustered_data1);
	calcClusteredData<std::string>(new_clustered_data_std1);
	new_clustered_data = new_clustered_data1;
	new_clustered_data_std = new_clustered_data_std1;
}

bool testString(bool stream_results, const random_pool_type& TEST_VALUES)
{
	//testBuffer();

	typedef TestKeyType hash_value_t;

	//const size_t hash_size = 1024;
	const size_t hash_size = TEST_VALUES.size();

	TIMER_TOOLS::ContainerTimer systimer323143333;
	systimer323143333.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		CELL::ALLOC::BUFFERPOD::TPODObjectPoolBuffer<char, 16> stra;
		char* sza = stra.mBuffer;

		char buffer[sizeof(unsigned int) * 8];
		// sprintf(buffer, "%05d", i);
		sprintf(buffer, "%05zu", i);

		CELL::SPOOL::spcopy(sza, buffer, 8);
		sza[9] = '\0';

		const size_t sz1 = CELL::SPOOL::splength(sza);
		//const size_t sz = sizeof(stra);
	}
	systimer323143333.endTiming();
	systimer323143333.print("Test POD", STREAM);

	TIMER_TOOLS::ContainerTimer systimer323143;
	systimer323143.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		std::string stra;
		char* sza = (char*)stra.c_str();//._Get_data()._Bx._Buf;

		char buffer[sizeof(unsigned int) * 8];
		// sprintf(buffer, "%05d", i);
		sprintf(buffer, "%05zu", i);

		char_traits<char>::copy(sza, buffer, 8);
		sza[9] = '\0';

		const size_t sz1 = char_traits<char>::length(sza);
		//const size_t sz = sizeof(stra);
	}
	systimer323143.endTiming();
	systimer323143.print("Test STD", STREAM);

	//TPool<heap_value_t>::Type values(TEST_VALUES.begin(), TEST_VALUES.begin() + hash_size);

	TPool<string_t>::Type keys(hash_size);
	std::vector<std::string> skeys(hash_size);

	for (size_t i = 0; i < hash_size; ++i)
	{
		//string_t key = SPTOOLS::number_to_string(i) + "_" + SPTOOLS::float_to_string(TEST_VALUES[i]);

		char buffer[sizeof(unsigned int) * 8];
		// sprintf(buffer, "%05d", i);
		sprintf(buffer, "%05zu", i);

		string_t key = string_t(buffer) + "_" + SPTOOLS::float_to_string(TEST_VALUES[i]);
		string_t key1;
		key1 = key;

		keys[i] = key;
		if (keys[i] != key)
		{
			STREAM << "Test String keys STD break" << std::endl;
			break;
		}
	}
	for (size_t i = 0; i < hash_size; ++i)
	{
		//string_t key = SPTOOLS::number_to_string(i) + "_" + SPTOOLS::float_to_string(TEST_VALUES[i]);

		char buffer[sizeof(unsigned int) * 8];
		// sprintf(buffer, "%05d", i);
		sprintf(buffer, "%05zu", i);

		std::string key = std::string(buffer) + "_" + to_string(TEST_VALUES[i]);

		skeys[i] = key;
		if (skeys[i] != key)
		{
			STREAM << "Test String skey CELL break" << std::endl;
			break;
		}
	}

	string_t key("0000_021346");
	TIMER_TOOLS::ContainerTimer systimer111;
	systimer111.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{

		//std::string key1 = std::string("0000_021346");
		if (keys[i] == key)
		{
			STREAM << "Test String Compare break" << std::endl;
			break;
		}
	}
	systimer111.endTiming();
	systimer111.print("Test String Compare CELL", STREAM);

	std::string key1("0000_021346");
	TIMER_TOOLS::ContainerTimer systimer101;
	systimer101.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{

		//std::string key1 = std::string("0000_021346");
		if (skeys[i] == key1)
		{
			STREAM << "Test String Compare break" << std::endl;
			break;
		}
	}
	systimer101.endTiming();
	systimer101.print("Test String Compare STD", STREAM);

	TIMER_TOOLS::ContainerTimer systimer1111;
	systimer1111.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		//string_t * key122 = new string_t("0000_021346");
		string_t key122("0000_021346");
		//if (keys[i] == key122)
		//	break;
	}
	systimer1111.endTiming();
	systimer1111.print("Test String string CELL", STREAM);

	TIMER_TOOLS::ContainerTimer systimer1011;
	systimer1011.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string * key11 = new std::string("0000_021346");
		std::string key11("0000_021346");
		//if (skeys[i] == key11)
		//	break;
	}
	systimer1011.endTiming();
	systimer1011.print("Test String string STD", STREAM);

	TIMER_TOOLS::ContainerTimer systimer111144444;
	systimer111144444.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		//string_t * key122 = new string_t("0000_021346");
		string_t key122("D:\\Visual Studio Online\\ElementsFramework\\ElementsFramework\\ContainerTest\\0000_021346");
		//if (keys[i] == key122)
		//	break;
	}
	systimer111144444.endTiming();
	systimer111144444.print("Test String string CELL Lang", STREAM);

	TIMER_TOOLS::ContainerTimer systimer1011333;
	systimer1011333.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string * key11 = new std::string("0000_021346");
		std::string key11("D:\\Visual Studio Online\\ElementsFramework\\ElementsFramework\\ContainerTest\\0000_021346");
		//if (skeys[i] == key11)
		//	break;
	}
	systimer1011333.endTiming();
	systimer1011333.print("Test String string STD Lang", STREAM);

	//=============================================================

	TIMER_TOOLS::ContainerTimer systimer11113;
	systimer11113.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		//string_t * key122 = new string_t("0000_021346");
		string_t key122 = string_t("0000_021346");
		string_t key12;
		key12 = key122;
		if (keys[i] == key122)
		{
			STREAM << "Test String string = CELL break" << std::endl;
			break;
		}
	}
	systimer11113.endTiming();
	systimer11113.print("Test String string = CELL", STREAM);

	TIMER_TOOLS::ContainerTimer systimer10112;
	systimer10112.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string * key11 = new std::string("0000_021346");
		std::string key11 = std::string("0000_021346");
		std::string key12;
		key12 = key11;
		if (skeys[i] == key11)
		{
			STREAM << "Test String string = STD break" << std::endl;
			break;
		}
	}
	systimer10112.endTiming();
	systimer10112.print("Test String string = STD", STREAM);

	TIMER_TOOLS::ContainerTimer systimer11114444434;
	systimer11114444434.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		//string_t * key122 = new string_t("0000_021346");
		string_t key122 = string_t("D:\\Visual Studio Online\\ElementsFramework\\ElementsFramework\\ContainerTest\\0000_021346");
		string_t key12;
		key12 = key122;
		if (keys[i] == key122)
		{
			STREAM << "Test String string = CELL Lang break" << std::endl;
			break;
		}
	}
	systimer11114444434.endTiming();
	systimer11114444434.print("Test String string = CELL Lang", STREAM);

	TIMER_TOOLS::ContainerTimer systimer101133356;
	systimer101133356.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string * key11 = new std::string("0000_021346");
		std::string key11 = std::string("D:\\Visual Studio Online\\ElementsFramework\\ElementsFramework\\ContainerTest\\0000_021346");
		std::string key12;
		key12 = key11;
		if (skeys[i] == key11)
		{
			STREAM << "Test String string = STD Lang  break" << std::endl;
			break;
		}
	}
	systimer101133356.endTiming();
	systimer101133356.print("Test String string = STD Lang", STREAM);

	return true;
}
//=============================================================================
//*****************************************************************************
//=============== CONSOLE APPLICATION ENTRY POINT =============================
//*****************************************************************************
	int main(int argc, char* argv[])
	{
		random_pool_type random_values;
		const uint64_t TEST_VALUES_SIZE = 1024 * 16;

		std::mt19937 generator;
		generator.seed(std::random_device()());
		std::uniform_real_distribution<random_value_type> distribution = std::uniform_real_distribution<random_value_type>(0.0, 1.0);
		random_value_type val = 0;
		for (size_t i = 0; i < TEST_VALUES_SIZE; ++i)
		{
			val = distribution(generator);
			random_values.push_back(val);
		}

		CELL::OPOOL::TPool<CELL::SPOOL::string_t>::Type clustered_data;
		CELL::OPOOL::TPool<std::string>::Type clustered_data_std;
		init_clustered_data_f(clustered_data, clustered_data_std);

		performanceTestsForPool();
		growingTestsForPool();

		testFIFOPool(true, random_values);
		testLIFOPool(true, random_values);

		testHashTable(true, random_values);
		testHashTableSTDString(true, random_values);
		testClusteredHashTable(true, clustered_data);

		testString(true, random_values);
		return EXIT_SUCCESS;
	}
//=============================================================================
