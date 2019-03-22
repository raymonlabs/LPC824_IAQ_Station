#ifndef SPS30_H
#define SPS30_H

#define SPS30_I2C_ADDR                  0xd2

#define SPS30_CMMD_STRT_MEAS            0x0010
#define SPS30_CMMD_STOP_MEAS            0x0104
#define SPS30_CMMD_GET_READY_STAT       0x0202
#define SPS30_CMMD_READ_MEAS            0x0300

#define SPS30_CMMD_AUTO_CLEAN_INTV      0x8004
#define SPS30_CMMD_START_FAN_CLEAN      0x5607

#define SPS30_CMMD_SOFT_RESET           0xD304

#define SPS30_CMMD_READ_SERIALNBR       0xD033
#define SPS30_CMMD_READ_ARTICLECODE     0xD025
    
#define SPS30_POLYNOMIAL                0x31   // P(x) = x^8 + x^5 + x^4 + 1 = 100110p01
#define SPS30_CRC_INIT                  0xff

#define SPS30_SN_SIZE                   33      //size of the s/n ascii string + CRC values
#define SPS30_ACODE_SIZE                33      //size of the article code ascii string + CRC values

    /** Create SPS30 controller class
     *
     * @param sps30 class
     *
     */
class sps30 {

public:
    enum SPSerror {
        SPSnoERROR,         //all ok
        SPSisReady,         //ready ststus register
        SPSnoAckERROR,      //no I2C ACK error
        SPStimeoutERROR,    //I2C timeout error
        SPScrcERROR,        //CRC error, any
        // SPScrcERRORv1,      //CRC error on value 1
        // SPScrcERRORv2,      //CRC error on value 2
        // SPScrcERRORv3,      //CRC error on value 3
        // SPScrcERRORv4,      //CRC error on value 4
        // SPScrcERRORv5,      //CRC error on value 5
        // SPScrcERRORv6,      //CRC error on value 6
    };
    
    uint16_t ready;         /**< 1 = ready, 0 = busy */
        
    float mass_1p0_f;          /**< float of Mass Conc of PM1.0 */
    float mass_2p5_f;          /**< float of Mass Conc of PM2.5 */
    float mass_4p0_f;          /**< float of Mass Conc of PM4.0 */
    float mass_10p0_f;          /**< float of Mass Conc of PM10 */

    float num_0p5_f;          /**< float of Number Conc of PM0.5 */
    float num_1p0_f;          /**< float of Number Conc of PM1.0 */
    float num_2p5_f;          /**< float of Number Conc of PM2.5 */
    float num_4p0_f;          /**< float of Number Conc of PM4.0 */
    float num_10p0_f;          /**< float of Number Conc of PM10 */

    float typ_pm_size_f;          /**< float of Typical Particle Size */
        
    uint8_t acode[24];         /**< Article code number?? */
    uint8_t sn[24];         /**< ASCII Serial Number */

    
    
    /** Create a SPS30 object using the specified I2C object
     * @param sda - mbed I2C interface pin
     * @param scl - mbed I2C interface pin
     * @param I2C Frequency (in Hz)
     *
     * @return none
     */
     sps30(PinName sda, PinName scl, int i2cFrequency);
     
    /** Destructor
     *
     * @param --none--
     *
     * @return none
     */
    ~sps30();
    
    /** Start Auto-Measurement 
     *
     * @param --none--
     *
     * @return enum SPSerror
     */
    uint8_t startMeasurement();
    
    /** Stop Auto-Measurement 
     *
     * @param --none--
     *
     * @return enum SPSerror
     */
    uint8_t stopMeasurement();
    
    /** Get Ready Status register 
     *
     * @param --none--
     * @see Ready Status result
     *
     * @return enum SPSerror
     */
    uint8_t getReadyStatus();
    
    /** Get all particulate matter parameters
     *
     * @param --none-
     * @see Results in Public member variables
     *
     * @return enum SPSerror
     */
    uint8_t readMeasurement();
    
    /** Calculate the SPS30 CRC value
     *
     * @param 16 bit value to perform a CRC check on
     *
     * @return 8 bit CRC value
     */
    uint8_t calcCrc2b(uint16_t seed);
    
    /** Compare received CRC value with calculated CRC value
     *
     * @param 16 bit value to perform a CRC check on
     * @param 8 bit value to compare CRC values
     *
     * @return enum SPSerror
     */
    uint8_t checkCrc2b(uint16_t seed, uint8_t crcIn);
    
    /** Get Article Code
     *
     * @param --none--
     * 
     *
     * @return enum SPSerror
     */
    uint8_t getArticleCode();
    
    /** Get Serial Number
     *
     * @param --none--
     * @see ASCII Serial Number in scdSTR structure
     *
     * @return enum SPSerror
     */
    uint8_t getSerialNumber();

    /** Perform a soft reset
     *
     * @param --none--
     *
     * @return enum SPSerror
     */
    uint8_t softReset();
 
private:
    char i2cBuff[60];

    uint16_t mass_1p0_m;          /**< High order 16 bit word of Mass Conc of PM1.0 */
    uint16_t mass_1p0_l;          /**< Low order 16 bit word of Mass Conc of PM1.0 */
    uint16_t mass_2p5_m;          /**< High order 16 bit word of Mass Conc of PM2.5 */
    uint16_t mass_2p5_l;          /**< Low order 16 bit word of Mass Conc of PM2.5 */
    uint16_t mass_4p0_m;          /**< High order 16 bit word of Mass Conc of PM4.0 */
    uint16_t mass_4p0_l;          /**< Low order 16 bit word of Mass Conc of PM4.0 */
    uint16_t mass_10p0_m;          /**< High order 16 bit word of Mass Conc of PM10 */
    uint16_t mass_10p0_l;          /**< Low order 16 bit word of Mass Conc of PM10 */

    uint16_t num_0p5_m;          /**< High order 16 bit word of Number Conc of PM0.5 */
    uint16_t num_0p5_l;          /**< Low order 16 bit word of Number Conc of PM0.5 */
    uint16_t num_1p0_m;          /**< High order 16 bit word of Number Conc of PM1.0 */
    uint16_t num_1p0_l;          /**< Low order 16 bit word of Number Conc of PM1.0 */
    uint16_t num_2p5_m;          /**< High order 16 bit word of Number Conc of PM2.5 */
    uint16_t num_2p5_l;          /**< Low order 16 bit word of Number Conc of PM2.5 */
    uint16_t num_4p0_m;          /**< High order 16 bit word of Number Conc of PM4.0 */
    uint16_t num_4p0_l;          /**< Low order 16 bit word of Number Conc of PM4.0 */
    uint16_t num_10p0_m;          /**< High order 16 bit word of Number Conc of PM10 */
    uint16_t num_10p0_l;          /**< Low order 16 bit word of Number Conc of PM10 */

    uint16_t typ_pm_size_m;          /**< High order 16 bit word of Typical Particle Size */
    uint16_t typ_pm_size_l;          /**< Low order 16 bit word of Typical Particle Size */

    uint32_t mass_1p0_i;          /**< 32 bit int of Mass Conc of PM1.0 */
    uint32_t mass_2p5_i;          /**< 32 bit int of Mass Conc of PM2.5 */
    uint32_t mass_4p0_i;          /**< 32 bit int of Mass Conc of PM4.0 */
    uint32_t mass_10p0_i;          /**< 32 bit int of Mass Conc of PM10 */

    uint32_t num_0p5_i;          /**< 32 bit int of Number Conc of PM0.5 */
    uint32_t num_1p0_i;          /**< 32 bit int of Number Conc of PM1.0 */
    uint32_t num_2p5_i;          /**< 32 bit int of Number Conc of PM2.5 */
    uint32_t num_4p0_i;          /**< 32 bit int of Number Conc of PM4.0 */
    uint32_t num_10p0_i;          /**< 32 bit int of Number Conc of PM10 */

    uint32_t typ_pm_size_i;          /**< 32 bit int of Typical Particle Size */
 
protected:
    I2C     _i2c;    

};    
#endif

