#include <vector>
#include <algorithm>  // for reverse
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class GameOfLifeBoard {
    public:
        vector<vector<bool>> _board;
        int _rows;
        int _columns;

		GameOfLifeBoard() {
			_rows = 70;
			_columns = 100;
			for(size_t i {0}; i < _rows; i++) {
				_board.push_back(vector<bool>());
				for(size_t j {0}; j < _columns; j++) {
					_board[i].push_back(false);
				}
			}
		}

		void writeBoardToFile(string filename) {
			ofstream ofs(filename);
			if (ofs.is_open()) {
				for (auto row : _board) {
					for (auto entry : row) {
						ofs << entry;
					}
					ofs << endl;
				}
				ofs.close();
			}
		}

		void setGliderGun(int row, int col) {
			if (row < 0 || col < 0 || (row+8) > this->_rows || (col+35) > this->_columns)
				throw runtime_error("Glider gun can only be set inside the board!");

			this->reviveCell(row + 5, col + 1);
			this->reviveCell(row + 5, col + 2);
			this->reviveCell(row + 6, col + 1);
			this->reviveCell(row + 6, col + 2);
			this->reviveCell(row + 3, col + 14);
			this->reviveCell(row + 3, col + 13);
			this->reviveCell(row + 4, col + 12);
			this->reviveCell(row + 5, col + 11);
			this->reviveCell(row + 6, col + 11);
			this->reviveCell(row + 7, col + 11);
			this->reviveCell(row + 8, col + 12);
			this->reviveCell(row + 9, col + 13);
			this->reviveCell(row + 9, col + 14);
			this->reviveCell(row + 6, col + 15);
			this->reviveCell(row + 4, col + 16);
			this->reviveCell(row + 8, col + 16);
			this->reviveCell(row + 5, col + 17);
			this->reviveCell(row + 6, col + 17);
			this->reviveCell(row + 7, col + 17);
			this->reviveCell(row + 6, col + 18);
			this->reviveCell(row + 3, col + 21);
			this->reviveCell(row + 3, col + 22);
			this->reviveCell(row + 4, col + 21);
			this->reviveCell(row + 4, col + 22);
			this->reviveCell(row + 5, col + 21);
			this->reviveCell(row + 5, col + 22);
			this->reviveCell(row + 2, col + 23);
			this->reviveCell(row + 6, col + 23);
			this->reviveCell(row + 1, col + 25);
			this->reviveCell(row + 2, col + 25);
			this->reviveCell(row + 6, col + 25);
			this->reviveCell(row + 7, col + 25);
			this->reviveCell(row + 3, col + 35);
			this->reviveCell(row + 4, col + 35);
			this->reviveCell(row + 3, col + 36);
			this->reviveCell(row + 4, col + 36);
		}

		void setRotator(int row, int col) {
			if (row < 0 || col < 0 || row >= this->_rows || col >= this->_columns)
				throw runtime_error("Rotator can only be set inside the board!");
			this->reviveCell(row, col);
			this->reviveCell(row+1, col);
			this->reviveCell(row-1, col);
		}

		void setPentadecathlon(int row, int col) {
			if ((row-1) < 0 || (col-4) < 0 || (row+1) >= this->_rows || (col+5) >= this->_columns)
				throw runtime_error("Rotator can only be set inside the board!");
			this->reviveCell(row, col-4);
			this->reviveCell(row, col-3);
			this->reviveCell(row+1, col-2);
			this->reviveCell(row-1, col-2);
			this->reviveCell(row, col-1);
			this->reviveCell(row, col);
			this->reviveCell(row, col+1);
			this->reviveCell(row, col+2);
			this->reviveCell(row+1, col+3);
			this->reviveCell(row-1, col+3);
			this->reviveCell(row, col+4);
			this->reviveCell(row, col+5);

		}

		void setBoard(vector<vector<bool>> board) {
			_board = board;
		}

		GameOfLifeBoard mirrorAtMajorDiagonal() {
			GameOfLifeBoard temp = this->mirrorVertically();
			return temp.mirrorHorizontally();
		}

		GameOfLifeBoard mirrorHorizontally() {
			GameOfLifeBoard board = *this;
			reverse(board._board.begin(), board._board.end());
			return board;
		}

		GameOfLifeBoard mirrorVertically() {
			GameOfLifeBoard board = *this;
			for (auto &row : board._board) {
				reverse(row.begin(), row.end());	
			}
			return board;
		}

		void readBoardFromFile(string filename) {
			_board = vector<vector<bool>>();
			_rows = 0;
			_columns = 0;
			cout << "Board:" << endl;
			ifstream ifs(filename);
			string line;
			int i { 0 }, value { 0 };
			int j = 0;

			if (ifs.is_open()) {
				cout << "Opening " << filename << " to read board..." << endl;
				while (getline(ifs, line)) {
					if (line.length() != _columns && _rows > 0)
						throw runtime_error("Invalid size of row when loading from file!");
					_columns = line.length();
					_board.push_back(vector<bool>());
					for (char const &character : line) {
						value = character - '0';
						_board[_rows].push_back(value);
					}
					++_rows;
				}
			}
			cout << "Imported board:" << endl;
			this->print();
			cout << endl << "With size rows=" << _rows << ", columns=" << _columns << endl;
			ifs.close();
		}

        bool doCellCycle(int row, int column) {
            int number_of_neighbors = getNumberOfNeighbors(row, column);
            //cout << "\tneighbors: " << number_of_neighbors << endl;

            if((number_of_neighbors == 2 || number_of_neighbors == 3) && _board[row][column]) {
                return true;
            } else if(number_of_neighbors < 2) {
                return false;
            } else if(number_of_neighbors > 3) {
                return false;
            } else if(number_of_neighbors == 3 && !_board[row][column]) {
                return true;
            } else if(number_of_neighbors == 2 && !_board[row][column]) {
                return false;
            } else {
                throw runtime_error("Could not evaluate cell!");
            }
        }

        void doGameCycle() {

            vector<vector<bool>> _tmp_board;  // copy of original board to avoid already updated cells, to affect the cells that still need updating

            for (size_t i = 0; i < _rows; i++)
            {   
                _tmp_board.push_back(vector<bool>());
                for (size_t j = 0; j < _columns; j++)
                {
                    //cout << "\nEvaluating cell " << i << ", " << j << endl;
                    _tmp_board[i].push_back(doCellCycle(i,j));
                }
            }

            _board = _tmp_board;
        }

        void initBoard() {
            for (size_t i = 0; i < _rows; i++)
            {
                _board.push_back(vector<bool>());
                for (size_t j = 0; j < _columns; j++)
                {
                    _board[i].push_back(false);
                }
            }
        }

		vector<vector<bool>> operator+(GameOfLifeBoard gol_board) {
			vector<vector<bool>> output;

			if (gol_board._rows != this->_rows || gol_board._columns != this->_columns)
				throw runtime_error("Dimensions of the boards do not allow element-wise addition");

			for (int row {0}; row < this->_rows; row++) {
				output.push_back(vector<bool>());
				for (int col {0}; col < this->_columns; col++) {
					cout << row << " " << col << " " << (this->_board[row][col] + gol_board._board[row][col]) << endl;
					output[row].push_back(this->_board[row][col] || gol_board._board[row][col]);
				}
			}
			this->print();
			gol_board.print();
			return output;
		}

        void killCell(int row, int column) {
            _board[row][column] = false;
        }
        
        void print() {
            // print upper border
            for (size_t i = 0; i < _columns+2; i++)
            {
                cout << "-";
            }
            cout << endl;

            // print rows in between
            for (size_t i = 0; i < _rows; i++)
            {
                cout << "|";
                for (size_t j = 0; j < _columns; j++)
                {
                   (_board[i][j]) ? cout << "X" : cout << " "; 
                }
                cout << "|" << endl;                
            }
            
            // print lower border
            for (size_t i = 0; i < _columns+2; i++)
            {
                cout << "-";
            } 
            cout << endl;
        }

        void reviveCell(int row, int column) {
            _board[row][column] = true;
        }


    //private:
        int getNumberOfNeighbors(int row, int column) {
            int count = 0;
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    if(i != 0 || j != 0) {
                        if ((row+i) >= 0 && (column+j) >= 0) { // if not at the upper or left border
                            if (_board[(row+i)%_rows][(column+j)%_columns]) {  // "x%rows" handles the upper bound borders
                                count++;
                            }
                        } else if ((row+i) == -1 && (column+j) == -1) {
                            if (_board[_rows-1][_columns-1]) count ++;
                        } else if ((row+i) != -1 && (column+j) == -1) {
                            if (_board[(row+i)%_rows][_columns-1]) count ++;
                        } else if ((row+i) == -1 && (column+j) != -1) {
                            if (_board[_rows-1][(column+j)%_columns]) count ++;
                        }
                    }
                }
            }
            return count;
        }

};
