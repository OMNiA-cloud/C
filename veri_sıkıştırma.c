#include <stdio.h>
#include <conio.h>
#include <stdlib.h>                         

void aaa2byte()
{
	system("cls");

	int a=0, byte = 0;
	int toplam = 0; 
	int file_byte; //dosya boyutu
	int x = 1; //sayac
	int y = 0; //çıktı
	int cıktı = 0;
	printf("\nGIRIS DOSYASINA YAZILAN a 'LARIN SAYISI = ");
	scanf("%d", &a);

	printf("a 'LAR KAC BYTE ILE KODLANACAK ? (2,3,4,...) = ");
	scanf("%d", &byte);

	
	while (toplam<a) {
		sum += x;
		x++;
		y++;
	}
	file_byte = (y + 1) * byte;

	printf("\nLZW KODLANMIS DOSYANIN BOYU %d BYTETIR.", file_byte); 

	getchar(); getchar();
}

void byte2aaa()
{
	system("cls");

	int i=0, byte = 0;
	int z; //cıktı veri sayısı
	int max_data ;
	printf("\nLZW KODLANMIS DOSYANIN BYTE CINSINDEN BOYU = ");
	scanf("%d", &i);

	printf("a 'LAR KAC BYTE ILE KODLANMISTI ? (2,3,4,...) = ");
	scanf("%d", &byte);

	z= (i / byte) - 1;
	max_data = z * (z + 1) / 2;



	printf("\nGIRIS DOSYASINA YAZILABILECEK MAX a SAYISI = %d DIR", max_data ); 

	getchar(); getchar();
}


void main(int argc, char* argv[])
{
	char s;

	do
	{
		system("cls");

		printf(" \n");
		printf(" aaa... LARDAN OLUSAN DOSYA LZW KODLANIRKEN :\n\n");
		printf(" [1] aaa... SAYISI VERILDIGINDE LZW KODLANMIS DOSYANIN BOYU \n");
		printf(" [2] KODLANMIS DOSYA BOYU ICIN GIRIS DOSYASI MAX a... SAYISI \n");
		printf(" [3] PROGRAMDAN CIKIS\n\n");
		printf(" SECIMINIZ [1..3] = ");

		s=getch();

		switch(s)
		{
			case '1' :	aaa2byte();				break;
			case '2' :	byte2aaa();				break;
			case '3' :	return;					break;
		}
		
	}  while(1);

	return;
}