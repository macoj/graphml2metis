/*
 * main.c
 *
 *  Created on: Jul 30, 2015
 *      Author: marcos
 */
#include <igraph/igraph.h>
#include <stdio.h>
#include <unistd.h>

void export_metis(const igraph_t* graph) {
	igraph_es_t edge_selector;
	igraph_vs_t vertex_selector;
	igraph_vs_t adjacent_vertex_selector;
	igraph_vit_t vertex_iterator;
	igraph_vit_t adjacent_vertex_iterator;
	igraph_integer_t nodes_number;
	igraph_integer_t edges_number;
	// edges selector
	igraph_es_all(&edge_selector, IGRAPH_EDGEORDER_ID);
	igraph_es_size(graph, &edge_selector, &edges_number);
	igraph_es_destroy(&edge_selector);
	// vertices selector
	igraph_vs_all(&vertex_selector);
	igraph_vs_size(graph, &vertex_selector, &nodes_number);
	printf("%li %li\n", (long int) nodes_number, (long int) edges_number);
	// vertice iterator
	igraph_vit_create(graph, vertex_selector, &vertex_iterator);
	while (!IGRAPH_VIT_END(vertex_iterator)) {
		igraph_vs_adj(&adjacent_vertex_selector,
				(long int) IGRAPH_VIT_GET(vertex_iterator), IGRAPH_ALL);
		igraph_vit_create(graph, adjacent_vertex_selector,
				&adjacent_vertex_iterator);
		while (!IGRAPH_VIT_END(adjacent_vertex_iterator)) {
			printf("%li ",
					((long int) IGRAPH_VIT_GET(adjacent_vertex_iterator) + 1));
			IGRAPH_VIT_NEXT(adjacent_vertex_iterator);
		}
		igraph_vit_destroy(&adjacent_vertex_iterator);
		igraph_vs_destroy(&adjacent_vertex_selector);
		printf("\n");
		IGRAPH_VIT_NEXT(vertex_iterator);
	}
	igraph_vit_destroy(&vertex_iterator);
	igraph_vs_destroy(&vertex_selector);
}

int main(int argc, char **argv) {
	FILE * input = 0;
	if (argc < 2) {
		printf("You need to give me the graphml filename!\n");
	} else {
		input = fopen(argv[1], "r");
		if (input != 0) {
			igraph_t graph;
			igraph_read_graph_graphml(&graph, input, 0);
			export_metis(&graph);
			igraph_destroy(&graph);
			fclose(input);
		} else {
			printf("I could not open '%s'\n", argv[1]);
		}
	}
	return 0;
}
