//The master code///////////////////////////////////////////////////
#ifdef master_using
//remeber init all GPIO after using it!
#define ASDA_Port GPIOA
#define ASDA      GPIO_PIN_13
#define ASDA_H    GPIO_SetBits(ASDA_Port,ASDA)
#define ASDA_L    GPIO_ResetBits(ASDA_Port,ASDA)
#define ASDA_READ GPIO_ReadInputDataBit(ASDA_Port,ASDA)

#define ASCL_Port GPIOB
#define ASCL      GPIO_PIN_1
#define ASCL_H    GPIO_SetBits(ASCL_Port,ASCL)
#define ASCL_L    GPIO_ResetBits(ASCL_Port,ASCL)
//I2C bus delay
static void i2c_Delay(void)
{
  Delay_us(5);
}
//I2C bus start sign
void i2c_Start(void)
{
  ASDA_Output();
  ASDA_H;
  ASCL_H;
  i2c_Delay();
  ASDA_L;
  i2c_Delay();
  ASCL_L;
  i2c_Delay();
}
//I2C bus stop sign
void i2c_Stop()
{
  ASDA_Output();
  ASDA_L;
  i2c_Delay();
  ASCL_H;
  i2c_Delay();
  ASDA_H;
  i2c_Delay();
}
//i2c bus send 8bit data
void i2c_SendByte(uint8_t _ucByte)
{
  uint8_t i;
  ASDA_Output();
  for(i=0;i<8;i++)
  {
    if(_ucByte & 0x80)
    {
      ASDA_H;
    }
    else
    {
      ASDA_L;
    }
    i2c_Delay();
    ASCL_L;
    if(i==7)
    {
      ASDA_H;
    }
    _ucByte <<= 1;//left move 1 byte
    i2c_Delay();
  }
}
//i2c bus read one byte
uint8_t i2c_ReadByte(void)
{
  uint8_t i;
  uint8_t value;
  value = 0;
  ASDA_Input();//SDA GPIO change input mode
  for(i=0;i<8;i++)
  {
    value <<= 1;
    ASCL_H;
    i2c_Delay();
    if(ASDA_READ == 1)
    {
      value |= 0x01;
    }
    ASCL_L;
    i2c_Delay();
  }
  return value;
}
//master i2c wait a cpu clock sign return
uint8_t i2c_WaitAck(void)
{
  uint8_t re = 0;
  ASDA_Input();
  i2c_Delay();
  ASCL_H;
  i2c_Delay();
  if(ASDA_READ == 1)
  {
    re = 1;
  }
  else
  {
    re = 0;
  }
  ASCL_L;
  i2c_Delay();
  ASDA_Output();//set the SDA GPIO OD output mode
  return re;//if return 0 is ture ack,return 1 is error ack 
}
//master product a ack sign to slave
void i2c_Ack()
{
  ASDA_Output();
  ASDA_L;
  i2c_Delay();
  ASCL_H;
  i2c_Delay();
  ASCL_L;
  i2c_Delay();
  ASDA_H;
}
//master product a Nack sign to slave
void i2c_NAck()
{
  ASDA_Output();
  ASDA_H;
  i2c_Delay();
  ASCL_H;
  i2c_Delay();
  ASCL_L;
  i2c_Delay();
}
//many bytes send funsion
void I2C_Send_Array(u8 *array,uint32_t len)
{
  uint32_t i;
  i2c_Start();
  i2c_SendByte(0x00);//this should edit your Slave address!!! 0x00 is wide address!!!
  if(i2c_WaitAck() != 0)
  {
    i2c_Stop();
    return;
  }
  for(i=0;i<len;i++)
  {
    i2c_SendByte(array[i]);//send some data
    if(i2c_WaitAck() != 0)
    {
      i2c_Stop();
      return;
    }
  }
  i2c_Stop();
}
//read all bytes funsion
void i2c_readBytes(uint8_t slave_address, uint8_t reg_address, uint8_t *buffer, uint16_t length)
{
  i2c_Start();
  i2c_SendByte(0x00);//or click slave_address
  i2c_WaitAck();
  i2c_SendByte(0x10);//or click reg_address
  i2c_WaitAck();

  i2c_Start();
  i2c_SendByte(slave_address | 0x01);//read address need to set 1 for last bit
  i2c_WaitAck();

  //read all byte
  for(i=0;i<length;i++)
  {
    buffer[i] = i2c_ReadByte();
    if(i < (length - 1))
    {
      i2c_Ack();
    }
    else
    {
      i2c_NAck();
    }
    Delay_ms(1);//according to the actual condition,if you cpu run fast,you can ignore it.
  }
  i2c_Stop();
}
#endif
//The master code////////////////////////////////////
