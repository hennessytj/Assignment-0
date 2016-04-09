/******************************************************************************
cs2123p0.c by Timothy Hennessy
Purpose:
    This program reads customers and their traits.  It displays that information.
Command Parameters:
    n/a
Input:
    The standard input file stream contains two types of records (terminated
    by EOF).  CUSTOMER records are followed by zero to many TRAIT records 
    (terminated by either EOF or another CUSTOMER record).
    CUSTOMER szCustomerId szName
      8s           6s        20s (but the name may have blanks)
    TRAIT    szTraitType  szTraitValue
      8s          10s        12s
Results:
    For each customer, it prints the customer ID, name, and the traits.
Returns:
    0 - normal
    1 - too many customers
    2 - too many traits
    3 - bad input data
Notes:
    1. This program only allows for 30 customers.
    2. A customer may have multiple occurrences of the same trait type.  For example,
       he/she may have multiple EXERCISE traits because he/she enjoys HIKE, 
       BIKE, and TENNIS.
    3. The number of traits for a customer must not exceed 12.
*******************************************************************************/
// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1

/*  Include Files */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "cs2123p0.h"

int main(int argc, char *argv[])
{
    Customer customerM[MAX_CUSTOMERS];  // customer array
    int iNumberOfCustomers = 0;         // number of customers in customerM

    getCustomerData(customerM, &iNumberOfCustomers);

    printCustomerData(customerM, iNumberOfCustomers);

    return 0;
}

/******************** getCustomerData **********************************************
void getCustomerData (Customer customerM[], int *piNumCustomer)
Purpose:
	This function parses lines of data from a given file (until MAX_LINE_SIZE bytes
	is reached, a newline is reached, or EOF is reached).  The function calls 
	getToken which returns a token from the buffer (szInputBuffer).  This function 
	deals with two record types: CUSTOMER and TRAIT.  The record type encountered 
	determines the programs behavior for storing the information collected in 
	szInputBuffer. 
Parameters:
    O    Customer customerM[]          Customer structure array
    I/O  int *piNumCustomer            Number of customers in input file
Notes:
	-MAX_LINE_SIZE is a constant set to 100 defined in cs2123p0.h 
	-MAX_CUSTOMERS is a constant set to 30 defined in cs2123p0.h
	-ERR_TOO_MANY_CUST is a constant set to 1 defined in cs2123p0.h 
	-ERR_BAD_INPUT is a constant set to 3 defined in cs2123p0.h
	-MAX_TRAITS is a constant set to 12 defined in cs2123p0.h
	-Reads lines from a redirected file on the command line at execution
	-Calls getToken and processes the returned token: CUSTOMER or TRAIT
	-Takes an address returned from getToken that points to the remaining string
	 and stores it in pszRemainingTxt
	-pszRemainingTxt is used to assign values to the customer structures
************************************************************************************/
void getCustomerData(Customer customerM[], int *piNumCustomer)
{
    FILE *pfileCustomer;                    // input stream file
    char szInputBuffer[MAX_LINE_SIZE + 1];  // input buffer for fgets

    int iNumTrait = 0;                      // Number of traits for the current customer
    char szRecordType[11];                  // record type of either CUSTOMER or TRAIT
    int i = -1;                             // current customer subscript. -1 indicates 
                                            // not on a customer yet
    int iScanfCnt;                          // scanf returns the number of successful inputs
    char *pszRemainingTxt;                  // After grabbing a token, this is the next
                                            // position.  This will be after the delimiter
                                            // unless the string was terminated by a zero
                                            // byte, then it will be on the zero byte.
    pfileCustomer = stdin;					// file pointer is set to standard input stream

    // read data input lines of text until EOF.  fgets returns NULL at EOF
    while (fgets(szInputBuffer, MAX_LINE_SIZE, pfileCustomer) != NULL)
    {
        // if the line is just a line feed, skip it.
        if (szInputBuffer[0] == '\n')
            continue;
    
        // print the input buffer as is (it also has a linefeed)
        printf("%s", szInputBuffer);

        // get the CUSTOMER or TRAIT command
        pszRemainingTxt = getToken(szInputBuffer, szRecordType, sizeof(szRecordType)-1);

        // see if getting a customer or a trait
        if (strcmp(szRecordType, "CUSTOMER") == 0)
        {
            i++;
            // see if we have too many customers to fit in the array
            if (i >= MAX_CUSTOMERS)
                ErrExit(ERR_TOO_MANY_CUST
                    , "Invalid input file, max customers is %d"
                    , MAX_CUSTOMERS);

            iNumTrait = 0;   // since we have a new customer, reset his/her number of traits
			customerM[i].iNumberOfTraits = iNumTrait;	// initializes iNumberOfTraits to 0
			
            iScanfCnt = sscanf(pszRemainingTxt, "%6s %20[^\n]\n"
                , customerM[i].szCustomerId
                , customerM[i].szCustomerName);
            
            // Check for bad input.  scanf returns the number of valid conversions
            if (iScanfCnt < 2)
            {
                WARNING("Expected ID and name, received %d successful values"
                    , iScanfCnt);
                continue;
            }
        }
        else if (strcmp(szRecordType, "TRAIT") == 0)
        {
            // what if we haven't received a CUSTOMER record yet
            if (i < 0)
                ErrExit(ERR_BAD_INPUT
                    , "TRAIT record without CUSTOMER");

            // what if we have too many traits 
            if (iNumTrait >= MAX_TRAITS)
                ErrExit(ERR_BAD_INPUT
                    , "Too many traits for Customer ID %s, only %d traits allowed"
                    , customerM[i].szCustomerId
                    , MAX_TRAITS);

            iScanfCnt = sscanf(pszRemainingTxt, "%10s %12s"
                , customerM[i].traitM[iNumTrait].szTraitType
                , customerM[i].traitM[iNumTrait].szTraitValue);

            // Check for bad input.  scanf returns the number of valid conversions
            if (iScanfCnt < 2)
            {
                WARNING(
                    "Expected trait type and value, received %d successful values"
                    , iScanfCnt);
                continue;
            }
            iNumTrait++;
            customerM[i].iNumberOfTraits = iNumTrait;
        }
        else
        {
            WARNING("Bad Command in input, found '%s'"
                , szRecordType);
            continue;
        }
    }
    *piNumCustomer = i + 1;
}

/************************ printCustomerData *************************************
void printCustomerData(Customer customerM[], int iNumCustomer)
Purpose:
    Prints data from an array of structures of type Customer including the 
	traits associated with each customer record.
Parameters:
    I	Customer customerM[]	Customer structure array
	I	int	     iNumCustomer	Number of elements in customerM[]
Notes:
    -Uses nested for loops to print each customer and their traits
********************************************************************************/
void printCustomerData(Customer customerM[], int iNumCustomer)
{
    int i;
    int j;
    // Print a heading for the list of customers and traits
    printf("ID         Customer Name\n"
		   "                Trait      Value\n");
	
    for (i = 0; i < iNumCustomer; i++)
    {
        // Print the customer information
		printf("%-11s%s\n" 
				, customerM[i].szCustomerId
				, customerM[i].szCustomerName);

        // Print each of the traits
        for (j = 0; j < customerM[i].iNumberOfTraits; j++)
        {
            // Print a trait
			printf("                %-11s%s\n"
					, customerM[i].traitM[j].szTraitType
					, customerM[i].traitM[j].szTraitValue);

        }
    }
}

/******************** getToken **************************************
char * getToken (char *pszInputTxt, char szToken[], int iTokenSize)
Purpose:
    Examines the input text to return the next token.  It also 
    returns the position in the text after that token.
Parameters:
    I   char *pszInputTxt       input buffer to be parsed
    O   char szToken[]          Returned token.
    I   int iTokenSize          The size of the token variable.  This is used
                                to prevent overwriting memory.  The size 
                                should be the memory size minus 1 (for
                                the zero byte).
Returns:
    Functionally: 
         Pointer to the next character following the delimiter after the token.  
         NULL - no token found.
    szToken parm - the returned token.  If not found, it will be an 
         empty string.
Notes:
    - If the token is larger than the szToken parm, we return a truncated value.
    - If a token isn't found, szToken is set to an empty string
    - This function does not skip over white space occurring prior to the token.
    - If the input buffer pointer is NULL, the function terminates with
      an algorithm error.
**************************************************************************/
char * getToken (char *pszInputTxt, char szToken[], int iTokenSize)
{
    int iDelimPos;                      // found position of delim
    int iCopy;                          // number of characters to copy
    char szDelims[20] = " \n\r";        // delimiters
    szToken[0] = '\0';

    // check for NULL pointer 
    if (pszInputTxt == NULL)
        ErrExit(ERR_ALGORITHM
            , "getToken passed a NULL pointer"); 

    // Check for no token if at zero byte
    if (*pszInputTxt == '\0')
        return NULL;

    // get the position of the first delim
    iDelimPos = strcspn(pszInputTxt, szDelims);

    // see if we have more characters than target token, if so, trunc
    if (iDelimPos > iTokenSize)
        iCopy = iTokenSize;             // truncated size
    else
        iCopy = iDelimPos;

    // copy the token into the target token variable
    memcpy(szToken, pszInputTxt, iCopy);
    szToken[iCopy] = '\0';              // null terminate

    // advance the position
    pszInputTxt += iDelimPos;
    if (*pszInputTxt == '\0')
        return pszInputTxt;
    else
        return pszInputTxt + 1;
}
/******************** ErrExit **************************************
  void ErrExit(int iexitRC, char szFmt[], ... )
Purpose:
    Prints an error message defined by the printf-like szFmt and the
    corresponding arguments to that function.  The number of 
    arguments after szFmt varies dependent on the format codes in
    szFmt.  
    It also exits the program with the specified exit return code.
Parameters:
    I   int iexitRC             Exit return code for the program
    I   char szFmt[]            This contains the message to be printed
                                and format codes (just like printf) for 
                                values that we want to print.
    I   ...                     A variable-number of additional arguments
                                which correspond to what is needed
                                by the format codes in szFmt. 
Returns:
    Returns a program exit return code:  the value of iexitRC.
Notes:
    - Prints "ERROR: " followed by the formatted error message specified 
      in szFmt. 
    - Prints the file path and file name of the program having the error.
      This is the file that contains this routine.
    - Requires including <stdarg.h>
**************************************************************************/
void ErrExit(int iexitRC, char szFmt[], ... )
{
    va_list args;               // This is the standard C variable argument list type
    va_start(args, szFmt);      // This tells the compiler where the variable arguments
                                // begins.  They begin after szFmt.
    printf("ERROR: ");
    vprintf(szFmt, args);       // vprintf receives a printf format string and  a
                                // va_list argument
    va_end(args);               // let the C environment know we are finished with the
                                // va_list argument
    printf("\n\tEncountered in file %s\n", __FILE__);  // this 2nd arg is filled in by
                                // the pre-compiler
    exit(iexitRC);
}