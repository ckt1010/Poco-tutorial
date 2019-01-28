#ifndef _SQLITE_C_H__
#define _SQLITE_C_H__
#ifdef __cplusplus

#include <iostream>

extern "C" {
#endif
#define DRIVERCENTER_MAX_NAME_LENGTH 50
typedef struct {
	char name[DRIVERCENTER_MAX_NAME_LENGTH];
	char nickName[DRIVERCENTER_MAX_NAME_LENGTH];
	uint8_t *photo;
	uint32_t photoSize;
	uint32_t photoLength;
	uint8_t *feature;
	uint32_t featureSize;
	uint32_t featureLength;
}driverInfo;

uint32_t DriverCenter_getCount();
uint32_t DriverCenter_getFeature(uint8_t *feature, uint32_t featureLength, char *driverName);
uint32_t DriverCenter_getPhoto(uint8_t *photo, uint32_t photoLength, char *driverName);
uint32_t DriverCenter_getDriverInfo(driverInfo *driverInfos, uint32_t driverInfoLength);
uint32_t DriverCenter_registerDriver(driverInfo driverInfo);
uint32_t DriverCenter_deleteDriver(char *driverName);
void DriverCenter_stop();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
