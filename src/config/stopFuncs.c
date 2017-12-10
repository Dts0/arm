#include "config.h"
#include "../main/defines.h"

void motor0_TieBi_stopFunc(void)
{
	while(getTieBiPosition()!=open);//等待汽缸达到90mm位置
}
void motor4_CeDangBan_stopFunc(void)
{
	while(getCeDangBanPosition()!=open);//等待汽缸达到120mm位置
}

