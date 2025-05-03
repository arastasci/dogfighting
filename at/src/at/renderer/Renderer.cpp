#include "Renderer.h"

namespace at
{
	

	void Renderer::Init()
	{
	#ifdef AT_DEBUG
			//glEnable(GL_DEBUG_OUTPUT);
			//glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	#endif

			glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_LINE_SMOOTH);


	}


	void Renderer::SetViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	void Renderer::SetPointLight(short id, const PointLight& light, std::shared_ptr<Shader> shader)
	{
		shader->use();
		std::string name = "pointLights[" + std::to_string(id) + "].";
		shader->setVec3(name + "position", light.position);

		shader->setVec3(name + "ambient", light.ambient);
		shader->setVec3(name + "specular", light.specular);
		shader->setVec3(name + "diffuse", light.diffuse);

		shader->setFloat(name + "constant", light.constant);
		shader->setFloat(name + "linear", light.linear);
		shader->setFloat(name + "quadratic", light.quadratic);
	}

	void Renderer::DrawElements(std::shared_ptr<VertexArray>& vertexArray, std::vector<Texture>& textures, std::shared_ptr<Shader> shader)
	{
		
		shader->use();
		vertexArray->Bind();

		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			std::string number;
			std::string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++); // transfer unsigned int to string
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to string
			else if (name == "texture_height")
				number = std::to_string(heightNr++); // transfer unsigned int to string

			// now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), i);
			// and finally bind the texture
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		uint32_t count = vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);


	}
}
