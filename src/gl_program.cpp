#include "gl_program.hpp"
#include <fstream>

mdie::Result mdie::compile_shader(GLuint *shader, GLenum type, const char *src) {
	*shader = glCreateShader(type);
	glShaderSource(*shader, 1, &src, nullptr);
	glCompileShader(*shader);

	GLint res = 0;
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &res);
	if (!res) {
		GLint logLength = 0;
		glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
		char *log = new char[logLength];
		glGetShaderInfoLog(*shader, logLength, nullptr, log);
		MDIE_LOG_ERROR(log, mdie::Result::GL_ERROR);
		delete[] log;
		return mdie::Result::GL_ERROR;
	}
	return mdie::Result::SUCCESS;
}

mdie::Result mdie::create_program(GLuint *program, const std::vector<std::pair<const char *, GLenum>> &shaders) {
	*program = glCreateProgram();
	std::vector<GLuint> shaderIDs(shaders.size());
	for (size_t i = 0; i < shaders.size(); i++) {
		if (compile_shader(&shaderIDs[i], shaders[i].second, shaders[i].first) != mdie::Result::SUCCESS) {
			MDIE_LOG_ERROR("Failed to compile shader", mdie::Result::GL_ERROR);
			return mdie::Result::GL_ERROR;
		}
		glAttachShader(*program, shaderIDs[i]);
	}

	glLinkProgram(*program);
	GLint res = 0;
	glGetProgramiv(*program, GL_LINK_STATUS, &res);
	if (!res) {
		GLint logLength = 0;
		glGetProgramiv(*program, GL_INFO_LOG_LENGTH, &logLength);
		char *log = new char[logLength];
		glGetProgramInfoLog(*program, logLength, nullptr, log);
		MDIE_LOG_ERROR(log, mdie::Result::GL_ERROR);
		delete[] log;
		return mdie::Result::GL_ERROR;
	}

	for (auto &shader : shaderIDs) { glDeleteShader(shader); }
	return mdie::Result::SUCCESS;
}

mdie::Result mdie::GLprogram::create(const std::vector<std::pair<const char *, GLenum>> &shaders) {
	mdie::Result res = create_program(&m_handle, shaders);
	if (res != mdie::Result::SUCCESS) { return res; }

	GLint param = 0;
	glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &param);
	m_uniformVariables.reserve(param);

	param = 0;
	glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_BLOCKS, &param);
	m_uniformBlocks.reserve(param);

	return res;
}

void mdie::GLprogram::prepopulate_cache() {
	GLint param = 0;
	char name[256];
	GLsizei len;

	glGetProgramInterfaceiv(m_handle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &param);
	for (size_t i = 0; i < param; i++) {
		glGetProgramResourceName(m_handle, GL_UNIFORM, i, sizeof(name), &len, name);
		GLint v = glGetUniformLocation(m_handle, name);
		m_uniformVariables.emplace(name, v);
	}

	glGetProgramInterfaceiv(m_handle, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &param);
	for (size_t i = 0; i < param; i++) {
		glGetProgramResourceName(m_handle, GL_UNIFORM_BLOCK, i, sizeof(name), &len, name);
		GLuint v = glGetUniformBlockIndex(m_handle, name);
		m_uniformBlocks.emplace(name, v);
	}
}

void mdie::GLprogram::destroy() {
	if (m_handle != 0) {
		glDeleteProgram(m_handle);
		m_handle = 0;
	}

	m_uniformVariables.clear();
	m_uniformBlocks.clear();
}

GLint mdie::GLprogram::get_uniform(const std::string &vName) {
	auto i = m_uniformVariables.find(vName);
	if (i != m_uniformVariables.end()) { return i->second; }

	GLint var = glGetUniformLocation(m_handle, vName.c_str());
	m_uniformVariables.try_emplace(vName, var);
	if (var == -1) { MDIE_WARN("Could not find uniform variable " + vName); }
	return var;
}

GLuint mdie::GLprogram::get_uniform_block(const std::string &vName) {
	auto i = m_uniformBlocks.find(vName);
	if (i != m_uniformBlocks.end()) { return i->second; }

	GLuint var = glGetUniformBlockIndex(m_handle, vName.c_str());
	m_uniformBlocks.try_emplace(vName, var);
	if (var == GL_INVALID_INDEX) { MDIE_WARN("Could not find uniform block " + vName); }
	return var;
}
