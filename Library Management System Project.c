#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#define MAX_YR  9999
#define MIN_YR  1900
#define MAX_SIZE_USER_NAME 30
#define MAX_SIZE_PASSWORD  20
#define FILE_NAME_DB "LibraryDataBase.bin"
#define FILE_NAME  "LibaryBooks.bin"
#define FILE_NAME_login "UserCredentials.bin"
#define MAX_BOOK_NAME   50
#define MAX_AUTHOR_NAME 50
#define MAX_STUDENT_NAME 50
#define FILE_HEADER_SIZE  sizeof(sLoginCred)

//structure to store date
typedef struct
{
    int yyyy;
    int mm;
    int dd;
} Date;
typedef struct
{
    char username[MAX_SIZE_USER_NAME];
    char password[MAX_SIZE_PASSWORD];
} sLoginCred;
typedef struct// to call in program
{
    unsigned int books_id; // declare the integer data type
    char bookName[MAX_BOOK_NAME];// declare the character data type
    char authorName[MAX_AUTHOR_NAME];// declare the charecter data type
    char studentName[MAX_STUDENT_NAME];// declare the character data type
    Date bookIssueDate;// declare the integer data type
} s_BooksInfo;
typedef struct// to call in program
{
    unsigned int books_id; // declare the integer data type
    char bookName[MAX_BOOK_NAME];// declare the character data type
    char authorName[MAX_AUTHOR_NAME];
} Librarydatabase;
static void printMessageCenter(const char* message)
{
    unsigned int len =0;
    unsigned int pos = 0;
    //calculate how many space need to print
    len = (78 - (unsigned int)strlen(message))/2;
    printf("\t\t\t");
    for(pos =0 ; pos < len ; pos++)
    {
        //print space
        printf(" ");
    }
    //print message
    printf("%s",message);
}
static void headMessage(const char *message)
{
    (void)system("cls");
    printf("\t\t\t###########################################################################");
    printf("\n\t\t\t############                                                   ############");
    printf("\n\t\t\t############      Library management System Project in C       ############");
    printf("\n\t\t\t############                                                   ############");
    printf("\n\t\t\t###########################################################################");
    printf("\n\t\t\t---------------------------------------------------------------------------\n");
    printMessageCenter(message);
    printf("\n\t\t\t----------------------------------------------------------------------------");
}
static void welcomeMessage()
{
    printf("\n\n\n\n\n");
    printf("\n\t\t\t  **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**\n");
    printf("\n\t\t\t        =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
    printf("\n\t\t\t        =                 WELCOME                   =");
    printf("\n\t\t\t        =                   TO                      =");
    printf("\n\t\t\t        =                 LIBRARY                   =");
    printf("\n\t\t\t        =               MANAGEMENT                  =");
    printf("\n\t\t\t        =                 SYSTEM                    =");
    printf("\n\t\t\t        =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
    printf("\n\t\t\t  **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**\n");
    printf("\n\n\n\t\t\t Enter any key to continue.....");
    (void)getchar();
}
static bool isNameValid(const char *name)
{
    bool validName = true;
    int len = 0;
    int index = 0;
    len = (int)strlen(name);
    for(index =0; index <len ; ++index)
    {
        if(!(isalpha(name[index])) && (name[index] != '\n') && (name[index] != ' '))
        {
            validName = false;
            break;
        }
    }
    return validName;
}
// Function to check leap year.
//Function returns 1 if leap year
static bool  IsLeapYear(int year)
{
    return (((year % 4 == 0) &&
             (year % 100 != 0)) ||
            (year % 400 == 0));
}
// returns 1 if given date is valid.
static bool isValidDate(Date *validDate)
{
    //check range of year,month and day
    if (validDate->yyyy > MAX_YR ||
            validDate->yyyy < MIN_YR)
        return false;
    if (validDate->mm < 1 || validDate->mm > 12)
        return false;
    if (validDate->dd < 1 || validDate->dd > 31)
        return false;
    //Handle feb days in leap year
    if (validDate->mm == 2)
    {
        if (IsLeapYear(validDate->yyyy))
            return (validDate->dd <= 29);
        else
            return (validDate->dd <= 28);
    }
    //handle months which has only 30 days
    if (validDate->mm == 4 || validDate->mm == 6 ||
            validDate->mm == 9 || validDate->mm == 11)
        return (validDate->dd <= 30);
    return true;
}
static void addBookLib()
{
    
    Librarydatabase addBook ;
    FILE *fp = NULL;
    bool status = false;
    fp = fopen(FILE_NAME_DB,"ab+");
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(0);
    }
    headMessage("ADD NEW BOOKS");
    printf("\n\n\t\t\tENTER YOUR DETAILS BELOW:");
    printf("\n\t\t\t---------------------------------------------------------------------------\n");
    printf("\n\t\t\tBook ID NO  = ");
    (void)fflush(stdin);
    (void)scanf("%u",&addBook.books_id);
    do
    {
        printf("\n\t\t\tBook Name  = ");
        (void)fflush(stdin);
        (void)fgets(addBook.bookName,MAX_BOOK_NAME,stdin);
        status = isNameValid(addBook.bookName);
        if (!status)
        {
            printf("\n\t\t\tName contain invalid character. Please enter again.");
        }
    }
    while(!status);
    do
    {
        printf("\n\t\t\tAuthor Name  = ");
        (void)fflush(stdin);
        (void)fgets(addBook.authorName,MAX_AUTHOR_NAME,stdin);
        status = isNameValid(addBook.authorName);
        if (!status)
        {
            printf("\n\t\t\tName contain invalid character. Please enter again.");
        }
    }
    while(!status);
	(void)fwrite(&addBook,sizeof(addBook), 1, fp);
    (void)fclose(fp);
}
// Add books in list
static void addBookInDataBase()
{
    bool found = false;
    char bookName[MAX_BOOK_NAME] ;
	
    s_BooksInfo addBookInfoInDataBase ;
    Librarydatabase books ;
	FILE *fp = NULL;
	FILE *fp2 = NULL;
    bool status = false;
    fp = fopen(FILE_NAME,"ab+");
	fp2 = fopen(FILE_NAME_DB,"rb");
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(0);
    }
    headMessage("ADD NEW BOOKS");
    printf("\n\n\t\t\tENTER YOUR DETAILS BELOW:");
    printf("\n\t\t\t---------------------------------------------------------------------------\n");
	do
    {
        printf("\n\t\t\tStudent Name  = ");
        (void)fflush(stdin);
   (void)fgets(addBookInfoInDataBase.studentName,MAX_STUDENT_NAME,stdin);
        status = isNameValid(addBookInfoInDataBase.studentName);
        if (!status)
        {
            printf("\n\t\t\tName contain invalid character. Please enter again.");
        }
    }
    while(!status);
	do
    {
        //get date year,month and day from user
        printf("\n\t\t\tEnter date in format (day/month/year): ");
        (void)scanf("%d/%d/%d",&addBookInfoInDataBase.bookIssueDate.dd,&addBookInfoInDataBase.bookIssueDate.mm,&addBookInfoInDataBase.bookIssueDate.yyyy);
        //check date validity
        status = isValidDate(&addBookInfoInDataBase.bookIssueDate);
        if (!status)
        {
            printf("\n\t\t\tPlease enter a valid date.\n");
        }
    }
    while(!status);
    printf("\n\n\t\t\tEnter Book Name to search:");
    (void)fflush(stdin);
    (void)fgets(bookName,MAX_BOOK_NAME,stdin);
	(void)fseek(fp2,0,SEEK_SET);
    while ((bool)fread (&books, sizeof(books), 1, fp2))
    {
        if(strcmp(books.bookName, bookName)==0)
        {
            found = true;
            break;
        }
    }
    if(found)
    {
        
		strcpy(addBookInfoInDataBase.bookName, books.bookName);
		strcpy(addBookInfoInDataBase.authorName, books.authorName);
		
		addBookInfoInDataBase.books_id = books.books_id;
		
        
    }
    else
    {
        printf("\n\t\t\tNo Record");
    }
	(void)fwrite(&addBookInfoInDataBase,sizeof(addBookInfoInDataBase), 1, fp);
    (void)fclose(fp);
	(void)fclose(fp2);
    printf("\n\n\n\t\t\tPress any key to go to main menu.....");
    (void)getchar();
}
// search books
static void searchBooks()
{
    int found = 0;
    char bookName[MAX_BOOK_NAME];
    s_BooksInfo addBookInfoInDataBase ;
    FILE *fp = NULL;
    fp = fopen(FILE_NAME,"rb");
    if(fp == NULL)
    {
        printf("\n\t\t\tFile is not opened\n");
        exit(0);
    }
    headMessage("SEARCH BOOKS");
    //put the control on books detail
    if (fseek(fp,0,SEEK_SET) != 0)
    {
        (void)fclose(fp);
        printf("\n\t\t\tFacing issue while reading file\n");
        exit(0);
    }
    printf("\n\n\t\t\tEnter Book Name to search:");
    (void)fflush(stdin);
    (void)fgets(bookName,MAX_BOOK_NAME,stdin);
	//fseek(fp,0,SEEK_SET);
    while ((bool)fread (&addBookInfoInDataBase, sizeof(addBookInfoInDataBase), 1, fp))
    {
        if(strcmp(addBookInfoInDataBase.bookName, bookName)==0)
        {
            found = 1;
            break;
        }
    }
    if((bool)found)
    {
        printf("\n\t\t\tBook id = %u\n",addBookInfoInDataBase.books_id);
        printf("\t\t\tBook name = %s",addBookInfoInDataBase.bookName);
        printf("\t\t\tBook authorName = %s",addBookInfoInDataBase.authorName);
        printf("\t\t\tBook issue date(day/month/year) =  (%d/%d/%d)",addBookInfoInDataBase.bookIssueDate.dd,
               addBookInfoInDataBase.bookIssueDate.mm, addBookInfoInDataBase.bookIssueDate.yyyy);
    }
    else
    {
        printf("\n\t\t\tNo Record");
    }
    (void)fclose(fp);
    printf("\n\n\n\t\t\tPress any key to go to main menu.....");
    (void)getchar();
}
static void viewbookslib()
{
    bool found = false;
    
    Librarydatabase ViewAll ;
    FILE *fp = NULL;
    
    unsigned int countBook = 1;
    headMessage("VIEW BOOKS DETAILS");
    fp = fopen(FILE_NAME_DB,"rb");
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(0);
    }
    if (fseek(fp,0,SEEK_SET) != 0)
    {
        (void)fclose(fp);
        printf("Facing issue while reading file\n");
        exit(0);
    }
    while ((bool)fread (&ViewAll, sizeof(ViewAll), 1, fp))
    {
        printf("\n\t\t\tBook Count = %u\n\n",countBook);
        printf("\t\t\tBook id = %u",ViewAll.books_id);
        printf("\n\t\t\tBook name = %s",ViewAll.bookName);
        printf("\t\t\tBook authorName = %s",ViewAll.authorName);
		found = true;
        ++countBook;
    }
    (void)fclose(fp);
    if(!found)
    {
        printf("\n\t\t\tNo Record");
    }
    printf("\n\n\t\t\tPress any key to go to main menu.....");
    (void)fflush(stdin);
    (void)getchar();
}
// v books function
static void viewBooks()
{
    bool found = false;
    
    s_BooksInfo addBookInfoInDataBase ;
    FILE *fp = NULL;
    
    unsigned int countBook = 1;
    headMessage("VIEW BOOKS DETAILS");
    fp = fopen(FILE_NAME,"rb");
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(0);
    }
    if (fseek(fp,0,SEEK_SET) != 0)
    {
        (void)fclose(fp);
        printf("Facing issue while reading file\n");
        exit(0);
    }
    while ((bool)fread (&addBookInfoInDataBase, sizeof(addBookInfoInDataBase), 1, fp))
    {
        printf("\n\t\t\tBook Count = %u\n\n",countBook);
		printf("\n\t\t\tStudent name = %s",addBookInfoInDataBase.studentName);
        printf("\t\t\tBook id = %u",addBookInfoInDataBase.books_id);
        printf("\n\t\t\tBook name = %s",addBookInfoInDataBase.bookName);
        printf("\t\t\tBook authorName = %s",addBookInfoInDataBase.authorName);
        printf("\t\t\tBook issue date(day/month/year) =  (%d/%d/%d)\n\n",addBookInfoInDataBase.bookIssueDate.dd,
               addBookInfoInDataBase.bookIssueDate.mm, addBookInfoInDataBase.bookIssueDate.yyyy);
        found = true;
        ++countBook;
    }
    (void)fclose(fp);
    if(!found)
    {
        printf("\n\t\t\tNo Record");
    }
    printf("\n\n\t\t\tPress any key to go to main menu.....");
    (void)fflush(stdin);
    (void)getchar();
}
// delete function
static void deleteBooks()
{
    bool found = false;
    unsigned int bookDelete = 0;
    sLoginCred fileHeaderInfo ;
   
    s_BooksInfo addBookInfoInDataBase ;
    FILE *fp = NULL;
    FILE *tmpFp = NULL;
    
    headMessage("Delete Books Details");
    fp = fopen(FILE_NAME,"rb");
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(0);
    }
    tmpFp = fopen("tmp.bin","wb");
    if(tmpFp == NULL)
    {
        (void)fclose(fp);
        printf("File is not opened\n");
        exit(0);
    }
    (void)fread (&fileHeaderInfo,0, 1, fp);
    (void)fwrite(&fileHeaderInfo,0, 1, tmpFp);
    printf("\n\t\t\tEnter Book ID NO. for delete:");
    (void)scanf("%u",&bookDelete);
    while ((bool)fread (&addBookInfoInDataBase, sizeof(addBookInfoInDataBase), 1, fp))
    {
        if(addBookInfoInDataBase.books_id != bookDelete)
        {
            (void)fwrite(&addBookInfoInDataBase,sizeof(addBookInfoInDataBase), 1, tmpFp);
        }
        else
        {
            found = true;
        }
    }
    (found)? printf("\n\t\t\tRecord deleted successfully....."):printf("\n\t\t\tRecord not found");
    (void)fclose(fp);
    (void)fclose(tmpFp);
    (void)remove(FILE_NAME);
    (void)rename("tmp.bin",FILE_NAME);
}

static void menu()
{
    int choice = 0;
    do
    {
        headMessage("MAIN MENU");
        printf("\n\n\n\t\t\t1.Add Books");
        printf("\n\t\t\t2.View Books");
        printf("\n\t\t\t0.Exit");
        printf("\n\n\n\t\t\tEnter choice => ");
        (void)scanf("%d",&choice);
	(void)system("cls");
        switch(choice)
        {
        case 1:
            addBookInDataBase();
            break;
        case 2:
            viewbookslib();
            break;
        case 0:
            printf("\n\n\n\t\t\t\tThank you!!!\n\n\n\n\n");
            exit(0);
            
        default:
            printf("\n\n\n\t\t\tINVALID INPUT!!! Try again...");
        }                                            //Switch Ended
    }
    while(choice!=0);                                        //Loop Ended
}
static void signIn()
{
sLoginCred UserDataBase ;
FILE *fp2 = NULL;
    bool status = false;
    fp2 = fopen(FILE_NAME_login,"ab+");
    if(fp2 == NULL)
    {
        printf("File is not opened\n");
        exit(0);
    }
 
   
        printf("\n\t\t\tUserName  = ");
        (void)fflush(stdin);
        (void)fgets(UserDataBase.username,MAX_SIZE_USER_NAME,stdin);
        status = isNameValid(UserDataBase.username);
        if (!status)
        {
            printf("\n\t\t\tName contain invalid character. Please enter again.");
        }
    
    
 printf("\n\t\t\tPassword = ");
    (void)fflush(stdin);
    (void)scanf("%s",&UserDataBase.password);
(void)fwrite(&UserDataBase,sizeof(UserDataBase), 1, fp2);
    (void)fclose(fp2);
    
}
//login password
static void Login()
{
    bool found = false; int L=0;
    char UserName[MAX_SIZE_USER_NAME] ;
    char PassWord[MAX_SIZE_PASSWORD] ;
    sLoginCred UserCred ;
    FILE *fp = NULL;
    
    fp = fopen(FILE_NAME_login,"ab+");
    if(fp == NULL)
    {
        printf("\n\t\t\tFile is not opened\n");
        exit(0);
    }
    headMessage("Login Page");
    
    if (fseek(fp,0,SEEK_SET) != 0)
    {
        (void)fclose(fp);
        printf("\n\t\t\tFacing issue while reading file\n");
        exit(0);
    }
    do{
		(void)fseek(fp,0,SEEK_SET);
    	printf("\n\n\t\t\tEnter User Name to search:");
    	(void)fflush(stdin);
    	(void)fgets(UserName,MAX_SIZE_USER_NAME,stdin);
    	printf("\n\t\tEnter PassWord:");
    	(void)fflush(stdin);
    	(void)fgets(PassWord,MAX_SIZE_PASSWORD, stdin);
		
		
    	while ((bool)fread (&UserCred, sizeof(UserCred), 1, fp))
    	{
			
			
			size_t len1=strlen(UserCred.username);
            size_t len2=strlen(UserCred.password);
            if((strncmp(UserCred.username, UserName,len1)==0) && (strncmp(UserCred.password, PassWord,len2)==0))
        	
        	{
            	found = true;
            	break;
        	}
    	}
    	if(found)
    	{
       		menu();
    	}
    	else
    	{
        	printf("\n\t\t\tNo Record");
		printf("\n\t\t\tLogin failed Enter Username and Password again:\n\n");
		L++;
    	}
    }
    while(L<=3);
    if(L>3)
    {
	headMessage("Login Failed");
        printf("\t\t\t\tSorry,Unknown User.");
        (void)fclose(fp);
        (void)system("cls");
    }
}
/*static int isFileExists(const char *path)
{
    // Try to open file
    FILE *fp = fopen(path, "rb");
    int status = 0;
    // If file does not exists
    if (fp != NULL)
    {
        status = 1;
        // File exists hence close file
        (void)fclose(fp);
    }
    return status;
}*/
static void Adminmenu()
{
    int choice = 0;
    do
    {
        headMessage("MAIN MENU");
		printf("\n\t\t\t1.Search Record");
        printf("\n\t\t\t2.View Records");
        printf("\n\t\t\t3.Delete Record");
		printf("\n\t\t\t4.Add Libary Book");
        printf("\n\t\t\t0.Exit");
        printf("\n\n\n\t\t\tEnter choice => ");
        (void)scanf("%d",&choice);
	(void)system("cls");
        switch(choice)
        {
		case 1:
            searchBooks();
            break;	
        case 2:
            viewBooks();
            break;
        case 3:
            deleteBooks();
            break;
		case 4:
			addBookLib();
			break;
        case 0:
            printf("\n\n\n\t\t\t\tThank you!!!\n\n\n\n\n");
            exit(0);
            
        default:
            printf("\n\n\n\t\t\tINVALID INPUT!!! Try again...");
        }                                            //Switch Ended
    }
    while(choice!=0);                                        //Loop Ended
}
static void Admin()
{
	bool found=false; int L=0;
	 char userName[MAX_SIZE_USER_NAME] ;
     char password[MAX_SIZE_PASSWORD] ;
    const char defaultUsername[] ="admin\n";
    const char defaultPassword[] ="admin123\n";
	headMessage("Login");
	do
    {
        printf("\n\n\n\t\t\t\tUsername:");
		(void)fflush(stdin);
        (void)fgets(userName,MAX_SIZE_USER_NAME,stdin);
		(void)fflush(stdin);
        printf("\n\t\t\t\tPassword:");
        (void)fgets(password,MAX_SIZE_PASSWORD,stdin);
        
        
		
		if((strcmp(defaultUsername, userName)==0) && (strcmp(defaultPassword, password)==0))
        {
            found = true;
            
        }
		if(found)
    	{
       		Adminmenu();
    	}	
		else
    	{
        	printf("\n\t\t\tNo Record");
		printf("\n\t\t\tLogin failed Enter Username and Password again:\n\n");
		L++;
    	}
    }
    while(L<=3);
    if(L>3)
    {
	headMessage("Login Failed");
        printf("\t\t\t\tSorry,Unknown User.");
        
        (void)system("cls");
    }	
}

static void init()
{
	
	int choice=0;
	do{	
	printf("\n\n\t\t\t1.Sign In as administrator");
	printf("\n\t\t\t2.Sign Up");
	printf("\n\t\t\t3.Log In");
	printf("\n\t\t\t4.Exit");
	printf("\n\t\t\tEnter choice =>");
	(void)scanf("%d",&choice);
	switch(choice)
	{
	case 1:
		Admin();
		break;
	case 2:
		signIn();
		break;
	case 3:
		Login();
		break;
	case 4: 
		printf("\n\n\n\t\t\t\tThank You!!\n\n\n");
		exit(0);
		
	default:
		printf("\n\n\n\t\t\t Inavild Iput!!!Try again...");
	}
	}while(choice!=0);
}
 int main()
{
	welcomeMessage();
    init();
    
    return 0;
}