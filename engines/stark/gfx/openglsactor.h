/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
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

#ifndef STARK_GFX_OPENGL_S_ACTOR_H
#define STARK_GFX_OPENGL_S_ACTOR_H

#include "common/hashmap.h"
#include "common/hash-ptr.h"

#include "engines/stark/gfx/renderentry.h"
#include "engines/stark/visual/actor.h"

namespace OpenGL {
	class Shader;
}

namespace Stark {
namespace Gfx {

class OpenGLSDriver;

class OpenGLSActorRenderer : public VisualActor {
public:
	OpenGLSActorRenderer(OpenGLSDriver *gfx);
	virtual ~OpenGLSActorRenderer();

	void render(const Math::Vector3d &position, float direction, const LightEntryArray &lights) override;

protected:
	typedef Common::HashMap<Face *, uint32> FaceBufferMap;

	OpenGLSDriver *_gfx;
	OpenGL::Shader *_shader, *_shadowShader;

	uint32 _faceVBO;
	FaceBufferMap _faceEBO;

	void clearVertices();
	void uploadVertices();
	uint32 createModelVBO(const Model *model);
	uint32 createFaceEBO(const Face *face);
	void setBonePositionArrayUniform(OpenGL::Shader *shader, const char *uniform);
	void setBoneRotationArrayUniform(OpenGL::Shader *shader, const char *uniform);
	void setLightArrayUniform(const LightEntryArray &lights);

	void setShadowUniform(const LightEntryArray &lights, const Math::Vector3d &actorPosition, Math::Matrix3 worldToModelRot);
	
	bool getPointLightContribution(LightEntry *light, const Math::Vector3d &actorPosition,
			Math::Vector3d &direction, float weight = 1.0f);
	bool getDirectionalLightContribution(LightEntry *light, const Math::Vector3d &actorPosition, Math::Vector3d &direction);
	bool getSpotLightContribution(LightEntry *light, const Math::Vector3d &actorPosition, Math::Vector3d &direction);
};

} // End of namespace Gfx
} // End of namespace Stark

#endif // STARK_GFX_OPENGL_S_ACTOR_H
