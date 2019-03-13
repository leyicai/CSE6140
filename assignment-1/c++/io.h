/**
 * io.h
 * Kasimir Gabert
 * Georgia Institute of Technology, Fall 2018
 *
 * Defines helper functions for reading/writing data
 */

#ifndef _H_IO_H
#define _H_IO_H

#include <string>

namespace IO {

/** Create a new graph object from a simple dimacs-like file */
Graph graph_from_gr(const std::string& gr_file);

/** Create a vertex set object from a simple file with vids */
VertexSet vertexset_from_file(const std::string& file);

/** Save vertex data in a graph to the given file */
void write_vdata_to_file(Graph& g, const std::string& file);

};

#endif /* _H_IO_H */
