#include <algorithm>
#include <array>
#include <chrono>
#include <vector>
#include <cmath>

#include "Headers/Global.hpp"
#include "Headers/MapCollision.hpp"

unsigned char map_collision(float i_x, float i_y, const std::vector<Cell>& i_check_cells, const Map& i_map)
{
	float cell_x = i_x / CELL_SIZE;
	float cell_y = i_y / CELL_SIZE;

	unsigned char output = 0;

	for (unsigned char a = 0; a < 4; a++)
	{
		short x;
		short y;

		switch (a)
		{
			case 0: //Top left cell
			{
				x = floor(cell_x);
				y = floor(cell_y);

				break;
			}
			case 1: //Top right cell
			{
				x = ceil(cell_x);
				y = floor(cell_y);

				break;
			}
			case 2: //Bottom left cell
			{
				x = floor(cell_x);
				y = ceil(cell_y);

				break;
			}
			case 3: //Bottom right cell
			{
				x = ceil(cell_x);
				y = ceil(cell_y);
			}
		}

		if (0 <= x && x < i_map.size())
		{
			if (0 <= y && y < i_map[0].size())
			{
				//Checking if the cell's type is one of the cell types we need to check.
				if (i_check_cells.end() != std::find(i_check_cells.begin(), i_check_cells.end(), i_map[x][y]))
				{
					//In binary this will be 0001, 0010, 0100 or 1000.
					output += pow(2, a);
				}
			}
		}
		//We're gonna assume that the map borders are walls.
		else if (i_check_cells.end() != std::find(i_check_cells.begin(), i_check_cells.end(), Cell::Wall))
		{
			output += pow(2, a);
		}
	}

	return output;
}
