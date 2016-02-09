#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

//expects 1 command line argument apart from the executable - the file name of the input board
bool is_number(const std::string& num)
{
	for(unsigned int i=0;i<num.length();++i)
	{
		if(!isdigit(num[i]))
			return false;
	}
	return true;
}

bool is_ok_to_add(const std::vector<std::vector<std::string> >& bd, int row,int col,std::string num)
{
	int multiplier = (int)pow(bd.size(),0.5);
	int low = 0;
	int high = bd.size()-1;
	int i;
	int j;
	
	//Row check
	for(i=low;i<=high;++i)
	{
		if(bd[row][i]==num)
		{
			if(i!=col)
				return false;
		}
	}
	
	
	//column check
	for(i=low;i<=high;++i)
	{
		if(bd[i][col]==num)
		{
			if(i!=row)
				return false;
		}
	}	
	
	int row_multieth = row/multiplier;
	int col_multieth = col/multiplier;
	
	int row_low = row_multieth*multiplier;
	int row_high = (row_low + multiplier) - 1;
	
	int col_low = col_multieth*multiplier;
	int col_high = (col_low + multiplier) - 1;
	
	for(i=row_low;i<=row_high;++i)
	{
		for(j=col_low;j<=col_high;++j)
		{
			if(bd[i][j]==num)
			{
				if(i!=row || j!=col)
					return false;
			}
		}
	}

	return true;
	
}


bool is_valid_board(const std::vector<std::vector<std::string> >& bd)
{
	int low = 0;
	int high = bd.size()-1;
	int outer;
	int inner;
	
	std::string star("*");
	for(outer=low;outer<=high;++outer)
	{
		for(inner=low;inner<=high;++inner)
		{
			if(bd[outer][inner]!= star)
			{
				if(!is_ok_to_add(bd,outer,inner,bd[outer][inner]))
					return false;
			}	
		}
	}
	
	return true;
}

bool is_solved_board(const std::vector<std::vector<std::string> >& bd)
{
	int low = 0;
	int high = bd.size()-1;
	int outer;
	int inner;
	
	std::string star("*");
	for(outer=low;outer<=high;++outer)
	{
		for(inner=low;inner<=high;++inner)
		{
			if(bd[outer][inner]!= star)
			{
				if(!is_ok_to_add(bd,outer,inner,bd[outer][inner]))
					return false;
			}
			else
				return false;
		}
	}
	
	return true;
}

void print_board(const std::vector<std::vector<std::string> >& bd)
{
	int multiplier = (int)pow(bd.size(),0.5);
	int low = 0;
	int high = bd.size()-1;
	int num_digits_high = 0;
	int temp = high+1;
	
	while(temp>0)
	{
		temp=temp/10;
		++num_digits_high;
	}
	
	std::string dashes ((multiplier*(1+num_digits_high) +2)*multiplier +1, '-');
	for(int outer=low;outer<=high;++outer)
	{
		if(outer%multiplier==0)
			std::cout << dashes << "\n";
			
		for(int inner=low;inner<=high;++inner)
		{
			if(inner%multiplier==0)
				std::cout << "| " ;
			std::cout.width(num_digits_high); std::cout << std::left << bd[outer][inner] << " ";
			if(inner==high)
				std::cout << "| " ;
		}
		std::cout << "\n" ;
		
		if(outer==high)
			std::cout << dashes << "\n";
		
	}
}

//bool solved board
int main(int argc, char * argv[] )
{
	//Reading Phase
	std::ifstream file_rd_str(argv[1]);
	
	//if file can't be opened, we exit
	if(!file_rd_str.good())
	{
		std::cerr << "Can't open " << argv[1] << " to read.\n";
		//exit(1); which is the same as 
		return 1;
	}
	
	std::string line;
	std::string new_line;
	std::string token;
	std::vector<std::vector<std::string> > board;
	while(getline(file_rd_str,line))
	{
		std::vector<std::string> row_vec;
		std::istringstream iss(line);
		
		while(iss >> token)
			row_vec.push_back(token);
		
		board.push_back(row_vec);
	}
	
	file_rd_str.close();
	
	if(board.size()!=board[0].size())
	{
		std::cerr << "Invalid dimensions of sudoku baord.\n";
		//exit(1); which is the same as 
		return 1;		
	}
	
	print_board(board);
	int row;
	int col;
	std::string number;
	if(is_valid_board(board) && !is_solved_board(board))
	{
		std::cout << "Proceed to solve...\n";
		do
		{
			std::cout << "\nOptions:\nTo enter a number type 0\nTo end type 1\nType option==> ";
			int option;
			if(!(std::cin >> option))
			{
				std::cout << "Illegal input. Good bye. \n";
				return 1;
			}
			else if(option==1)
				break;
			
			else if(option==0)
			{
				std::cout << "Enter row, column and number.\n";
				std::cin >> row >> col >> number;
				
				if(!is_number(number))
					std::cout << "Invalid number. Try again.\n";
					
				if(is_ok_to_add(board,row,col,number))
				{
					board[row][col]=number;
					print_board(board);
				}
				else
					std::cout << "number not allowed at that position. \n";
				
				
			}
		
		
		}while(true);
	}
	
	return 0;
}