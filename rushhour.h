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


////////////////////////////////////////////////////////////////////////////////
// your stuff
////////////////////////////////////////////////////////////////////////////////

std::vector< std::tuple<unsigned, Direction, unsigned> >
SolveRushHour(std::string const& filename);

std::vector< std::tuple<unsigned, Direction, unsigned> >
SolveRushHourOptimally(std::string const& filename);    //BFS






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
	int numMoves = 0;  //How many moves it took to get here
	unsigned numCars = 0;  //hNumber of cars in the parking lot
	std::tuple<unsigned, Direction, unsigned> moveToGetHere;  //The move used to get to this board state


	RushHour(const RushHour& other) : height(other.height), width(other.width), parking_lot(NULL), exit_direction(other.exit_direction), car(other.car), filename(other.filename), parentIndex(other.parentIndex), treeDepth(other.treeDepth), numMoves(other.numMoves), numCars(other.numCars), moveToGetHere(other.moveToGetHere)
	{
		unsigned * parking_lot_data = new unsigned[height*width];
		for (unsigned i = 0; i < height; i++)

			parking_lot = new unsigned*[height];
		for (unsigned i = 0; i < height; ++i) {
			parking_lot[i] = parking_lot_data + i*width;
			for (unsigned j = 0; j < width; j++)
			{
				parking_lot[i][j] = other.parking_lot[i][j];
			}
		}
	}



	bool operator==(const RushHour& other)
	{
		for(unsigned i=0;i<height;i++)
		{
			for (unsigned j = 0; j < width; j++)
			{
				if(parking_lot[i][j] != other.parking_lot[i][j])
				{
					return false;
				}
			}
		}
	
		return true;
	}

	bool isValidIndex(int x, int y)
	{
		return (x > height || x<0 || y>width || y < 0);
	}





	bool operator<(const RushHour& other) { return numMoves < other.numMoves; }
	bool operator>(const RushHour& other) { return numMoves > other.numMoves; }


	//public:
	//friend std::ostream& operator<<(std::ostream& os, RushHour const& pl);
	//void Print(std::string const& filename_out);
	unsigned const& Car() const { return car; }
	Direction const& Dir() const { return exit_direction; }

	//RushHour( std::string const&  filename );
	~RushHour();
	bool makeMove(std::tuple< unsigned, Direction, unsigned > move);
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

	

	//int CheckBrief(std::vector< std::tuple<unsigned, Direction, unsigned> > const& sol);




	//void Print(std::string const& filename_out);





};







//
//
//
//class ParkingLotSolver {
//    private:
//    
//    public:
//        std::vector< std::tuple<unsigned, Direction, unsigned> > Solve();
//        std::vector< std::tuple<unsigned, Direction, unsigned> > SolveOptimally();
//};













#endif
