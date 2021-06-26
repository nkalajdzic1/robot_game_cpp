#include <iostream>
#include <vector>
#include <cmath>

typedef std::vector<std::vector<std::vector<char>>> CharMatrix3D;
typedef std::vector<std::vector<char>> CharMatrix2D;

struct Coordinate {
    int x,y;
};

//------------------------------------------------- helping functions ------------------------------------------------------//

void error()
{
    std::cout<<"Wrong coordinates for the position of the robot!"<<std::endl;
}

bool check(CharMatrix2D &matrix, struct Coordinate begin, struct Coordinate end, int p)
{
    if(begin.x != end.x && begin.y != end.y) return false;
    if(begin.y + 1 > matrix.size() || end.y + 1 > matrix.size()) return false;
    if(begin.x + 1 > matrix.at(0).size() || end.x + 1 > matrix.at(0).size()) return false;
    if(begin.x == end.x && abs(begin.y-end.y) != p) return false;
    return !(begin.y == end.y && abs(begin.x - end.x) != p);
}

bool isOverlapping(CharMatrix2D &matrix, struct Coordinate begin, struct Coordinate end)
{
    if(begin.x == end.x) {
        int from = std::min(begin.y, end.y);
        int to = std::max(begin.y,end.y);
        for(int i = from; i <= to; i++) if(matrix.at(i).at(begin.x) != '-') return true;
    } else {
        int from = std::min(begin.y, end.y);
        int to = std::max(begin.y,end.y);
        for(int i = from; i <= to; i++) if(matrix.at(begin.y).at(i) != '-') return true;
    }

    return false;
}

void markIn(CharMatrix2D &matrix, struct Coordinate begin, struct Coordinate end, char c)
{
    if(begin.x == end.x) {
        int from = std::min(begin.y, end.y);
        int to = std::max(begin.y,end.y);
        for(int i = from; i <= to; i++) matrix.at(i).at(begin.x) = c;
    } else {
        int from = std::min(begin.x, end.x);
        int to = std::max(begin.x,end.x);
        for(int i = from; i <= to; i++) matrix.at(begin.y).at(i) = c;
    }
}

bool isPossibleToFit(const CharMatrix2D& matrix, const int& p)
{
    int duzina1 = 0, duzina2 = 0;
    for(int i = 0; i < matrix.size(); i++) {
        for(int j = 0; j < matrix.size(); j++) {
            if(matrix.at(i).at(j) == '-') duzina1++;
            if(matrix.at(j).at(i) == '-') duzina2++;
        }
        if(duzina1 >= p || duzina2 >= p) return true;
        duzina1 = 0, duzina2 = 0;
    }
    return false;
}

void checkMatrixAndNumOfRobots(std::vector<std::vector<std::vector<char>>>&mat, int p){

    // matrix check
    if(mat.empty()) throw std::domain_error("Matrix doesn't exist!");
    if(mat.size() != 2) throw std::range_error("3D matrix has wrong dnamensions!");
    if(mat.at(0).empty() || mat.at(1).empty()) throw std::domain_error("Matrix cannot be empty!");

    for(int i = 0; i < mat.at(0).size(); i++)
        if(mat.at(0).at(i).size() != mat.at(0).at(0).size()) throw std::domain_error("Matrix cannot be rectangular!");
    for(int i = 0; i < mat.at(1).size(); i++)
        if(mat.at(1).at(i).size() != mat.at(1).at(0).size()) throw std::domain_error("Matrix cannot be rectangular!");

    for(int i = 0; i < mat.at(0).size(); i++)
        if(mat.at(0).at(i).size() != mat.at(0).size()) throw std::domain_error("Matrix must be a square matrix!");
    for(int i = 0; i < mat.at(1).size(); i++)
        if(mat.at(1).at(i).size() != mat.at(1).size()) throw std::domain_error("Matrix must be a square matrix!");

    if(mat.at(0).size() != mat.at(1).size()) throw std::domain_error("Matrixes need to have the same dnamensions!");

    if(p <= 0) throw std::domain_error("Cannot play without any robots!");
    if(p > mat.at(0).at(0).size()) throw std::overflow_error("You want to play with too many robots!");
}

//----------------------------------------------- main functions --------------------------------------------------------//

CharMatrix3D createMatrixes(int n, int m)
{
    if(n <= 0 || m <= 0) throw std::domain_error("Matrix dimensions need to be positive!");
    if(n > 15 || m > 15) throw std::domain_error("Dimensions are too large for matrix!");
    if(n != m) throw std::domain_error("Matrixes need to be square matrixes!");

    CharMatrix2D m1(m, std::vector<char>(n, '-')), m2(m, std::vector<char>(n, '-'));
    return CharMatrix3D {m1,m2};
}

void draw(const CharMatrix3D &mat, int n)
{

    for(const auto& row: mat.at(n)) {
        for(auto element: row) (element == '-' || element == 'X' || element == 'O') ? std::cout<<element<<" " : std::cout<<"R ";
        std::cout<<std::endl;
    }
}

void inputOfMatrix(CharMatrix3D &matrix, int p, const std::string& name, int person)
{
    char c = 'A';

    for(int i = 0; i < p; i++) {
        repeat:
        std::cout<<name<<", please put in the start and ending coordinates of the "<< i + 1 <<". robot: "<<std::endl;
        struct Coordinate begin{}, end{};
        for(;;) {
            std::cin>>begin.x>>begin.y>>end.x>>end.y;
            if(std::cin) break;
            error(), std::cout<<name<<", please put in the start and ending coordinates of the "<< i + 1 <<". robot: "<<std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        if(begin.x == -2 && begin.y == -2 && end.x == -2 && end.y == -2) {
            draw(matrix, person);
            goto repeat;
        } else if(begin.x < 0 || begin.y < 0 || end.x < 0 || end.y < 0) {
            error();
            goto repeat;
        }
        if(!check(matrix.at(person), begin, end, i)) {
            error();
            goto repeat;
        }
        if(isOverlapping(matrix.at(person), begin, end)) {
            error();
            goto repeat;
        }
        markIn(matrix.at(person), begin, end, c);
        if(i + 1 < p && !isPossibleToFit(matrix.at(person), i+2)) throw std::overflow_error("Cannot end the positioning of the robot!");

        (c == 'O') ? c == 'a': c++;

    }
}

void input(CharMatrix3D &matrix,const std::string &name1, const std::string &name2, int p)
{
    checkMatrixAndNumOfRobots(matrix, p);

    inputOfMatrix(matrix, p, name1, 0);
    inputOfMatrix(matrix, p, name2, 1);
}

bool isTheTargetOfRobotCorrect(std::vector<std::vector<std::vector<char>>>&mat, const std::string& name1, const std::string& name2, int p)
{
    checkMatrixAndNumOfRobots(mat, p);
    int person = 0;
    int numberOfWholeN = p, numberOfWholeA = p;
    int numberOfWoundedN = 0, numberOfWoundedA = 0;
    int numberOfDestroyedN = 0, numberOfDestroyedA = 0;
    while(numberOfDestroyedN != p && numberOfDestroyedA != p) {
        repeat:
        (person == 0) ? std::cout<<name1<<", enter the target coordinates of the robot you want to destroy: "<<std::endl :
        std::cout<<name2<<", enter the target coordinates of the robot you want to destroy: "<<std::endl;
        struct Coordinate coordinate{};
        std::cin>>coordinate.x>>coordinate.y;
        if(!std::cin) {
            std::cin.clear(), std::cin.ignore(10000, '\n');
            error(), (person == 0) ? person = 1 : person = 0;
            continue;
        }
        if(coordinate.x == -1 && coordinate.y == -1) {
            std::cout<<name1<<" has "<<numberOfDestroyedN<<" destroyed, "<<numberOfWoundedN<<" damaged and "<<numberOfWholeN<<" whole robots."<<std::endl;
            std::cout<<name2<<" has "<<numberOfDestroyedA<<" destroyed, "<<numberOfWoundedA<<" damaged and "<<numberOfWholeA<<" whole robots."<<std::endl;
            return false;
        }
        if(coordinate.x == -2 && coordinate.y == -2) {
            draw(mat, person);
            goto repeat;
        }
        if(coordinate.x < 0 || coordinate.y < 0 || coordinate.x > mat.at(0).size() - 1 || coordinate.y > mat.at(0).size() - 1) {
            error(), (person == 0) ? person = 1 : person = 0;
            continue;

        }
        if(person == 0) {
            if(mat.at(1).at(coordinate.y).at(coordinate.x) == '-')
                std::cout<<"Miss"<<std::endl, mat.at(1).at(coordinate.y).at(coordinate.x) = 'O';
            else if(mat.at(1).at(coordinate.y).at(coordinate.x) == 'O' || mat.at(1).at(coordinate.y).at(coordinate.x) == 'X')
                std::cout<<"You already targeted this coordinate"<<std::endl;
            else {

                int isThereMore = 0;
                for(const auto& row: mat.at(1))
                    for(auto x: row) if(x == mat.at(1).at(coordinate.y).at(coordinate.x)) isThereMore++;

                if(mat.at(1).at(coordinate.y).at(coordinate.x) == 'A') numberOfDestroyedA++, numberOfWholeA--, std::cout<<"Destroyed"<<std::endl;
                else if(mat.at(1).at(coordinate.y).at(coordinate.x) - 'A' + 1 == isThereMore) numberOfWoundedA++, numberOfWholeA--, std::cout<<"Hit"<<std::endl;
                else if(mat.at(1).at(coordinate.y).at(coordinate.x) - 'A' + 1 > isThereMore && isThereMore != 1) std::cout<<"Hit"<<std::endl;
                else if(isThereMore == 1) numberOfDestroyedA++, numberOfWoundedA--, std::cout<<"Destroyed"<<std::endl;

                mat.at(1).at(coordinate.y).at(coordinate.x) = 'X';
            }

        } else {
            if(mat.at(0).at(coordinate.y).at(coordinate.x) == '-')
                std::cout<<"Mis"<<std::endl, mat.at(0).at(coordinate.y).at(coordinate.x) = 'O';
            else if(mat.at(0).at(coordinate.y).at(coordinate.x) == 'O' || mat.at(0).at(coordinate.y).at(coordinate.x) == 'X')
                std::cout<<"You already targeted this field/coordinate"<<std::endl;
            else {

                int isThereMore = 0;
                for(const auto& row: mat.at(0))
                    for(auto x: row) if(x == mat.at(0).at(coordinate.y).at(coordinate.x)) isThereMore++;

                if(mat.at(0).at(coordinate.y).at(coordinate.x) == 'A') numberOfDestroyedN++, numberOfWholeN--, std::cout<<"Destroyed"<<std::endl;
                else if(mat.at(0).at(coordinate.y).at(coordinate.x) - 'A' + 1 == isThereMore) std::cout<<"Hit"<<std::endl, numberOfWoundedN++, numberOfWholeN--;
                else if(mat.at(0).at(coordinate.y).at(coordinate.x) - 'A' + 1 > isThereMore && isThereMore != 1) std::cout<<"Hit"<<std::endl;
                else if(isThereMore == 1) numberOfDestroyedN++, numberOfWoundedN--, std::cout<<"Destroyed"<<std::endl;

                mat.at(0).at(coordinate.y).at(coordinate.x) = 'X';
            }
        }

        if(numberOfDestroyedA == p)
            std::cout<<name1<<" has won. "<<name1<<" has "<<numberOfDestroyedN<<" destroyed, "<<numberOfWoundedN<<" damaged and "<<numberOfWholeN<<" whole robots.";
        else if(numberOfDestroyedN == p)
            std::cout<<name2<<" has won. "<<name2<<" has "<<numberOfDestroyedA<<" destroyed, "<<numberOfWoundedA<<" damaged and "<<numberOfWholeA<<" whole robots.";
        (person == 0) ? person = 1 : person = 0;
    }
    return true;
}

//---------------------------------------------- main -----------------------------------------------//

int main ()
{
    dimensions:
    int n, m, numberOfRobots;
    std::string name1, name2;
    std::cout<<"Welcome! Before we go any further, please enter your names: "<<std::endl;
    std::cout<<"Player 1: ";
    std::getline(std::cin, name1);
    std::cout<<"Player 2: ";
    std::getline(std::cin, name2);
    CharMatrix3D triD;
    try {
        std::cout<<"The game started. Input the matrix dimensions (two numbers):"<<std::endl;
        std::cin>>n>>m;
        triD = createMatrixes(n,m);
    } catch(const std::exception &e) {
        std::cout<<e.what()<<std::endl;
        goto dimensions;
    }
    robot:
    try {
        std::cout<<"Enter the number of robots:"<<std::endl;
        std::cin>>numberOfRobots;
        unos:
        try {
            triD = createMatrixes(n,m);
            input(triD, name1, name2, numberOfRobots);
        } catch (std::overflow_error& error) {
            std::cout<<error.what()<<std::endl;
            if(error.what() == std::string("Cannot end the positioning of the robot!") ) goto unos;
            else goto robot;

        } catch (const std::exception &e) {
            std::cout<<e.what()<<std::endl;
            goto robot;
        }
    } catch (const std::exception &e) {
        std::cout<<e.what()<<std::endl;
        goto robot;
    }
    std::cout<<"You have successfully entered the position of robots. The game can now begin."<<std::endl;
    isTheTargetOfRobotCorrect(triD, name1, name2, numberOfRobots);

    return 0;
}