# VCF

[![sampctl](https://shields.southcla.ws/badge/sampctl-VCF-2f2f2f.svg?style=for-the-badge)](https://github.com/MrKnockout/VCF)

## Installation

Simply install to your project:

```bash
sampctl package install MrKnockout/VCF
```

Include in your code and begin using the library:

```pawn
#include <VCF>
```

## Usage

```pawn
// Check this light is on or off  
IsLightEnabled(vehicleid, light, lightSide); 

// Changes the light status  
ToogleLightState(vehicleid, light, lightSide); 

/* Get the current state of one of the vehicle doors  
Note: Hood and trunk are considered doors in SA-MP */  
GetDoorState(vehicleid, door); 

// Changes the state of one of the vehicle doors  
SetDoorState(vehicleid, door, doorState); 

// Check that the tire is inflated 
IsTireInflated(vehicleid, tire); 

// Change tire condition  
ToogleTireState(vehicleid, tire);  
```

## Testing

To test, simply run the package:

```bash
sampctl package run
```
