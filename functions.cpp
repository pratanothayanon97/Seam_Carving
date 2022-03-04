#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;

Pixel** createImage(int width, int height) {
  cout << "Start createImage... " << endl;
  
  // Create a one dimensional array on the heap of pointers to Pixels 
  //    that has width elements (i.e. the number of columns)
  Pixel** image = new Pixel*[width];
  
  bool fail = false;
  
  for (int i=0; i < width; ++i) { // loop through each column
    // assign that column to a one dimensional array on the heap of Pixels
    //  that has height elements (i.e. the number of rows)
    image[i] = new Pixel[height];
    
    if (image[i] == nullptr) { // failed to allocate
      fail = true;
    }
  }
  
  if (fail) { // if any allocation fails, clean up and avoid memory leak
    // deallocate any arrays created in for loop
    for (int i=0; i < width; ++i) {
      delete [] image[i]; // deleting nullptr is not a problem
    }
    delete [] image; // dlete array of pointers
    return nullptr;
  }
  
  // initialize cells
  //cout << "Initializing cells..." << endl;
  for (int row=0; row<height; ++row) {
    for (int col=0; col<width; ++col) {
      //cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = { 0, 0, 0 };
    }
  }
  cout << "End createImage... " << endl;
  return image;
}

void deleteImage(Pixel** image, int width) {
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i=0; i<width; ++i) {
    delete [] image[i]; // delete each individual array placed on the heap
  }
  delete [] image;
  image = nullptr;
}

int* createSeam(int length) {
  
  int* seam = new int[length];
  for (int i = 0; i < length; ++i) {
    seam[i] = 0;
  }
  
  return seam;
  
}

void deleteSeam(int* seam) {
  delete [] seam;
}

bool loadImage(string filename, Pixel** image, int width, int height) {
  cout << "Loading image..." << endl;
  // declare/define and open input file stream
  ifstream ifs (filename);
  
  // check if input stream opened successfully
  if (!ifs.is_open()) {
    cout << "Error: failed to open input file - " << filename << endl;
    return false;
  }
  
  // get type from preamble
  char type[3];
  ifs >> type; // should be P3
  if ((toupper(type[0]) != 'P') || (type[1] != '3')) { // check that type is correct
    cout << "Error: type is " << type << " instead of P3" << endl;
    return false;
  }
  
  // get width (w) and height (h) from preamble
  int w = 0, h = 0;
  ifs >> w;
  if (ifs.fail()) {   
    cout << "Error: read non-integer value" << endl;
    ifs.clear();
    return false;  
  }
  if (w != width) { // check that width matches what was passed into the function
    cout << "Error: input width (" << width << ") does not match value in file (" << w << ")" << endl;
    cout << " - input width: " << width << endl;
    cout << " -  file width: " << w << endl;
    return false;
  }
  ifs >> h;
  if (ifs.fail()) {   
    cout << "Error: read non-integer value" << endl;
    ifs.clear();
    return false;  
  }
  if (h != height) { // check that height matches what was passed into the function
    cout << "Error: input height (" << height << ") does not match value in file (" << h << ")" << endl;
    cout << " - input height: " << height << endl;
    cout << " -  file height: " << h << endl;
    return false;
  }
  
  // get maximum value from preamble
  int colorMax = 0;
  ifs >> colorMax;
  if (colorMax != 255) {
    cout << "Error: file is not using RGB color values." << endl;
    return false;
  }
 
  //Check errors when reading the file
  for (int row = 0; row < height; row++) {
	  for (int col = 0; col < width; col++) {
      
        ifs >> image[col][row].r; 
        if (ifs.fail() && ifs.eof()) {
          cout << "Error: not enough color values" << endl;
          return false;
        }
        if (ifs.fail()) {
          cout << "Error: read non-integer value" << endl;
          ifs.clear();
          return false;
        }
        
        if (image[col][row].r > 255 || image[col][row].r < 0) {
          cout << "Error: invalid color value " << image[col][row].r << endl;
          return false;
        }
        else {
          ifs >> image[col][row].g;
          if (ifs.fail()&& ifs.eof()) {
            cout << "Error: not enough color values" << endl;
            return false;
          }
          if (ifs.fail()) {
          cout << "Error: read non-integer value" << endl;
          ifs.clear();
          return false;
          }
          
          if (image[col][row].g > 255 || image[col][row].g < 0) {
            cout << "Error: invalid color value " << image[col][row].g << endl;
            return false;
          }
          else {
            ifs >> image[col][row].b;
            if (ifs.fail() && ifs.eof()){
              cout << "Error: not enough color values" << endl;
              return false;
            }
            if (ifs.fail()) {
              cout << "Error: read non-integer value" << endl;
              ifs.clear();
              return false;
            }
            
            if (image[col][row].b > 255 || image[col][row].b < 0) {
              cout << "Error: invalid color value " << image[col][row].b << endl;
              return false;
            }
          }  
        }
    }
  }
  int check;
  ifs >> check;
  if (!ifs.fail()) {
    cout << "Error: too many color values" << endl; 
    return false;
    //ifs.clear()
  }
//}
return true;
}

bool outputImage(string filename, Pixel** image, int width, int height) {
  cout << "Outputting image..." << endl;
  //declare/define and open output file stream
  ofstream outFS;
  outFS.open(filename);      //create the file
  //check if output stream opened successfully
  if (!outFS.is_open()) {        //make sure it's opened
        cout << "Error: failed to open output file" << filename << endl;
        return false;
  }
  if(outFS.fail()){
    return false;
  }
  //output preamble
  int colorMAX = 255;
  outFS << "P3" << endl;
  outFS << width << " " << height << endl;
  outFS << colorMAX << endl;
  //output pixels
  for (int row = 0; row < height; row++) {
	  for (int col = 0; col < width; col++) {
        outFS << image[col][row].r << endl; 
        outFS << image[col][row].g << endl; 
        outFS << image[col][row].b << endl;
    }
  }
  outFS.close();  
  return true;
}

int energy(Pixel** image, int x, int y, int width, int height) { 

  int rx_gradiant;
  int gx_gradiant;
  int bx_gradiant;
  int ry_gradiant;
  int gy_gradiant;
  int by_gradiant;
  int gradiant_x_square;
  int gradiant_y_square;
  int energy;
      
  if (y == 0){       //y is row          //top
    ry_gradiant = image[x][y + 1].r - image[x][height - 1].r;
    gy_gradiant = image[x][y + 1].g - image[x][height - 1].g;
    by_gradiant = image[x][y + 1].b - image[x][height - 1].b;
  }
  else if (y == height - 1){  //bottom
    ry_gradiant = image[x][height - 2].r - image[x][0].r;
    gy_gradiant = image[x][height - 2].g - image[x][0].g;
    by_gradiant = image[x][height - 2].b - image[x][0].b;
  }
  else { //non-boarder case
    ry_gradiant = image[x][y + 1].r - image[x][y - 1].r;
    gy_gradiant = image[x][y + 1].g - image[x][y - 1].g;
    by_gradiant = image[x][y + 1].b - image[x][y - 1].b;
  }
  if (x == 0){               //leftmost
    rx_gradiant = image[x + 1][y].r - image[width - 1][y].r;
    gx_gradiant = image[x + 1][y].g - image[width - 1][y].g;
    bx_gradiant = image[x + 1][y].b - image[width - 1][y].b;
  }
  else if (x == width - 1){  //rightmost
    rx_gradiant = image[width - 2][y].r - image[0][y].r;
    gx_gradiant = image[width - 2][y].g - image[0][y].g;
    bx_gradiant = image[width - 2][y].b - image[0][y].b;  
  }
  else { //non-boarder case
    rx_gradiant = image[x + 1][y].r - image[x - 1][y].r;
    gx_gradiant = image[x + 1][y].g - image[x - 1][y].g;
    bx_gradiant = image[x + 1][y].b - image[x - 1][y].b;  
  }
  
  //calculate gradiant x and y square & energy
  gradiant_x_square = (rx_gradiant * rx_gradiant) + (gx_gradiant * gx_gradiant) + (bx_gradiant * bx_gradiant);
  gradiant_y_square = (ry_gradiant * ry_gradiant) + (gy_gradiant * gy_gradiant) + (by_gradiant * by_gradiant);
  energy = gradiant_x_square + gradiant_y_square;
  return energy;
}

int loadVerticalSeam(Pixel** image, int start_row, int width, int height, int* seam) {
  seam[0] = start_row;
  int total_energy = energy(image, start_row, 0, width, height);
  for (int row = 1; row < height; ++row) {
    int curr_col = seam[row - 1];
    int left, right, middle;
    middle = energy(image, curr_col, row, width, height);
    if (curr_col == 0) {
      left = INT32_MAX; 
    }
    else {
      left = energy(image, curr_col - 1, row, width, height); 
    }
    if (curr_col == width - 1) {
      right = INT32_MAX;
    }
    else {
      right = energy(image, curr_col + 1, row, width, height);
    }
  if (middle < left) {        
      if (middle <= right) {
        total_energy += middle;
        seam[row] = curr_col;
      }
      else {
        total_energy += right;
        seam[row] = curr_col + 1;
      }
    }
  else if (middle < right) {
      if (middle <= left) {
        total_energy += middle;
       seam[row]  = curr_col;
      }
      else {
        total_energy += left;
        seam[row] = curr_col - 1;
      }
    }
  else if (middle > left || middle > right) {   //middle is not minimum
      if (right <= left) {
        total_energy += right;
        seam[row] = curr_col + 1;
      }
      else {
        total_energy += left;
        seam[row] = curr_col - 1;
      }
    }
  else {  //All equal
        total_energy += middle;
        seam[row] = curr_col;
      }
}

  return total_energy;
}

int loadHorizontalSeam(Pixel** image, int start_row, int width, int height, int* seam) {
 
  seam[0] = start_row;
  int total_energy = energy(image,  0, start_row, width, height);
  for (int col = 1; col < width; ++col) {
    int curr_row = seam[col - 1];
    int top, bottom, middle;
    middle = energy(image, col, curr_row, width, height);
    if (curr_row == 0) {
      top = INT32_MAX; 
    }
    else {
      top = energy(image, col , curr_row - 1, width, height); 
    }
    if (curr_row == height - 1) {
      bottom = INT32_MAX;
    }
    else {
      bottom = energy(image, col, curr_row + 1, width, height);
    }
  if (middle < top) {        
      if (middle <= bottom) {
        total_energy += middle;
        seam[col] = curr_row;
      }
      else {
        total_energy += bottom;
        seam[col] = curr_row + 1;
      }
    }
  else if (middle < bottom) {
      if (middle <= top) {
        total_energy += middle;
       seam[col]  = curr_row;
      }
      else {
        total_energy += top;
        seam[col] = curr_row - 1;
      }
    }
  else if (middle > top || middle > bottom) {   //middle is not minimum
      if (top <= bottom) {
        total_energy += top;
        seam[col] = curr_row - 1;
      }
      else {
        total_energy += bottom;
        seam[col] = curr_row + 1;
      }
    }
  else {  //All equal
        total_energy += middle;
        seam[col] = curr_row;
      }
}
return total_energy;
}

int* findMinVerticalSeam(Pixel** image, int width, int height) {
  int* seam_min = createSeam(height);   //create seam to store min value (Is this correct?)
  int minEnergy = loadVerticalSeam(image, 0, width, height, seam_min);   //store first value as min    
  for (int col = 1; col < width; ++col) {    //iterate through each column
    int* seam_temp = createSeam(height);
    int energyVertical = loadVerticalSeam(image, col, width, height, seam_temp);  //total min energy for each seam's column
    if (energyVertical < minEnergy) {
      minEnergy = energyVertical;       //set to the min value
      for(int j = 0; j < height; ++j){
        seam_min[j] = seam_temp[j];
      }
    }
  deleteSeam(seam_temp);
  }
  return seam_min;
}


int* findMinHorizontalSeam(Pixel** image, int width, int height) {
  int* seam_min_Horizontal = createSeam(width);   //create seam to store min value 
  int minEnergy = loadHorizontalSeam(image, 0, width, height, seam_min_Horizontal);   //store first value as min
  for (int row = 1; row < height; ++row) {    //iterate through each row
    int* seam_temp_Horizontal = createSeam(width);
    int energyHorizontal = loadHorizontalSeam(image, row, width, height, seam_temp_Horizontal);  //total min energy for each seam's row
    if (energyHorizontal < minEnergy) {
      minEnergy = energyHorizontal;       //set to the min value
      for(int i = 0; i < width; ++i){
        seam_min_Horizontal[i] = seam_temp_Horizontal[i];
      }
    }
  deleteSeam(seam_temp_Horizontal);
  }
  return seam_min_Horizontal;
  
}



void removeVerticalSeam(Pixel** image, int width, int height, int* verticalSeam) {
 
  for (int row = 0; row < height; ++row) {     
      for (int col = verticalSeam[row]; col < width - 1; ++col) {  //if the col matches the index   
          image[col][row] = image[col + 1][row];    //shift the next column
      }          
  }
}

void removeHorizontalSeam(Pixel** image, int width, int height, int* horizontalSeam) {
  for (int col = 0; col < width; ++col) {     
      for (int row = horizontalSeam[col]; row < height - 1; ++row) {  //if the col matches the index   
          image[col][row] = image[col][row + 1];    //shift the next column
      }          
  } 
}



