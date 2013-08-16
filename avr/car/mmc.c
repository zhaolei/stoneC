//=====================================================================



//=====================================================================
#include "mmc.h"
//=====================================================================

//---------------------------------------------------------------------
//Init MMC/SD card(SPI-MODE)
//---------------------------------------------------------------------
U08 mmc_init ()
{
	U08 Timeout = 0;
	MMC_Direction_REG &=~(1<<SPI_DI);			//MMC_DI    Input
	MMC_Direction_REG |= (1<<SPI_Clock);		//MMC_Clock Output
	MMC_Direction_REG |= (1<<SPI_DO);			//MMC_DO    Output
	MMC_Direction_REG |= (1<<MMC_Chip_Select);	//MMC_CS    Output
	MMC_Direction_REG |= (1<<SPI_SS);	
	MMC_Write |= (1<<MMC_Chip_Select);			//MMC_CS    HIGH
    
	unsigned char a;
    for(a=0;a<200;a++){nop(); };		//Wait ......

	#if SPI_Mode   //SPI INIT
		SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPR1); //Enable SPI, SPI in Master Mode	
		SPSR = (0<<SPI2X);
	#endif
	for (a = 0;a<0x0f;a++){mmc_write_byte(0xff);}//wait  >74  Clk
	
	//--------- send CMD0  ---------
	U08 CMD[] = {0x40,0x00,0x00,0x00,0x00,0x95};
	while(mmc_write_command (CMD) !=1)
	{
		if (Timeout++ > 200)
		   { return(1); }// CMD0 Error!
	}
    //--------- send CMD1  ---------
	Timeout = 0;
	CMD[0] = 0x41;
	CMD[5] = 0xFF;
	while( mmc_write_command (CMD) !=0)
	{
		if (Timeout++ > 100)
			{
			return(2); //CMD1 Error!
			}
	}
	#if SPI_Mode    // SPI clk 
		SPCR &= ~((1<<SPR0) | (1<<SPR1));
		SPSR = SPSR|(1<<SPI2X);
	#endif
	MMC_Disable();//MMC_CS = high
	return(0);
}

//---------------------------------------------------------------------
//Send a Command to MMC/SD-Card
//Return: the second byte of response register of MMC/SD-Card
//---------------------------------------------------------------------
U08 mmc_write_command (U08 *cmd)
{
	U08 tmp = 0xff;
	U16 Timeout = 0;

	MMC_Disable();        //MMC_CS = HIGH
	mmc_write_byte(0xFF); //send 8 Clock Impulse
	MMC_Enable();         //MMC_CS = LOW   

	//send 6 Byte Command
    U08 a;
	for (a = 0;a<0x06;a++)  
		{
		  mmc_write_byte(*cmd++);
		}

	//get 16 bit response
	while (tmp == 0xff)	
		{
		tmp = mmc_read_byte();  //Only last 8 bit is used here.Read it out. 
		if (Timeout++ > 500){break;}
		}
	return(tmp);
}

//---------------------------------------------------------------------
//Routine for reading a byte from MMC/SD-Card
//---------------------------------------------------------------------
inline U08 mmc_read_byte (void)
{
	U08 Byte = 0;

    #if SPI_Mode	 
	  SPDR = 0xff;
	  while(!(SPSR & (1<<SPIF))){};
	  Byte = SPDR;
    #else			//Software SPI
	
	  for (U08 a=8; a>0; a--) //1 Byte  ( MSB First)
	      {
             MMC_Write &=~(1<<SPI_Clock); //MMC_SCK (Low) 
		     if (bit_is_set(MMC_Read,SPI_DI) > 0)  
		        {
			    	Byte |= (1<<(a-1));
			     }
			  else
			     {
			         Byte &=~(1<<(a-1));
				  }
		       MMC_Write |=(1<<SPI_Clock); //MMC_SCK (High)		
		  }
    #endif
	return (Byte);
}

//---------------------------------------------------------------------
//Routine for sending a byte to MMC/SD-Card
//---------------------------------------------------------------------
inline void mmc_write_byte (U08 Byte)
{
    #if SPI_Mode		 
	  SPDR = Byte; 	 
	  while(!(SPSR & (1<<SPIF))) {}
    #else			
	  for (U08 a=8; a>0; a--) // 1 Byte ( MSB First)
	    {
		  if (bit_is_set(Byte,(a-1))>0)	{MMC_Write |= (1<<SPI_DO); }//MMC.DO  High
		  else {MMC_Write &= ~(1<<SPI_DO);}//MMC.DO  Low
		  MMC_Write &= ~(1<<SPI_Clock);	//MMC.SCK  (LOW)
		  MMC_Write |= (1<<SPI_Clock);   //MMC.SCK  (High)
	     }
	   MMC_Write |= (1<<SPI_DO);	       //MMC.DO  High
    #endif
}

//---------------------------------------------------------------------
//Blocks(512Byte) 
//Routine for writing a Block(512Byte) to MMC/SD-Card
//---------------------------------------------------------------------
U08 mmc_write_sector (U32 addr,U08 *Buffer)
{
	unsigned char tmp;
	unsigned char cmd[] = {0x58,0x00,0x00,0x00,0x00,0xFF}; //CMD 24  

	addr = addr << 9; //addr = addr * 512
	
	cmd[1] = ((addr & 0xFF000000) >>24 );
	cmd[2] = ((addr & 0x00FF0000) >>16 );
	cmd[3] = ((addr & 0x0000FF00) >>8 );

	//Send Command CMD24 to MMC/SD-Card (Write 1 Block/512 Bytes)
	tmp = mmc_write_command (cmd);
	if (tmp != 0) { return(tmp); }  ////send commamd Error!

	//Before writing,send 100 clock to MMC/SD-Card
	unsigned char  a;

	for (a=0;a<100;a++)
		{
		mmc_read_byte();
		}
 
 	mmc_write_byte(0xFE);//Send Start Byte to MMC/SD-Card	


	//Now send real data Bolck (512Bytes) to MMC/SD-Card
    int   b;
    for (b=0;b<512;a++)
		{
		mmc_write_byte(*Buffer++);
		}
	
	//CRC-Byte  
	mmc_write_byte(0xFF); //Dummy CRC
	mmc_write_byte(0xFF); //CRC Code 
	
	//Wait till MMC/SD-Card is not busy
	while (mmc_read_byte() != 0xff){};
	
	//set MMC_CS    high 
	MMC_Disable();
	
return(0);
}

//---------------------------------------------------------------------
//Routine for reading data Registers of MMC/SD-Card
//Return 0 if no Error.
//---------------------------------------------------------------------

void mmc_read_block(U08 *cmd,U08 *Buffer,U16 Bytes)
{	
	//Send CMD0
	if (mmc_write_command (cmd) != 0)
			{
			 return;//block write Error!
			}

	//Read Start Byte form MMC/SD-Card (FEh/Start Byte)
	while (mmc_read_byte() != 0xfe){};

	//Write blocks(normal 512Bytes) to MMC/SD-Card
    int   a;
	for (a=0;a<Bytes;a++)
		{
		*Buffer++ = mmc_read_byte();
		}
	//CRC-Byte  
	mmc_read_byte();//CRC - Byte 
	mmc_read_byte();//CRC - Byte 
	
	MMC_Disable();//MMC_CS = high (MMC/SD-Card no active )
	
	return;
}

//---------------------------------------------------------------------
//Routine for reading Blocks(512Byte) from MMC/SD-Card
//Return 0 if no Error.
//---------------------------------------------------------------------
U08 mmc_read_sector (U32 addr,U08 *Buffer)
{	
	//Command 16 is reading Blocks from MMC/SD-Card
	U08 cmd[] = {0x51,0x00,0x00,0x00,0x00,0xFF}; 
	
	//Address conversation(logic block address-->byte address)  
	addr = addr << 9; //addr = addr * 512

	cmd[1] = ((addr & 0xFF000000) >>24 );
	cmd[2] = ((addr & 0x00FF0000) >>16 );
	cmd[3] = ((addr & 0x0000FF00) >>8 );

    mmc_read_block(cmd,Buffer,512);

	return(0);
}

//---------------------------------------------------------------------
//Routine for reading CID Registers from MMC/SD-Card (16Bytes) 
//Return 0 if no Error.
//---------------------------------------------------------------------
U08 mmc_read_cid (U08 *Buffer)
{
	U08 cmd[] = {0x4A,0x00,0x00,0x00,0x00,0xFF}; //Command for reading CID Registers
	mmc_read_block(cmd,Buffer,16);
	return(0);
}

//---------------------------------------------------------------------
//Routine for reading CSD Registers from MMC/SD-Card (16Bytes)
//Return 0 if no Error.
//---------------------------------------------------------------------
U08 mmc_read_csd (U08 *Buffer)
{	
	U08 cmd[] = {0x49,0x00,0x00,0x00,0x00,0xFF}; //Command for reading CSD Registers
	mmc_read_block(cmd,Buffer,16);
	return(0);
}
 

//---------------------------------------------------------------------


//---------------------------------------------------------------------

//---------------------------------------------------------------------


//---------------------------------------------------------------------


//---------------------------------------------------------------------

//---------------------------------------------------------------------


//---------------------------------------------------------------------





