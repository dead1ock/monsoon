/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#include <Math/Vector.h>

struct GcsCoord
{
	GcsCoord(int _degrees, int _minutes, int _seconds)
		: degrees(_degrees)
		, minutes(_minutes)
		, seconds(_seconds) {

	}

	GcsCoord()
		: degrees(0)
		, minutes(0)
		, seconds(0) {

	}

	int degrees;
	int minutes;
	int seconds;

	float ToDecimal() {
		return degrees + (minutes / 60.0f) + (seconds / 60.0f);
	}
};

struct GcsLocation
{
	GcsLocation(GcsCoord _latitude, GcsCoord _longitude)
		: latitude(_latitude)
		, longitude(_longitude) { }

	GcsLocation() { }

	GcsCoord latitude;
	GcsCoord longitude;
};

Monsoon::Math::Vector2 GcsToCartesian(GcsCoord latitude, GcsCoord longitude)
{
	Monsoon::Math::Vector2 cartesian(0.0f, 0.0f);
	cartesian.mX = latitude.ToDecimal();
	cartesian.mY = longitude.ToDecimal();
	return cartesian;
}

Monsoon::Math::Vector2 GcsToCartesian(GcsLocation location)
{
	Monsoon::Math::Vector2 cartesian(0.0f, 0.0f);
	cartesian.mX = location.latitude.ToDecimal();
	cartesian.mY = location.longitude.ToDecimal();
	return cartesian;
}

GcsLocation CartesianToGcs(Monsoon::Math::Vector2 cartesian)
{
	GcsLocation gcs;
	gcs.latitude.degrees = (int)cartesian.mX;
	gcs.latitude.minutes = 60 * (cartesian.mX - gcs.latitude.degrees);
	gcs.latitude.seconds = 3600 * (cartesian.mX - gcs.latitude.degrees) - (60 * (gcs.latitude.minutes));
	gcs.longitude.degrees = (int)cartesian.mY;
	gcs.longitude.minutes = 60 * (cartesian.mY - gcs.longitude.degrees);
	gcs.longitude.seconds = 3600 * (cartesian.mY - gcs.longitude.degrees) - (60 * (gcs.longitude.minutes));
	return gcs;
}

float MetersToCartesian(float meters) {
	return (meters / 20.0f);
}