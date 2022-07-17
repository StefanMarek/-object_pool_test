/*****************************************************************************/
/*!
	@author			Stefan Marek
	@description	System specific classes:
					- class ContainerTimer;
*/
/*****************************************************************************/

#ifdef _MSC_VER 
#pragma once
#endif

#ifndef __CONTAINER_TIMER_H__
#define __CONTAINER_TIMER_H__

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

namespace TIMER_TOOLS { 

	//=============================================================================
	//============================= CONTAINER TIMER ===============================
	//=============================================================================
	class ContainerTimer
	{
	private:
		using clock_type = std::chrono::steady_clock;
		using second_type = std::chrono::duration<double, std::ratio<1> >;

		std::chrono::time_point<clock_type> m_beg;
		std::chrono::time_point<clock_type> m_end;

	public:
		ContainerTimer() = default;
		~ContainerTimer() = default;

		void beginTiming()
		{
			m_beg = clock_type::now();
		}
		void endTiming()
		{
			m_end = clock_type::now();
		}
		double getElapsedTiming() const
		{
			return std::chrono::duration_cast<second_type>(m_end - m_beg).count();
		}
		void print(const std::string& message, std::ostream& os = std::cout) const
		{
			os << std::endl
				<< "=============== ContainerTimer ============" << std::endl
				<< message.c_str() << ": " << getElapsedTiming() << std::endl
				<< "========================================" << std::endl;
		}
		void print(std::ostream& os = std::cout) const
		{
			os << std::endl
				<< "=============== ContainerTimer ============" << std::endl
				<< "elapsed: " << getElapsedTiming() << std::endl
				<< "========================================" << std::endl;
		}
	};
//=============================================================================
} // namespace TIMER_TOOLS
//=============================================================================

#endif // __CONTAINER_TIMER_H__
