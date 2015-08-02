/*
 * graphml2metis.c
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

	// vertices iterator
	igraph_vit_create(graph, vertex_selector, &vertex_iterator);

	while (!IGRAPH_VIT_END(vertex_iterator)) {
		igraph_vs_adj(&adjacent_vertex_selector, (long int) IGRAPH_VIT_GET(vertex_iterator), IGRAPH_ALL);
		igraph_vit_create(graph, adjacent_vertex_selector, &adjacent_vertex_iterator);
		while (!IGRAPH_VIT_END(adjacent_vertex_iterator)) {
			// igraph indexes starting from 0, we need it starting from 1
			printf("%li ", ((long int) IGRAPH_VIT_GET(adjacent_vertex_iterator) + 1));
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


void export_nodes_attributes(const igraph_t* graph) {
	igraph_es_t edge_selector;
	igraph_vs_t vertex_selector;
	igraph_vs_t adjacent_vertex_selector;
	igraph_vit_t vertex_iterator;
	igraph_vit_t adjacent_vertex_iterator;
	igraph_integer_t nodes_number;
	igraph_integer_t edges_number;
	igraph_vector_t vertices_attributes_types;
	igraph_strvector_t vertices_attributes_names;

	igraph_vector_init(&vertices_attributes_types, 0);
	igraph_strvector_init(&vertices_attributes_names, 0);

	igraph_cattribute_list(graph, NULL, NULL, &vertices_attributes_names, &vertices_attributes_types, NULL, NULL);

	int attribute = 0;
	for (attribute = 0; attribute < vertices_attributes_names.len - 1; attribute++) {
		printf("%s,", vertices_attributes_names.data[attribute]);
	}
	printf("%s\n", vertices_attributes_names.data[attribute]);

	// vertices selector
	igraph_vs_all(&vertex_selector);
	igraph_vs_size(graph, &vertex_selector, &nodes_number);

	// vertices iterator
	igraph_vit_create(graph, vertex_selector, &vertex_iterator);

	long int vertice;
	while (!IGRAPH_VIT_END(vertex_iterator)) {
		vertice = (long int) IGRAPH_VIT_GET(vertex_iterator);
		for (attribute = 0; attribute < vertices_attributes_names.len - 1; attribute++) {
			if (VECTOR(vertices_attributes_types)[attribute] == IGRAPH_ATTRIBUTE_NUMERIC) {
				igraph_real_printf(VAN(graph, STR(vertices_attributes_names, attribute), vertice));
				printf(",");
			} else {
				printf("%s,", VAS(graph, STR(vertices_attributes_names, attribute), vertice));
			}
		}
		if (VECTOR(vertices_attributes_types)[attribute] == IGRAPH_ATTRIBUTE_NUMERIC) {
			igraph_real_printf(VAN(graph, STR(vertices_attributes_names, attribute), vertice));
		} else {
			printf("%s", VAS(graph, STR(vertices_attributes_names, attribute), vertice));
		}
		printf("\n");
		IGRAPH_VIT_NEXT(vertex_iterator);
	}
	igraph_vit_destroy(&vertex_iterator);
	igraph_vs_destroy(&vertex_selector);
	return;
}


int read_graph_and_export_metis(const char * graph_file_name) {
	FILE* input = fopen(graph_file_name, "r");
	int ret = -1;
	if (input != 0) {
		igraph_i_set_attribute_table(&igraph_cattribute_table);  // this must be before the graph creation.
		igraph_t graph;
		ret = igraph_read_graph_graphml(&graph, input, 0);
		if (ret == IGRAPH_SUCCESS) {
//			export_metis(&graph);
			export_nodes_attributes(&graph);
			igraph_destroy(&graph);
		}
	}
	return ret;
}
