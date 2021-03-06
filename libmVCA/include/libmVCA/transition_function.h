/* $Id$
 * vim: fdm=marker
 *
 * This file is part of libmVCA.
 *
 * libmVCA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libmVCA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libmVCA.  If not, see <http://www.gnu.org/licenses/>.
 *
 * (c) 2009,2010 Lehrstuhl Softwaremodellierung und Verifikation (I2), RWTH Aachen University
 *           and Lehrstuhl Logik und Theorie diskreter Systeme (I7), RWTH Aachen University
 *           and David R. Piegdon <david-i2@piegdon.de>
 * Author: David R. Piegdon <david-i2@piegdon.de>
 *
 */

#ifndef __libmvca_transition_function_h__
# define __libmvca_transition_function_h__

#include <set>
#include <string>
#include <map>

#include <libmVCA/pushdown.h>
#include <libmVCA/serialize.h>

namespace libmVCA {

// NOTE: these transition-functions DO NOT SUPPORT epsilon transitions.

// partial implementation, interface for nondet. and det. transitions
class transition_function {
	public:
		transition_function()
		{ /* nothing */};

		virtual ~transition_function()
		{ /* nothing */ };

		virtual std::set<int> transmute(const std::set<int> & states, int sigma) const = 0;
		virtual std::set<int> transmute(int state, int sigma) const = 0;

		virtual void endo_transmute(std::set<int> & states, int sigma) const;

		virtual bool is_deterministic() const = 0;

		virtual std::basic_string<int32_t> serialize() const = 0;
		virtual bool deserialize(::serial_stretch serial) = 0;

		virtual std::string get_transition_dotfile(int m, int m_bound) const = 0;
};

class mVCA; // forward decl.

class deterministic_transition_function : public transition_function {
	public: // types
		typedef std::map<int, std::map<int, int > >::iterator iterator;
		typedef std::map<int, std::map<int, int > >::const_iterator const_iterator;
	public: // data
		// transitions :: state -> sigma -> state
		std::map<int, std::map<int, int > > transitions;

	public: // methods
		virtual ~deterministic_transition_function()
		{ /* nothing */ };

		virtual std::set<int> transmute(const std::set<int> & states, int sigma) const;
		virtual std::set<int> transmute(int state, int sigma) const;

		virtual std::basic_string<int32_t> serialize() const;
		virtual bool deserialize(::serial_stretch serial);
		virtual bool is_deterministic() const;

		virtual std::string get_transition_dotfile(int m, int m_bound) const;
};

inline std::basic_string<int32_t> serialize(const deterministic_transition_function & f)
{ return f.serialize(); };

inline bool deserialize(deterministic_transition_function & f, serial_stretch & serial)
{ return f.deserialize(serial); };

class nondeterministic_transition_function : public transition_function {
	public: // types
		typedef std::map<int, std::map<int, std::set<int> > >::iterator iterator;
		typedef std::map<int, std::map<int, std::set<int> > >::const_iterator const_iterator;
	public: // data
		// transitions :: state -> sigma -> std::set<states>
		std::map<int, std::map<int, std::set<int> > > transitions;

	public: // methods
		virtual ~nondeterministic_transition_function()
		{ /* nothing */ };

		virtual std::set<int> transmute(const std::set<int> & states, int sigma) const;
		virtual std::set<int> transmute(int state, int sigma) const;

		virtual std::basic_string<int32_t> serialize() const;
		virtual bool deserialize(::serial_stretch serial);
		virtual bool is_deterministic() const;

		virtual std::string get_transition_dotfile(int m, int m_bound) const;
};

inline std::basic_string<int32_t> serialize(const nondeterministic_transition_function & f)
{ return f.serialize(); };

inline bool deserialize(nondeterministic_transition_function & f, serial_stretch & serial)
{ return f.deserialize(serial); };

}; // end of namespace libmVCA.

#endif // __libmvca_transition_function_h__

