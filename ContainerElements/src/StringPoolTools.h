/*****************************************************************************/
/*!
@author			Stefan Marek
@description	Implements:
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __STRING_POOL_TOOLS_H__
#define __STRING_POOL_TOOLS_H__

#include "Definitions.h"
#include "ErrorHandling.h"

#include "ObjectPool.h"
#include "StringPool.h"

#include <cstdio> //sprintf sscanf

//=============================================================================
#ifdef  _MSC_VER
#pragma PRAGMA_PACK_PUSH_CRT_PACKING
#endif  
//=============================================================================
#ifdef _MSC_VER 
#pragma PRAGMA_WARNING_PUSH_LEVEL4
#pragma PRAGMA_WARNING_DISABLE4996
#pragma warning(disable: 4477)
#endif
//=============================================================================

namespace SPTOOLS {
	
	//=============================================================================
	//typedef CELL::SPOOL::StringPool string_t;
	//typedef typename CELL::OPOOL::TPool< CELL::SPOOL::StringPool >::Type string_array;
	//=============================================================================
	INLINE CELL::SPOOL::string_t number_to_string(const int & value)
	{
		char buffer[sizeof(int) * 8];

		//vsprintf_s(buffer, SIZEOF_NUMBER_TYPE, "%d", value);
		sprintf(buffer, "%d", value);
		return CELL::SPOOL::string_t(buffer);
	}
	INLINE CELL::SPOOL::string_t number_to_string(const unsigned int & value)
	{
		char buffer[sizeof(unsigned int) * 8];

		//vsprintf_s(buffer, SIZEOF_NUMBER_TYPE, "%d", value);
		sprintf(buffer, "%u", value);
		return CELL::SPOOL::string_t(buffer);
	}
	//INLINE CELL::SPOOL::string_t number_to_string(const size_t & value)
	//{
	//	char buffer[sizeof(size_t) * 8];

	//	sprintf(buffer, "%d", value);
	//	return CELL::SPOOL::string_t(buffer);
	//}
	INLINE CELL::SPOOL::string_t number_to_string(const long & value)
	{
		char buffer[sizeof(long) * 8];

		//vsprintf_s(buffer, SIZEOF_NUMBER_TYPE, "%d", value);
		sprintf(buffer, "%ld", value);
		return CELL::SPOOL::string_t(buffer);
	}
	INLINE CELL::SPOOL::string_t number_to_string(const unsigned long & value)
	{
		char buffer[sizeof(unsigned long) * 8];

		//vsprintf_s(buffer, SIZEOF_NUMBER_TYPE, "%d", value);
		sprintf(buffer, "%lu", value);
		return CELL::SPOOL::string_t(buffer);
	}
	INLINE CELL::SPOOL::string_t number_to_string(const long long & value)
	{
		char buffer[sizeof(long long) * 8];

		//vsprintf_s(buffer, SIZEOF_NUMBER_TYPE, "%d", value);
		sprintf(buffer, "%lld", value);
		return CELL::SPOOL::string_t(buffer);
	}
	INLINE CELL::SPOOL::string_t number_to_string(const unsigned long long & value)
	{
		char buffer[sizeof(unsigned long long) * 8];

		//vsprintf_s(buffer, SIZEOF_NUMBER_TYPE, "%d", value);
		sprintf(buffer, "%llu", value);
		return CELL::SPOOL::string_t(buffer);
	}
	INLINE CELL::SPOOL::string_t number_to_string(const bool value)
	{
		if (value)
		{
			return CELL::SPOOL::string_t("true");
		}
		else
		{
			return CELL::SPOOL::string_t("false");
		}
	}
	//=============================================================================
	INLINE CELL::SPOOL::string_t float_to_string(const float & value)
	{
		char buffer[sizeof(float) * 8];

		//vsprintf_s(buffer, SIZEOF_NUMBER_TYPE, "%f", value);
		sprintf(buffer, "%f", value);

		return CELL::SPOOL::string_t(buffer);
	}
	INLINE CELL::SPOOL::string_t float_to_string(const double & value)
	{
		char buffer[sizeof(double) * 8];

		//vsprintf_s(buffer, SIZEOF_NUMBER_TYPE, "%f", value);
		sprintf(buffer, "%lf", value);

		return CELL::SPOOL::string_t(buffer);
	}
	INLINE CELL::SPOOL::string_t float_to_string(const long double & value)
	{
		char buffer[sizeof(long double) * 8];

		//vsprintf_s(buffer, SIZEOF_NUMBER_TYPE, "%lf", value);
		sprintf(buffer, "%Lf", value);

		return CELL::SPOOL::string_t(buffer);
	}
	//=============================================================================
	//INLINE bool number_from_string(bool & value, const CELL::SPOOL::string_t & str)
	//{
	//	const char * src = str.c_str();
	//	return (sscanf_s(src, "%d", &value) != 0);
	//}
	//INLINE bool number_from_string(short & value, const CELL::SPOOL::string_t & str)
	//{
	//	const char * src = str.c_str();
	//	return (sscanf_s(src, "%d", &value) != 0);
	//}
	//INLINE bool number_from_string(unsigned short & value, const CELL::SPOOL::string_t & str)
	//{
	//	const char * src = str.c_str();
	//	return (sscanf_s(src, "%d", &value) != 0);
	//}
	INLINE bool number_from_string(int & value, const CELL::SPOOL::string_t & str)
	{
		const char * src = str.c_str();
		return (sscanf(src, "%d", &value) != 0);
	}
	INLINE bool number_from_string(unsigned int & value, const CELL::SPOOL::string_t & str)
	{
		const char * src = str.c_str();
		return (sscanf(src, "%u", &value) != 0);
	}
	INLINE bool number_from_string(long & value, const CELL::SPOOL::string_t & str)
	{
		const char * src = str.c_str();
		return (sscanf(src, "%ld", &value) != 0);
	}
	INLINE bool number_from_string(unsigned long & value, const CELL::SPOOL::string_t & str)
	{
		const char * src = str.c_str();
		return (sscanf(src, "%lu", &value) != 0);
	}
	INLINE bool number_from_string(long long & value, const CELL::SPOOL::string_t & str)
	{
		const char * src = str.c_str();
		return (sscanf(src, "%lld", &value) != 0);
	}
	INLINE bool number_from_string(unsigned long long & value, const CELL::SPOOL::string_t & str)
	{
		const char * src = str.c_str();
		return (sscanf(src, "%llu", &value) != 0);
	}
	//INLINE bool number_from_string(bool & value, const char * src)
	//{
	//	return (sscanf_s(src, "%d", &value) != 0);
	//}
	//INLINE bool number_from_string(short & value, const char * src)
	//{
	//	return (sscanf_s(src, "%d", &value) != 0);
	//}
	//INLINE bool number_from_string(unsigned short & value, const char * src)
	//{
	//	return (sscanf_s(src, "%d", &value) != 0);
	//}
	INLINE bool number_from_string(int & value, const char * src)
	{
		return (sscanf(src, "%d", &value) != 0);
	}
	INLINE bool number_from_string(unsigned int & value, const char * src)
	{
		return (sscanf(src, "%u", &value) != 0);
	}
	//INLINE bool number_from_string(size_t & value, const char * src)
	//{
	//	return (sscanf(src, "%lld", &value) != 0);
	//}
	INLINE bool number_from_string(long & value, const char * src)
	{
		return (sscanf(src, "%ld", &value) != 0);
	}
	INLINE bool number_from_string(unsigned long & value, const char * src)
	{
		return (sscanf(src, "%lu", &value) != 0);
	}
	INLINE bool number_from_string(long long & value, const char * src)
	{
		return (sscanf(src, "%lld", &value) != 0);
	}
	INLINE bool number_from_string(unsigned long long & value, const char * src)
	{
		return (sscanf(src, "%llu", &value) != 0);
	}
	//=============================================================================
	INLINE bool float_from_string(float & value, const CELL::SPOOL::string_t & str)
	{
		const char * src = str.c_str();
		return (sscanf(src, "%f", &value) != 0);
	}
	INLINE bool float_from_string(double & value, const CELL::SPOOL::string_t & str)
	{
		const char * src = str.c_str();
		return (sscanf(src, "%lf", &value) != 0);
	}
	INLINE bool float_from_string(long double & value, const CELL::SPOOL::string_t & str)
	{
		const char * src = str.c_str();
		return (sscanf(src, "%Lf", &value) != 0);
	}
	INLINE bool float_from_string(float & value, const char * src)
	{
		return (sscanf(src, "%f", &value) != 0);
	}
	INLINE bool float_from_string(double & value, const char * src)
	{
		return (sscanf(src, "%lf", &value) != 0);
	}
	INLINE bool float_from_string(long double & value, const char * & src)
	{
		return (sscanf(src, "%Lf", &value) != 0);
	}
	//=============================================================================
	//============================= TOKENIZE STRING ===============================
	//=============================================================================
	//INLINE void	tokenize_string( CELL::SPOOL::string_array & vec, const CELL::SPOOL::string_t & str, const CELL::SPOOL::string_t & sep ) 
	//{  
	//	if ( !str.empty() )
	//	{
	//		char * char_str = const_cast<char *>( str.c_str() );
	//		char * next_token = nullptr;
	//		char * token = strtok_s( char_str, sep.c_str(), &next_token ); 
	//		while( nullptr != token )  
	//		{  
	//			vec.push_back( token );  
	//			token = strtok_s( nullptr, sep.c_str(), &next_token );  
	//		}
	//	}
	//} 
	//=============================================================================
	//============================= SPLIT STRING ==================================
	//=============================================================================
	// Split string is a string tokenizer utility.
	//=============================================================================
	INLINE void	split_string(CELL::SPOOL::string_array & vec, const CELL::SPOOL::string_t & str, const CELL::SPOOL::string_t & sep = " ")
	{
		if (!str.empty())
		{
			const size_t seplen = sep.length();
			size_t n = 0;
			size_t nOld = 0;
			while (n != CELL::SPOOL::string_t::npos)
			{
				n = str.find(sep, n);
				if (n != CELL::SPOOL::string_t::npos)
				{
					if (n != nOld)
					{
						vec.push_back(str.substr(nOld, n - nOld));
					}
					n += seplen;
					nOld = n;
				}
			}
			const CELL::SPOOL::string_t strLast = str.substr(nOld, str.length() - nOld);
			if (!strLast.empty())
			{
				vec.push_back(strLast);
			}
		}
	}
	//=============================================================================
	INLINE CELL::SPOOL::string_array split_string(const CELL::SPOOL::string_t & str, const CELL::SPOOL::string_t & sep = " ")
	{
		CELL::SPOOL::string_array vec;
		split_string(vec, str, sep);
		return vec;
	}
	//=============================================================================
	//============================= TRIM STRING ===================================
	//=============================================================================
	// trims whitespace form the beginning of the string
	INLINE CELL::SPOOL::string_t trim_string_left(const CELL::SPOOL::string_t & strTrim, const CELL::SPOOL::string_t & strWhitespace = "\n\r\t ")
	{
		const size_t index = strTrim.find_first_not_of(strWhitespace);
		if (index != CELL::SPOOL::string_t::npos)
		{
			return strTrim.substr(index);
		}
		else
		{
			return strTrim;
			//return TString();
		}
	}
	//=============================================================================
	// trims whitespace form the end of the string
	INLINE CELL::SPOOL::string_t trim_string_right(const CELL::SPOOL::string_t & strTrim, const CELL::SPOOL::string_t & strWhitespace = "\n\r\t ")
	{
		const size_t index = strTrim.find_last_not_of(strWhitespace);
		if (index != CELL::SPOOL::string_t::npos)
		{
			return strTrim.substr(0, index + 1);
		}
		else
		{
			return strTrim;
		}
	}
	//=============================================================================
	// trims whitespace from beginning and end of the string
	INLINE CELL::SPOOL::string_t trim_string(const CELL::SPOOL::string_t & strTrim, const CELL::SPOOL::string_t & strWhitespace = "\n\r\t ")
	{
		return trim_string_right(trim_string_left(strTrim, strWhitespace), strWhitespace);
	}
	//=============================================================================
	//============================= TO UPPER / LOWER STRING =======================
	//=============================================================================
	//	INLINE void
	//	string_to_upper(const CELL::SPOOL::string_t & str, CELL::SPOOL::string_t & res )
	//	{
	//		std::transform( str.begin(), str.end(), res.begin(), std::toupper );
	//	}
	////=============================================================================
	//	INLINE void
	//	string_to_lower(const CELL::SPOOL::string_t & str, CELL::SPOOL::string_t & res)
	//	{
	//		std::transform( str.begin(), str.end(), res.begin(), std::tolower );
	//	}
//=============================================================================
} // namespace STRING_TOOLS
//=============================================================================
#ifdef  _MSC_VER
#pragma PRAGMA_PACK_POP
#endif 
//=============================================================================
#ifdef _MSC_VER 
#pragma PRAGMA_WARNING_DEFAULT4996
#pragma warning(default: 4477)
#pragma PRAGMA_WARNING_POP
#endif
//=============================================================================

#endif // __STRING_POOL_TOOLS_H__ 
