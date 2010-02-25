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
 * Author: David R. Piegdon <david-i2@piegdon.de>
 *
 */

#ifdef _WIN32
# include <winsock.h>
# include <stdio.h>
#else
# include <arpa/inet.h>
#endif

#include <libmVCA/pushdown.h>

namespace libmVCA {

pushdown_alphabet::pushdown_alphabet()
{ alphabet_size = 0; };

pushdown_alphabet::pushdown_alphabet(int alphabet_size)
{ this->alphabet_size = alphabet_size; };

pushdown_alphabet::~pushdown_alphabet()
{ /* nothing */ };

void pushdown_alphabet::set_alphabet_size(int alphabet_size)
{{{
	if(alphabet_size < 0)
		alphabet_size = 0;
	// remove obsolete direction data,
	for(int sigma = alphabet_size; sigma < this->alphabet_size; sigma++)
		this->directions.erase(sigma);
	// initialise new direction data
	for(int sigma = this->alphabet_size; sigma < alphabet_size; sigma++)
		this->directions[sigma] = DIR_STAY;
	this->alphabet_size = alphabet_size;
}}}

int pushdown_alphabet::get_alphabet_size()
{ return this->alphabet_size; };

enum pushdown_direction pushdown_alphabet::get_direction(int sigma)
{{{
	if(sigma >= 0 && sigma < alphabet_size)
		return this->directions[sigma];
	else
		return DIR_INDEFINITE;
}}}

bool pushdown_alphabet::set_direction(int sigma, enum pushdown_direction direction)
{{{
	if(sigma >= 0 && sigma < alphabet_size && (direction == DIR_UP || direction == DIR_STAY || direction == DIR_DOWN)) {
		this->directions[sigma] = direction;
		return true;
	} else {
		return false;
	}
}}}

int pushdown_alphabet::prefix_countervalue(list<int>::iterator word, list<int>::iterator limit)
{{{
	int ret;
	for(ret = 0; word != limit; ++word) {
		enum pushdown_direction d = get_direction(*word);
		if(d != DIR_INDEFINITE) {
			ret += (int)d;
			if(ret < 0)
				break;
		} else {
			ret = -1;
			break;
		}
	}
	return ret;
}}}

std::basic_string<int32_t> pushdown_alphabet::serialize()
{{{
	return ::serialize(alphabet_size) + ::serialize(directions);
}}}

bool pushdown_alphabet::deserialize(::serial_stretch serial)
{{{
	clear();

	if(!::deserialize(alphabet_size, serial)) return false;
	if(!::deserialize(directions, serial)) return false;

	return true;
}}}

string pushdown_alphabet::to_string()
{{{
	string ret;
	char buf[128];

	snprintf(buf, 128, "pushdown alphabet of size %d\n", alphabet_size);
	ret += buf;

	map<int, enum pushdown_direction>::iterator di;
	for(di = directions.begin(); di != directions.end(); ++di) {
		snprintf(buf, 128, "  %d :: %d\n", di->first, di->second);
		ret += buf;
	}
	ret += "\n";

	return ret;
}}}

} // end of namespace libmVCA
