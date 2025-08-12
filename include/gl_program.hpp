#ifndef MDIE_GL_PROGRAM_H
#define MDIE_GL_PROGRAM_H

#include <error.hpp>
#include <glad/glad.h>
#include <unordered_map>
#include <vector>

namespace mdie {
mdie::Result compile_shader(GLuint *shader, GLenum type, const char *src);
mdie::Result create_program(GLuint *program, const std::vector<std::pair<const char *, GLenum>> &shaders);

class GLprogram {
public:
	GLprogram() {}
	GLprogram(const std::vector<std::pair<const char *, GLenum>> &shaders) {
		if (create(shaders) != mdie::Result::SUCCESS) { throw std::runtime_error("Failed to create program"); }
	}
	~GLprogram() { destroy(); }

	mdie::Result create(const std::vector<std::pair<const char *, GLenum>> &shaders);
	void prepopulate_cache();
	void destroy();

	GLuint handle() const { return m_handle; }
	GLint get_uniform(const std::string &vName);
	GLuint get_uniform_block(const std::string &vName);

	void bind() const { glUseProgram(m_handle); }
	static void unbind() { glUseProgram(0); }

private:
	GLuint m_handle = 0;

	std::unordered_map<std::string, GLint> m_uniformVariables;
	std::unordered_map<std::string, GLuint> m_uniformBlocks;
};
}// namespace mdie

#endif// !MDIE_GL_PROGRAM_H
