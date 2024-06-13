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
