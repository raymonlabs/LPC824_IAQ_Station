//#include <string>
#include "mbed.h"
#include "scd30.h"
#include "sps30.h"

//#define SDA0                    P0_11 // FOR LPC824 P0_11 dp4-sda
//#define SCL0                    P0_10 // FOR LPC824 P0_10 dp5-scl
#define LEDON                   0 // Active LOW  LED
#define LEDOFF                  1

DigitalOut myled(P0_20); // 

sps30 sps(I2C_SDA, I2C_SCL, 100000);
scd30 scd(I2C_SDA, I2C_SCL, 100000);

Serial pc(USBTX, USBRX); // FOR LPC824 -> Set GPIO


void initSplash() 
{
    pc.printf("\r\n\r\n");
    pc.printf("-----------------------------------------------------------------------------\r\n");
}

void initSPS30()
{
    pc.printf("Initializing SPS30...\r\n");
    uint8_t dbg = sps.softReset();
    if (dbg != sps30::SPSnoERROR) pc.printf("No ack \r\n");
    wait_ms(1000);
    sps.getSerialNumber();
    pc.printf(" - SPS30 s/n ascii: ");
    for(int i = 0; i < sizeof(sps.sn); i++) {
        pc.printf("%c", sps.sn[i]);
    }
    pc.printf("\r\n");
    sps.getArticleCode();
    pc.printf(" - SPS30 article code ascii: ");
    for(int i = 0; i < sizeof(sps.acode); i++) {
        pc.printf("%c", sps.acode[i]);
    }

    pc.printf("\r\n");
    sps.startMeasurement();
}

void initSCD30() 
{
    pc.printf("Initializing SCD30...\r\n");
    scd.softReset();
    wait_ms(1000);
    scd.getSerialNumber();
    pc.printf(" - SCD30 s/n: ");
    for(int i = 0; i < sizeof(scd.scdSTR.sn); i++) pc.printf("%c", scd.scdSTR.sn[i]);
    pc.printf("\r\n");
        
    scd.setMeasInterval(2);
    scd.startMeasurement(0);
}

int main()
{
    myled = LEDOFF;

    pc.baud(115200);
    wait_ms(200);

    initSplash();
    initSPS30();
    initSCD30();
    int countSPS = 0;
//    string mass_1p0, mass_2p5, mass_4p0, mass_10p0, num_0p5, num_1p0, num_2p5, num_4p0, num_10p0, typ_pm_size;
    pc.printf("Ready...\r\n");
    pc.printf(" count |       MASS CONCENTRATION (ug/m3)       |           NUMBER CONCENTRATION (#/cm3)           | Typical Particle Size \r\n");
    pc.printf("       |  PM1.0  |  PM2.5  |  PM4.0  |  PM10.0  |  PM0.5  |  PM1.0  |  PM2.5  |  PM4.0  |  PM10.0  |         (um)          \r\n");
    pc.printf("---------------------------------------------------------------------------------------------------------------------------\r\n");
    while(1)
    {
        myled = !myled;
        wait_ms(250);
        sps.getReadyStatus();
        uint16_t redy = sps.ready;
        if(redy == sps30::SPSisReady) 
        {
            uint8_t crcc = sps.readMeasurement();
            countSPS++;
            if(crcc != sps30::SPSnoERROR) pc.printf("ERROR: %d\r\n", crcc);
            else pc.printf(" %5d | %7.3f | %7.3f |  %7.3f | %7.3f | %7.3f | %7.3f | %7.3f | %7.3f |  %7.3f |       %7.3f \r\n", \
                            countSPS, sps.mass_1p0_f, sps.mass_2p5_f, sps.mass_4p0_f, sps.mass_10p0_f, sps.num_0p5_f, \
                            sps.num_1p0_f, sps.num_2p5_f, sps.num_4p0_f, sps.num_10p0_f, sps.typ_pm_size_f);
        }

        wait(2);

        int count = 0;
        
        wait_ms(250);
        scd.getReadyStatus();
        uint16_t reAAdy = scd.scdSTR.ready;
        if(reAAdy == scd30::SCDisReady) 
        {
            uint8_t crcc = scd.readMeasurement();
            count++;
            if(crcc != scd30::SCDnoERROR) pc.printf("ERROR: %d\r\n", crcc);
            else pc.printf("%5d  -> CO2: %9.3f   Temp: %7.3f   Hum: %5.2f\r\n", 
                            count, scd.scdSTR.co2f, scd.scdSTR.tempf, scd.scdSTR.humf);
            if((int)scd.scdSTR.co2f > 10000) initSCD30();
        }
    }
}
