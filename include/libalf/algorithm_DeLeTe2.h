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


/*
 * DeLeTe2 is an offline learning algorithm for RFSA,
 * described in
 *	* F. Denis, A. Lemay, A. Terlutte: "Learning regular languages using RFSAs" [1]
 */


#ifndef __libalf_algorithm_delete2_h__
# define __libalf_algorithm_delete2_h__

#include <stdio.h>

#include <string>
#include <list>
#include <map>
#include <queue>

#include <libalf/knowledgebase.h>
#include <libalf/learning_algorithm.h>

namespace libalf {

using namespace std;

template <class answer>
class DeLeTe2 : public learning_algorithm<answer> {
	public:	// types

	protected: // data

	public: // methods
		DeLeTe2(knowledgebase<answer> * base, logger * log, int alphabet_size)
		{{{
			this->set_alphabet_size(alphabet_size);
			this->set_logger(log);
			this->set_knowledge_source(base);
		}}}
		virtual ~DeLeTe2()
		{{{
			// nothing
		}}}

		virtual void increase_alphabet_size(int new_asize)
		{{{
			this->set_alphabet_size(new_asize);
		}}}

		virtual void get_memory_statistics(statistics & stats)
		{ };

		virtual bool sync_to_knowledgebase()
		{{{
			return true;
		}}}

		virtual bool supports_sync()
		{{{
			return true;
		}}}

		virtual basic_string<int32_t> serialize()
		{{{
			basic_string<int32_t> ret;

			// we don't have any internal, persistent data
			ret += htonl(1);
			ret += htonl(learning_algorithm<answer>::ALG_DELETE2);

			return ret;
		}}}
		virtual bool deserialize(basic_string<int32_t>::iterator &it, basic_string<int32_t>::iterator limit)
		{{{
			if(it == limit) return false;
			if(ntohl(*it) != 1)
				return false;

			it++; if(it == limit) return false;
			if(ntohl(*it) != learning_algorithm<answer>::ALG_DELETE2)
				return false;

			it++;

			return true;
		}}}

		virtual void print(ostream &os)
		{{{
			os << tostring();
		}}}
		virtual string tostring()
		{{{
			string s;
			return s;
		}}}

		// conjecture is always ready if there is a non-empty knowledgebase
		virtual bool conjecture_ready()
		{{{
			// we're offline. just performe some tests and
			// give a warning if they fail.

			// according to [1], we generate a
			// prefix-automaton from S+, but here we just
			// work with the whole given tree. thus we throw
			// a warning if there are non-accepting leafs

			if((this->my_knowledge != NULL) && (this->my_knowledge->count_answers() > 0)) {
				list<int> sample;
				if(leaf_is_non_accepting(this->my_knowledge->get_rootptr(), sample)) {
					string s;
					s = word2string(sample);
					(*this->my_logger)(LOGGER_WARN, "DeLeTe2 expects a sample-set consisting of pref(S+). "
							"but this sampleset has a non-accepting leaf \"%s\". "
							"you may try anyway, this is just a sidenote.\n", s.c_str());
				}

				return true;
			} else {
				return false;
			}
		}}}

		// stubs for counterexample will throw a warning to the logger
		virtual void add_counterexample(list<int>)
		{{{
			(*this->my_logger)(LOGGER_ERROR, "DeLeTe2 does not support counter-examples, as it is an offline-algorithm. please add the counter-example directly to the knowledgebase and rerun the algorithm.\n");
		}}}

	protected:
		bool leaf_is_non_accepting(typename knowledgebase<answer>::node* n, list<int> & sample, bool prefix_accepting = false)
		// check if all leafs (i.e. states that have no suffixes that either accept or reject) accept
		{{{
			list<int> w;
			if(n->is_answered())
				if(n->get_answer() == true)
					prefix_accepting = true;
				else
					if(n->get_answer() == false)
						prefix_accepting = false;

			int i;
			bool has_children = false;
			typename knowledgebase<answer>::node * c;

			for(i = 0; i < n->max_child_count(); i++) {
				c = n->find_child(i);
				if(c) {
					has_children = true;
					if(leaf_is_non_accepting(c, sample, prefix_accepting))
						return true;
				}
			}
			if(!has_children && !prefix_accepting) {
				sample = n->get_word();
				return true;
			}

			return false;
		}}}

		virtual bool complete()
		{{{
			// we're offline.
			return true;
		}}}
		// derive an automaton from data structure
		virtual bool derive_automaton(bool & t_is_dfa, int & t_alphabet_size, int & t_state_count, set<int> & t_initial, set<int> & t_final, multimap<pair<int, int>, int> & t_transitions)
		{
			
			return false;
		}

};


}; // end namespace libalf

#endif // __libalf_algorithm_delete2_h__

