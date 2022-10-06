/******************************************************************************
 * File: Trailblazer.cpp
 *
 * Implementation of the graph algorithms that comprise the Trailblazer
 * assignment.
 */

#include "Trailblazer.h"
#include "TrailblazerGraphics.h"
#include "TrailblazerTypes.h"
#include "TrailblazerPQueue.h"
#include "random.h"
#include "foreach.h"
using namespace std;


// here code trains in to the greed. and also writes with color it is, and also number. in one word information

Map<Loc,information> trainToWorld(Grid<double>& world,Loc start){
	Map<Loc,information> info;
	for(int i=0; i< world.nCols; i++){
		for(int j=0; j<world.nRows; j++){
			Loc current= makeLoc(j,i);
			info[current].color= GRAY;
			if(current==start){
				info[current].color=YELLOW;
				info[current].number=0;
			}
		}
	}
	return info;
}

/* Function: shortestPath
 * 
 * Finds the shortest path between the locations given by start and end in the
 * specified world.	 The cost of moving from one edge to the next is specified
 * by the given cost function.	The resulting path is then returned as a
 * Vector<Loc> containing the locations to visit in the order in which they
 * would be visited.	If no path is found, this function should report an
 * error.
 *
 * In Part Two of this assignment, you will need to add an additional parameter
 * to this function that represents the heuristic to use while performing the
 * search.  Make sure to update both this implementation prototype and the
 * function prototype in Trailblazer.h.
 */
// here is main code. I have new stucture, with is in TraiblazeTypes. this structure was, not here
Vector<Loc>
shortestPath(Loc start, Loc end,	Grid<double>& world, 
double costFunction(	Loc one, Loc two,
Grid<double>& world), 
double heuristic(Loc start, Loc end, Grid<double>& world)){
	// TODO: Fill this in!
	Vector<Loc> way;
	double heuro = heuristic(start,end,world);
	Map<Loc,information> allWorld;
	allWorld = trainToWorld(world,start);
	colorCell(world, start, YELLOW);
	if(start==end){
		way.add(start);
		way.add(end);
		return way;
	}
	TrailblazerPQueue<Loc> nextLocs;
	int row = start.row-1;
	int col = start.col-1;
	nextLocs.enqueue(start,heuro);
	while(!nextLocs.isEmpty()){
		Loc curr = nextLocs.dequeueMin();
		colorCell(world,curr, GREEN);
		allWorld[curr].color=GREEN;
		if(curr==end){
			way.add(end);
			Loc parent = allWorld[curr].parent;
			way.add(parent);
			while(parent!=start){
				curr=parent;
				parent = allWorld[curr].parent;
				way.add(parent);
			}
			Vector<Loc> shortest;
			for(int i=way.size()-1; i>=0; i--){
				shortest.add(way[i]);
			}
			return shortest;
		}
		int row = curr.row-1;
		int col = curr.col-1;
		for(int i=0; i<3; i++){
			for(int j=0; j<3; j++){
				if((row+i)>=0&&(col+j)>=0&&(row+i)<world.nRows&&(col+j)<world.nCols){
					Loc v = makeLoc(row+i,col+j);
					if(allWorld[v].color==GRAY){
						colorCell(world,v, YELLOW);
						allWorld[v].number = allWorld[curr].number+costFunction(curr,v,world);
						allWorld[v].parent = curr;
						allWorld[v].color= YELLOW;
						nextLocs.enqueue(v,allWorld[v].number + heuristic(v,end,world) );
					}else if(allWorld[v].color==YELLOW){
						double n = allWorld[curr].number+costFunction(curr,v,world);
						if(n<allWorld[v].number){
							allWorld[v].number = n;
							allWorld[v].parent = curr;
							nextLocs.decreaseKey(v,n+ heuristic(v,end,world));
						}

						
					}


				}
			}
		}
	}

	error("shortestPath is not implemented yet.");
    return way;

}

// this code creates the maze
Set<Edge> createMaze(int numRows, int numCols) {
	// TODO: Fill this in!
	Set<Edge> maze;
		Map<Loc, Set<Loc> > connect;
		Vector<Set<Loc> > con;
	TrailblazerPQueue<Edge> mazeQueue;
	for(int i=0; i<numRows; i++){
		for(int j=0; j<numCols; j++){
			Edge real;
			real.start=makeLoc(i,j);
			Set<Loc> sets;
			sets.add(makeLoc(i,j));
			con.add(sets);
			connect.put(makeLoc(i,j),sets);
			if(i+1<numRows){
				real.end = makeLoc(i+1,j);
				mazeQueue.enqueue(real,randomReal(0,10));

			}
			if(j+1<numCols){
				real.end = makeLoc(i, j+1);
				mazeQueue.enqueue(real,randomReal(0,10));
			}
		}
	}

	int numClaster = numRows*numCols;
	while(numClaster>1){
		Edge edge = mazeQueue.dequeueMin();
		bool cont = false;
		int n1 = -1;
		int n2 =0;
		for(int i=0; i<con.size(); i++){
			if(con[i].contains(edge.end)&&!con[i].contains(edge.start)) {
				cont = true;
					n2=i;
			}
			if(!con[i].contains(edge.end)&&con[i].contains(edge.start)) {
				cont = true;
					n1=i;
			}

		}
		if(cont){
			foreach(Loc loc in con[n2]){
				con[n1].add(loc);
			}
			con.remove(n2);
			maze.add(edge);
			numClaster = numClaster - 1;
		}

		

	}
	//error("createMaze is not implemented yet.");
    return maze;
}
