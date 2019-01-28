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
	char *photo;
	uint32_t photoLength;
	char *feature;
	uint32_t featureLength;
}driverInfo;

uint32_t DriverCenter_getCount();
uint32_t DriverCenter_getFeature(uint8_t *feature, char *driverName);
uint32_t DriverCenter_getPhoto(uint8_t *photo, char *driverName);
uint32_t DriverCenter_getDriverInfo(driverInfo *driverInfos, uint32_t driverInfoLength);
uint32_t DriverCenter_registerDriver(driverInfo driverInfo);
uint32_t DriverCenter_deleteDriver(char *driverName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
