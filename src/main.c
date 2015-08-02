/*
 * main.c
 *
 *  Created on: Aug 2, 2015
 *      Author: marcos
 */
#include <igraph/igraph.h>

int main(int argc, char **argv) {
	int ret = -1;
	if (argc < 2) {
		printf("You need to give me the graphml filename!\n");
	} else {
		ret = read_graph_and_export_metis(argv[1]);
		switch (ret) {
			case -1:
				printf("I could not open '%s'\n", argv[1]);
				break;
			case IGRAPH_PARSEERROR:
				printf("I could not understand '%s', parser error.\n", argv[1]);
				break;
			case IGRAPH_UNIMPLEMENTED:
				printf("I can not understand '%s', there is no support to graphml reading.\n", argv[1]);
				break;
			default:
				break;
		}
	}
	return ret;
}
