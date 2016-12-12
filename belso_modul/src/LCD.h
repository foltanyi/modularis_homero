/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LCD_h
#define LCD_h

/* ------------------ DDRAM addresses: -----------------------*/
#define Line1_Column1 	0x00
#define Line1_Column2 	0x01
#define Line1_Column3 	0x02
#define Line1_Column4 	0x03
#define Line1_Column5 	0x04
#define Line1_Column6 	0x05
#define Line1_Column7 	0x06
#define Line1_Column8 	0x07
#define Line1_Column9 	0x08
#define Line1_Column10 	0x09
#define Line1_Column11 	0x0A
#define Line1_Column12 	0x0B
#define Line1_Column13 	0x0C
#define Line1_Column14 	0x0D
#define Line1_Column15 	0x0E
#define Line1_Column16 	0x0F


#define Line2_Column1 	0x40
#define Line2_Column2 	0x41
#define Line2_Column3 	0x42
#define Line2_Column4	0x43
#define Line2_Column5 	0x44
#define Line2_Column6 	0x45
#define Line2_Column7 	0x46
#define Line2_Column8	0x47
#define Line2_Column9 	0x48
#define Line2_Column10	0x49
#define Line2_Column11	0x4A
#define Line2_Column12	0x4B
#define Line2_Column13	0x4C
#define Line2_Column14	0x4D
#define Line2_Column15	0x4E
#define Line2_Column16 	0x4F


/*---------------------függvények----------------------------*/




/*------------------LCD DB adat küldés-----------------------*/
void LCD_DB_write(uint8_t data);

/*------------------LCD sztring kiírása----------------------*/
void LCD_BD_write_string(char* data);

/*----------------- LCD inicializálása ----------------------*/
void LCD_init();

/*----------------- instruction word küldése-------------------*/
void LCD_IC_write(uint8_t data);

/*-----------LCD sztring kiírása a mgeadott pozíciótól kezdve-------*/
void LCD_disp_data_at_pos(char* data,uint8_t pos);

/*-----------Az elsõ sorba kiírjuk, hogy "benti temp:"
  			 A második sorba kiírjuk, hogy "kinti temp:"---------*/
void LCD_temp_szoveg_kiirasa();


void read_lcd();

#endif
