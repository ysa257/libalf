package de.libalf.jni;

/**
 * <p>
 * Gracıa and Oncina's regular positive, negative inference algorithm (RPNI) for
 * regular languages. See
 * <ul>
 * <li>
 * <em>P. Gracıa and J. Oncina: Inferring regular languages in polynomial update time</em>
 * </li>
 * </ul>
 * </p>
 * <p>
 * Gracıa and Oncina's regular positive, negative inference algorithm is an
 * <em>offline learning algorithm</em>. See {@link AlgorithmBiermannMiniSAT} for
 * an example how to use offline learning algorithms in LibALF.
 * </p>
 * 
 * @author Daniel Neider (<a
 *         href="mailto:neider@automata.rwth-aachen.de">neider@automata.
 *         rwth-aachen.de</a>), Chair of Computer Science 7, RWTH Aachen
 *         University
 * @version 1.0
 * 
 */
public class AlgorithmRPNI extends JNIAlgorithm {

	/**
	 * Creates a new object implementing the RPNI algorithm.
	 * 
	 * @param knowledgebase
	 *            the source of information for the algorithm.
	 * @param alphabet_size
	 *            the size of the used alphabet
	 */
	public AlgorithmRPNI(Knowledgebase knowledgebase, int alphabet_size) {
		this.knowledgebase = knowledgebase;
		this.pointer = init(knowledgebase.getPointer(), alphabet_size);
	}

	/**
	 * <p>
	 * <em>JNI method call:</em>
	 * </p>
	 * Invokes the JNI interface to initialize a new C++ <code>RPNI</code>
	 * object with the pointer to a <code>knowledgebase</code> and the size of
	 * the alphabet. The pointer to the new created C++ object is returned.
	 * 
	 * @param knowledgebase_pointer
	 *            a pointer to a knowledgebase C++ object
	 * @param alphabet_size
	 *            the size of the used alphabet
	 * @return a pointer to the memory location of the new C++ object.
	 */
	native long init(long knowledgebase_pointer, int alphabet_size);

	/**
	 * Creates a new object implementing the RPNI algorithm.
	 * 
	 * @param knowledgebase
	 *            the source of information for the algorithm.
	 * @param alphabet_size
	 *            the size of the used alphabet
	 * @param logger
	 *            a logger
	 */
	public AlgorithmRPNI(Knowledgebase knowledgebase, int alphabet_size,
			BufferedLogger logger) {
		this.knowledgebase = knowledgebase;
		this.logger = logger;
		this.pointer = init(knowledgebase.getPointer(), alphabet_size, logger
				.getPointer());
	}

	/**
	 * <p>
	 * <em>JNI method call:</em>
	 * </p>
	 * Invokes the JNI interface to initialize a new C++ <code>RPNI</code>
	 * object with the pointer to a <code>knowledgebase</code>, the size of the
	 * alphabet and a pointer to a <code>buffered_logger</code>. The pointer to
	 * the new created C++ object is returned.
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
}
