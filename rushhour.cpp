#include <cstdio> /* sscanf */
#include <iostream>
#include <fstream>
#include <regex>    /* regular expression */
#include <vector>
#include <tuple>
#include <set>
#include <stack>
#include <queue>
#include <functional>
#include "rushhour.h"



//
//// Keep this
//////////////////////////////////////////////////////////////////////////////////
//// global functions
//std::vector< std::tuple<unsigned, Direction, unsigned> >
//SolveRushHour(std::string const& filename)   //DFS
//{
//	RushHour r(filename);  return r.Solve();
//}
//std::vector< std::tuple<unsigned, Direction, unsigned> >
//SolveRushHourOptimally(std::string const& filename)    //BFS
//{
//	RushHour r(filename); return r.SolveOptimally();
//}














// Keep this
std::ostream& operator<<( std::ostream& os, Direction const& d ) {
    switch ( d ) {
        case up:    os << "up "; break;
        case left:  os << "left "; break;
        case down:  os << "down "; break;
        case right: os << "right "; break;
        default:    os << "undefined"; break;
    }
    return os;
}

/*
void RushHour::Print(std::string const& filename_out)
{
	std::ofstream os;
	os.open(filename_out, std::ofstream::out | std::ofstream::app);
	for (unsigned i = 0; i<height; ++i) {
		for (unsigned j = 0; j<width; ++j) {
			os << parking_lot[i][j] << " ";
		}
	}
	os << std::endl;
}
*/


RushHour::~RushHour()
{
	for (unsigned i = 0; i < height; i++)
	{
		if (parking_lot[i])
		{
			delete[] parking_lot[i];
		}
	}
	delete[] parking_lot;
}




int RushHour::CheckBrief(std::vector< std::tuple<unsigned, Direction, unsigned> > const& sol)
{
	std::cout << "Number of steps = " << sol.size() << std::endl;
	try {
		for (std::tuple<unsigned, Direction, unsigned> const& m : sol) {
			makeMove(m);
		}
		bool final_pos = IsSolved();
		std::cout << "Solved = " << final_pos << std::endl;
		return final_pos;
	}
	catch (char const * msg) {
		std::cout << "ERROR - " << msg << std::endl;
		return 1;
	}
}


std::ostream& operator<<(std::ostream& os, RushHour const& pl)
{
	//    os << pl.height << std::endl;
	//    os << pl.width << std::endl;
	for (unsigned i = 0; i<pl.height; ++i) {
		for (unsigned j = 0; j<pl.width; ++j) {
			os << pl.parking_lot[i][j] << " ";
		}
		os << std::endl;
	}
	return os;
}

bool RushHour::IsSolved() const
{
	unsigned i_car_pos = height;
	unsigned j_car_pos = width;
	Orientation orientation = horisontal;
	bool done = false;
	for (unsigned i = 0; i<height && !done; ++i) {
		for (unsigned j = 0; j<width && !done; ++j) {
			if (parking_lot[i][j] == car) {
				if (i_car_pos == height) { // first time seeing car
					i_car_pos = i;
					j_car_pos = j;
				}
				else { //second time - ready to make decision
					if (j_car_pos == j) { // default is horisontal
						orientation = vertical;
					}
					done = true;
				}
			}
		}
	}

	bool solved = false;
	switch (exit_direction) {
	case up:    solved = orientation == vertical   && parking_lot[0][j_car_pos] == car; break;
	case left:  solved = orientation == horisontal && parking_lot[i_car_pos][0] == car; break;
	case down:  solved = orientation == vertical   && parking_lot[height - 1][j_car_pos] == car; break;
	case right: solved = orientation == horisontal && parking_lot[i_car_pos][width - 1] == car; break;
	default: break;
	}
	return solved;
}

int RushHour::Check(std::vector< std::tuple<unsigned, Direction, unsigned> > const& sol)
{
	try {
		// Print( "solution.txt" );
		for (std::tuple<unsigned, Direction, unsigned> const& m : sol) {
			std::cout << "Car " << std::get<0>(m) << " to be moved " << std::get<1>(m) << " " << std::get<2>(m) << " positions\n";
			makeMove(m);
			// Print( "solution.txt" );
			std::cout << *this;
		}
		bool final_pos = IsSolved();
		std::cout << "Number of steps = " << sol.size() << std::endl;
		std::cout << "Solved = " << final_pos << std::endl;
		return final_pos;
	}
	catch (char const * msg) {
		std::cout << "ERROR - " << msg << std::endl;
		return 1;
	}
}


RushHour::RushHour( std::string const& filename) : filename(filename)
{
	std::ifstream infile(filename);
	std::string   data; // all data from input file

	if (!infile.is_open()) {
		std::cerr << "Errors in input file: cannot open \"" << filename << "\"" << std::endl;
		throw "Errors in input file: cannot open";
	}

	// read the whole file into a string
	while (!infile.eof()) {
		std::string line;
		std::getline(infile, line);
		data.append(line);
		data.append(" ");
	}

	infile.close();

	// parse data
	std::smatch match;

	if (std::regex_match(data, match, std::regex(".*width\\s+(\\d+).*", std::regex_constants::icase)) && match.size() == 2) {
		// The first sub_match is the whole string; the next
		// sub_match is the first parenthesized expression.
		width = std::stoi(match[1]);
		data = std::regex_replace(data, std::regex("width\\s+\\d+", std::regex_constants::icase), "");
	}
	else {
		std::cerr << "Errors in input file: cannot find \"width\"" << std::endl;
		throw "Errors in input file: cannot find \"width\"";
	}
	if (std::regex_match(data, match, std::regex(".*height\\s+(\\d+).*", std::regex_constants::icase)) && match.size() == 2) {
		height = std::stoi(match[1]);
		data = std::regex_replace(data, std::regex("height\\s+\\d+", std::regex_constants::icase), "");
	}
	else {
		std::cerr << "Errors in input file: cannot find \"height\"" << std::endl;
		throw "Errors in input file: cannot find \"height\"";
	}
	if (std::regex_match(data, match, std::regex(".*car\\s+(\\d+).*", std::regex_constants::icase)) && match.size() == 2) {
		car = std::stoi(match[1]);
		data = std::regex_replace(data, std::regex("car\\s+\\d+", std::regex_constants::icase), "");
	}
	else {
		std::cerr << "Errors in input file: cannot find \"car\"" << std::endl;
		throw "Errors in input file: cannot find \"car\"";
	}
	if (std::regex_match(data, match, std::regex(".*exit\\s+([a-z]+).*", std::regex_constants::icase)) && match.size() == 2) {
		std::string dir_str = match[1].str();
		if (dir_str == "left") { exit_direction = left; }
		else if (dir_str == "right") { exit_direction = right; }
		else if (dir_str == "up") { exit_direction = up; }
		else if (dir_str == "down") { exit_direction = down; }
		else { throw "unknown exit direction "; }
		data = std::regex_replace(data, std::regex("exit\\s+[a-z]+", std::regex_constants::icase), "");
	}
	else {
		std::cerr << "Errors in input file: cannot find \"exit\"" << std::endl;
		throw "Errors in input file: cannot find \"exit\"";
	}

	// only cells are left in the data
	std::regex re_cell("(\\d+)"); // each cell is a number or dot
	std::sregex_iterator cell_matches_begin = std::sregex_iterator(data.begin(), data.end(), re_cell);
	std::sregex_iterator cell_matches_end = std::sregex_iterator();

	// should have exactly height*width numbers and dots
	if (std::distance(cell_matches_begin, cell_matches_end) == height*width) {
		unsigned * parking_lot_data = new unsigned[height*width];
		parking_lot = new unsigned*[height];
		for (unsigned i = 0; i<height; ++i) {
			parking_lot[i] = parking_lot_data + i*width;
		}
		unsigned pos = 0;
		for (std::sregex_iterator iter = cell_matches_begin; iter != cell_matches_end; ++iter) {
			parking_lot_data[pos++] = std::stoi((*iter).str());
		}
	}
	else {
		std::cerr << "Errors in input file: number of cells should be " << height << "*" << width << ". Found " << std::distance(cell_matches_begin, cell_matches_end) << std::endl;
		throw "Errors in input file: number of cells";
	}
}

// Direction is an enum (see header):
// +------------>j
// |    ^ 0
// |    |
// |1<--+-->3
// |    |
// |    V 2
// V        update (i,j) given 1 step in direction d
// i        i += (d-1)*((3-d)%2)    j += (d-2)*(d%2)
// 0 = -1,0
// 1 = 0,-1
// 2 = 1,0
// 3 = 0,1
//

//                                move = car,    direction, num positions
bool RushHour::makeMove(std::tuple< unsigned, Direction, unsigned > move)  //Returns TRUE if the move went through without a hitch
{
	int d = std::get<1>(move); // convert direction to int
	int di = (d - 1)*((3 - d) % 2);    // see comment before function
	int dj = (d - 2)*(d % 2);        // see comment before function
	int scan_direction = di + dj; // -1 (up,left) or 1 (down,right)

	unsigned num_positions = std::get<2>(move);
	unsigned car = std::get<0>(move);

	unsigned i_start = (scan_direction == 1) ? height - 1 : 0;
	unsigned j_start = (scan_direction == 1) ? width - 1 : 0;
	for (unsigned step = 0; step<num_positions; ++step) { // move car 1 position at a time
		for (unsigned i = i_start; i<height; i -= scan_direction) {
			for (unsigned j = j_start; j<width; j -= scan_direction) {
				if (parking_lot[i][j] == car) {
					parking_lot[i][j] = 0;
					// check if legal
					if (i + di >= height && j + dj >= width) {
						throw("Car moved outside of parking lot");
						parking_lot[i][j] = car;  //Reset car back to where it should be
						return false;
					}
					if (parking_lot[i + di][j + dj] > 0) {
						throw("Car moved on top of another car");
						parking_lot[i][j] = car;   //Reset car back to where it should be
						return false;
					}
					parking_lot[i + di][j + dj] = car;
				}
			}
		}
	}

	return true;
}


std::vector< std::tuple<unsigned, Direction, unsigned> > SolveRushHour(const std::string& filename)  //DFS
{
	std::vector<RushHour> holder;
	std::stack<RushHour> openList;
	std::vector<RushHour> closedList;
	std::vector<std::tuple<unsigned, Direction, unsigned>> soln;
	RushHour startingState(filename);


	openList.push(startingState);
	holder.push_back(startingState);

	//unsigned depth = 1;

	for (int depth = 1; true; depth++)
	{
		//	unsigned childDepth = 1;

		while (!openList.empty())
		{
			int pIndex = static_cast<int>( holder.size() - 1);
			RushHour parent(openList.top());
			openList.pop();
			//if(parent.treeDepth)



			if (parent.IsSolved())
			{

				//Loop back through and get solution
				while(parent.parentIndex != -1)
				{
					soln.push_back(parent.moveToGetHere);
					parent = holder[parent.parentIndex];
				}
				return soln;

			}







			else
			{
				if(parent.treeDepth<depth)
				{
				for (unsigned i = 1; i < startingState.numCars; i++)
				{
					//For each car, spawn its children
					Orientation orientation=horisontal;
					unsigned indexX = 0;
					unsigned indexY = 0;  //Won't know if this is the LEFTMOST index or the TOPMOST index until after we make the check

					//Look through parking lot to determine particular car's orientation
					for (unsigned x = 0; x < startingState.height; x++)
					{
						for (unsigned y = 0; x < startingState.width; y++)
						{
							if (startingState.isValidIndex(x, y) && startingState.parking_lot[x][y] == i)
							{
								indexX = x;
								indexY = y;
								if (startingState.isValidIndex(x + 1, y) && startingState.parking_lot[x + 1][y] == i)  //Check that car is vertical
								{
									orientation = vertical;
								}
								else
								{
									orientation = horisontal;
								}

								x = startingState.height;
								y = startingState.width;
							}
						}
					}


					if (orientation == vertical)  //Push back children trying vertical moves
					{
						RushHour childU(parent), childD(parent);
						childU.treeDepth++;
						childU.parentIndex = pIndex;
						childU.parentIndex = pIndex;
						childD.treeDepth++;
						unsigned botIndex = indexX;
						childU.numMoves++;
						childD.numMoves++;




						//car, direction, #positions moved
						for (unsigned q = indexX; q < startingState.height; q++)
						{
							if (childU.parking_lot[q][indexY] == childU.car)
							{
								botIndex = q;
							}
						}

						//Push back all possible down, up moves for this car

						for (unsigned q = botIndex; q < startingState.height; q++)
						{
							if (childU.makeMove(std::tuple<unsigned, Direction, unsigned>(i, up, startingState.height - q)))//If the move goes through, it'll return TRUE
							{
								childU.moveToGetHere = std::tuple<unsigned, Direction, unsigned>(i, up, startingState.height - q);
								
								bool isOnClosedList = false;

								for (auto it = closedList.begin(); it != closedList.end(); it++)
								{
									if (*it == childU)
									{
										isOnClosedList = true;
										it = closedList.end()-1;
									}
								}






								if (!isOnClosedList)
								{
									holder.push_back(childU);
									openList.push(childU);
								}
							
							//If the move went through, we'll need to reset this child back to the original board state before performing more moves
								childU = childD;
							}
						}

						childU = childD;
						//childD = childU;



						for (unsigned q = 1; q < indexX; q++)   //botIndex is the bottom-most index, so indexX is the topmost
						{
							if (childD.makeMove(std::tuple<unsigned, Direction, unsigned>(i, down, q)))
							{


								childD.moveToGetHere = std::tuple<unsigned, Direction, unsigned>(i, down,  q);

								bool isOnClosedList = false;

								for (auto it = closedList.begin(); it != closedList.end(); it++)
								{
									if (*it == childD)
									{
										isOnClosedList = true;
										it = closedList.end() - 1;
									}
								}






								if (!isOnClosedList)
								{
									holder.push_back(childD);
									openList.push(childD);
								}

								//If the move went through, we'll need to reset this child back to the original board state before performing more moves
								childU = childD;




							}

						}





					}

					else
					{
						RushHour childL(parent), childR(parent);
						childL.treeDepth++;
						childL.parentIndex = pIndex;
						childR.parentIndex = pIndex;
						childR.treeDepth++;
						unsigned rightIndex = indexY;
						childR.numMoves++;
						childL.numMoves++;



						//car, direction, #positions moved
						for (unsigned q = indexY; q < startingState.height; q++)
						{
							if (childL.parking_lot[indexX][q] == childL.car)
							{
								rightIndex = q;
							}
						}


						//Push back all possible left, right moves for this car









						for (unsigned q = rightIndex; q < startingState.width; q++)
						{
							if (childR.makeMove(std::tuple<unsigned, Direction, unsigned>(i, right, startingState.width - q)))//If the move goes through, it'll return TRUE
							{
								childR.moveToGetHere = std::tuple<unsigned, Direction, unsigned>(i, right, startingState.width - q);

								bool isOnClosedList = false;

								for (auto it = closedList.begin(); it != closedList.end(); it++)
								{
									if (*it == childR)
									{
										isOnClosedList = true;
										it = closedList.end() - 1;
									}
								}






								if (!isOnClosedList)
								{
									holder.push_back(childR);
									openList.push(childR);
								}

								//If the move went through, we'll need to reset this child back to the original board state before performing more moves
								childR = childL;
							}
						}

						childR = childL;
						//childD = childU;



						for (unsigned q = 1; q < indexY; q++)   //botIndex is the bottom-most index, so indexX is the topmost
						{
							if (childL.makeMove(std::tuple<unsigned, Direction, unsigned>(i, left, q)))
							{


								childL.moveToGetHere = std::tuple<unsigned, Direction, unsigned>(i, left, q);

								bool isOnClosedList = false;

								for (auto it = closedList.begin(); it != closedList.end(); it++)
								{
									if (*it == childL)
									{
										isOnClosedList = true;
										it = closedList.end() - 1;
									}
								}






								if (!isOnClosedList)
								{
									holder.push_back(childL);
									openList.push(childL);
								}

								//If the move went through, we'll need to reset this child back to the original board state before performing more moves
								childL = childR;




							}

						}


						//Children should now be pushed onto openList--now to push parent onto closed list


						closedList.push_back(parent);













					}

				}

			}

		}
	}

	}






	}












std::vector< std::tuple<unsigned, Direction, unsigned> > SolveRushHourOptimally(const std::string& filename)  //BFS
{
	std::vector<RushHour> holder;
	std::priority_queue<RushHour, std::vector<RushHour>,std::greater<RushHour>> openList;
	std::vector<RushHour> closedList;
	std::vector<std::tuple<unsigned, Direction, unsigned>> soln;
		


	RushHour startingState(filename);


	openList.push(startingState);
	holder.push_back(startingState);

	//unsigned depth = 1;


		//	unsigned childDepth = 1;

		while (!openList.empty())
		{
			int pIndex =  static_cast<int>(holder.size() - 1);
			RushHour parent(openList.top());
			openList.pop();
			//if(parent.treeDepth)



			if (parent.IsSolved())
			{

				//Loop back through and get solution
				while (parent.parentIndex != -1)
				{
					soln.push_back(parent.moveToGetHere);
					parent = holder[parent.parentIndex];
				}
				return soln;

			}







			else
			{
				
					for (unsigned i = 1; i < startingState.numCars; i++)
					{
						//For each car, spawn its children
						Orientation orientation = horisontal;
						unsigned indexX=0, indexY=0;  //Won't know if this is the LEFTMOST index or the TOPMOST index until after we make the check

												  //Look through parking lot to determine particular car's orientation
						for (unsigned x = 0; x < startingState.height; x++)
						{
							for (unsigned y = 0; x < startingState.width; y++)
							{
								if (startingState.isValidIndex(x, y) && startingState.parking_lot[x][y] == i)
								{
									indexX = x;
									indexY = y;
									if (startingState.isValidIndex(x + 1, y) && startingState.parking_lot[x + 1][y] == i)  //Check that car is vertical
									{
										orientation = vertical;
									}
									else
									{
										orientation = horisontal;
									}

									x = startingState.height;
									y = startingState.width;
								}
							}
						}


						if (orientation == vertical)  //Push back children trying vertical moves
						{
							RushHour childU(parent), childD(parent);
							childU.treeDepth++;
							childU.parentIndex = pIndex;
							childU.parentIndex = pIndex;
							childD.treeDepth++;
							unsigned botIndex = indexX;
							childU.numMoves++;
							childD.numMoves++;




							//car, direction, #positions moved
							for (unsigned q = indexX; q < startingState.height; q++)
							{
								if (childU.parking_lot[q][indexY] == childU.car)
								{
									botIndex = q;
								}
							}

							//Push back all possible down, up moves for this car

							for (unsigned q = botIndex; q < startingState.height; q++)
							{
								if (childU.makeMove(std::tuple<unsigned, Direction, unsigned>(i, up, startingState.height - q)))//If the move goes through, it'll return TRUE
								{
									childU.moveToGetHere = std::tuple<unsigned, Direction, unsigned>(i, up, startingState.height - q);

									bool isOnClosedList = false;

									for (auto it = closedList.begin(); it != closedList.end(); it++)
									{
										if (*it == childU)
										{
											isOnClosedList = true;
											it = closedList.end() - 1;
										}
									}






									if (!isOnClosedList)
									{
										holder.push_back(childU);
										openList.push(childU);
									}

									//If the move went through, we'll need to reset this child back to the original board state before performing more moves
									childU = childD;
								}
							}

							childU = childD;
							//childD = childU;



							for (unsigned q = 1; q < indexX; q++)   //botIndex is the bottom-most index, so indexX is the topmost
							{
								if (childD.makeMove(std::tuple<unsigned, Direction, unsigned>(i, down, q)))
								{


									childD.moveToGetHere = std::tuple<unsigned, Direction, unsigned>(i, down, q);

									bool isOnClosedList = false;

									for (auto it = closedList.begin(); it != closedList.end(); it++)
									{
										if (*it == childD)
										{
											isOnClosedList = true;
											it = closedList.end() - 1;
										}
									}






									if (!isOnClosedList)
									{
										holder.push_back(childD);
										openList.push(childD);
									}

									//If the move went through, we'll need to reset this child back to the original board state before performing more moves
									childU = childD;




								}

							}





						}

						else
						{
							RushHour childL(parent), childR(parent);
							childL.treeDepth++;
							childL.parentIndex = pIndex;
							childR.parentIndex = pIndex;
							childR.treeDepth++;
							unsigned rightIndex = indexY;
							childR.numMoves++;
							childL.numMoves++;



							//car, direction, #positions moved
							for (unsigned q = indexY; q < startingState.height; q++)
							{
								if (childL.parking_lot[indexX][q] == childL.car)
								{
									rightIndex = q;
								}
							}


							//Push back all possible left, right moves for this car









							for (unsigned q = rightIndex; q < startingState.width; q++)
							{
								if (childR.makeMove(std::tuple<unsigned, Direction, unsigned>(i, right, startingState.width - q)))//If the move goes through, it'll return TRUE
								{
									childR.moveToGetHere = std::tuple<unsigned, Direction, unsigned>(i, right, startingState.width - q);

									bool isOnClosedList = false;

									for (auto it = closedList.begin(); it != closedList.end(); it++)
									{
										if (*it == childR)
										{
											isOnClosedList = true;
											it = closedList.end() - 1;
										}
									}






									if (!isOnClosedList)
									{
										holder.push_back(childR);
										openList.push(childR);
									}

									//If the move went through, we'll need to reset this child back to the original board state before performing more moves
									childR = childL;
								}
							}

							childR = childL;
							//childD = childU;



							for (unsigned q = 1; q < indexY; q++)   //botIndex is the bottom-most index, so indexX is the topmost
							{
								if (childL.makeMove(std::tuple<unsigned, Direction, unsigned>(i, left, q)))
								{


									childL.moveToGetHere = std::tuple<unsigned, Direction, unsigned>(i, left, q);

									bool isOnClosedList = false;

									for (auto it = closedList.begin(); it != closedList.end(); it++)
									{
										if (*it == childL)
										{
											isOnClosedList = true;
											it = closedList.end() - 1;
										}
									}






									if (!isOnClosedList)
									{
										holder.push_back(childL);
										openList.push(childL);
									}

									//If the move went through, we'll need to reset this child back to the original board state before performing more moves
									childL = childR;




								}

							}


							//Children should now be pushed onto openList--now to push parent onto closed list


							closedList.push_back(parent);













						}

					}

				}

			
		

	}

		
	return soln;
		
}