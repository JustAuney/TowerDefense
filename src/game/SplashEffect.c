#include "game/SplashEffect.h"

bool splash_should_apply(SplashEffect effect, double distance) {
	switch (effect) {
		case NONE:
			return false;
		case PYRO:
			return distance < SQUARED_PYRO_RADIUS;
		case VAPORIZATION:
			return distance < SQUARED_VAPORIZATION_RADIUS;
	}
	__builtin_unreachable();
}
