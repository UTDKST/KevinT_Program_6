//Kevin Truong
//kst150030@utdallas.edu
//CS3377.501
//
//Uses binary file I/O techniques to open a binary file called cs3377.bin.
//From the binary file, read/display the header record and then read/display the data records.
//The header record and data records will be displayed in a CDK window.
//The CDK window is a 3 wide by 5 high display matrix. 
//First row of the table will display the three fields found in the header record
////The three fields contain the magic number, the version, and the number of records.
//The rest of the rows will display the two fields of the first 4 data records in the file (or the total number of data records if less than 4).
////These two fields contain the string length and the string.
//After display, program waits for the user types a character on the keyboard. This will exit the program.

#include <iostream>
#include "cdk.h"
#include <fstream>
#include <iomanip>
#include <stdint.h>

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 25
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

//For the header record in the binary file
class BinaryFileHeader
{
 public:

  uint32_t magicNumber;
  uint32_t versionNumber;
  uint64_t numRecords;
};

//For the data record in the binary file
const int maxRecordStringLength = 25;
class BinaryFileRecord
{
 public:

  uint8_t strLength;
  char stringBuffer[maxRecordStringLength];
};


int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  // Remember that matrix starts out at 1,1.
  // Since arrays start out at 0, the first entries
  // below ("R0", and "C0") are just placeholders
  // 
  // Finally... make sure your arrays have enough entries given the
  // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
  // above.

  const char 		*rowTitles[] = {"0", "a", "b", "c", "d", "e"};
  const char 		*columnTitles[] = {"0", "a", "b", "c"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);

  /*
   * Display a message
   */

  ifstream binInfile ("cs3377.bin", ios::in | ios::binary);
  char str[1024];
 
  //Display the header record in the first row 
  BinaryFileHeader *myHeader = new BinaryFileHeader();
  binInfile.read((char *) myHeader, sizeof(BinaryFileHeader));

  sprintf(str, "Magic: 0x%04X", myHeader->magicNumber);
  setCDKMatrixCell(myMatrix, 1, 1, str);
  sprintf(str, "Version: %d", myHeader->versionNumber);
  setCDKMatrixCell(myMatrix, 1, 2, str);
  sprintf(str, "NumRecords: %lu", myHeader->numRecords);
  setCDKMatrixCell(myMatrix, 1, 3, str);

  //Display the first 4 data records or the total number of data records if less than 4
  BinaryFileRecord *myRecord = new BinaryFileRecord();
  int rowCount = 2;
  while(binInfile.read((char *) myRecord, sizeof(BinaryFileRecord)) && rowCount <= MATRIX_HEIGHT )
    {
      sprintf(str, "strlen: %d", myRecord->strLength);
      setCDKMatrixCell(myMatrix, rowCount, 1, str);
      sprintf(str, myRecord->stringBuffer);
      setCDKMatrixCell(myMatrix, rowCount, 2, str);

      rowCount++;
    }

  binInfile.close();

  drawCDKMatrix(myMatrix, true);    /* required  */

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK();
}
