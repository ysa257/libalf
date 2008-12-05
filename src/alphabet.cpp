/* $Id$
 * vim: fdm=marker
 *
 * libalf - Automata Learning Factory
 *
 * (c) by David R. Piegdon, i2 Informatik RWTH-Aachen
 *        <david-i2@piegdon.de>
 *
 * see LICENSE file for licensing information.
 */

#include <algorithm>
#include <list>
#include <ostream>
#include <iterator>

#include <arpa/inet.h>

#include "libalf/alphabet.h"
#include "libalf/alf.h"

namespace libalf {

using namespace std;

// return ptr to new list with first∙second
list<int>* concat(list<int> &first, list<int> &second)
{{{
	list<int> *l = new list<int>;
	list<int>::iterator li;

	if(first.size() != 0)
		l->assign(first.begin(), first.end());
	if(second.size() != 0)
		for(li = second.begin(); li != second.end(); li++)
			l->push_back(*li);

	return l;
}}}

bool is_prefix_of(list<int> &prefix, list<int> &word)
{{{
	unsigned int prs, ws;
	prs = prefix.size();
	if(prs == 0)
		return true;
	ws = word.size();
	if(prs > ws)
		return false;
	list<int>::iterator pi, li;
	for(pi = prefix.begin(), li = word.begin(); pi != prefix.end(); pi++, li++)
		if(*pi != *li)
			return false;
	return true;
}}}

bool is_suffix_of(list<int> &suffix, list<int> &word)
{{{
	unsigned int sus,ws;
	sus = suffix.size();
	if(sus == 0)
		return true;
	ws = word.size();
	if(sus > ws)
		return false;
	list<int>::iterator pi, li;

	li = word.begin();
	for(unsigned int i = 0; i < ws - sus; i++)
		li++;
	for(pi = suffix.begin(); pi != suffix.end(); pi++)
		if(*pi != *li)
			return false;
	return true;
}}}

void print_word(ostream &os, list<int> &word)
{{{
	ostream_iterator<int> out(os, ".");
	os << ".";
	copy(word.begin(), word.end(), out);
}}}

void print_word(list<int> &word)
{{{
	printf(".");
	for(list<int>::iterator l = word.begin(); l != word.end(); l++)
		printf("%d.", *l);
}}}

basic_string<int32_t> serialize_word(list<int> &word)
{{{
	basic_string<int32_t> ret;

	ret += htonl(word.size());

	for(list<int>::iterator l = word.begin(); l != word.end(); l++)
		ret += htonl(*l);

	return ret;
}}}

bool deserialize_word(list<int32_t> &into, basic_string<int32_t>::iterator &it, basic_string<int32_t>::iterator limit)
{{{
	int length;

	into.clear();

	if(it == limit) return false;

	length = ntohl(*it);
	it++;

	if(it == limit) return false;

	for(/* -- */; it != limit && length > 0; length--, it++)
		into.push_back(ntohl(*it));

	if(length) {
		into.clear();
		return false;
	}

	return true;
}}}

}
