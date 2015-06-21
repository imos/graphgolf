#ifndef GRAPHGOLF_IO_H_
#define GRAPHGOLF_IO_H_
#include <memory>

#include "base/base.h"
#include "graphgolf/graph.h"

std::unique_ptr<Graph> LoadGraph();

void SaveGraph(const Graph& graph);

#endif  // GRAPHGOLF_IO_H_
