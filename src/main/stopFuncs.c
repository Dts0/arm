#include "config.h"
#include "defines.h"

void motor0_TieBi_stopFunc()
{
	while(getTieBiPosition()!=open);//等待汽缸达到90mm位置
}
void motor4_CeDangBan_stopFunc()
{
	while(getCeDangBanPosition()!=open);//等待汽缸达到120mm位置
}

