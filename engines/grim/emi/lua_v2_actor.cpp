/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "engines/grim/emi/lua_v2.h"
#include "engines/grim/lua/lua.h"

#include "engines/grim/actor.h"
#include "engines/grim/grim.h"
#include "engines/grim/costume.h"

#include "engines/grim/emi/emi.h"
#include "engines/grim/emi/costumeemi.h"
#include "engines/grim/emi/skeleton.h"
#include "engines/grim/emi/costume/emichore.h"
#include "engines/grim/emi/costume/emiskel_component.h"

namespace Grim {

void Lua_V2::SetActorLocalAlpha() {
	lua_Object actorObj = lua_getparam(1);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);
	if (!actor)
		return;

	// FIXME: implement missing code
	warning("Lua_V2::SetActorLocalAlpha: stub, actor: %s", actor->getName().c_str());
}


void Lua_V2::SetActorGlobalAlpha() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object alphaObj = lua_getparam(2);
//  lua_Object meshObj = lua_getparam(3);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);
	if (!actor)
		return;

	if (!lua_isnumber(alphaObj))
		return;

	float alpha = lua_getnumber(alphaObj);
	if (alpha == Actor::AlphaOff ||
	    alpha == Actor::AlphaReplace ||
	    alpha == Actor::AlphaModulate) {
			actor->setAlphaMode((Actor::AlphaMode) (int) alpha);
	} else {
		actor->setGlobalAlpha(alpha);
	}
}

void Lua_V2::PutActorInOverworld() {
	lua_Object actorObj = lua_getparam(1);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);

	actor->setInOverworld(true);
	actor->playLastWearChore();
}

void Lua_V2::RemoveActorFromOverworld() {
	lua_Object actorObj = lua_getparam(1);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);
	if (!actor)
		return;

	actor->setInOverworld(false);
}

void Lua_V2::UnloadActor() {
	lua_Object actorObj = lua_getparam(1);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);
	if (!actor)
		return;

	g_grim->invalidateActiveActorsList();
	g_grim->immediatelyRemoveActor(actor);
	delete actor;
}

void Lua_V2::SetActorWalkRate() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object rateObj = lua_getparam(2);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;
	if (!lua_isnumber(rateObj))
		return;

	Actor *actor = getactor(actorObj);
	float rate = lua_getnumber(rateObj);
	// const below only differ from grim
	actor->setWalkRate(rate * 3.279999971389771);
}

void Lua_V2::GetActorWalkRate() {
	lua_Object actorObj = lua_getparam(1);
	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);
	// const below only differ from grim
	lua_pushnumber(actor->getWalkRate() * 0.3048780560493469);
}

void Lua_V2::SetActorTurnRate() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object rateObj = lua_getparam(2);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;
	if (!lua_isnumber(rateObj))
		return;

	Actor *actor = getactor(actorObj);
	float rate = lua_getnumber(rateObj); // FIXME verify negate values of rate

	// special handling of value 1 only used for voodoo chair
	actor->setTurnRate((rate == 1) ? 100 : rate);
}

void Lua_V2::LockChoreSet() {
	lua_Object choreObj = lua_getparam(1);

	const char *choreName = lua_getstring(choreObj);
	warning("Lua_V2::LockChoreSet: chore: %s", choreName);
}

void Lua_V2::UnlockChoreSet() {
	lua_Object choreObj = lua_getparam(1);

	const char *choreName = lua_getstring(choreObj);
	warning("Lua_V2::UnlockChoreSet: chore: %s", choreName);
}

void Lua_V2::LockChore() {
	lua_Object nameObj = lua_getparam(1);
	lua_Object filenameObj = lua_getparam(2);

	if (!lua_isstring(nameObj) || !lua_isstring(filenameObj)) {
		lua_pushnil();
		return;
	}

	const char *name = lua_getstring(nameObj);
	const char *filename = lua_getstring(filenameObj);
	warning("Lua_V2::LockChore, name: %s, filename: %s", name, filename);
	// FIXME: implement missing rest part of code
}

void Lua_V2::UnlockChore() {
	lua_Object nameObj = lua_getparam(1);
	lua_Object filenameObj = lua_getparam(2);

	if (!lua_isstring(nameObj) || !lua_isstring(filenameObj)) {
		lua_pushnil();
		return;
	}

	const char *name = lua_getstring(nameObj);
	const char *filename = lua_getstring(filenameObj);
	warning("Lua_V2::UnlockChore, name: %s, filename: %s", name, filename);
	// FIXME: implement missing rest part of code
}

void Lua_V2::IsActorChoring() {
	lua_Object actorObj = lua_getparam(1);
	bool excludeLoop = getbool(2);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);
	Costume *costume = actor->getCurrentCostume();

	if (!costume) {
		lua_pushnil();
		return;
	}

	for (int i = 0; i < costume->getNumChores(); i++) {
		int chore = costume->isChoring(i, excludeLoop);
		if (chore != -1) {
			// Ignore talk chores.
			bool isTalk = false;
			for (int j = 0; j < 10; j++) {
				if (costume == actor->getTalkCostume(j) && actor->getTalkChore(j) == chore) {
					isTalk = true;
					break;
				}
			}
			if (isTalk)
				continue;

			lua_pushnumber(chore);
			
			pushbool(true);
			return;
		}
	}

	lua_pushnil();
}

void Lua_V2::IsChoreValid() {
	lua_Object choreObj = lua_getparam(1);

	if (!lua_isuserdata(choreObj) || lua_tag(choreObj) != MKTAG('C','H','O','R'))
		return;

	int chore = lua_getuserdata(choreObj);
	Chore *c = EMIChore::getPool().getObject(chore);

	if (c) {
		pushbool(c != nullptr);
	} else {
		lua_pushnil();
	}
}

void Lua_V2::IsChorePlaying() {
	lua_Object choreObj = lua_getparam(1);

	if (!lua_isuserdata(choreObj) || lua_tag(choreObj) != MKTAG('C','H','O','R'))
		return;

	int chore = lua_getuserdata(choreObj);
	Chore *c = EMIChore::getPool().getObject(chore);

	if (c) {
		pushbool(c->isPlaying());
	} else {
		lua_pushnil();
	}
}

void Lua_V2::IsChoreLooping() {
	lua_Object choreObj = lua_getparam(1);

	if (!lua_isuserdata(choreObj) || lua_tag(choreObj) != MKTAG('C','H','O','R'))
		return;

	int chore = lua_getuserdata(choreObj);
	Chore *c = EMIChore::getPool().getObject(chore);

	if (c) {
		pushbool(c->isLooping());
	} else {
		lua_pushnil();
	}
}

void Lua_V2::SetChoreLooping() {
	lua_Object choreObj = lua_getparam(1);
	if (!lua_isuserdata(choreObj) || lua_tag(choreObj) != MKTAG('C','H','O','R'))
		return;

	int chore = lua_getuserdata(choreObj);
	Chore *c = EMIChore::getPool().getObject(chore);

	if (c) {
		c->setLooping(false);
	}
	lua_pushnil();
}

void Lua_V2::PlayChore() {
	lua_Object choreObj = lua_getparam(1);

	if (!lua_isuserdata(choreObj) || lua_tag(choreObj) != MKTAG('C','H','O','R'))
		return;
	int chore = lua_getuserdata(choreObj);

	Chore *c = EMIChore::getPool().getObject(chore);
	if (c) {
		c->setPaused(false);
	}
}

void Lua_V2::PauseChore() {
	lua_Object choreObj = lua_getparam(1);

	if (!lua_isuserdata(choreObj) || lua_tag(choreObj) != MKTAG('C','H','O','R'))
		return;
	int chore = lua_getuserdata(choreObj);

	Chore *c = EMIChore::getPool().getObject(chore);
	if (c) {
		c->setPaused(true);
	}
}

void Lua_V2::StopChore() {
	lua_Object choreObj = lua_getparam(1);
	lua_Object fadeTimeObj = lua_getparam(2);

	if (!lua_isuserdata(choreObj) || lua_tag(choreObj) != MKTAG('C','H','O','R'))
		return;

	int chore = lua_getuserdata(choreObj);
	float fadeTime = 0.0f;

	if (!lua_isnil(fadeTimeObj)) {
		if (lua_isnumber(fadeTimeObj))
			fadeTime = lua_getnumber(fadeTimeObj);
	}

	Chore *c = EMIChore::getPool().getObject(chore);
	if (c) {
		c->stop((int)(fadeTime * 1000));
	}
}

void Lua_V2::AdvanceChore() {
	lua_Object choreObj = lua_getparam(1);
	lua_Object timeObj = lua_getparam(2);

	if (!lua_isuserdata(choreObj) || lua_tag(choreObj) != MKTAG('C','H','O','R') || !lua_isnumber(timeObj))
		return;

	int chore = lua_getuserdata(choreObj);
	float time = lua_getnumber(timeObj);
	Chore *c = EMIChore::getPool().getObject(chore);
	if (c) {
		if (!c->isPlaying()) {
			warning("AdvanceChore() called on stopped chore %s (%s)",
					c->getName(), c->getOwner()->getFilename().c_str());
			if (c->isLooping()) {
				c->getOwner()->playChoreLooping(c->getName());
			} else {
				c->getOwner()->playChore(c->getName());
			}
		}
		c->advance(time * 1000);
	}
}

// TODO: Implement, verify, and rename parameters
void Lua_V2::CompleteChore() {
	lua_Object param1 = lua_getparam(1);
	lua_Object param2 = lua_getparam(2);

	if (!lua_isuserdata(param1) || !lua_isnumber(param2))
		error("Lua_V2::CompleteChore - Unknown params");

	// Guesswork based on StopChore:
	int chore = lua_getuserdata(param1);
	float time = lua_getnumber(param2);

	error("Lua_V2::CompleteChore(%d, %f) - TODO: Implement opcode", chore, time);
}

void Lua_V2::SetActorSortOrder() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object orderObj = lua_getparam(2);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	if (!lua_isnumber(orderObj))
		return;

	Actor *actor = getactor(actorObj);
	int order = (int)lua_getnumber(orderObj);
	actor->setSortOrder(order);

	g_emi->invalidateSortOrder();
}

void Lua_V2::GetActorSortOrder() {
	lua_Object actorObj = lua_getparam(1);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);
	lua_pushnumber(actor->getSortOrder());
}

void Lua_V2::ActorActivateShadow() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object qualityObj = lua_getparam(2);
	lua_Object planeObj = lua_getparam(3);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);
	if (!actor)
		return;
	int quality = (int)lua_getnumber(qualityObj);
	const char *plane = "NULL";
	if (lua_isstring(planeObj))
		plane = lua_getstring(planeObj);
	warning("Lua_V2::ActorActivateShadow, actor: %s, aquality: %d, plane: %s", actor->getName().c_str(), quality, plane);
	actor->activateShadow(quality);
}

void Lua_V2::ActorStopMoving() {
	lua_Object actorObj = lua_getparam(1);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);

	actor->stopWalking();
	// FIXME: Also stop turning?

	warning("Lua_V2::ActorStopMoving, actor: %s", actor->getName().c_str());
	// FIXME: Inspect the rest of the code to see if there's anything else missing
}

void Lua_V2::GetActorWorldPos() {
	lua_Object actorObj = lua_getparam(1);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);
	if (!actor)
		return;

	Math::Vector3d pos = actor->getWorldPos();
	lua_pushnumber(pos.x());
	lua_pushnumber(pos.y());
	lua_pushnumber(pos.z());
}

void Lua_V2::PutActorInSet() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object setObj = lua_getparam(2);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);

	if (!lua_isstring(setObj) && !lua_isnil(setObj)) {
		lua_pushnil();
		return;
	}

	const char *set = lua_getstring(setObj);

	// FIXME verify adding actor to set
	if (!set) {
		actor->putInSet("");
		lua_pushnil();
	} else {
		if (!actor->isInSet(set)) {
			actor->putInSet(set);
			actor->playLastWearChore();
		}
		lua_pushnumber(1.0);
	}
}

void Lua_V2::SetActorRestChore() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object choreObj = lua_getparam(2);
	lua_Object costumeObj = lua_getparam(3);
	Costume *costume = nullptr;
	int chore;

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R') ||
			(!lua_isstring(choreObj) && !lua_isnil(choreObj))) {
		return;
	}

	Actor *actor = getactor(actorObj);

	setChoreAndCostume(choreObj, costumeObj, actor, costume, chore);

	actor->setRestChore(chore, costume);
}

void Lua_V2::SetActorWalkChore() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object choreObj = lua_getparam(2);
	lua_Object costumeObj = lua_getparam(3);
	Costume *costume = nullptr;
	int chore;

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R') ||
			(!lua_isstring(choreObj) && !lua_isnil(choreObj))) {
		return;
	}

	Actor *actor = getactor(actorObj);

	setChoreAndCostume(choreObj, costumeObj, actor, costume, chore);

	actor->setWalkChore(chore, costume);
}

void Lua_V2::SetActorTurnChores() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object leftChoreObj = lua_getparam(2);
	lua_Object rightChoreObj = lua_getparam(3);
	lua_Object costumeObj = lua_getparam(4);
	Costume *costume;

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R')) {
		return;
	} else if (!lua_isnil(leftChoreObj) && !lua_isstring(leftChoreObj)) {
		return;
	} else if (!lua_isnil(rightChoreObj) && !lua_isstring(rightChoreObj)) {
		return;
	}

	Actor *actor = getactor(actorObj);

	if (!findCostume(costumeObj, actor, &costume))
		return;

	if (!costume) {
		costume = actor->getCurrentCostume();
	}

	int leftChore = costume->getChoreId(lua_getstring(leftChoreObj));
	int rightChore = costume->getChoreId(lua_getstring(rightChoreObj));

	actor->setTurnChores(leftChore, rightChore, costume);
}



void Lua_V2::SetActorTalkChore() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object indexObj = lua_getparam(2);
	lua_Object choreObj = lua_getparam(3);
	lua_Object costumeObj = lua_getparam(4);
	Costume *costume = nullptr;
	int chore;

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R') ||
			!lua_isnumber(indexObj) ||
			(!lua_isstring(choreObj) && !lua_isnil(choreObj))) {
		return;
	}

	int index = (int)lua_getnumber(indexObj);
	if (index < 0 || index >= 16)
		return;

	Actor *actor = getactor(actorObj);

	setChoreAndCostume(choreObj, costumeObj, actor, costume, chore);

	actor->setTalkChore(index + 1, chore, costume);
}

void Lua_V2::SetActorMumblechore() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object choreObj = lua_getparam(2);
	lua_Object costumeObj = lua_getparam(3);
	Costume *costume = nullptr;
	int chore;

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R') ||
			(!lua_isstring(choreObj) && !lua_isnil(choreObj))) {
		return;
	}

	Actor *actor = getactor(actorObj);

	setChoreAndCostume(choreObj, costumeObj, actor, costume, chore);

	actor->setMumbleChore(chore, costume);
}

void Lua_V2::GetActorChores() {
	lua_Object actorObj = lua_getparam(1);
	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;
	Actor *actor = getactor(actorObj);
	Costume *costume = actor->getCurrentCostume();

	lua_Object result = lua_createtable();
	lua_pushobject(result);

	if (!costume) {
		lua_pushstring("count");
		lua_pushnumber(0);
		lua_settable();
		lua_pushobject(result);
		return;
	}

	int num = costume->getNumChores();

	lua_pushstring("count");
	lua_pushnumber(num);
	lua_settable();

	for (int i = 0; i < num; ++i) {
		lua_pushobject(result);
		lua_pushnumber(i);
		lua_pushusertag(((EMIChore *)costume->getChore(i))->getId(), MKTAG('C','H','O','R'));
		lua_settable();
	}

	lua_pushobject(result);
}

bool Lua_V2::findCostume(lua_Object costumeObj, Actor *actor, Costume **costume) {
	*costume = nullptr;
	if (lua_isnil(costumeObj))
		return true;
	if (lua_isstring(costumeObj)) {
		const char *costumeName = lua_getstring(costumeObj);
		*costume = actor->findCostume(costumeName);
		if (*costume == nullptr) {
			actor->pushCostume(costumeName);
			*costume = actor->findCostume(costumeName);
		}
	}
	return (*costume != nullptr);
}

void Lua_V2::PlayActorChore() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object choreObj = lua_getparam(2);
	lua_Object costumeObj = lua_getparam(3);
	lua_Object modeObj = lua_getparam(4);
	lua_Object fadeTimeObj = lua_getparam(5);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);

	if (!lua_isstring(choreObj) || !lua_isstring(costumeObj))
		lua_pushnil();

	bool mode = false;
	float fadeTime = 0.0f;

	if (!lua_isnil(modeObj)) {
		if (lua_getnumber(modeObj) != 0.0)
			mode = true;
	}

	if (!lua_isnil(fadeTimeObj)) {
		if (lua_isnumber(fadeTimeObj))
			fadeTime = lua_getnumber(fadeTimeObj);
	}

	const char *choreName = lua_getstring(choreObj);

	const char *costumeName = lua_getstring(costumeObj);
	Costume *costume;
	// If a new wear chore is set and it uses a different costume than the
	// current one and neither of them is the shadow costume stop all active
	// chores and remove the old costume before setting the new one.
	//
	// This is necessary, because always the last costume on the stack, even
	// if it is not active, is returned by getCurrentCostume(). This would
	// cause an issue if the costumes would have different joints and the lua
	// code would consider a different costume active than the C code.
	if (0 == strncmp("wear_", choreName, 5)) {
		if (0 != strncmp("fx/dumbshadow.cos", costumeName, 17)) {
			if (actor->getCurrentCostume() != nullptr &&
			    actor->getCurrentCostume()->getFilename() != "fx/dumbshadow.cos" &&
			    actor->getCurrentCostume()->getFilename().compareToIgnoreCase(costumeName) != 0) {
				actor->stopAllChores();
				actor->setRestChore(-1, nullptr);
				actor->setWalkChore(-1, nullptr);
				actor->setTurnChores(-1, -1, nullptr);
				actor->setMumbleChore(-1, nullptr);
				actor->popCostume();
			}
		}
	}
	if (!findCostume(costumeObj, actor, &costume))
		return;

	EMIChore *chore = (EMIChore *)costume->getChore(choreName);
	if (0 == strncmp("wear_", choreName, 5)) {
		EMICostume *emiCostume = static_cast<EMICostume *>(costume);
		emiCostume->setWearChoreActive(true);
		actor->setLastWearChore(costume->getChoreId(choreName), costume);
	}

	if (mode) {
		costume->playChoreLooping(choreName, (int)(fadeTime * 1000));
	} else {
		costume->playChore(choreName, (int)(fadeTime * 1000));
	}
	if (chore) {
		lua_pushusertag(chore->getId(), MKTAG('C','H','O','R'));
	} else {
		lua_pushnil();
	}

}

void Lua_V2::StopActorChores() {
	lua_Object actorObj = lua_getparam(1);
	// Guesswork for boolean parameter
	bool ignoreLoopingChores = getbool(2);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);
	if (!actor)
		return;

	actor->stopAllChores(ignoreLoopingChores);

	// Reset the wearChore as well
	EMICostume *cost = static_cast<EMICostume *>(actor->getCurrentCostume());
	if (cost != nullptr)
		cost->setWearChoreActive(false);
}

void Lua_V2::SetActorLighting() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object lightModeObj = lua_getparam(2);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);
	if (!actor)
		return;

	if (lua_isnil(lightModeObj) || !lua_isnumber(lightModeObj))
		return;

	int lightMode = (int)lua_getnumber(lightModeObj);
	actor->setLightMode((Actor::LightMode)lightMode);
}

void Lua_V2::SetActorCollisionMode() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object modeObj = lua_getparam(2);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);
	assert(actor);
	int mode = (int)lua_getnumber(modeObj);

	Actor::CollisionMode m;
	switch (mode) {
		case Actor::CollisionOff:
			m = Actor::CollisionOff;
			break;
		case Actor::CollisionBox:
			m = Actor::CollisionBox;
			break;
		case Actor::CollisionSphere:
			m = Actor::CollisionSphere;
			break;
		default:
			warning("Lua_V2::SetActorCollisionMode(): wrong collisionmode: %d, using default 0", mode);
			m = Actor::CollisionOff;
	}
	actor->setCollisionMode(m);
}

void Lua_V2::SetActorCollisionScale() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object scaleObj = lua_getparam(2);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);
	assert(actor);

	float scale = lua_getnumber(scaleObj);
	actor->setCollisionScale(scale);
}

void Lua_V2::GetActorPuckVector() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object addObj = lua_getparam(2);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R')) {
		lua_pushnil();
		return;
	}

	Actor *actor = getactor(actorObj);
	// Note: The wear chore of dumbshadow.cos is only started from Lua if
	// GetActorPuckVector returns a non-nil value. The original engine seems
	// to return nil for all actors that have never followed walkboxes.
	if (!actor || !actor->hasFollowedBoxes()) {
		lua_pushnil();
		return;
	}

	Math::Vector3d result = actor->getPuckVector();
	if (!lua_isnil(addObj))
		result += actor->getPos();

	lua_pushnumber(result.x());
	lua_pushnumber(result.y());
	lua_pushnumber(result.z());
}

void Lua_V2::SetActorHeadLimits() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object param2Obj = lua_getparam(2);
	lua_Object param3Obj = lua_getparam(3);
	lua_Object param4Obj = lua_getparam(4);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);
	if (!actor)
		return;

	if (lua_isnumber(param2Obj) && lua_isnumber(param3Obj) && lua_isnumber(param4Obj)) {
		float param2 = lua_getnumber(param2Obj); // belows needs multiply by some runtime value
		float param3 = lua_getnumber(param3Obj);
		float param4 = lua_getnumber(param4Obj);
		// FIXME: implement missing func
		//actor->func(param2, param3, param4);
		warning("Lua_V2::SetActorHeadLimits: implement opcode. actor: %s, params: %f, %f, %f", actor->getName().c_str(), param2, param3, param4);
	}
}

void Lua_V2::SetActorHead() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object jointObj = lua_getparam(2);
	lua_Object xObj = lua_getparam(3);
	lua_Object yObj = lua_getparam(4);
	lua_Object zObj = lua_getparam(5);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A', 'C', 'T', 'R'))
		return;

	Actor *actor = getactor(actorObj);
	if (!actor)
		return;

	if (lua_isstring(jointObj) && lua_isnumber(xObj) && lua_isnumber(yObj) && lua_isnumber(zObj)) {
		const char *joint = lua_getstring(jointObj);
		Math::Vector3d offset;
		offset.x() = lua_getnumber(xObj);
		offset.y() = lua_getnumber(yObj);
		offset.z() = lua_getnumber(zObj);
		actor->setHead(joint, offset);
	}
}

void Lua_V2::SetActorFOV() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object fovObj = lua_getparam(2);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);
	if (!actor)
		return;

	if (lua_isnumber(fovObj)) {
		float fov = lua_getnumber(fovObj);
		// FIXME: implement missing code
		//actor->func(fov); // cos(fov * some tuntime val * 0.5)
		warning("Lua_V2::SetActorFOV: implement opcode. actor: %s, param: %f", actor->getName().c_str(), fov);
	}
}

void Lua_V2::AttachActor() {
	// Missing lua parts
	lua_Object attachedObj = lua_getparam(1);
	lua_Object actorObj = lua_getparam(2);
	lua_Object jointObj = lua_getparam(3);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A', 'C', 'T', 'R'))
		return;

	Actor *actor = getactor(actorObj);
	if (!actor)
		return;

	if (!lua_isuserdata(attachedObj) || lua_tag(attachedObj) != MKTAG('A', 'C', 'T', 'R'))
		return;

	Actor *attached = getactor(attachedObj);
	if (!attached)
		return;

	const char *joint = nullptr;
	if (!lua_isnil(jointObj)) {
		joint = lua_getstring(jointObj);
	}

	attached->attachToActor(actor, joint);
	warning("Lua_V2::AttachActor: attaching %s to %s (on %s)", attached->getName().c_str(), actor->getName().c_str(), joint ? joint : "(none)");

	g_emi->invalidateSortOrder();
}

void Lua_V2::DetachActor() {
	// Missing lua parts
	lua_Object attachedObj = lua_getparam(1);

	if (!lua_isuserdata(attachedObj) || lua_tag(attachedObj) != MKTAG('A','C','T','R'))
		return;

	Actor *attached = getactor(attachedObj);
	if (!attached)
		return;

	warning("Lua_V2::DetachActor: detaching %s from parent actor", attached->getName().c_str());
	attached->detach();

	g_emi->invalidateSortOrder();
}

void Lua_V2::WalkActorToAvoiding() {
	lua_Object actorObj = lua_getparam(1);
	lua_Object actor2Obj = lua_getparam(2);
	lua_Object xObj = lua_getparam(3);
	lua_Object yObj = lua_getparam(4);
	lua_Object zObj = lua_getparam(5);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	if (!lua_isuserdata(actor2Obj) || lua_tag(actor2Obj) != MKTAG('A','C','T','R'))
		return;

	Math::Vector3d destVec;
	Actor *actor = getactor(actorObj);
	if (!lua_isnumber(xObj)) {
		if (!lua_isuserdata(xObj) || lua_tag(xObj) != MKTAG('A','C','T','R'))
			return;
		Actor *destActor = getactor(xObj);
		destVec = destActor->getPos();
	} else {
		float x = lua_getnumber(xObj);
		float y = lua_getnumber(yObj);
		float z = lua_getnumber(zObj);
		destVec.set(x, y, z);
	}

	// TODO: Make this actually avoid the second actor

	actor->walkTo(destVec);
}

void Lua_V2::EnableActorPuck() {
	lua_Object actorObj = lua_getparam(1);

	if (!lua_isuserdata(actorObj) || lua_tag(actorObj) != MKTAG('A','C','T','R'))
		return;

	Actor *actor = getactor(actorObj);
	if (!actor)
		return;

	bool enable = getbool(2);

	// FIXME: Implement.
	warning("Lua_V2::EnableActorPuck: stub, actor: %s enable: %s", actor->getName().c_str(), enable ? "TRUE" : "FALSE");
}

void Lua_V2::setChoreAndCostume(lua_Object choreObj, lua_Object costumeObj, Actor *actor, Costume *&costume, int &chore) {
	if (lua_isnil(choreObj)) {
		chore = -1;
	} else {
		if (!findCostume(costumeObj, actor, &costume))
			return;

		if (!costume) {
			costume = actor->getCurrentCostume();
		}

		const char *choreStr = lua_getstring(choreObj);
		chore = costume->getChoreId(choreStr);
	}
}

} // end of namespace Grim
