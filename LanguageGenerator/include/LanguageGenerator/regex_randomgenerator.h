/* $Id$
 * vim: fdm=marker
 *
 * LanguageGenerator
 * RegEx Random Generator, similar to the one described in section 4.3 of
 * "F. Denis, A. Lemay and A. Terlutte - Learning regular languages using RFSAs"
 *
 * (c) by David R. Piegdon, i2 Informatik RWTH-Aachen
 *        <david-i2@piegdon.de>
 *
 * see LICENSE file for licensing information.
 */

#include <set>
#include <map>
#include <string>

#ifndef __LanguageGenerator_regex_randomgenerator_h__
# define __LanguageGenerator_regex_randomgenerator_h__

namespace LanguageGenerator {

using namespace std;

class regex_randomgenerator {
	private:

	public:
		regex_randomgenerator();

		// create a random regular expression
		// the sum of all probablities may be != 1, but >= 0. the algorithm will normalize.
		string generate(int num_op, int alphabet_size, float p_sigma[], float p_epsilon, float p_concat, float p_union, float p_star);
		// this version expects normalized probablilities
		string normalized_generate(int num_op, int & alphabet_size, float p_sigma[], float & p_epsilon, float & p_concat, float & p_union, float & p_star);
};

}; // end of namespace LanguageGenerator

#endif

