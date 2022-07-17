
#include "TestHash.h"
#include <unordered_map>
#include <functional>
#include <type_traits>

//=============================================================================
struct hash_string_t
{
	typedef CELL::SPOOL::string_t argument_type;
	typedef size_t result_type;
	std::hash<char> hashf;

	size_t operator()(const CELL::SPOOL::string_t & _Keyval) const
	{	// hash _Keyval to size_t value by pseudorandomizing transform
		return CELL::TOOLS::HASH::STL_Hash_seq((const unsigned char *)_Keyval.c_str(), _Keyval.size());
		//return (std::_Hash_bytes((const unsigned char *)_Keyval.c_str(), _Keyval.size()));
	}
};
//=============================================================================
bool testClusteredHashTable(bool stream_results, CELL::OPOOL::TPool<CELL::SPOOL::string_t>::Type & clustered_data)
{
	//typedef std::string string_t;
	//typedef CELL::SPOOL::string_t string_t;

	typedef TestKeyType hash_value_t;

	//const size_t hash_size = 1024;
	const size_t hash_size = clustered_data.size();

	CELL::OPOOL::TPool<CELL::SPOOL::string_t>::Type & keys = clustered_data;
	//std::vector<std::string> skeys(hash_size);

	TIMER_TOOLS::ContainerTimer systimer212121;
	systimer212121.beginTiming();

	for (size_t i = 0; i < hash_size; ++i)
	{
		string_t & key = keys[i];
		size_t hash_v = CELL::TOOLS::HASH::jenkins_one_at_a_time_hash_index(key.c_str(), key.size(), hash_size);
		if (hash_v > hash_size - 1)
		{
			STREAM << "Test jenkins_one_at_a_time_hash break" << std::endl;
			break;
		}
	}
	systimer212121.endTiming();
	systimer212121.print("Test jenkins_one_at_a_time_hash", STREAM);

	TIMER_TOOLS::ContainerTimer systimer2121213131222666;
	systimer2121213131222666.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		string_t & key = keys[i];
		size_t hash_v = CELL::TOOLS::HASH::THashIndexChars(key.c_str(), key.size(), hash_size);
		if (hash_v > hash_size - 1)
		{
			STREAM << "Test BasicHash break" << std::endl;
			break;
		}
	}
	systimer2121213131222666.endTiming();
	systimer2121213131222666.print("Test Basic Hash", STREAM);

	TIMER_TOOLS::ContainerTimer systimer2121213131;
	systimer2121213131.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		string_t & key = keys[i];
		size_t hash_v = CELL::TOOLS::HASH::STL_Hash_Index_seq(key.c_str(), key.size(), hash_size);
		if (hash_v > hash_size - 1)
		{
			STREAM << "Test STL_Hash_seq break" << std::endl;
			break;
		}
	}
	systimer2121213131.endTiming();
	systimer2121213131.print("Test STL_Hash_seq", STREAM);

	//=============================================================================

	//typedef std::unordered_map< std::string, hash_value_t *> hash_t1;
	//typedef std::pair< std::string, hash_value_t *> hash_p1;

	typedef std::pair< string_t, hash_value_t *> hash_p1;

	//typedef std::unordered_map<CELL::SPOOL::string_t, GLTexture2D *, hash_string_t> tex_map_t;
	typedef std::unordered_map< string_t, hash_value_t *, hash_string_t> hash_t1;
	//typedef std::unordered_map< string_t, hash_value_t *> hash_t1;

	typedef typename THPool< string_t, hash_value_t *>::Type hash_pt;
	//typedef THTable< string_t, hash_value_t *>::Type hash_t;

	typedef std::pair< uint32_t, hash_value_t*> mesh_hash_p1;

	hash_value_t * val = nullptr;

	//if (stream_results)
	//	STREAM << std::endl << "Test Case: Hash Basics " << std::endl;

	//if (stream_results)
	//	STREAM << std::endl << "Test Case OUT: Hash Basics  " << std::endl;

	//=============================================================================

	if (stream_results)
		STREAM << std::endl << "Test Case: THashTable " << std::endl;

	TIMER_TOOLS::ContainerTimer systimer1;
	systimer1.beginTiming();

#ifdef PREALLOC_HASH
	hash_t1 hash_table1(hash_size);
#else
	hash_t1 hash_table1;
#endif

	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key(keys[i].c_str());
		string_t key(keys[i]);

		//TestType value(i, string_t(key.c_str(), key.length()));
		TestType value(i, key);

		val = new hash_value_t(keys[i], value);

		hash_table1.insert(hash_p1(key, val));
	}
	systimer1.endTiming();
	systimer1.print("Test unordered_map", STREAM);

	TIMER_TOOLS::ContainerTimer systimer234;
	systimer234.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key(keys[i].c_str());
		string_t key(keys[i]);
		hash_value_t * val1 = nullptr;
		hash_t1::const_iterator iter = hash_table1.find(key);
		if (iter == hash_table1.end())
		{
			STREAM << std::endl << "unordered_map broken at: " << key.c_str() << " with: " << (*iter).first.c_str() << std::endl;
			break;
		}
	}
	systimer234.endTiming();
	systimer234.print("Test unordered_map find", STREAM);

	TIMER_TOOLS::ContainerTimer systimer234111111;
	systimer234111111.beginTiming();

	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key(keys[i].c_str());
		string_t key(keys[i]);
		hash_table1.erase(key);
		hash_value_t * val1 = nullptr;

		hash_t1::const_iterator iter = hash_table1.find(key);
		if (iter != hash_table1.end())
		{
			STREAM << std::endl << "unordered_map broken at: " << key.c_str() << " with: " << (*iter).first.c_str() << std::endl;
			break;
		}
	}
	systimer234111111.endTiming();
	systimer234111111.print("Test unordered_map erase", STREAM);

	STREAM << std::endl << "unordered_map size: " << hash_table1.size() << std::endl;


	//TIMER_TOOLS::ContainerTimer systimer21;
	//systimer21.beginTiming();

	//hash_t hash_table_init(hash_size);

	//systimer21.endTiming();
	//systimer21.print("Test Hash Table Init", STREAM);

	//=============================================================================

	TIMER_TOOLS::ContainerTimer systimer2;
	systimer2.beginTiming();

#ifdef PREALLOC_HASH
	hash_pt hash_table(hash_size);
#else
	hash_pt hash_table;
#endif

	hash_value_t * val1 = nullptr;
	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key = std::string(keys[i].c_str());
		string_t & key = keys[i];
		TestType value(i, key);

		val1 = new hash_value_t(key, value);

		hash_table.insert(key, val1);
	}
	//#ifdef _DEBUG
	//	const size_t colls_counter = hash_t::HASH_TABLE_COLLISION_COUNTER;
	//#endif
	//const size_t colls_counter = hash_pt::HASH_POOL_COLLISION_COUNTER;
	//STREAM << std::endl << "HASH_TABLE_COLLISION_COUNTER: " << colls_counter << std::endl;

	systimer2.endTiming();
	systimer2.print("Test Hash Table", STREAM);

	size_t active_slots = hash_table.activeSlots();

	TIMER_TOOLS::ContainerTimer systimer23;
	systimer23.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key = std::string(keys[i].c_str());
		string_t & key = keys[i];
		hash_value_t * val1 = nullptr;
		if (!hash_table.find(key, val1))
		{
			std::string key11 = std::string(keys[i].c_str());
			STREAM << std::endl << "Hash Table broken at: " << key11 << std::endl;
			break;
		}
	}
	systimer23.endTiming();
	systimer23.print("Test Hash Table find", STREAM);

	hash_pt::pool_type hash_pool;
	hash_table.activeElements(hash_pool);
  
	TIMER_TOOLS::ContainerTimer systimer231122334455;
	systimer231122334455.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key = std::string(keys[i].c_str());
		string_t & key = keys[i];

		if (!hash_table.remove(key))
		{
			std::string key11 = std::string(keys[i].c_str());
			STREAM << std::endl << "Hash Table broken at: " << key11 << std::endl;
			break;
		}
		hash_value_t * val1 = nullptr;
		if (hash_table.find(key, val1))
		{
			std::string key11 = std::string(keys[i].c_str());
			STREAM << std::endl << "Hash Table broken at: " << key11 << std::endl;
			break;
		}
	}
	systimer231122334455.endTiming();
	systimer231122334455.print("Test Hash Table remove", STREAM);

	STREAM << std::endl << "Hash Table size: " << hash_table.size() << std::endl;
	size_t active_slots1 = hash_table.activeSlots();
	STREAM << std::endl << "Hash Table active_slots: " << active_slots1 << std::endl;

	hash_pt::pool_type hash_pool22;
	hash_table.activeElements(hash_pool22);

	
	if (stream_results)
		STREAM << std::endl << "Test Case OUT: THashTable " << std::endl;

	return true;
}

bool testHashTable(bool stream_results, const random_pool_type & TEST_VALUES)
{
	//typedef std::string string_t;
	typedef CELL::SPOOL::string_t string_t;

	typedef TestKeyType hash_value_t;
	typedef TestKeyTypeInt hash_int_t;

	//const size_t hash_size = 1024;
	const size_t hash_size = TEST_VALUES.size();

	TPool<string_t>::Type keys(hash_size);
	//std::vector<std::string> skeys(hash_size);

	for (size_t i = 0; i < hash_size; ++i)
	{
		char buffer[sizeof(unsigned int) * 8];
		// sprintf(buffer, "%05d", i);
		sprintf(buffer, "%05zu", i);

		//string_t key = string_t(buffer) + "_" + to_string(TEST_VALUES[i]);
		string_t key = string_t(buffer) + "_" + SPTOOLS::float_to_string(TEST_VALUES[i]);

		keys[i] = key;
		if (keys[i] != key)
			break;
	}

	TIMER_TOOLS::ContainerTimer systimer212121;
	systimer212121.beginTiming();

	for (size_t i = 0; i < hash_size; ++i)
	{
		string_t & key = keys[i];
		size_t hash_v = CELL::TOOLS::HASH::jenkins_one_at_a_time_hash_index(key.c_str(), key.size(), hash_size);
		if (hash_v > hash_size - 1)
		{
			STREAM << "Test jenkins_one_at_a_time_hash break" << std::endl;
			break;
		}
	}
	systimer212121.endTiming();
	systimer212121.print("Test jenkins_one_at_a_time_hash", STREAM);

	TIMER_TOOLS::ContainerTimer systimer2121213131222666;
	systimer2121213131222666.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		string_t & key = keys[i];
		size_t hash_v = CELL::TOOLS::HASH::THashIndexChars(key.c_str(), key.size(), hash_size);
		if (hash_v > hash_size - 1)
		{
			STREAM << "Test BasicHash break" << std::endl;
			break;
		}
	}
	systimer2121213131222666.endTiming();
	systimer2121213131222666.print("Test Basic Hash", STREAM);

	TIMER_TOOLS::ContainerTimer systimer2121213131;
	systimer2121213131.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		string_t & key = keys[i];
		size_t hash_v = CELL::TOOLS::HASH::STL_Hash_Index_seq(key.c_str(), key.size(), hash_size);
		if (hash_v > hash_size - 1)
		{
			STREAM << "Test STL_Hash_seq break" << std::endl;
			break;
		}
	}
	systimer2121213131.endTiming();
	systimer2121213131.print("Test STL_Hash_seq", STREAM);

	//=============================================================================

	typedef std::pair< string_t, hash_value_t *> hash_p1;

	typedef std::unordered_map< string_t, hash_value_t *, CELL::TOOLS::HASH::TMapHashFunc> hash_t1;

	typedef typename THPool< string_t, hash_value_t *>::Type hash_pt;
	hash_value_t * val = nullptr;

	//=============================================================================

	if (stream_results)
		STREAM << std::endl << "Test Case: THashTable " << std::endl;

	TIMER_TOOLS::ContainerTimer systimer1;
	systimer1.beginTiming();

#ifdef PREALLOC_HASH
	hash_t1 hash_table1(hash_size);
#else
	hash_t1 hash_table1;
#endif

	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key(keys[i].c_str());
		string_t key(keys[i]);

		//TestType value(i, string_t(key.c_str(), key.length()));
		TestType value(i, key);

		val = new hash_value_t(keys[i], value);

		hash_table1.insert(hash_p1(key, val));
	}
	systimer1.endTiming();
	systimer1.print("Test unordered_map", STREAM);
	
	TIMER_TOOLS::ContainerTimer systimer234;
	systimer234.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key(keys[i].c_str());
		string_t key(keys[i]);
		hash_value_t * val1 = nullptr;
		hash_t1::const_iterator iter = hash_table1.find(key);
		if (iter == hash_table1.end())
		{
			STREAM << std::endl << "unordered_map broken at: " << key.c_str() << " with: " << (*iter).first.c_str() << std::endl;
			break;
		}
	}
	systimer234.endTiming();
	systimer234.print("Test unordered_map find", STREAM);

	TIMER_TOOLS::ContainerTimer systimer234111111;
	systimer234111111.beginTiming();

	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key(keys[i].c_str());
		string_t key(keys[i]);
		hash_table1.erase(key);
		hash_value_t * val1 = nullptr;

		hash_t1::const_iterator iter = hash_table1.find(key);
		if (iter != hash_table1.end())
		{
			STREAM << std::endl << "unordered_map broken at: " << key.c_str() << " with: " << (*iter).first.c_str() << std::endl;
			break;
		}
	}
	systimer234111111.endTiming();
	systimer234111111.print("Test unordered_map erase", STREAM);

	STREAM << std::endl << "unordered_map size: " << hash_table1.size() << std::endl;


	//TIMER_TOOLS::ContainerTimer systimer21;
	//systimer21.beginTiming();

	//hash_t hash_table_init(hash_size);

	//systimer21.endTiming();
	//systimer21.print("Test Hash Table Init", STREAM);

	//=============================================================================

	TIMER_TOOLS::ContainerTimer systimer2;
	systimer2.beginTiming();

#ifdef PREALLOC_HASH
	hash_pt hash_table(hash_size);
#else
	hash_pt hash_table;
#endif

	hash_value_t * val1 = nullptr;
	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key = std::string(keys[i].c_str());
		string_t & key = keys[i];
		TestType value(i, key);

		val1 = new hash_value_t(key, value);

		hash_table.insert(key, val1);
	}
//#ifdef _DEBUG
//	const size_t colls_counter = hash_t::HASH_TABLE_COLLISION_COUNTER;
//#endif
	//const size_t colls_counter = hash_pt::HASH_POOL_COLLISION_COUNTER;
	//STREAM << std::endl << "HASH_TABLE_COLLISION_COUNTER: " << colls_counter << std::endl;

	systimer2.endTiming();
	systimer2.print("Test Hash Table", STREAM);

	size_t active_slots = hash_table.activeSlots();

	TIMER_TOOLS::ContainerTimer systimer23;
	systimer23.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key = std::string(keys[i].c_str());
		string_t & key = keys[i];
		hash_value_t * val1 = nullptr;
		if (!hash_table.find(key, val1))
		{
			std::string key11 = std::string(keys[i].c_str());
			STREAM << std::endl << "Hash Table broken at: " << key11 << std::endl;
			break;
		}
	}
	systimer23.endTiming();
	systimer23.print("Test Hash Table find", STREAM);

	//hash_pt::pool_type hash_pool;
	//hash_table.activeElements(hash_pool);

	//hash_pt::pool_type hash_pool2;
	//typedef hash_pt::iterator hash_iterator;
	//hash_iterator iter = hash_table.begin();
	//hash_iterator iter_end = hash_table.end();
	//while (iter != iter_end)
	//{
	//	hash_pool2.push_back(*iter);
	//	++iter;
	//}

	STREAM << std::endl << "Hash Table size: " << hash_table.size() << std::endl;
	size_t active_slots13 = hash_table.activeSlots();
	STREAM << std::endl << "Hash Table active_slots: " << active_slots13 << std::endl;

	TIMER_TOOLS::ContainerTimer systimer231122334455;
	systimer231122334455.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key = std::string(keys[i].c_str());
		string_t & key = keys[i];

		hash_table.remove(key);

		hash_value_t * val1 = nullptr;
		if (hash_table.find(key, val1))
		{
			std::string key11 = std::string(keys[i].c_str());
			STREAM << std::endl << "Hash Table broken at: " << key11 << std::endl;
			break;
		}
	}
	systimer231122334455.endTiming();
	systimer231122334455.print("Test Hash Table remove", STREAM);
	
	STREAM << std::endl << "Hash Table size: " << hash_table.size() << std::endl;
	size_t active_slots1 = hash_table.activeSlots();
	STREAM << std::endl << "Hash Table active_slots: " << active_slots1 << std::endl;

	//hash_t::pool_type hash_pool22;
	//hash_table.activeElements(hash_pool22);

	//=============================================================================

	typedef typename CELL::HPOOL::THPool<uint32_t, hash_int_t*>::Type mesh_object_map_t;

	TIMER_TOOLS::ContainerTimer systimer3;
	systimer3.beginTiming();

#ifdef PREALLOC_HASH
	mesh_object_map_t hash_table2(hash_size);
#else
	mesh_object_map_t hash_table2;
#endif

	hash_int_t* val12 = nullptr;
	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key = std::string(keys[i].c_str());
		string_t& key = keys[i];
		TestType value(i, key);

		val12 = new hash_int_t(i, value);

		hash_table2.insert(i, val12);
	}

	systimer3.endTiming();
	systimer3.print("Test Hash Table2", STREAM);

	size_t active_slots2 = hash_table2.activeSlots();

	TIMER_TOOLS::ContainerTimer systimer32;
	systimer32.beginTiming();
	for (uint32_t i = 0; i < hash_size; ++i)
	{
		//std::string key = std::string(keys[i].c_str());
		string_t& key = keys[i];
		hash_int_t* val1 = nullptr;
		if (!hash_table2.find(i, val1))
		{
			std::string key11 = std::string(keys[i].c_str());
			STREAM << std::endl << "Hash Table 2 broken at: " << to_string(i) << " keys " << key11 << std::endl;
			break;
		}
		if (val1->getKey() != i)
		{
			std::string key11 = std::string(keys[i].c_str());
			STREAM << std::endl << "Hash Table 2 broken at: " << to_string(i) << " keys " << key11 << std::endl;
			break;
		}
	}
	systimer32.endTiming();
	systimer32.print("Test Hash Table2 find", STREAM);

	mesh_object_map_t::pool_type hash_pool3;
	hash_table2.activeElements(hash_pool3);

	TIMER_TOOLS::ContainerTimer systimer33;
	systimer33.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key = std::string(keys[i].c_str());
		string_t& key = keys[i];

		if (!hash_table2.remove(i))
		{
			std::string key11 = std::string(keys[i].c_str());
			STREAM << std::endl << "Hash Table 2 broken at: " << key11 << std::endl;
			break;
		}
		hash_int_t* val1 = nullptr;
		if (hash_table2.find(i, val1))
		{
			std::string key11 = std::string(keys[i].c_str());
			STREAM << std::endl << "Hash Table 2 broken at: " << key11 << std::endl;
			break;
		}
	}
	systimer33.endTiming();
	systimer33.print("Test Hash Table 2 remove", STREAM);

	STREAM << std::endl << "Hash Table 2 size: " << hash_table2.size() << std::endl;
	size_t active_slots31 = hash_table2.activeSlots();
	STREAM << std::endl << "Hash Table 2 active_slots: " << active_slots31 << std::endl;

	mesh_object_map_t::pool_type hash_pool32;
	hash_table2.activeElements(hash_pool32);

	if (stream_results)
		STREAM << std::endl << "Test Case OUT: THashTable " << std::endl;

	return true;
}

bool testHashTableSTDString(bool stream_results, const random_pool_type & TEST_VALUES)
{
	typedef std::string string_t;

	typedef TestKeyTypeSTDString hash_value_t;

	//const size_t hash_size = 1024;
	const size_t hash_size = TEST_VALUES.size();

	TPool<string_t>::Type keys(hash_size);
	//std::vector<std::string> skeys(hash_size);

	for (size_t i = 0; i < hash_size; ++i)
	{
		char buffer[sizeof(unsigned int) * 8];
		// sprintf(buffer, "%05d", i);
		sprintf(buffer, "%05zu", i);

		string_t key = string_t(buffer) + "_" + to_string(TEST_VALUES[i]);

		keys[i] = key;
		if (keys[i] != key)
			break;
	}

	TIMER_TOOLS::ContainerTimer systimer212121;
	systimer212121.beginTiming();

	for (size_t i = 0; i < hash_size; ++i)
	{
		string_t & key = keys[i];
		size_t hash_v = CELL::TOOLS::HASH::jenkins_one_at_a_time_hash_index(key.c_str(), key.size(), hash_size);
		if (hash_v > hash_size - 1)
		{
			STREAM << "Test jenkins_one_at_a_time_hash break" << std::endl;
			break;
		}
	}
	systimer212121.endTiming();
	systimer212121.print("Test jenkins_one_at_a_time_hash", STREAM);

	TIMER_TOOLS::ContainerTimer systimer2121213131222666;
	systimer2121213131222666.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		string_t & key = keys[i];
		size_t hash_v = CELL::TOOLS::HASH::THashIndexChars(key.c_str(), key.size(), hash_size);
		if (hash_v > hash_size - 1)
		{
			STREAM << "Test BasicHash break" << std::endl;
			break;
		}
	}
	systimer2121213131222666.endTiming();
	systimer2121213131222666.print("Test Basic Hash", STREAM);

	TIMER_TOOLS::ContainerTimer systimer2121213131;
	systimer2121213131.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		string_t & key = keys[i];
		size_t hash_v = CELL::TOOLS::HASH::STL_Hash_Index_seq(key.c_str(), key.size(), hash_size);
		if (hash_v > hash_size - 1)
		{
			STREAM << "Test STL_Hash_seq break" << std::endl;
			break;
		}
	}
	systimer2121213131.endTiming();
	systimer2121213131.print("Test STL_Hash_seq", STREAM);

	//=============================================================================

	typedef std::pair< string_t, hash_value_t *> hash_p1;

	typedef std::unordered_map< string_t, hash_value_t *> hash_t1;

	typedef typename THPool< string_t, hash_value_t *>::Type hash_pt;
	hash_value_t * val = nullptr;

	//=============================================================================

	if (stream_results)
		STREAM << std::endl << "Test Case: THashTable " << std::endl;

	TIMER_TOOLS::ContainerTimer systimer1;
	systimer1.beginTiming();

#ifdef PREALLOC_HASH
	hash_t1 hash_table1(hash_size);
#else
	hash_t1 hash_table1;
#endif

	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key(keys[i].c_str());
		string_t key(keys[i]);

		//TestType value(i, string_t(key.c_str(), key.length()));
		TestTypeSTDString value(i, key);

		val = new hash_value_t(keys[i], value);

		hash_table1.insert(hash_p1(key, val));
	}
	systimer1.endTiming();
	systimer1.print("Test unordered_map", STREAM);

	TIMER_TOOLS::ContainerTimer systimer234;
	systimer234.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key(keys[i].c_str());
		string_t key(keys[i]);
		hash_value_t * val1 = nullptr;
		hash_t1::const_iterator iter = hash_table1.find(key);
		if (iter == hash_table1.end())
		{
			STREAM << std::endl << "unordered_map broken at: " << key.c_str() << " with: " << (*iter).first.c_str() << std::endl;
			break;
		}
	}
	systimer234.endTiming();
	systimer234.print("Test unordered_map find", STREAM);

	TIMER_TOOLS::ContainerTimer systimer234111111;
	systimer234111111.beginTiming();

	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key(keys[i].c_str());
		string_t key(keys[i]);
		hash_table1.erase(key);
		hash_value_t * val1 = nullptr;

		hash_t1::const_iterator iter = hash_table1.find(key);
		if (iter != hash_table1.end())
		{
			STREAM << std::endl << "unordered_map broken at: " << key.c_str() << " with: " << (*iter).first.c_str() << std::endl;
			break;
		}
	}
	systimer234111111.endTiming();
	systimer234111111.print("Test unordered_map erase", STREAM);

	STREAM << std::endl << "unordered_map size: " << hash_table1.size() << std::endl;


	//TIMER_TOOLS::ContainerTimer systimer21;
	//systimer21.beginTiming();

	//hash_t hash_table_init(hash_size);

	//systimer21.endTiming();
	//systimer21.print("Test Hash Table Init", STREAM);

	//=============================================================================

	TIMER_TOOLS::ContainerTimer systimer2;
	systimer2.beginTiming();

#ifdef PREALLOC_HASH
	hash_pt hash_table(hash_size);
#else
	hash_pt hash_table;
#endif

	hash_value_t * val1 = nullptr;
	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key = std::string(keys[i].c_str());
		string_t & key = keys[i];
		TestTypeSTDString value(i, key);

		val1 = new hash_value_t(key, value);

		hash_table.insert(key, val1);
	}
	//#ifdef _DEBUG
	//	const size_t colls_counter = hash_t::HASH_TABLE_COLLISION_COUNTER;
	//#endif
	//const size_t colls_counter = hash_pt::HASH_POOL_COLLISION_COUNTER;
	//STREAM << std::endl << "HASH_TABLE_COLLISION_COUNTER: " << colls_counter << std::endl;

	systimer2.endTiming();
	systimer2.print("Test Hash Table", STREAM);

	size_t active_slots = hash_table.activeSlots();

	TIMER_TOOLS::ContainerTimer systimer23;
	systimer23.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key = std::string(keys[i].c_str());
		string_t & key = keys[i];
		hash_value_t * val1 = nullptr;
		if (!hash_table.find(key, val1))
		{
			std::string key11 = std::string(keys[i].c_str());
			STREAM << std::endl << "Hash Table broken at: " << key11 << std::endl;
			break;
		}
	}
	systimer23.endTiming();
	systimer23.print("Test Hash Table find", STREAM);

	//hash_pt::pool_type hash_pool;
	//hash_table.activeElements(hash_pool);

	//hash_pt::pool_type hash_pool2;
	//typedef hash_pt::iterator hash_iterator;
	//hash_iterator iter = hash_table.begin();
	//hash_iterator iter_end = hash_table.end();
	//while (iter != iter_end)
	//{
	//	hash_pool2.push_back(*iter);
	//	++iter;
	//}

	TIMER_TOOLS::ContainerTimer systimer231122334455;
	systimer231122334455.beginTiming();
	for (size_t i = 0; i < hash_size; ++i)
	{
		//std::string key = std::string(keys[i].c_str());
		string_t & key = keys[i];

		hash_table.remove(key);

		hash_value_t * val1 = nullptr;
		if (hash_table.find(key, val1))
		{
			std::string key11 = std::string(keys[i].c_str());
			STREAM << std::endl << "Hash Table broken at: " << key11 << std::endl;
			break;
		}
	}
	systimer231122334455.endTiming();
	systimer231122334455.print("Test Hash Table remove", STREAM);

	STREAM << std::endl << "Hash Table size: " << hash_table.size() << std::endl;
	size_t active_slots1 = hash_table.activeSlots();
	STREAM << std::endl << "Hash Table active_slots: " << active_slots1 << std::endl;

	//hash_t::pool_type hash_pool22;
	//hash_table.activeElements(hash_pool22);

	if (stream_results)
		STREAM << std::endl << "Test Case OUT: THashTable " << std::endl;

	return true;
}
