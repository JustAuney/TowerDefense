#pragma once

#include <stdbool.h>

#include "utils/constants.h"

typedef enum _splash_effect {
	NONE,
	PYRO,
	VAPORIZATION
} SplashEffect;

bool splash_should_apply(SplashEffect effect, double distance);
