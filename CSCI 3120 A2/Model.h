#ifndef _Model_h_
#define _Model_h_

#include "dataStructure.h"

int modelMessageReady();
int modelSync(CntrlToModel *msg);
void *modelPoll(void *shmidMSGRD);
int parseMSG(CntrlToModel *msg);

#endif
