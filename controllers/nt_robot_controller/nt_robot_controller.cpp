#include <webots/Robot.hpp>
#include <networktables/NetworkTableInstance.h>
#include <nlohmann/json.hpp>
#include "NTMotor.h"

using namespace webots;

int main(int argc, char **argv) {
 Robot* robot = new Robot();

  auto ntInst = nt::NetworkTableInstance::GetDefault();
  int timeStep = (int)robot->getBasicTimeStep();

  std::vector<NTMotor> motors;

  nlohmann::json j;

  for (int i = 1; i < argc; i++) {
	  try
	  {
		  j = nlohmann::json::parse(argv[i]);
		  if (j.at("type") == "motor") {
			  NTMotor::Config conf = j.at("value").template get<NTMotor::Config>();
			  std::cout << conf.GearRatio << std::endl;
			  motors.emplace_back(robot, conf);
		  }
	  }
	  catch (const std::exception& e)
	  {
		  std::cerr << e.what() << std::endl;
	  }
	 
  }

  bool initialized = false;

  while (robot->step(timeStep) != -1) {
	  if (!initialized) {
		  for (auto& motor : motors) {
			  motor.Init();
		  }
		  initialized = true;
	  } else {
		  for (auto& motor : motors) {
			  motor.Update();
		  }
	  }
  };

  delete robot;
  return 0;
}
