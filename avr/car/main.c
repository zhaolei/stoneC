/*
 Copyright:       
 Author:          
 Remarks:        
 known Problems: none
 Version:         
 Description:    
*/


#include "main.h"
 
 
 
//AVR_ATmega32
#	define USR UCSRA
#	define UCR UCSRB
#	define UBRR UBRRL
#	define EICR EICRB
 

#define SYSCLK	7372800  
#define BAUD_RATE 9600	//Baud Rate 

 
//Installation  
void IOInit (void)
{
	DDRD=(1 << DDD0);
	UCR=(1 << TXEN)|(1 << RXEN)  ;//Enable TXEN AND RXEN in Register UCR  
	UBRR=(SYSCLK / (BAUD_RATE * 16L) - 1);
}
  
//================================================================
 
//printf
void USART_Transmit( char c)
{
    if (c == '\n')
	USART_Transmit('\r');//0DH
	loop_until_bit_is_set(USR, UDRE);
	UDR = c;
}

//================================================================

unsigned char USART_Receive( void )
{
  loop_until_bit_is_set(USR, RXC);
  return UDR;
   
}


//================================================================
//================================================================
//================================================================
//================================================================
//================================================================



unsigned char Root_directory() 


{
	unsigned int Clustervar;
	unsigned char Dir_Attrib = 0;
	unsigned long Size = 0;
	unsigned char a;
    int tmp;
	unsigned char Buffer[512];

	printf("\n[Root Directory:]\n");

	for (a = 0;a < 240;a++)
	{
		Clustervar = fat_read_dir_ent(0,a,&Size,&Dir_Attrib,Buffer);
//Clustervar 起始簇
		if (Clustervar == 0xffff){break;}

		if (Size<=0x0000FFFF)
		   {
		   	tmp = (Size & 0x0000FFFF);
		   }
		   else 
		   {
		    tmp=Size/1024;
		   }
 
		printf("Cluster = %4x DirA = %2x Size= %8d  FileName = ",Clustervar,Dir_Attrib,tmp);
		printf(Buffer);
		printf("\n");
	}

	printf("\n>[Root Directory End!]\n\n>");

  

  return(0);
 }

//================================================================


unsigned char read_file(U08 *filename) 
{
	unsigned int Clustervar;
	unsigned char Dir_Attrib = 0;
	unsigned long Size = 0;
    unsigned char Buffer[512];
    int tmp;

	Clustervar = 0;//search in Root Directory
	if (fat_search_file(filename,&Clustervar,&Size,&Dir_Attrib,Buffer) == 1)
       {
        printf("\n");
		int b;
        int c;

		c=Size/BlockSize;//512

        //1010BYTE /512 = 2.

//    size (bytes) / BlockSize
//eg.  1025        /  512     = 2*512   + 1   bytes = 2 个 block 再加下一个block里的前 1   个byte
//     3456        /  512     = 6*512   + 384 bytes = 6 个 block 再加下一个block里的前 384 个byte


//总簇数 


		for (b = 0;b<=c;b++)//已读的
			{
			fat_read_file (Clustervar,Buffer,b);

			for (tmp = 0;tmp<512;tmp++)
			   {

			    if ((b==c)&&(tmp==Size%512))
				  {

				  printf("\n\n\n[the File end1 !]\n>");
				  return(0);
				  
				  }

				printf ("%c",Buffer[tmp]);
				}
			}
		}
		
		
     else
	 {

       printf("\n");

 	   printf(filename);

 	   printf("   file not Found!\n>");

 


	   return(1);
	 }
	
	 
	printf("\n\n\n[the File end !]\n>");


  return(0);
}

//================================================================

unsigned char exec_cmd (U08 *command)

{	
  	unsigned char filename[16];
    unsigned char b;
	U08 *pointer; 
	pointer = filename;
  
  switch ( command[0] )
  {
   case 'D':
		 Root_directory() ;
	     return(0);
	     break;
   case 'T':
		 for(b=2;b<16;b++)
			{
    			if ((command[b] != 13)&&	(command[b]  != SPACE))
					{
						*pointer++=command[b];
					}
			}

         read_file(filename) ; 
		 return(0);
	     break;
  }

  printf ("\n>[bad command ......]\n>");

  return(0);
}



//================================================================



//---------------------------------------------
//------------------ main ---------------------
//---------------------------------------------
 
int main (void)
{
    unsigned char Buffer[512];
    unsigned int tmp;
	unsigned char command[16];
	unsigned char rxdcnt;

	char i;
	
	IOInit();
	fdevopen (USART_Transmit,0);
  
    printf ("\n\n............. MMC/SD + FAT16 Test System ..............\n\n");	
 	printf(">Detecting MMC/SD Card ...");
	
//-------------- detect mmc/sd card ----------------
	while ( mmc_init() !=0)    
		{
	      printf(".");
		}
	
//------------- load Cluster OFFSET and Size  -------
	fat_cluster_data_store(); //  

	printf("\n\n>Found MMC/SD CARD !!!\n");


//get mmc/sd card volume_info...
	mmc_read_cid (Buffer);
	// get the name of the card 
    printf(">Card Name:");
    for (tmp = 3;tmp<8;tmp++){printf("%c",Buffer[tmp]);};
    printf("\n");

	
    unsigned char sector_multiply;
    int sector_count,size_MB;

	mmc_read_csd (Buffer);
 
	sector_count = Buffer[6] & 0x03;
	sector_count <<= 8;
	sector_count += Buffer[7];
	sector_count <<= 2;
	sector_count += (Buffer[8] & 0xc0) >> 6;
		
	// get the val for C_SIZE_MULT. bits [49:47] of sectorBuffer.data
	// [49:48] == sectorBuffer.data[5] && 0x03
	// [47]    == sectorBuffer.data[4] && 0x80
	sector_multiply = Buffer[9] & 0x03;
	sector_multiply <<= 1;
	sector_multiply += (Buffer[10] & 0x80) >> 7;

	// work out the MBs
	// mega bytes in u08 == C_SIZE / (2^(9-C_SIZE_MULT))
	size_MB = sector_count >> (9-sector_multiply);

    printf(">Card Size:");
	printf("%d",size_MB);
    printf("MB\n>"); 

    
//----------------------------------
//-------- Root Directory ----------
//----------------------------------  

 U08 j;

	while(1)
		{
		i=USART_Receive();
		USART_Transmit(i); 
        

		if (rxdcnt<=16)
		   {
		      command[rxdcnt]=i;
		   }

         if (i == '\r')  //0DH
		    {
			 
			  exec_cmd(command);
              rxdcnt=0;


             for (j = 0;j < 16;j++){command[j]=0; }

			  	
			}else {rxdcnt++;}
			


  
		}

/*


			
			*/


/*
	unsigned int Clustervar;
	unsigned char Dir_Attrib = 0;
	unsigned long Size = 0;
	unsigned char a;

	printf("\n[Root Directory] \n\n");

	for (a = 0;a < 240;a++)
	{
		Clustervar = fat_read_dir_ent(0,a,&Size,&Dir_Attrib,Buffer);
//Clustervar 起始簇
		if (Clustervar == 0xffff){break;}

		if (Size<=0x0000FFFF)
		   {
		   	tmp = (Size & 0x0000FFFF);
		   }
		   else 
		   {
		    tmp=Size/1024;
		   }
 
		printf("Cluster = %4x DirA = %2x Size= %8d  FileName = ",Clustervar,Dir_Attrib,tmp);
		printf(Buffer);
		printf("\n");
	}

	printf("\n>Root Directory End!\n\n");
*/
	 
	
/*
	printf("find file ...\n\n");

	 

		unsigned int Clustervar;
	unsigned char Dir_Attrib = 0;
	unsigned long Size = 0;
 


	Clustervar = 0;//search in Root Directory
	if (fat_search_file("c.TXT",&Clustervar,&Size,&Dir_Attrib,Buffer) == 1)
	
		{
		
		printf("the File Found!!\n\n");
		printf("FAT.TXT:\n\n");
		
		//Lese File und gibt es auf der seriellen Schnittstelle aus

 
        printf("\n");


		int b;
        int c;
		c=Size/512;


		for (b = 0;b<c+1;b++)//已读的Bock
			{
			fat_read_file (Clustervar,Buffer,b);

			for (tmp = 0;tmp<512;tmp++)
			   {
				printf ("%c",Buffer[tmp]);
				}
			}
		}
		
		
     else
	 {
	   printf("the File not Found!!\n\n");
	 }
	
	
	
	 
	printf("...");

*/
	while (1)
		{
		i=USART_Receive();
		USART_Transmit(i);
		if (i == '0')
		{
		printf("  sector  \n");
		
		
	    mmc_read_sector (0,Buffer);
		
		
	i=0;
	for (tmp = 0;tmp<512;tmp++)
		{
		 
					
		if (i == 16)
			{
			 i=0;
             printf("\n");
			}
			
		if (i == 0)
			{
			 
             printf("%04X:  ",tmp);
			}
			
		i++;
		
		printf("%02X ",Buffer[tmp]);
		
		};


		printf("\n");
	
		
		}
		
		
		
		if (i == '1')
		{
		printf("  sector  \n");
		
		
	    mmc_read_sector (1,Buffer);
		
		
	i=0;
	for (tmp = 0;tmp<512;tmp++)
		{
		 
					
		if (i == 16)
			{
			 i=0;
             printf("\n");
			}
			
		if (i == 0)
			{
			 
             printf("%04X:  ",tmp);
			}
			
		i++;
		
		printf("%02X ",Buffer[tmp]);
		
		};


		printf("\n");
	
		
		}
		
		
		
		}
	
	
	
		
		
	
	
	
	while (1)
		{
		 
		//USART_Transmit('as');
		i=USART_Receive();
		
 	    USART_Transmit(i);
		
		
		 
		}
return (1);
}


/*

    U08 i;
	int tmp;
    i=0;
	printf("\n");
	for (tmp = 0;tmp<512;tmp++)
		{
		if (i == 16)
			{
			 i=0;
             printf("\n");
			}
			
		if (i == 0)
			{
			 
             printf("%04X:  ",tmp);
			}
			
		i++;
		
		printf("%02X ",Buffer[tmp]);
		
		};

 printf("\n");

   for (;;);



*/


