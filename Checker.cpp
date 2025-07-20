#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
using namespace std;


#define NUMBER_OF_PARAMETERS 3

typedef enum
{
  NORMAL_RANGE_E,
  LOW_RANGE_E,
  HIGH_RANGE_E
}battParamRangeType_e;

typedef struct {
  const char* name;
  float value;
  float lowerLimit;
  float upperLimit;
  battParamRangeType_e battStatus;
} batteryHeath_ts;

//Check Battery Paramters range
battParamRangeType_e checkParamRange (float paramValue, float lowLimit, float highLimit)
{
  if(paramValue < lowLimit)
  {
    return LOW_RANGE_E;
  }
  else if(paramValue > highLimit)
  {
    return HIGH_RANGE_E;
  }
  else
  {
    return NORMAL_RANGE_E;
  }
}

//Compute result
void reportBatteryHeathStatus(const batteryHeath_ts* heathStatus)
{
  if(heathStatus->battStatus != NORMAL_RANGE_E)
  {
    const char* level = (heathStatus->battStatus == LOW_RANGE_E) ? "LOW" : "HIGH";
      printf("%s is %s! Value: %.2f, Expected range: [%.2f - %.2f]\n",
               heathStatus->name, level, heathStatus->value, heathStatus->lowerLimit, heathStatus->upperLimit);
  }
}

bool batteryIsOk(float temperature, float soc, float chargeRate) 
{
  bool battStatus = true;

  batteryHeath_ts checkBattParam[NUMBER_OF_PARAMETERS] = 
  {
    {"Temperature", temperature, 0, 45, NORMAL_RANGE_E},
    {"State of Charge", soc, 20, 80, NORMAL_RANGE_E},
    {"Charge Rate", chargeRate, 0, 0.8, NORMAL_RANGE_E}
  };

    
  for(int i = 0; i < NUMBER_OF_PARAMETERS; ++i) 
  {
    checkBattParam[i].battStatus = checkParamRange(checkBattParam[i].value,
                                    checkBattParam[i].lowerLimit,
                                    checkBattParam[i].upperLimit);
                                    reportBatteryHeathStatus(&checkBattParam[i]);

    if(checkBattParam[i].battStatus != NORMAL_RANGE_E) 
    {
      battStatus = false;
    }
    }
    return battStatus;
}

void testBatteryLimit() 
{
    // Good case
    assert(batteryIsOk(25, 70, 0.7) == true);

    // Failed cases
    assert(batteryIsOk(-1, 50, 0.7) == false);  // Temp low
    assert(batteryIsOk(48, 70, 0.6) == false);  // Temp high
    assert(batteryIsOk(32, 18, 0.5) == false);  // SOC low
    assert(batteryIsOk(33, 81, 0.4) == false);  // SOC high
    assert(batteryIsOk(40, 65, 0.9) == false);  // Charge rate high
    assert(batteryIsOk(-3, 15, 1.0) == false);  // All parameters out of range

    // Boundary check
    assert(batteryIsOk(0, 20, 0.0) == true);
    assert(batteryIsOk(45, 80, 0.8) == true);

    // Edge + one parameter fail scenario
    assert(batteryIsOk(44, 80, 0.8) == false);   // Temp high
    assert(batteryIsOk(0, 80, 0.9) == false);    // Charge rate high
    assert(batteryIsOk(45, 19, 0.7) == false);   // Soc high
}


int main() {
  testBatteryLimit();
  printf("Battery test passed successfully.\n");
}
