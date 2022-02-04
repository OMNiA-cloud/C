//
// LZW data compression/expansion demonstration program.
// http://marknelson.us/1989/10/01/lzw-data-compression/
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int BITS;  
int	TABLE_SIZE;

#define HASHING_SHIFT	(BITS-8)       
#define MAX_VALUE		(1 << BITS) - 1  
#define MAX_CODE		MAX_VALUE - 1 

// compTest(in), compress(in, out), expand(in, out) KOSMADAN ONCE
// ASAGIDAKI 3 DEGISKEN main()'de ILGILI YERLERDE SETLENMELIDIR.
// SETLEMELERI LZW'DAN KOPYALAYABILIRSINIZ (94-96. SATIRLAR ARASI).
int				*code_value;                          
unsigned int	*prefix_code;        
unsigned char	*append_character;    
//////////////////////////////////////////////////////////

unsigned char decode_stack[4000]; // This array holds the decoded string 

bool			compTest(FILE *input);
void			compress(FILE *input,FILE *output);
void			expand(FILE *input,FILE *output);
int				find_match(int hash_prefix,unsigned int hash_character);
void			output_code(FILE *output,unsigned int code);
unsigned int	input_code(FILE *input);
unsigned char	*decode_string(unsigned char *buffer,unsigned int code);


void main(int argc, char *argv[])
{
	system("cls");	
	clock_t start = clock();

	if (argc != 4)
	{
		printf("\nUSAGE : Optimum c/d input output		(c:compress, d:decompress)\n");
		exit(1);
	}

	int TABLESIZE[26] = {
		317, 317, 317, 317, 317, 317, 317, 317,					// BITS = 0..7 
		317, 631, 1279, 2557, 5119, 10223, 20479, 40949,		// BITS = 8..15
		81919, 163819, 327673, 655357, 1310719, 2621431,		// BITS = 16..21
		5242877, 10485751, 20971507, 41943023					// BITS = 22..25
	};

	BITS = 12;

	FILE* in  = fopen(argv[2], "rb");				
	FILE* out = fopen(argv[3], "wb");	

	if (argv[1][0]=='c')
	{																												// MERT UÇAK-365378
		while (1)
		{
			// BURADA TABLE_SIZE ve YUKARIDAKI 3 DEGISKENI (code_value, prefix_code, append_character) 
			// O ANKI BITS'E GORE SETLEDIKTEN SONRA compTest(in) FONKSIYONU false DONDURENE 
			// (DICTIONARY TABLE'IN ILK BOS KALDIÐI DEGERE) KADAR BITS++ YAPARAK KOSUNUZ.
			// HERBIR compTest(in) ONCESI in = fopen(argv[2], "rb"); YAZMAKTA FAYDA VARDIR.
			// TABLE_SIZE O ANKI BITS'E GORE TABLESIZE[BITS] ILE SETLENECEKTIR. 


			TABLE_SIZE = TABLESIZE[BITS];
			code_value = new int[TABLE_SIZE];
			prefix_code = new unsigned int[TABLE_SIZE];
			append_character = new unsigned char[TABLE_SIZE];
			in = fopen(argv[2], "rb");
			if (compTest(in) == false)
				break;
			else BITS++;
		}


		// BURADA OPTIMUM BITS'I, DICTIONARY TABLE'IN SON DOLDUGU DEGER OLARAK (BITS-1) bits.txt'YE YAZINIZ.
		// SONRA TABLE_SIZE ve 3 DEGISKENI SETLEYIP compress(in, out) ILE DOSYAYI KODLAYINIZ.
		// KODLAMA ONCESI YINE in = fopen(argv[2], "rb"); YAZMAKTA FAYDA VARDIR.

		FILE* yaz = fopen("bits.txt", "w");
		fprintf(yaz, "%d", BITS - 1);
		fclose(yaz);
		TABLE_SIZE = TABLESIZE[BITS];
		code_value = new int[TABLE_SIZE];
		prefix_code = new unsigned int[TABLE_SIZE];
		append_character = new unsigned char[TABLE_SIZE];
		BITS--;
		in = fopen(argv[2], "rb");
		compress(in, out);
		printf("\n\n COMPRESSING FROM %s WHICH IS %ld BYTES \n TO %s WHICH IS %ld BYTES TAKES = %1.2f sn. \n\n COMPRESSION RATIO = %4.2f\n",
			argv[2], ftell(in), argv[3], ftell(out), double(clock() - start) / CLOCKS_PER_SEC, 100 * (float)ftell(out) / (float)ftell(in));


	}
	else
	{

		// BURADA OPTIMUM BITS DEGERINI bits.txt'DEN OKUYUNUZ.
		// TABLE_SIZE ve 3 DEGISKENI OKUNAN BITS'E GORE SETLEDIKTEN 
		// SONRA expand(in, out) ILE DOSYAYI DEKODLAYINIZ. 
		int bit_temp = 0;
		FILE* oku = fopen("bits.txt", "r");
		fscanf(oku, "%d", &bit_temp);
		BITS = bit_temp;
		TABLE_SIZE = TABLESIZE[BITS];
		code_value = new int[TABLE_SIZE];
		prefix_code = new unsigned int[TABLE_SIZE];
		append_character = new unsigned char[TABLE_SIZE];
		expand(in, out);

	}
	// PROGRAM KOMUT SATIRINDAN KOSULACAGINDAN F5'E BASMAYINIZ. BUILD YAPIP .EXE OLUSTURUNUZ !!!
}

bool compTest(FILE *input)
{
	unsigned int next_code;
	unsigned int character;
	unsigned int string_code;
	unsigned int index;

	next_code = 256;								// Next code is the next available string code  
	for (int i=0;i<TABLE_SIZE;i++)					// Clear out the string table before starting 
		code_value[i]=-1;


	printf("Testing for BITS = %d\n", BITS);
	string_code=getc(input);						// Get the first code    

	// This is the main loop where it all happens.  
	// This loop runs util all of the input has been exhausted. 
	// Note that it stops adding codes to the table after all of the possible codes have been defined.	  
	while ((character=getc(input)) != (unsigned)EOF)
	{
		index=find_match(string_code,character);	// See if the string is in   
		if (code_value[index] != -1)				// the table.  If it is,   
			string_code=code_value[index];			// get the code value.  If   
		else										// the string is not in the
		{											// table, try to add it.    
			if (next_code <= MAX_CODE)
			{
				code_value[index]=next_code++;
				prefix_code[index]=string_code;
				append_character[index]=character;
			}
			else
				return true;							// Dictionary tablosu doldu !!!

			string_code=character;
		}											// I output the last string  
	}												// after adding the new one
	
	printf("\nDictionary Table is not filled for BITS = %d\n", BITS);
	printf("Optimum BITS = %d if last filled value is used\n", BITS-1);
	printf("Optimum BITS = %d if first not filled value is used\n", BITS);
	printf("This program uses the first approach so the Optimum BITS = %d \n", BITS-1);

	return false;
}

void compress(FILE *input, FILE *output)
{
	unsigned int next_code;
	unsigned int character;
	unsigned int string_code;
	unsigned int index;

	next_code = 256;              // Next code is the next available string code  
	for (int i=0;i<TABLE_SIZE;i++)  // Clear out the string table before starting 
		code_value[i]=-1;


	printf("\nCompressing with BITS = %d ...  \n", BITS);
	string_code=getc(input);						// Get the first code    


	// This is the main loop where it all happens.  
	// This loop runs util all of the input has been exhausted. 
	// Note that it stops adding codes to the table after all of the possible codes have been defined.	  
	while ((character=getc(input)) != (unsigned)EOF)
	{
		index=find_match(string_code,character);	// See if the string is in   
		if (code_value[index] != -1)				// the table.  If it is,   
			string_code=code_value[index];			// get the code value.  If   
		else										// the string is not in the
		{											// table, try to add it.    
			if (next_code <= MAX_CODE)
			{
				code_value[index]=next_code++;
				prefix_code[index]=string_code;
				append_character[index]=character;
			}

			output_code(output,string_code);		// When a string is found    
			string_code=character;					// that is not in the table
		}											// I output the last string  
	}												// after adding the new one

	output_code(output,string_code);				// Output the last code                 
	output_code(output,MAX_VALUE);					// Output the end of buffer code    
}


// This is the hashing routine.  It tries to find a match for the prefix+char string in the string table.
// If it finds it, the index is returned.  
// If the string is not found, the first available index in the string table is returned instead.
int find_match(int hash_prefix, unsigned int hash_character)
{
	int index;
	int offset;

	index = (hash_character << HASHING_SHIFT) ^ hash_prefix;

	if (index == 0)
		offset = 1;
	else
		offset = TABLE_SIZE - index;

	while (1)
	{
		if (code_value[index] == -1)
			return(index);
		if (prefix_code[index] == hash_prefix && append_character[index] == hash_character)
			return(index);

		index -= offset;
		if (index < 0)
			index += TABLE_SIZE;
	}
}
  
// This is the expansion routine.  It takes an LZW format file, and expands it to an output file.
// The code here should be a fairly close match to the algorithm in the accompanying article.
void expand(FILE *input, FILE *output)
{
	unsigned int next_code;
	unsigned int new_code;
	unsigned int old_code;
	int character;
	unsigned char *string;

	next_code=256;							// This is the next available code to define   
         
	printf("\nExpanding... \n");

	old_code	= input_code(input);		// Read in the first code, initialize the   
	character	= old_code;					// character variable, and send the first 
	putc(old_code,output);					// code to the output file                  
	  
	// This is the main expansion loop.  It reads in characters from the LZW file
	// until it sees the special code used to inidicate the end of the data.
	while ((new_code=input_code(input)) != (MAX_VALUE))
	{		  
		// This code checks for the special STRING+CHARACTER+STRING+CHARACTER+STRING
		// case which generates an undefined code.  It handles it by decoding
		// the last code, and adding a single character to the end of the decode string.	
		if (new_code >= next_code)
		{
			*decode_stack = character;
			string=decode_string(decode_stack+1,old_code);
		}		
		else					//	Otherwise we do a straight decode of the new code.
			string=decode_string(decode_stack,new_code);

		// Now we output the decoded string in reverse order.
		character = *string;
		while (string >= decode_stack)
			putc(*string--,output);
		
		// Finally, if possible, add a new code to the string table.
		if (next_code <= MAX_CODE)
		{
			prefix_code[next_code]		= old_code;
			append_character[next_code] = character;
			next_code++;
		}
		old_code=new_code;
	}
}

// This routine simply decodes a string from the string table, storing it in a buffer.
// The buffer can then be output in reverse order by the expansion program.
unsigned char *decode_string(unsigned char *buffer,unsigned int code)
{
	int i;

	i=0;
	while (code > 255)
	{
		*buffer++ = append_character[code];
		code=prefix_code[code];
		if (i++>=MAX_CODE)
		{
			printf("Fatal error during code expansion.\n");
			exit(-3);
		}
	}
	*buffer=code;
	return(buffer);
}

// The following two routines are used to output variable length codes.
// They are written strictly for clarity, and are not particularyl efficient.
unsigned int input_code(FILE *input)
{
	unsigned int return_value;
	static int input_bit_count=0;
	static unsigned long input_bit_buffer=0L;

	while (input_bit_count <= 24)
	{
		input_bit_buffer |= (unsigned long) getc(input) << (24-input_bit_count);
		input_bit_count += 8;
	}

	return_value		=	input_bit_buffer >> (32-BITS);
	input_bit_buffer	<<= BITS;
	input_bit_count		-=	BITS;
	return(return_value);
}

void output_code(FILE *output,unsigned int code)
{
	static int output_bit_count=0;
	static unsigned long output_bit_buffer=0L;

	output_bit_buffer	|= (unsigned long) code << (32-BITS-output_bit_count);
	output_bit_count	+= BITS;

	while (output_bit_count >= 8)
	{
		putc(output_bit_buffer >> 24,output);
		output_bit_buffer	<<= 8;
		output_bit_count	-=  8;
	}
}