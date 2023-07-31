#ifndef DHT11_H
#define DHT11_H

#define uchar unsigned char
extern void Delay_us(void);
extern void Delay_10us(void);
extern void Delay_ms(unsigned int xms);	
extern void COM(void);                  
extern void DHT11(void);               

extern uchar humid_1, humid_0, temp_1, temp_0;


#endif
