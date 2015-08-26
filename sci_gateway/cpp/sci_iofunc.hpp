// Symphony Toolbox for Scilab
// (Declaration of) Functions for input and output from Scilab
// By Keyur Joshi

#ifndef SCI_IOFUNCHEADER
#define SCI_IOFUNCHEADER

//input
int getDoubleFromScilab(int argNum, double *dest);
int getUIntFromScilab(int argNum, int *dest);
int getIntFromScilab(int argNum, int *dest);
int getFixedSizeDoubleMatrixFromScilab(int argNum, int rows, int cols, double **dest);
int getDoubleMatrixFromScilab(int argNum, int *rows, int *cols, double **dest);

//output
int return0toScilab();
int returnDoubleToScilab(double retVal);

#endif //SCI_IOFUNCHEADER
