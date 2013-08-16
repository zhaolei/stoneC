
//=====================================================================
//FAT for AVR(MMC/SD) 
//=====================================================================

//---------------------------------------------------------------------
//    _______________________________________
//   |      |         |     |      |      | |
//   |  DBR | RsvdSec | FAT | FDT  | DATA | |
//   |______|_________|_____|______|______|_|

//    启动扇区  保留扇区 FAT区 根目录区  数据区


//根目录开始扇区 = DBR的扇区地址  +     保留扇区 +  FAT数 *  每FAT扇区数


//数据区开始簇号 = 目录扇区总数+ 根目录开始扇区号


//---------------------------------------------------------------------
#include "fat.h"

//---------------------------------------------------------------------
U08 cluster_size;             //每簇扇区数
U16 fat_offset;               //保留扇区数
U16 cluster_offset;           //数据区开始簇号
U16 volume_boot_record_addr;  //DBR的扇区地址


//---------------------------------------------------------------------
//Selections that Adresse of the Volume Boot Record on MBR
//根据MBR计算DBR的扇区地址
//---------------------------------------------------------------------
U16 fat_addr (U08 *Buffer)
{
	U16 volume_boot_record_addr;
	mmc_read_sector (MASTER_BOOT_RECORD,Buffer);      //read MBR to buffer
    volume_boot_record_addr = Buffer[VBR_ADDR] + (Buffer[VBR_ADDR+1] << 8);  
 	//Computed Volume Boot Record 
	mmc_read_sector (volume_boot_record_addr,Buffer); //Read DBR to buffer
    return (volume_boot_record_addr);
}
//---------------------------------------------------------------------
//获取根目录开始扇区号
//---------------------------------------------------------------------

U16 fat_root_dir_addr (U08 *Buffer)    
{
	struct BootSec *bootp; //bootp : the structure of DBR
	U16 FirstRootDirSecNum;
	//Selections of the Volume Boot Record of that MMC/SD Karte 
	mmc_read_sector (volume_boot_record_addr,Buffer);
	bootp=(struct BootSec *)Buffer;

	//Computed that first Sector of the Root Directory
	FirstRootDirSecNum = ( bootp->BPB_RsvdSecCnt +
	                       (bootp->BPB_NumFATs * bootp->BPB_FATSz16));
                         //保留扇区数 +  FAT个数 *  每FAT扇区数
	
	FirstRootDirSecNum+= volume_boot_record_addr;//根目录开始扇区 =  保留扇区数 +  FAT个数 *  每FAT扇区数 
	                                                              // + DBR的扇区地址
	
	return(FirstRootDirSecNum);

}

//---------------------------------------------------------------------
//	Expenditure of the indicated Directory Entry in Entry_Count
//	is none Entry available, is that Entry in 
//	...kgabe Cluster 0xFFFF.it becomes always only ein Entry spent
//	over Storage location too save umit also f黵 small Atmels too use

//读FDT 根目录中文件名  属性  起始簇 ....
//---------------------------------------------------------------------

U16 fat_read_dir_ent (U16 dir_cluster, //Dir Cluster     //0   
					U08 Entry_Count,   //which Dir Entry 已经读到第几个目录结构数据   
					U32 *Size, 		   //the size of File or directory
					U08 *Dir_Attrib,   //the Attributs of File or directory
					U08 *Buffer) 	   //Working Buffer 
{
	U08 *pointer;
	U16 TMP_Entry_Count = 0;
	U32 Block = 0;
	struct DirEntry *dir; //dir...struct

	pointer = Buffer;

	if (dir_cluster == 0)
		{
		Block = fat_root_dir_addr(Buffer);//获取根目录开始扇区号
		}
	else
		{
		//computation of the Blocks out Block Count and Cluster out FAT Tabelle
		//computation which Cluster too load is
		//Selections that FAT - Tabelle


		//???????????
		fat_load (dir_cluster,&Block,Buffer);	
				 
		Block = ((Block-2) * cluster_size) + cluster_offset;  //目录扇区号= (目录扇区号-2) * 每簇扇区数  +  数据区开始簇号

		}

	//Selections of the Entry Root Directory
    U16 blk;
	for (blk = Block;;blk++)
	{
		mmc_read_sector (blk,Buffer);	//one Blocks of the Root Directory  512 bytes

        U16 a;
		 
		for (a=0;a<BlockSize; a = a + 32)  //BlockSize=512
		{
		 dir=(struct DirEntry *)&Buffer[a]; //Pointer up current Listing Entry get
		 
			if (dir->DIR_Name[0] == 0) //name=00H 表示此项未用
			{
			return (0xFFFF);
			}
			
			//Pr黤en obit ein 8.3 Entry is
			//that is that case if it itself not over one Entry f黵 are enough  File name
			//or over one as gel鰏cht marked Entry acts.

   			if ((dir->DIR_Attr != ATTR_LONG_NAME) &&     //不是长文件
				(dir->DIR_Name[0] != DIR_ENTRY_IS_FREE)) //没有删除.
			{
				//is it that gew黱schte Listing Entry
				if (TMP_Entry_Count == Entry_Count) 
				{
					//store of the Listing Entryes in that R點kgabe Buffer
                    U08 b;
					for(b=0;b<11;b++)
					{
					if (dir->DIR_Name[b] != SPACE)
						{
						if (b == 8)
							{
							*pointer++= '.';
							}
						*pointer++=dir->DIR_Name[b];
						}
					}						
					*pointer++='\0';


					*Dir_Attrib = dir->DIR_Attr;//the Attributs of File or directory
					 
					*Size=dir->DIR_FileSize;// the size of file
					
					dir_cluster = dir->DIR_FstClusLO;//the first cluster(low)
					 
					return(dir_cluster);//返回起始簇
				}
			TMP_Entry_Count++;
			}
		}
	}
	return (0xFFFF); //none Entry more found return with 0xFFFF
}
//---------------------------------------------------------------------
//	Selections that Cluster f黵 ein File out that FAT
//	in that Buffer(512Byte). one 128MB MMC/SD 
//	Karte is those Cluster gr鲞e normal proves 16KB gro�
//	that meant that File can max. 4MByte gro�  its
//	that gr鲞eren Files mu� der Buffer gr鲞er definiert
//	become! (Ready)
//	Cluster = Start Cluster Indication out that Directory	
//根据首簇或上一簇  读FAT表中下一簇
//---------------------------------------------------------------------
void fat_load (	U16 Cluster, 		//Start cluster
				U32 *Block,         //需要读取的簇
				U08 *TMP_Buffer) 	//Working buffer
{
	//to 躡erpr黤en ob that FAT Block already loaded became
	U16 FAT_Block_Store = 0;	

	//Byte Adresse within of the Fat Blocks
	U16 FAT_Byte_Addresse;	

	//FAT Block Adresse
	U16 FAT_Block_Addresse;
	
	//computation that first FAT Block (FAT Start Addresse)
    U16 a;
	for (a = 0;;a++)
	{	
		if (a == *Block)
			{
			*Block = (0x0000FFFF & Cluster);
			return;
			}
		
		if (Cluster == 0xFFFF)
			{
			break; //is that Ende of the Files 
			}
		//computation of the Bytes within of the FAT Block
		FAT_Byte_Addresse = (Cluster*2) % BlockSize;//BlockSize=512 //开始位置所在扇区簇内偏移
        //根据目录表32字节里的起始簇 计算 FAT表中的簇号在FAT的地址

//目录表中 文件或目录 开始位置 =  起始簇 * 2 / 512 的余数
 
			
		//computation of the Blocks that read become mu�
		FAT_Block_Addresse = ((Cluster*2) / BlockSize) + volume_boot_record_addr + fat_offset;	


		//Vintages of the FAT Blocks
		//躡erpr黤ung ob this Block already read became
		if (FAT_Block_Addresse != FAT_Block_Store)
			{
			FAT_Block_Store = FAT_Block_Addresse;
			//Vintages of the FAT Blocks
			mmc_read_sector (FAT_Block_Addresse,TMP_Buffer);	
			}

		//取下个簇号
		Cluster = (TMP_Buffer[FAT_Byte_Addresse + 1] << 8) + TMP_Buffer[FAT_Byte_Addresse];	
			
	}
	return;
}

//---------------------------------------------------------------------
//数据区开始簇号
//---------------------------------------------------------------------

void fat_cluster_data_store (void)
{
 	struct BootSec *bootp; //Boot sector Struct

	U08 Buffer[BlockSize]; //BlockSize=512

	volume_boot_record_addr = fat_addr (Buffer); //DBR的扇区地址
  
 	mmc_read_sector (volume_boot_record_addr,Buffer);//读取DBR数据
 
    bootp=(struct BootSec *)Buffer;

	cluster_size   = bootp->BPB_SecPerClus;   //每簇扇区数
	fat_offset     = bootp->BPB_RsvdSecCnt;   //保留扇区数
	cluster_offset = ((bootp->BPB_BytesPerSec * 32)/BlockSize);//目录扇区总数=BPB.根目录项数*32/512
	cluster_offset += fat_root_dir_addr(Buffer);//数据区开始簇号= 目录扇区总数+ 根目录开始扇区号



}

//---------------------------------------------------------------------
//Vintages one 512Bytes Blocks of one File
//---------------------------------------------------------------------
void fat_read_file (U16 Cluster,//簇
				 U08 *Buffer,	  //data buffer
				 U32 BlockCount)  //已读的Bock ;Indication which Bock of File loaded 
										      //become is a 512 Bytes
{
	//computation of the Blocks out BlockCount and Cluster out FAT Tabelle
	//computation which Cluster load is
	
	U32 Block = (BlockCount/cluster_size);//(  已读的Bock / 每簇扇区数 )
	
	fat_load (Cluster,&Block,Buffer);	//读FAT表中下一簇

	Block = ((Block-2) * cluster_size) + cluster_offset; //block=(block-2) * 每簇扇区数 + 根目录开始扇区号

	//computation of the Blocks within of the Cluster
	Block += (BlockCount % cluster_size);//每簇扇区数
	//Read Data Block from Device
	mmc_read_sector (Block,Buffer);	
	return;
}

//---------------------------------------------------------------------
//Vintages one 512 Bytes Blocks of one File
//---------------------------------------------------------------------
void fat_write_file (U16 cluster,   //Indication of the Start clusters of File
					U08 *buffer,	  //data buffer
					U32 blockCount)	  //Indication which Bock of File stored 
									  //become is a 512 Bytes
{
	//computation of the Blocks out BlockCount and Cluster out FATTabelle
	//computation which Cluster too store is
	U08 tmp_buffer[513];	
	U32 block = (blockCount/cluster_size);
	
	//Selections that FAT - Tabelle
	fat_load (cluster,&block,tmp_buffer);	 	 
	block = ((block-2) * cluster_size) + cluster_offset;
	//computation of the Blocks within of the Cluster
	block += (blockCount % cluster_size);
	//Write Data Block to Device
	mmc_write_sector (block,buffer);	
	return;
}

//---------------------------------------------------------------------
//Search File in Directory
//---------------------------------------------------------------------

U08 fat_search_file (U08 *File_Name,		//File_Name
							U16 *Cluster, 	//Dir Cluster 
							U32 *Size, 		//return size of file 
							U08 *Dir_Attrib,//return attrib of file
							U08 *Buffer) 	//data Buffer
{
	U16 Dir_Cluster_Store = *Cluster;
    U08 a;
	for (a = 0;a < 100;a++)
	{
		*Cluster = fat_read_dir_ent(Dir_Cluster_Store,a,Size,Dir_Attrib,Buffer);
		if (*Cluster == 0xffff)
			{
			return(0); //File not Found
			}
		if(strcasecmp(File_Name,Buffer) == 0)
			{
			return(1); //File Found
			}
	}
	return(2); //Error
}

 
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
