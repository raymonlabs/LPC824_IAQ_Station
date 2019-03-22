#include "mbed.h"
#include "sps30.h"

//-----------------------------------------------------------------------------
// Constructor 

sps30::sps30(PinName sda, PinName scl, int i2cFrequency)  : _i2c(sda, scl) {
        _i2c.frequency(i2cFrequency);
}

//-----------------------------------------------------------------------------
// Destructor

sps30::~sps30() {
}

//-----------------------------------------------------------------------------
// start auto-measurement with barometer reading (in mB)
//

uint8_t sps30::startMeasurement()   // done
{
    i2cBuff[0] = SPS30_CMMD_STRT_MEAS >> 8;
    i2cBuff[1] = SPS30_CMMD_STRT_MEAS & 255;
    i2cBuff[2] = 0x03;
    i2cBuff[3] = 0x00;
    i2cBuff[4] = sps30::calcCrc2b(0x0300);
    int res = _i2c.write(SPS30_I2C_ADDR, i2cBuff, 5, false);
    if(res) return SPSnoAckERROR;
    return SPSnoERROR;
}

//-----------------------------------------------------------------------------
// Stop auto-measurement

uint8_t sps30::stopMeasurement()    // done
{
    i2cBuff[0] = SPS30_CMMD_STOP_MEAS >> 8;
    i2cBuff[1] = SPS30_CMMD_STOP_MEAS & 255;
    int res = _i2c.write(SPS30_I2C_ADDR, i2cBuff, 2, false);
    if(res) return SPSnoAckERROR;
    return SPSnoERROR;
}

//-----------------------------------------------------------------------------
// Get ready status value

uint8_t sps30::getReadyStatus() // done
{
    i2cBuff[0] = SPS30_CMMD_GET_READY_STAT >> 8;
    i2cBuff[1] = SPS30_CMMD_GET_READY_STAT & 255;
    int res = _i2c.write(SPS30_I2C_ADDR, i2cBuff, 2, false);
    if(res) return SPSnoAckERROR;
    
    _i2c.read(SPS30_I2C_ADDR | 1, i2cBuff, 3, false);
    uint16_t stat = (i2cBuff[0] << 8) | i2cBuff[1];
    ready = stat;
    uint8_t dat = sps30::checkCrc2b(stat, i2cBuff[2]);
    
    if(dat == SPScrcERROR) return SPScrcERROR;
    if(dat == SPSisReady) return SPSisReady;
    return SPSnoERROR;
}

//-----------------------------------------------------------------------------
// Get all the measurement values, stick them into the array

uint8_t sps30::readMeasurement()    // editing
{
    i2cBuff[0] = SPS30_CMMD_READ_MEAS >> 8;
    i2cBuff[1] = SPS30_CMMD_READ_MEAS & 255;
    int res = _i2c.write(SPS30_I2C_ADDR, i2cBuff, 2, false);
    if(res) return SPSnoAckERROR;
    
    _i2c.read(SPS30_I2C_ADDR | 1, i2cBuff, 60, false);
    
    /* TO DO: update the members used in the class */
    uint16_t stat = (i2cBuff[0] << 8) | i2cBuff[1];
    mass_1p0_m = stat;
    uint8_t dat = sps30::checkCrc2b(stat, i2cBuff[2]);
    if(dat == SPScrcERROR) return SPScrcERROR;
    
    stat = (i2cBuff[3] << 8) | i2cBuff[4];
    mass_1p0_l = stat;
    dat = sps30::checkCrc2b(stat, i2cBuff[5]);
    if(dat == SPScrcERROR) return SPScrcERROR;


    
    stat = (i2cBuff[6] << 8) | i2cBuff[7];
    mass_2p5_m = stat;
    dat = sps30::checkCrc2b(stat, i2cBuff[8]);
    if(dat == SPScrcERROR) return SPScrcERROR;
    
    stat = (i2cBuff[9] << 8) | i2cBuff[10];
    mass_2p5_l = stat;
    dat = sps30::checkCrc2b(stat, i2cBuff[11]);
    if(dat == SPScrcERROR) return SPScrcERROR;


    
    stat = (i2cBuff[12] << 8) | i2cBuff[13];
    mass_4p0_m = stat;
    dat = sps30::checkCrc2b(stat, i2cBuff[14]);
    if(dat == SPScrcERROR) return SPScrcERROR;
    
    stat = (i2cBuff[15] << 8) | i2cBuff[16];
    mass_4p0_l = stat;
    dat = sps30::checkCrc2b(stat, i2cBuff[17]);
    if(dat == SPScrcERROR) return SPScrcERROR;



    stat = (i2cBuff[18] << 8) | i2cBuff[19];
    mass_10p0_m = stat;
    dat = sps30::checkCrc2b(stat, i2cBuff[20]);
    if(dat == SPScrcERROR) return SPScrcERROR;
    
    stat = (i2cBuff[21] << 8) | i2cBuff[22];
    mass_10p0_l = stat;
    dat = sps30::checkCrc2b(stat, i2cBuff[23]);
    if(dat == SPScrcERROR) return SPScrcERROR;



    stat = (i2cBuff[24] << 8) | i2cBuff[25];
    num_0p5_m = stat;
    dat = sps30::checkCrc2b(stat, i2cBuff[26]);
    if(dat == SPScrcERROR) return SPScrcERROR;
    
    stat = (i2cBuff[27] << 8) | i2cBuff[28];
    num_0p5_l = stat;
    dat = sps30::checkCrc2b(stat, i2cBuff[29]);
    if(dat == SPScrcERROR) return SPScrcERROR;


    stat = (i2cBuff[30] << 8) | i2cBuff[31];
    num_1p0_m = stat;
    dat = sps30::checkCrc2b(stat, i2cBuff[32]);
    if(dat == SPScrcERROR) return SPScrcERROR;
    
    stat = (i2cBuff[33] << 8) | i2cBuff[34];
    num_1p0_l = stat;
    dat = sps30::checkCrc2b(stat, i2cBuff[35]);
    if(dat == SPScrcERROR) return SPScrcERROR;


    
    stat = (i2cBuff[36] << 8) | i2cBuff[37];
    num_2p5_m = stat;
    dat = sps30::checkCrc2b(stat, i2cBuff[38]);
    if(dat == SPScrcERROR) return SPScrcERROR;
    
    stat = (i2cBuff[39] << 8) | i2cBuff[40];
    num_2p5_l = stat;
    dat = sps30::checkCrc2b(stat, i2cBuff[41]);
    if(dat == SPScrcERROR) return SPScrcERROR;


    
    stat = (i2cBuff[42] << 8) | i2cBuff[43];
    num_4p0_m = stat;
    dat = sps30::checkCrc2b(stat, i2cBuff[44]);
    if(dat == SPScrcERROR) return SPScrcERROR;
    
    stat = (i2cBuff[45] << 8) | i2cBuff[46];
    num_4p0_l = stat;
    dat = sps30::checkCrc2b(stat, i2cBuff[47]);
    if(dat == SPScrcERROR) return SPScrcERROR;


    stat = (i2cBuff[48] << 8) | i2cBuff[49];
    num_10p0_m = stat;
    dat = sps30::checkCrc2b(stat, i2cBuff[50]);
    if(dat == SPScrcERROR) return SPScrcERROR;
    
    stat = (i2cBuff[51] << 8) | i2cBuff[52];
    num_10p0_l = stat;
    dat = sps30::checkCrc2b(stat, i2cBuff[53]);
    if(dat == SPScrcERROR) return SPScrcERROR;


    stat = (i2cBuff[54] << 8) | i2cBuff[55];
    typ_pm_size_m = stat;
    dat = sps30::checkCrc2b(stat, i2cBuff[56]);
    if(dat == SPScrcERROR) return SPScrcERROR;
    
    stat = (i2cBuff[57] << 8) | i2cBuff[58];
    typ_pm_size_l = stat;
    dat = sps30::checkCrc2b(stat, i2cBuff[59]);
    if(dat == SPScrcERROR) return SPScrcERROR;
    
    mass_1p0_i = (mass_1p0_m << 16) | mass_1p0_l;
    mass_2p5_i = (mass_2p5_m << 16) | mass_2p5_l;
    mass_4p0_i = (mass_4p0_m << 16) | mass_4p0_l;
    mass_10p0_i = (mass_10p0_m << 16) | mass_10p0_l;

    num_0p5_i = (num_0p5_m << 16) | num_0p5_l;
    num_1p0_i = (num_1p0_m << 16) | num_1p0_l;
    num_2p5_i = (num_2p5_m << 16) | num_2p5_l;
    num_4p0_i = (num_4p0_m << 16) | num_4p0_l;
    num_10p0_i = (num_10p0_m << 16) | num_10p0_l;          

    typ_pm_size_i = (typ_pm_size_m << 16) | typ_pm_size_l;          

    mass_1p0_f = *(float*)&mass_1p0_i;
    mass_2p5_f = *(float*)&mass_2p5_i;
    mass_4p0_f = *(float*)&mass_4p0_i;
    mass_10p0_f = *(float*)&mass_10p0_i;

    num_0p5_f = *(float*)&num_0p5_i;
    num_1p0_f = *(float*)&num_1p0_i;
    num_2p5_f = *(float*)&num_2p5_i;
    num_4p0_f = *(float*)&num_4p0_i;
    num_10p0_f = *(float*)&num_10p0_i;          

    typ_pm_size_f = *(float*)&typ_pm_size_i;
    
    return SPSnoERROR;
}
    
//-----------------------------------------------------------------------------
// Calculate the CRC of a 2 byte value using the SPS30 CRC polynomial

uint8_t sps30::calcCrc2b(uint16_t seed)
{
  uint8_t bit;                  // bit mask
  uint8_t crc = SPS30_CRC_INIT; // calculated checksum
  
  // calculates 8-Bit checksum with given polynomial

    crc ^= (seed >> 8) & 255;
    for(bit = 8; bit > 0; --bit)
    {
      if(crc & 0x80) crc = (crc << 1) ^ SPS30_POLYNOMIAL;
      else           crc = (crc << 1);
    }

    crc ^= seed & 255;
    for(bit = 8; bit > 0; --bit)
    {
      if(crc & 0x80) crc = (crc << 1) ^ SPS30_POLYNOMIAL;
      else           crc = (crc << 1);
    }
    
  return crc;
}

//-----------------------------------------------------------------------------
// Compare the CRC values

uint8_t sps30::checkCrc2b(uint16_t seed, uint8_t crcIn)
{
    uint8_t crcCalc = sps30::calcCrc2b(seed);
    if(crcCalc != crcIn) return SPScrcERROR;
    return SPSnoERROR;
}

//-----------------------------------------------------------------------------
// Get article code

uint8_t sps30::getArticleCode()
{
    i2cBuff[0] = SPS30_CMMD_READ_ARTICLECODE >> 8;
    i2cBuff[1] = SPS30_CMMD_READ_ARTICLECODE & 255;
    int res = _i2c.write(SPS30_I2C_ADDR, i2cBuff, 2, false);
    if(res) return SPSnoAckERROR;
    
    int i = 0;
    for(i = 0; i < sizeof(acode); i++) acode[i] = 0;
    for(i = 0; i < sizeof(i2cBuff); i++) i2cBuff[i] = 0;
    
    _i2c.read(SPS30_I2C_ADDR | 1, i2cBuff, SPS30_ACODE_SIZE, false);
    int t = 0;
    for(i = 0; i < SPS30_ACODE_SIZE; i +=3) {
        uint16_t stat = (i2cBuff[i] << 8) | i2cBuff[i + 1];
        acode[i - t] = stat >> 8;
        acode[i - t + 1] = stat & 255;
        uint8_t dat = sps30::checkCrc2b(stat, i2cBuff[i + 2]);
        t++;
        if(dat == SPScrcERROR) return SPScrcERROR;
        if(stat == 0) break;
    }

    return SPSnoERROR;
}

//-----------------------------------------------------------------------------
// Get sps30 serial number

uint8_t sps30::getSerialNumber()
{
    i2cBuff[0] = SPS30_CMMD_READ_SERIALNBR >> 8;
    i2cBuff[1] = SPS30_CMMD_READ_SERIALNBR & 255;
    int res = _i2c.write(SPS30_I2C_ADDR, i2cBuff, 2, false);
    if(res) return SPSnoAckERROR;
    
    int i = 0;
    for(i = 0; i < sizeof(sn); i++) sn[i] = 0;
    for(i = 0; i < sizeof(i2cBuff); i++) i2cBuff[i] = 0;
    
    _i2c.read(SPS30_I2C_ADDR | 1, i2cBuff, SPS30_SN_SIZE, false);
    int t = 0;
    for(i = 0; i < SPS30_SN_SIZE; i +=3) {
        uint16_t stat = (i2cBuff[i] << 8) | i2cBuff[i + 1];
        sn[i - t] = stat >> 8;
        sn[i - t + 1] = stat & 255;
        uint8_t dat = sps30::checkCrc2b(stat, i2cBuff[i + 2]);
        t++;
        if(dat == SPScrcERROR) return SPScrcERROR;
        if(stat == 0) break;
    }

    return SPSnoERROR;
}

//-----------------------------------------------------------------------------
// Perform a soft reset on the SPS30

uint8_t sps30::softReset()
{
    i2cBuff[0] = SPS30_CMMD_SOFT_RESET >> 8;
    i2cBuff[1] = SPS30_CMMD_SOFT_RESET & 255;
    int res = _i2c.write(SPS30_I2C_ADDR, i2cBuff, 2, false);
    if(res) return SPSnoAckERROR;
    return SPSnoERROR;
}