#define  DOOR_FIXED 				_:VCF_DOOR_FIXED
#define  DOOR_OPENED 				_:VCF_DOOR_OPENED
#define  DOOR_DAMAGED 				_:VCF_DOOR_DAMAGED
#define  DOOR_REMOVED 				_:VCF_DOOR_REMOVED
#define  VCF_DOOR_OPENED_DAMAGED 	0b11

#define  LIGHT_FRONT 	Light:VCF_LIGHT_FRONT
#define  LIGHT_BACK 	Light:VCF_LIGHT_BACK
#define  LIGHT_RIGHT 	LightSide:VCF_LIGHT_RIGHT
#define  LIGHT_LEFT 	LightSide:VCF_LIGHT_LEFT

enum Light {
	VCF_LIGHT_FRONT,
	VCF_LIGHT_BACK
}

enum LightSide( <<= 2 ) {
	VCF_LIGHT_LEFT = 1,
	VCF_LIGHT_RIGHT
}

enum Door {
	DOOR_HOOD,
	DOOR_TRUNK,
	DOOR_DRIVER,
	DOOR_PASSENGER
}

enum DoorState(<<=1) {
	VCF_DOOR_FIXED = 0,
	VCF_DOOR_OPENED = 1,
	VCF_DOOR_DAMAGED,
	VCF_DOOR_REMOVED
}

stock IsLightEnabled(vehicleid, Light:light, LightSide:lightSide) {

	if(0 > vehicleid > MAX_VEHICLES)
		return false;

	new VCF_panels, VCF_doors, VCF_lights, VCF_tires;
	GetVehicleDamageStatus(vehicleid, VCF_panels, VCF_doors, VCF_lights, VCF_tires);

	return ((VCF_lights >>> (4 * _:light)) & _:lightSide) ^ _:lightSide;
}

stock AlterLightState(vehicleid, Light:light, LightSide:lightSide) {

	if(0 > vehicleid > MAX_VEHICLES)
		return false;

	new VCF_panels, VCF_doors, VCF_lights, VCF_tires;
	GetVehicleDamageStatus(vehicleid, VCF_panels, VCF_doors, VCF_lights, VCF_tires);

	VCF_lights = VCF_lights ^ (1 << ((4 * _:light) + ((lightSide == LIGHT_RIGHT) ? (2) : (0))));

	UpdateVehicleDamageStatus(vehicleid, VCF_panels, VCF_doors, VCF_lights, VCF_tires);
	
	return true;
}

stock GetDoorState(vehicleid, Door:door) {

	if(0 > vehicleid > MAX_VEHICLES)
		return false;

	new VCF_panels, VCF_doors, VCF_lights, VCF_tires;
	GetVehicleDamageStatus(vehicleid, VCF_panels, VCF_doors, VCF_lights, VCF_tires);

	return (VCF_doors >>> (8 * _:door)) & (0b111);
}

stock SetDoorState(vehicleid, Door:door, doorState) {

	if(0 > vehicleid > MAX_VEHICLES)
		return false;


	if(_:door == _:DOOR_TRUNK && doorState == DOOR_OPENED)
		return false;

	new VCF_panels, VCF_doors, VCF_lights, VCF_tires;
	GetVehicleDamageStatus(vehicleid, VCF_panels, VCF_doors, VCF_lights, VCF_tires);

	new _:VCF_tmp = (_:GetDoorState(vehicleid, door) << (8 * _:door)) ^ VCF_doors;

	VCF_doors = (doorState << (8 * _:door)) | _:VCF_tmp;
	
	if(doorState == DOOR_FIXED) {
		new Float:VCF_health;
		GetVehicleHealth(vehicleid, VCF_health);
		RepairVehicle(vehicleid);
		UpdateVehicleDamageStatus(vehicleid, VCF_panels, VCF_doors, VCF_lights, VCF_tires);
		SetVehicleHealth(vehicleid, VCF_health);
	} else 
		UpdateVehicleDamageStatus(vehicleid, VCF_panels, VCF_doors, VCF_lights, VCF_tires);
	
	return true;
}

stock GetVehicleRotation(vehicleid, &Float:rx, &Float:ry, &Float:rz) {

	if(0 > vehicleid > MAX_VEHICLES)
		return false;

    new Float:qw, Float:qx, Float:qy, Float:qz;
    GetVehicleRotationQuat(vehicleid, qw, qx, qy, qz);
    rx = asin(2 * qy * qz - 2 * qx * qw);
    ry = -atan2(qx * qz + qy * qw, 0.5 - qx * qx - qy * qy);
    rz = -atan2(qx * qy + qz * qw, 0.5 - qx * qx - qz * qz);

    return true;
}