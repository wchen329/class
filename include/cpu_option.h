//////////////////////////////////////////////////////////////////////////////
//
//    PRISCAS - Computer architecture simulator
//    Copyright (C) 2019 Winor Chen
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __CPUOPTION_H__
#define __CPUOPTION_H__
#include <algorithm>
#include <iterator>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "mt_exception.h"
#include "primitives.h"
#include "priscas_global.h"

namespace priscas
{
	typedef std::vector<UPString> CPU_Option_Name_Vec;
	typedef int PCPU_OpRawV;

	/* Represents a CPU option
	 * (i.e. forcing a certain control signal or the like)
	 * wchen329
	 */
	class CPU_Option
	{
		public:
			
			/* UPString& getName() const
			 * Get the CPU Option's name
			 */
			const UPString& getName() const { return nameDesc.getName(); }
			
			/* UPString& getName() const
			 * Get the CPU Option's description
			 */
			const UPString& getDescription() const { return nameDesc.getDescription(); }
			
			/* UPString& getName() const
			 * Get the CPU Option's possible string values
			 */
			const CPU_Option_Name_Vec& get_PossibleValues() const { return this->strNames; }
			
			/* UPString& add_Value(...)
			 * Add a valid possible value to the CPU
			 * A unique raw value must also be specified
			 */
			void add_Value(const UPString& strValue, PCPU_OpRawV intValue);
			
			/* UPString& set_Value(...)
			 * Set the current CPU option value
			 * This will also automatically set the raw value accordingly
			 */
			void set_Value(const UPString& strValue);
			
			/* PCPU_OpRawV getValue_Raw() const
			 * Get the current raw value of the CPU option
			 */
			PCPU_OpRawV getValue_Raw() const { return this->value;} 
			
			/* const UPString& getValue() const
			 * Get the current string value of the CPU option
			 */
			const UPString& getValue() const { return this->IntToStr.at(value); }

			/* bool operator==(...) const
			 *
			 * Two CPU options are said to be equal (i.e. "the same") if
			 * their names match. Nothing else needs to match.
			 * Note: this is NOT logical identity-
			 */
			inline bool operator==(const CPU_Option& op_two) const
			{
				return std::binder1st<std::equal_to<UPString>>(std::equal_to<UPString>(), this->getName())(op_two.getName());
			}

			/* bool operator==(...) const
			 * The negative of the equals operation (NOT equals).
			 */
			inline bool operator!=(const CPU_Option& op_two) const
			{
				return std::unary_negate<std::binder1st<std::equal_to<UPString>>>(std::binder1st<std::equal_to<UPString>>(std::equal_to<UPString>(), this->getName()))(op_two.getName());
			}

			/* CPU_Option()
			 * Creates a nameless and valueless cpu option
			 * This is considered "invalid"
			 */
			CPU_Option() : nameDesc("", "") {};
			CPU_Option(const std::string& name, const std::string& description, int default_value = 0);

		private:
			std::vector<std::string> strNames;
			std::map<std::string, int> strToInt;
			std::map<int, std::string> IntToStr;
			NameDescPair nameDesc;	// name and description of the CPU options
			PCPU_OpRawV value;				// Current value of CPU option (internal value)
	};

	/* CPU_Option_List
	 */
	typedef std::list<CPU_Option> CPU_Option_List;
	typedef std::list<CPU_Option>::iterator CPU_Option_List_Iter;
	typedef std::list<CPU_Option>::const_iterator CPU_Option_List_CIter;

	inline bool operator<(const CPU_Option& op_one, const CPU_Option& op_two)
	{
		return std::less<const UPString&>()(op_one.getName(), op_two.getName());
	}


	/* CPU_ControlPanel
	 * Represents a collection of CPU options for a processor
	 * wchen329
	 *
	 */
	class CPU_ControlPanel
	{
		public:

			/* add_Control(...)
			 * Declare a new control
			 */
			void add_Control(const CPU_Option&);

			/* bool empty() const
			 * Check if the control panel has any cpu options registered
			 *
			 * Return: bool
			 *         true if has at least one cpu option
			 *         false if has zero cpu options
			 */
			bool empty() const { return ops.empty(); }

			/* set_ControlValue(...)
			 * Sets the value of this CPU option
			 * Return: None
			 */
			void set_ControlValue(const UPString& option_Name, const UPString& option_Value);

			/* get_ControlValue(...)
			 * Gets the value of this CPU option (as a string)
			 * Return: const UPString&
			 *         string value of cpu option
			 */
			const UPString& get_ControlValue(const UPString& option_Name) const;

			/* get_ControlValue(...)
			 * Gets the value of this CPU option (as the raw value)
			 * Return: PCPU_OpRawV
			 *         raw cpu option value
			 */
			PCPU_OpRawV get_ControlValue_Raw(const UPString& option_Name) const;

			/* exists_ControlValue(...)
			 * Looks to see if a CPU option of the given name exists.
			 * Return: bool
			 *         true if given option exists
			 *         false if given option does not exists
			 */
			bool exists_ControlValue(const UPString& option_Name) const;

			/* CPU_Option_List list_Controls(...)
			 * Get a list of the constituent cpu options (const)
			 */
			const CPU_Option_List list_Controls() const;

			/* finalize()
			 * Finalize the control panel, that meaning do not allow any more options / controls
			 * to be added to the control panel.
			 * Existing options can still be edited, however
			 */
			void finalize() { this->is_finalized = true; }

			CPU_ControlPanel() :
				is_finalized(false)
			{}

		private:
			
			/* Transmute_Option_to_NameValue 
			 * Unary functor to make an option into name value pair
			 * and push it into a given list
			 */
			class Transmute_OptionPair_to_OptionListEntry_Push
			{
				public:
					inline void operator()(const std::pair<UPString, CPU_Option>& opst_val)
					{
						target.push_back(opst_val.second);
					}

					Transmute_OptionPair_to_OptionListEntry_Push(CPU_Option_List& set_target) :
						target(set_target)
					{
					}

				private:
					CPU_Option_List& target;
			};

			/* Internals
			 *
			 */
			typedef std::map<UPString, CPU_Option> CPUOp_Cont;
			typedef std::map<UPString, CPU_Option> CPUOp_Cont_Iter;
			CPUOp_Cont ops;
			bool is_finalized;
	};
}

#endif