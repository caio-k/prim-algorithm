/************************************************************************
In this project is realized the algorithm of Prim, which find a Minimum 
Spanning Tree (MST). It's received a txt file with the numbers of vertex, 
edges and the edges with their costs. The result is printed in a txt file. 
*************************************************************************/

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define INFINITY 1000000000000

typedef struct edge {
	int destination_vertex;
	double cost;
	struct edge *next;
} TypeEdge, *PEDGE;

typedef struct {
	PEDGE *adjacentList;
	int numVertex;
	int numEdge;
	int *predecessor; //using in prim
	double *cost;	  //using in prim
	double totalCost;
} TypeGraphs, *PGRAPHS;


//initializes graphs
void initializesGraphs(TypeGraphs* g, int nv, int na) {
	g->adjacentList = (PEDGE*) malloc(nv*sizeof(PEDGE));
	g->predecessor = (int*) malloc (nv*sizeof(int));
	g->cost = (double*) malloc (nv*sizeof(double));
	g->numVertex = nv;
	g->numEdge = na;
	g->totalCost = 0;
	
	int i;
	for (i = 0; i < g->numVertex; i++) 
		g->adjacentList[i] = NULL;
}

//insert edges (v1, v2) and (v2, v1) with cost c in the graph g, because the graph is directed
void insertEdge(int v1, int v2, double c, TypeGraphs* g) {
	if (v1 >= 0 && v1 < g->numVertex && v2 >= 0 && v2 < g->numVertex) {
		PEDGE new1 = (PEDGE) malloc(sizeof(TypeEdge));
		new1->destination_vertex = v2;
		new1->cost = c;
		new1->next = g->adjacentList[v1];
		g->adjacentList[v1] = new1;
		
		PEDGE new2 = (PEDGE) malloc(sizeof(TypeEdge));
		new2->destination_vertex = v1;
		new2->cost = c;
		new2->next = g->adjacentList[v2];
		g->adjacentList[v2] = new2;
	}
}

//executes the prim's algorithm
void executesPrim(TypeGraphs* g) {
	
	//declaration of variables
	int i;
	int minVertex = 0;
	int count = g->numVertex;
	double minCost;
	bool available = true;
	bool vertexAdded[g->numVertex];
	
	//initialization
	for (i = 0; i < g->numVertex; i++) {
		g->cost[i] = INFINITY;
		g->predecessor[i] = -1;
		vertexAdded[i] = false;
	}
	
	g->cost[0] = 0;
	
	while (count > 0) {
		
		available = true;
		
		//find the available vertex that have the smaller cost
		for (i = 0; i < g->numVertex; i++) {
			if (!vertexAdded[i]) {
				if (available || g->cost[i] < minCost) {
					minVertex = i;
					minCost = g->cost[i];
					available = false;
				}
			}
		}
		
		//adds the vertex that have the smaller cost in vertexAdded
		vertexAdded[minVertex] = true; 
		
		//visits every adjacent vertex of selected vertex
		PEDGE nextU = g->adjacentList[minVertex];
		while (nextU != NULL) {
			if (!vertexAdded[nextU->destination_vertex] && nextU->cost < g->cost[nextU->destination_vertex]) {
				g->predecessor[nextU->destination_vertex] = minVertex;
				g->cost[nextU->destination_vertex] = nextU->cost;
			}
			nextU = nextU->next;
		}
		count--;	
	}
	
	//calculates the total cost
	for(i = 0; i < g->numVertex; i++) 
		g->totalCost += g->cost[i];
}	



int main(int argc, char* argv[]) {
	//argv[0] = prim.exe
	//argv[1] = inputArchive.txt
	//argv[2] = outputArchive.txt
	
	TypeGraphs* g = (TypeGraphs*) malloc(sizeof(TypeGraphs));
	
	//declaration of variables
	FILE *fileIn, *fileOut;
	int nv, na, i, u, v;
	double cost;
	
	//input of datas
	fileIn = fopen(argv[1], "r");
	fscanf(fileIn, "%i %i", &nv, &na);
	initializesGraphs(g, nv, na);
	
	i = 0;
	while (i < na) {
		fscanf(fileIn, "%i %i %lf", &u, &v, &cost);
		insertEdge(u, v, cost, g);
		i++;
	}
	
	fclose(fileIn);
	
	
	//executes the prim's algorithm
	executesPrim(g);
	
	//output of datas
	fileOut = fopen(argv[2], "w");
	fprintf(fileOut, "%g\n", g->totalCost);
	
	for(i = 1; i < g->numVertex; i++) 
		fprintf(fileOut, "%d %d\n", g->predecessor[i], i);
	
	fclose(fileOut);

	return 0;
}