#include <iostream> // cout
#include <fstream>  // fopen, fscanf, fclose

using namespace std;

// Each element of grid is this type
struct Grid_Elements {
    char op_type;
    int center_row;
    int center_col;
    int scale;
};

// Needed in moving shapes to its new location
struct Location {
    int center_row;
    int center_col;
};

// Function declarations
Grid_Elements** create_grid (int rows, int cols);
void delete_grid(Grid_Elements** grid, int rows);
void init_with_null (Grid_Elements** grid, int rows, int cols);
void print_grid (Grid_Elements** grid, int rows, int cols);
void create_shape (Grid_Elements** grid, char op_type, int center_row, int center_col, int scale);
void delete_shape (Grid_Elements** grid, Grid_Elements target_element);
bool check_border_error (int grid_rows, int grid_cols, char op_type, int center_row, int center_col, int scale, Grid_Elements target_element, Location new_location, char task);
bool check_conflict_error (Grid_Elements** grid, int grid_rows, int grid_cols, char op_type, int center_row, int center_col, int scale, Grid_Elements target_element, Location new_location, char task);
void print_placement_success (char op_type, int center_row, int center_col, int scale);
void print_move_success (char op_type, int center_row, int center_col, Location new_location);
void print_placement_error (char error_type, char op_type, int center_row, int center_col, int scale);
void print_move_error (char error_type, char op_type, int center_row, int center_col, Location new_location);
void copy_element (Grid_Elements** grid, Grid_Elements &target_element, int target_row, int target_col);
void find_new_location ( Location &new_location, char direction, int center_row, int center_col, int move_by);


/**
 *
 * @param argc number of arguements passed to the function
 * @param argv grid file and instructions file
 *
 */
int main(int argc, char* argv[]) {
    int grid_rows;
    int grid_cols;

    // File pointers are declared
    FILE *grid_file_ptr; // grid file pointer
    FILE *inst_file_ptr; // instructions file pointer

    // Operations with grid file starts here

    // If it cannot open grid file, it returns 1
    grid_file_ptr = fopen(argv[1], "r"); // opens the grid file in reading mode
    if(!grid_file_ptr) {
        return 1;
    }

    // Scans the grid's rows  and columns to create dynamic grid
    fscanf(grid_file_ptr, "%d %d ", &grid_rows, &grid_cols);

    cout << "A grid is created: " << grid_rows << " " << grid_cols << endl;

    char op_type = 0;    // type of operator (+,-,x or /)
    int center_row = 0;  // row of the center of the shape
    int center_col = 0;  // column of the center of the shape
    int scale = 0;       // scale/size of the shape
    char task = 'P';     // current task ('P' for placement)

    // initialization of new location of the shape to be moved
    struct Location new_location;
    new_location.center_row = 0;
    new_location.center_col = 0;

    // initialization of the properties of the element to be moved
    struct Grid_Elements target_element;
    target_element.op_type = 0;
    target_element.center_row = 0;
    target_element.center_col = 0;
    target_element.scale = 0;


    //create grid and initialize elements with NULL character
    Grid_Elements** grid = create_grid(grid_rows, grid_cols);
    init_with_null(grid, grid_rows, grid_cols);


    // scan each line in the grid file until the end of file
    while(!feof(grid_file_ptr)) {
        fscanf(grid_file_ptr, "%c %d %d %d ", &op_type, &center_row, &center_col, &scale); // scan

        // check if there is a border error or conflict error
        bool border_error = check_border_error(grid_rows, grid_cols, op_type, center_row, center_col, scale, target_element, new_location, task);
        bool conflict_error = check_conflict_error(grid, grid_rows, grid_cols, op_type, center_row, center_col, scale, target_element, new_location, task);


        if (!border_error && !conflict_error) {     // if there is no error
            create_shape(grid, op_type, center_row, center_col, scale);         // create shape
            print_placement_success(op_type, center_row, center_col, scale);    // print success message
        }
    } // end of grid file reading

    fclose(grid_file_ptr);  // close the grid file
    // Operations with grid file ends here

    // Operations with instructions file starts here


    char direction;     // direction of move
    int target_row;     // row of the element of the shape to be moved
    int target_col;     // column of the element of the shape to be moved
    int move_by;        // magnitude of move

    task = 'M';         // shows the current task ('M' for movement)


    inst_file_ptr = fopen(argv[2], "r");  // opens the instructions file in reading mode

    // If it cannot open grid file, it returns 1
    if (!inst_file_ptr) {
        return 1;
    }

    // scan each line in the grid file until the end of file
    while (!feof(inst_file_ptr)) {
        fscanf(inst_file_ptr, "%*c%*c%c %d %d %d ", &direction, &target_row, &target_col, &move_by);   // scan

        copy_element(grid, target_element, target_row, target_col); // copy the properties of the element to the target element

        find_new_location(new_location, direction, target_element.center_row, target_element.center_col, move_by); // finds out the new location of the shape to be moved

        // check if there is a border error or conflict error
        bool border_error = check_border_error(grid_rows, grid_cols, target_element.op_type, new_location.center_row, new_location.center_col, target_element.scale, target_element, new_location, task);
        bool conflict_error = check_conflict_error(grid, grid_rows, grid_cols, target_element.op_type, new_location.center_row, new_location.center_col, target_element.scale, target_element, new_location, task);

        if (!border_error && !conflict_error) {     // if there is no error
            delete_shape(grid, target_element);     // delete the shape to be moved
            create_shape(grid, target_element.op_type, new_location.center_row, new_location.center_col, target_element.scale); // create shape in its new location
            print_move_success(target_element.op_type, target_element.center_row, target_element.center_col, new_location);     // print success message
        }
    } // end of reading the instructions file

    fclose(inst_file_ptr); // close the instructions file

    // Operations with instructions file ends here

    delete_grid(grid, grid_rows);   // delete the allocated memory for grid

    return 0;       // returns with 0
} // end of main


/*
 * Creates a 2D struct array of pointers
 *
 * @param rows number of rows in the grid
 * @param cols number of columns in the grid
 * @return grid_ptr 2D struct array of pointers
 */
Grid_Elements **create_grid(int rows, int cols) {

    Grid_Elements **grid_ptr;
    grid_ptr = new Grid_Elements*[rows];
    for (int i = 0; i < rows; i++) {
        grid_ptr[i] = new Grid_Elements[cols];
    }
    return grid_ptr;
}

/*
 * Deletes the memory allocated by 2D struct array of pointers
 *
 * @param grid grid to be deleted
 * @param rows number of rows in the grid
 *
 */
void delete_grid(Grid_Elements** grid, int rows) {
    for (int i = 0;  i < rows; i++) {
        delete[] grid[i];
    }
    delete[] grid;
}

/*
 * Initializes all the properties of each element with 0
 *
 * @param grid grid to be worked on
 * @param rows number of rows in the grid
 * @param cols number of columns in the grid
 */
void init_with_null(Grid_Elements** grid, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grid[i][j].op_type = 0;
            grid[i][j].center_row = 0;
            grid[i][j].center_col = 0;
            grid[i][j].scale = 0;
        }
    }
}

/*
 * Creates the shape in the grid
 *
 * @param grid grid to be worked on
 * @param op_type operator type
 * @param center_row row of the center of the shape
 * @param center_col column of the center of the shape
 * @param scale scale of the shape
 */
void create_shape (Grid_Elements** grid, char op_type, int center_row, int center_col, int scale) {
    int i;
    switch (op_type) {
        case '+':
            for (i = 0; i <= scale * 2; i++) {
                grid[center_row - 1][((center_col - 1) + (scale - i))].op_type = op_type;
                grid[center_row - 1][((center_col - 1) + (scale - i))].center_row = center_row;
                grid[center_row - 1][((center_col - 1) + (scale - i))].center_col = center_col;
                grid[center_row - 1][((center_col - 1) + (scale - i))].scale = scale;
            }
            for (i = 0; i <= scale * 2; i++) {
                grid[(center_row - 1) + (scale - i)][center_col - 1].op_type = op_type;
                grid[(center_row - 1) + (scale - i)][center_col - 1].center_row = center_row;
                grid[(center_row - 1) + (scale - i)][center_col - 1].center_col = center_col;
                grid[(center_row - 1) + (scale - i)][center_col - 1].scale = scale;
            }
            break;
        case '-':
            for (i = 0; i <= scale * 2; i++) {
                grid[center_row - 1][((center_col - 1) + (scale - i))].op_type = op_type;
                grid[center_row - 1][((center_col - 1) + (scale - i))].center_row = center_row;
                grid[center_row - 1][((center_col - 1) + (scale - i))].center_col = center_col;
                grid[center_row - 1][((center_col - 1) + (scale - i))].scale = scale;
            }
            break;
        case 'x':
            for (i = 0; i <= scale * 2; i++) {
                grid[((center_row - 1) + (scale - i))][((center_col - 1) + (scale - i))].op_type = op_type;
                grid[((center_row - 1) + (scale - i))][((center_col - 1) + (scale - i))].center_row = center_row;
                grid[((center_row - 1) + (scale - i))][((center_col - 1) + (scale - i))].center_col = center_col;
                grid[((center_row - 1) + (scale - i))][((center_col - 1) + (scale - i))].scale = scale;
            }
            for (i = 0; i <= scale * 2; i++) {
                grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].op_type = op_type;
                grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].center_row = center_row;
                grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].center_col = center_col;
                grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].scale = scale;
            }
            break;
        case '/':
            for (i = 0; i <= scale * 2; i++) {
                grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].op_type = op_type;
                grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].center_row = center_row;
                grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].center_col = center_col;
                grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].scale = scale;
            }
            break;
    }
}

/*
 * Deletes the shape
 *
 * @param grid grid to be worked on
 * @param target_element one of the elements of the shape
 */
void delete_shape (Grid_Elements** grid, Grid_Elements target_element) {
    int i;
    char op_type = target_element.op_type;
    int center_row = target_element.center_row;
    int center_col = target_element.center_col;
    int scale = target_element.scale;

    switch (op_type) {
        case '+':
            for (i = 0; i <= scale * 2; i++) {
                grid[center_row - 1][((center_col - 1) + (scale - i))].op_type = 0;
                grid[center_row - 1][((center_col - 1) + (scale - i))].center_row = 0;
                grid[center_row - 1][((center_col - 1) + (scale - i))].center_col = 0;
                grid[center_row - 1][((center_col - 1) + (scale - i))].scale = 0;
            }
            for (i = 0; i <= scale * 2; i++) {
                grid[(center_row - 1) + (scale - i)][center_col - 1].op_type = 0;
                grid[(center_row - 1) + (scale - i)][center_col - 1].center_row = 0;
                grid[(center_row - 1) + (scale - i)][center_col - 1].center_col = 0;
                grid[(center_row - 1) + (scale - i)][center_col - 1].scale = 0;
            }
            break;
        case '-':
            for (i = 0; i <= scale * 2; i++) {
                grid[center_row - 1][((center_col - 1) + (scale - i))].op_type = 0;
                grid[center_row - 1][((center_col - 1) + (scale - i))].center_row = 0;
                grid[center_row - 1][((center_col - 1) + (scale - i))].center_col = 0;
                grid[center_row - 1][((center_col - 1) + (scale - i))].scale = 0;
            }
            break;
        case 'x':
            for (i = 0; i <= scale * 2; i++) {
                grid[((center_row - 1) + (scale - i))][((center_col - 1) + (scale - i))].op_type = 0;
                grid[((center_row - 1) + (scale - i))][((center_col - 1) + (scale - i))].center_row = 0;
                grid[((center_row - 1) + (scale - i))][((center_col - 1) + (scale - i))].center_col = 0;
                grid[((center_row - 1) + (scale - i))][((center_col - 1) + (scale - i))].scale = 0;
            }
            for (i = 0; i <= scale * 2; i++) {
                grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].op_type = 0;
                grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].center_row = 0;
                grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].center_col = 0;
                grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].scale = 0;
            }
            break;
        case '/':
            for (i = 0; i <= scale * 2; i++) {
                grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].op_type = 0;
                grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].center_row = 0;
                grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].center_col = 0;
                grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].scale = 0;
            }
            break;
    }
}

/*
 * Checks if there is a border error
 * @param grid_rows row number of grid
 * @param grid col column number of grid
 * @param op_type operator type
 * @param center_row row of the center of the shape
 * @param center_col column of the center of the shape
 * @param scale scale of the shape
 * @param target_element one of the elements of the shape
 * @param new_location new location of the shape
 * @return bool true or false
 */
bool check_border_error (int grid_rows, int grid_cols, char op_type, int center_row, int center_col, int scale, Grid_Elements target_element, Location new_location, char task) {
    switch (op_type) {
        case '+':
            if (((center_row - scale) < 1) || ((center_row + scale) > grid_rows) || ((center_col - scale) < 1) ||
                ((center_col + scale) > grid_cols)) {
                task == 'M' ? print_move_error ('B', op_type, target_element.center_row, target_element.center_col, new_location) : print_placement_error('B', op_type, center_row, center_col, scale);
                return true;
            }
            break;
        case '-':
            if ((center_col - scale < 1) || (center_col + scale > grid_cols)) {
                task == 'M' ? print_move_error ('B', op_type, target_element.center_row, target_element.center_col, new_location) : print_placement_error('B', op_type, center_row, center_col, scale);
                return true;
            }
            break;
        case 'x':
            if (((center_row - scale) < 1) || ((center_row + scale) > grid_rows) || ((center_col - scale) < 1) ||
                ((center_col + scale) > grid_cols)) {
                task == 'M' ? print_move_error ('B', op_type, target_element.center_row, target_element.center_col, new_location) : print_placement_error('B', op_type, center_row, center_col, scale);
                return true;
            }
            break;
        case '/':
            if (((center_row - scale) < 1) || ((center_row + scale) > grid_rows) || ((center_col - scale) < 1) ||
                ((center_col + scale) > grid_cols)) {
                task == 'M' ? print_move_error ('B', op_type, target_element.center_row, target_element.center_col, new_location) : print_placement_error('B', op_type, center_row, center_col, scale);
                return true;
            }
            break;
    }
    return false;
}

/*
 * Checks if there is a conflict error
 * @param grid grid to be worked on
 * @param grid_rows row number of grid
 * @param grid col column number of grid
 * @param op_type operator type
 * @param center_row row of the center of the shape
 * @param center_col column of the center of the shape
 * @param scale scale of the shape
 * @param target_element one of the elements of the shape
 * @param new_location new location of the shape
 * @param task current task
 * @return bool true or false
 */
bool check_conflict_error (Grid_Elements** grid, int grid_rows, int grid_cols, char op_type, int center_row, int center_col, int scale, Grid_Elements target_element, Location new_location, char task) {
    int i;
    switch (op_type) {
        case '+':
            for (i = 0; i <= scale * 2; i++) {
                if ((((center_col) + (scale - i)) < 1) || (((center_col) + (scale - i)) > grid_cols)) {
                    continue;
                }
                if (grid[center_row - 1][((center_col - 1) + (scale - i))].op_type != 0) {
                    if (task == 'M') {
                        if ((grid[center_row - 1][((center_col - 1) + (scale - i))].center_row == target_element.center_row) &&
                            (grid[center_row - 1][((center_col - 1) + (scale - i))].center_col == target_element.center_col)) {
                            continue;
                        } else {
                            print_move_error('C', op_type, target_element.center_row, target_element.center_col, new_location);
                            return true;
                        }
                    } else {
                        print_placement_error('C', op_type, center_row, center_col, scale);
                        return true;
                    }

                }
            }
            for (i = 0; i <= scale * 2; i++) {
                if (((center_row) + (scale - i) < 1) || ((center_row) + (scale - i) > grid_rows)) {
                    continue;
                }
                if (grid[(center_row - 1) + (scale - i)][center_col - 1].op_type != 0) {
                    if (task == 'M') {
                        if ((grid[(center_row - 1) + (scale - i)][center_col - 1].center_row == target_element.center_row) &&
                            (grid[(center_row - 1) + (scale - i)][center_col - 1].center_col == target_element.center_col)) {
                            continue;
                        } else {
                            print_move_error('C', op_type, target_element.center_row, target_element.center_col, new_location);
                            return true;
                        }
                    } else {
                        print_placement_error('C', op_type, center_row, center_col, scale);
                        return true;
                    }
                }
            }
            break;
        case '-':
            for (i = 0; i <= scale * 2; i++) {
                if ((((center_col) + (scale - i)) < 1) || (((center_col) + (scale - i)) > grid_rows)) {
                    continue;
                }
                if (grid[center_row - 1][((center_col - 1) + (scale - i))].op_type != 0) {
                    if (task == 'M') {
                        if ((grid[center_row - 1][((center_col - 1) + (scale - i))].center_row == target_element.center_row) &&
                            (grid[center_row - 1][((center_col - 1) + (scale - i))].center_col == target_element.center_col)) {
                            continue;
                        } else {
                            print_move_error('C', op_type, target_element.center_row, target_element.center_col, new_location);
                            return true;
                        }
                    } else {
                        print_placement_error('C', op_type, center_row, center_col, scale);
                        return true;
                    }
                }
            }
            break;
        case 'x':
            for (i = 0; i <= scale * 2; i++) {
                if ((((center_row) + (scale - i)) < 1) || (((center_row) + (scale - i)) > grid_rows) || (((center_col) + (scale - i)) < 1) || (((center_col) + (scale - i)) > grid_cols)) {
                    continue;
                }
                if (grid[((center_row - 1) + (scale - i))][((center_col - 1) + (scale - i))].op_type != 0) {
                    if (task == 'M') {
                        if ((grid[((center_row - 1) + (scale - i))][((center_col - 1) + (scale - i))].center_row == target_element.center_row) &&
                            (grid[((center_row - 1) + (scale - i))][((center_col - 1) + (scale - i))].center_col == target_element.center_col)) {
                            continue;
                        } else {
                            print_move_error('C', op_type, target_element.center_row, target_element.center_col, new_location);
                            return true;
                        }
                    } else {
                        print_placement_error('C', op_type, center_row, center_col, scale);
                        return true;
                    }
                }
            }
            for (i = 0; i <= scale * 2; i++) {
                if ((((center_row) - (scale - i)) < 1) || (((center_row) - (scale - i)) > grid_rows) || (((center_col) + (scale - i)) < 1) || (((center_col) + (scale - i)) > grid_cols)) {
                    continue;
                }
                if (grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].op_type != 0) {
                    if (task == 'M') {
                        if ((grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].center_row == target_element.center_row) &&
                            (grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].center_col == target_element.center_col)) {
                            continue;
                        } else {
                            print_move_error('C', op_type, target_element.center_row, target_element.center_col, new_location);
                            return true;
                        }
                    } else {
                        print_placement_error('C', op_type, center_row, center_col, scale);
                        return true;
                    }
                }
            }
            break;
        case '/':
            for (i = 0; i <= scale * 2; i++) {
                if ((((center_row) - (scale - i)) < 1) || ((center_row) - (scale - i) > grid_rows) || ((center_col) + (scale - i)) || (((center_col) + (scale - i)) > grid_cols)) {
                    continue;
                }
                if (grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].op_type != 0) {
                    if (task == 'M') {
                        if ((grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].center_row == target_element.center_row) &&
                            (grid[((center_row - 1) - (scale - i))][((center_col - 1) + (scale - i))].center_col == target_element.center_col)) {
                            continue;
                        } else {
                            print_move_error('C', op_type, target_element.center_row, target_element.center_col, new_location);
                            return true;
                        }
                    } else {
                        print_placement_error('C', op_type, center_row, center_col, scale);
                        return true;
                    }
                }
            }
            break;
    }
    return false;
}

/*
 * Prints success message if the placement is successful
 *
 * @param op_type operator type
 * @param center_row row of the center of the shape
 * @param center_col column of the center of the shape
 * @param scale scale of the shape
 */
void print_placement_success (char op_type, int center_row, int center_col, int scale) {
    cout << "SUCCESS: Operator " << op_type << " with size " << scale << " is placed on (" <<
         center_row << "," << center_col << ")." << endl;
}

/*
 * Prints success message if the movement is successful
 *
 * @param op_type operator type
 * @param center_row row of the center of the shape
 * @param center_col column of the center of the shape
 * @param new_location new location of the shape
 */
void print_move_success (char op_type, int center_row, int center_col, Location new_location) {
    cout << "SUCCESS: " << op_type << " moved from (" << center_row << "," << center_col << ") to (" <<
         new_location.center_row << "," << new_location.center_col << ")." << endl;
}

void print_placement_error (char error_type, char op_type, int center_row, int center_col, int scale) {
    switch (error_type) {
        case 'B':
            cout << "BORDER ERROR: Operator " << op_type << " with size " << scale << " can not be placed on (" <<
                 center_row << "," << center_col << ")." << endl;
            break;
        case 'C':
            cout << "CONFLICT ERROR: Operator " << op_type << " with size " << scale << " can not be placed on (" <<
                 center_row << "," << center_col << ")." << endl;
            break;
    }
}

void print_move_error (char error_type, char op_type, int center_row, int center_col, Location new_location) {
    switch (error_type) {
        case 'B':
            cout << "BORDER ERROR: " << op_type << " can not be moved from (" << center_row << "," << center_col << ") to (" <<
            new_location.center_row << "," << new_location.center_col << ")." << endl;
            break;
        case 'C':
            cout << "CONFLICT ERROR: " << op_type << " can not be moved from (" << center_row << "," << center_col << ") to (" <<
                 new_location.center_row << "," << new_location.center_col << ")." << endl;
            break;
    }
}

void copy_element (Grid_Elements** grid, Grid_Elements &target_element, int target_row, int target_col) {
    target_element.op_type = grid[(target_row - 1)][(target_col - 1)].op_type;
    target_element.center_row = grid[(target_row - 1)][(target_col - 1)].center_row;
    target_element.center_col = grid[(target_row - 1)][(target_col - 1)].center_col;
    target_element.scale = grid[(target_row - 1)][(target_col - 1)].scale;
}


void find_new_location ( Location &new_location, char direction, int center_row, int center_col, int move_by) {
    new_location.center_row = center_row;
    new_location.center_col = center_col;

    switch (direction) {
        case 'R':
            new_location.center_col = center_col + move_by;
            break;
        case 'L':
            new_location.center_col = center_col - move_by;
            break;
        case 'U':
            new_location.center_row = center_row - move_by;
            break;
        case 'D':
            new_location.center_row = center_row + move_by;
            break;
    }
}








