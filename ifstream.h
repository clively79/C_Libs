#ifndef TRUE
#define TRUE 1

#ifndef FALSE
#define FALSE 0

#ifndef NULL
#define NULL 0

#ifndef LF
#define LF '\n'

#ifndef SPACE
#define SPACE ' '

#ifndef IFS_MAX
#define IFS_MAX 256

typedef struct ifstream ifstream;

struct ifstream {
	int REQ;  				// number of chars to read at a time
	char* i;				// read ahead pointer
	char* j;				// trailing pointer
	char* fname;			// name of open file
	int fd; 				// file descriptor to be assigned by open() function
	int read;				// number of charecters returned by open() function
	char buffer[IFS_MAX+1]; // temprorary location to read a buffer of charecters
	_Bool eof;    			// End of file flag
	_Bool error; 			// Error state flag
	_Bool open;				// reports file open and ready.
};

/******************************************************
	openFile() is a user defined function designed to
	open a file for reading and store relevent filestream
	information in the ifstream struct.

	Arguments:
		1. address of ifstream struct
		2. pointer to file name character array
		3. mode
*/
void openFile(ifstream* file, char* filename, int n) 
{
	file->fd = open(filename, n);
	file->error = !(file->fd > 0);
	file->eof = *file->i == NULL;
	file->open = (file->fd > 0);
	file->REQ = IFS_MAX;
	
	if (file->open) {
		file->fname = filename;
		file->read = read(file->fd, file->buffer, file->REQ);
		file->buffer[IFS_MAX] = NULL;
		file->i = file->buffer;
		file->j = file->buffer;
	}
	
	else {	
		file->i = NULL;
		file->j = NULL;
	}		
	
}

/*************************************************************
	Given an ifstream type containing an open file, will close
	the file and set associated pointers to null,  and 
	aprobriate states flags to FALSE. */
void closeFile(ifstream* file)
{
	if (file->open) {
		file->i = NULL;
		file->j = NULL;
		if (file->buffer) {
		 	free(file->buffer); 
		}
		file->fd = close(file->fd);
		free(file->fname);
		file->fname = NULL;		
		file->read = 0;			
		file->eof = FALSE;    		
		file->error = FALSE; 		
		file->open = FALSE;	
	}
}

/****************************************************************
	Given a pointer to and ifstream type, if there are reamaining
	unread charecters in an open file, the existing buffer of
	characters will be over written with a new set to be read from 
	the file  */
void rebuff(ifstream* file)
{
	
	if (file->read == file->REQ){
		if ( file->j == file->i && *file->i == NULL) {
			file->read = read(file->fd, file->buffer, file->REQ);
			if (file->read==0) {
				file->eof = TRUE;
				file->i=NULL;
				file->j=NULL;
			}
			else if (file->read < file->REQ) {
				file->buffer[file->read] = NULL;
				file->i = file->buffer;
				file->j = file->buffer;
			}
			else {
				file->buffer[file->REQ]=NULL;
				file->i = file->buffer;
				file->j = file->buffer;
			}
		}
	}
	if (file->read < file->REQ && *file->j == NULL) {
		file->eof = TRUE;
		file->i=NULL;
		file->j=NULL;
	}		
}

/****************************************************
	Given a pointer to an ifstream type,  if the file 
	is open,  has not been completely read and is not 
	in an error state,  will copy the next unread 
	string of characters in the file upto but not 
	including the linefeed. a pointer to the copy of 
	the copied tring is returned; the copied string
	is NULL terminated */
char* getline(ifstream* file)
{
	char* saved;
	char* temp;
	int n;
	
	// file not open, return NULL
	if (!file->open)
		saved = NULL;

	// end of file or error state, return NULL
	else if (file->eof || file->error)
		saved = NULL;

	else {
		saved = NULL;
		temp = NULL;
		
		// scan file for LF
		if (file->open && file->read) {
			
			// search buffer for a line feed
			while(*file->i != LF) {   
			
				// skip squential linefeeds
				if(*file->i == LF && file->i - file->j < 2 ) {
					for(;*file->i != LF; file->i++)
					  ;		
					file->j = file->i;
				}

				// check for end of buffer, if reached,
				// save buffer[j..i] and rebuff
				if (*file->i == NULL) {			
					n = 0;
			
					if (!saved) {
						saved = (char*) malloc((file->i - file->j+1)*sizeof(char));
						
						while(*file->j)				 		
				 			saved[n++] = *file->j++;

						saved[n] = NULL;
						
					}
					else {
						temp = (char*) malloc((file->i - file->j+1)*sizeof(char));
			
						while(*file->j)				 		
				 			temp[n++] = *file->j++;
						
						temp[n]=NULL;
						saved = strcat(saved, temp);
						free(temp);
						temp = NULL;
					}
				}
					
				// if a NULL was found, refresh the buffer
				if (*file->i == NULL)
					rebuff(file);
					
				// only increment if no rebuff needed
				// we want the pointers to be at buffer[0]
				// after rebuff
				else{
					file->i++;
				}
			}

			// It's Business time! concattenate 
			// saved[] + buffer[i..j] and return
			// *saved   							
			n = 0;
			if (!saved) {
				saved = (char*) malloc((file->i-file->j+1)*sizeof(char));
				while(*file->j != LF)
				 	saved[n++] = *file->j++;
				
				saved[n] = NULL;
			}
			else {
				temp = (char*) malloc((file->i-file->j+1)*sizeof(char));
				while(*file->j != LF)
				 	temp[n++] = *file->j++;
				
				temp[n]=NULL;
				saved = strcat(saved, temp);
				free(temp);
				temp = NULL;			
			}
			
			// setup i & J for next call
			file->i++;
			file->j= file->i;
		}
	}
	file->eof = file->read < file->REQ && *file->j == NULL;
	return saved;
}