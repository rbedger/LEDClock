// swirl.h

#ifndef _SWIRL_h
#define _SWIRL_h

#include <FastLED.h>
#include "ledutils.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Swirl {
public:
	Swirl(Ledutils& ledutils);
	void handle(CRGB* leds);

private:
	Ledutils _ledutils;
	unsigned long _lastExecutionTime;
	int _count;
	int _indices[105] = {
		52,
		53,
		30,
		31,
		32,
		51,
		74,
		73,
		72,
		71,
		54,
		29,
		12,
		11,
		10,
		9,
		8,
		33,
		50,
		75,
		92,
		93,
		94,
		95,
		96,
		97,
		70,
		55,
		28,
		13,
		7,
		34,
		49,
		76,
		91,
		98,
		69,
		56,
		27,
		14,
		6,
		35,
		48,
		77,
		90,
		99,
		68,
		57,
		26,
		15,
		5,
		36,
		47,
		78,
		89,
		100,
		67,
		58,
		25,
		16,
		4,
		37,
		46,
		79,
		88,
		101,
		66,
		59,
		24,
		17,
		3,
		38,
		45,
		80,
		87,
		102,
		65,
		60,
		23,
		18,
		2,
		39,
		44,
		81,
		86,
		103,
		64,
		61,
		22,
		19,
		1,
		40,
		43,
		82,
		85,
		104,
		63,
		62,
		21,
		20,
		0,
		41,
		42,
		83,
		84,
	};
	int* _current;
};

#endif

