/* $Id$
 * vim: fdm=marker
 *
 * This file is part of libalf.
 *
 * libalf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libalf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libalf.  If not, see <http://www.gnu.org/licenses/>.
 *
 * (c) 2010 David R. Piegdon <david-i2@piegdon.de>
 * Author: David R. Piegdon <david-i2@piegdon.de>
 *
 */

#ifndef __libalf_algorithm_rivest_shapire_h__
# define __libalf_algorithm_rivest_shapire_h__

#include <libalf/algorithm_angluin.h>

namespace libalf {

using namespace std;

/*
 * Rivest Shapire Extension of Angluins L* algorithm.
 * It tries to find an optimal split in a given counterexample
 */

template <class answer>
class rivest_shapire_table : public angluin_simple_table<answer> {
	protected: // types
		typedef list< algorithm_angluin::simple_row<answer, vector<answer> > > table_t;

	protected: // data
		bool counterexample_mode;
		list<int> counterexample;
		answer cex_answer;
		bool cex_answer_set;
		int cex_front, cex_back, cex_latest_bad;

		bool conjecture_stored;
		libalf::finite_automaton latest_cj;
		list<algorithm_angluin::automaton_state<table_t> > latest_cj_statemapping;

	protected: // methods
		virtual conjecture * derive_conjecture()
		{{{
			finite_automaton * cj;
			cj = this->derive_conjecture_memorize(latest_cj_statemapping);
			if(cj) {
				latest_cj = *cj;
				conjecture_stored = true;
			} else {
				latest_cj.valid = false;
			}

			return cj;
		}}}
		virtual bool complete()
		{{{
			if(!this->initialized) {
				this->initialize_table();
				counterexample_mode = false;
				latest_cj.valid = false;
			}

			if(counterexample_mode) {
				return analyse_counterexample();
			} else {
				// normal operation, almost like angluin L*

				if(   this->fill_missing_columns(this->upper_table)
				   && this->fill_missing_columns(this->lower_table) ) {

					if(!this->close())
						return complete();
//				// OBSOLETE for RV, as the table is always consistent:
//					if(!make_consistent())
//						return complete();

					return true;
				} else{
					return false;
				}
			}
		}}}
		virtual bool analyse_counterexample()
		{{{
			// the basic idea behind the RV algorithm is to analyse a counterexample
			// to find an optimal suffix that discriminated between two stated that
			// were merged in the hypothesis. this suffix is than added as a new column.
			// we do this using a binary search through all possible suffixes, in
			// the bound of cex_front and cex_back. cex_latest_bad marks the latest
			// position that is found to be descriminating between the hypothesis
			// and teacher.

			answer a;
			bool missing_knowledge = false;

			if(!cex_answer_set)
				cex_answer_set = this->my_knowledge->resolve_or_add_query(counterexample, cex_answer);
			if(!cex_answer_set)
				missing_knowledge = true;

#ifdef DEBUG_RIVEST_SHAPIRE // {{{
			(*this->my_logger)(LOGGER_DEBUG, "RV:    cex_front %d, cex_back %d\n", cex_front, cex_back);
#endif // }}}
			if(cex_back < cex_front) // end is reached -> cex is invalid
			{{{
				(*this->my_logger)(LOGGER_ERROR, "rivest_shapire_table: no discriminating suffix found. you gave an invalid counterexample. aborting counterexample mode.\n");
				counterexample_mode = false;
				cex_front = -1;
				cex_back = -1;
				cex_latest_bad = -1;
				cex_answer_set = false;

				return false;
			}}}

			// binary search for the state that needs to be split
			int current_index = (cex_front + cex_back) / 2;
			int i;
			list<int>::const_iterator current_pos, li;

#ifdef DEBUG_RIVEST_SHAPIRE // {{{
			(*this->my_logger)(LOGGER_DEBUG, "RV: CEX mode, picked index %d from [%d,%d]\n", current_index, cex_front, cex_back);
#endif // }}}


			for(i = 0, current_pos = counterexample.begin(); (i < current_index) && (current_pos != counterexample.end()); ++i, ++current_pos)
				/* nothing */ ;

			list<int> new_word;
			// generate new word to test
			{{{
				// get different prefix from last hypothesis
				set<int> current_states;
				typename list<algorithm_angluin::automaton_state<table_t> >::const_iterator mi;

				current_states = latest_cj.initial_states;
#ifdef DEBUG_RIVEST_SHAPIRE // {{{
				(*this->my_logger)(LOGGER_DEBUG, "RV:     replacing prefix: .");
				list<int>::const_iterator li;
				for(li = counterexample.begin(); li != current_pos; ++li)
					(*this->my_logger)(LOGGER_DEBUG, "%d.", *li);
				(*this->my_logger)(LOGGER_DEBUG, "\n");
#endif // }}}
				latest_cj.run(current_states, counterexample.begin(), current_pos);

				int current_state = *( current_states.begin() );
				for(i = 0, mi = latest_cj_statemapping.begin(); mi != latest_cj_statemapping.end(); ++i, ++mi)
					if(mi->id == current_state)
						break;
				new_word = mi->tableentry->index;
#ifdef DEBUG_RIVEST_SHAPIRE // {{{
				(*this->my_logger)(LOGGER_DEBUG, "RV:     new prefix is %s\n", word2string(new_word).c_str());
#endif // }}}


				// add same suffix
				while(current_pos != counterexample.end()) {
					new_word.push_back(*current_pos);
					++current_pos;
				};
#ifdef DEBUG_RIVEST_SHAPIRE // {{{
				(*this->my_logger)(LOGGER_DEBUG, "RV:     new word thus %s\n", word2string(new_word).c_str());
#endif // }}}
			}}}

			if(!this->my_knowledge->resolve_or_add_query(new_word, a))
				return false;
#ifdef DEBUG_RIVEST_SHAPIRE // {{{
			(*this->my_logger)(LOGGER_DEBUG, "RV:     cex: %c, new_word: %c\n", (cex_answer == true) ? '+' : '-', (a == true) ? '+' : '-');
#endif // }}}
			if(missing_knowledge)
				return false;

			// compare hypothesis and teacher and advance accordingly.
			// i.e. either continue binary search or add new suffix
			{{{
				if(cex_front != cex_back) {
					// advance binary search until we find the first bad state
					if(a != cex_answer) {
						cex_latest_bad = current_index;
						cex_back = current_index-1;
					} else {
						cex_front = current_index+1;
					}

					if(cex_front <= cex_back)
						return complete();
				}

				if(a != cex_answer)
					cex_latest_bad = current_index;

				// add new suffix to table that discriminated the two states
				while(cex_latest_bad > 0) {
					counterexample.pop_front();
					cex_latest_bad--;
				}

#ifdef DEBUG_RIVEST_SHAPIRE
				(*this->my_logger)(LOGGER_INFO, "rivest_shapire_table: adding new suffix %s due to last counterexample.\n", word2string(counterexample).c_str());
#endif
				if(!this->add_column(counterexample))
					(*this->my_logger)(LOGGER_ERROR, "rivest_shapire_table: invalid counterexample or internal bug! suffix already contained in table.\n");

				counterexample_mode = false;
				cex_front = -1;
				cex_back = -1;
				cex_latest_bad = -1;
				cex_answer_set = false;

				return complete();
			}}}
		}}}

	public: // methods
		rivest_shapire_table()
		{{{
			this->set_alphabet_size(0);
			this->set_knowledge_source(NULL);
			this->set_logger(NULL);
			counterexample_mode = false;
			cex_front = -1;
			cex_back = -1;
			cex_latest_bad = -1;
			latest_cj.valid = false;
			conjecture_stored = false;
		}}}
		rivest_shapire_table(knowledgebase<answer> *base, logger *log, int alphabet_size)
		{{{
			this->set_alphabet_size(alphabet_size);
			this->set_logger(log);
			this->set_knowledge_source(base);
			counterexample_mode = false;
			cex_front = -1;
			cex_back = -1;
			cex_latest_bad = -1;
			latest_cj.valid = false;
			conjecture_stored = false;
		}}}

		virtual bool add_counterexample(list<int> word)
		{{{
#ifdef DEBUG_RIVEST_SHAPIRE // {{{
			(*this->my_logger)(LOGGER_DEBUG, "RV: counterexample: %s\n", word2string(word).c_str());
#endif // }}}
			list<int>::const_iterator li;
			if(this->my_knowledge == NULL) {
				(*this->my_logger)(LOGGER_ERROR, "rivest_shapire_table: add_counterexample() without knowledgebase!\n");
				return false;
			}

			if(!conjecture_stored || !latest_cj.valid) {
				(*this->my_logger)(LOGGER_ERROR, "rivest_shapire_table: add_counterexample() without having a conjecture stored! did you ever advance until you had a conjecture?\n");
				return false;
			}

			// check for increase in alphabet size
			int new_asize = this->get_alphabet_size();
			bool asize_changed = false;
			for(li = word.begin(); li != word.end(); li++) {
				if(*li >= new_asize) {
					new_asize = *li+1;
					asize_changed = true;
				}
			}
			if(asize_changed) {
				(*this->my_logger)(LOGGER_ALGORITHM, "rivest_shapire_table: add_counterexample(): increase of alphabet_size from %d to %d.\nNOTE: it is possible that the next hypothesis does not increase in state-count.\n", this->get_alphabet_size(), new_asize);
				this->increase_alphabet_size(new_asize);
				// don't store the cex, we need to re-run and generate a new cex before analysing it.
				return true;
			}

			counterexample = word;
			counterexample_mode = true;
			cex_answer_set = false;
			cex_front = 0;
			cex_back = counterexample.size()-1;
			cex_latest_bad = -1;

			if(cex_back < cex_front) {
				(*this->my_logger)(LOGGER_ERROR, "rivest_shapire_table: you gave epsilon as a counterexample. why didn't you set the right answer in the beginning? aborting counterexample mode. if epsilon is really wrong, please restart with a fresh instance.\n");
				counterexample_mode = false;
				cex_front = cex_back = cex_latest_bad = -1;
				latest_cj.clear();
				latest_cj_statemapping.clear();
				cex_latest_bad = false;
				conjecture_stored = false;
			}
			return true;
		}}}

		virtual basic_string<int32_t> serialize() const
		{{{
			if(conjecture_stored)
				(*this->my_logger)(LOGGER_WARN, "rivest_shapire_table: serialize during counterexample mode is a bad idea. please avoid it for now.\n");

			return angluin_simple_table<answer>::serialize();
		}}}
		virtual bool deserialize(serial_stretch & serial)
		{{{
			counterexample_mode = false;
			counterexample.clear();
			cex_answer_set = false;
			cex_front = cex_back = cex_latest_bad = -1;

			conjecture_stored = false;
			latest_cj.clear();
			latest_cj_statemapping.clear();

			return angluin_simple_table<answer>::deserialize(serial);
		}}}
};


}; // end of namespace libalf

#endif // __libalf_algorithm_rivest_shapire_h__

