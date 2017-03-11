#include "Models.h"
#include "wrappers/priwrapper.h"
void VectorData::FromWrapper(Vector v)
{
	data = v;
}

void RotatorData::FromWrapper(Rotator r)
{
	data = r;
}

void ActorData::FromWrapper(ActorWrapper cw)
{
	location.FromWrapper(cw.GetLocation());
	velocity.FromWrapper(cw.GetVelocity());
	rotation.FromWrapper(cw.GetRotation());
}

void CarData::FromWrapper(CarWrapper cw)
{
	ActorData::FromWrapper(cw);
	boostAmount = cw.GetBoost().GetCurrentBoostAmount();
	playerIdx = cw.GetPRI().GetPlayerID();
}
