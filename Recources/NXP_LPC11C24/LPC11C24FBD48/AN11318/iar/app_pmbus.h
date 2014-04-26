// Anacortes PMB demo source code

#ifndef _NXP_PMBUS_H_
#define _NXP_PMBUS_H_

/* ***Constant Definitions*** */
#define	PARMS_MAX	 202  // 0 to 201
#define FIVE_YEARS	 0xAB18	 // counts 5 years 24*365*5=43800

#define SMBUS_ARA   				0x18		// alert address 0x18 would expect 0x30 << 1
#define SMBUS_DEFAULT   			0x26            //0xC2		// I2C default SMBUS address 
#define IIC_BUF_SIZE                            0x15



#define PMBUS_FACTORY_SETTINGS   	0			// EEPROM mem location start for factory values
#define PMBUS_USER_SETTINGS   		PARMS_MAX	// EEPROM mem location start for user vlaues
#define TIMER_1MS					(0xFFFF - 0xE66) // counts it takes for 1ms /*   ***Internal Oscillator*** 7373000 */
#define TIMER_10MS					(0xFFFF - 0x8FFC) // counts it takes for 10ms /*   ***Internal Oscillator*** 7373000 */
#define TIMER_5MS					(0xFFFF - 0x47FE) // counts it takes for 5ms /*   ***Internal Oscillator*** 7373000 */
#define TIMER_TH_1MS				(TIMER_1MS & 0xFF00) >> 8	// Timer 0 clock rate user to get 1ms
#define TIMER_TL_1MS				(TIMER_1MS & 0xFF)			// Timer 0 clock rate user to get 1ms
//#define TIMER_TH_10MS				(TIMER_10MS & 0x0FF00) >> 8	// Timer 0 clock rate user to get 10ms
//#define TIMER_TL_10MS				(TIMER_10MS & 0x0FF)		// Timer 0 clock rate user to get 10ms
//#define TIMER_TH_5MS				(TIMER_5MS & 0x0FF00) >> 8	// Timer 0 clock rate user to get 5ms
//#define TIMER_TL_5MS				(TIMER_5MS & 0x0FF)		    // Timer 0 clock rate user to get 5ms

#define PMBUS_OUTPUT_ENABLED  0x02   // P2
#define PMBUS_CONTROL_HIGH    0x40
#define PMBUS_FAN1_HIGH       0x40
#define PMBUS_FAN2_HIGH       0x80

/********************************************************************
*							    	    *
*					PMBus defs here		    * 
*								    *
*********************************************************************/

	

									  
#define PMBUS_OPERATION				         0x01
#define PMBUS_ON_OFF_CONFG				 0x02
#define	PMBUS_CLEAR_FAULTS				 0x03 
#define	PMBUS_WRITE_PROTECT				 0x10 
#define	PMBUS_STORE_DEFAULT_ALL				 0x11 
#define	PMBUS_RESTORE_DEFAULT_ALL			 0x12 
#define	PMBUS_STORE_DEFAULT_CODE			 0x13 
#define	PMBUS_RESTORE_DEFAULT_CODE			 0x14 
#define	PMBUS_STORE_USER_ALL				 0x15 
#define	PMBUS_RESTORE_USER_ALL				 0x16 
#define	PMBUS_STORE_USER_CODE				 0x17 
#define	PMBUS_RESTORE_USER_CODE				 0x18 
#define	PMBUS_CAPABILITY				 0x19 
#define	PMBUS_QUERY					 0x1A 
#define	PMBUS_VOUT_MODE					 0x20 
#define	PMBUS_VOUT_COMMAND				 0x21 
#define	PMBUS_VOUT_TRIM					 0X22 
#define	PMBUS_VOUT_CAL_OFFSET				 0X23 
#define	PMBUS_VOUT_MAX					 0x24 
#define	PMBUS_VOUT_MARGIN_HIGH				 0x25 
#define	PMBUS_VOUT_MARGIN_LOW				 0x26 
#define	PMBUS_VOUT_TRANSITION_RATE			 0x27 
#define	PMBUS_VOUT_SCALE_LOOP				 0x29 
#define	PMBUS_VOUT_SCALE_MONITOR	 		 0x2A 
#define	PMBUS_COEFFICIENTS				 0x30  // FOLLOWED BY SUBCOMMAND THEN GOES TO COMMAND COEFFICIENTS
#define	PMBUS_VIN_ON 					 0x35 
#define	PMBUS_VIN_OFF 					 0x36 
#define	PMBUS_IOUT_CAL_GAIN 				 0x38 
#define	PMBUS_IOUT_CAL_OFFSET 				 0x39 
#define	PMBUS_FAN_CONFIG_1_2				 0x3A 
#define	PMBUS_FAN_COMMAND_1 				 0x3B 
#define	PMBUS_FAN_COMMAND_2 				 0x3C 
#define	PMBUS_VOUT_OV_FAULT_LIMIT 			 0x40 
#define	PMBUS_VOUT_OV_FAULT_RESPONSE		         0x41 
#define	PMBUS_VOUT_OV_WARN_LIMIT 			 0x42 
#define	PMBUS_VOUT_UV_WARN_LIMIT 			 0x43 
#define	PMBUS_VOUT_UV_FAULT_LIMIT 			 0x44 
#define	PMBUS_VOUT_UV_FAULT_RESPONSE		         0x45 
#define	PMBUS_IOUT_OC_FAULT_LIMIT 			 0x46 
#define	PMBUS_IOUT_OC_FAULT_RESPONSE		         0x47 
#define	PMBUS_IOUT_OC_LV_FAULT_LIMIT 		         0x48 									
#define	PMBUS_IOUT_OC_LV_FAULT_RESPONSE		         0x49 	
#define	PMBUS_IOUT_OC_WARN_LIMIT 			 0x4A 
#define	PMBUS_OT_FAULT_LIMIT 				 0x4F 
#define	PMBUS_OT_FAULT_RESPONSE				 0x50 
#define	PMBUS_OT_WARN_LIMIT 				 0x51 
#define	PMBUS_VIN_OV_FAULT_LIMIT 			 0x55 
#define	PMBUS_VIN_OV_FAULT_RESPONSE			 0x56 
#define	PMBUS_VIN_OV_WARN_LIMIT 			 0x57 
#define	PMBUS_VIN_UV_WARN_LIMIT 			 0x58 
#define	PMBUS_VIN_UV_FAULT_LIMIT 			 0x59 
#define	PMBUS_VIN_UV_FAULT_RESPONSE			 0x5A 
#define	PMBUS_IIN_OC_FAULT_LIMIT			 0x5B 
#define	PMBUS_IIN_OC_FAULT_RESPONSE			 0x5C 
#define	PMBUS_IIN_OC_WARN_LIMIT 			 0x5D 
#define	PMBUS_TON_DELAY 				 0x60 
#define	PMBUS_TON_RISE 					 0x61 
#define	PMBUS_TON_MAX_FAULT_LIMIT 			 0x62 
#define	PMBUS_TON_MAX_FAULT_RESPONSE		         0x63 
#define	PMBUS_TOFF_DELAY 				 0x64 
#define	PMBUS_TOFF_FALL 				 0x65 
#define	PMBUS_STATUS_BYTE				 0x78  
#define	PMBUS_STATUS_WORD 				 0x79 
#define	PMBUS_STATUS_VOUT				 0x7A 
#define	PMBUS_STATUS_IOUT				 0x7B 
#define	PMBUS_STATUS_INPUT				 0x7C 
#define	PMBUS_STATUS_TEMPERATURE			 0x7D 
#define	PMBUS_STATUS_CML				 0x7E 	
#define	PMBUS_STATUS_OTHER				 0x7F 
#define	PMBUS_STATUS_MFR_SPECIFIC			 0x80 
#define	PMBUS_STATUS_FANS_1_2				 0x81 
#define	PMBUS_READ_VIN 					 0x88 
#define	PMBUS_READ_IIN 					 0x89 
#define	PMBUS_READ_VOUT 				 0x8B 
#define	PMBUS_READ_IOUT 				 0x8C 
#define	PMBUS_READ_TEMPERATURE_1 			 0x8D 
#define	PMBUS_READ_TEMPERATURE_2 			 0x8E 
#define	PMBUS_READ_FANSPEED_1 				 0x90 
#define	PMBUS_READ_FANSPEED_2 				 0x91 
#define	PMBUS_PMBUS_REVISION				 0x98 
#define	PMBUS_MFR_ID 					 0x99 
#define	PMBUS_MFR_MODEL 				 0x9A 
#define	PMBUS_MFR_REVISION 				 0x9B 
#define	PMBUS_MFR_CATION 				 0x9C 
#define	PMBUS_MFR_DATE 					 0x9D 
#define	PMBUS_MFR_SERIAL 				 0x9E 
#define	PMBUS_MFR_VIN_MIN 				 0xA0 
#define	PMBUS_MFR_VIN_MAX 				 0xA1 
#define	PMBUS_MFR_IIN_MAX 				 0xA2 
#define	PMBUS_MFR_PIN_MAX 				 0xA3 
#define	PMBUS_MFR_VOUT_MIN 				 0xA4 
#define	PMBUS_MFR_VOUT_MAX 				 0xA5 
#define	PMBUS_MFR_IOUT_MAX 				 0xA6 
#define	PMBUS_MFR_POUT_MAX 				 0xA7 
#define	PMBUS_MFR_TAMBIENT_MAX 				 0xA8 
#define	PMBUS_MFR_TAMBIENT_MIN 				 0xA9 
#define	PMBUS_USER_DATA_00 				 0xB0 
#define	PMBUS_FAN_1_UNDERSPEED_WARN_LIMIT 	         0xD0 	
#define	PMBUS_FAN_1_UNDERSPEED_FAULT_LIMIT 	         0xD1 	
#define	PMBUS_FAN_2_UNDERSPEED_WARN_LIMIT 	         0xD2 
#define	PMBUS_FAN_2_UNDERSPEED_FAULT_LIMIT 	         0xD3 
#define	PMBUS_RETRY_TIME_MS	 			 0xD6  // the retry time in msec 	
#define	PMBUS_READ_USER_AD 				 0xD7
#define	PMBUS_TIME_UP	 	 			 0xD8 
#define PMBUS_TIME_RESET				 0xD9 
#define	PMBUS_CONFIG_COMMAND				 0xDA 
#define	PMBUS_ALERT_CONFIG				 0xDD  // ALERT bits for ALARM signal 
#define	PMBUS_I2C_ADDRESS 	 			 0xDE 
#define	PMBUS_PIP_VERSION	 			 0xDF  // Version data 2 bytes 
#define PMBUS_PIP_RESET					 0xE0  // PIP software reset command	 
#define PMBUS_PIP_PARM_READ				 0xFA 


// End PMBus commands

	
#define PMBUS_OPERATION_IMMEDIATE_OFF		0x00
#define PMBUS_OPERATION_SOFT_OFF		0x40
#define PMBUS_OPERATION_IMMEDIATE_ON		0x80
#define PMBUS_OPERATION_MARGIN_LOW_IGNORE	0x94
#define PMBUS_OPERATION_MARGIN_LOW_ACT		0x98
#define PMBUS_OPERATION_MARGIN_HIGH_IGNORE	0xA4
#define PMBUS_OPERATION_MARGIN_HIGH_ACT		0xA8

#define PMBUS_ON_OFF_TURN_OFF_DELAY		0x00
#define PMBUS_ON_OFF_TURN_OFF_NOW		0x01
#define PMBUS_ON_OFF_OPERATION_NOT_IGNORED	0x08
#define PMBUS_ON_OFF_POWER_UP_ANYTIME		0x10

#define RESPONSE_VOUT_UV_FAULT  0x01
#define RESPONSE_VOUT_OV_FAULT  0x02
#define RESPONSE_IOUT_OC_FAULT  0x04	
#define RESPONSE_TON_MAX_FAULT  0x08
#define RESPONSE_VIN_OV_FAULT  	0x80
#define RESPONSE_VIN_UV_FAULT  	0x10
#define RESPONSE_IIN_OC_FAULT  	0x20	
#define RESPONSE_OT_FAULT  	0x40


#define VOUT_OV_FAULT  0x80
#define VOUT_UV_FAULT  0x10
#define IOUT_OV_FAULT  0x80	
#define TON_MAX_FAULT  0x04

#define VIN_OV_FAULT  0x80
#define VIN_UV_FAULT  0x10
#define IIN_OV_FAULT  0x04	
#define OT_FAULT  	  0x80

//#define I2C_BUSY	   0x01		/** PIP8K is bussy no reseption of packets from I2C at this time, NACK all requests */
#define I2C_MSG	  	   0x02		/** indicates a write recieved that need to be procesed in the mail loop */
//#define I2C_ALERT	   0x04		/** need to respond to ARA command */		    
//#define I2C_ALERT_END  0x08		/** end of ARA switch back to normal I2C */
#define I2C_READ	   0x10		/** indicates if this is a host read */
//#define I2C_ERROR	   0x20		/** indicates I2C ERROR */
//#define I2C_START_READ 0x40		/** indicates  */
//#define I2C_ALERT_START 0x80
#define I2C_STOP	0x80
typedef struct {
	uint8_t operation             ; // 0
	uint8_t on_off_config         ; // 1
	uint8_t write_protect         ; // 2
	uint8_t vout_mode             ; // 3
	uint16_t vout_command_scale      ; // 4
	uint16_t  vout_command_offset     ; // 6
	uint8_t vout_command_r        ; // 8
	uint16_t vout_command             ; // 16
	/* not used */
	uint8_t i2c_address_and_pin           ; // 18
	uint16_t vout_trim                        ; // 19
	uint16_t vout_cal                         ; // 20
	/* end not used */
	uint8_t capability            ; // 22
	uint16_t vout_max                 ; // 24
	uint16_t vout_margin_high         ; // 26
	uint16_t vout_margin_low          ; // 28
	/* not used */
	uint16_t vout_transition_rate             ; // 30
	uint16_t vout_scale_loop                  ; // 32
	uint16_t vout_scale_monitor               ; // 34
	/* end not used */
	uint16_t vin_on                   ; // 36
	uint16_t vin_off                  ; // 38
	/* not used */
	uint16_t iout_cal_gain                    ; // 40
	uint16_t iout_cal_offset                  ; // 42
	/* end not used */
	uint8_t fan_config_1_2        ; // 43
	uint16_t fan_command_1            ; // 44
	uint16_t fan_command_2            ; // 46
	uint16_t vout_ov_fault_limit      ; // 48
	uint8_t vout_ov_fault_response  ; // 50
	uint16_t vout_ov_warn_limit       ; // 48
	uint16_t vout_uv_warn_limit       ; // 50
	uint16_t vout_uv_fault_limit      ; // 52
	uint8_t vout_uv_fault_response     ; // 54  ; //15
	uint16_t iout_oc_fault_limit       ; //55
	uint8_t iout_oc_fault_response      ; //57  ; //14
	uint16_t iout_oc_warn_limit       ; // 58
	uint16_t ot_fault_limit           ; // 60
	uint8_t ot_fault_response          ; // 62  ; //18
	uint16_t ot_warn_limit             ; //63
	uint16_t vin_ov_fault_limit        ; //65
	uint8_t vin_ov_fault_response      ; // 67  ; //19
	uint16_t vin_ov_warn_limit        ; // 68
	uint16_t vin_uv_warn_limit        ; // 70
	uint16_t vin_uv_fault_limit       ; // 72
	uint8_t vin_uv_fault_response      ; // 74  ; //16
	uint16_t iin_oc_fault_limit       ; // 75
	uint8_t iin_oc_fault_response      ; // 77  ; //15
	uint16_t iin_oc_warn_limit        ; // 78
	uint16_t ton_delay                ; // 80
	/* not used */
	uint16_t ton_rise      ;
	/* end not used */
	uint16_t ton_max_fault_limit      ; // 84
	uint8_t ton_max_fault_response     ; // 86  ; //13
	uint16_t toff_delay               ; // 87
	/* not used */
	uint16_t toff_fall      ;
	/* end not used */
	uint8_t status_fans_1_2            ; // 111 ; //16
	uint16_t status_word              ; // 112
	uint8_t status_vout             ; //    118 ; //03
	uint8_t status_iout             ; //    119 ; //03
	uint8_t status_input             ; //   120 ; //03
	uint8_t status_temperature        ; //  121 ; //03
	uint8_t status_cml                ; //  122 ; //03
	uint8_t status_other              ; //  123 ; //03
	uint8_t status_mfr_specific        ; // 124 ; //03
	uint8_t pmbus_reversion_dummy	; //	125
	uint16_t read_vin 		; //			126
	uint16_t read_iin 		; //			128
	uint16_t read_vout 		; //		130
	uint16_t read_iout 		; //		132
	uint16_t read_temperature_1 	; //	134
	uint16_t read_temperature_2 	; //	136
	uint16_t read_fan1                  ; //	114
	uint16_t read_fan2                ; // 	116
	uint16_t mfr_id                   ; // 138    //14
	uint16_t mfr_model                ; // 140
	uint16_t mfr_revision             ; // 142
	uint16_t mfr_location              ; //144
	uint16_t mfr_date                  ; //146
	uint16_t mfr_serial               ; // 170
	uint16_t mfr_vin_min               ; //148
	uint16_t mfr_vin_max               ; //150
	uint16_t mfr_iin_max               ; //152
	uint16_t mfr_pin_max               ; //1/54
	uint16_t mfr_vout_min              ; //156
	uint16_t mfr_vout_max              ; //158
	uint8_t pmbus_revision         ; //	125
	uint16_t mfr_iout_max              ; //160
	uint16_t mfr_pout_max              ; //162
	uint16_t mfr_tambient_max          ; //164
	uint16_t mfr_tambient_min          ; //166
	uint16_t user_data_00              ; //168

	/* not used */
	uint8_t mfr_serial_end     ;
	/* not used */
	uint16_t read_iin_scale           ; // 101
	uint16_t read_iin_offset          ; // 103
	uint8_t read_iin_r                ; //  105
	uint16_t read_temp_1_scale        ; // 106
	uint16_t read_temp_1_offset       ; // 108
	uint8_t read_temp_1_r              ; // 110
	uint16_t read_user_ad_scale        ; //172
	uint16_t read_user_ad_offset       ; //174
	uint8_t read_user_ad_r             ; // 176
	uint16_t fan_1_us_warn_limit       ; //183
	uint16_t fan_1_us_fault_limit      ; //185
	uint16_t fan_2_us_warn_limit       ; //187
	uint16_t fan_2_us_fault_limit      ; //189
	uint8_t retry_time_ms          ; //191 ; //23
	uint8_t config                 ; //192 ; //26
	uint16_t read_temp_2_scale        ; // 177
	uint16_t read_temp_2_offset       ; // 179
	uint8_t read_temp_2_r              ; // 181
	uint16_t read_vout_scale          ; // 10
	uint16_t read_vout_offset         ; // 12
	uint8_t read_vout_r           ; // 14
	uint8_t alert_config           ; //193 ; //28
	uint8_t i2c_address            ; //  1; //94 ; //28
	uint16_t read_user_ad 	;	 //		195
	uint16_t version 		; //		    197

	uint16_t read_iout_scale          ; // 91
	uint16_t read_iout_offset         ; // 93
	uint8_t read_iout_r                ; // 95
	uint16_t read_vin_scale           ; // 96
	uint16_t read_vin_offset          ; // 98
	uint8_t read_vin_r                ; //  100

} pmbus_commands;

typedef struct {
	uint16_t  read1;
	uint16_t ton_max;
	uint16_t fan_count_1;      // interim fan speed count
	uint16_t fan_count_2;      // interim fan speed count
	uint8_t coefficients;
	uint8_t crc_temp;
	uint8_t sign;   //
	uint8_t addr;				// used as NVRAM address counter
} calculation_temperaty_value;	

typedef struct {
	uint8_t inprogress;	    //* when = 0 no fault response now serviced; set to the PARMS[value] of the fault response
	uint8_t inprogress_response;	//* when = 0 no fault response now serviced; set to the PARMS[value] of the fault response
	uint8_t wait_for_clear;
	uint8_t still_fault;	//* internal variable to check is fault condition is removed PARM[fault] can only be removed by host
	uint8_t required;		//* indicates is a responce is required
	uint8_t	resp_choice;	//* set to the PARMS[xxx_fault_response]
	uint8_t	dly_seconds; 	//* response delay between retries
	uint8_t	num_retries; 	//* response retry limit
	uint8_t pwr_good;		//* indicates if output is up and fault checks should happen
	uint16_t dly_ms;			//* delay in msec
	uint8_t status_bits;    /* 00001 used to schedule actions every 1 sec
									// 00010 indicats is a fan measurement in ongoing. 
									// 00100 used for NVRAM write OK 
									// 01000 VOUT_COMMAND or OPERATION change
									// 10000 Configuration file corrupt or not inizialized error   */
} response_struct;

typedef struct {
	uint8_t crc_of_i2c_addr;	// The I2C slave address CRC-8 value I need for
	uint8_t i2c_address_and_pin;
	uint8_t status;		// I2C status for ALERT, ALERT_END, MSG received, Busy
	uint8_t crc;			// CRC-8 byte based on C(x) = x8 + x2 + x + 1
	uint8_t buf_indx;		// index for data input
	uint8_t buf_indx_out;	//
	uint8_t buf_debug;	//
	uint8_t buf[IIC_BUF_SIZE];		// I2C buffer
} i2c_struct;

typedef struct {
	float X;	//  
	float Temp;	// 
	char N;		// 
	int Y;		// 
	uint8_t sign;   //
} liniar_struct;

typedef struct {
	uint16_t  scale    ;
	uint16_t  offset   ;
	uint8_t r        ;
} coefficients_struct;

typedef struct timeStruct  {                      /**< Time Structure */
	uint16_t Hour;
	uint8_t Minute;
	uint8_t Second;
} timeObject_struct;


/* Function Prototypes - Applications */
void drivers_init(void);
void process_cmd(uint8_t cmd);
void CRC8_BlockChecksum( uint8_t length );

void read_or_store_parms (uint8_t bytes);
void ReadEEPROM(uint16_t adr_start,uint16_t adr_end);
void WriteEEPROM(uint16_t adr_start,uint16_t adr_end);

void check_out (void);
void check_in (void);
void check_fan (void);
void check_temp (void);
void read_fans (void);
void read_ADC (void);


void vout_volt_set(char bit);
void ms_timer(uint16_t dly_cnt);
void PMBusTimer (void);


void fault_response(void);
void power_up_or_down(void);
void PMBus_Defaults( void );



#endif // _NXP_PMBUS_H_
