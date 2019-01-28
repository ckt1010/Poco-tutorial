#include "sqlite-c.h"
#include "stdio.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

void printDriverInfo(driverInfo *printDriverInfo, uint32_t printLength) {
    printf("Print driverInfo:");
    for (uint8_t i = 0; i < printLength; i++) {
        printf("name %s, nickName %s\n", printDriverInfo[i].name,
               printDriverInfo[i].nickName);
        if (0 != printDriverInfo[i].featureLength) {
            printf("feature: %s, length: %u\n", printDriverInfo[i].feature,
                   printDriverInfo[i].featureLength);
        }
        if (0 != printDriverInfo[i].photoLength) {
            char fileName[100];
            snprintf(fileName, 100, "%s_%s.jpg", printDriverInfo[i].name,
                     printDriverInfo[i].nickName);
			printf("photoLength: %u\n", printDriverInfo[i].photoLength);
            FILE *file = fopen(fileName, "wb");
            fwrite(printDriverInfo[i].photo, printDriverInfo[i].photoLength, 1,
                   file);
            fclose(file);
        }
    }
}

void insertDriverInfo(char *name, char *nickName, char *feature) {
	FILE *photoFile;
	driverInfo driverInfoSet;
    uint32_t photoFilePtr = 0;
    strcpy(driverInfoSet.name, name);
    strcpy(driverInfoSet.nickName, nickName);
    driverInfoSet.feature = (uint8_t *)malloc(512);
    driverInfoSet.featureSize = 512;
    strcpy((char *)driverInfoSet.feature, feature);
	driverInfoSet.featureLength = strlen(feature);
    driverInfoSet.photo = (uint8_t *)malloc(100 * 1000 * 1000);
	driverInfoSet.photoSize = 100 * 1000 * 1000;
    photoFile = fopen("./1.jpg", "rb");
    while (1000 == fread(&driverInfoSet.photo[photoFilePtr], 1, 1000, photoFile)) {
        photoFilePtr += 1000;
    }
    driverInfoSet.photoLength = photoFilePtr;
    fclose(photoFile);
    DriverCenter_registerDriver(driverInfoSet);
	free(driverInfoSet.feature);
    free(driverInfoSet.photo);
}
int main() {
    uint32_t count = 0;
    driverInfo driverInfoSet, driverInfoGet[10];
    for (uint8_t i = 0; i < 10; i++) {
        driverInfoGet[i].feature = (uint8_t *)malloc(512);
		driverInfoGet[i].featureSize = 512;
        driverInfoGet[i].photo = (uint8_t *)malloc(100 * 1000 * 1000);
		driverInfoGet[i].photoSize = 100 * 1000 * 1000;
    }

	printf("\n-------------empty test---------------\n");
    count = DriverCenter_getCount();
    printf("count: %u\n", count);
	DriverCenter_getDriverInfo(driverInfoGet, 10);
	printDriverInfo(driverInfoGet, count);


	printf("\n-------------without feature and photo test---------------\n");
    strcpy(driverInfoSet.name, "test1");
    strcpy(driverInfoSet.nickName, "test1_nickName");
    DriverCenter_registerDriver(driverInfoSet);
    count = DriverCenter_getCount();
    printf("count: %u\n", count);
	DriverCenter_getDriverInfo(driverInfoGet, 10);
	printDriverInfo(driverInfoGet, count);

	printf("\n-------------with feature and photo & get all test---------------\n");
 	insertDriverInfo("test2", "test2_nickName", "test2 feature input");
    count = DriverCenter_getCount();
    printf("count: %u\n", count);
	DriverCenter_getDriverInfo(driverInfoGet, 10);
	printDriverInfo(driverInfoGet, count);

	printf("\n-------------with feature and photo & get feature test---------------\n");
	uint8_t getFeature[512];
 	insertDriverInfo("test3", "test3_nickName", "test3 feature input");
    count = DriverCenter_getCount();
    printf("count: %u\n", count);
	DriverCenter_getDriverInfo(driverInfoGet, 10);
	printDriverInfo(driverInfoGet, count);
	DriverCenter_getFeature(getFeature, 512, "test3");
	printf("feature: %s\n", getFeature);

	printf("\n-------------with feature and photo & get photo test---------------\n");
	uint8_t photo[20000];
 	insertDriverInfo("test4", "test4_nickName", "test4 feature input");
    count = DriverCenter_getCount();
    printf("count: %u\n", count);
	DriverCenter_getPhoto(photo, 20000, "test4");
	FILE *file = fopen("test4.jpg", "wb");
    fwrite(photo, 20000, 1, file);
    fclose(file);

	printf("\n-------------delete driver test---------------\n");
    DriverCenter_deleteDriver("test1");
    DriverCenter_deleteDriver("test2");
	DriverCenter_deleteDriver("test3");
	DriverCenter_deleteDriver("test4");
    count = DriverCenter_getCount();
    printf("count: %u\n", count);
    DriverCenter_stop();

	for (uint8_t i = 0; i < 10; i++) {
        free(driverInfoGet[i].feature);
        free(driverInfoGet[i].photo);
    }
    return 0;
}