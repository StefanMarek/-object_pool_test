/*****************************************************************************/
/*!
	@author			Stefan Marek
	@description	Implements:
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __STRING_TOOLS_H__
#define __STRING_TOOLS_H__

#include "Definitions.h"
#include "ErrorHandling.h"

#include <limits>
//#include <limits>	// float_round_style, float_denorm_style
#include <iomanip>  //setprecision
//#include <ios>		//stream.good()
#include <sstream>	// stringstream
#include <vector>
#include <string>
#include <cstring> // strtok_s
//=============================================================================
//#ifndef USE_TCHAR_TYPES
//	#define USE_TCHAR_TYPES
//	#include <tchar.h>
//#endif // USE_TCHAR_TYPES
//=============================================================================

namespace STRING_TOOLS { 

	// Problem: We have two STL versions for the same string and stream 
	// container. One for ANSI and one for UNICODE. 
	// Solution: TCHAR is wchar_t if UNICODE is enabled and char otherwise.
	// Conclusion: instead of using wstring / string, wstringstream / ... 
	// typedef TCHAR based strings and streams.
	// A TCHAR based std::string  
	//typedef TCHAR char_t;
	// A string_t based char_t  
	//typedef std::basic_string< char_t > string_t;
	// A std::vector with string_t
	//typedef std::vector< string_t > string_vector_t;
	// TCHAR based streams
	//typedef std::basic_stringstream< char_t, std::char_traits< char_t >, 
	//	std::allocator< char_t > > stringstream_t;
	//typedef std::basic_ostream< char_t, std::char_traits< char_t > > ostream_t;
	//typedef std::basic_istream< char_t, std::char_traits< char_t > > istream_t;
	//typedef std::basic_ofstream< char_t, std::char_traits< char_t > > ofstream_t; 
	//typedef std::basic_ifstream< char_t, std::char_traits< char_t > > ifstream_t; 

	typedef std::vector< std::string > string_vector;

	//static const char C_STREND	= '\0';
	//static const char C_NEWLINE = '\n';
	//static const char C_TAB		= '\t';
	//static const char C_SPACE	= ' ';

//=============================================================================
//============================= TYPE FROM STRING ==============================
//=============================================================================
	template <class Type>
	INLINE const Type from_string(const std::string & str)
	{
		Type tValue;
		std::stringstream(str) >> tValue;
		return tValue;
	}
	//=============================================================================
	//template <class Type>
	//INLINE bool 
	//from_string( Type & t, const std::string & str, 
	//	std::ios_base & (*f)(std::ios_base&) )
	//{
	//	std::istringstream iss( str );
	//	return !(iss >> f >> t).fail();
	//}
	//=============================================================================
	template <class Type>
	INLINE bool from_string(Type & tValue, const std::string & strString)
	{
		Type tTemp;
		std::stringstream ss;
		ss << strString;
		ss >> std::ws;
		if (ss >> tTemp)
		{
			ss >> std::ws;
			if (ss.eof())
			{
				tValue = tTemp;
				return true;
			}
		}
		return false;
	}
	//=============================================================================

//=============================================================================
//============================= TO STRING =====================================
//=============================================================================
	template <class Type>
	INLINE const std::string 
	to_string(const Type & value)
	{
		std::stringstream out;
		out << value;
		return out.str();
	}
//=============================================================================
	template<> 
	INLINE const std::string 
	to_string<bool>(const bool & value)
	{
		std::ostringstream out;
		out << std::boolalpha << value;
		return out.str();
	}
//=============================================================================
	template<> 
	INLINE const std::string 
	to_string<double>(const double & value)
	{
		const int sigdigits = std::numeric_limits<double>::digits10;
		std::ostringstream out;
		out << std::setprecision(sigdigits) << value;
		return out.str();
	}
//=============================================================================
	template<> 
	INLINE const std::string 
	to_string<float>(const float & value)
	{
		const int sigdigits = std::numeric_limits<float>::digits10;
		std::ostringstream out;
		out << std::setprecision(sigdigits) << value;
		return out.str();
	}
//=============================================================================

//=============================================================================
//============================= STRING/WSTRING ================================
//=============================================================================
	INLINE std::wstring 
	string_to_wstring( const std::string & str )
	{
		return std::wstring( str.begin(), str.end() );
	}
//=============================================================================
	//INLINE std::string 
	//wstring_to_string( const std::wstring & str )
	//{
	//	return std::string( str.begin(), str.end() );
	//}
//=============================================================================

//=============================================================================
//============================= TOKENIZE STRING ===============================
//=============================================================================
	INLINE void 
	tokenize_string( string_vector & vec, const std::string & str, 
		const std::string & sep ) 
	{  
		if ( !str.empty() )
		{
			char * char_str = const_cast<char *>( str.c_str() );
			char * next_token = NULL;
			char * token = strtok_s( char_str, sep.c_str(), &next_token ); 
			while( NULL != token )  
			{  
				vec.push_back( token );  
				token = strtok_s( NULL, sep.c_str(), &next_token );  
			}
		}
	} 
//=============================================================================
//============================= SPLIT STRING ==================================
//=============================================================================
// Split string is a string tokenizer utility.
//=============================================================================
	INLINE void 
	split_string( string_vector & vec, const std::string & str, 
		const std::string & sep = " " )
	{
		if ( !str.empty() )
		{
			const size_t seplen = sep.length();
			size_t n = 0;
			size_t nOld = 0;
			while (n != std::string::npos)
			{
				n = str.find(sep, n);
				if (n != std::string::npos)
				{
					if (n != nOld)
					{
						vec.push_back( str.substr(nOld, n - nOld) );
					}
					n += seplen;
					nOld = n;
				}
			}
			vec.push_back( str.substr(nOld, str.length() - nOld) );
		}
	}
//=============================================================================
	INLINE string_vector 
	split_string( const std::string & str, const std::string & sep = " " )
	{
		string_vector vec;
		split_string( vec, str, sep );
		return vec;
	}
//=============================================================================

//=============================================================================
//============================= TRIM STRING ===================================
//=============================================================================
	// trims whitespace form the beginning of the string
	INLINE std::string 
	trim_string_left( const std::string & strTrim, 
			   const std::string & strWhitespace = "\n\r\t " )
	{
		const size_t index = strTrim.find_first_not_of(strWhitespace);
		if ( index != std::string::npos )
		{
			return strTrim.substr( index );
		}
		else
		{
			return strTrim;
			//return TString();
		}
	}
//=============================================================================
	// trims whitespace form the end of the string
	INLINE std::string 
	trim_string_right( const std::string & strTrim, 
			    const std::string & strWhitespace = "\n\r\t " )
	{
		const size_t index = strTrim.find_last_not_of(strWhitespace);
		if ( index != std::string::npos )
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
	INLINE std::string 
	trim_string( const std::string & strTrim, 
		   const std::string & strWhitespace = "\n\r\t " )
	{
		return trim_string_right( trim_string_left(strTrim, strWhitespace), strWhitespace );
	}
//=============================================================================

//=============================================================================
//============================= TO UPPER / LOWER STRING =======================
//=============================================================================
//	INLINE void
//	string_to_upper(const std::string & str, std::string & res )
//	{
//		std::transform( str.begin(), str.end(), res.begin(), std::toupper );
//	}
////=============================================================================
//	INLINE void
//	string_to_lower(const std::string & str, std::string & res)
//	{
//		std::transform( str.begin(), str.end(), res.begin(), std::tolower );
//	}
//=============================================================================
} // namespace STRING_TOOLS
//=============================================================================

#endif // __STRING_TOOLS_H__ 
