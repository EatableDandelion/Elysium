#pragma once

#include <memory>
#include <unordered_map>
#include "Math.h"
#include "Profile.h"


#define CIRCE_STRING_ID(text) std::hash<std::string>{}(text)
#define CIRCE_INFO(text) {Circe::log.print("INFO", __FILE__, __LINE__, text);}
#define CIRCE_WARNING(text) {Circe::log.print("WARNING", __FILE__, __LINE__, text);}
#define CIRCE_ERROR(text) {Circe::log.print("ERROR", __FILE__, __LINE__, text);}
#define CIRCE_PRINT(text) {Circe::log.print(text);}


#define DIMENSION 2

using Circe::Real;
using Circe::Vec2;
using Circe::Vec3;
using Circe::Complex;
using Circe::Quaternion;
using Circe::Mat;

using Vec 			= Circe::Vec<DIMENSION>;
using Vec2 			= Circe::Vec<2>;
using Vec3 		 	= Circe::Vec<3>;
using Transform 	= std::shared_ptr<Circe::Trans<DIMENSION>>;
using Transform2 	= std::shared_ptr<Circe::Trans<2>>;
using Transform3 	= std::shared_ptr<Circe::Trans<3>>;

template<typename K, typename V>
using map = std::unordered_map<K,V>;

namespace Circe
{
	const double PI = 3.1415926535897932384626433832795;
}
