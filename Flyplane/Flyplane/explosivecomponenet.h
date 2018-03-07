#pragma once


struct ExplosiveComponenet {
	ExplosiveComponenet(double explodeRadius, double explodeDamage) : explodeDamage(explodeDamage), explodeRadius(explodeRadius) {}
	double explodeRadius = 10, explodeDamage = 100;
};