/*
   FFXIV Cactpot Perfect Solver v2.0
   Algorithm Provided by: /u/Aureolux
   UI and Allowing users to define their states in the program by: /u/messem10

   Release Date: March 17, 2015
   Patch Notes:
      2.0 - /u/messem10's release
      1.0 - /u/Aureolux's initial release
*/

#include <iostream>
#include <algorithm>
#include <string>
#define EPS 0.00001

using namespace std;

double solve_1 (int *state, int *rewards, bool *flip);
double solve_2 (int *state, int *rewards, bool *flip);
double solve_3 (int *state, int *rewards, bool *flip);
double solve_4 (int *state, int *rewards, bool *flip);

int getPosInt(string pos);
string getPosString(int i);
string getRowString(int i);
int solver(int num_revealed, int *state);
void callSolver(int *state, int num_revealed);
bool isNumInGrid(int *state, int val);

int main()
{
    int state[9] = {}; //Zeros out the array.

    cout << "Grid Definition" << endl;
    cout << "UL UC UR" << endl;
    cout << "ML MC MR" << endl;
    cout << "BL BC BR" << endl << endl;

    cout << "Initial State Input - One Revealed" << endl;
    callSolver(state, 1); //Calls callSolver with state and num_revealed = 1.

    cout << "Second State Input - Two Revealed" << endl;
    callSolver(state, 2); //Calls callSolver with state and num_revealed = 2.

    cout << "Third State Input - Three revealed" << endl;
    callSolver(state, 3); //Calls callSolver with state and num_revealed = 3.

    cout << "Final State Input - Four revealed" << endl;
    callSolver(state, 4); //Calls callSolver with state and num_revealed = 4.

    std::cout << std::endl << "Program finished. Press enter to close.";
    std::cin.get();
    cin.get();
    return 0;
}

int getPosInt(string pos) //Converts the letter position into the corresponding integer.
{
   std::transform(pos.begin(), pos.end(), pos.begin(), ::tolower); //Makes the string lowercase.

   if(pos == "ul" || pos == "tl") return 0;
   if(pos == "uc" || pos == "tc") return 1;
   if(pos == "ur" || pos == "tr") return 2;
   if(pos == "cl" || pos == "ml") return 3; //Some may specify this as CL (Center Left)
   if(pos == "cc" || pos == "c" || pos == "mc") return 4; //Multiple possible letter combinations for center
   if(pos == "cr" || pos == "mr") return 5; //Some may Specify this as CR (Center Right)
   if(pos == "bl" || pos == "ll") return 6;
   if(pos == "bc" || pos == "lc") return 7;
   if(pos == "br" || pos == "lr") return 8;

   return -1; //No position found.
}

string getPosString(int i) //Converts the integer into the corresponding tile. Makes it easier to read.
{
   switch(i)
   {
      case 0: return "Upper Left"; break;
      case 1: return "Upper Center"; break;
      case 2: return "Upper Right"; break;
      case 3: return "Middle Left"; break;
      case 4: return "Middle Center"; break;
      case 5: return "Middle Right"; break;
      case 6: return "Bottom Left"; break;
      case 7: return "Bottom Center"; break;
      case 8: return "Bottom Right"; break;
   }
}

string getRowString(int i) //Converts the integer into the corresponding row. Makes it easier to read.
{
   switch(i)
   {
      case 0: return "Top Row"; break;
      case 1: return "Middle Row"; break;
      case 2: return "Bottom Row"; break;
      case 3: return "Left Column"; break;
      case 4: return "Middle Column"; break;
      case 5: return "Right Column"; break;
      case 6: return "Diagonal (Upper Left to Bottom Right)"; break;
      case 7: return "Diagonal (Upper Right to Bottom Left)"; break;
   }
}

//Takes state and num_revealed, sets up the solver. Allows more modularity with the main() function.
void callSolver(int *state, int num_revealed)
{
    int index, value;
    string pos;
    cout << "Pos Value" << endl;
    cin >> pos >> value;
    index = getPosInt(pos);
    if(index == -1)
    {
       cerr << "No index found. Please Check your input and try again. Exiting" << endl;
       cin.get();
       cin.get();
       exit(1);
    }

    if(state[index] != 0)
    {
       cerr << "ERROR: Tile already defined. Please check your input and try again. Exiting" << endl;
       cin.get();
       cin.get();
       exit(2);
    }

    if(isNumInGrid(state, value))
    {
       cerr << "ERROR: Number already in grid. Please check your inputs and try again. Exiting." << endl;
       cin.get();
       cin.get();
       exit(3);
    }


    state[index] = value;
    solver(num_revealed, state);

    cout << endl;
}

bool isNumInGrid(int *state, int val)
{
   for(int i=0; i<9; i++)
   {
      if(val == state[i])
         return true;
   }

   return false;
}

int solver(int num_revealed, int *state) //Sets up and calls /u/Aureolux's solver.
{
    // List of rewards for each sum
    int rewards[25] = {0, 0, 0, 0, 0, 0, 10000, 36, 720, 360, 80, 252, 108, 72, 54, 180, 72, 180, 119, 36, 306, 1080, 144, 1800, 3600};
    // If four are visible, we are picking between eight rows. Otherwise, we are picking
    // between nine tiles (although we'll never be picking revealed tiles)
    int num_options = 9;
    if (num_revealed == 4)
        num_options = 8;

    // Run the appropriate function to solve for the optimal choice
    bool *which_to_flip = new bool[num_options];
    for (int i = 0; i < num_options; i++)
        which_to_flip[i] = false;
    std::cout << "Expected winnings with optimal play: ";
    switch (num_revealed)
    {
    case 1:
        std::cout << solve_1 (state, rewards, which_to_flip);
        break;
    case 2:
        std::cout << solve_2 (state, rewards, which_to_flip);
        break;
    case 3:
        std::cout << solve_3 (state, rewards, which_to_flip);
        break;
    case 4:
        std::cout << solve_4 (state, rewards, which_to_flip);
        break;
    }
    std::cout << " MGP." << std::endl << std::endl;

    // Output which choices are optimal
    string finalOutput = "";
    if (num_revealed < 4)
    {
       std::cout << "Tiles to flip: ";
       for (int i = 0; i < num_options; i++)
       {
          if (which_to_flip[i])
          {
             string output = getPosString(i);
             finalOutput += output + ", ";
          }

       }
       cout << finalOutput.substr(0, finalOutput.size()-2) << endl;
    }else{
       std::cout << "Rows to choose: ";
       for (int i = 0; i < num_options; i++)
       {
          if (which_to_flip[i])
          {
             string output = getRowString(i);
             finalOutput += output + " or ";
          }
       }
      cout << finalOutput.substr(0, finalOutput.size()-4) << endl;
    }
}


/* Solve for the best row choice when 3 tiles have been flipped.
Returns the expected value of flipping that tile.

state is an array of the current board state, e.g., 001050020
0 means the tile is not flipped. 2 means the tile is showing a 2.

rewards is an array of rewards for each sum.
For example, rewards[6] = 10000;

flip is an array of which tiles are the best choice to maximize expected value.
It is passed into the function as an array of FALSE.
The function changes the best choices to TRUE. Indices are as follows:
0 = top left
1 = top center
2 = top right
3 = middle left
4 = direct center
5 = middle right
6 = bottom left
7 = bottom middle
8 = bottom right
For example, if the function changes flip[5] to TRUE, that means the middle right tile
 is (potentially tied for) the best choice for maximizing expected value. */
double solve_4 (int *state, int *rewards, bool *row)
{
    int unknowns[5] = {};
    int ids[5] = {};
    int has[10] = {};
    int tot_win[8] = {};
    int cur = 0;
    for (int i = 0; i < 9; i++)
    {
        if (state[i] == 0)
        {
            // Storing the ids of all locations which are currently unrevealed
            ids[cur] = i;
            cur++;
        }
        else
        {
            // Checking which numbers are currently visible
            has[state[i]] = 1;
        }
    }
    cur = 0;
    // From the previous step, we know which numbers are not yet visible:
    //  these are the possible unknowns
    for (int i = 1; i < 10; i++)
    {
        if (has[i] == 0)
        {
            unknowns[cur] = i;
            cur++;
        }
    }
    // Loop over all possible permutations on the unknowns
    do
    {
        for (int i = 0; i < 5; i++)
            state[ids[i]] = unknowns[i];
        // For each row, cumulatively sum the winnings for picking that row
        tot_win[0] += rewards[state[0] + state[1] + state[2]];
        tot_win[1] += rewards[state[3] + state[4] + state[5]];
        tot_win[2] += rewards[state[6] + state[7] + state[8]];
        tot_win[3] += rewards[state[0] + state[3] + state[6]];
        tot_win[4] += rewards[state[1] + state[4] + state[7]];
        tot_win[5] += rewards[state[2] + state[5] + state[8]];
        tot_win[6] += rewards[state[0] + state[4] + state[8]];
        tot_win[7] += rewards[state[2] + state[4] + state[6]];
    } while (std::next_permutation(unknowns, unknowns + 5));
    // Find the maximum. Start by assuming row 0 is best.
    int curmax = tot_win[0];
    row[0] = true;
    for (int i = 1; i < 8; i++)
    {
        // If another row yielded a higher expected value:
        if (tot_win[i] > curmax)
        {
            // Mark all the previous rows as FALSE (not optimal) and the current one as TRUE
            curmax = tot_win[i];
            for (int j = 0; j < i; j++)
                row[j] = false;
            row[i] = true;
        }
        else if(tot_win[i] == curmax)
        {
            // For a tie, mark the current one as TRUE, and leave the previous ones intact
            row[i] = true;
        }
    }
    // The current totals are for 120 possible configurations.
    // Divide by 120 to get the actual expected value.
    return double(curmax) / 120.;
}

double solve_3 (int *state, int *rewards, bool *flip)
{
    bool dummy_array[8] = {};
    int unknowns[6] = {};
    int ids[6] = {};
    int has[10] = {};
    double tot_win[6] = {};
    int cur = 0;
    for (int i = 0; i < 9; i++)
    {
        if (state[i] == 0)
        {
            ids[cur] = i;
            cur++;
        }
        else
        {
            has[state[i]] = 1;
        }
    }
    cur = 0;
    for (int i = 1; i < 10; i++)
    {
        if (has[i] == 0)
        {
            unknowns[cur] = i;
            cur++;
        }
    }
    // This is the only part that really differs from the previous function.
    // Loop over every unknown tile and every possible value that could appear.
    // Solve the resulting cases with solve_4.
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            state[ids[i]] = unknowns[j];
            tot_win[i] += solve_4(state, rewards, dummy_array);
            for (int k = 0; k < 6; k++)
                state[ids[k]] = 0;
        }
    }
    double curmax = tot_win[0];
    flip[ids[0]] = false;
    for (int i = 1; i < 6; i++)
    {
        if (tot_win[i] > curmax + EPS)
        {
            curmax = tot_win[i];
            for (int j = 0; j < i; j++)
                flip[ids[j]] = false;
            flip[ids[i]] = true;
        }
        else if(tot_win[i] > curmax - EPS)
        {
            flip[ids[i]] = true;
        }
    }
    // Each tile can be flipped to reveal one of 6 values.
    // Divide by 6 to get the true expected value.
    return double(curmax) / 6.;
}

// Same as above, replace 3 with 2
double solve_2 (int *state, int *rewards, bool *flip)
{
    bool dummy_array[9] = {};
    int unknowns[7] = {};
    int ids[7] = {};
    int has[10] = {};
    double tot_win[7] = {};
    int cur = 0;
    for (int i = 0; i < 9; i++)
    {
        if (state[i] == 0)
        {
            ids[cur] = i;
            cur++;
        }
        else
        {
            has[state[i]] = 1;
        }
    }
    cur = 0;
    for (int i = 1; i < 10; i++)
    {
        if (has[i] == 0)
        {
            unknowns[cur] = i;
            cur++;
        }
    }
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            state[ids[i]] = unknowns[j];
            tot_win[i] += solve_3(state, rewards, dummy_array);
            for (int k = 0; k < 7; k++)
                state[ids[k]] = 0;
        }
    }
    double curmax = tot_win[0];
    flip[ids[0]] = true;
    for (int i = 1; i < 7; i++)
    {
        if (tot_win[i] > curmax + EPS)
        {
            curmax = tot_win[i];
            for (int j = 0; j < i; j++)
                flip[ids[j]] = false;
            flip[ids[i]] = true;
        }
        else if(tot_win[i] > curmax - EPS)
        {
            flip[ids[i]] = true;
        }
    }
    return double(curmax) / 7.;
}

// Same as above, replace 2 with 1
double solve_1 (int *state, int *rewards, bool *flip)
{
    bool dummy_array[9] = {};
    int unknowns[8] = {};
    int ids[8] = {};
    int has[10] = {};
    double tot_win[8] = {};
    int cur = 0;
    for (int i = 0; i < 9; i++)
    {
        if (state[i] == 0)
        {
            ids[cur] = i;
            cur++;
        }
        else
        {
            has[state[i]] = 1;
        }
    }
    cur = 0;
    for (int i = 1; i < 10; i++)
    {
        if (has[i] == 0)
        {
            unknowns[cur] = i;
            cur++;
        }
    }
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            state[ids[i]] = unknowns[j];
            tot_win[i] += solve_2(state, rewards, dummy_array);
            for (int k = 0; k < 8; k++)
                state[ids[k]] = 0;
        }
    }
    double curmax = tot_win[0];
    flip[ids[0]] = true;
    for (int i = 1; i < 8; i++)
    {
        if (tot_win[i] > curmax + EPS)
        {
            curmax = tot_win[i];
            for (int j = 0; j < i; j++)
                flip[ids[j]] = false;
            flip[ids[i]] = true;
        }
        else if(tot_win[i] > curmax - EPS)
        {
            flip[ids[i]] = true;
        }
    }
    return double(curmax) / 8.;
}
