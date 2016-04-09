/**********************************************************************
cs2123p0.h
Purpose:
   Defines the typedefs:
       Trait
       Customer
   Defines maximum size constants
   Defines error constants
   Defines function prototypes used only for this program.
   Defines WARNING macro.
Notes:
Description of any special information around usage.
**********************************************************************/

/* constants */
#define MAX_CUSTOMERS 30         // Maximum number of customers
#define MAX_TRAITS 12            // Maximum number of traits per customer       
#define MAX_LINE_SIZE 100        // Maximum number of character per input line

#define ERR_TOO_MANY_CUST 1      // Too many customers
#define ERR_TOO_MANY_TRAITS 2    // Too many traits
#define ERR_BAD_INPUT 3          // Bad input 
#define ERR_ALGORITHM 999        // Unexpected error in algorithm

#define TRUE 1
#define FALSE 0

/* typedefs and function prototypes */

/* Trait typedef defines a trait type (e.g., GENDER) and
** corresponding trait value (e.g., M, F)
*/
typedef struct
{
    char szTraitType[11];        // Trait Type can be one of:
    //    GENDER
    //    SMOKING
    //    MOVIE
    //    BOOK
    //    EXERCISE 
    char szTraitValue[13];       // This corresponds to the Trait Type:
    //    GENDER:   M, F
    //    SMOKING:  Y, N
    //    MOVIE:    ROMANCE, COMEDY, ACTION, FAMILY, HORROR
    //    BOOK:     SCIFI, DRAMA, MYSTERY, HORROR, TRAVEL, ROMANCE, 
    //              COOKING
    //    EXERCISE: TENNIS, GOLF, JOG, RUN, YOGA, DANCE, HIKE, BIKE 
} Trait;

/* Customer typedef contains customer Id, customer name, and an array of traits */
typedef struct
{
    char szCustomerId[7];               // Customer Identifier
    char szCustomerName[21];            // Customer Full Name
    int  iNumberOfTraits;               // The number of traits for each customer
    Trait traitM[MAX_TRAITS];
} Customer;

void getCustomerData(Customer customerM[], int *piNumCustomer);
void printCustomerData(Customer customerM[], int iNumCustomer);

char * getToken(char *pszInputTxt, char szToken[], int iTokenSize);
void ErrExit(int iexitRC, char szFmt[], ...);

/* 
  WARNING macro 
  Parameters:
     I szFmt - a printf format
     I ...   - a variable number of parameters corresponding to szFmt's
               format codes.
  Results:
     Prints "WARNING" and the value(s) specified by the szFmt.
  Notes:
     Since this generates multiple C statements, we surround them
     with a dummy do while(0) which only executes once.  Notice that the
     dummy do while isn't ended with a ";" since the user of 
     the macro naturally specifies a ";".  Example:
         if (x != 0)
             WARNING("X must be blah blah");
         else
         {   // normal processing
            ....
         } 
     If we used {} in the macro definition instead of the dummy do while(0),
     the generated code would have a bad ";":
         if (x != 0)
         {
             printf("\tWARNING: "); 
             printf("X must be blah blah");
             printf("\n");
         } ;  // yuck, bad ";" causing the compiler to not understand the else
         else
         {   // normal processing
            ....
         } 
*/
#define WARNING(szFmt, ...) do {    \
    printf("\tWARNING: ");          \
    printf(szFmt, __VA_ARGS__);     \
    printf("\n");                   \
    } while (0)

