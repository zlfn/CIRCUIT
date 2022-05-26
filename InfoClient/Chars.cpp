//GNU GPL 3.0 lisence
/*
 * Á¤º¸ ¼öÇàÆò°¡ °ÔÀÓ
 * Copyright (C) 2022 ¹ÚÂù¿õ, ±èÁø¼­, ¹ÚÁöÈ¯
 *
 * ÀÌ ÇÁ·Î±×·¥Àº ÀÚÀ¯ ¼ÒÇÁÆ®¿þ¾îÀÔ´Ï´Ù. ¼ÒÇÁÆ®¿þ¾îÀÇ ÇÇ¾çµµÀÚ´Â ÀÚÀ¯ ¼ÒÇÁÆ®¿þ¾î
 * Àç´ÜÀÌ °øÇ¥ÇÑ GNU ÀÏ¹Ý °øÁß »ç¿ë Çã°¡¼­ 3ÆÇ È¤Àº ±× ÀÌÈÄ ÆÇÀ» ÀÓÀÇ·Î ¼±ÅÃÇÏ¿©
 * ±× ±ÔÁ¤¿¡ µû¶ó ÇÁ·Î±×·¥À» °³ÀÛÇÏ°Å³ª Àç¹èÆ÷ÇÒ ¼ö ÀÖ½À´Ï´Ù.
 *
 * ÀÌ ÇÁ·Î±×·¥Àº À¯¿ëÇÏ°Ô »ç¿ëµÉ ¼ö ÀÖÀ¸¸®¶ó´Â Èñ¸Á¿¡¼­ ¹èÆ÷µÇ°í ÀÖÁö¸¸, Æ¯Á¤ÇÑ
 * ¸ñÀû¿¡ ¸Â´Â ÀûÇÕ¼º ¿©ºÎ³ª ÆÇ¸Å¿ëÀ¸·Î »ç¿ëÇÒ ¼ö ÀÖÀ¸¸®¶ó´Â ¹¬½ÃÀûÀÎ º¸ÁõÀ» Æ÷ÇÔÇÑ
 * ¾î¶°ÇÑ ÇüÅÂÀÇ º¸Áõµµ Á¦°øÇÏÁö ¾Ê½À´Ï´Ù. º¸´Ù ÀÚ¼¼ÇÑ »çÇ×¿¡ ´ëÇØ¼­´Â
 * GNU ÀÏ¹Ý °øÁß Çã°¡¼­¸¦ Âü°íÇÏ½Ã±â ¹Ù¶ø´Ï´Ù.
 *
 * GNU ÀÏ¹Ý °øÁß »ç¿ë Çã°¡¼­´Â ÀÌ ÇÁ·Î±×·¥°ú ÇÔ²² Á¦°øµË´Ï´Ù. ¸¸¾à ¹®¼­°¡ ´©¶ôµÇ¾îÀÖ´Ù¸é
 * <http://www.gnu.org/licenses/>À» ÂüÁ¶ÇÏ½Ã±â ¹Ù¶ø´Ï´Ù.
 */

//ÄÜ¼Ö¿¡¼­ ÇÑ±ÛÀ» Æ÷ÇÔÇÑ À¯´ÏÄÚµå¸¦ ÀÌ¿ëÇÏ±â À§ÇÑ ÇÔ¼ö¸¦ ¸ð¾Æ³õÀº ÆÄÀÏÀÔ´Ï´Ù.
#include <iostream>
#include <Windows.h>
#include "Chars.h"

int initWchar(void)
{
	setlocale(LC_ALL, ".UTF-8");
	return 0;
}

bool isWide(wchar s)
{
	//(¿¾ÇÑ±ÛÀ» Á¦¿ÜÇÏ¸é) ÇÑ±Û À¯´ÏÄÚµåÀÇ Ã¹±ÛÀÚ´Â °¡, ³¡ ±ÛÀÚ´Â ÆRÀÔ´Ï´Ù.
	const wchar start = L'°¡';
	const wchar end = L'ÆR';
	if(s >= start && s <= end) return true;
	return false;
}
