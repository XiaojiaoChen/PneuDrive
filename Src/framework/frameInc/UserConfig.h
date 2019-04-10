/*
 * UserConfig.h
 *
 *  Created on: Mar 20, 2019
 *      Author: 402072495
 */

#ifndef FRAMEWORK_FRAMEINC_USERCONFIG_H_
#define FRAMEWORK_FRAMEINC_USERCONFIG_H_


/***************************Configure control and display frequency********************/
#define CONTROL_FREQUENCY 1000	//The loop() frequency in Hz
#define DISPLAY_FREQUENCY 1000	//The serialDisplay() frequency in Hz

/***************************Configure hardware extention********************/
#define ADBOARD_NUM 	0		//The quantity of AD7616 extension boards
#define DABOARD_NUM 	0		//The quantity of LTC2668 extension boards
#define PWMBOARDSPI_NUM 0		//The quantity of LT8500 extension boards
#define PWMBOARDI2C_NUM 0		//The quantity of PCA9685 extension boards
#define PWM_DIGITAL_OUTPUT_REVERSED	0 //Normally 0. Set to 1 if the power amplifier board needs reversed output.



#endif /* FRAMEWORK_FRAMEINC_USERCONFIG_H_ */
