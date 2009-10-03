package de.libalf.jni;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

import de.libalf.Knowledgebase;
import de.libalf.Logger;

/**
 * <p>
 * DeLeTe2 is an offline learning algorithm for RFSA. See
 * <ul>
 * <li>
 * <em>Learning regular languages using RFSAs. F. Denis, A. Lemay, A. Terlutte.</em>
 * </li>
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
public class JNIAlgorithmDeLeTe2 extends JNILearningAlgorithm {
	private static final long serialVersionUID = 1L;

	/**
	 * Creates a new object implementing the DeLeTe2 inference algorithm for
	 * regular languages.
	 * 
	 * @param knowledgebase
	 *            the source of information for the algorithm.
	 * @param alphabet_size
	 *            the size of the used alphabet
	 */
	public JNIAlgorithmDeLeTe2(Knowledgebase knowledgebase, int alphabet_size) {
		this.knowledgebase = (JNIKnowledgebase) knowledgebase;
		this.pointer = init(this.knowledgebase.getPointer(), alphabet_size);
	}

	/**
	 * <p>
	 * <em>JNI method call:</em>
	 * </p>
	 * Invokes the JNI interface to initialize a new C++ <code>DeLeTe2</code>
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
	 * Creates a new object implementing the DeLeTe2 inference algorithm for
	 * regular languages.
	 * 
	 * @param knowledgebase
	 *            the source of information for the algorithm.
	 * @param alphabet_size
	 *            the size of the used alphabet
	 * @param logger
	 *            a logger
	 */
	public JNIAlgorithmDeLeTe2(Knowledgebase knowledgebase, int alphabet_size,
			Logger logger) {
		this.knowledgebase = (JNIKnowledgebase) knowledgebase;
		this.logger = (JNIBufferedLogger) logger;
		this.pointer = init(this.knowledgebase.getPointer(), alphabet_size,
				this.logger.getPointer());
	}

	/**
	 * <p>
	 * <em>JNI method call:</em>
	 * </p>
	 * Invokes the JNI interface to initialize a new C++ <code>DeLeTe2</code>
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

	/**
	 * Performs no action and just returns <code>null</code>.
	 * 
	 * @param data
	 *            some data.
	 * 
	 * @return <code>null</code>.
	 */
	public int[] deserialize_magic(int[] data) {
		return null;
	}

	/**
	 * @see Serializable
	 */
	private void readObject(ObjectInputStream in) throws IOException,
			ClassNotFoundException {
		in.defaultReadObject();
		int alphabet_size = in.readInt();
		if (this.logger != null)
			this.pointer = init(this.knowledgebase.getPointer(), alphabet_size,
					this.logger.getPointer());
		else
			this.pointer = init(this.knowledgebase.getPointer(), alphabet_size);
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