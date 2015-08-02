/*
 * main.c
 *
 *  Created on: Aug 2, 2015
 *      Author: marcos
 */
#include <stdio.h>
#include <unistd.h>
#include <igraph/igraph.h>

#include "export_nodes.h"

void print_help() {
	printf("Usage:\n$graphml2metis -i graphml_filename [-o metis_output_filename] [-a attributes_output_filename]\n");
}

int main(int argc, char **argv) {
	int ret = -1;
	int opt = 0;
	char * graphml_filename = NULL;
	char * output_metis_filename = NULL;
	char * attributes_filename = NULL;
	while ((opt = getopt(argc, argv, "i:o:a:")) != -1) {
		switch(opt) {
			case 'i':
					graphml_filename = optarg;
					break;
			case 'o':
					output_metis_filename = optarg;
					break;
			case 'a':
					attributes_filename = optarg;
					break;
			default:
					print_help();
					break;
		}
	}
	if (graphml_filename == NULL) {
		print_help();
	} else {
		ret = read_graph_and_export(graphml_filename, output_metis_filename, attributes_filename);
		switch (ret) {
			case -1:
				printf("I could not open '%s'\n", argv[1]);
				break;
			case IGRAPH_PARSEERROR:
				printf("I could not understand '%s', parser error.\n", argv[1]);
				break;
			case IGRAPH_UNIMPLEMENTED:
				printf("I can not understand '%s', there is no support to graphml files reading.\n", argv[1]);
				break;
			default:
				break;
		}
	}
	return ret;
}
