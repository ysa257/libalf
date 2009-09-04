package de.libalf.dispatcher;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

/**
 * <p>
 * Biermann and Feldman's inference algorithm for regular languages. See
 * <ul>
 * <li>
 * <em>A.W. Biermann and J.A. Feldman - On the Synthesis of Finite-State Machines from
 * Samples of their Behavior</em></li>
 * </ul>
 * This implementation is based on the paper
 * <ul>
 * <li>
 * <em>Arlindo L. Oliveira and João P.M. Silva - Efficient Algorithms for the Inference of Minimum Size DFAs</em>
 * </li>
 * </ul>
 * and uses the MiniSAT SAT-solving library.
 * </p>
 * <p>
 * Biermann and Feldman's inference algorithm for regular languagesis an
 * <em>offline learning algorithm</em>. The following example illustrates how to
 * use offline learning algorithms in LibALF:
 * 
 * <PRE>
 * public BasicAutomaton learn() {
 * 	// Prepare the knowledgebase and the algorithm
 * 	Knowledgebase knowledgebase = new Knowledgebase();
 * 	int alphabetSize = // Set some size
 * 	BufferedLogger logger = new BufferedLogger();
 * 	LearningAlgorithm algorithm = new AlgorithmBiermannMiniSAT(knowledgebase,
 * 			alphabetSize, logger);
 * 
 * 	// Add knowledge to the knowledgebase
 * 	knowledgebase.add_knowledge(word, acceptance);
 * 	// ...
 * 
 * 	// Start inference
 * 	BasicAutomaton inferredAutomaton = algorithm.advance();
 * 
 * 	return inferredAutomaton;
 * 
 * }
 * </PRE>
 * 
 * Note that the user needs to
 * <ul>
 * <li>adjust the <code>alphabetSize</code> and</li>
 * <li>add knowledge to the knowledgebase</li>
 * </ul>
 * </p>
 * 
 * @author Daniel Neider (<a
 *         href="mailto:neider@automata.rwth-aachen.de">neider@automata.
 *         rwth-aachen.de</a>), Chair of Computer Science 7, RWTH Aachen
 *         University
 * @version 1.0
 * 
 */
public class AlgorithmBiermannMiniSAT extends JNIAlgorithm {
	private static final long serialVersionUID = 2L;

	/**
	 * Creates a new object implementing Biermann and Feldman's inference
	 * algorithm for regular languages using a SAT solver.
	 * 
	 * @param knowledgebase
	 *            the source of information for the algorithm.
	 * @param alphabet_size
	 *            the size of the used alphabet
	 */
	public AlgorithmBiermannMiniSAT(Knowledgebase knowledgebase,
			int alphabet_size) {
		this.knowledgebase = knowledgebase;
		this.id = init(knowledgebase.getID(), alphabet_size);
	}

	/**
	 * <p>
	 * <em>JNI method call:</em>
	 * </p>
	 * Invokes the JNI interface to initialize a new C++
	 * <code>MiniSat_biermann</code> object with the pointer to a
	 * <code>knowledgebase</code> and the size of the alphabet. The pointer to
	 * the new created C++ object is returned.
	 * 
	 * @param knowledgebase_pointer
	 *            a pointer to a knowledgebase C++ object
	 * @param alphabet_size
	 *            the size of the used alphabet
	 * @return a pointer to the memory location of the new C++ object.
	 */
	native long init(long knowledgebase_pointer, int alphabet_size);

	/**
	 * Creates a new object implementing Biermann and Feldman's inference
	 * algorithm for regular languages using a SAT solver.
	 * 
	 * @param knowledgebase
	 *            the source of information for the algorithm.
	 * @param alphabet_size
	 *            the size of the used alphabet
	 * @param logger
	 *            a logger
	 */
	public AlgorithmBiermannMiniSAT(Knowledgebase knowledgebase,
			int alphabet_size, BufferedLogger logger) {
		this.knowledgebase = knowledgebase;
		this.logger = logger;
		this.id = init(knowledgebase.getID(), alphabet_size, logger
				.getID());
	}

	/**
	 * <p>
	 * <em>JNI method call:</em>
	 * </p>
	 * Invokes the JNI interface to initialize a new C++
	 * <code>MiniSat_biermann</code> object with the pointer to a
	 * <code>knowledgebase</code>, the size of the alphabet and a pointer to a
	 * <code>buffered_logger</code>. The pointer to the new created C++ object
	 * is returned.
	 * 
	 * @param knowledgebase_pointer
	 *            a pointer to a knowledgebase C++ object
	 * @param alphabet_size
	 *            the size of the used alphabet
	 * @param logger_pointer
	 *            a pointer to a buffered_logger C++ object
	 * @return a pointer to the memory location of the new C++ object.
	 */
	native long init(long knowledgebase_pointer, int alphabet_size,
			long logger_pointer);

	/**
	 * @see Serializable
	 */
	private void readObject(ObjectInputStream in) throws IOException, ClassNotFoundException {
		in.defaultReadObject();
		int alphabet_size = in.readInt();
		this.id = init(this.knowledgebase.getID(), alphabet_size, this.logger == null ? 0 : this.logger.getID());
		int[] serialization = (int[]) in.readObject();
		deserialize(serialization);
	}

	/**
	 * @see Serializable
	 */
	private void writeObject(ObjectOutputStream out) throws IOException {
		out.defaultWriteObject();
		out.writeInt(get_alphabet_size());
		out.writeObject(serialize());
    }
}