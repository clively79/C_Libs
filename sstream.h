/****************************************************************
	A Simple C library to provide limited StringStream open and 
	get functionality similar to C++.   
*/ 

typedef struct stringstream stringstream;

struct stringstream {
	char* s;				// pointer to a string
	char* i;				// lead pointer
	char* j;				// trailing pointer
	_Bool eol;				// flag indicating *j = NULL
	_Bool error; 			// indicates an error state
};

/****************************************************************
	Given a pointer to a stringstream type that is not in an
	error state , will copy the next unread string of unread
	characters upto but not including the next SPACE in the 
	string buffer. Will return a pointer to the copied string. */ 
char* get(stringstream* s)
{
	char* t;
	int n;
	t = NULL;

	if (!s->error) {
		//search for SPACE or NULL
		for(;*s->i != SPACE && *s->i;s->i++) 
		 ;
		
		//found null terminator
		if (!*s->i){ 
			n = 0;
			t = (char*) malloc( (s->i - s->j +1)* sizeof(char));
			while(*s->j)
				t[n++] = *s->j++;

			t[n+1] = NULL;
			s->eol = TRUE;
		}
		if (*s->i == SPACE) {
			n=0;
			t = (char*) malloc( (s->i - s->j +1)* sizeof(char));
			while(*s->j != SPACE)
				t[n++] = *s->j++;

			t[n+1] = NULL;
			s->i++;
			s->j = s->i;
		}
	}
	return t;
}

/*************************************************************
	Given a pointer to a stringtream type, and a pointer to a 
	string,  will place the passed string in a buffer and set
	associated string pointers to the begginning address of
	the string buffer.  Error and EOL set to FALSE. */
void sstreamOpen(stringstream* strm, char* str){

	if (strm->eol) {
		strm->i = NULL;
		strm->j = NULL;
		free(strm->s);
		strm->s = NULL;
		strm->error = FALSE;
		strm->eol = FALSE;
	}

	if (strlen(str) > 0){
		if (str[strlen(str)] != NULL) {
			strm->s = NULL;
			strm->i = NULL;
			strm->j = NULL;
			strm->error = TRUE;
			strm->eol = TRUE;
		}
		else {
			strm->s = str;
			strm->i = strm->s;
			strm->j = strm->s;
			strm->error = FALSE;
			strm->eol = FALSE;
		}	
	}	
}