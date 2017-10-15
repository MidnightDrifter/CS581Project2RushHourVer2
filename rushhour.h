#ifndef RUSHHOUR_H
#define RUSHHOUR_H

#include <vector>
#include <tuple>

// Keep this
enum Direction   { up, left, down, right, undefined };

// Keep this
enum Orientation { horisontal, vertical};

// Keep this
std::ostream& operator<<( std::ostream& os, Direction const& d );

// Keep this
////////////////////////////////////////////////////////////////////////////////
// global functions
std::vector< std::tuple<unsigned, Direction, unsigned> > 
SolveRushHour( std::string const& filename );   //DFS

std::vector< std::tuple<unsigned, Direction, unsigned> > 
SolveRushHourOptimally( std::string const& filename );    //BFS

////////////////////////////////////////////////////////////////////////////////
// your stuff
////////////////////////////////////////////////////////////////////////////////








class RushHour {
	//private:
public:
	unsigned height = 0;            // size of parking lot
	unsigned width = 0;            // size of parking lot
	unsigned ** parking_lot = nullptr;      // parking lot
	Direction exit_direction = undefined;    // exit direction
	unsigned car = 0;            // car to be navigated
	std::string filename = std::string();// filename for data


	//My stuff
	int parentIndex = 0; //index to parent RushHour object
	int treeDepth = 0;  //Depth in the tree--self explanatory
	unsigned numCars = 0;  //Number of cars in the parking lot
	std::tuple<unsigned, Direction, unsigned> moveToGetHere;  //The move used to get to this board state




	//public:
	//friend std::ostream& operator<<(std::ostream& os, RushHour const& pl);
	void Print(std::string const& filename_out);
	unsigned const& Car() const { return car; }
	Direction const& Dir() const { return exit_direction; }

	//RushHour( std::string const&  filename );
	~RushHour();
	void makeMove(std::tuple< unsigned, Direction, unsigned > move);
	std::vector< std::tuple<unsigned, Direction, unsigned> > Solve()
	{
		return SolveRushHour(filename);
	}
	std::vector< std::tuple<unsigned, Direction, unsigned> > SolveOptimally()
	{
		return SolveRushHourOptimally(filename);
	}
	bool IsSolved() const;
	int Check(std::vector< std::tuple<unsigned, Direction, unsigned> > const& sol);
	int CheckBrief(std::vector< std::tuple<unsigned, Direction, unsigned> > const& sol);






	// RushHour implementation
	RushHour(std::string const&  filename);

	

	int CheckBrief(std::vector< std::tuple<unsigned, Direction, unsigned> > const& sol);




	void Print(std::string const& filename_out);





};










class ParkingLotSolver {
    private:
    
    public:
        std::vector< std::tuple<unsigned, Direction, unsigned> > Solve();
        std::vector< std::tuple<unsigned, Direction, unsigned> > SolveOptimally();
};

#endif
