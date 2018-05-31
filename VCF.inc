#if __Pawn != 0x030A // != Zeex pawn
	enum VCF_INFO {
#else
	#if defined __PawnBuild // Zeex pawn 3.10.1
		static enum VCF_INFO {
	#else
		enum VCF_INFO {
	#endif
#endif
	Float:vcf_x,
	Float:vcf_y,
	Float:vcf_z,
	Float:vcf_a,
	vcf_color_1,
	vcf_color_2,
	vcf_respawndelay,
	vcf_addsiren
};

#if !defined VCF_SPAWN_FIX		//quando veiculo é criado, chamar OnVehicleSpawn(vehicleid)
	#define VCF_SPAWN_FIX	true 
#endif

#define  DOOR_FIXED 	_:VCF_DOOR_FIXED
#define  DOOR_OPENED 	_:VCF_DOOR_OPENED
#define  DOOR_DAMAGED 	_:VCF_DOOR_DAMAGED
#define  DOOR_REMOVED 	_:VCF_DOOR_REMOVED

#define  LIGHT_FRONT 	_:VCF_LIGHT_FRONT
#define  LIGHT_BACK 	_:VCF_LIGHT_BACK
#define  LIGHT_RIGHT 	_:VCF_LIGHT_RIGHT
#define  LIGHT_LEFT 	_:VCF_LIGHT_LEFT

enum Light {
	VCF_LIGHT_FRONT,
	VCF_LIGHT_BACK
}

enum LightSide( <<= 2 ) {
	VCF_LIGHT_RIGHT = 1,
	VCF_LIGHT_LEFT
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

static VCFVehicleInfo[MAX_VEHICLES][VCF_INFO];

forward OnVehicleCreate(vehicleid, modelid, Float:x,Float:y,Float:z,Float:rotation,color1,color2,respawn_delay,addsiren);

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

	VCF_lights = VCF_lights ^ (1 << ((4 * _:light) + (_:lightSide >> 1)));

	UpdateVehicleDamageStatus(vehicleid, VCF_panels, VCF_doors, VCF_lights, VCF_tires);
}

stock GetDoorState(vehicleid, Door:door) {

	if(0 > vehicleid > MAX_VEHICLES)
		return false;

	new VCF_panels, VCF_doors, VCF_lights, VCF_tires;
	GetVehicleDamageStatus(vehicleid, VCF_panels, VCF_doors, VCF_lights, VCF_tires);

	return (VCF_doors >>> (8 * _:door));
}

stock SetDoorState(vehicleid, Door:door, doorState) {

	if(0 > vehicleid > MAX_VEHICLES)
		return false;


	if(_:door == _:DOOR_TRUNK && doorState == DOOR_OPENED)
		return false;

	new VCF_panels, VCF_doors, VCF_lights, VCF_tires;
	GetVehicleDamageStatus(vehicleid, VCF_panels, VCF_doors, VCF_lights, VCF_tires);

	new _:VCF_tmp = (_:GetDoorState(vehicleid, door) << (8 * _:door)) ^ VCF_doors;

	VCF_doors |= ((doorState << (8 * _:door)) | _:VCF_tmp);
	
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
}

stock IsPlayerDriver(playerid) {

	if(0 > playerid > MAX_PLAYERS)
		return false;

    return GetPlayerVehicleSeat(playerid) == 0 ? true : false ;
}

stock VCF_CreateVeh(vehicleid, modelid, Float:x,Float:y,Float:z,Float:rotation,color1,color2,respawn_delay,addsiren)
{
	if( 400 <= modelid < 611)
		return 0;

	if( vehicleid == INVALID_VEHICLE_ID)
		return 0;

	#if VCF_SPAWN_FIX
		CallLocalFunction("OnVehicleSpawn", "d", vehicleid);
	#endif

	if(funcidx(#OnVehicleCreate)
		CallLocalFunction("OnVehicleCreate", "ddffffdddd",vehicleid, modelid, x,y,z,rotation,color1,color2,respawn_delay,addsiren);


	VCFVehicleInfo[vehicleid][vcf_x]=x;
	VCFVehicleInfo[vehicleid][vcf_y]=y;
	VCFVehicleInfo[vehicleid][vcf_z]=z;
	VCFVehicleInfo[vehicleid][vcf_a]=rotation;
	VCFVehicleInfo[vehicleid][vcf_color_1]=color1;
	VCFVehicleInfo[vehicleid][vcf_color_2]=color2;
	VCFVehicleInfo[vehicleid][vcf_respawndelay]=respawn_delay;
	VCFVehicleInfo[vehicleid][vcf_addsiren]=addsiren;

	return 1;
}

#if !defined GetVehicleColor
stock GetVehicleColor(vehicleid,&color1,&color2)
{
	if(0 <= vehicleid < MAX_VEHICLES)
		return 0;

	color1= VCFVehicleInfo[vehicleid][vcf_color_1];
	color2= VCFVehicleInfo[vehicleid][vcf_color_2];
	return 1;
}
#endif

stock GetVehiclePrimaryColor(vehicleid)
{	
	if(0 <= vehicleid < MAX_VEHICLES)
		return -1;

	return VCFVehicleInfo[vehicleid][vcf_color_1];
}
stock GetVehicleSecundaryColor(vehicleid)
{
	if(0 <= vehicleid < MAX_VEHICLES)
		return -1;

	return VCFVehicleInfo[vehicleid][vcf_color_2];
}

stock GetVehicleSpawnInfo(vehicleid, modelid, Float:x,Float:y,Float:z,Float:a,color1,color2,rdelay,siren)
{
	if(0 <= vehicleid < MAX_VEHICLES)
		return 0;

	x=		VCFVehicleInfo[vehicleid][vcf_x];
	y=		VCFVehicleInfo[vehicleid][vcf_y];
	z=		VCFVehicleInfo[vehicleid][vcf_z];
	a= 		VCFVehicleInfo[vehicleid][vcf_a];
	color1= VCFVehicleInfo[vehicleid][vcf_color_1];
	color2= VCFVehicleInfo[vehicleid][vcf_color_2];
	rdelay= VCFVehicleInfo[vehicleid][vcf_respawndelay];
	siren= 	VCFVehicleInfo[vehicleid][vcf_addsiren];

	return 1;
}

//-------------------------------------------------------------------------------------------------------------------------------
stock VCF_CreateDynVeh(vehicletype, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay=-1, addsiren=0)
{
	new vehicleid=CreateVehicle(vehicletype, x, y, z, rotation, color1, color2, respawn_delay,addsiren);		
	VCF_CreateVeh(vehicleid, modelid, x,y,z,rotation,color1,color2,respawn_delay,addsiren);
	return vehicleid;		
}

#if defined _ALS_CreateVehicle
  #undef CreateVehicle 
#else 
#define _ALS_CreateVehicle
#endif 

#define CreateVehicle VCF_CreateDynVeh


stock VCF_CreateStaticVeh(vehicletype, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay=-1, addsiren=0)
{
	new vehicleid=AddStaticVehicleEx(vehicletype, x, y, z, rotation, color1, color2, respawn_delay,addsiren);		
	VCF_CreateVeh(vehicleid, modelid, x,y,z,rotation,color1,color2,respawn_delay,addsiren);
	return vehicleid;		
}

#if defined _ALS_AddStaticVehicle
  #undef AddStaticVehicle 
#else 
#define _ALS_AddStaticVehicle
#endif 

#define AddStaticVehicleEx VCF_CreateStaticVeh

#if defined _ALS_AddStaticVehicleEx
  #undef AddStaticVehicleEx
#else 
#define _ALS_AddStaticVehicleEx
#endif 

#define AddStaticVehicleEx VCF_CreateStaticVeh
//-------------------------------------------------------------------------------------------------------------------------------
stock VCF_ChangeVehicleColor(vehicleid, color1, color2)
{
	VCFVehicleInfo[vehicleid][vcf_color_1]=color1;
	VCFVehicleInfo[vehicleid][vcf_color_2]=color2;
	return ChangeVehicleColor(vehicleid, color1, color2);
}

#define ChangeVehicleColor VCF_ChangeVehicleColor

#if defined _ALS_ChangeVehicleColor
  #undef ChangeVehicleColor
#else 
#define _ALS_ChangeVehicleColor
#endif 

#define ChangeVehicleColor VCF_ChangeVehicleColor
//-------------------------------------------------------------------------------------------------------------------------------
