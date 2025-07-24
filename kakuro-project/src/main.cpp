#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <string>
#include <cctype>
#include <algorithm>
#include <stdexcept>
#include <set>

using namespace std;

//----------------------------
// ENUMERATIONS and CONSTANTS
//----------------------------

enum class CellType {
    BLACK,    // unmodifiable black cell (no play)
    CLUE,     // cell that provides sum clues for adjacent groups
    PLAYABLE  // cell to be filled with a number 1-9
};

//----------------------------
// CLASS: Cell (Abstract Base)
//----------------------------

class Cell {
public:
    virtual CellType getType() const = 0;
    virtual ~Cell() = default;
};

//----------------------------
// CLASS: BlackCell
//----------------------------

class BlackCell : public Cell {
public:
    CellType getType() const override { return CellType::BLACK; }
};

//----------------------------
// CLASS: ClueCell
//----------------------------

class ClueCell : public Cell {
public:

    int horizontalClue;
    int verticalClue;

    ClueCell(int hClue, int vClue)
        : horizontalClue(hClue), verticalClue(vClue) {
    }

    CellType getType() const override { return CellType::CLUE; }
};

//----------------------------
// CLASS: PlayableCell
//----------------------------

class PlayableCell : public Cell {
public:
    int value; // 0 means unfilled, otherwise 1-9

    PlayableCell() : value(0) {}

    CellType getType() const override { return CellType::PLAYABLE; }
};

//----------------------------
// CLASS: KakuroGrid
//----------------------------

class KakuroGrid {
public:
    int rows;
    int cols;

    vector<vector<shared_ptr<Cell>>> grid;


    struct Group {
        int requiredSum;
        vector<PlayableCell*> cells;
    };
    vector<Group> groups;

    KakuroGrid(int r, int c) : rows(r), cols(c) {
        grid.resize(rows, vector<shared_ptr<Cell>>(cols, nullptr));
    }

    // Print the grid (for debugging or display)
    void print() const {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                CellType type = grid[i][j]->getType();
                if (type == CellType::BLACK) {
                    cout << " X ";
                }
                else if (type == CellType::CLUE) {

                    auto clue = dynamic_pointer_cast<ClueCell>(grid[i][j]);
                    cout << "[";
                    if (clue->verticalClue > 0)
                        cout << "v" << clue->verticalClue;
                    else
                        cout << "  ";
                    cout << "/";
                    if (clue->horizontalClue > 0)
                        cout << "h" << clue->horizontalClue;
                    else
                        cout << "  ";
                    cout << "]";
                }
                else if (type == CellType::PLAYABLE) {
                    auto play = dynamic_pointer_cast<PlayableCell>(grid[i][j]);
                    if (play->value == 0)
                        cout << " . ";
                    else
                        cout << " " << play->value << " ";
                }
            }
            cout << "\n";
        }
    }


    void computeGroups() {
        groups.clear();

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (grid[i][j]->getType() == CellType::CLUE) {
                    auto clue = dynamic_pointer_cast<ClueCell>(grid[i][j]);

                    if (clue->horizontalClue > 0) {
                        Group g;
                        g.requiredSum = clue->horizontalClue;
                        int col = j + 1;
                        while (col < cols && grid[i][col]->getType() == CellType::PLAYABLE) {
                            auto play = dynamic_pointer_cast<PlayableCell>(grid[i][col]);
                            g.cells.push_back(play.get());
                            col++;
                        }
                        if (!g.cells.empty())
                            groups.push_back(g);
                    }

                    if (clue->verticalClue > 0) {
                        Group g;
                        g.requiredSum = clue->verticalClue;
                        int row = i + 1;
                        while (row < rows && grid[row][j]->getType() == CellType::PLAYABLE) {
                            auto play = dynamic_pointer_cast<PlayableCell>(grid[row][j]);
                            g.cells.push_back(play.get());
                            row++;
                        }
                        if (!g.cells.empty())
                            groups.push_back(g);
                    }
                }
            }
        }
    }
};

//----------------------------
// Abstract GridLoader (Factory Pattern)
//----------------------------

class GridLoader {
public:
    virtual unique_ptr<KakuroGrid> loadGrid(const string& filename) = 0;
    virtual ~GridLoader() = default;
};



class SimpleGridLoader : public GridLoader {
public:
    unique_ptr<KakuroGrid> loadGrid(const string& filename) override {
        ifstream infile(filename);
        if (!infile)
            throw runtime_error("Unable to open file: " + filename);

        int rows, cols;
        infile >> rows >> cols;
        infile.ignore(); // skip end-of-line

        auto gridPtr = make_unique<KakuroGrid>(rows, cols);

        for (int i = 0; i < rows; i++) {
            string line;
            getline(infile, line);
            istringstream iss(line);
            for (int j = 0; j < cols; j++) {
                string token;
                if (!(iss >> token))
                    throw runtime_error("Invalid grid format in file.");

                if (token == "X") {
                    gridPtr->grid[i][j] = make_shared<BlackCell>();
                }
                else if (token[0] == 'C') {
                    // Remove the leading 'C'
                    token.erase(0, 1);
                    int vClue = 0, hClue = 0;
                    // Expected format "vertical/horizontal"
                    size_t slashPos = token.find('/');
                    if (slashPos != string::npos) {
                        string vStr = token.substr(0, slashPos);
                        string hStr = token.substr(slashPos + 1);
                        vClue = stoi(vStr);
                        hClue = stoi(hStr);
                    }
                    else {
                        vClue = stoi(token);
                    }
                    gridPtr->grid[i][j] = make_shared<ClueCell>(hClue, vClue);
                }
                else if (token == ".") {
                    gridPtr->grid[i][j] = make_shared<PlayableCell>();
                }
                else if (isdigit(token[0])) {
                    // Pre-filled playable cell.
                    auto cell = make_shared<PlayableCell>();
                    cell->value = stoi(token);
                    gridPtr->grid[i][j] = cell;
                }
                else {
                    throw runtime_error("Unknown token in grid: " + token);
                }
            }
        }
        gridPtr->computeGroups();
        return gridPtr;
    }
};

//----------------------------
// AdvancedGridLoader Implementation

//----------------------------

class AdvancedGridLoader : public GridLoader {
public:
    unique_ptr<KakuroGrid> loadGrid(const string& filename) override {
        ifstream infile(filename);
        if (!infile)
            throw runtime_error("Unable to open file: " + filename);

        string header;
        getline(infile, header);

        istringstream headerStream(header);
        int rows, cols;
        char comma;
        headerStream >> rows >> comma >> cols;

        auto gridPtr = make_unique<KakuroGrid>(rows, cols);

        for (int i = 0; i < rows; i++) {
            string line;
            getline(infile, line);
            istringstream iss(line);
            for (int j = 0; j < cols; j++) {
                string token;
                if (!getline(iss, token, ','))
                    throw runtime_error("Invalid CSV grid format.");
                // Remove spaces.
                token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
                if (token == "X") {
                    gridPtr->grid[i][j] = make_shared<BlackCell>();
                }
                else if (token[0] == 'C') {

                    token.erase(0, 1);
                    int vClue = 0, hClue = 0;
                    size_t dashPos = token.find('-');
                    if (dashPos != string::npos) {
                        string vStr = token.substr(0, dashPos);
                        string hStr = token.substr(dashPos + 1);
                        vClue = stoi(vStr);
                        hClue = stoi(hStr);
                    }
                    else {
                        vClue = stoi(token);
                    }
                    gridPtr->grid[i][j] = make_shared<ClueCell>(hClue, vClue);
                }
                else if (token == ".") {
                    gridPtr->grid[i][j] = make_shared<PlayableCell>();
                }
                else if (isdigit(token[0])) {
                    auto cell = make_shared<PlayableCell>();
                    cell->value = stoi(token);
                    gridPtr->grid[i][j] = cell;
                }
                else {
                    throw runtime_error("Unknown token in CSV grid: " + token);
                }
            }
        }
        gridPtr->computeGroups();
        return gridPtr;
    }
};

//----------------------------
// GridLoaderFactory (Factory Design Pattern)
//----------------------------

class GridLoaderFactory {
public:

    static unique_ptr<GridLoader> createLoader(const string& type) {
        if (type == "simple")
            return make_unique<SimpleGridLoader>();
        else if (type == "advanced")
            return make_unique<AdvancedGridLoader>();
        else
            throw invalid_argument("Unknown loader type: " + type);
    }
};

//----------------------------
// CLASS: Solver
//----------------------------


class Solver {
public:
    KakuroGrid* grid;
    // List of pointers to all playable cells for easier backtracking.
    vector<PlayableCell*> playableCells;

    Solver(KakuroGrid* g) : grid(g) {
        // Collect all playable cells.
        for (int i = 0; i < grid->rows; i++) {
            for (int j = 0; j < grid->cols; j++) {
                if (grid->grid[i][j]->getType() == CellType::PLAYABLE) {
                    auto play = dynamic_pointer_cast<PlayableCell>(grid->grid[i][j]);
                    playableCells.push_back(play.get());
                }
            }
        }
    }


    bool isValidGroup(const KakuroGrid::Group& group) {
        int sum = 0;
        set<int> used;
        int unfilled = 0;
        for (auto cell : group.cells) {
            int v = cell->value;
            if (v == 0) {
                unfilled++;
            }
            else {
                if (used.find(v) != used.end())
                    return false;
                used.insert(v);
                sum += v;
            }
        }
        if (unfilled == 0)
            return (sum == group.requiredSum);
        else
            return (sum < group.requiredSum);
    }


    bool isValid() {
        for (const auto& group : grid->groups)
            if (!isValidGroup(group))
                return false;
        return true;
    }


    bool solve(size_t index = 0) {
        if (index == playableCells.size())
            return isValid();

        for (int digit = 1; digit <= 9; digit++) {
            playableCells[index]->value = digit;
            if (isValid()) {
                if (solve(index + 1))
                    return true;
            }
            playableCells[index]->value = 0; // backtrack
        }
        return false;
    }

    // Display the solved grid.
    void displaySolution() {
        grid->print();
    }
};

//----------------------------
// MAIN FUNCTION
// Command-line usage:
//   Project.exe <grid_filename> <loader_type: simple>
//----------------------------

int main(int argc, char* argv[]) {

    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <grid_filename> <loader_type: simple|advanced>" << endl;
        return 1;
    }

    string filename = argv[1];
    string loaderType = argv[2];

    try {

        unique_ptr<GridLoader> loader = GridLoaderFactory::createLoader(loaderType);
        unique_ptr<KakuroGrid> kakuro = loader->loadGrid(filename);

        cout << "Loaded Kakuro grid:" << endl;
        kakuro->print();
        cout << "\nComputing solution..." << endl;

        Solver solver(kakuro.get());
        if (solver.solve()) {
            cout << "\nSolution found:" << endl;
            solver.displaySolution();
        }
        else {
            cout << "\nNo solution exists for the given grid." << endl;
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
